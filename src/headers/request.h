#pragma once

#include <string>
#include <unordered_map>

#include "options.h"

using namespace std;

class Request
{

public:
  string url;
  string host;
  string method;
  string protocol;

  string body;

  Options *options;

  unordered_map<string, string> params;
  unordered_map<string, string> headers;

  Request(string http_str, Options *options);

  friend ostream &operator<<(ostream &out, const Request &req);

private:
  void parse_first_line(string first_line);
  void parse_headers_and_body(vector<string> headers_line);
};
