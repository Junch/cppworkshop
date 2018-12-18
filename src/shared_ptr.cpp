#include <chrono>
#include <gtest/gtest.h>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: GotW #91: Smart Pointer Parameters
// Web: https://herbsutter.com/2013/06/05/gotw-91-solution-smart-pointer-parameters/
//      https://stackoverflow.com/questions/3310737/should-we-pass-a-shared-ptr-by-reference-or-by-value
///////////////////////////////////////////////////////////////////////////////

void foo(const std::shared_ptr<int> &p)
{
    *p += 1;
}

void goo(std::shared_ptr<int> p)
{
    *p += 1;
}

TEST(shared_ptr, value_vs_reference)
{
    {
        auto start = std::chrono::high_resolution_clock::now();

        auto p = std::make_shared<int>(0);
        for (int i = 0; i < 10000000; ++i)
        {
            foo(p);
        }

        ASSERT_EQ(10000000, *p);

        auto end = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Passing by reference takes micro-seconds: " << delta.count() << '\n';
    }

    {
        auto start = std::chrono::high_resolution_clock::now();

        auto p = std::make_shared<int>(0);
        for (int i = 0; i < 10000000; ++i)
        {
            goo(p);
        }

        ASSERT_EQ(10000000, *p);

        auto end = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Passing by value takes micro-seconds: " << delta.count() << '\n';
    }
}
