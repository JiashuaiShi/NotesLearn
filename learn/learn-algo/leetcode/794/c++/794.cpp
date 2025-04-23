#include <iostream>
#include <vector>
#include <string>


using namespace std;

class Solution {
public:
    vector<string> g;
    int XCount = 0;
    int OCount = 0;
    int r = 0;
    int c = 0;

    void getCharCount() {
        for (int i = 0; i < r; ++i) {
            for (int j = 0; j < c; ++j) {
                if (g[i][j] == 'X') {
                    XCount++;
                }
                if (g[i][j] == 'O') {
                    OCount++;
                }
            }
        }
    }

    bool checkRes(char c) {
        for (int i = 0; i < 3; ++i) {
            if (g[i][0] == c && g[i][1] == c && g[i][2] == c) {
                return true;
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (g[0][i] == c && g[1][i] == c && g[2][i] == c) {
                return true;
            }
        }

        if (g[0][0] == c && g[1][1] == c && g[2][2] == c) {
            return true;
        }

        if (g[2][0] == c && g[1][1] == c && g[0][2] == c) {
            return true;
        }

        return false;
    }

    bool validTicTacToe(vector<string> &board) {
        g = board;
        r = g.size();
        c = g[0].size();

        bool resX = checkRes('X');
        bool resO = checkRes('O');
        getCharCount();

        if (resX && resO) {
            return false;
        }

        if (resX && XCount != OCount + 1) {
            return false;
        }

        if (resO && XCount != OCount) {
            return false;
        }

        if (XCount != OCount && XCount != OCount + 1) {
            return false;
        }

        return true;
    }
};

int main() {
    Solution s;
    vector<string> in = {"O  ", "   ", "   "};
    auto res = s.validTicTacToe(in);
    cout << res << endl;
}