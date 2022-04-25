#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

class Solution {
public:
    int countQuadruplets(vector<int> &nums) {
        int n = nums.size();
        int ans = 0;
        unordered_map<int, int> tMap;
        for (int b = n - 3; b >= 1; b--) {
            for (int d = n - 1; d > b + 1; d--) {
                int c = b + 1;
                int t = nums[d] - nums[c];
                tMap[t]++;
            }

            for (int a = 0; a < b; a++) {
                ans += tMap[nums[a] + nums[b]];
            }
        }

        return ans;
    }
};

int main() {
    cout << endl;
    return 0;
}