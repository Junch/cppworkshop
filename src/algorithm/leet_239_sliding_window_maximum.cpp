#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 239. Sliding Window Maximum
// Web: https://leetcode.com/problems/sliding-window-maximum/
///////////////////////////////////////////////////////////////////////////////

namespace leet_239
{

class Solution
{
  public:
    vector<int> maxSlidingWindow(vector<int> & /*nums*/, int /*k*/)
    {
        vector<int> a;
        return a;
    }
};

class leet_239_test : public testing::TestWithParam<std::tuple<vector<int>, int, vector<int>>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_239_test, ::testing::Values(
    std::make_tuple(vector<int>{1, 3, -1, -3, 5, 3, 6, 7}, 3, vector<int>{3, 4, 5, 5, 6, 7})
));
// clang-format on

TEST_P(leet_239_test, test)
{
    auto nums = std::get<0>(GetParam());
    int k = std::get<1>(GetParam());
    auto result = std::get<2>(GetParam());
    Solution sln;
    auto v = sln.maxSlidingWindow(nums, k);
    ASSERT_EQ(result, v);
}

} // namespace leet_239
