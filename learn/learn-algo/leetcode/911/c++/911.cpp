#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

class TopVotedCandidate {
public:
    vector<int> winnerIdByTime;
    vector<int> timeArr;

    TopVotedCandidate(vector<int> &persons, vector<int> &times) {
        int n = persons.size();
        vector<int> scores;
        timeArr = times;

        int scoreMax = 0;
        int winnerId = 0;

        scores.resize(n);

        for (int i = 0; i < n; ++i) {
            int id = persons[i];
            scores[id]++;

            if (scores[id] >= scoreMax) {
                scoreMax = scores[id];
                winnerId = id;

            }
            winnerIdByTime.push_back(winnerId);
        }
    }

    int q(int t) {
        int id = upper_bound(timeArr.begin(), timeArr.end(), t) - timeArr.begin() - 1;
        return winnerIdByTime[id];
    }
};

/**
 * Your TopVotedCandidate object will be instantiated and called as such:
 * TopVotedCandidate* obj = new TopVotedCandidate(persons, times);
 * int param_1 = obj->q(t);
 */

int main() {
    vector<int> persons = {0, 0, 0, 0, 1};
    vector<int> times = {0, 6, 39, 52, 75};
    vector<int> queries = {45, 49, 59, 68, 42, 37, 99, 26, 78, 43};

    TopVotedCandidate *obj = new TopVotedCandidate(persons, times);

    for_each(obj->timeArr.begin(), obj->timeArr.end(), [](auto t) { cout << t << " "; });
    cout << endl;


    for (auto i = 0; i < obj->winnerIdByTime.size(); i++) {
        cout << obj->winnerIdByTime[i] << " ";
    }
    cout << endl;

    for (auto q: queries) {
        auto res = obj->q(q);
        cout << res << endl;
    }

    return 0;
}