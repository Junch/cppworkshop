#include <array>
#include <codecvt>
#include <gtest/gtest.h>
#include <memory>
#include <regex>
#include <stdarg.h>

#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib, "Rpcrt4")
#else
#include <uuid/uuid.h>
#endif

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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

std::wstring s2ws(const std::string &str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring &wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

template <class Container>
void split1(const std::string &str, Container &cont)
{
    std::istringstream iss(str);
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(cont));
}

void replaceStringInPlace(std::string &subject, const std::string &search, const std::string &replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

bool stringMatch(const std::string &searchString, const std::string &userName)
{
    std::vector<std::string> container;
    split1(searchString, container);
    bool matched = false;

    try
    {
        std::stringstream iss;
        iss << ".*";
        // https://stackoverflow.com/questions/40195412/c11-regex-search-for-exact-string-escape
        std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
        for (const std::string &item : container)
        {
            // matches any characters that need to be escaped in RegEx
            std::string santized_item = std::regex_replace(item, specialChars, R"(\$&)");
            iss << "\\b" << santized_item << ".*";
        }

        std::string regString = iss.str();
        std::cout << regString << "\n";
        printf("%s\n", regString.c_str());
        std::regex ex(regString, std::regex_constants::icase);

        matched = std::regex_match(userName, ex);
    }
    catch (std::regex_error)
    {
    }

    return matched;
}

///////////////////////////////////////////////////////////////////////////////
// No. 2
// Title: How to convert wstring into string?
// Web: https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
///////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4566)
#endif

TEST(stringConvert, string2wstring)
{
    std::string str(u8"Hello World! 你好!");
    std::wstring s = s2ws(str);
    // MessageBoxW(NULL, s.c_str(), L"MessageBoxW", MB_OK);
    // MessageBoxA(NULL, str.c_str(), "MessageBoxA", MB_OK);
    ASSERT_STREQ(L"Hello World! 你好!", s.c_str());
}

TEST(stringConvert, wstring2string)
{
    std::wstring str(L"Hello World! 你好!");
    std::string s = ws2s(str);
    ASSERT_STREQ(u8"Hello World! 你好!", s.c_str());
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

TEST(stringConvert, oneline_string2wstring)
{
    std::string str(u8"Hello World! 你好!");
    std::wstring s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
    ASSERT_STREQ(L"Hello World! 你好!", s.c_str());
}

TEST(stringConvert, oneline_wstring2string)
{
    std::wstring str(L"Hello World! 你好!");
    std::string s = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
    ASSERT_STREQ(u8"Hello World! 你好!", s.c_str());
}

#ifdef _MSC_VER
#pragma warning(pop)
#pragma warning(pop)
#endif

TEST(split1, simple)
{
    std::string abc("It is  very   interesting");
    std::vector<std::string> container;
    split1(abc, container);
    for (const auto &item : container)
    {
        printf("%s\n", item.c_str());
    }
}

class RegexTest : public testing::TestWithParam<std::tuple<std::string, std::string, bool>>
{
  public:
    void SetUp() override {}
    void TearDown() override {}
};

// clang-format off
INSTANTIATE_TEST_SUITE_P(stringMatch, RegexTest,
                        ::testing::Values(std::make_tuple("Chen", "Chen Yu", true),
                                          std::make_tuple("Chen", "Aran Chen", true),
                                          std::make_tuple("chen", "ChengZhi Li", true),
                                          std::make_tuple("chen", "CChen Yu", false),
                                          std::make_tuple("j c", "Jack Chen", true),
                                          std::make_tuple("j c", "Aviva Jia Chen", true),
                                          std::make_tuple("av c", "", false),
                                          std::make_tuple("", "", true),
                                          std::make_tuple("", "abc", true),
                                          std::make_tuple("+123", "+12345", true),
                                          std::make_tuple("12+3", "12+345", true),
                                          std::make_tuple("12-3", "12-345", true),
                                          std::make_tuple("12[3", "12[345", true),
                                          std::make_tuple("av c", "Aviva Jia Chen", true)));
// clang-format on

TEST_P(RegexTest, one_word)
{
    const std::string &serachString = std::get<0>(GetParam());
    const std::string &userName = std::get<1>(GetParam());
    bool bMatch = std::get<2>(GetParam());

    ASSERT_EQ(bMatch, stringMatch(serachString, userName));
}

#ifdef _WIN32

// https://stackoverflow.com/questions/1327157/whats-the-c-version-of-guid-newguid
// https://stackoverflow.com/questions/24981119/c-uuid-to-stl-string/24981146
std::string genUuid()
{
    std::string uuidString;

    UUID uuid{0};
    UuidCreate(&uuid);
    RPC_CSTR pUIDStr = NULL;
    if (UuidToStringA(&uuid, &pUIDStr) == RPC_S_OK)
    {
        uuidString = (char *)pUIDStr;
        RpcStringFreeA(&pUIDStr);
    }

    return uuidString;
}

#else

std::string genUuid()
{
    std::string uuidString;

    uuid_t uuid;
    char buffer[40];

    uuid_generate_random(uuid);
    uuid_unparse(uuid, buffer);
    uuidString = buffer;

    return uuidString;
}

#endif

TEST(uuid, create)
{
    printf("%s\n", genUuid().c_str());
}

} // namespace string_utils
