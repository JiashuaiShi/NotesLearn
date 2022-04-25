#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>

using namespace std;

class Solution {
public:
    vector<int> sortJumbled(vector<int> &mapping, vector<int> &nums) {
        vector<pair<int, int>> target;

        auto convert = [&](int x) -> int {
            string a = to_string(x);
            string b;
            for (auto c: a) {
                b += mapping[(c - '0')] + '0';
            }
            return atoi(b.c_str());
        };

        for (int i = 0; i < nums.size(); i++) {
            target.push_back(pair<int, int>{convert(nums[i]), i});
        }

        sort(target.begin(), target.end(), [](pair<int, int> a, pair<int, int> b) {
            return a.first < b.first;
        });

        vector<int> ans;

        for(int i = 0; i < target.size(); i++) {
            ans.push_back(nums[target[i].second]);
        }

        return ans;
    }
};


int main() {
    Solution s;
    vector<int> in = {8,9,4,0,2,1,3,5,7,6};
    vector<int> nums = {991,338,38};

    auto res = s.sortJumbled(in, nums);

    for_each(res.begin(), res.end(), [](auto a) { cout << a << " "; });
    cout << endl;

    return 0;
}
