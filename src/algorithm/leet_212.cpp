#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

using namespace std;

namespace leet_212
{
class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        // This soliution will leed to TLE (Time Limit Exceeded)
        // https://zxi.mytechroad.com/blog/searching/leetcode-212-word-search-ii/

        vector<string> ans;
        int dirs[] = {-1, 0, 1, 0, -1};
        
        function<bool(string&, int, int, int)> dfs = [&](string& word, int n, int r, int c)->bool{
            // printf("r=%d, c=%d, n=%d\n", r, c, n);
            if (n == word.size()) {
                return true;
            }

            if (r < 0 || r >= board.size() || 
                c <0  || c >= board[0].size() ||
                board[r][c] != word[n]) {
                return false;
            }
            
            bool found = false;
            board[r][c] = '#';
            for (int i=0; i<4; ++i) {
                found = dfs(word, n+1, r + dirs[i], c + dirs[i+1]);
                if (found) break;           
            }
            board[r][c] = word[n];
            return found;
        };

        auto exist = [&](string& word)->bool{
            for (int i=0; i<board.size(); i++) {
                for (int j=0; j<board[0].size(); j++) {
                    if (dfs(word, 0, i, j))
                        return true;
                }
            }

            return false;
        };
        
        for (auto word: words){
            if (exist(word)) ans.push_back(word);
        }
                                      
        return ans;
    }
};

TEST(leet_212, test1) {
    vector<vector<char>> board{
        {'o','a','a','n'}, 
        {'e','t','a','e'},
        {'i','h','k','r'},
        {'i','f','l','v'}
    };

    vector<string> words {"oath","pea","eat","rain"};
    Solution sln;
    auto ans = sln.findWords(board, words);
    ASSERT_EQ(ans.size(), 2);
    ASSERT_EQ(ans[0], "oath");
    ASSERT_EQ(ans[1], "eat");
}

TEST(leet_212, test2) {
    vector<vector<char>> board{
        {'a','a'},
    };

    vector<string> words {"aaa"};
    Solution sln;
    auto ans = sln.findWords(board, words);
    ASSERT_EQ(ans.size(), 0);
}

}