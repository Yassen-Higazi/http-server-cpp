#include <string>
#include <vector>
#include <cctype>
#include <locale>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

#include "headers/utils.h"

using namespace std;

string readFile(const char *filename)
{
    // Create a text string, which is used to output the text file
    string line;
    string content;

    // Read from the text file
    ifstream file(filename);

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(file, line))
    {
        // Output the text from the file
        content += line;
    }

    if (line.empty())
    {
        throw invalid_argument("File does not exist");
    }

    // Close the file
    file.close();

    return content;
}

bool writeFile(const char *filename, const char *content)
{
    // Create and open a text file
    ofstream file(filename);

    // Write to the file
    file << content;

    // Close the file
    file.close();

    return true;
}

vector<string> split(string s, string delim)
{
    string token = "";

    vector<string> res;

    for (int i = 0; i < s.size(); i++)
    {
        bool flag = true;

        for (int j = 0; j < delim.size(); j++)
        {
            if (s[i + j] != delim[j])
                flag = false;
        }

        if (flag)
        {
            if (token.size() > 0)
            {
                res.push_back(token);
                token = "";
                i += delim.size() - 1;
            }
        }
        else
        {
            token += s[i];
        }
    }

    res.push_back(token);

    return res;
}

string bufferToString(char *buffer, int buff_len)
{
    string ret(buffer, buff_len);

    return ret;
}

// trim from start (in place)
void ltrim(string &s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch)
                               { return !isspace(ch); }));
}

// trim from end (in place)
void rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                    { return !isspace(ch); })
                .base(),
            s.end());
}

string compress(const std::string &str, int compression_level)
{
    z_stream zs; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compression_level) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef *)str.data();
    zs.avail_in = str.size(); // set the z_stream's input

    int ret;
    char out_buffer[10240];
    std::string out_string;

    // retrieve the compressed bytes block wise
    do
    {
        zs.next_out = reinterpret_cast<Bytef *>(out_buffer);
        zs.avail_out = sizeof(out_buffer);

        ret = deflate(&zs, Z_FINISH);

        if (out_string.size() < zs.total_out)
        {
            // append the block to the output string
            out_string.append(out_buffer, zs.total_out - out_string.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END)
    { // an error occurred that was not EOF
        ostringstream oss;

        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;

        throw(runtime_error(oss.str()));
    }

    return out_string;
}
