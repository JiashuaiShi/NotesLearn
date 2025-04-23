#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    bool canConstruct(string r, string m) {
        unordered_map<char, int> t;
        bool res = true;
        for (auto c: m) {
            t[c]++;
        }

        for (auto c: r) {
            if (--t[c] < 0) {
                res = false;
            }
        }

        return res;
    }
};

int main() {
    auto r = "abcd";
    auto m = "abcde";

    Solution s;
    auto res = s.canConstruct(r, m);
    cout << res << endl;
}


