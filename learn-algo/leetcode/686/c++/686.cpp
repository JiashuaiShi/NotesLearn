#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

class Solution {
public :
    int repeatedStringMatch(string a, string b) {
        bool cache[26] = {false};
        for (const char &c: a) {
            cache[c - 'a'] = true;
        }

        for (const char &c: b) {
            if (cache[c - 'a'] == false) {
                return -1;
            }
        }

        int k = b.size() / a.size();
        string t;
        t.reserve(b.size());

        for (int i = 0; i < k; ++i) {
            t.append(a);
        }

        for (int i = 0; i < 3; ++i) {
            if (t.find(b) != string::npos) {
                return k + 1;
            }
            t.append(a);
        }
        return -1;
    }
};

int main() {
    cout << endl;
    return 0;
}