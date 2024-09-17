#include <string>
#include <iostream>
#include <unistd.h>

#include "utils.h"
#include "request.h"
#include "response.h"
#include "request_handler.h"

using namespace std;

HandleRequestTask::HandleRequestTask(int client_socket_fd) : Task()
{
  cout << "Client connected\n";

  buffer = new char[1024];

  client_fd = client_socket_fd;
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

  if (request.url.compare("/") == 0)
  {
    response.set_status(200, "OK");
  }
  else if (request.url.contains("/echo"))
  {
    vector<string> splits = split(request.url, "/");

    string data = "";

    for (string split : splits)
    {
      if (split.compare("echo") == 0)
      {
        continue;
      }

      data += split;
    }

    response.set_status(200, "OK");
    response.set_body(data, "text/plain");
  }
  else if (request.url.contains("/user-agent"))
  {
    response.set_status(200, "OK");
    response.set_body(request.headers["User-Agent"], "text/plain");
  }
  else
  {
    response.set_status(404, "Not Found");
  }

  return response.to_http_format();
}
