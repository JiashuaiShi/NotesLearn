#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution {
public:
    vector<int> ans;
    vector<int> w;
    vector<vector<int>> g;

    void dfs(int i) {
        if(ans[i] != -1) {
            return;
        }

        ans[i] = i;

        for(auto &e : g[i]) {
            dfs(e);
            if(w[ans[e]] < w[ans[i]]) {
                ans[i] = ans[e];
            }
        }
    }

    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        g.resize(n);
        ans.resize(n, -1);
        w= quiet;

        for(auto &e: richer) {
            auto a = e[0];
            auto b = e[1];

            g[b].push_back(a);
        }


        for(int i = 0; i < n; i++) {
            dfs(i);
        }

        return ans;
    }
};