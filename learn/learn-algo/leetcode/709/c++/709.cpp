#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    string toLowerCase(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
};

int main() {
    Solution s;
    vector<int> in = {1, 2, 1, 2, 6, 7, 5, 1};
    string str = "Hello";
    auto res = s.toLowerCase(str);
    for_each(res.begin(), res.end(), [](auto t) { cout << t ; });
    cout << endl;
    return 0;
}