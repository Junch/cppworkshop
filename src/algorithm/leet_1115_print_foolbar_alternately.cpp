#include <condition_variable>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace leet_1115
{

void printFoo()
{
    std::cout << "foo";
}

void printBar()
{
    std::cout << "bar";
}

class FooBar
{
  private:
    int n;
    std::mutex _mutex;
    bool foo_printed = false;
    std::condition_variable _cv;

  public:
    FooBar(int n) { this->n = n; }

    void foo(function<void()> printFoo)
    {
        for (int i = 0; i < n; i++)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // while (foo_printed)
            // {
            //     _cv.wait(lock);
            // }
            _cv.wait(lock, [this] { return !foo_printed; });

            // printFoo() outputs "foo". Do not change or remove this line.
            printFoo();

            foo_printed = true;

            // https://en.cppreference.com/w/cpp/thread/condition_variable
            // https://stackoverflow.com/questions/17101922/do-i-have-to-acquire-lock-before-calling-condition-variable-notify-one
            lock.unlock();
            _cv.notify_one();
        }
    }

    void bar(function<void()> printBar)
    {
        for (int i = 0; i < n; i++)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            // while (!foo_printed)
            // {
            //     _cv.wait(lock);
            // }
            _cv.wait(lock, [this] { return foo_printed; });

            // printBar() outputs "bar". Do not change or remove this line.
            printBar();

            foo_printed = false;
            lock.unlock();
            _cv.notify_one();
        }
    }
};

class leet_1115_test : public testing::TestWithParam<int>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, leet_1115_test, ::testing::Values(1, 2, 5));

TEST_P(leet_1115_test, foobar)
{
    FooBar foobar(GetParam());
    std::thread t1(&FooBar::foo, &foobar, printFoo);
    std::this_thread::sleep_for(100ms);
    std::thread t2(&FooBar::bar, &foobar, printBar);
    t1.join();
    t2.join();
    std::cout << '\n';
}

} // namespace leet_1115
