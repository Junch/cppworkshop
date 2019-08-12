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

namespace leet_1114_print_in_order_2
{

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

TEST(leet_1114_2, 123)
{
    Foo foo;
    std::thread t1(&Foo::first, &foo, printFirst);
    std::this_thread::sleep_for(100ms);
    std::thread t2(&Foo::second, &foo, printSecond);
    std::this_thread::sleep_for(100ms);
    std::thread t3(&Foo::third, &foo, printThird);
    t1.join();
    t2.join();
    t3.join();
}

TEST(leet_1114_2, 132)
{
    Foo foo;
    std::thread t1(&Foo::first, &foo, printFirst);
    std::this_thread::sleep_for(100ms);
    std::thread t2(&Foo::third, &foo, printThird);
    std::this_thread::sleep_for(100ms);
    std::thread t3(&Foo::second, &foo, printSecond);
    t1.join();
    t2.join();
    t3.join();
}

} // namespace leet_1114_print_in_order_2
