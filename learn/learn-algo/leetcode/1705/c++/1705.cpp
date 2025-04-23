#include <iostream>
#include <queue>
#include <vector>
#include <numeric>

using namespace std;

typedef pair<int, int> pii;

class Solution {
public:
    int eatenApples(vector<int> &apples, vector<int> &days) {
        int n = apples.size(), ans = 0, i = 0;
        priority_queue<pii, vector<pii>, greater<pii>> pq;

        while (i < n) {
            while (!pq.empty() && pq.top().first <= i) {
                pq.pop();
            }

            int lastDay = days[i] + i;
            int cnt = apples[i];

            if (cnt > 0) {
                pq.emplace(lastDay, cnt);
            }

            if (!pq.empty()) {
                pii cur = pq.top();
                cur.second--;
                ans++;
                if (cur.second > 0) {
                    pq.emplace(cur);
                }
            }
            i++;
        }

        while(!pq.empty()) {
            while(!pq.empty() && pq.top().first <= i) {
                pq.pop();
            }

            if(pq.empty()) {
                break;
            }

            pii cur = pq.top();
            pq.pop();
            int cnt = min(cur.first-i, cur.second);
            ans += cnt;
            i += cnt;
        }

        return ans;
    }
};