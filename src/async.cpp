#include <future>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

namespace async
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: The promises and challenges of std::async task-based parallelism in C++11
// Web: https://eli.thegreenplace.net/2016/the-promises-and-challenges-of-stdasync-task-based-parallelism-in-c11/
///////////////////////////////////////////////////////////////////////////////

void accumulate_block_worker(const std::vector<int>& v, int *result)
{
    *result = std::accumulate(v.begin(), v.end(), 0);
}

TEST(async, thread)
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    int result = 0;
    std::thread worker(accumulate_block_worker, std::ref(v), &result);
    worker.join();
    ASSERT_EQ(36, result);
}

int accumulate_block_worker_ret(const std::vector<int>& v)
{
    return std::accumulate(v.begin(), v.end(), 0);
}

TEST(async, async)
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
    std::future<int> result = std::async(std::launch::async, accumulate_block_worker_ret, std::ref(v));
    ASSERT_EQ(36, result.get());
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: The promises and challenges of std::async task-based parallelism in C++11
// Web: https://eli.thegreenplace.net/2016/the-promises-and-challenges-of-stdasync-task-based-parallelism-in-c11/
///////////////////////////////////////////////////////////////////////////////

int accumulate_block_worker_throw(const std::vector<int>& v)
{
    throw std::runtime_error("something broke");
    return std::accumulate(v.begin(), v.end(), 0);
}

TEST(async, excpetion)
{
    try
    {
        std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8};
        std::future<int> result = std::async(std::launch::async, accumulate_block_worker_throw, std::ref(v));
        result.get();
    }
    catch (const std::runtime_error &error)
    {
        ASSERT_STREQ("something broke", error.what());
    }
}

} // namespace async
