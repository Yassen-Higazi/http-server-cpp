#ifndef Request
#include <string>

class Request {

  public:
    std::string url;
    std::string host;
    std::string a;

    Request(std::string http_str);
};
#endif
