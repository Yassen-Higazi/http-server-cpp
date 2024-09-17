#pragma once

#include "request_handler.h"

class HttpServer {

  public:
    HttpServer();

    ~HttpServer();

    void start();
  
  private:
    int server_fd;
    ThreadPool *pool;
};