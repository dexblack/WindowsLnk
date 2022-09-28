#include "pch.h"

#include "TestData.h"
#include "RegexIncludeExclude.h"


namespace RETest
{
  TEST_CLASS(TestRegexIncludeExclude),
    TestRegexIncludeExcludeBase
  {
  public:
    typedef RegexIncludeExclude::strings_type strings_type;
    TEST_METHOD(TestKeepAndDrop)
    {
      RegexIncludeExclude reIncExc;
      for (auto const& test : ie_tests)
      {
#if defined(_DEBUG)
        if (test.stop) _CrtDbgBreak();
#endif
        reIncExc.configure(test.include, test.exclude);

        strings_type kept, dropped;
        for (auto const& item : test.items)
        {
          if (reIncExc.keep(item))
          {
            kept.push_back(item);
          }
          if (!reIncExc.drop(item))
          {
            dropped.push_back(item);
          }
        }
        Assert::AreEqual(test.kept, kept, L"kept");
        Assert::AreEqual(test.dropped, dropped, L"dropped");

        if (test.finish) break;
      }
    }
  };
}
