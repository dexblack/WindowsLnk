#include "pch.h"

#include "TestData.h"

namespace
{
  typedef string_type const str_const;

  str_const ESCAPE(1, L'\\');
  str_const BEGIN{ L"^" };
  str_const ANY_CHAR{ L"." };
  str_const ONE_OR_MORE{ L"+" };
  str_const ZERO_OR_MORE{ L"*" };
  str_const END{ L"$" };
  str_const PATH_SEP(2, L'\\');
  str_const NAME_PART{ LR"=([^"*:<>?/\\|])=" };
  str_const PATH_NAME{ NAME_PART + ONE_OR_MORE };
  str_const DIRECTORY{ PATH_SEP + PATH_NAME };
  str_const SERVER{ PATH_SEP + DIRECTORY };
  str_const DRIVE_OR_SERVER{ L"(([A-Z]:)|" L"(" + SERVER + L")" L")" };
  str_const DIRECTORIES_OPTIONAL{ L"(" + DIRECTORY + L")" + ZERO_OR_MORE };
  str_const PATH_NAME_OPTIONAL{ NAME_PART + ZERO_OR_MORE };
  str_const DIR_END{ PATH_SEP + ANY_CHAR + ZERO_OR_MORE };
  str_const ANY_DIR{ BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL };
}

namespace RETest
{

  TestRegexBase::TestRegexPatterns
    TestRegexBase::path_tests
  {
    { // Empty string tests.
      L"",
      L"",
      {
        { L"", true },
      },
      false,
      false
    },{// Test whitespace trimming.
      L" \t\rA:\\x\\\n\v ",
      BEGIN + LR"=(A:\\x)=" + DIR_END + END,
      {
        { LR"=(A:\x)=", false },
        { LR"=(A:\x\)=", true },
      },
    },{
      LR"=(A:\one\)=",
      BEGIN + LR"=(A:\\one)=" + DIR_END + END,
      {
        { LR"=(A:\one)=", false },
        { LR"=(A:\one\)=", true },
        { LR"=(A:\one\anything.txt)=", true },
        { LR"=(A:\one\anything.)=", true },
        { LR"=(A:\one\.anything)=", true },
        { LR"=(A:\one\two)=", true },
        { LR"=(A:\one\two\)=", true },
        { LR"=(A:\one\two\anything.txt)=", true },
      },
    },{
      LR"=(B:\*)=",
      BEGIN + L"B:" + PATH_SEP + PATH_NAME + END,
      {
        { LR"=(B:\)=", false },
        { LR"=(B:\one)=", true },
        { LR"=(B:\two\a file)=", false },
        { LR"=(B:\two\a file.)=", false },
        { LR"=(B:\two\a file.txt)=", false },
        { LR"=(B:\two\anydir1\)=", false },
        { LR"=(B:\two\anydir2\a file)=", false },
      },
    },{
      LR"=(C:\*\my)=",
      BEGIN + L"C:" + PATH_SEP + PATH_NAME + PATH_SEP + L"my" + END,
      {
        { LR"=(C:\\my)=", false },
        { LR"=(C:\\my\)=", false },
        { LR"=(C:\one\my\)=", false },
        { LR"=(C:\one\my)=", true },
        { LR"=(C:\one\two\my)=", false },
      }
    },{
      LR"=(D:\*\my\)=",
      BEGIN + L"D:" + PATH_SEP + PATH_NAME + PATH_SEP + L"my" + DIR_END + END,
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
      BEGIN + L"E:" + PATH_SEP + PATH_NAME + PATH_SEP + L"my" + DIR_END + END,
      {
        { LR"=(E:\one\a file)=", false },
        { LR"=(E:\two\my\)=", true},
        { LR"=(E:\three\my.\a file)=", false },
        { LR"=(E:\four\my\adir\)=", true},
        { LR"=(E:\four\my\adir\a file)=", true},
      }
    },{
      LR"=(F:\**\my\)=",
      BEGIN + L"F:" + DIRECTORIES_OPTIONAL + PATH_SEP + L"my" + DIR_END + END,
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
      LR"=(\**\my\)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + LR"=(my)=" + DIR_END + END,
      {
        { LR"=(\\server\FDrive\my)=", false },
        { LR"=(\\server\FDrive\my\)=", true },
        { LR"=(\\server\FDrive\my\a file)=", true },
        { LR"=(\\server\FDrive\my\adir\)=", true },
        { LR"=(\\server\FDrive\my\adir\a file)=", true },
        { LR"=(\\server\FDrive\one\my\adir\a file)=", true },
        { LR"=(\\server\FDrive\two\two\my\adir\a file)=", true },
        { LR"=(\\server\FDrive\three\three\three\my)=", false },
        { LR"=(\\server\FDrive\three\three\three\my\)=", true },
      }
    },{
      LR"=(.git\)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + ESCAPE + LR"=(.git)=" + DIR_END + END,
      {
        { LR"=(G:\dev\.git\)=", true },
        { LR"=(G:\dev\.git)=", false },
        { LR"=(G:\dev\.git\hooks)=", true },
        { LR"=(G:\dev\not.git\hooks)=", false },
        { LR"=(\\server\GDrive\dev\not.git\hooks)=", false },
      }
    },{
      LR"=(Downloads\)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + L"Downloads" + DIR_END + END,
      {
        { LR"=(H:\Users\dex\Downloads\junk)=", true },
        { LR"=(\\server\share\Users\dex\Downloads\junk)=", true },
      }
    },{
      LR"=(~*\)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + L"~" + PATH_NAME + DIR_END + END,
      {
        { LR"=(I:\Users\dex\~)=", false },
        { LR"=(I:\Users\dex\~\)=", false },
        { LR"=(I:\Users\dex\~anydir)=", false },
        { LR"=(I:\Users\dex\~anydir\)=", true },
        { LR"=(I:\Users\dex\~anydir\anyfile)=", true },
        { LR"=(I:\Users\dex\~\anyfile)=", false },
        { LR"=(I:\Users\dex\~\anydir\)=", false },
        { LR"=(I:\Users\dex\~\anydir\anyfile)=", false },
        { LR"=(\\server\IDrive\Users\dex\~anydir\)=", true },
        { LR"=(\\server\IDrive\Users\dex\~anydir\anyfile)=", true },
        { LR"=(\\server\IDrive\Users\dex\~anydir\anydir2\)=", true },
        { LR"=(\\server\IDrive\Users\dex\~anydir\anydir2\anyfile)=", true },
      }
    },{
      LR"=(~*\**\*)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP +
      L"~" + PATH_NAME + DIRECTORIES_OPTIONAL + PATH_SEP + PATH_NAME + END,
      {
        { LR"=(J:\Users\dex\~)=", false },
        { LR"=(J:\Users\dex\~\)=", false },
        { LR"=(J:\Users\dex\~anydir)=", false },
        { LR"=(J:\Users\dex\~anydir\)=", false },
        { LR"=(J:\Users\dex\~anydir\anyfile)=", true },
        { LR"=(J:\Users\dex\~\anyfile)=", false },
        { LR"=(J:\Users\dex\~\anydir\)=", false },
        { LR"=(J:\Users\dex\~\anydir\anyfile)=", false },
        { LR"=(\\server\JDrive\Users\dex\~anydir\)=", false },
        { LR"=(\\server\JDrive\Users\dex\~anydir\)=", false },
        { LR"=(\\server\JDrive\Users\dex\~anydir\anyfile)=", true },
      }
    }, {
      LR"=(~*\**\)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP +
      L"~" + PATH_NAME + DIRECTORIES_OPTIONAL + DIR_END + END,
      {
        { LR"=(K:\Users\dex\~)=", false },
        { LR"=(K:\Users\dex\~\)=", false },
        { LR"=(K:\Users\dex\~anydir)=", false },
        { LR"=(K:\Users\dex\~anydir\)=", true },
        { LR"=(K:\Users\dex\~anydir\anyfile)=", true },
        { LR"=(K:\Users\dex\~\anyfile)=", false },
        { LR"=(K:\Users\dex\~\anydir\)=", false },
        { LR"=(K:\Users\dex\~\anydir\anyfile)=", false },
        { LR"=(\\server\KDrive\Users\dex\~anydir\)=", true },
        { LR"=(\\server\KDrive\Users\dex\~anydir\anyfile)=", true },
        { LR"=(\\server\KDrive\Users\dex\~anydir\anydir2\)=", true },
        { LR"=(\\server\KDrive\Users\dex\~anydir\anydir2\anyfile)=", true },
      }
    }, {
      LR"=(~\)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + L"~" + DIR_END + END,
      {
        { LR"=(L:\Users\dex\~)=", false },
        { LR"=(L:\Users\dex\~\)=", true },
        { LR"=(L:\Users\dex\~\anfile)=", true },
        { LR"=(L:\Users\dex\~\anydir\)=", true },
        { LR"=(\\server\LDrive\Users\dex\~)=", false },
        { LR"=(\\server\LDrive\Users\dex\~\)=", true },
        { LR"=(\\server\LDrive\Users\dex\~\anfile)=", true },
        { LR"=(\\server\LDrive\Users\dex\~\anydir\)=", true },
      }
    },{
      LR"=(~*)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + L"~" + PATH_NAME + END,
      {
        { LR"=(N:\Users\dex\~\junk.txt)=", false },
        { LR"=(N:\Users\~Anything\junk.txt)=", false },
        { LR"=(N:\Users\~Anything.txt)=", true },
        { LR"=(N:\Users\~.txt)=", true },
        { LR"=(N:\Users\~)=", false },
      }
    },{
      LR"=(~)=",
      BEGIN + DRIVE_OR_SERVER + DIRECTORIES_OPTIONAL + PATH_SEP + L"~" + END,
      {
        { LR"=(S:\One\~)=", true },
        { LR"=(S:\One\~\)=", false },
        { LR"=(S:\One\Two\~)=", true },
        { LR"=(\\Server1\~)=", true },
        { LR"=(\\Server2\~\)=", false },
        { LR"=(\\Server6\~a)=", false },
        { LR"=(\\Server3\somedir\~)=", true },
        { LR"=(\\Server4\somedir\~a)=", false },
        { LR"=(\\Server5\somedir\~\a)=", false },
        { LR"=(\\Server6\somedir\~\~)=", true },
        { LR"=(\\Server7\somedir\~\a~)=", false },
        { LR"=(\\Server7\somedir\~\~a)=", false },
      }
    },
  };

  TestRegexIncludeExcludeBase::TestIncludeExcludePatterns
    TestRegexIncludeExcludeBase::ie_tests
  {
    // unfiltered,
    // kept,
    // dropped,
    // include,
    // exclude
    //
    { // empty list test
      {},
      {},
      {},
      {}
    }, {
      {L"abc", L"def", L"ghi"},
      {L"abc", L"def", L"ghi"},
      {},
      {L"abc"},
      {L"abc"}
    }, {
      {L"abc", L"abcdef", L"abcdefghi"},
      {L"abc", L"abcdef", L"abcdefghi"},
      {L"abc"},
      {L"c"},
      {L"d"}
    }, {
      {L"abcd", L"abcde", L"abcdef"},
      {L"abcde", L"abcdef"},
      {},
      {L"e"},
      {L"d"}
    }, {
      {L"abcdeg", L"abcdef", L"abcdefg"},
      {L"abcdeg", L"abcdefg"},
      {L"abcdeg"},
      {L"g"},
      {L"f"}
    }, {
      {L"abcde", L"abcdef", L"abcdefg", L"abcdeg"},
      {L"abcde", L"abcdefg", L"abcdeg"},
      {L"abcdeg"},
      {L"g"},
      {L"f"}
    }, {
      {L"jkl", L"mno"},
      {L"jkl", L"mno"},
      {},
      {L".*"},
      {L".*"}
    }, {
      {L"pqr", L"pqrs", L"pqrsx", L"pqrstuvw", L"uvw", L"xw", L"w"},
      {L"pqr", L"pqrs", L"pqrsx", L"pqrstuvw", L"w"},
      {L"pqrsx"},
      {L".*s.+", L".*t"},
      {L".*uv.+", L".+w"}
    },
  };

  TestPatternToRegExStrBase::TestPatterns
    TestPatternToRegExStrBase::pattern_tests
  {
    // git_re, win_re, ok, stop, finish
    {L"", L"",true,true,false},
    {L"B:*", L"",false},
    {L"C:?", L"",false},
    {L"D:name", L"",false},

    {L"a", ANY_DIR + PATH_SEP + L"a" + END, true},

    {LR"=(\)=", ANY_DIR + LR"=(\\)=" + END, true, false},
    {LR"=(\\)=", BEGIN + PATH_SEP + PATH_SEP + END , true},
    {LR"=(\\\)=", BEGIN + LR"=(ERROR)=" + END, false, false},

    {LR"=(\a)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + L"a" + END, true},
    {LR"=(\\a)=", BEGIN + PATH_SEP + PATH_SEP + L"a" + END, true},
    {LR"=(\\\a)=", BEGIN + LR"=(ERROR)=" + END, false},

    {LR"=(\b\)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + LR"=(b)=" + DIR_END + END, true},

    {LR"=(\c\\c)=", BEGIN + LR"=(ERROR)=" + END, false},
    {LR"=(\d\d\\)=", BEGIN + LR"=(ERROR)=" + END, false},

    {LR"=(\e\e\e\)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + L"e" + PATH_SEP + L"e" + PATH_SEP + L"e" + DIR_END + END, true},
    {LR"=(\f\f\f)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + LR"=(f)=" + PATH_SEP + LR"=(f)=" + PATH_SEP + LR"=(f)=" + END, true},

    {LR"=(*)=", ANY_DIR + PATH_SEP + PATH_NAME + END, true},
    {LR"=(\*)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + PATH_NAME + END, true},
    {LR"=(*\)=", ANY_DIR + PATH_SEP + PATH_NAME + DIR_END + END, true},
    {LR"=(\*\)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + PATH_NAME + DIR_END + END, true},

    {LR"=(.)=", ANY_DIR + PATH_SEP + ESCAPE + ANY_CHAR + END, true},
    {LR"=(..)=", ANY_DIR + PATH_SEP + ESCAPE + ANY_CHAR + ESCAPE + ANY_CHAR + END, true},
    {LR"=(.\)=", ANY_DIR + PATH_SEP + ESCAPE + ANY_CHAR + DIR_END + END, true},
    {LR"=(..\)=", ANY_DIR + PATH_SEP + ESCAPE + ANY_CHAR + ESCAPE + ANY_CHAR + DIR_END + END, true},

    {LR"=(?)=", ANY_DIR + PATH_SEP + ANY_CHAR + END, true},
    {LR"=(\?)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + ANY_CHAR + END, true},
    {LR"=(?\)=", ANY_DIR + PATH_SEP + ANY_CHAR + DIR_END + END, true},
    {LR"=(\?\)=", BEGIN + DRIVE_OR_SERVER + PATH_SEP + ANY_CHAR + DIR_END + END, true},

    {LR"=(**)=", ANY_DIR + END, false},
    {LR"=(**\)=", ANY_DIR + DIR_END + END, true},
    {LR"=(\**)=", ANY_DIR + END, true},
    {LR"=(\**\)=", ANY_DIR + DIR_END + END, true},
    {LR"=(\\**\)=", BEGIN + SERVER + DIRECTORIES_OPTIONAL + DIR_END + END, true},
    //{L"*", LR"=()=",true},
  };

  TestRegexIgnoreBase::TestRegexIgnoreBasePatterns
    TestRegexIgnoreBase::ignore_tests
  {
    // items, ignore patterns, result, stop, finish
    { L"Empty list test.",
      {},{},{}, false
    },
    { L"Single letter file name.",
      {
        L"a",
        LR"=(A:\a)=",
        LR"=(A:\anydir\a)=",
        LR"=(\\serverC\a)=",
        LR"=(\\serverC\anydir\a)=",
      },
      {
        {L"a"},
      },
      {
        {L"a"},
      },
      false, true
    },
    { L"Any file",
      {
        L"anyfile.ext",
        LR"=(C:\anyfile.ext)=",
        LR"=(C:\anydir\anyfile.ext)=",
        LR"=(\\serverC\anyfile.ext)=",
        LR"=(\\serverC\anydir\anyfile.ext)=",
      },
      {
        {L"*"},
      },
      {},
      false,
    },
    { L"test name",
      {},
      {{}},
      {},
      false
    },
    { L"test name",
      {},
      {{}},
      {},
      false
    },
  };

}
