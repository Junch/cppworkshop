#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <unordered_map>

// https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key

struct Key
{
    std::string first;
    std::string second;
    int third;

    bool operator==(const Key &other) const
    {
        return first == other.first && second == other.second && third == other.third;
    }
};

namespace std
{
template <>
struct hash<Key>
{
    std::size_t operator()(const Key &k) const
    {
        using std::hash;
        using std::size_t;
        using std::string;

        return ((hash<string>()(k.first) ^ (hash<string>()(k.second) << 1)) >> 1) ^ (hash<int>()(k.third) << 1);
    }
};
} // namespace std

TEST(hash, test)
{
    std::unordered_map<Key, std::string> m6 = {{{"John", "Doe", 12}, "example"}, {{"Mary", "Sue", 21}, "another"}};
    Key k{"John", "Doe", 12};
    auto iter = m6.find(k);
    if (iter != m6.end())
    {
        std::cout << iter->second << '\n';
    }

    std::cout << m6[k] << '\n';
}
