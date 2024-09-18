#include <iostream>

#include "headers/server.h"

using namespace std;

void index(Request *req, Response *res)
{
  res->set_body("Hello World!", "text/plain");
  res->set_status(200, "OK");
}

void get_user_agent(Request *req, Response *res)
{
  res->set_body(req->headers.at("User-Agent"), "text/plain");
  res->set_status(200, "OK");
}

void echo(Request *req, Response *res)
{
  res->set_body(req->params.at("content"), "text/plain");
  res->set_status(200, "OK");
}

int main(int argc, char **argv)
{
  // Flush after every cout / cerr
  cout << unitbuf;
  cerr << unitbuf;

  HttpServer server = HttpServer();

  server.router.get("/", index);
  server.router.get("/user-agent", get_user_agent);
  server.router.get("/echo/:content", get_user_agent);

  server.start();
}
