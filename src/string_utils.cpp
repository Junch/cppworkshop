#include <array>
#include <cctype>
#include <codecvt>
#include <gtest/gtest.h>
#include <memory>
#include <regex>
#include <stdarg.h>

#ifdef _WIN32
#include <Windows.h>
#include <atlstr.h>
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
                                          // std::make_tuple("+123", "+12345", true),
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

// https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// https://stackoverflow.com/questions/44973435/stdptr-fun-replacement-for-c17
static inline std::string rtrim(std::string s)
{
    int pos = static_cast<int>(s.length() - 1);
    while (pos >= 0 && std::isspace(s[pos]))
    {
        --pos;
    }

    s.erase(pos + 1);
    return s;
}

static bool phone_match(const std::string &phone, const std::string &searchQuery)
{
    std::string phone_trimed = rtrim(phone);
    std::size_t found = phone_trimed.rfind(searchQuery);
    if (found == std::string::npos)
        return false;

    return found + searchQuery.length() == phone_trimed.length();
}

TEST(string, rtrim)
{
    {
        std::string phone{"133 6197 \t"};
        auto ret = rtrim(phone);
        ASSERT_STREQ(ret.c_str(), "133 6197");
    }

    {
        std::string phone{"133 6197"};
        auto ret = rtrim(phone);
        ASSERT_STREQ(ret.c_str(), "133 6197");
    }

    {
        std::string phone;
        auto ret = rtrim(phone);
        ASSERT_STREQ(ret.c_str(), "");
    }
}

TEST(string, phone_match)
{
    {
        std::string phone{"133 6197 \t"};
        std::string searchQuery{"6197"};
        ASSERT_TRUE(phone_match(phone, searchQuery));
    }

    {
        std::string phone{"133 6197"};
        std::string searchQuery{"6197"};
        ASSERT_TRUE(phone_match(phone, searchQuery));
    }

    {
        std::string phone{"133 6197"};
        std::string searchQuery{"619"};
        ASSERT_FALSE(phone_match(phone, searchQuery));
    }
}

#ifdef _WIN32

// https://blog.csdn.net/leitianjun/article/details/5605410
// https://onlineunicodetools.com/convert-unicode-to-utf8
// https://onlineunicodetools.com/convert-unicode-to-utf16

std::string WidestringToUTF8(const CStringW &wstr)
{
    if (wstr.IsEmpty())
    {
        return std::string();
    }

    int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr, wstr.GetLength(), NULL, 0, NULL, NULL);

    //  The solution commented is also OK
    // std::unique_ptr<char[]> buf(new char[size]);
    // WideCharToMultiByte(CP_UTF8, 0, wstr, wstr.GetLength(), buf.get(), size, NULL, NULL);
    // return std::string(buf.get());

    std::string utf8String(utf8len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, wstr.GetLength(), &utf8String[0], utf8len, NULL, NULL);
    return utf8String;
}

CStringW UTF8ToWidestring(const std::string &s)
{
    CStringW utf16String;
    int size_needed = ::MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.length() + 1, NULL, 0);
    // The solution commented is also OK
    // std::unique_ptr<wchar_t[]> buf(new wchar_t[size_needed]);
    // ::MultiByteToWideChar(CP_UTF8, 0, &s[0], -1, buf.get(), size_needed);
    // return CStringW(buf.get());

    ::MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.length() + 1, utf16String.GetBufferSetLength(size_needed), size_needed);
    return utf16String;
}

TEST(string, CStringW_2_UTF8_FAIL)
{
    std::string utf8str{u8"李世民☹"};
    CString utf16str{L"李世民☹"};

    USES_CONVERSION;
    std::string temp = T2A(utf16str.GetBuffer());
    ASSERT_STRNE(temp.c_str(), utf8str.c_str());
}

TEST(string, UTF8_2_CStringW_FAIL)
{
    std::string utf8str{u8"李世民☹"};
    CString utf16str{L"李世民☹"};

    USES_CONVERSION;
    CString temp = A2W(utf8str.c_str());
    ASSERT_STRNE(temp.GetString(), utf16str.GetString());
}

TEST(string, CStringW_2_UTF8)
{
    std::string utf8str{u8"李世民☹"};
    CString utf16str{L"李世民☹"};

    std::string temp = WidestringToUTF8(utf16str);
    ASSERT_STREQ(temp.c_str(), utf8str.c_str());
}

TEST(string, UTF8_2_CStringW)
{
    std::string utf8str{u8"李世民☹"};
    CString utf16str{L"李世民☹"};

    CString temp = UTF8ToWidestring(utf8str);
    ASSERT_STREQ(temp.GetString(), utf16str.GetString());

    utf8str.clear();
    temp = UTF8ToWidestring(utf8str);
    ASSERT_STREQ(temp.GetString(), CString().GetString());
}

#endif

} // namespace string_utils
