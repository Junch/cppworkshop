#include <gtest/gtest.h>
#include <vector>
using namespace std;

// The base case: we just have a single number.
template <typename T>
double sum(T t)
{
    return t;
}

// The recursive case: we take a number, alongside
// some other numbers, and produce their sum.
template <typename T, typename... Rest>
double sum(T t, Rest... rest)
{
    return t + sum(rest...);
}

TEST(variadic, sum)
{
    // Introduction to C++ Variadic Templates
    // http://kevinushey.github.io/blog/2016/01/27/introduction-to-c++-variadic-templates

    auto value = sum(1.0, 2.0, 3.0);
    ASSERT_EQ(value, 6.0);
}
