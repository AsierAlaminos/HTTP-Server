#include "../include/server.h"

char *handle_request(char *file_route) {
	char *status_code;
	char *response;
	char *content;

	if (!exist_file(file_route)) {
		status_code = get_status_code(404);
	} else {
		status_code = get_status_code(200);
	}

	content = predefinied_content(status_code);
	response = create_content("text/html", content);

	free(content);

	return (response);
}
