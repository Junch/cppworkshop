#include <deque>
#include <gtest/gtest.h>
#include <vector>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 239. Sliding Window Maximum
// Web: https://leetcode.com/problems/sliding-window-maximum/
//      http://zxi.mytechroad.com/blog/heap/leetcode-239-sliding-window-maximum/
///////////////////////////////////////////////////////////////////////////////

namespace leet_239
{

class MonotonicQueue
{
  public:
    void push(int e)
    {
        while (!data_.empty() && e > data_.back())
            data_.pop_back();
        data_.push_back(e);
    }

    void pop() { data_.pop_front(); }

    int max() const { return data_.front(); }

  private:
    deque<int> data_;
};

class Solution
{
  public:
    vector<int> maxSlidingWindow(vector<int> &nums, int k)
    {
        MonotonicQueue q;
        vector<int> ans;

        for (int i = 0; i < nums.size(); ++i)
        {
            q.push(nums[i]);
            if (i - k + 1 >= 0)
            {
                ans.push_back(q.max());
                if (nums[i - k + 1] == q.max())
                    q.pop();
            }
        }
        return ans;
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
    std::make_tuple(vector<int>{1, 3, -1, -3, 5, 3, 6, 7}, 3, vector<int>{3, 3, 5, 5, 6, 7})
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
