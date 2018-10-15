#include <future>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

namespace promise
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: C++11 Multithreading – Part 8: std::future , std::promise and Returning values from Thread
// Web: https://thispointer.com/c11-multithreading-part-8-stdfuture-stdpromise-and-returning-values-from-thread/
///////////////////////////////////////////////////////////////////////////////

void initiazer(std::promise<int> *promObj)
{
    std::cout << "Inside Thread\n";
    promObj->set_value(35);
}

TEST(promise, simple)
{
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();
    std::thread th(initiazer, &promiseObj);
    std::cout << futureObj.get() << std::endl;
    th.join();
}

} // namespace promise
