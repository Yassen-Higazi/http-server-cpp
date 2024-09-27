#pragma once
#include <iostream>
#include <filesystem>
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

#include "utils.h"
#include "router.h"
#include "options.h"
#include "request_handler.h"

namespace fs = std::filesystem;

class HttpServer
{

public:
  HttpServer(string file_dir);

  ~HttpServer();

  void start();

  Router router;

  Options options;

private:
  int server_fd;
  ThreadPool *pool;
};