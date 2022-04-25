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
    int mostFrequent(vector<int>& nums, int key) {
        set<int> arr;

        map<int, int> tMap;
        for(auto x : nums) {
            tMap[x]++;
        }

        for(int i = 0; i < nums.size(); i++) {
            if(nums[i] == key && i < nums.size()-1) {
                arr.insert(nums[i+1]);
            }
        }

        int ans = 0;
        int cnt = 0;

        for(auto x : arr) {
            if(tMap[x] > cnt) {
                ans = x;
                cnt = tMap[x];
            }
        }

        return ans;

    }
};