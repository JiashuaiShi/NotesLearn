// 使用lambda表达式做递归，据说效率不高
// 建议使用Y组合器，这是一种函数式编程的方法

#include <functional>  // 用于lambda进行递归
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

class Solution {
public:
    int countBattleships(vector<vector<char>> &board) {
        int res = 0;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[i].size(); ++j) {
                if (i > 0 && board[i - 1][j] == 'X') continue;
                if (j > 0 && board[i][j - 1] == 'Y') continue;
                if (board[i][j] == 'X')
                    res++;
            }
        }
        return res;
    }

//    int countBattleships(vector<vector<char>> &board) {
//        vector<int> dx = {0, 1, 0, -1};
//        vector<int> dy = {1, 0, -1, 0};
//
//        int r = board.size();
//        int c = board[0].size();
//        int ans = 0;
//
//        vector<vector<int>> st = vector<vector<int>>(r, vector<int>(c));
//
//        auto check = [&](int x, int y) -> bool {
//            if (x - 1 < 0 || y + 1 >= c || x + 1 >= r || y - 1 < 0) {
//                return true;
//            }
//
//            if (y - 1 >= 0 && board[x][y - 1] != 'X') {
//                return true;
//            }
//
//            if (y + 1 <= c - 1 && board[x][y + 1] != 'X') {
//                return true;
//            }
//
//            if (x - 1 >= 0 && board[x - 1][y] != 'X') {
//                return true;
//            }
//
//            if (x + 1 <= r - 1 && board[x + 1][y] != 'X') {
//                return true;
//            }
//
//            return false;
//        };
//
//        function<void(int, int, int)> dfs = [&](int x, int y, int u) -> void {
//            if (st[x][y] != 0) {
//                return;
//            }
//
//            st[x][y] = 1;
//
//            if (check(x, y)) {
//                ans = u + 1;
//            }
//
//            for (int i = 0; i < 4; ++i) {
//                int a = x + dx[i];
//                int b = y + dy[i];
//
//                if (!(a < 0 || b >= c || a >= r || b < 0)) {
//                    if (board[a][b] == 'X') {
//                        dfs(a, b, u);
//                    }
//                }
//            }
//        };
//
//        for (auto i = 0; i < r; ++i) {
//            for (auto j = 0; j < c; ++j) {
//                if (board[i][j] != 'X' || st[i][j] != 0) {
//                    continue;
//                }
//                dfs(i, j, ans);
//            }
//        }
//
//        return ans;
//    }
};


int main() {
    vector<vector<char>> board = {
            {'X', '.'},
            {'.', 'X'}
    };

    Solution s;
    auto res = s.countBattleships(board);
    cout << res << endl;
    return 0;
}