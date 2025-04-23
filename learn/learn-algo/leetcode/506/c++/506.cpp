#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../../tools/tools-lib/tools.h"

using namespace std;

class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& score) {
        int n = score.size();
        vector<pair<int,int>> vec;
        for(int i = 0; i < n; ++i) {
            pair<int, int> t = pair<int,int>(score[i], i);
            vec.push_back(t);
        }

        sort(vec.begin(), vec.end(), [](auto a, auto b){return a.first > b.first;});

        vector<string> t = {"Gold Medal","Silver Medal","Bronze Medal"};

        vector<string> res(n);
        for(int i = 0; i < score.size(); ++i) {
            if(i < 3) {
                res[vec[i].second] = t[i];
            } else {
                res[vec[i].second] = to_string(i);
            }

        }

        return res;
    }
};

int main() {
    Solution s;
    vector<int> in = {10,3,8,9,4};
    auto res = s.findRelativeRanks(in);
    printC(res);
    return 0;
}