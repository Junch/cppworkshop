#include <vector>
#include <gtest/gtest.h>

namespace valgrind
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Valgrind is *NOT* a leak checker
// Web: http://maintainablecode.logdown.com/posts/245425-valgrind-is-not-a-leak-checker
///////////////////////////////////////////////////////////////////////////////

TEST(valgrind, use_uninitialized_values)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
    bool condition;
    if (condition) {
        //Do the thing
    }
#pragma GCC diagnostic pop
}

TEST(valgrind, accessing_wrong_memory)
{
    std::vector<int> v { 1, 2, 3, 4, 5 };
    v[5] = 0; 
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: Memory leak detector for Google Test on Windows
// Web: https://github.com/stbrenner/gtest_mem
///////////////////////////////////////////////////////////////////////////////

TEST(valgrind, memory_leak)
{
    void *p = malloc(7);
    p = nullptr;
    // Should fail as 7 bytes are not freed.
}

}
