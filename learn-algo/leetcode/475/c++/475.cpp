#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

class Solution {
public:
    int findRadius(vector<int> &houses, vector<int> &heaters) {
        int res = 0;
        sort(heaters.begin(), heaters.end());
        sort(houses.begin(), houses.end());

        for (int i = 0; i < houses.size(); i++) {
            auto right = lower_bound(heaters.begin(), heaters.end(), houses[i]);
            auto left = right;

            int r = right == heaters.end() ? INT_MAX : *right - houses[i];
            int l = left == heaters.begin() ? INT_MAX : houses[i] - *(left - 1);
            res = max(res, min(l, r));
        }
        return res;
    }
};

int main() {
    cout << endl;
    return 0;
}