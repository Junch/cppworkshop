#include <gtest/gtest.h>
#include <memory>
#include <stdarg.h>
#include <codecvt>

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

std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}


///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: How to convert wstring into string?
// Web: https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
///////////////////////////////////////////////////////////////////////////////

TEST(stringConvert, string2wstring)
{
    std::string str("Hello World! 你好!");
    std::wstring s = s2ws(str);
    ASSERT_STREQ(L"Hello World! 你好!", s.c_str());
}

TEST(stringConvert, oneline_string2wstring)
{
    std::string str("Hello World! 你好!");
    std::wstring s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
    ASSERT_STREQ(L"Hello World! 你好!", s.c_str());
}

TEST(stringConvert, wstring2string)
{
    std::wstring str(L"Hello World! 你好!");
    std::string s = ws2s(str);
    ASSERT_STREQ("Hello World! 你好!", s.c_str());
}

TEST(stringConvert, oneline_wstring2string)
{
    std::wstring str(L"Hello World! 你好!");
    std::string s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
    ASSERT_STREQ("Hello World! 你好!", s.c_str());
}

} // namespace string_utils
