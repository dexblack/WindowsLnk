#include "pch.h"

#include <string>
#include <CppUnitTest.h>

#include "TestRegexIncludeExcludePort.h"

#include "TestData.h"
#include "RegexIgnore.h"
#include "PatternToRegExStr.h"


namespace RETest
{
  TEST_CLASS(PatternToRegExStrValid),
    TestRegexBase
  {
  public:

    TEST_METHOD(ValidPathPatterns)
    {
      for (auto const& test : path_tests)
      {
#if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
#endif
        string_type const win_re = pattern_to_regex_str(test.git_re);
        Assert::AreEqual(test.win_re, win_re, test.git_re.c_str());

        if (test.finish) break;
      }
    }


    TEST_METHOD(PatternMatchingPaths)
    {
      bool finish = false;
      for (auto const& test : path_tests)
      {
  #if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
  #endif
        string_type const win_re = pattern_to_regex_str(test.git_re);

        for (auto const& rtc : test.rtcs)
        {
  #if defined(_DEBUG)
          if (rtc.stop) _CrtDbgBreak();
  #endif
          RegEx::match_type match_results;
          RegEx regex(win_re); // Using RE built_from(test.git_re) instead of the test.win_re
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

        if (test.finish) break;
      }
    }
  };


  TEST_CLASS(PatternToRegExStrBasic),
    TestPatternToRegExStrBase
  {
  public:

    TEST_METHOD(BasicPatterns)
    {
      for (auto const& test : pattern_tests)
      {
#if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
#endif
        try
        {
          string_type const win_re = pattern_to_regex_str(test.git_re);
          if (test.result)
          {
            Assert::AreEqual(test.win_re, win_re, test.git_re.c_str());
          }
        }
        catch (PatternError const& perr)
        {
          Logger::WriteMessage(perr.what());
          Logger::WriteMessage("\n");
        }
        if (test.finish) break;
      }
    }
  };

  TEST_CLASS(RegexIgnoreCore),
    TestRegexIgnoreBase
  {
  public:

    TEST_METHOD(IgnorePatternLists)
    {
      for (auto const& test : ignore_tests)
      {
#if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
#endif
        RegexIgnore::RegexIgnoreItemStrs reiis;
        for (auto const& to_be_tested : test.ignore)
        {
          reiis.push_back(
            RegexIgnore::RegexIgnoreItemStr(
              to_be_tested.pattern, to_be_tested.ignore));
        }
        RegexIgnore::ConfigureMatching config;
        RegexIgnore rei(reiis, config);
        strings_type result;
        for (auto const& item : test.items)
        {
          if (rei.keep(item))
          {
            result.push_back(item);
          }
        }
        Assert::AreEqual(test.result, result, test.name.c_str());

        if (test.finish) break;
      }
    }
  };

}
