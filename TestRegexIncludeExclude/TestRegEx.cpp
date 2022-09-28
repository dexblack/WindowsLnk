#include "pch.h"

#include <string>
#include <algorithm>

#include <CppUnitTest.h>

#include "TestData.h"
#include "RegEx.h"


namespace RETest
{
  TEST_CLASS(TestRegex),
    TestRegexBase
  {
  public:

    TEST_METHOD(BasicMatching)
    {
      RegEx win_re(L"abc|def");
      RegEx::match_type match_results;
      std::wstring in(L"abcdef");
      Assert::IsTrue(win_re(in, match_results));
      std::vector<string_type> results;
      std::copy(match_results.cbegin(), match_results.cend(), std::back_inserter(results));
      std::vector<string_type> const expected{ L"abc" };
      Assert::IsTrue(expected == results);
    }

    TEST_METHOD(PathMatching)
    {
      bool finish = false;
      for (auto const& test : path_tests)
      {
#if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
#endif
        for (auto const& rtc : test.rtcs)
        {
#if defined(_DEBUG)
          if (rtc.stop) _CrtDbgBreak();
#endif
          RegEx::match_type match_results;
          RegEx regex(test.win_re);
          bool const matched = regex(rtc.str, match_results);
          bool const matched_all = match_results.suffix().length() == 0;
          bool const ok = matched && matched_all;
          Assert::AreEqual(rtc.result, ok, rtc.str.c_str());
          if (rtc.result)
          {
            std::vector<string_type> matches;
            std::copy(match_results.cbegin(), match_results.cend(), std::back_inserter(matches));
            Assert::AreEqual(rtc.str, matches[0]);
          }
          if (rtc.finish)
          {
            finish = true;
            break;
          }
        }
        if (finish || test.finish) break;
      }
    }

    TEST_METHOD(BasicStringMatching)
    {
      struct RegExStringTest
      {
        std::string const str;
        std::string const win_re;
        std::vector<std::string> const results;
        bool const stop = false;    // =true and set a breakpoint to stop on it.
        bool const finish = false;  // =true skip further tests.
      };
      std::vector<RegExStringTest> const str_tests
      {
        {"abcdef", "a", { "a" } },
        {"abcdef", "f", { "f" } },
        {"abcdef", "abc|def", { "abc" } },
        {"abcdef", "(abc|def)", { "abc", "abc" }},
        {"abcdef", ".*abc|def", { "abc" }},
        {"abcdef", ".*(abc|def)", { "abcdef", "def" }},
        {"abcdef", ".*abc|.*def", { "abc" }},
        {"abcdef", "(.*abc|.*def)", { "abc", "abc" }},
        {"abcdef", "abc|abcdef", { "abc" }},
        {"abcdef", "abcdef|abc", { "abcdef" }},
        {"abcdef", "(abcdef|abc)", { "abcdef", "abcdef" }},
      };
      for (auto const& test : str_tests)
      {
#if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
#endif
        std::smatch m;
        std::regex_search(test.str, m, std::regex(test.win_re));
        std::vector<std::string> matches;
        std::copy(m.cbegin(), m.cend(), std::back_inserter(matches));
        Assert::AreEqual(test.results, matches);

        if (test.finish) break;
      }
    }
  };
}
