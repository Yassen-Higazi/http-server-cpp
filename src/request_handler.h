#pragma once

#include "thread_pool.h"

class HandleRequestTask : public Task
{
public:
  HandleRequestTask(int client_fd);

  ~HandleRequestTask();

  virtual void run();

  virtual void showTask();

  string handle_connection(char *buffer);

private:
  char *buffer;
  int client_fd;
};