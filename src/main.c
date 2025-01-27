#include "../include/server.h"
#include <stdio.h>

static volatile int keepRunning = 1;

void cleanup(int signo) {
	(void)signo;
	keepRunning = 0;
}

void error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

char *read_request(int client_fd) {
	char buffer[1024] = "";
	memset(buffer, 0, sizeof(buffer));
	int bytes_received;
	char *request = NULL;
	int request_lenght = 0;

	printf("[*] fuera del bucle read_request\n");
	while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytes_received] = '\0';
		if (request == NULL) {
			request = malloc(bytes_received + 1);
			if (request == NULL) {
				perror("[!] Error malloc");
				return (NULL);
			}
			memcpy(request, buffer, bytes_received);
			request_lenght = bytes_received;
		} else {
			request = realloc(request, request_lenght + bytes_received + 1);
			if (request == NULL) {
				perror("[!] Error malloc");
				return (NULL);
			}
			memcpy(request + request_lenght, buffer, bytes_received);
			request_lenght += bytes_received;
		}

		if (strstr(buffer, "\r\n\r\n") != NULL) {
			break;
		}
		printf("[*] bucle read_request\n");
	}

	printf("[*] final del bucle read_request\n");

	if (bytes_received == 0) {
		perror("[!] Error al leer datos del cliente");
	}

	if (request != NULL) {
		request[request_lenght] = '\0';
	}

	return (request);
}

char *get_route(char *request) {
	char *request_splitted = strtok(request, "\n");
	
	strtok(request_splitted, " ");
	char *path = strtok(NULL, " ");

	return (path);
}

int main() {
	int socket_fd;
	int port = 8080;
	struct sockaddr_in server_addr;

	struct sigaction sa;
	sa.sa_handler = cleanup;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0) {
		error("socket failed");
	}

	int opt = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

	printf("[*] Socket creado con exito\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		error("socket failed");
	}
	printf("[*] Socket enlazado al puerto %d\n", port);

	if (listen(socket_fd, 10) < 0) {
		error("listen failed");
	}
	printf("[*] Servidor escuchando en el puerto %d\n", port);

	while (keepRunning) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int *client_fd = malloc(sizeof(int));

		if ((*client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
			perror("accept failed");
			continue;
		}
		char *client_ip = inet_ntoa(client_addr.sin_addr);
		printf("\n\n[*] Cliente conectado: %s\n", client_ip);

		pthread_t ptid;
		pthread_create(&ptid, NULL, handle_request, (void *)client_fd);
		pthread_detach(ptid);
	}

	printf("[*] Cerrando servidor...\n");
	close(socket_fd);

	return (0);
}
