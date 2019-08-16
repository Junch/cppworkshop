#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <gtest/gtest.h>
#include <iomanip>
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
        auto start = std::chrono::high_resolution_clock::now();
        std::cv_status status = cv.wait_for(lock, std::chrono::milliseconds{2000});
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct std::tm *ptm = std::localtime(&timenow);

        if (status == std::cv_status::no_timeout)
        {
            std::cout << std::put_time(ptm, "%b %d %H:%M:%S %Y") << " elapsed=" << microseconds << "us ready=" << ready
                      << " no_timeout\n";
        }
        else
        {
            std::cout << std::put_time(ptm, "%b %d %H:%M:%S %Y") << " elapsed=" << microseconds << "us ready=" << ready
                      << " timeout\n";
        }
    }
}

TEST(condition_variable, wait_for)
{
    using namespace std::chrono_literals;

    std::thread t(read);
    std::this_thread::sleep_for(3000ms);
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