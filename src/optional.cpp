#include <gtest/gtest.h>
#include <optional>

std::optional<int> asInt(std::string s)
{
    try
    {
        return std::stoi(s);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

TEST(optional, simple)
{
    for (auto s : {"42", " 077", "hello", "0x33"})
    {
        std::optional<int> oi = asInt(s);
        if (oi)
        {
            std::cout << "convert '" << s << "' to int: " << *oi << '\n';
        }
        else
        {
            std::cout << "can't convert '" << s << "' to int \n";
        }
    }
}
