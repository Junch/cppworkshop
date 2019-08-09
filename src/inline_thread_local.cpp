#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <thread>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Thread-Local Data
// Web: https://www.modernescpp.com/index.php/thread-local-data
///////////////////////////////////////////////////////////////////////////////

std::mutex coutMutex;

// Thread-local data exclusively belongs to the thread and behaves like static data.
// It will be created at its first usage and its lifetime is bound to the lifetime of the thread
thread_local std::string s("hello from ");

void addThreadLocal(std::string const &s2)
{
    s += s2;
    // protect std::cout
    std::lock_guard<std::mutex> guard(coutMutex);
    std::cout << s << " &s: " << &s << "\n";
}

TEST(thread_local, test)
{
    std::thread t1(addThreadLocal, "t1");
    std::thread t2(addThreadLocal, "t2");
    std::thread t3(addThreadLocal, "t3");
    std::thread t4(addThreadLocal, "t4");

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Inline Variable
// Book: C++17 - The Complete Guide
///////////////////////////////////////////////////////////////////////////////

struct MyData
{
    inline static std::string gName = "global";
    inline static thread_local std::string tName = "tls";
    std::string lName = "local";

    void print(const std::string &msg) const
    {
        std::cout << msg << '\n';
        std::cout << "- gName: " << gName << '\n';
        std::cout << "- tName: " << tName << '\n';
        std::cout << "- lName: " << lName << '\n';
    }
};

inline thread_local MyData myThreadData;

void foo()
{
    myThreadData.print("foo() begin:");

    myThreadData.gName = "thread2 name";
    myThreadData.tName = "thread2 name";
    myThreadData.lName = "thread2 name";
    myThreadData.print("foo() end:");
}

TEST(inline_thread_local, test)
{
    myThreadData.print("main() begin:");
    myThreadData.gName = "thread1 name";
    myThreadData.tName = "thread1 name";
    myThreadData.lName = "thread1 name";
    myThreadData.print("main() later:");

    std::thread t(foo);
    t.join();
    myThreadData.print("main() end:");
}
