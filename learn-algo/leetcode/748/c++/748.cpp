#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string> &words) {
        // 统一格式化为小写
        transform(licensePlate.begin(), licensePlate.end(), licensePlate.begin(), ::tolower);

        unordered_map<char, int> kMap;
        for_each(licensePlate.begin(), licensePlate.end(), [&kMap](char c) { if (isalpha(c)) kMap[c]++; });

//        for_each(kMap.begin(), kMap.end(), [](auto t){cout << t.first << " " << t.second << endl; });

        int minLenIndex = words.size() - 1;
        int minLen = INT_MAX;

        for (auto i = 0; i < words.size(); i++) {
            unordered_map<char, int> tMap;
            bool isSuccess = true;
            auto word = words[i];
            for_each(word.begin(), word.end(), [&tMap](char c) { if (isalpha(c)) tMap[c]++; });
            for (auto v: kMap) {
                if (tMap[v.first] < v.second) {
                    isSuccess = false;
                    break;
                }
            }

            if (!isSuccess) {
                continue;
            }

            if (word.size() < minLen) {
                minLenIndex = i;
                minLen = word.size();
            }
        }

        return words[minLenIndex];

    }
};

int main() {
    Solution s;
    auto licensePlate = "1s3 PSt";
    vector<string> words = {"step", "steps", "stripe", "stepple"};

    auto res = s.shortestCompletingWord(licensePlate, words);
    cout << res << endl;
    return 0;
}