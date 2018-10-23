#include <ctime>
#include <gtest/gtest.h>
#include <vector>

namespace stringftime
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: GETTING THE DATE AND TIME IN C++
// Web: http://www.martinbroadhurst.com/getting-the-date-and-time-in-c.html
///////////////////////////////////////////////////////////////////////////////

std::string stringftime(const char *format = "%c", const struct tm *tm = nullptr)
{
    if (tm == nullptr)
    {
        time_t t = std::time(nullptr);
        tm = std::localtime(&t);
    }
    const size_t size = 256;
    std::vector<char> vtime(size);
    if (std::strftime(&vtime[0], size, format, tm) == 0)
    {
        vtime[0] = '\0';
    }
    return &vtime[0];
}

TEST(stringftime, simple)
{
    std::cout << stringftime() << '\n';
    std::cout << stringftime("Today is %A, %B %d.") << '\n';
    std::cout << stringftime("The time is %I:%M %p.") << '\n';
}

} // namespace stringftime