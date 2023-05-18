#ifndef HELPERS_H_
#define HELPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DATA_BUFFER 2000
#define CHUNK_SIZE 512

size_t send_data(int socket, int time_out, char * msg);

char * receive_data(int socket, int timeout);

char * parse_header(char * msg) {

#endif // !HELPERS_H_
