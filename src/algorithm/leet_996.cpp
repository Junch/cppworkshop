#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

namespace leet_996
{

class Solution {
public:
    int numSquarefulPerms(vector<int>& nums) {
        int counts = 0;
        std::sort(nums.begin(), nums.end());
        int size = nums.size();
        
        vector<int> path;
        vector<bool> used(size, false);
        
        function<void()> dfs = [&]() {
            if (path.size() == size) {    
                ++counts;
                return;
            }
            
            for (int i=0; i<size; ++i) {
                if (used[i]) continue;
                if (i>0 && nums[i]==nums[i-1] && !used[i-1]) continue;
                if (!path.empty() && !isPerfectSquare(path.back(), nums[i])) continue;
                
                path.push_back(nums[i]);
                used[i] = true;
                dfs();
                path.pop_back();
                used[i] = false;
            }
        };
        
        dfs();
        
        return counts;
    }
    
    bool isPerfectSquare(int a, int b) {
        int k = sqrt(a + b);
        return k*k == a + b;
    }
};

class leet_996_test : public testing::TestWithParam<std::tuple<vector<int>, int>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_996_test, ::testing::Values(
    std::make_tuple(vector<int>{1, 17, 8}, 2),
    std::make_tuple(vector<int>{2, 2, 2}, 1)
));
// clang-format on

TEST_P(leet_996_test, test)
{
    auto a = std::get<0>(GetParam());
    auto b = std::get<1>(GetParam());
    Solution sln;
    auto result = sln.numSquarefulPerms(a);
    ASSERT_EQ(b, result);
}

}