#pragma once

#include <string>
#include <unordered_map>

#include "request.h"

using namespace std;

class Response
{

public:
  int status = 200;

  string method;
  string status_name = "OK";
  string protocol = "HTTP/1.1";
  string content_type = "text/plain";

  string body;

  unordered_map<string, string> headers;

  Response(string method, string protocol);

  Response(Request *request);

  void set_status(int status, string status_name);

  void set_body(string body, string content_type);

  string to_http_format();

  friend ostream &operator<<(ostream &out, const Response &req);

  void handle_compression(string accept_encoding);
};
