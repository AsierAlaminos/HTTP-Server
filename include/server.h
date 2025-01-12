#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int PORT = 8080;

void error(char *msg);

char *read_file(char *route);
int exist_file(char *route);

#endif
