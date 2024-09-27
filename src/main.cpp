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
  res->set_body(req->params.at(":content"), "text/plain");
  res->set_status(200, "OK");
}

void read_file(Request *req, Response *res)
{
  fs::path dir(req->options->file_directory);
  fs::path file(req->params.at(":filename"));
  fs::path full_path = dir / file;

  try
  {
    string content = readFile(full_path.c_str());

    res->set_body(content, "application/octet-stream");
    res->set_status(200, "OK");
  }
  catch (const exception &err)
  {
    cerr << err.what() << endl;

    res->set_status(404, "Not Found");
  }
}

void write_file(Request *req, Response *res)
{
  fs::path dir(req->options->file_directory);
  fs::path file(req->params.at(":filename"));
  fs::path full_path = dir / file;

  writeFile(full_path.c_str(), req->body.c_str());

  res->set_status(200, "OK");
}

int main(int argc, char **argv)
{
  // Flush after every cout / cerr
  cout << unitbuf;
  cerr << unitbuf;

  cout << argv << endl;

  HttpServer server = HttpServer();

  server.router.get("/", index);
  server.router.get("/echo/:content", echo);
  server.router.get("/user-agent", get_user_agent);
  server.router.get("/files/:filename", read_file);
  server.router.post("/files/:filename", write_file);

  server.start();
}
