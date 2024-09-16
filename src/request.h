#ifndef Request

#include <string>
#include <unordered_map>

class Request
{

public:
  std::string url;
  std::string host;
  std::string method;
  std::string protocol;

  std::string body;

  std::unordered_map<std::string, std::string> params;
  std::unordered_map<std::string, std::string> headers;

  Request(std::string http_str);

  friend std::ostream &operator<<(std::ostream &out, const Request &req);

private:
  void parse_first_line(std::string first_line);
  void parse_headers_and_body(std::vector<std::string> headers_line);
};
#endif
