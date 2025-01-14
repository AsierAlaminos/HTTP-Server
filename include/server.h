#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void error(char *msg);

char *read_file(char *route);
int exist_file(char *route);

char *handle_request(char *file_route);

char *get_status_code(int code);
char *create_content(char *response, int response_len, char *content_type, char *content);
char *predefinied_content(char *status_code);

#endif
