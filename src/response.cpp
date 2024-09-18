#include <string>
#include <iostream>

#include "headers/utils.h"
#include "headers/response.h"

using namespace std;

Response::Response(Request *request)
{
  method = request->method;
  protocol = request->protocol;
}

Response::Response(string method, string protocol)
{
  protocol = protocol;
  method = method;
};

void Response::set_status(int new_status, string new_status_name)
{
  status = new_status;
  status_name = new_status_name;
}

void Response::set_body(string data, string content_type)
{
  body = data;

  headers["Content-Type"] = content_type;
  headers["Content-Length"] = to_string(data.size());
}

string Response::to_http_format()
{
  string res = protocol + " " + to_string(status) + " " + status_name + "\r\n";

  for (auto kv : headers)
  {
    res += kv.first + ": " + kv.second + "\r\n";
  }

  res += "\r\n";

  res += body;

  return res;
}

ostream &operator<<(ostream &out, Response const &req)
{
  out << "Response { ";
  out << "Method: " << req.method << ", ";
  out << "Status: " << req.status << ", ";
  out << "Protocol: " << req.protocol << ", ";

  out << "headers: {";
  for (auto kv : req.headers)
  {
    out << " " << kv.first << ": " << kv.second << ",  ";
  }
  out << "}, ";

  out << "Body: " << req.body;

  out << " }";

  return out;
}
