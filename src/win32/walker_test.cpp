#include "StackWalker.h"
#include <gtest/gtest.h>

using namespace std;

namespace StackWalkerTest
{

class StackWalkerToConsole : public StackWalker
{
  public:
    StackWalkerToConsole() : StackWalker(StackWalker::RetrieveNone) {}

  protected:
    virtual void OnOutput(LPCSTR szText) { printf("%s", szText); }
};

__declspec(noinline) void Func5()
{
    StackWalkerToConsole sw;
    sw.ShowCallstack();
}

__declspec(noinline) void Func4()
{
    Func5();
}

__declspec(noinline) void Func3()
{
    Func4();
}

__declspec(noinline) void Func2()
{
    Func3();
}

__declspec(noinline) void Func1()
{
    Func2();
}

TEST(StackWalker, test)
{
    Func1();
}

} // namespace StackWalkerTest
