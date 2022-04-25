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
    string modifyString(string s) {
        int arr[3] = {'a', 'b', 'c'};
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '?') {
                for (int j = 0; j < 3; j++) {
                    if ((i > 0 && arr[j] == s[i - 1]) || (j < s.size() && arr[j] == s[i + 1])) {
                        continue;
                    }
                    s[i] = arr[j];
                    break;
                }
            }
        }

        return s;
    }
};

int main() {
    cout << endl;
    return 0;
}