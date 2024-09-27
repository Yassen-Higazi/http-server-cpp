#pragma once

#include <cstring>
#include <string>
#include <zlib.h>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

string readFile(const char *filename);

bool writeFile(const char *filename, const char *content);

vector<string> split(string s, string delim);

string bufferToString(char *buffer, int buff_len);

void ltrim(string &s);

void rtrim(string &s);

string compress(const std::string &str, int compression_level = Z_BEST_COMPRESSION);