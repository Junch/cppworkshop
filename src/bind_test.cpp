
#include <algorithm>
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace bind_test
{

double divide(double x, double y)
{
    return x / y;
}

TEST(bind, function)
{
    auto half = std::bind(divide, std::placeholders::_1, 2);
    ASSERT_EQ(half(10), 5.0);

    auto divide2 = std::bind(divide, 2, std::placeholders::_1);
    ASSERT_EQ(divide2(10), 0.2);
}

class Object
{
  public:
    void hello(const std::string &s) { std::cout << "Hello " << s << '\n'; }
};

TEST(bind, class_member)
{
    Object o;
    auto f = std::bind(&Object::hello, &o, std::placeholders::_1);
    f("World");
}

ostream &print(ostream &os, const string &s, char c)
{
    os << s << c;
    return os;
}

TEST(bind, reference)
{
    std::vector<string> words{"hello", "world", "this", "is", "C++11"};
    char c = ' ';

    {
        std::ostringstream oss;
        std::for_each(begin(words), end(words), bind(print, ref(oss), placeholders::_1, c));
        ASSERT_EQ(oss.str(), "hello world this is C++11 ");
    }

    {
        std::ostringstream oss;
        std::for_each(begin(words), end(words), [&oss, c](const string &s) { oss << s << c; });
        ASSERT_EQ(oss.str(), "hello world this is C++11 ");
    }
}

} // namespace bind_test