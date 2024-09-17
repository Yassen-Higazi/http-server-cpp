#include <string>
#include <iostream>
#include <unistd.h>

#include "utils.h"
#include "request.h"
#include "response.h"
#include "request_handler.h"

using namespace std;

HandleRequestTask::HandleRequestTask(int client_socket_fd, Router *r) : Task()
{
  cout << "Client connected\n";

  buffer = new char[1024];

  client_fd = client_socket_fd;

  router = r;
};

HandleRequestTask::~HandleRequestTask()
{
  // Debug prints
  pthread_mutex_lock(&console_mutex);

  printf("Connection Client: %d - is being closed\n", client_fd);

  close(client_fd);

  printf("Thread Id: %d - being deleted\n", (int)(size_t)pthread_self());

  pthread_mutex_unlock(&console_mutex);
}

void HandleRequestTask::run()
{
  int bytes_read = read(client_fd, buffer, 1024);

  if (bytes_read <= 0)
  {
    close(client_fd);

    exit(bytes_read);
  }

  string response_body = handle_connection(buffer);

  write(client_fd, response_body.c_str(), response_body.size());

  delete this;
}

void HandleRequestTask::showTask()
{
  // More debug printing
  pthread_mutex_lock(&console_mutex);
  printf("thread %d computing \n", (int)(size_t)pthread_self());
  pthread_mutex_unlock(&console_mutex);
}

string HandleRequestTask::handle_connection(char *buffer)
{
  Request request = Request(buffer);

  Response response = Response(&request);

  try
  {
    Handler_return result = router->get_handler(request.method, request.url);

    (*result.handler)(&request, &response);
  }
  catch (const invalid_argument &e)
  {
    response.set_status(404, "Not Found");
  }

  return response.to_http_format();
}
