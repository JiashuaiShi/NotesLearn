#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    int mod = 1337;

    int computePow(int a, int b) {
        long long res = 1;
        while (b--) {
            res = res * a % mod;
        }
        return res;
    }

    int dfs(int a, vector<int> b, int u) {
        if (u == -1) {
            return 1;
        }
        auto t1 = computePow(dfs(a, b, u - 1), 10) % mod;
        auto t2 = computePow(a, b[u]) % mod;
        return t1 * t2 % mod;
    }

    int superPow(int a, vector<int> &b) {
        auto res = dfs(a, b, b.size() - 1);
        return res;
    }
};

int main() {
    Solution s;
    int a = 2;
    vector<int> b = {3};
    auto res = s.superPow(a, b);
    cout << res << endl;
}