#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 15. 3Sum
// Web: https://leetcode.com/problems/3sum/
///////////////////////////////////////////////////////////////////////////////

namespace leet_15
{

class Solution
{
  public:
    vector<vector<int>> threeSum(vector<int>& nums)
    {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        if (nums.empty() || nums.front() > 0 || nums.back() < 0) return res;

        for (int k=0; k < (int)nums.size() - 2; ++k)
        {
            if (nums[k] > 0) break;
            if (k > 0 && nums[k] == nums[k-1]) continue;

            int target = -nums[k], i = k + 1, j = (int)nums.size() - 1;

            while(i < j)
            {
                int sum = nums[i] + nums[j];
                if (sum == target)
                {
                    res.push_back({nums[k], nums[i], nums[j]});
                    while(i<j && nums[i] == nums[i+1]) ++i;
                    while(i<j && nums[j] == nums[j-1]) --j;
                    ++i;
                    --j;
                }
                else if(sum > target)
                {
                    --j;
                }
                else
                {
                    ++i;
                }
            }
        }

        return res;
    }
};

class leet_15_test : public testing::TestWithParam<std::tuple<vector<int>, vector<vector<int>>>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_15_test, ::testing::Values(
    std::make_tuple(vector<int>{-1, 0, 1, 2, -1, -4}, vector<vector<int>>{{-1, -1, 2}, {-1, 0, 1}})
));
// clang-format on

TEST_P(leet_15_test, test)
{
    auto a = std::get<0>(GetParam());
    auto b = std::get<1>(GetParam());
    Solution sln;
    auto result = sln.threeSum(a);
    ASSERT_EQ(b, result);
}

} // namespace leet_15