#include <gtest/gtest.h>
#include <tuple>

namespace cpp_17
{

TEST(structural_bindings, tuple)
{
    auto person = std::make_tuple("fred", 42);
    auto [name, age] = person;
    ASSERT_STREQ(name, "fred");
    ASSERT_EQ(age, 42);
}

TEST(structural_bindings, tuple_reference)
{
    auto person = std::make_tuple("fred", 42);
    auto &[name, age] = person;
    ASSERT_STREQ(name, "fred");
    ASSERT_EQ(age, 42);
    age += 1;
    ASSERT_EQ(std::get<1>(person), 43);
}

TEST(structural_bindings, loop)
{
    std::map<int, int> myMap{{1, 11}, {2, 22}, {3, 33}};

    for (const auto &[key, value] : myMap)
    {
        std::cout << key << ": " << value << std::endl;
    }
}

} // namespace cpp_17