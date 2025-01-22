#include "../include/server.h"
#include <stdio.h>
#include <string.h>

char *get_status_code(int code) {
	if (code == 200) {
		return ("200 OK");
	} else if (code == 400) {
		return ("400 BAD REQUEST");
	} else if (code == 404) {
		return ("404 NOT FOUND");
	}

	return (NULL);
}

char *get_content(char *html_template, char *status_text, int len) {
	char *content = (char *)malloc(len + 1);
	snprintf(content, len, html_template, status_text);
	return (content);
}

char *predefinied_content(char *status_code) {
	char html_template[] = "<html lang='en'>"
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
	int content_len = strlen(html_template);
	return (get_content(html_template, status_code, content_len + strlen(status_code)));
}

char *create_content(char *status_line, char *content_type, int *headers_len) {
	int body_len;
	int response_len;
	char *buffer;
	char *response;
	
	body_len = snprintf(NULL, 0, "Content-Type: %s\r\nContent-Length: ", content_type);
	buffer = (char *)malloc(body_len + 1);
	snprintf(buffer, body_len + 1, "Content-Type: %s\r\nContent-Length: ", content_type);

	response_len = snprintf(NULL, 0, "%s\r\n%s", status_line, buffer) + 1;
	response = (char *)malloc(response_len);
	snprintf(response, response_len, "%s\r\n%s", status_line, buffer);
	*headers_len = response_len;

	free(buffer);

	return (response);
}
