#include "../include/server.h"
#include <stddef.h>
#include <stdlib.h>

char *url_decode(char *url) {
	size_t url_len = strlen(url);
	char *decode_url = malloc(url_len + 1);

	size_t decode_len = 0;
	for (size_t i = 0; i < url_len; i++) {
		if (url[i] == '%' && i + 2 < url_len && isxdigit(url[i + 1]) && isxdigit(url[i + 2])) {
			int hex_val;
			sscanf(url + i + 1, "%2x", &hex_val);
			decode_url[decode_len++] = hex_val;
			i += 2;
		} else if (url[i] == '+') {
			decode_url[decode_len++] = ' ';
		} else {
			decode_url[decode_len++] = url[i];
		}
	}

	char *resized = realloc(decode_url, decode_len + 1);
	if (resized == NULL) {
		free(decode_url);
		return (NULL);
	}

	resized[decode_len] = '\0';

	return (resized);
}

int directory_traversal(char *url, size_t url_len) {
	for (size_t i = 0; i < url_len - 1; i++) {
		if (url[i] == '.' && url[i + 1] == '.') {
			return (-1);
		}
	}

	return (1);
}
