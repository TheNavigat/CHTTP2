#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int socket_descriptor, client_socket, read_size;
  struct sockaddr_in server;
  char client_message[2000];

  // Create socket

  socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

  if(socket_descriptor == -1)
    printf("Could not create socket.\n");

  printf("Socket created\n");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8888);

  // Bind
  if(bind(socket_descriptor, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    printf("Bind failed, error.\n");
    return 1;
  }

  printf("Bind done.\n");

  // Listen
  listen(socket_descriptor, 3);

  printf("Waiting for incoming connections...\n");

  int c;

  c = sizeof(struct sockaddr_in);

  client_socket = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&c);

  if(client_socket < 0)
  {
    printf("Accepting failed.\n");
    return 1;
  }

  printf("Connection accepted.\n");

  // Receive message from client
  while((read_size = recv(client_socket, client_message, 2000, 0)) > 0)
  {
    // Echo
    client_message[read_size] = 0;
    printf("Strlen: %d\n", strlen(client_message));
    write(client_socket, client_message, strlen(client_message));
    //memset(client_message, '\0', sizeof(client_message));
    client_message[0] = '\0';
  }

  if(read_size == 0)
  {
    puts("Client disconnected.\n");
    fflush(stdout);
  }
  else if(read_size < 0)
  {
    printf("recv failed.\n");
  }

  return 0;
}

