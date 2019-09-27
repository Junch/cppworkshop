#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 134. Gas Station
// Web: https://leetcode.com/problems/gas-station/
///////////////////////////////////////////////////////////////////////////////

namespace leet_134
{

class Solution
{
  public:
    int canCompleteCircuit(vector<int> &gas, vector<int> &cost)
    {
        int len = gas.size();

        for (int start = 0; start < len; ++start)
        {
            int remaining = 0;
            int i = start;
            for (int j = 0; j < len; ++j, ++i)
            {
                i = i % len;
                remaining += gas[i];
                remaining -= cost[i];

                if (remaining < 0)
                {
                    break;
                }
            }

            if (remaining >= 0)
            {
                return start;
            }
        }

        return -1;
    }
};

class leet_134_test : public testing::TestWithParam<std::tuple<vector<int>, vector<int>, int>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_134_test, ::testing::Values(
    std::make_tuple(vector<int>{1, 2, 3, 4, 5}, vector<int>{3, 4, 5, 1, 2}, 3),
    std::make_tuple(vector<int>{2, 3, 4}, vector<int>{3, 4, 3}, -1)
));
// clang-format on

TEST_P(leet_134_test, test)
{
    auto a = std::get<0>(GetParam());
    auto b = std::get<1>(GetParam());
    auto c = std::get<2>(GetParam());
    Solution sln;
    auto result = sln.canCompleteCircuit(a, b);
    ASSERT_EQ(result, c);
}

} // namespace leet_134