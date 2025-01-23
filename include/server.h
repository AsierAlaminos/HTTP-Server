#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>

void error(char *msg);
char *read_request(int client_fd);
char *get_route(char *request);

char *read_file(char *route, char *content_type, char *response, int headers_len, int *response_len);
int exist_file(char *route);

void *handle_request(void *arg);

char *get_status_code(int code);
char *create_content(char *status_line, char *content_type, int *headers_len);
char *predefinied_content(char *status_code);

#endif
