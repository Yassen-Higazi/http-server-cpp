This is my take on the
["Build Your Own HTTP server" Challenge](https://app.codecrafters.io/courses/http-server/overview). in C++.

[HTTP](https://en.wikipedia.org/wiki/Hypertext_Transfer_Protocol) is the
protocol that powers the web. In this challenge, you'll build a HTTP/1.1 server
that is capable of serving multiple clients.

Along the way you'll learn about TCP servers,
[HTTP request syntax](https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html),
and more.

**Note**: If you're viewing this repo on GitHub, head over to
[codecrafters.io](https://codecrafters.io) to try the challenge.

## Run

To run the application simply run

```shell
./run.sh
```

And you can test it by calling "<http://localhost:4221>"

```shell
 curl -v http://localhost:4221/
```

## new Routes

To define new routes edit the code in the [main file](src/main.rs) and use `server.router` to define new routes

example:

```cpp

void index(Request *req, Response *res)
{
  res->set_body("Hello World!", "text/plain");
  res->set_status(200, "OK");
}

int main(int argc, char **argv)
{
  HttpServer server = HttpServer();

  server.router.get("/", index);

  server.start();
}

```
