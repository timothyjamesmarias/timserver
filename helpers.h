#ifndef HELPERS_H_
#define HELPERS_H_

#define DATA_BUFFER 2000
#define CHUNK_SIZE 512

size_t send_data(int socket, int time_out, char * msg);

char * receive_data(int socket, int timeout);

#endif // !HELPERS_H_
