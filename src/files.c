#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_file_route(char *route) {
	int len_route = strlen("test_files");
	len_route += strlen(route) + 1;
	char *file_route;

	file_route = (char *)malloc(len_route);

	snprintf(file_route, len_route, "test_files%s", route);

	return (file_route);
}

char *read_file(char *route, char *content_type, char *response, int headers_len, int *response_len) {
	int file_fd;
	char *file_route;
	char *buffer = NULL;

	file_route = get_file_route(route);
	file_fd = open(file_route, O_RDONLY);
	if (file_fd == -1) {
		return (NULL);
	}

	free(file_route);

	struct stat file_stat;
	fstat(file_fd, &file_stat);
	int file_size = (int)file_stat.st_size;

	int size_len = snprintf(NULL, 0, "%d\r\n\r\n", file_size);
	response = realloc(response, headers_len + size_len + file_size + 1);
	buffer = (char *)malloc(headers_len);
	memcpy(buffer, response, headers_len);
	snprintf(response, headers_len + size_len, "%s%d\r\n\r\n", buffer, file_size);
	*response_len = strlen(response);
	free(buffer);

	ssize_t bytes_read = read(file_fd, response + *response_len, file_size);
	*response_len += bytes_read;
	if (!strstr(content_type, "image")) {
		response[headers_len + size_len + bytes_read] = '\0';
	}

	close(file_fd);
	return (response);
}

int exist_file(char *route) {
	FILE *file;
	char *file_route;

	file_route = get_file_route(route);

	file = fopen(file_route, "r");

	if (file == NULL) {
		return (0);
	}
free(file_route);
	fclose(file);
	return (1);
}

