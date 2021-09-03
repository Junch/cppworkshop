#include <gmock/gmock.h>
#include <vector>

using namespace std;

namespace vector_tip
{

TEST(vector, erase_remove)
{
    // c++17 STL Cookbook: using the erase-remote idiom on std::vector
    vector<int> v{1, 2, 3, 5, 2, 6, 2, 4, 8};
    const auto new_end(remove(begin(v), end(v), 2));
    v.erase(new_end, end(v));
    v.shrink_to_fit();

    ASSERT_THAT(v, testing::ElementsAre(1, 3, 5, 6, 4, 8));
}

template <typename T>
void quick_remove_at(std::vector<T> &v, typename std::vector<T>::iterator it)
{
    if (it != std::end(v))
    {
        *it = std::move(v.back());
        v.pop_back();
    }
}

template <typename T>
void quick_remove_at(std::vector<T> &v, std::size_t idx)
{
    if (idx < v.size())
    {
        v[idx] = std::move(v.back());
        v.pop_back();
    }
}

TEST(vector, quick_remove)
{
    {
        // c++17 STL Cookbook: deleting items from unsorted std::vector in O(1) time
        vector<int> v{123, 456, 789, 100, 200};
        quick_remove_at(v, find(begin(v), end(v), 123));

        ASSERT_THAT(v, testing::ElementsAre(200, 456, 789, 100));
    }

    {
        vector<int> v{123, 456, 789, 100, 200};
        quick_remove_at(v, 2);

        ASSERT_THAT(v, testing::ElementsAre(123, 456, 200, 100));
    }

    {
        vector<int> v{123, 456, 789, 100, 200};
        const auto new_end = remove(begin(v), end(v), 123);
        v.erase(new_end, end(v));

        ASSERT_THAT(v, testing::ElementsAre(456, 789, 100, 200));
    }
}

TEST(vector, lower_bound)
{
    // [ closed interval
    vector<int> v{1, 2, 4, 5, 5, 6};
    auto iter1 = lower_bound(begin(v), end(v), 3);
    auto iter2 = lower_bound(begin(v), end(v), 4);
    ASSERT_EQ(*iter1, 4);
    ASSERT_EQ(*iter2, 4);
}

TEST(vector, upper_bound)
{
    // ( open interval
    vector<int> v{1, 2, 4, 5, 5, 6};
    auto iter1 = upper_bound(begin(v), end(v), 3);
    auto iter2 = upper_bound(begin(v), end(v), 4);
    ASSERT_EQ(*iter1, 4);
    ASSERT_EQ(*iter2, 5);
    ASSERT_EQ(std::distance(begin(v), iter2), 3);
}

} // namespace vector_tip