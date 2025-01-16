#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *get_file_route(char *route) {
	int len_route = strlen("test_files");
	len_route += strlen(route) + 1;
	char *file_route;

	file_route = (char *)malloc(len_route);

	snprintf(file_route, len_route, "test_files%s", route);

	return (file_route);
}

char *read_file(char *route) {
	FILE *file;
	char *file_route;
	char *file_content = NULL;
	long content_size = 0;

	file_route = get_file_route(route);
	file = fopen(file_route, "r");
	if (file == NULL) {
		return (NULL);
	}

	char cwd[100];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		perror("[!] getcwd error");
	} else {
		printf("[*] Current directory: %s\n", cwd);
	}
	printf("file_route: %s\n", file_route);

	fseek(file, 0, SEEK_END);
	content_size = ftell(file);
	rewind(file);

	file_content = (char *)malloc(content_size + 1);

	fread(file_content, sizeof(char), content_size, file);
	file_content[content_size] = '\0';

	fclose(file);
	return (file_content);
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

