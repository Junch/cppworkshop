#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;
using namespace std::literals;

///////////////////////////////////////////////////////////////////////////////
// Title: 1114. Print in Order
// Web: https://leetcode.com/problems/print-in-order
// Note: This solution will crash on windows
///////////////////////////////////////////////////////////////////////////////

namespace leet_1114_print_in_order
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
    Foo()
    {
        m1.lock();
        m2.lock();
    }

    void first(function<void()> printFirst)
    {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        m1.unlock();

        // In visual studio there will be assert: unlock of unowned mutex
        // https://stackoverflow.com/questions/14202094/c11-thread-mutex-implementation-in-vs2012-assertion-fired
        // https://zhuanlan.zhihu.com/p/260773684
        // An explanation: If the calling thread does not own the mutex, the behavior is undefined.
    }

    void second(function<void()> printSecond)
    {
        m1.lock();
        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        m2.unlock();
    }

    void third(function<void()> printThird)
    {
        m2.lock();
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }

  private:
    std::mutex m1, m2;
};

TEST(leet_1114, DISABLED_123)
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

TEST(leet_1114, DISABLED_132)
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

} // namespace leet_1114_print_in_order
