#include <vector>
#include <string>
#include <algorithm>
#include "../../tools/tools-lib/tools.h"

using namespace std;

class Solution {
public:
    vector<vector<int>> g;
    vector<vector<int>> st;
    int n, m;

    int dx[4] = {1, 0, -1, 0};
    int dy[4] = {0, 1, 0, -1};

    vector<vector<int>> colorBorder(vector<vector<int>> &grid, int row, int col, int color) {
        g = grid;
        int n = grid.size();
        int m = grid[0].size();

        st = vector<vector<int >>(n, vector<int>(m));

        st[row][col] = 1;
        dfs(row, col);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (st[i][j] == 2) {
                    g[i][j] = color;
                }
            }
        }

        return g;
    }


    void dfs(int x, int y) {
        for (int i = 0; i < 4; ++i) {
            int a = dx[i] + x;
            int b = dy[i] + y;

            if (a >= 0 && a < g.size() && b >= 0 && b < g[0].size() && g[a][b] == g[x][y]) {
                if (st[a][b] == 0) {
                    st[a][b] = 1;
                    dfs(a, b);
                }
            } else {
                st[x][y] = 2;
            }
        }
    }
};

int main() {
    Solution s;
    vector<vector<int>> grid = {{1, 2, 2},
                                {2, 3, 2}};
    auto res = s.colorBorder(grid, 0, 1, 3);
    for_each(res.begin(), res.end(), [](auto t) {
        for_each(t.begin(), t.end(), [](auto u) { cout << u << " "; });
        cout << endl;
    });
    return 0;
}