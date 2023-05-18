#ifndef TIMSERVER_H_
#define TIMSERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netdb.h>

#define MAX_CONNECTIONS 5
#define MAX_BUFFER 256

void throw_error(const char *msg);

#endif // !TIMSERVER_H_
