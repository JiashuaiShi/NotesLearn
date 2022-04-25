#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <sstream>


using namespace std;

class Solution {
public:
    int dayOfYear(string date) {
        tm t{};
        t.tm_year = stoi(date.substr(0, 4)) - 1900;
        t.tm_mon = stoi(date.substr(5, 2)) - 1;
        t.tm_mday = stoi(date.substr(8, 2));
        mktime(&t);
        return t.tm_yday + 1;
    }

    int dayOfYear2(string date) {
        tm t{};
        istringstream ss(date);
        ss >> get_time(&t, "%Y-%m-%d");
        mktime(&t);
        return t.tm_yday + 1;
    }
};


int main() {
    Solution s;
    auto res = s.dayOfYear2("2019-01-09");
    cout << res << endl;
    return 0;
}