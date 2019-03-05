#include <gtest/gtest.h>
#include <map>
#include <string>

TEST(map, emplace)
{
    std::map<int, std::string> number{{1, "One"}, {2, "Two"}, {3, "Three"}};

    auto result = number.emplace(4, "Four");
    ASSERT_TRUE(result.second);
    ASSERT_STREQ("Four", number[4].c_str());

    result = number.emplace(2, "Deux");
    ASSERT_FALSE(result.second); // emplace failed if the key exists
    ASSERT_STREQ("Two", number[2].c_str());
    ASSERT_EQ(2, result.first->first);
    ASSERT_STREQ("Two", result.first->second.c_str());

    result = number.insert(std::make_pair<int, std::string>(2, "Deux"));
    ASSERT_FALSE(result.second); // insert failed if the key exists
    ASSERT_STREQ("Two", number[2].c_str());
    ASSERT_EQ(2, result.first->first);
    ASSERT_STREQ("Two", result.first->second.c_str());

    number[2] = "Deux";
    ASSERT_STREQ("Deux", number[2].c_str());
}
