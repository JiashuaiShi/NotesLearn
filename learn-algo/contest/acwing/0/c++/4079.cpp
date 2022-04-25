#include <iostream>
#include <string>

using namespace std;


string initString(int n) {
    string res;
    for(auto i = 0; i <= n; ++i) {
        res += to_string(i);
    }
    return res;
}


int main() {
    string s = initString(1000);
    int cnt;

    cin >> cnt;

    while(cnt--) {
        int i= 0;
        cin >> i;
        cout << s[i] << endl;
    }

    return 0;
}
