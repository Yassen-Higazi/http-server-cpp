#include <vector>

#include "headers/utils.h"
#include "headers/router.h"

regex const params_regex(":([a-zA-Z0-9_]+)");

Router::Router()
{
  root = TrieNode();
}

void Router::get(string url, Handler handler)
{
  define_route("GET", url, handler);
}

void Router::post(string url, Handler handler)
{
  define_route("POST", url, handler);
}

void Router::put(string url, Handler handler)
{
  define_route("PUT", url, handler);
}

void Router::patch(string url, Handler handler)
{
  define_route("PATCH", url, handler);
}

void Router::del(string url, Handler handler)
{
  define_route("DELETE", url, handler);
}

void Router::options(string url, Handler handler)
{
  define_route("OPTIONS", url, handler);
}

void Router::head(string url, Handler handler)
{
  define_route("HEAD", url, handler);
}

void Router::define_route(string method, string url, Handler handler)
{
  WriteLock w_lock(routerLock);

  unordered_map<string, int> params = unordered_map<string, int>();

  auto params_begin = sregex_iterator(url.begin(), url.end(), params_regex);

  auto params_end = sregex_iterator();

  for (sregex_iterator k = params_begin; k != params_end; ++k)
  {
    smatch match = *k;
    string match_str = match.str();

    params[match_str] = match.position();
  }

  url = regex_replace(url, params_regex, "*");

  TrieNode *current_node = &root;

  for (string segment : split(url, "/"))
  {
    if (segment.empty())
    {
      continue;
    }

    if (current_node->children[segment] == nullptr)
    {
      TrieNode *newNode = new TrieNode();

      current_node->children[segment] = newNode;
    }

    current_node = current_node->children[segment];
  }

  current_node->params = params;
  current_node->handler[method] = handler;
}

Handler_return Router::get_handler(string method, string url)
{
  ReadLock r_lock(routerLock);

  TrieNode *current_node = &root;

  for (string segment : split(url, "/"))
  {
    if (segment.empty())
    {
      continue;
    }

    if (current_node->children[segment] == nullptr)
    {
      if (current_node->children["*"] == nullptr)
      {
        throw invalid_argument("No Handler found for " + method + ": " + url);
      }

      current_node = current_node->children["*"];
    }

    current_node = current_node->children[segment];
  }

  Handler_return result;

  cout << endl
       << "Test1" << endl;
  result.params = &current_node->params;

  cout << endl
       << "Test2" << endl;
  result.handler = &current_node->handler.at(method);
  cout << endl
       << "Test3" << endl;

  return result;
}