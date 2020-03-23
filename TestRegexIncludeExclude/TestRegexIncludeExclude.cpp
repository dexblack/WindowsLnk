#include "pch.h"

#include <string>
#include <algorithm>

#include "CppUnitTest.h"

#include "RegEx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef std::vector<std::wstring> wstrings;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
  template<> inline std::wstring ToString<wstrings>(wstrings const& strs)
  {
    std::wstring out{ L"{ " };
    for (auto const s : strs)
    {
      out.append(s);
      out.append(L", ");
    }
    *out.rbegin() = L'}';
    return out;
  }

}

std::wstring const RE_BEGIN{ LR"=(^)=" };
std::wstring const RE_DRIVE_OR_SERVER{ LR"=(^([A-Z]:|\\\\[^"\*:<>?/\\|]+)?)=" };
std::wstring const RE_DIRECTORIES{ LR"=((\\[^"*:<>?/\\|]+)*\\)=" };
std::wstring const RE_PATH_NAME_OPTIONAL{ LR"=([^"*:<>?/\\|]*)=" };
std::wstring const RE_PATH_NAME{ LR"=([^"*:<>?/\\|]+)=" };
std::wstring const RE_END{ LR"=($)=" };
std::wstring const RE_DIR_END{ LR"=(\\.*)=" + RE_END };


namespace TestRegexIncludeExclude
{

  TEST_CLASS(TestRegexIncludeExclude)
  {
  public:

    TEST_METHOD(Matching)
    {
      RegEx re(L"abc|def");
      RegEx::match_type match_results;
      std::wstring in(L"abcdef");
      Assert::IsTrue(re(in, match_results));
      std::vector<RegEx::string_type> results;
      std::copy(match_results.cbegin(), match_results.cend(), std::back_inserter(results));
      std::vector<RegEx::string_type> const expected{ L"abc" };
      Assert::IsTrue(expected == results);
    }

    TEST_METHOD(MatchingPaths)
    {
      struct TestRegexPattern
      {
        struct RegexTestCase
        {
          std::wstring str;
          bool result;
          bool stop = false; // =true and set a breakpoint to stop on it.
        };
        typedef std::vector<RegexTestCase> RegexTestCases;

        std::wstring gitre;  // "easy" gitignore style patterns to expand
        std::wstring re;  // regex to match against
        RegexTestCases rtcs;  // strings to search
      };
      typedef std::vector<TestRegexPattern> TestRegexPatterns;

      TestRegexPatterns tests
      {
        {
          LR"=(A:\one\)=",
          RE_BEGIN + LR"=(A:\\one)=" + RE_DIR_END,
          {
            { LR"=(A:\one)=", false },
            { LR"=(A:\one\)=", true },
            { LR"=(A:\one\anything.txt)=", true },
            { LR"=(A:\one\anything.)=", true },
            { LR"=(A:\one\.anything)=", true },
            { LR"=(A:\one\two)=", true },
            { LR"=(A:\one\two\)=", true },
            { LR"=(A:\one\two\anything.txt)=", true },
          }
        },{
          LR"=(B:*)=",
          RE_BEGIN + LR"=(B:.*$)=",
          {
            { LR"=(B:)=", true },
            { LR"=(B:\)=", true },
            { LR"=(B:\)=", true },
            { LR"=(B:\one)=", true },
            { LR"=(B:\two\anydir2\)=", true },
            { LR"=(B:\two\anydir2\a file)=", true },
            { LR"=(B:\two\anydir2\a file.txt)=", true },
          }
        },{
          LR"=(B:\*)=",
          RE_BEGIN + LR"=(B:)=" + RE_DIR_END,
          {
            { LR"=(B:\)=", true },
            { LR"=(B:\one)=", true },
            { LR"=(B:\two\a file)=", true },
            { LR"=(B:\two\a file.)=", true },
            { LR"=(B:\two\a file.txt)=", true },
            { LR"=(B:\two\anydir1\)=", true },
            { LR"=(B:\two\anydir2\a file)=", true },
          }
        },{
          LR"=(C:\*\my)=",
          RE_BEGIN + LR"=(C:\\[^"\*:<>?/\\|]+\\my$)=",
          {
            { LR"=(C:\\my)=", false },
            { LR"=(C:\\my\)=", false },
            { LR"=(C:\one\my\)=", false },
            { LR"=(C:\one\my)=", true },
            { LR"=(C:\one\two\my)=", false },
          }
        },{
          LR"=(D:\*\my\)=",
          RE_BEGIN + LR"=(D:\\[^"\*:<>?/\\|]+\\my)=" + RE_DIR_END,
          {
            { LR"=(D:\\my\)=", false },
            { LR"=(D:\my\)=", false },
            { LR"=(D:\one\my\)=", true },
            { LR"=(D:\one\my\a file)=", true },
            { LR"=(D:\one\two\my)=", false },
            { LR"=(D:\one\two\my\a file)=", false },
          }
        },{
          LR"=(E:\*\my\)=",
          RE_BEGIN + LR"=(E:\\[^"\*:<>?/\\|]*\\my)=" + RE_DIR_END,
          {
            { LR"=(E:\one\a file)=", false },
            { LR"=(E:\two\my\)=", true},
            { LR"=(E:\three\my.\a file)=", false },
            { LR"=(E:\four\my\adir\)=", true},
            { LR"=(E:\four\my\adir\a file)=", true},
          }
        },{
          LR"=(F:\**\my\)=",
          RE_BEGIN + LR"=(F:(\\[^"*:<>?/\\|]+)*\\my)=" + RE_DIR_END,
          {
            { LR"=(F:\my)=", false },
            { LR"=(F:\my\)=", true },
            { LR"=(F:\my\a file)=", true },
            { LR"=(F:\my\adir\)=", true },
            { LR"=(F:\my\adir\a file)=", true },
            { LR"=(F:\one\my\adir\a file)=", true },
            { LR"=(F:\two\two\my\adir\a file)=", true },
            { LR"=(F:\three\three\three\my)=", false },
            { LR"=(F:\three\three\three\my\)=", true },
          }
        },{
          LR"=(.git\)=",
          RE_DRIVE_OR_SERVER + RE_DIRECTORIES + LR"=(.git)=" + RE_DIR_END,
          {
            { LR"=(G:\dev\.git\)=", true },
            { LR"=(G:\dev\.git)=", false },
            { LR"=(G:\dev\.git\hooks)=", true },
            { LR"=(G:\dev\not.git\hooks)=", false },
          }
        },{
          LR"=(Downloads\)=",
          RE_DRIVE_OR_SERVER + RE_DIRECTORIES + LR"=(Downloads)=" + RE_DIR_END,
          {
            { LR"=(H:\Users\dex\Downloads\junk)=", true },
            { LR"=(\\server\share\Users\dex\Downloads\junk)=", true },
          }
        },{
          LR"=(~*\)=",
          RE_DRIVE_OR_SERVER + RE_DIRECTORIES + LR"=(~)=" + RE_PATH_NAME_OPTIONAL + RE_DIR_END,
          {
            { LR"=(I:\Users\dex\~)=", false },
            { LR"=(I:\Users\dex\~\)=", true },
            { LR"=(I:\Users\dex\~anydir)=", false },
            { LR"=(I:\Users\dex\~anydir\)=", true },
            { LR"=(I:\Users\dex\~anydir\anyfile)=", true },
            { LR"=(I:\Users\dex\~\anyfile)=", true },
            { LR"=(I:\Users\dex\~\anydir\)=", true },
            { LR"=(I:\Users\dex\~\anydir\anyfile)=", true },
            { LR"=(\\server\HDrive\Users\dex\~anydir\)=", true },
          }
        },{
          LR"=(~\)=",
          RE_DRIVE_OR_SERVER + RE_DIRECTORIES + LR"=(~)=" + RE_DIR_END,
          {
            { LR"=(I:\Users\dex\~\junk)=", true },
          }
        },{
          LR"=(~*)=",
          RE_DRIVE_OR_SERVER + RE_DIRECTORIES + LR"=(~)=" + RE_PATH_NAME_OPTIONAL + RE_END,
          {
            { LR"=(N:\Users\dex\~\junk.txt)=", false },
            { LR"=(N:\Users\~Anything\junk.txt)=", false },
            { LR"=(N:\Users\~Anything.txt)=", true },
            { LR"=(N:\Users\~.txt)=", true },
            { LR"=(N:\Users\~)=", true },
          }
        },{
          LR"=(~)=",
          RE_DRIVE_OR_SERVER + RE_DIRECTORIES + LR"=(~)=",
          {
            { LR"=(S:\Users\~)=", true },
            { LR"=(T:\Users\~)=", true },
          }
        },
      };
      for (auto const& test : tests)
      {
        for (auto const& rtc : test.rtcs)
        {
#if defined(_DEBUG)
          if (rtc.stop) _CrtDbgBreak();
#endif
          RegEx::match_type match_results;
          RegEx regex(test.re);
          bool const matched = regex(rtc.str, match_results);
          bool const matched_all = match_results.suffix().length() == 0;
          bool const ok = matched && matched_all;
          Assert::AreEqual(rtc.result, ok, rtc.str.c_str());
          if (rtc.result)
          {
            std::vector<RegEx::string_type> matches;
            std::copy(match_results.cbegin(), match_results.cend(), std::back_inserter(matches));
            Assert::AreEqual(rtc.str, matches[0]);
          }
        }
      }
    }

    TEST_METHOD(RegExString)
    {
      struct RegExStringTest
      {
        std::string str, re;
        std::vector<std::string> results;
      };
      std::vector<RegExStringTest> tests
      {
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
      for (auto const& t : tests)
      {
        std::smatch m;
        std::regex_search(t.str, m, std::regex(t.re));
        std::vector<std::string> matches;
        std::copy(m.cbegin(), m.cend(), std::back_inserter(matches));
        Assert::IsTrue(t.results == matches);
      }
    }
  };
}
