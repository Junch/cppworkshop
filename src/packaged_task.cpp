#include <chrono>
#include <deque>
#include <future>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

namespace packaged_task
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: C++11 并发指南四(<future> 详解二 std::packaged_task 介绍)
// Web: http://www.cnblogs.com/haippy/p/3279565.html
///////////////////////////////////////////////////////////////////////////////

int countdown(int from, int to)
{
    for (int i = from; i != to; --i)
    {
        std::cout << i << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Finished!\n";
    return from - to;
}

TEST(packaged_task, simple)
{
    std::packaged_task<int(int, int)> task(countdown);
    std::future<int> ret = task.get_future();
    std::thread th(std::move(task), 5, 0);
    int value = ret.get();

    std::cout << "The countdown lasted for " << value << " seconds.\n";
    th.join();
}

TEST(packaged_task, simple_1)
{
    std::packaged_task<int()> task([] { return countdown(5, 0); }); // better than std::bind(countdown, 5, 0)
    std::future<int> ret = task.get_future();
    std::thread th(std::move(task));
    int value = ret.get();

    std::cout << "The countdown lasted for " << value << " seconds.\n";
    th.join();
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: C++ Threading #9: packaged_task
// Web: https://www.youtube.com/watch?v=FfbZfBk-3rI&index=9&list=PL5jc9xFGsL8E12so1wlMS0r0hTQoJL74M
///////////////////////////////////////////////////////////////////////////////

std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cv;

void work()
{
    std::packaged_task<int()> t;
    {
        std::unique_lock<std::mutex> locker(mu);
        cv.wait(locker, []() { return !task_q.empty(); });
        t = std::move(task_q.front());
        task_q.pop_front();
    }
    t();
}

TEST(packaged_task, queue)
{
    std::thread workthread(work);
    std::packaged_task<int()> task([] { return countdown(5, 0); }); // better than std::bind(countdown, 10, 0)
    std::future<int> fu = task.get_future();
    {
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(std::move(task));
    }
    cv.notify_one();
    std::cout << fu.get() << '\n';

    workthread.join();
}

} // namespace packaged_task
