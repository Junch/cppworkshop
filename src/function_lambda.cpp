#include <array>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>

namespace function_lambda
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Under the hood of lambdas and std::function
// Web: https://shaharmike.com/cpp/lambdas-and-functions/
///////////////////////////////////////////////////////////////////////////////

void globalF()
{
    std::cout << "global_f()\n";
}

struct Functor
{
    void operator()() { std::cout << "Functor\n"; }
};

TEST(function, simple)
{
    std::function<void()> f;
    std::cout << "sizeof(f) == " << sizeof(f) << std::endl;

    f = globalF;
    f();

    f = []() { std::cout << "Lambda\n"; };
    f();

    Functor functor;
    f = functor;
    f();
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: Under the hood of lambdas and std::function
// Web: https://shaharmike.com/cpp/lambdas-and-functions/
///////////////////////////////////////////////////////////////////////////////

struct A
{
    A() { std::cout << "Constructed\n"; }
    A(A const &) { std::cout << "Copy-Constructed\n"; }
    ~A() { std::cout << "Destructed\n"; }
};

TEST(lambda, scope)
{
    std::cout << "Creating A...\n";
    A a;

    {
        std::cout << "Creating lambda...\n";
        auto f = [a]() {}; // note 'a' is captured by-value
        std::cout << "Destroying lambda...\n";
    }

    std::cout << "Destroying A...\n";
}

///////////////////////////////////////////////////////////////////////////////
// No. 3
// Title: Under the hood of lambdas and std::function
// Web: https://shaharmike.com/cpp/lambdas-and-functions/
///////////////////////////////////////////////////////////////////////////////

TEST(lambda, size)
{
    auto f1 = []() {};
    ASSERT_EQ(1, sizeof(f1));

    std::array<char, 100> ar;
    auto f2 = [&ar]() {};
    ASSERT_EQ(sizeof(void *), sizeof(f2));

    auto f3 = [ar]() {};
    ASSERT_EQ(100, sizeof(f3));
}

///////////////////////////////////////////////////////////////////////////////
// No. 4
// Title: Under the hood of lambdas and std::function
// Web: https://shaharmike.com/cpp/lambdas-and-functions/
///////////////////////////////////////////////////////////////////////////////

TEST(lambda, copy)
{
    int i = 0;
    auto foo = [i]() mutable { std::cout << i++ << std::endl; };
    auto bar = [&i]() {
        std::cout << i << std::endl;
        i = 8;
    };
    i = 10;
    foo();
    bar();
    auto foo2 = foo;
    foo2();
    std::cout << i << std::endl;
}

} // namespace function_lambda