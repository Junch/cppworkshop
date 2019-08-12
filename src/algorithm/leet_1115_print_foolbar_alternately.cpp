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
    bool foo_printed;
    std::condition_variable _cv;

  public:
    FooBar(int n) : foo_printed(false) { this->n = n; }

    void foo(function<void()> printFoo)
    {
        for (int i = 0; i < n; i++)
        {
            std::unique_lock lock(_mutex);
            while (foo_printed)
            {
                _cv.wait(lock);
            }

            // printFoo() outputs "foo". Do not change or remove this line.
            printFoo();

            foo_printed = true;
            _cv.notify_one();
        }
    }

    void bar(function<void()> printBar)
    {
        for (int i = 0; i < n; i++)
        {
            std::unique_lock lock(_mutex);
            while (!foo_printed)
            {
                _cv.wait(lock);
            }

            // printBar() outputs "bar". Do not change or remove this line.
            printBar();

            foo_printed = false;
            _cv.notify_one();
        }
    }
};

class Leet_1115_Test : public testing::TestWithParam<int>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, Leet_1115_Test, ::testing::Values(1, 2, 5));

TEST_P(Leet_1115_Test, foobar)
{
    FooBar foobar(GetParam());
    std::thread t1(&FooBar::foo, &foobar, printFoo);
    std::this_thread::sleep_for(100ms);
    std::thread t2(&FooBar::bar, &foobar, printBar);
    t1.join();
    t2.join();
}

} // namespace leet_1115
