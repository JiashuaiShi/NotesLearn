#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

class Solution {
public:
    int numWaterBottles(int numBottles, int numExchange) {
        int res = numBottles;
        int left = numBottles;
        while(left >= numExchange) {
            int t = left / numExchange;
            int r = left % numExchange;
            left = r + t;
            res += t;
        }
        return res;
    }
};