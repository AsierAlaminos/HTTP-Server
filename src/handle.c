#include "../include/server.h"
#include <stdio.h>
#include <stdlib.h>

char *handle_request(char *file_route) {
	char *status_code;
	char *response;
	char *content;
	char *status_line;
	int status_line_len;

	if (!exist_file(file_route)) {
		status_code = get_status_code(404);
	} else {
		status_code = get_status_code(200);
	}
	
	status_line_len = snprintf(NULL, 0, "HTTP/1.1 %s", status_code) + 1;
	status_line = (char *)malloc(status_line_len);
	snprintf(status_line, status_line_len, "HTTP/1.1 %s", status_code);

	content = predefinied_content(status_code);
	response = create_content(status_line, status_line_len, "text/html", content);
	free(content);

	return (response);
}
