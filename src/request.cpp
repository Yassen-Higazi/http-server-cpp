#include <string>
#include <iostream>

#include "utils.h"
#include "request.h"

using namespace std;

Request::Request(string http_str)
{
  vector<string> http_vec = split(http_str, "\r\n");

  parse_first_line(http_vec[0]);
  parse_headers_and_body(vector<string>(http_vec.begin() + 1, http_vec.end()));
};

void Request::parse_first_line(string line)
{
  vector<string> line_parts = split(line, " ");

  url = line_parts[1];
  method = line_parts[0];
  protocol = line_parts[2];
};

void Request::parse_headers_and_body(vector<string> request_vec)
{
  for (int i = 0; i < request_vec.size(); i++)
  {
    string line = request_vec[i];

    vector<string> pair = split(line, ": ");

    if (pair.size() != 2)
    {
      body = request_vec[i];

      ltrim(body);

      break;
    }

    headers[pair[0]] = pair[1];
  }
};

ostream &operator<<(ostream &out, Request const &req)
{
  out << "Request { ";
  out << "Method: " << req.method << ' ';
  out << "Url: " << req.url << ' ';
  out << "Protocol: " << req.protocol << ' ';

  out << "headers: { ";
  for (auto kv : req.headers)
  {
    out << kv.first << ": " << kv.second << ", ";
  }
  out << " } ";

  out << "Body: " << req.body;

  out << " }";

  return out;
}
