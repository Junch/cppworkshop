#pragma once
#include <vector>

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

std::vector<int> in_order(TreeNode *root);
std::vector<int> pre_order(TreeNode *root);
TreeNode *construct(const std::vector<int> &v);
