#include <string>
#include <iostream>

#include "utils.h"
#include "request.h"

Request::Request(std::string http_str) {
  std::vector<std::string> http_vec = split(http_str, "\r\n");

  for (auto i: http_vec) std::cout << i;
}

