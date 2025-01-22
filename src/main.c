#include "../include/server.h"
#include <string.h>


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

	while ((bytes_received = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytes_received] = '\0';
		if (request == NULL) {
			request = malloc(bytes_received + 1);
			if (request == NULL) {
				perror("[!] Error malloc");
				return NULL;
			}
			memcpy(request, buffer, bytes_received);
			request_lenght = bytes_received;
		} else {
			request = realloc(request, request_lenght + bytes_received + 1);
			if (request == NULL) {
				perror("[!] Error malloc");
				return NULL;
			}
			memcpy(request + request_lenght, buffer, bytes_received);
			request_lenght += bytes_received;
		}

		if (strstr(buffer, "\r\n\r\n") != NULL) {
			break;
		}
	}

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
		int client_fd;

		if ((client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
			perror("accept failed");
			continue;
		}
		printf("\n\n[*] Cliente conectado\n");

		char *client_request = read_request(client_fd);

		printf("[*] Request:\n%s\n", client_request);

		char *route = get_route(client_request);

		if (route == NULL || !strcmp(route, "/")) {
			route = "/index.html";
		}

		int response_len = 0;
		char *response = handle_request(route, &response_len);

		printf("[*] Response: %s\n", response);

		if (send(client_fd, response, response_len, 0) < 0) {
			perror("[!] Error al enviar datos al cliente");
		} else {
			printf("[*] Respuesta enviada al cliente\n");
		}
		
		close(client_fd);
		printf("[*] Conexión cerrada\n");
		free(client_request);
		free(response);
	}

	printf("[*] Cerrando servidor...\n");
	close(socket_fd);

	return (0);
}
