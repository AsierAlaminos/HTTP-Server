#include "../include/server.h"
#include <string.h>

typedef struct MimeType {
	char *extension;
	char *mime_type;
} MimeType;

void send_response(int client_fd, char *response, int response_len) {
	if (send(client_fd, response, response_len, 0) < 0) {
		perror("[!] Error al enviar datos al cliente");
	} else {
		printf("[*] Respuesta enviada al cliente\n");
	}
}

char *handle_type(char *file_route) {
	MimeType mime_types[] = {
		{".html", "text/html"},
		{".css", "text/css"},
		{".js", "application/javascript"},
		{".png", "image/png"},
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".gif", "image/gif"},
		{".svg", "image/svg+xml"},
		{".json", "application/json"},
		{".txt", "text/plain"},
		{".xml", "application/xml"}
	};

	char *extension;
	
	extension = strrchr(file_route, '.');

	if (extension == NULL)
		return ("application/octet-stream");

	for (size_t i = 0; i < sizeof(mime_types) /sizeof(mime_types[0]); i++) {
		if (!strcmp(extension, mime_types[i].extension)) {
			return (mime_types[i].mime_type);
		}
	}

	return ("application/octet-stream");
}

void *handle_request(void *arg) {
	char *response;
	char *status_line;
	char *content_type;
	int headers_len;
	int response_len = 0;
	int client_fd = *((int *)arg);

	char *client_request = read_request(client_fd);
	if (client_request == NULL) {
		response = predefinied_content(create_status_header(500), get_status_code(500));
		printf("[*] Response: %s\n", response);
		send_response(client_fd, response, strlen(response));
		close(client_fd);
		free(response);
		return (NULL);
	}

	printf("[*] Request:\n%s\n", client_request);

	char *file_route = get_route(client_request);

	if (file_route == NULL || !strcmp(file_route, "/")) {
		file_route = "/index.html";
	}

	char *decoded = url_decode(file_route);
	int dt = directory_traversal(decoded, strlen(decoded));
	printf("[*] decoded: %s / dt: %d\n", decoded, dt);

	if (!exist_file(decoded)) {
		response = predefinied_content(create_status_header(404), get_status_code(400));
		response_len = strlen(response);
	} else if(directory_traversal(decoded, strlen(decoded)) == -1) {
		response = predefinied_content(create_status_header(403), get_status_code(403));
		response_len = strlen(response);
	} else {
		status_line = create_status_header(200);
		content_type = handle_type(decoded);
		response = create_header(status_line, content_type, &headers_len);
		response = read_file(decoded, content_type, response, headers_len, &response_len);
		free(status_line);
	}

	printf("[*] Response: %s\n", response);

	send_response(client_fd, response, response_len);

	close(client_fd);
	free(decoded);
	free(response);
	free(client_request);
	printf("[*] Conexión cerrada\n");

	return (NULL);
}

//TODO
//Gestionar el resto de codigos de estado
//1XX
// 100, 101
//2XX
// 200, 201, 204
//3XX
// 301, 302, 304
//4XX
// 400, 401, 403, 404, 405, 408, 429
//5XX
// 500, 501, 502, 503, 504
