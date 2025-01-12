#include "../include/server.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_file_route(char *route) {
	int len_route = strlen("test_files");
	len_route += strlen(route);
	char *file_route;

	snprintf(file_route, len_route, "test_files%s", route);

	return (file_route);
}

char *read_file(char *route) {
	FILE *file;
	char *file_route;
	char *file_content = NULL;
	unsigned long content_size = 0;
	int character;

	file_route = get_file_route(route);
	file = fopen(file_route, "r");

	while ((character = fgetc(file)) != EOF) {
		++content_size;
		char *temp = realloc(file_content, content_size + 2);
		if (temp == NULL) {
			free(file_content);
			fclose(file);
			error("[!] Error al reservar memoria");
		}

		file_content = temp;
		file_content[content_size++] = character;
	}
	
	if (file_content != NULL) {
		file_content[content_size] = '\0';
	}

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

	fclose(file);
	return (1);
}

