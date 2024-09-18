#pragma once

#include <string>
#include <arpa/inet.h>

using namespace std;

struct Options
{
  sockaddr_in addr;
  string file_directory = "./";
};