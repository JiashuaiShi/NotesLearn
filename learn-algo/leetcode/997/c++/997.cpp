#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>

using namespace std;

//class Solution {
//public:
//    int findJudge(int n, vector<vector<int>>& trust) {
//        map<int, int> tMap;
//        for(int i = 1; i <= n; ++i) {
//            tMap.insert({i,1});
//        }
//
//        for(int i = 1; i <= n; i++) {
//            auto t = trust[i];
//            tMap[t[1]]++;
//        }
//
//        for(int i = 0; i < trust.size(); i++) {
//            auto t = trust[i];
//            tMap.erase(t[0]);
//        }
//
//        int res = -1;
//
//
//        for(auto t : tMap) {
//            if(t.second == n) {
//                res = t.first;
//            }
//        }
//        return res;
//    }
//};


class Solution {
public:
    int findJudge(int n, vector<vector<int>>& trust) {
        map<int, int> tMap;

        for(int i = 1; i <= n; ++i) {
            tMap.insert({i,1});
        }

        for(int i = 0; i < trust.size(); i++) {
            auto t = trust[i];
            tMap[t[1]]++;
        }

        for(int i = 0; i < trust.size(); i++) {
            auto t = trust[i];
            tMap.erase(t[0]);
        }

        int res = -1;


        for(auto t : tMap) {
            if(t.second == n-1) {
                res = t.first;
            }
        }
        return res;
    }
};