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