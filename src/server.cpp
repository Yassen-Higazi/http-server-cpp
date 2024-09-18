#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <shared_mutex>

#include "headers/server.h"

using namespace std;

HttpServer::HttpServer()
{
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

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

  pool = new ThreadPool(100);
}

HttpServer::~HttpServer()
{
  delete &pool;
  close(server_fd);
}

void HttpServer::start()
{
  cout << "Waiting for a client to connect...\n";

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  // Create work for it
  while (true)
  {
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);

    pool->addTask(new HandleRequestTask(client_fd, &router));
  }
}