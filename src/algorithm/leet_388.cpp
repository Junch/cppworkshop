#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

using namespace std;

namespace leet_388
{

class Solution
{
  public:
    int lengthLongestPath(string input)
    {
        vector<string> path;
        int longestLength = 0;

        std::istringstream f(input);
        std::string line;
        while (std::getline(f, line))
        {
            int tabs = 0, len = line.length();
            while (line[tabs] == '\t' && tabs < len)
                ++tabs;
            auto cur = line.substr(tabs);
            path.resize(tabs + 1);
            path[tabs] = cur;

            if (cur.find('.') != cur.npos)
            {
                ostringstream oss;
                oss << path[0];
                for (int i = 1; i < path.size(); ++i)
                {
                    oss << '/' << path[i];
                }
                int length = oss.str().length();
                longestLength = max(longestLength, length);
            }
        }

        return longestLength;
    }
};

TEST(leet_388, test)
{
    string input = "dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext";
    Solution sln;
    int len = sln.lengthLongestPath(input);
    ASSERT_EQ(len, 19);
}
} // namespace leet_388