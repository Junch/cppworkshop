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

void initiazer(std::promise<int> *prom_obj)
{
    std::cout << "Inside Thread\n";
    prom_obj->set_value(35);
}

TEST(promise, simple)
{
    std::promise<int> promise_obj;
    std::future<int> future_obj = promise_obj.get_future();
    std::thread th(initiazer, &promise_obj);
    std::cout << future_obj.get() << std::endl;
    th.join();
}

} // namespace promise
