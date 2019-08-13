#include <condition_variable>
#include <functional>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Title: 1117. Building H2O
// Web: https://leetcode.com/problems/building-h2o/
///////////////////////////////////////////////////////////////////////////////

void releaseHydrogen()
{
    std::cout << "H";
}

void releaseOxygen()
{
    std::cout << "O";
}

class H2O
{
  public:
    H2O() {}

    void hydrogen(function<void()> releaseHydrogen)
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this] { return _hydrogen_count < 2; });

        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        ++_hydrogen_count;
        _cv.notify_all();
    }

    void oxygen(function<void()> releaseOxygen)
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this] { return _hydrogen_count == 2; });

        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        _hydrogen_count = 0;
        _cv.notify_all();
    }

  private:
    int _hydrogen_count = 0;
    std::mutex _mutex;
    std::condition_variable _cv;
};

class Leet_1117_Test : public testing::TestWithParam<int>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

INSTANTIATE_TEST_SUITE_P(leet, Leet_1117_Test, ::testing::Values(1, 2));

TEST_P(Leet_1117_Test, building_h2o)
{
    H2O obj;
    int number = GetParam();

    std::thread t1([&] {
        for (int i = 0; i < number; ++i)
        {
            obj.hydrogen(releaseHydrogen);
        }
    });

    std::thread t2([&] {
        for (int i = 0; i < number; ++i)
        {
            obj.hydrogen(releaseHydrogen);
        }
    });

    std::thread t3([&] {
        for (int i = 0; i < number; ++i)
        {
            obj.oxygen(releaseOxygen);
        }
    });

    t1.join();
    t2.join();
    t3.join();
    std::cout << '\n';
}
