#include "timserver.h"
#include "helpers.h"

void setupAddressStruct(struct sockaddr_in* address, int portNumber) {
}

void throw_error(const char *msg) {
  perror(msg);
  fflush(stdout);
  exit(1);
} 

int main(int argc, char const* argv[]) {
  int connectionSocket;
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);
  pthread_t pids[MAX_CONNECTIONS];
  int pid_idx = 0;
  
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    fflush(stdout);
    exit(1);
  } 
  
  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  
  if (listenSocket < 0)
    throw_error("ERROR opening socket");

  setupAddressStruct(&serverAddress, atoi(argv[1]));
  
  memset((char*) serverAddress, '\0', sizeof(*serverAddress)); 
  serverAddress->sin_family = AF_INET;
  serverAddress->sin_port = htons(portNumber);
  serverAddress->sin_addr.s_addr = INADDR_ANY;

  if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    throw_error("ERROR on binding");
  else {
    printf("SERVER: Listening on port %d\n", atoi(argv[1]));
    fflush(stdout);
  }
  
  listen(listenSocket, MAX_CONNECTIONS); 

  while(1) {
    int pid_count = 0;

    if ((pid_count = fork()) == 0) {
      connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); 
      if (connectionSocket < 0)
          throw_error("ERROR on accept");
      else
        printf("SERVER: Connected to client running at host %d port %d\n", ntohs(clientAddress.sin_addr.s_addr), ntohs(clientAddress.sin_port));

      char * msg = receive_data(connectionSocket, 5);

      if (msg == NULL) {
        printf("SERVER: Timeout reached. No data received.\n");
        fflush(stdout);
      }
      else {
        printf("SERVER: Received \"%s\" from client\n", msg);
        fflush(stdout);
      }

      free(msg);
      close(connectionSocket); 
    }
    else {
      pids[pid_idx++] = pid_count;
      if (pid_idx >= MAX_CONNECTIONS - 1) {
        pid_idx = 0;
        while(pid_idx < MAX_CONNECTIONS){
          waitpid(pids[pid_idx++], NULL, 0);
        }
        pid_idx = 0;
      }
    }
  }
  close(listenSocket); 
 return 0;
}
