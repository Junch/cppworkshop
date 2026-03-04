#include <gtest/gtest.h>
#include <ranges>
#include <iostream>

TEST(ranges, simple)
{
    std::vector<int> vec{-1, 2, -3, 4, -5, 6};
    auto print = [](int i) {
         std::cout << i << " "; 
    };
    std::ranges::sort(vec);
    std::cout << "regular sort:\n";
    std::ranges::for_each(vec, print);
    std::cout << "\n";
}

struct Task 
{
    std::string desc;
    unsigned int priority {0};
};

TEST(ranges, task)
{
    std::vector<Task> tasks { 
        { "clean up my room", 10 }, 
        { "finish homework", 5 },
        { "test a car", 8 }, 
        { "buy new monitor", 12 }
    };
    auto print = [](Task& t) { 
        std::cout << t.desc << ", priority: " << t.priority << '\n'; 
    };
    std::ranges::sort(tasks, std::ranges::greater{}, &Task::priority);
    std::cout << "my next priorities:\n";
    std::ranges::for_each(tasks, print);
}
