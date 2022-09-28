#include "pch.h"

#include <string>
#include <regex>

#include <CppUnitTest.h>

#include "PatternToRegExStr.h"


namespace RETest
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;

  TEST_CLASS(TestTokenizers)
  {
  public:

    TEST_METHOD(Test_std_regex_token_iterator)
    {
      std::wstring const input{ LR"=(B:\my\videos\GeForceRecordings\somefile.mp4)=" };
      std::wregex slash(LR"=(\\)=");
      std::wsmatch mr;


      for (std::wsregex_token_iterator it(input.begin(), input.end(), slash, -1);
        it != std::wsregex_token_iterator(); ++it)
      {
        Logger::WriteMessage((L"<" + it->str() + L">").c_str());
        Logger::WriteMessage(L"\n");
      }

      Logger::WriteMessage(L"~~~~~~~~~~~~~~~~~~\n");

      std::wstring const input2{ LR"=(,a,b,c,d,,)=" };
      std::wregex recomma(L",");
      std::vector<std::wstring> tokens;
      std::copy(std::wsregex_token_iterator(input2.begin(), input2.end(), recomma, -1),
        std::wsregex_token_iterator(),
        std::back_inserter(tokens));

      Logger::WriteMessage(L"~~~~~~~~~~~~~~~~~~\n");

      for (auto const& token : tokens)
      {
        Logger::WriteMessage((L"<" + token + L">").c_str());
        Logger::WriteMessage(L"\n");
      }

      std::vector<std::wstring> tokens2;
      std::copy(std::wsregex_token_iterator(input.begin(), input.end(), slash, -1),
        std::wsregex_token_iterator(),
        std::back_inserter(tokens2));

      Logger::WriteMessage(L"~~~~~~~~~~~~~~~~~~\n");

      for (auto const& token : tokens2)
      {
        Logger::WriteMessage((L"<" + token + L">").c_str());
        Logger::WriteMessage(L"\n");
      }

      Logger::WriteMessage(L"~~~~~~~~~~~~~~~~~~\n");

      std::wstring const input3{ LR"=(/anydir/x/y/)=" };
      std::wregex re(LR"=([^\/]*)=");
      for (std::wsregex_iterator it(input3.begin(), input3.end(), re, std::regex_constants::match_flag_type());
        it != std::wsregex_iterator(); ++it)
      {
        Logger::WriteMessage((L"<" + it->str() + L">").c_str());
        Logger::WriteMessage(L"\n");
      }
    };

    TEST_METHOD(TestAnyDirTokenizer)
    {
      struct AnyDirTest
      {
        string_type input;
        strings_type expected;
      }
      tests[]
      {
        { LR"=(A:\**\a\aa.ext)=", { L"A:", L"a\\aa.ext" }},
        { LR"=(**\b\bb.ext)=", { L"", L"b\\bb.ext" }},
        { LR"=(c\cc.ext\**)=", { L"c\\cc.ext" }},
        { LR"=(**\d\**\dd.ext)=", { L"", L"d", L"dd.ext" }},
        { LR"=(**\e\**\ee.ext\**)=", { L"", L"e", L"ee.ext" }},
        // These special cases are handled through the eptError discriminator.
        { LR"=(F:\**\f\**\ff.ext\**\**)=", { L"F:", L"f", L"ff.ext", L"**" }},
        { LR"=(G:\**\g\**\**\gg.ext\**\**)=", { L"G:", L"g", L"**\\gg.ext", L"**" }},
        { LR"=(H:\**\**\h\**\**\hh.ext\**\**)=", { L"H:", L"**\\h", L"**\\hh.ext", L"**" }},
        { LR"=(**\**\i\ii.ext)=", { L"", L"**\\i\\ii.ext" }},
        { LR"=(j\**\**\j\jj.ext)=", { L"j", L"**\\j\\jj.ext" }},
        { LR"=(h\hh\**\**)=", { L"h\\hh", L"**" }},
      };
      AnyDirTokenizer tokenizer;
      for (auto const& test: tests)
      {
        std::vector<std::wstring> tokens;
        tokenizer(test.input, tokens);
        Assert::AreEqual(test.expected, tokens, test.input.c_str());
      }
    }

    TEST_METHOD(TestWinPathTokenizer)
    {
      std::wstring const input{ LR"=(B:\my\videos\somefile.mov)=" };
      WinPathTokenizer tokenizer;
      std::vector<std::wstring> tokens;
      std::vector<std::wstring> const expected{
        L"B:", L"my", L"videos", L"somefile.mov"
      };
      tokenizer(input, tokens);
      Assert::AreEqual(expected, tokens);
    }

    TEST_METHOD(TestNixPathTokenizer)
    {
      std::wstring const input{ LR"=(/my/videos/somefile.mp4)=" };
      NixPathTokenizer tokenizer;
      std::vector<std::wstring> tokens;
      std::vector<std::wstring> const expected{
        L"", L"my", L"videos", L"somefile.mp4"
      };
      tokenizer(input, tokens);
      Assert::AreEqual(expected, tokens);
    }

    TEST_METHOD(TestDriveTokenizer)
    {
      std::wstring const input{ LR"=(B:)=" };
      DriveTokenizer tokenizer;
      std::vector<std::wstring> tokens;
      std::vector<std::wstring> const expected{
        L"B"
      };
      tokenizer(input, tokens);
      Assert::AreEqual(expected, tokens);
    }

    TEST_METHOD(TestNameTokenizer)
    {
      std::wstring const input{ LR"=(..anyfile.part.ext)=" };
      NameTokenizer tokenizer;
      std::vector<std::wstring> tokens;
      std::vector<std::wstring> const expected{
        L"", L"", L"anyfile", L"part", L"ext"
      };
      tokenizer(input, tokens);
      Assert::AreEqual(expected, tokens);
    }


  };

}