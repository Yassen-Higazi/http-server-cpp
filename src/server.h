#pragma once

#include "router.h"
#include "request_handler.h"

class HttpServer {

  public:
    HttpServer();

    ~HttpServer();

    void start();

    Router router;
  
  private:
    int server_fd;
    ThreadPool *pool;
};