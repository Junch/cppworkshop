#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 218. The Skyline Problem
// Web: https://leetcode.com/problems/the-skyline-problem/
///////////////////////////////////////////////////////////////////////////////

namespace leet_218
{

class Solution
{
  public:
    vector<vector<int>> getSkyline(vector<vector<int>>& /*buildings*/) {
        vector<vector<int>> a;
        return a;
    }
};

class leet_218_test : public testing::TestWithParam<std::tuple<vector<vector<int>>, vector<vector<int>>>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_218_test, ::testing::Values(
    std::make_tuple(vector<vector<int>>{{2, 9, 10}, {3, 7, 15}, {5, 12, 12}, {15, 20, 10}, {19, 24, 8}}, 
        vector<vector<int>>{{2, 10}, {3, 15}, {7, 12}, {12, 0}, {15, 10}, {20, 8}, {24, 0}}
    )
));
// clang-format on

TEST_P(leet_218_test, DISABLED_test)
{
    auto buildings = std::get<0>(GetParam());
    auto skyline = std::get<1>(GetParam());
    Solution sln;
    ASSERT_EQ(sln.getSkyline(buildings), skyline);
}

} // namespace leet_334
