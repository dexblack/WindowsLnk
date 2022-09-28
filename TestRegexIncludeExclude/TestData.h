#pragma once
#include <string>
#include <vector>

#include <CppUnitTest.h>
#include <RegExBaseTypes.h>
#include <RegexDllPort.h>

#include "TestRegexIncludeExcludePort.h"


namespace RETest
{
  typedef std::vector<string_type> strings_type;
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
  typedef RETest::strings_type strings_type;

  template<> inline string_type ToString<strings_type>(strings_type const& strs)
  {
    string_type out{ L"{ " };
    for (auto const& s : strs)
    {
      out.append(s);
      out.append(L", ");
    }
    if (out.size() > 2)
    {
      out[out.size() - 2] = L' ';
    }
    *out.rbegin() = L'}';
    return out;
  }

  typedef std::vector<std::string> strings_t;

  template<> inline string_type ToString<strings_t>(strings_t const& strs)
  {
    string_type out{ L"{ " };
    for (auto const& s : strs)
    {
      out.append(ToString(s));
      out.append(L", ");
    }
    if (out.size() > 2)
    {
      out[out.size() - 2] = L' ';
    }
    *out.rbegin() = L'}';
    return out;
  }
}

#pragma warning(push)
#pragma warning(disable : 4251)

namespace RETest
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;

  // Regular expression test data.
  //
  struct TestRegexIncExcDllPort TestRegexBase
  {
    ~TestRegexBase() {}

    struct TestRegexPattern
    {
      ~TestRegexPattern() {}

      struct RegexTestCase
      {
        string_type str;
        bool result;
        bool const stop = false; // =true breakpoint stops at this test.
        bool const finish = false; // Skip further testing.
      };
      typedef std::vector<RegexTestCase> RegexTestCases;

      string_type git_re;  // "easy" gitignore style patterns to expand
      string_type win_re;  // regex to match against
      RegexTestCases rtcs;  // strings to search
      bool const stop = false; // =true breakpoint stops at this test.
      bool const finish = false; // =true Skip further testing.
    };
    typedef std::vector<TestRegexPattern> TestRegexPatterns;

    static TestRegexPatterns path_tests;
  };


  // Regular expression Include Exclude algorithm tests.
  //
  struct TestRegexIncExcDllPort TestRegexIncludeExcludeBase
  {
    ~TestRegexIncludeExcludeBase() {}

    struct TestIncludeExcludePattern
    {
      ~TestIncludeExcludePattern() {}

      strings_type items;   // The items to be filtered.
      strings_type kept;    // List resulting from applying keep()==true.
      strings_type dropped; // List resulting from applying drop()==true.
      strings_type include; // Include patterns regex strings.
      strings_type exclude; // Exclude patterns regex strings.
      bool const stop = false;    // =true and set a breakpoint to stop on it.
      bool const finish = false;  // =true skip further tests.
    };
    typedef std::vector<TestIncludeExcludePattern> TestIncludeExcludePatterns;

    static TestIncludeExcludePatterns ie_tests;
  };


  struct TestRegexIncExcDllPort TestPatternToRegExStrBase
  {
    ~TestPatternToRegExStrBase() {}

    struct TestPattern
    {
      ~TestPattern() {}

      string_type git_re; // "easy" gitignore style patterns to expand
      string_type win_re; // regex to match against
      bool const result;  // ==false => expecting an error.
      bool const stop = false; // =true breakpoint stops at this test.
      bool const finish = false; // =true Skip further testing.
    };
    typedef std::vector<TestPattern> TestPatterns;

    static TestPatterns pattern_tests;
  };

  // Regular expression Include Exclude algorithm tests.
   //
  struct TestRegexIncExcDllPort TestRegexIgnoreBase
  {
    ~TestRegexIgnoreBase() {}

    struct TestRegexIgnoreBasePattern
    {
      ~TestRegexIgnoreBasePattern() {}

      struct TestRegexIgnoreItem
      {
        string_type pattern;  // Ignore pattern as gitignore style strings.
        bool ignore = true;   // Default is to ignore matching items.
      };
      typedef std::vector<TestRegexIgnoreItem> TestRegexIgnoreItems;

      string_type name;  // A unique name for this test.
      strings_type items;  // The items to be filtered.
      TestRegexIgnoreItems ignore; // The ignore list to be used.
      strings_type result; // Expected filtered list of items.
      bool const stop = false; // =true breakpoint stops at this test.
      bool const finish = false;  // =true skip further tests.
    };
    typedef std::vector<TestRegexIgnoreBasePattern> TestRegexIgnoreBasePatterns;

    static TestRegexIgnoreBasePatterns ignore_tests;
  };

#pragma warning(pop)

}
