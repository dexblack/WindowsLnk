#include "pch.h"

#include "string_convert.h"

namespace TestString
{
  struct string_test
  {
    std::string sw;
    std::wstring ws;
  };
  std::vector<string_test> const string_tests
  {
    //{ "résumé façade", L"résumé façade" },
    { "\xce\xba\xe1\xbd\xb0\xcf\x83\xce\xbc\xce\xb5", L"\xce\xba\xe1\xbd\xb0\xcf\x83\xce\xbc\xce\xb5" },
  };

  TEST(convert, string)
  {
    for (auto const& st : string_tests)
    {
      std::wstring wsOut(string_to_wstring(st.sw));
      EXPECT_EQ(wsOut, st.ws);
    }
  }

  TEST(convert, wstring)
  {
    for (auto const& st : string_tests)
    {
      std::string sOut(wstring_to_string(st.ws));
      EXPECT_EQ(sOut, st.sw);
    }
  }
}
