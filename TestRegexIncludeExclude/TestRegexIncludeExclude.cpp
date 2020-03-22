#include "pch.h"

#include <string>
#include <algorithm>

#include "CppUnitTest.h"

#include "RegEx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


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
      struct RegExStringTestW
      {
        std::wstring easy, str, re;
        bool result;
      };
      std::vector<RegExStringTestW> tests
      {
        {LR"=(A:\my\)=",     LR"=(A:\my\anywhere\anything.txt)=",  LR"=(A:\\my\\.*)=", true },
        {LR"=(B:\*)=",       LR"=(B:\my)=",                        LR"=(B:\\.*)=", true },
        {LR"=(C:\*\my\)=",   LR"=(C:\x\my\)=",                     LR"=(C:\\[^\\]*\\my\\.*)=", true },
        {LR"=(D:\*\my\)=",   LR"=(D:\x\my\zz)=",                   LR"=(D:\\[^\\]*\\my\\.*)=", true },
        {LR"=(E:\*\my\)=",   LR"=(E:\x\y\my\zz)=",                 LR"=(E:\\[^\\]*\\my\\.*)=", false },
        {LR"=(F:\**\my\)=",  LR"=(F:\x\y\my\anything\some.txt)=",  LR"=(F:\\.*\\my\\.*)=", true },
        {LR"=(Downloads\)=", LR"=(G:\Users\dex\Downloads\junk)=",  LR"=([A-Z]:.*\\Downloads\\.*)=", true},
        {LR"=(~*\)=",        LR"=(H:\Users\dex\~Anything\)=",      LR"=([A-Z]:.*\\~[^\\]*\\.*)=", true},
        {LR"=(~*\)=",        LR"=(I:\Users\dex\~\junk)=",          LR"=([A-Z]:.*\\~[^\\]*\\.*)=", true},
        {LR"=(~\)=",         LR"=(J:\Users\dex\~\junk)=",          LR"=([A-Z]:.*\\~[^\\]*\\.*)=", true},
        {LR"=(~*\)=",        LR"=(K:\Users\dex\~Anything\junk)=",  LR"=([A-Z]:.*\\~[^\\]*\\.*)=", true},
        {LR"=(.git\)=",      LR"=(L:\dev\.git\hooks)=",            LR"=([A-Z]:.*\\.git\\.*)=", true},
        {LR"=(.git\)=",      LR"=(M:\dev\not.git\hooks)=",         LR"=([A-Z]:.*\\.git\\.*)=", false},
        {LR"=(~*)=",         LR"=(N:\Users\dex\~\junk.txt)=",      LR"=([A-Z]:.*\\~[^\\]*)=", false},
        {LR"=(~*)=",         LR"=(O:\Users\~Anything\junk.txt)=",  LR"=([A-Z]:.*\\~[^\\]*)=", false},
        {LR"=(~*)=",         LR"=(P:\Users\~Anything.txt)=",       LR"=([A-Z]:.*\\~[^\\]*)=", true},
        {LR"=(~*)=",         LR"=(Q:\Users\~.txt)=",               LR"=([A-Z]:.*\\~[^\\]*)=", true},
        {LR"=(~*)=",         LR"=(R:\Users\~)=",                   LR"=([A-Z]:.*\\~[^\\]*)=", true},
        //{LR"=()=", LR"=()=", LR"=()=", true},
        //{LR"=()=", LR"=()=", LR"=()=", true},
        //{LR"=()=", LR"=()=", LR"=()=", true},
      };
      for (auto const& t : tests)
      {
        RegEx::match_type match_results;
        RegEx regex(t.re);
        bool matched = regex(t.str, match_results) && match_results.suffix().length() == 0;
        //Assert::AreEqual(matched, t.result);
        if (t.result)
        {
          std::vector<RegEx::string_type> matches;
          std::copy(match_results.cbegin(), match_results.cend(), std::back_inserter(matches));
          std::vector<RegEx::string_type> const expected{ t.str };
          bool fail = expected != matches;
          //Assert::IsFalse(fail);
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
