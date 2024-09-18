#pragma once

#include "router.h"
#include "options.h"
#include "thread_pool.h"

class HandleRequestTask : public Task
{
public:
  HandleRequestTask(int client_fd, Router *router, Options *opts);

  ~HandleRequestTask();

  virtual void run();

  virtual void showTask();

  string handle_connection(char *buffer);

private:
  char *buffer;
  int client_fd;

  Router *router;
  Options *options;
};