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
    vector<int> findEvenNumbers(vector<int> &a) {
        vector<int> ans;
        map<int, int> dup;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] == 0) continue;
            for (int j = 0; j < a.size(); j++) {
                if (i == j) continue;

                for (int k = 0; k < a.size(); k++) {
                    if (i == k || j == k) continue;

                    if (a[k] % 2 == 1) continue;

                    int x = a[i] * 100 + a[j] * 10 + a[k];

                    if (dup[x] == 1) continue;

                    ans.push_back(x);
                    dup[x] = 1;
                }
            }
        }

        sort(ans.begin(), ans.end());
        return ans;
    }
};

int main() {
    Solution s;
    vector<int> in = {2, 1, 3, 0};
    auto res = s.findEvenNumbers(in);

    for_each(res.begin(), res.end(), [](auto a) { cout << a << " "; });
    cout << endl;

    return 0;
}
