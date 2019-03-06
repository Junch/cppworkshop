#include "type_name.h"
#include <gtest/gtest.h>
#include <iostream>
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

TEST(map, lower_bound)
{
    std::map<int, std::string> number{{1, "One"}, {2, "Two"}, {4, "Four"}, {5, "Five"}};
    {
        auto itlow = number.lower_bound(3); // Iterator pointing to the first element that is not less than key.
        auto itup = number.upper_bound(3);  // Iterator pointing to the first element that is greater than key

        ASSERT_EQ(4, itlow->first);
        ASSERT_EQ(4, itup->first);
    }

    {
        auto itlow = number.lower_bound(4); // Iterator pointing to the first element that is not less than key.
        auto itup = number.upper_bound(4);  // Iterator pointing to the first element that is greater than key

        ASSERT_EQ(4, itlow->first);
        ASSERT_EQ(5, itup->first);
    }
}

TEST(map, type)
{
    std::map<int, std::string> number{{1, "One"}, {2, "Two"}, {3, "Three"}};
    auto iter1 = number.find(2);
    std::cout << "decltype(iter1) is " << type_name<decltype(iter1)>() << '\n';

    const auto iter2 = number.find(2);
    std::cout << "decltype(iter2) is " << type_name<decltype(iter2)>() << '\n';

    std::map<int, std::string>::iterator iter3 = number.find(2);
    std::cout << "decltype(iter3) is " << type_name<decltype(iter3)>() << '\n';

    std::map<int, std::string>::const_iterator iter4 = number.find(2);
    std::cout << "decltype(iter4) is " << type_name<decltype(iter4)>() << '\n';
}
