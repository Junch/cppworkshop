#include <condition_variable>
#include <functional>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>

using namespace std;

namespace leet_1116
{

void printNumber(int number)
{
    std::cout << number;
}

class ZeroEvenOdd
{
  private:
    int n;
    std::mutex _mutex;
    std::condition_variable _cv;
    int number = 0;
    int flag = 0;
    bool bExit = false;

  public:
    ZeroEvenOdd(int n) { this->n = n; }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber)
    {
        while (true)
        {
            std::unique_lock lock(_mutex);
            _cv.wait(lock, [this] { return flag == 0; });

            ++number;
            if (number > n)
            {
                bExit = true;
                _cv.notify_all();
                return;
            }

            printNumber(0);

            if (number % 2 == 0)
            {
                flag = 2;
            }
            else
            {
                flag = 1;
            }

            _cv.notify_all();
        }
    }

    void even(function<void(int)> printNumber)
    {
        while (true)
        {
            std::unique_lock lock(_mutex);
            _cv.wait(lock, [this] { return flag == 2 || bExit; });

            if (bExit)
            {
                _cv.notify_all();
                return;
            }
            else
            {
                printNumber(number);
                flag = 0;
                _cv.notify_all();
            }
        }
    }

    void odd(function<void(int)> printNumber)
    {
        while (true)
        {
            std::unique_lock lock(_mutex);
            _cv.wait(lock, [this] { return flag == 1 || bExit; });

            if (bExit)
            {
                _cv.notify_all();
                return;
            }
            else
            {
                printNumber(number);
                flag = 0;
                _cv.notify_all();
            }
        }
    }
};

class leet_1116_test : public testing::TestWithParam<int>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, leet_1116_test, ::testing::Values(1, 2, 5));

TEST_P(leet_1116_test, zero_even_odd)
{
    ZeroEvenOdd obj(GetParam());
    std::thread t1(&ZeroEvenOdd::zero, &obj, printNumber);
    std::this_thread::sleep_for(100ms);
    std::thread t2(&ZeroEvenOdd::even, &obj, printNumber);
    std::this_thread::sleep_for(100ms);
    std::thread t3(&ZeroEvenOdd::odd, &obj, printNumber);

    t1.join();
    t2.join();
    t3.join();
    std::cout << '\n';
}

} // namespace leet_1116