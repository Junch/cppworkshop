#include <gtest/gtest.h>
#include <memory>
#include <stdarg.h>

namespace string_utils
{

///////////////////////////////////////////////////////////////////////////////
// No. 1
// Title: HOW TO DO STRING FORMATTING IN C++
// Web: http://www.martinbroadhurst.com/string-formatting-in-c.html
//      Alternatives are boost::format and folly::format
///////////////////////////////////////////////////////////////////////////////

std::string stringFormat(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    size_t len = std::vsnprintf(nullptr, 0, format, args);
    va_end(args);
    std::unique_ptr<char[]> buf(new char[len + 1]);
    va_start(args, format);
    std::vsnprintf(buf.get(), len + 1, format, args);
    va_end(args);
    return std::string(buf.get(), buf.get() + len);
}

TEST(stringFormat, simple)
{
    char str[] = "%s => %d";
    std::string r = stringFormat(str, "apples", 7);
    ASSERT_STREQ(r.c_str(), "apples => 7");
}

} // namespace string_utils
