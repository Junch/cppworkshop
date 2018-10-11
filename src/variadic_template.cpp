#include <gtest/gtest.h>
#include <vector>
using namespace std;

namespace variadic_template
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Book: In-Depth C++11 深入应用C++11
// Page: 103
///////////////////////////////////////////////////////////////////////////////

template <typename... T>
int print(T... args)
{
    return sizeof...(args);
}

TEST(variadic, simple)
{
    ASSERT_EQ(0, print());
    ASSERT_EQ(2, print(1, 2));
    ASSERT_EQ(3, print(1, 2.5, ""));
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: Introduction to C++ Variadic Templates
// Web: http://kevinushey.github.io/blog/2016/01/27/introduction-to-c++-variadic-templates
///////////////////////////////////////////////////////////////////////////////

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
    auto value = sum(1.0, 2.0, 3.0);
    ASSERT_EQ(value, 6.0);
}

///////////////////////////////////////////////////////////////////////////////
// No. 3
// Book: In-Depth C++11 深入应用C++11
// Page: 113
///////////////////////////////////////////////////////////////////////////////

struct A
{
    A(int &&n) { printf("rvalue overload, n=%d\n", n); }
    A(int &n) { printf("lvalue overload, n=%d\n", n); }
};

// The version is without the perfect forward
// template <typename T, typename... Args>
// T *Instance(Args... args)
// {
//     return new T(args...);
// };

template <typename T, typename... Args>
T *Instance(Args &&... args)
{
    return new T(std::forward<Args>(args)...);
};

TEST(variadic, factory)
{
    int value = 1;
    Instance<A>(1);
    Instance<A>(value);
}

} // namespace variadic_template
