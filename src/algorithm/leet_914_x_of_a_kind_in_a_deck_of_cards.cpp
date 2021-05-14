#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 914. X of a Kind in a Deck of Cards
// Web: https://leetcode.com/problems/x-of-a-kind-in-a-deck-of-cards/
//      https://zxi.mytechroad.com/blog/math/leetcode-914-x-of-a-kind-in-a-deck-of-cards/
//      https://leetcode.com/problems/x-of-a-kind-in-a-deck-of-cards/discuss/338479/c%2B%2B-solution
///////////////////////////////////////////////////////////////////////////////

namespace leet_914
{

class Solution
{
  public:
    int gcd(int a, int b)
    {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }

    bool hasGroupsSizeX(vector<int> &deck)
    {
        if (deck.size() <= 1)
            return false;

        std::sort(deck.begin(), deck.end());

        std::vector<int> counts;
        int val = deck[0];
        int count = 0;
        for (size_t i = 0, len = deck.size(); i < len; ++i)
        {
            if (deck[i] == val)
            {
                ++count;
            }
            else
            {
                counts.push_back(count);

                val = deck[i];
                count = 1;
            }
        }

        counts.push_back(count);

        int num = counts[0];
        for (size_t i = 1, len = counts.size(); i < len; ++i)
        {
            if (counts[i] < 2)
            {
                return false;
            };

            num = gcd(num, counts[i]);
        };

        return num >= 2;
    }

    bool hasGroupsSizeX_optimised(vector<int> &deck)
    {
        unordered_map<int, int> counts;
        for (int card : deck)
        {
            ++counts[card];
        }

        int X = (int)deck.size();
        for (const auto& p : counts)
        {
            X = gcd(X, p.second);
        }

        return X >= 2;
    }
};

class leet_914_test : public testing::TestWithParam<std::pair<std::vector<int>, bool>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_914_test, ::testing::Values(
    std::make_pair(std::vector<int>{1}, false),
    std::make_pair(std::vector<int>{1, 1}, true),
    std::make_pair(std::vector<int>{1, 2, 3, 4, 4, 3, 2, 1}, true),
    std::make_pair(std::vector<int>{1, 1, 1, 2, 2, 2, 3, 3}, false),
    std::make_pair(std::vector<int>{1, 1, 1, 1, 2, 2, 2, 2, 2, 2}, true)
));
// clang-format on

TEST_P(leet_914_test, initial)
{
    auto v = GetParam().first;
    Solution sln;
    ASSERT_EQ(sln.hasGroupsSizeX(v), GetParam().second);
}

TEST_P(leet_914_test, optimised)
{
    auto v = GetParam().first;
    Solution sln;
    ASSERT_EQ(sln.hasGroupsSizeX_optimised(v), GetParam().second);
}

} // namespace leet_914