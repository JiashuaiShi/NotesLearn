#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    vector<int> maxSumOfThreeSubarrays(vector<int> &nums, int k) {
        int n = nums.size();
        vector<int> s(n + 1);
        for (auto i = 1; i <= n; ++i) {
            s[i] = s[i - 1] + nums[i - 1];
        }

        int x = n + 1, y = 3;
        vector<vector<int>> f(n + 2, vector<int>(4));
        for (int i = n - k + 1; i > 0; --i) {
            for (int j = 1; j <= 3; ++j) {
                f[i][j] = max(f[i + 1][j],
                              f[i + k][j - 1] + s[i + k - 1] - s[i - 1]); // s[i+n+1] - s[i] 表示下标从i到i+n的所有数的和
            }
            if (f[x][3] <= f[i][3]) {
                x = i;
            }
        }

        vector<int> res;
        while (y > 0) {
            while (f[x][y] != f[x + k][y - 1] + s[x + k - 1] - s[x - 1]) { // x到x+k-1的所有的和
                x++;
            }

            res.push_back(x - 1);
            x += k;
            y--;
        }

        return res;
    }
};

int main() {
    Solution s;
    vector<int> in = {1, 2, 1, 2, 6, 7, 5, 1};
    int k = 2;
    auto res = s.maxSumOfThreeSubarrays(in, k);
    for_each(res.begin(), res.end(), [](auto t) { cout << t << " "; });
    cout << endl;
    return 0;
}