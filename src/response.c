#include "../include/server.h"

char *create_status_header(int code) {
	char *status_line;
	char *status_code;
	int status_line_len;
	char *template = "HTTP/1.1 %s";

	status_code = get_status_code(code);
	status_line_len = snprintf(NULL, 0, template, status_code) + 1;
	status_line = (char *)malloc(status_line_len);
	snprintf(status_line, status_line_len, template, status_code);
	
	return (status_line);
}

char *get_status_code(int code) {
	if (code == 200) {
		return ("200 OK");
	} else if (code == 400) {
		return ("400 Bad Request");
	} else if (code == 404) {
		return ("404 Not Found");
	} else if (code == 403) {
		return ("403 Forbidden");
	}

	return (NULL);
}

char *predefinied_content(char *headers, char *status_code) {
	char template[] = "%s\r\n\r\n<html lang='en'>"
			"<head>"
			"<metacharset='UTF-8'>"
			"<metaname='viewport'content='width=device-width,initial-scale=1.0'>"
			"<metahttp-equiv='X-UA-Compatible'content='ie=edge'>"
			"<title>Server HTTP</title>"
			"</head>"
			"<body>"
			"<h1>%s</h1>"
			"</body>"
			"</html>";
	int content_len = strlen(template);
	char *content = (char *)malloc(content_len + strlen(headers) + strlen(status_code) + 1);
	snprintf(content, content_len + strlen(headers) + strlen(status_code), template, headers, status_code);

	return (content);
}

char *create_header(char *status_line, char *content_type, int *headers_len) {
	int body_len;
	int response_len;
	char *buffer;
	char *response;
	char *template = "server: HTTP server in C\r\nContent-Type: %s\r\nContent-Length: ";
	
	body_len = snprintf(NULL, 0, template, content_type);
	buffer = (char *)malloc(body_len + 1);
	snprintf(buffer, body_len + 1, template, content_type);

	response_len = snprintf(NULL, 0, "%s\r\n%s", status_line, buffer) + 1;
	response = (char *)malloc(response_len);
	snprintf(response, response_len, "%s\r\n%s", status_line, buffer);
	*headers_len = response_len;

	free(buffer);

	return (response);
}
