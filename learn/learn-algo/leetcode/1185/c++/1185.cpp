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
    vector <string> arr = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    vector<int> monthDays = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

    string dayOfTheWeek(int day, int month, int year) {
        // 19701970 年 1212 月 3131 日是星期四

        auto daysOfYear = (year - 1971) * 365 + (year - 1969) / 4;
        auto dayOfMonth = 0;


        for (int i = 1; i < month; i++) {
            dayOfMonth += monthDays[i];
        }

        if (((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) && month >= 3) {
            dayOfMonth += 1;
        }

        auto index = (dayOfMonth + daysOfYear + day + 3) % 7;

        return arr[index];
    }
};


int main() {
    cout << endl;
    return 0;
}