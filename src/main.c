#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../include/server.h"

void error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

char *read_response(int client_fd) {
	char buffer[1024] = "";
	memset(buffer, 0, sizeof(buffer));
	int bytes_received;
	char *response = NULL;
	int response_lenght = 0;


	while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytes_received] = '\0';
		if (response == NULL) {
			response = malloc(bytes_received + 1);
			if (response == NULL) {
				perror("[!] Error malloc");
				return NULL;
			}
			memcpy(response, buffer, bytes_received);
			response_lenght = bytes_received;
		} else {
			response = realloc(response, response_lenght + bytes_received + 1);
			if (response == NULL) {
				perror("[!] Error malloc");
				return NULL;
			}
			memcpy(response + response_lenght, buffer, bytes_received);
			response_lenght += bytes_received;
		}

		if (strstr(buffer, "\r\n\r\n") != NULL) {
			break;
		}
	}

	if (bytes_received == 0) {
		perror("[!] Error al leer datos del cliente");
	}

	if (response != NULL) {
		response[response_lenght] = '\0';
	}

	return response;
}

char *get_route(char *request) {

	char *request_splitted = strtok(request, "\n");
	
	strtok(request_splitted, " ");
	char *path = strtok(NULL, " ");

	return path;
}

int main() {
	int socket_fd;
	struct sockaddr_in server_addr;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0) {
		error("socket failed");
	}
	printf("[*] Socket creado con exito\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		error("socket failed");
	}
	printf("[*] Socket enlazado al puerto %d\n", PORT);

	if (listen(socket_fd, 10) < 0) {
		error("listen failed");
	}
	printf("[*] Servidor escuchando en el puerto %d\n", PORT);

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_fd;

		if ((client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
			perror("accept failed");
			continue;
		}
		printf("[*] Cliente conectado\n");

		char *client_request = read_response(client_fd);

		printf("[*] Response:\n%s\n", client_request);

		char *route = get_route(client_request);
		printf("[*] Route: %s\n", route);

		char body[] = 
			"<html lang='en'>"
			"<head>"
			"<metacharset='UTF-8'>"
			"<metaname='viewport'content='width=device-width,initial-scale=1.0'>"
			"<metahttp-equiv='X-UA-Compatible'content='ie=edge'>"
			"<title>HTML5Boilerplate</title>"
			"<linkrel='stylesheet'href='style.css'>"
			"</head>"
			"<body>"
			"<h1>Hola</h1>"
			"</body>"
			"</html>";

		int body_len = strlen(body);

		char response[1024];

		sprintf(response, "HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: %d\r\n"
			"\r\n"
			"%s"
			, body_len, body);

		
		if (send(client_fd, response, sizeof(response) - 1, 0) < 0) {
			perror("[!] Error al enviar datos al cliente");
		} else {
			printf("[*] Respuesta enviada al cliente\n");
		}
		
		close(client_fd);
		printf("[*] Conexión cerrada\n");
	}

	close(socket_fd);

	return (0);
}
