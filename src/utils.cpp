#include <string>
#include <vector>

std::vector<std::string> split(std::string s, std::string delim) {
    std::string token = "";

    std::vector<std::string> res;

    for (int i = 0; i < s.size(); i++) {
        bool flag = true;

        for (int j = 0; j < delim.size(); j++) {
            if (s[i + j] != delim[j]) flag = false;
        }

        if (flag) {
            if (token.size() > 0) {
                res.push_back(token);
                token = "";
                i += delim.size() - 1;
            }
        } else {
            token += s[i];
        }
    }

    res.push_back(token);
    
    return res;
}

std::string bufferToString(char* buffer, int buff_len)
{
    std::string ret(buffer, buff_len);

    return ret;
}

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}