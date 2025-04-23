#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

using namespace std;
class Solution {
public:
    int largestSumAfterKNegations(vector<int>& nums, int k) {
        // 先求负数的个数和绝对值最小数
        int minusCount = 0;
        int minV = 101;
        int res = 0;

        vector<int> z;
        vector<int> f;

        for(auto i : nums) {
            if(i < 0) {
                minusCount++;
                f.push_back(i);
            } else {
                z.push_back(i);
            }

            minV = minV > abs(i) ? abs(i) : minV;
        }

        // k <=  minusCount
        if (k <= minusCount) {
            sort(f.begin(), f.end());
            int u = k;
            for(int i = 0; i < u; ++i) {
                f[i] = -f[i];
            }

            int t1 = accumulate(f.begin(), f.end(), 0);
            int t2 = accumulate(z.begin(), z.end(), 0);
            res = t1 + t2;
        }

        // k > minusCount
        if(k > minusCount) {
            int fs = f.size();
            k -= fs;

            if( k % 2 == 0) {
                res = accumulate(z.begin(), z.end(), 0) - accumulate(f.begin(), f.end(), 0);
            } else {
                res = accumulate(z.begin(), z.end(), 0) - accumulate(f.begin(), f.end(), 0);
                res -= 2 * minV;
            }

        }
        return res;
    }
};

int main() {
    vector<int> in = {};
    Solution s;
    auto res = s.largestSumAfterKNegations(in, 29);
    cout << res << endl;
    return 0;
}