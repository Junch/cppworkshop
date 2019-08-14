#include <condition_variable>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>

namespace condition_variable
{

std::condition_variable cv;
std::mutex mtx;
bool ready = false;

void write()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }

    cv.notify_one();
}

void read()
{
    std::unique_lock<std::mutex> lock(mtx);
    for (int i = 0; i < 10; ++i)
    {
        std::cv_status status = cv.wait_for(lock, std::chrono::milliseconds{2000});
        if (status == std::cv_status::no_timeout)
        {
            std::cout << std::this_thread::get_id() << " no_timeout ready=" << ready << '\n';
        }
        else
        {
            std::cout << std::this_thread::get_id() << " timeout\n";
        }
    }
}

TEST(condition_variable, wait_for)
{
    using namespace std::chrono_literals;

    std::thread t(read);
    std::this_thread::sleep_for(1000ms);
    write();
    t.join();
}

// https://developercommunity.visualstudio.com/content/problem/193041/the-visual-c-2017-condition-variable-implementatio.html

TEST(condition_variable, vs_bug)
{
    for (int i = 0; i < 100; ++i)
    {
        auto timeout = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(50);
        std::unique_lock<std::mutex> lock(mtx);

        std::cv_status cvStatus = cv.wait_until(lock, timeout);

        if (cvStatus != std::cv_status::timeout)
        {
            std::cout << "Failed to time out at iteration " << i << std::endl;
        }
    }
}

} // namespace condition_variable