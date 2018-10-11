#include <gtest/gtest.h>
#include <string>
using namespace std;

namespace variadic_template
{
///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: Specialized template for function signature
// Web: https://stackoverflow.com/questions/13887637/specialized-template-for-function-signature
//      https://stackoverflow.com/questions/34033586/what-does-this-syntax-mean-class-template-class-r-class-args-class-name
//      https://shaharmike.com/cpp/naive-std-function/
///////////////////////////////////////////////////////////////////////////////

template <typename>
class Signal;

template <typename T, typename U>
class Signal<T(U)>
{
  public:
    Signal<T(U)>(T (*ptr)(U)) {}
};

void Print(string const &str)
{
    printf("%s", str.c_str());
}

TEST(function_signature, simple)
{
    Signal<void(string const &)> sig = &Print;
}

} // namespace variadic_template