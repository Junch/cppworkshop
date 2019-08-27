#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 350. Intersection of Two Arrays II
// Web: https://leetcode.com/problems/intersection-of-two-arrays-ii/
///////////////////////////////////////////////////////////////////////////////

namespace leet_350
{

class Solution
{
  public:
    vector<int> intersect(vector<int> &nums1, vector<int> &nums2)
    {
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());

        size_t len1 = nums1.size();
        size_t len2 = nums2.size();

        vector<int> result;

        for (size_t i = 0, j = 0; i < len1 && j < len2;)
        {
            if (nums1[i] == nums2[j])
            {
                result.push_back(nums1[i]);
                ++i;
                ++j;
            }
            else if (nums1[i] < nums2[j])
            {
                ++i;
            }
            else
            {
                ++j;
            }
        }

        return result;
    }
};

class leet_350_test : public testing::TestWithParam<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_350_test, ::testing::Values(
    std::make_tuple(std::vector<int>{1, 2, 2, 1}, std::vector<int>{2, 2}, std::vector<int>{2, 2}),
    std::make_tuple(std::vector<int>{4, 9, 5}, std::vector<int>{9, 4, 9, 8, 4}, std::vector<int>{4, 9})
));
// clang-format on

TEST_P(leet_350_test, test)
{
    auto a = std::get<0>(GetParam());
    auto b = std::get<1>(GetParam());
    auto c = std::get<2>(GetParam());
    Solution sln;
    auto v = sln.intersect(a, b);
    ASSERT_EQ(v, c);
}

} // namespace leet_350