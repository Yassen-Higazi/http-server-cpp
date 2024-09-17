#pragma once

#include <regex>
#include <string>
#include <iostream>
#include <shared_mutex>
#include <unordered_map>

#include "request.h"
#include "response.h"

using namespace std;

typedef shared_mutex Lock;
typedef unique_lock<Lock> WriteLock; // C++ 11
typedef shared_lock<Lock> ReadLock;  // C++ 14

using Handler = void (*)(Request *req, Response *res);

struct Handler_return
{
  Handler *handler;
  unordered_map<string, int> *params;
};

class TrieNode
{
public:
  unordered_map<string, Handler> handler;
  unordered_map<string, int> params;
  unordered_map<string, TrieNode *> children;
};

class Router
{
public:
  void get(string url, Handler handler);
  void put(string url, Handler handler);
  void del(string url, Handler handler);
  void post(string url, Handler handler);
  void head(string url, Handler handler);
  void patch(string url, Handler handler);
  void options(string url, Handler handler);

  Handler_return get_handler(string method, string url);
  void define_route(string method, string url, Handler Handler);

  Router();

private:
  TrieNode root;
  Lock routerLock;
};