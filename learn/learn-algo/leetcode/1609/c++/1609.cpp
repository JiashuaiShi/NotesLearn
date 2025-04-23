#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


// INT_MAX 2147483647 10位数

typedef long long LL;

class Solution {
public:
    bool isOdd(LL n) {
        return n & 1;
    }

    bool isEven(LL n) {
        return !(n & 1);
    }

    bool isEvenOddTree(TreeNode *root) {
        queue<TreeNode *> q;

        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int n = q.size();
            int preV = isOdd(level) ? INT_MAX : INT_MIN;
            while (n) {
                TreeNode *t = q.front();
                q.pop();

                if (isOdd(level)) {
                    if (!isEven(t->val) || t->val >= preV) {
                        cout << "l, v: " << level << " " << t->val << " odd" << endl;
                        return false;
                    }
                    preV = t->val;
                } else {
                    if (!isOdd(t->val) || t->val <= preV) {
                        cout << "l, v: " << level << " "<< t->val << " even" << endl;
                        return false;
                    }
                    preV = t->val;
                }

                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
                n--;
            }
            level++;
        }
        return true;
    }
};


int main() {
    cout << endl;
    return 0;
}