#include <array>
#include <functional>
#include <future>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::literals;

///////////////////////////////////////////////////////////////////////////////
// Title: 1114. Print in Order
// Web: https://leetcode.com/problems/print-in-order
//      https://blog.csdn.net/fuxuemingzhu/article/details/95889253
///////////////////////////////////////////////////////////////////////////////

void printFirst()
{
    std::cout << "first";
}

void printSecond()
{
    std::cout << "second";
}

void printThird()
{
    std::cout << "third";
}
namespace leet_1114_print_in_order_2
{
class Foo
{
  public:
    Foo() {}

    void first(function<void()> printFirst)
    {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        p1.set_value();
    }

    void second(function<void()> printSecond)
    {
        p1.get_future().wait();
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        p2.set_value();
    }

    void third(function<void()> printThird)
    {
        p2.get_future().wait();
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }

  private:
    std::promise<void> p1;
    std::promise<void> p2;
};

class leet_1114_test2 : public testing::TestWithParam<std::array<int, 3>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, leet_1114_test2,
                         ::testing::Values(std::array{1, 2, 3}, std::array{3, 1, 2}, std::array{3, 2, 1}));

TEST_P(leet_1114_test2, print_in_order)
{
    auto a = GetParam();
    Foo foo;
    std::vector<std::thread> threads;

    for (int i = 0; i < 3; ++i)
    {
        switch (a[i])
        {
        case 1:
            threads.emplace_back(&Foo::first, &foo, printFirst);
            break;
        case 2:
            threads.emplace_back(&Foo::second, &foo, printSecond);
            break;
        case 3:
            threads.emplace_back(&Foo::third, &foo, printThird);
            break;
        default:
            FAIL() << "Unexpected input";
        }
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << '\n';
}

} // namespace leet_1114_print_in_order_2

namespace leet_1114_print_in_order_3
{
class Foo
{
  public:
    Foo():lock1(m1, try_to_lock), lock2(m2, try_to_lock)  {}

    void first(function<void()> printFirst)
    {
        printFirst();
        lock1.unlock();
    }

    void second(function<void()> printSecond)
    {
        lock_guard<mutex> guard(m1);
        printSecond();
        lock2.unlock();
    }

    void third(function<void()> printThird)
    {
        lock_guard<mutex> guard(m2);
        printThird();
    }

  private:
    std::mutex m1, m2;
    std::unique_lock<mutex> lock1, lock2;
};

class leet_1114_test3 : public testing::TestWithParam<std::array<int, 3>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, leet_1114_test3,
                         ::testing::Values(std::array{1, 2, 3}, std::array{3, 1, 2}, std::array{3, 2, 1}));

TEST_P(leet_1114_test3, DISABLED_print_in_order)
{
    // Failed on windows
    // unlock of unowned mutex
    auto a = GetParam();
    Foo foo;
    std::vector<std::thread> threads;

    for (int i = 0; i < 3; ++i)
    {
        switch (a[i])
        {
        case 1:
            threads.emplace_back(&Foo::first, &foo, printFirst);
            break;
        case 2:
            threads.emplace_back(&Foo::second, &foo, printSecond);
            break;
        case 3:
            threads.emplace_back(&Foo::third, &foo, printThird);
            break;
        default:
            FAIL() << "Unexpected input";
        }
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << '\n';
}

} // namespace leet_1114_print_in_order_3

namespace leet_1114_print_in_order_4
{
class Foo
{
  public:
    Foo() {}

    void first(function<void()> printFirst)
    {
        unique_lock<mutex> lock(mu); // without this line. the test may hang in the repeated tests --gtest_repeat=100
        printFirst();
        k = 1;
        cv.notify_all();
    }

    void second(function<void()> printSecond)
    {
        unique_lock<mutex> lock(mu);
        cv.wait(lock, [this](){ return k == 1; });
        printSecond();
        k = 2;
        cv.notify_all();
    }

    void third(function<void()> printThird)
    {
        unique_lock<mutex> lock(mu);
        cv.wait(lock, [this](){ return k == 2; });
        printThird();
    }

  private:
    int k = 0;
    std::mutex mu;
    std::condition_variable cv;
};

class leet_1114_test4 : public testing::TestWithParam<std::array<int, 3>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, leet_1114_test4,
                         ::testing::Values(std::array{1, 2, 3}, std::array{3, 1, 2}, std::array{3, 2, 1}));

TEST_P(leet_1114_test4, print_in_order)
{
    auto a = GetParam();
    Foo foo;
    std::vector<std::thread> threads;

    for (int i = 0; i < 3; ++i)
    {
        switch (a[i])
        {
        case 1:
            threads.emplace_back(&Foo::first, &foo, printFirst);
            break;
        case 2: 
            threads.emplace_back(&Foo::second, &foo, printSecond);
            break;
        case 3:
            threads.emplace_back(&Foo::third, &foo, printThird);
            break;
        default:
            FAIL() << "Unexpected input";
        }
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << '\n';
}

} // namespace leet_1114_print_in_order_4