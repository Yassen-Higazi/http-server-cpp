#include <iostream>

#include "server.h"

using namespace std;

int main(int argc, char **argv)
{
  // Flush after every cout / cerr
  cout << unitbuf;
  cerr << unitbuf;

  HttpServer server = HttpServer();

  server.start();
}
