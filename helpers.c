#include "helpers.h"

size_t send_data(int socket, int time_out, char * msg){
  struct timeval begin, now;
  double time_difference;
  ssize_t charsRead = 0;
  size_t idx = 0, tmp_string_idx = 0;

  char * data_chunk_1 = malloc(CHUNK_SIZE * sizeof(char));
  memset(data_chunk_1, 0, CHUNK_SIZE);

  if (strlen(msg) < CHUNK_SIZE) {
    strcpy(data_chunk_1, msg);
    charsRead = send(socket, data_chunk_1, strlen(data_chunk_1), 0);
  }
  else {
    char ** tmp_string = malloc(DATA_BUFFER * sizeof(char*));
    
    fcntl(socket, F_SETFL, O_NONBLOCK);

    gettimeofday(&begin , NULL);

    while (1) {
      gettimeofday(&now, NULL);
      time_difference = (now.tv_sec - begin.tv_sec) + 1e-6 *(now.tv_sec - begin.tv_sec);

      tmp_string[tmp_string_idx] = (char*) malloc(CHUNK_SIZE * sizeof(char));
      strncpy(tmp_string[tmp_string_idx], msg + idx, CHUNK_SIZE); 
      
      charsRead += send(socket, tmp_string[tmp_string_idx], CHUNK_SIZE, 0);

      if (charsRead == (ssize_t)strlen(msg) || charsRead > (ssize_t)strlen(msg))
        break;
      else if (charsRead == 0 && time_difference > time_out)
        break;

      tmp_string_idx++;
      idx += CHUNK_SIZE;
    }  

    for (size_t i = 0; i <= tmp_string_idx; i++)
      free(tmp_string[i]);
    
    free(tmp_string);
    
  }

  free(data_chunk_1);

  return charsRead;
}

char * receive_data(int socket, int timeout) {
  ssize_t package_size = 0, total_size = 0;
  struct timeval begin, now;
  double time_difference;
  
  char * data_chunk = malloc(CHUNK_SIZE * sizeof(char));
  char ** tmp_string = malloc(DATA_BUFFER * sizeof(char*));
  size_t tmp_string_idx = 0;

  fcntl(socket, F_SETFL, O_NONBLOCK);
  gettimeofday(&begin , NULL);

  while(1) {

      gettimeofday(&now, NULL);
      time_difference = (now.tv_sec - begin.tv_sec) + 1e-6 *(now.tv_sec - begin.tv_sec);

      if (total_size > 0 && time_difference > timeout)
        break;
      else if (time_difference > timeout * 2)
        break;

      memset(data_chunk, '\0', CHUNK_SIZE);

      package_size = recv(socket, data_chunk, CHUNK_SIZE, 0);
      if (package_size <= 0)
        sleep(1);
      else {
        total_size += package_size;
        tmp_string[tmp_string_idx] = malloc(package_size + 1 * sizeof(char));
        memset(tmp_string[tmp_string_idx], '\0',  package_size + 1);
        strncpy((tmp_string[tmp_string_idx]), data_chunk, package_size);
        tmp_string_idx++;

        gettimeofday(&begin , NULL);
      }
  }

  char * total_data = malloc(total_size + 1 * sizeof(char));
  memset(total_data, '\0', total_size + 1);

  for (size_t i = 0; i < tmp_string_idx; i++)
      strncat(total_data, tmp_string[i], strlen(tmp_string[i]));

  for (size_t i = 0; i < tmp_string_idx; i++) {
      if (tmp_string[i] != NULL) {
        free(tmp_string[i]);
      }
  }

  free(tmp_string);
  free(data_chunk);
  
  return total_data;
}

char ** parse_request(char * data) {
  if (data == NULL)
    return NULL;

  char ** parsed_request = calloc(DATA_BUFFER, sizeof(char*));
  size_t idx = 0;
  char * token = strtok(data, "\r\n");

  while (token != NULL) {
    parsed_request[idx] = malloc(strlen(token) + 1 * sizeof(char));
    memset(parsed_request[idx], '\0', strlen(token) + 1);
    strncpy(parsed_request[idx], token, strlen(token));
    idx++;
    token = strtok(NULL, "\r\n");
  }

  return parsed_request;
}

