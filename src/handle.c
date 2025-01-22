#include "../include/server.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MimeType {
	char *extension;
	char *mime_type;
} MimeType;

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

char *handle_request(char *file_route, int *response_len) {
	char *status_code;
	char *response;
	char *status_line;
	char *content_type;
	int headers_len;
	int status_line_len;

	if (!exist_file(file_route)) {
		status_code = get_status_code(404);
	} else {
		status_code = get_status_code(200);
		content_type = handle_type(file_route);
	}
	
	status_line_len = snprintf(NULL, 0, "HTTP/1.1 %s", status_code) + 1;
	status_line = (char *)malloc(status_line_len);
	snprintf(status_line, status_line_len, "HTTP/1.1 %s", status_code);

	response = create_content(status_line, content_type, &headers_len);
	response = read_file(file_route, content_type, response, headers_len, response_len);
	//TODO: crear dos funciones, una para crear los headers y otra para el contenido
	//el content se crea directamente con read_file, se pasa el response y el buffer
	//es el response, en vez de usar una variable externa se escribe directamente
	//en la memoria reservada de response
	//se puede dar un tamaño para los headers y luego aumentarlo en read_file
	//con un realloc
	free(status_line);

	return (response);
}
