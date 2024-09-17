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

#include "utils.h"
#include "request.h"
#include "response.h"
#include "request_handler.h"

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

static volatile bool keep_running = true;

int server_fd;

void signalHandler(int signum)
{
  cout << "Interrupt signal (" << signum << ") received.\n";

  keep_running = false;

  // cleanup and close up stuff here
  close(server_fd);
  // terminate program

  //  exit(signum);
}

int main(int argc, char **argv)
{
  // Flush after every cout / cerr
  cout << unitbuf;
  cerr << unitbuf;

  server_fd = setup_server();

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);

  cout << "Waiting for a client to connect...\n";

  signal(SIGINT, signalHandler);

  // Create a thread pool
  ThreadPool *pool = new ThreadPool(100);

  // Create work for it
  while (keep_running)
  {
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);

    pool->addTask(new HandleRequestTask(client_fd));
  }

  delete pool;

  close(server_fd);

  return 0;
}
