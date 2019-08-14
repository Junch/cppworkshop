#include "treenode.h"
#include <gtest/gtest.h>
#include <stack>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Title: 二叉树的非递归遍历
// web: https://www.cnblogs.com/dolphin0520/archive/2011/08/25/2153720.html
///////////////////////////////////////////////////////////////////////////////

std::vector<int> pre_order(TreeNode *root)
{
    std::vector<int> v;
    std::stack<TreeNode *> s;
    TreeNode *p = root;

    while (p != nullptr || !s.empty())
    {
        while (p != nullptr)
        {
            v.push_back(p->val);
            s.push(p);
            p = p->left;
        }

        if (!s.empty())
        {
            p = s.top();
            s.pop();
            p = p->right;
        }
    }

    return v;
}

std::vector<int> in_order(TreeNode *root)
{
    std::vector<int> v;
    std::stack<TreeNode *> s;
    TreeNode *p = root;

    while (p != nullptr || !s.empty())
    {
        while (p != nullptr)
        {
            s.push(p);
            p = p->left;
        }

        if (!s.empty())
        {
            p = s.top();
            v.push_back(p->val);
            s.pop();
            p = p->right;
        }
    }

    return v;
}

///////////////////////////////////////////////////////////////////////////////
// Title: Construct a complete binary tree from given array in level order fashion
// Web: https://www.geeksforgeeks.org/construct-complete-binary-tree-given-array/
///////////////////////////////////////////////////////////////////////////////

TreeNode *insert_level_order(const std::vector<int> &v, size_t index)
{
    TreeNode *p = nullptr;
    if (index < v.size() && v[index] != 0)
    {
        p = new TreeNode(v[index]);
    }

    if (p)
    {
        p->left = insert_level_order(v, 2 * index + 1);
        p->right = insert_level_order(v, 2 * index + 2);
    }

    return p;
}

TreeNode *construct(const std::vector<int> &v)
{
    return insert_level_order(v, 0);
}

///////////////////////////////////////////////////////////////////////////////

class tree_node_test : public testing::TestWithParam<std::vector<int>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, tree_node_test, ::testing::Values(
    std::vector<int>{},
    std::vector<int>{1, 2, 3, 4, 5, 6},
    std::vector<int>{1, 2, 3, 0, 5, 6}
));
// clang-format on

TEST_P(tree_node_test, construct)
{
    auto v = GetParam();
    TreeNode *tree = construct(v);
    auto result = in_order(tree);
    for (auto i : result)
    {
        std::cout << i << ' ';
    }

    std::cout << '\n';
}
