#include <string>
#include <vector>
#include <cctype>
#include <locale>
#include <algorithm>
#include <functional>

using namespace std;

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