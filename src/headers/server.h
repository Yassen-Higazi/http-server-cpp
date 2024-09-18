#pragma once
#include <iostream>
#include <filesystem>

#include "utils.h"
#include "router.h"
#include "options.h"
#include "request_handler.h"

namespace fs = std::filesystem;

class HttpServer
{

public:
  HttpServer();

  ~HttpServer();

  void start();

  Router router;

  Options options;

private:
  int server_fd;
  ThreadPool *pool;
};