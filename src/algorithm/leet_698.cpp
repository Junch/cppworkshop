#include <algorithm>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

using namespace std;

namespace leet_698
{

class Solution
{
  public:
    bool canPartitionKSubsets(vector<int> &nums, int k)
    {
        // https://www.youtube.com/watch?v=FQ7du5dpgb8

        int sum = accumulate(begin(nums), end(nums), 0);
        if (sum % k != 0)
            return false;
        int n = nums.size();
        int array_sum = sum / k;
        vector<bool> used(n, false);
        sort(begin(nums), end(nums));

        function<bool(int, int, int)> dfs = [&](int index, int group, int sum) -> bool
        {
            if (group == k)
                return true;
            if (sum == array_sum)
                return dfs(0, group + 1, 0);

            for (int i = index; i < n; ++i)
            {
                if (used[i])
                    continue;
                if (i > index && nums[i] == nums[i - 1] && !used[i - 1])
                    continue;
                if (sum + nums[i] > array_sum)
                    break;

                used[i] = true;
                if (dfs(index + 1, group, sum + nums[i]))
                    return true;
                used[i] = false;
            }

            return false;
        };

        return dfs(0, 0, 0);
    }
};

TEST(leet_698, test)
{
    vector<int> nums{4, 3, 2, 3, 5, 2, 1};
    Solution sln;
    auto ans = sln.canPartitionKSubsets(nums, 4);
    ASSERT_TRUE(ans);
}

} // namespace leet_698