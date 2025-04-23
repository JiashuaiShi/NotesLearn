#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <queue>

using namespace std;

class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        sort(courses.begin(), courses.end(), [](auto a, auto b) { return a[1] < b[1];});
        priority_queue<int> heap;  // 默认是大顶堆
        int cnt = 0;
        for(auto & c : courses) {
            cnt += c[0];
            heap.push(c[0]);
            cout << "push";

            if(cnt > c[1]) {
                cnt -= heap.top();
                heap.pop();
            }
        }

        return heap.size();
    }
};


int main() {

//    Solution s;
//    vector<vector<int>> in = {{5, 5}, {4, 6}, {2, 6}};
//    auto res = s.scheduleCourse(in);
//    cout << res << endl;

    priority_queue<int> heap;  // 默认是大顶堆
    heap.push(5);
    heap.push(45);
    heap.push(10);
    heap.push(1);
    heap.push(100);
    heap.push(200);
    heap.push(500);
    heap.push(360);

    
    return 0;
}