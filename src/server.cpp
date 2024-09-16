#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>

#include "request.h"

using namespace std;

int setup_server()
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0)
  {
    cerr << "Failed to create server socket\n";

    exit(1);
  }

  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
  {
    cerr << "setsockopt failed\n";

    exit(1);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(4221);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
  {
    cerr << "Failed to bind to port 4221\n";

    exit(1);
  }

  int connection_backlog = 5;

  if (listen(server_fd, connection_backlog) != 0)
  {
    cerr << "listen failed\n";

    exit(1);
  }

  return server_fd;
}

int main(int argc, char **argv)
{
  // Flush after every cout / cerr
  cout << unitbuf;
  cerr << unitbuf;

  int server_fd = setup_server();

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  cout << "Waiting for a client to connect...\n";

  int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);

  cout << "Client connected\n";

  char *buffer = new char[1024];

  int bytes_read = read(client_fd, buffer, 1024);

  if (bytes_read <= 0)
  {
    close(client_fd);
    close(server_fd);

    exit(bytes_read);
  }

  const string message = "HTTP/1.1 200 OK\r\n\r\n";

  Request request = Request(buffer);

  cout << request;

  write(client_fd, message.c_str(), message.size());

  close(client_fd);
  close(server_fd);

  return 0;
}
