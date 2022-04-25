//
// Created by ryder on 2021/12/9.
//

#include "tools-lib/tools.h"

int main() {
    vector<int> in = {1, 2, 3};
    printC(in);

    string s1 = "abcdefG";
    string s2 = "ABCDEFg";

    strToUpper(s1);
    strToLower(s2);

    cout << s1 << endl;
    cout << s2 << endl;

    return 0;
}