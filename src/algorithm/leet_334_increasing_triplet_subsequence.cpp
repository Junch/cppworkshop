#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 334. Increasing Triplet Subsequence
// Web: https://leetcode.com/problems/increasing-triplet-subsequence/
///////////////////////////////////////////////////////////////////////////////

namespace leet_334
{

class Solution
{
  public:
    bool increasingTriplet(vector<int> & /*nums*/) { return false; }
};

class leet_334_test : public testing::TestWithParam<std::tuple<vector<int>, bool>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_334_test, ::testing::Values(
    std::make_tuple(vector<int>{1, 2, 3, 4, 5}, true),
    std::make_tuple(vector<int>{5, 4, 3, 2, 1}, false)
));
// clang-format on

TEST_P(leet_334_test, test)
{
    auto arr = std::get<0>(GetParam());
    auto exist = std::get<1>(GetParam());
    Solution sln;
    ASSERT_EQ(sln.increasingTriplet(arr), exist);
}

} // namespace leet_334
