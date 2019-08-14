#include "treenode.h"
#include <gtest/gtest.h>
#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// 654. Maximum Binary Tree
// Web: https://leetcode.com/problems/maximum-binary-tree/
///////////////////////////////////////////////////////////////////////////////

namespace leet_654
{

class Solution
{
  public:
    TreeNode *construct(vector<int>::iterator begin, vector<int>::iterator end)
    {
        auto iter = std::max_element(begin, end);
        TreeNode *p = new TreeNode(*iter);

        if (iter != begin)
        {
            p->left = construct(begin, iter);
        }

        if (iter + 1 != end)
        {
            p->right = construct(iter + 1, end);
        }

        return p;
    }

    TreeNode *constructMaximumBinaryTree(vector<int> &nums) { return construct(nums.begin(), nums.end()); }
};

TEST(leet_654, simple)
{
    Solution sln;
    vector<int> v{3, 2, 1, 6, 0, 5};
    TreeNode *tree = sln.constructMaximumBinaryTree(v);
    auto result = pre_order(tree);
    for (auto i : result)
    {
        std::cout << i << ' ';
    }

    std::cout << '\n';
}

} // namespace leet_654
