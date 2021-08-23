#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>

using namespace std;

namespace leet_127
{

class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // The dfs solution will meet TLE for the test case 2

        int n = wordList.size();
        int shortestLength = INT_MAX;
        vector<string> path;
        vector<bool> used(n, false);
        
        auto adjacent = [&](const string& a, const string& b)->bool{
            int differ = 0;
            for(int i=0; i<a.size(); ++i) {
                if (a[i] != b[i]) {
                    ++differ;
                    if (differ > 1) return false;
                }
            }
            
            return differ == 1;
        };
        
        function<void()> dfs = [&](){
            string s = path.back();
            if (s == endWord) {
                if (path.size() < shortestLength)
                    shortestLength = path.size();

                return;
            }
            
            if (path.size() >= shortestLength)
                return;
            
            for (int i=0; i<n; ++i){
                if (used[i]) continue;
                if (!adjacent(s, wordList[i])) continue;
    
                path.push_back(wordList[i]);
                used[i] = true;
                dfs();
                path.pop_back();
                used[i] = false;
            }               
        };
        
        path.push_back(beginWord);
        dfs();
        return shortestLength == INT_MAX? 0: shortestLength;        
    }
};


class SolutionBFS {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // https://zxi.mytechroad.com/blog/searching/127-word-ladder

        unordered_set<string> dict(wordList.begin(), wordList.end());        
        if (!dict.count(endWord)) return 0;
        
        queue<string> q;
        q.push(beginWord);
        
        int l = beginWord.length();
        int step = 0;
        
        while (!q.empty()) {
            ++step;
            for (int size = q.size(); size > 0; size--) {                
                string w = q.front();                
                q.pop();
                for (int i = 0; i < l; i++) {                
                    char ch = w[i];
                    for (int j = 'a'; j <= 'z'; j++) {
                        w[i] = j;
                        // Found the solution
                        if (w == endWord) return step + 1;
                        // Not in dict, skip it
                        if (!dict.count(w)) continue;
                        // Remove new word from dict
                        dict.erase(w);
                        // Add new word into queue
                        q.push(w);                    
                    }
                    w[i] = ch;
                }
            }
        }
        return 0;
     
    }
};

class leet_127_test : public testing::TestWithParam<std::tuple<string, string, vector<string>, int>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(leet, leet_127_test, ::testing::Values(
    std::make_tuple("hit", "cog", vector<string>{"hot","dot","dog","lot","log","cog"}, 5),
    std::make_tuple("qa", "sq", vector<string>{"si","go","se","cm","so","ph","mt","db","mb","sb","kr","ln","tm","le","av",
                                               "sm","ar","ci","ca","br","ti","ba","to","ra","fa","yo","ow","sn","ya","cr",
                                               "po","fe","ho","ma","re","or","rn","au","ur","rh","sr","tc","lt","lo","as",
                                               "fr","nb","yb","if","pb","ge","th","pm","rb","sh","co","ga","li","ha","hz",
                                               "no","bi","di","hi","qa","pi","os","uh","wm","an","me","mo","na","la","st",
                                               "er","sc","ne","mn","mi","am","ex","pt","io","be","fm","ta","tb","ni","mr",
                                               "pa","he","lr","sq","ye"}, 5)
));
// clang-format on

TEST_P(leet_127_test, DISABLED_dfs)
{
    auto beginWord = std::get<0>(GetParam());
    auto endWord = std::get<1>(GetParam());
    auto wordList = std::get<2>(GetParam());
    auto ans = std::get<3>(GetParam());
    Solution sln;
    auto result = sln.ladderLength(beginWord, endWord, wordList);
    ASSERT_EQ(ans, result);
}

TEST_P(leet_127_test, bfs)
{
    auto beginWord = std::get<0>(GetParam());
    auto endWord = std::get<1>(GetParam());
    auto wordList = std::get<2>(GetParam());
    auto ans = std::get<3>(GetParam());
    SolutionBFS sln;
    auto result = sln.ladderLength(beginWord, endWord, wordList);
    ASSERT_EQ(ans, result);
}

}