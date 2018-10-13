#include <chrono>
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

// count down taking a second for each value:
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
    std::thread th(std::move(task), 10, 0);
    int value = ret.get();

    std::cout << "The countdown lasted for " << value << " seconds.\n";
    th.join();
}

} // namespace packaged_task
