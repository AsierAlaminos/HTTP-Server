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

void read_response(int client_fd) {
	char buffer[1024] = "";
	memset(buffer, 0, sizeof(buffer));
	int bytes_received;

	while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytes_received] = '\0';
		printf("[*] Client: \n%s\n", buffer);

		if (strstr(buffer, "\r\n\r\n") != NULL) {
			break;
		}
	}

	if (bytes_received == 0) {
		perror("[!] Error al leer datos del cliente");
	}
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

		read_response(client_fd);

		char response[] =
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain\r\n"
			"Content-Length: 5\r\n"
			"\r\n"
			"Hola";
		
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
