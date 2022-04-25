#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    string truncateSentence(string s, int k) {
        vector <string> vec;
        for (int i = 0; i < s.size(); ++i) {
            int j = i;
            string t;
            while (j < s.size() && s[j] != ' ') {
                t += s[j];
                j++;
            }
            vec.push_back(t);
            i = j;
        }

        string res;
        for (int i = 0; i < vec.size() && i < k; i++) {
            res += vec[i];
            res += " ";

        }

        res.pop_back();
        return res;
    }
};