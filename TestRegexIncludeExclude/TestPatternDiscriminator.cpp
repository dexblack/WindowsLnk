#include "pch.h"

#include <algorithm>

#include "TestPatternDiscriminator.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
  typedef RETest::strings_type strings_type;

  template<> inline string_type ToString<ePatternType>(ePatternType const& pid)
  {
    string_type::value_type const* pidName[]{
      L"None",
      L"Error",
      L"File",
      L"Directory",
      L"PathWin",
      L"PathNix",
    };
    string_type out{ L"{ " };
    PatternId x = static_cast<PatternId>(pid);
    int const N = sizeof(pidName) / sizeof(pidName[0]);
    for (int j = 0; j < N; ++j)
    {
      if (x & (1 << j))
      {
        out.append(pidName[j]);
        out.append(L", ");
      }
    }
    if (out.size() > 2)
    {
      out[out.size() - 2] = L' ';
    }
    *out.rbegin() = L'}';
    return out;
  }
}

namespace RETest
{

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_errors
  { {
  { L"", { EPT(None) } },
  { L" ", { EPT(None), EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { L" \r\t\f\v ", { EPT(None), EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { L" rtfv ", { EPT(File), EPT(PathWin), EPT(PathNix) } }, // not an error!
  { LR"=(.)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(...)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(A:\.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(B:\..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(C:\.\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(D:\..\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(.\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(..\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(/.)=", { EPT(Error), EPT(PathNix) } },
  { LR"=(/..)=", { EPT(Error), EPT(PathNix) } },
  { LR"=(/./)=", { EPT(Error), EPT(Directory), EPT(PathNix) } },
  { LR"=(/../)=", { EPT(Error), EPT(Directory), EPT(PathNix) } },
  { LR"=(./)=", { EPT(Error), EPT(Directory), EPT(PathNix) } },
  { LR"=(../)=", { EPT(Error), EPT(Directory), EPT(PathNix) } },
  { LR"=(/E/./)=", { EPT(Error), EPT(Directory), EPT(PathNix) } },
  { LR"=(/E/../)=", { EPT(Error), EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(AUX)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(CON)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(COM0)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(LPT0)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(NUL)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(PRN)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(X:\AUX)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(X:\CON)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(X:\COM0)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(X:\LPT0)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(X:\NUL)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(X:\PRN)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(X:\anydir\AUX\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\CON\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\COM0\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\LPT0\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\NUL\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\PRN\)=", { EPT(Error), EPT(Directory), EPT(PathWin) } },

  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(Z:)=", { EPT(Error) } },
  { LR"=(?:)=", { EPT(Error) } },
  { LR"=(\?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:)=", { EPT(Error) } },
  { LR"=(Z:*.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:.*err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:.e*rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:.err*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:?.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:.?err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:.e?rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Z:.err?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(?:*.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.*err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.e*rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.err*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:?.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.?err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.e?rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.err?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(*:*.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.*err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.e*rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.err*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:?.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.?err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.e?rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.err?)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?:*.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.*err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.e*rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(?:.err*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:*.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:*..err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:.*.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:..*err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:..e*rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(*:..err*)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?:\\*.err)=", { EPT(Error) } },
  { LR"=(?:\\.*err)=", { EPT(Error) } },
  { LR"=(?:\\.e*rr)=", { EPT(Error) } },
  { LR"=(?:\\.err*)=", { EPT(Error) } },
  { LR"=(*:\\*.err)=", { EPT(Error) } },
  { LR"=(*:\\*..err)=", { EPT(Error) } },
  { LR"=(*:\\.*.err)=", { EPT(Error) } },
  { LR"=(*:\\..*err)=", { EPT(Error) } },
  { LR"=(*:\\..e*rr)=", { EPT(Error) } },
  { LR"=(*:\\..err*)=", { EPT(Error) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?:\*.\\an.err)=", { EPT(Error) } },
  { LR"=(?:\.\\an.*err)=", { EPT(Error) } },
  { LR"=(?:\.\\an.e*rr)=", { EPT(Error) } },
  { LR"=(?:\.\\an.err*)=", { EPT(Error) } },
  { LR"=(*:\*.\\an.err)=", { EPT(Error) } },
  { LR"=(*:\*.\\an..\\an.err)=", { EPT(Error) } },
  { LR"=(*:\.\\an.*.\\an.err)=", { EPT(Error) } },
  { LR"=(*:\.\\an..\\an.*err)=", { EPT(Error) } },
  { LR"=(*:\.\\an..\\an.e*rr)=", { EPT(Error) } },
  { LR"=(*:\.\\an..\\an.err*)=", { EPT(Error) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(**.err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(**..err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.**.err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..**err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..e**rr)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..err**)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(***.err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(***..err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.***.err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..***err)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..e***rr)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..err***)=", { EPT(Error), EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(A:\**.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(B:\**..err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(C:\.**.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(D:\..**err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(E:\..e**rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(F:\..err**)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(G:\***.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(H:\***..err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(I:\.***.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(J:\..***err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(K:\..e***rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(L:\..err***)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(M:\.**\.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(N:\..**\err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(O:\..e**\rr)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(P:\***\.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(Q:\***\..err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(R:\.***\.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(S:\..***\err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(T:\..e***\rr)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(U:\**\**\.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(V:\**\**\err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(W:\err\**\**)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\\X\**\**\.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\\Y\anydir\**\**\.err)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\\Z\anydir\.err\**\**)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/X/**/**/.err)=", { EPT(Error), EPT(PathNix) } },
  { LR"=(/Y/anydir/**/**/.err)=", { EPT(Error), EPT(PathNix) } },
  { LR"=(/Z/anydir/.err/**/**)=", { EPT(Error), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  { LR"=(A:\**\A\**\AA.EXT\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(B:\**\B\****\BB.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(C:\**\**\C\**\**\CC.EXT\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(**\**\D\DD.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(E\**\**\E\EE.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(F\FF\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\**\**\G\GG.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\H\**\**\H\HH.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\I\II\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(J:\**\J\**\JJ.EXT\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(K:\**\K\**\**\KK.EXT\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(L:\**\**\L\**\**\LL.EXT\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(**\**\M\MM.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(N\**\**\N\NN.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(O\OO\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\**\**\P\PP.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\Q\**\**\Q\QQ.EXT)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\R\RR\**\**)=", { EPT(Error), EPT(PathWin) }},
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(a:\**\a\**\aa.ext\**\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(b:\**\b\**\**\**\bb.ext)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(c:\**\**\**\c\**\**\cc.ext\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(**\**\\**d\dd.ext)=", { EPT(Error) }},
  { LR"=(e\**\**\\**e\ee.ext)=", { EPT(Error) }},
  { LR"=(f\ff\**\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\**\**\\**g\gg.ext)=", { EPT(Error) }},
  { LR"=(\\h\**\**\\**h\hh.ext)=", { EPT(Error) }},
  { LR"=(\\i\ii\**\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(j:\**\j\**\jj.ext\**\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(k:\**\k\**\**\**\kk.ext\**\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(l:\**\**\\**l\**\**\**\ll.ext\**\**\**)=", { EPT(Error) }},
  { LR"=(**\**\\**m\mm.ext)=", { EPT(Error) }},
  { LR"=(n\**\**\\**n\nn.ext)=", { EPT(Error) }},
  { LR"=(o\oo\**\**\**)=", { EPT(Error), EPT(PathWin) }},
  { LR"=(\\**\**\\**p\pp.ext)=", { EPT(Error) }},
  { LR"=(\\q\**\**\\**q\qq.ext)=", { EPT(Error) }},
  { LR"=(\\r\rr\**\**\**)=", { EPT(Error), EPT(PathWin) }},


  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_file
  { {
  { LR"=(?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.a)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..aa)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*..)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.b)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..cc)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*.d)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*..ee)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*?.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*?..gg)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?*.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?*..hh)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(??.i)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(??..jj)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.?l)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?lll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..l?ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*m)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*.mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*mmm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..m*mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..mmm*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*?n)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*?.nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*?nnn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..n*?nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..nnn*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*o)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*.oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?*ooo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..o?*oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..ooo?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??p)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??.pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..??ppp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..p??pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..ppp??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(r?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r.a)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..aa)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r?.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r?..)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r*.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*..)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r?.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r?..*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r?.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r?..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r*.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r?.b)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r?..cc)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*.d)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*..ee)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*?.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r*?..gg)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r?*.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r?*..hh)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r??.i)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r??..jj)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(r.?l)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?.ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..?lll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..l?ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*m)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*.mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..*mmm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..m*mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..mmm*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*?n)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.*?.nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..*?nnn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..n*?nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..nnn*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?*o)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.?*.oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..?*ooo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..o?*oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..ooo?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.??p)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r.??.pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..??ppp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..p??pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(r..ppp??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?s)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.s.aa)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?s.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s..)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*s.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s..)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?s.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s..*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?s.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*s.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s..?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?s.b)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s..cc)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s.d)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s..ee)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s?.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*s?..gg)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s*.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s*..hh)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s?.i)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?s?..jj)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.s?l)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?.ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.?lll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.l?ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*m)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*.mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.*mmm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.m*mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.mmm*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*?n)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s*?.nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.*?nnn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.n*?nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.nnn*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?*o)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s?*.oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.?*ooo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.o?*oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.ooo?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s??p)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s??.pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.??ppp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.p??pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.s.ppp??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?.t.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..t?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*.t.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t.)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..t*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.t*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?.t.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t.*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..t?*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.t?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?.t.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..t??)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*.t?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*.t.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t.?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..t*?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.yb)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?.t.cc)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*.yd)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*.t.ee)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*?t.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*?t..gg)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?*t.f)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?*t..hh)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(??t.i)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(??t..jj)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(.?yl)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t.ll)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t.mm)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t?n)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*t?.nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t*o)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t*.oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t?p)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?t?.pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(.?.u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.*.u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.*u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..u*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.u*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?u*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.?u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..u?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.u?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?u?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*.?u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*..u?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*?u)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*.u?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*u?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*?.u.gg)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?*.u.hh)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(??.u.jj)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.*?u.nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*u.oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??u.pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?..v*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*v)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.v*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?v*)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.?v)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..v?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??v)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?.v?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..?v?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*.?v)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*..v?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*?v)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.*.v?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(..*v?)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(*?.v.gg)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?*.v.hh)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(??.v.jj)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(.*?v.nn)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.?*v.oo)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(.??v.pp)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(*.?.w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(*..?w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(??..w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?.?.w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..?w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  { LR"=(?.*.w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },
  { LR"=(?..*w)=", { EPT(File), EPT(PathWin), EPT(PathNix) } },

  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_win_file
  { {
  { LR"=(X:\?)=", { EPT(PathWin) } },
  { LR"=(X:\*)=", { EPT(PathWin) } },

  { LR"=(X:\.a)=", { EPT(PathWin) } },
  { LR"=(X:\..aa)=", { EPT(PathWin) } },

  { LR"=(X:\?.)=", { EPT(PathWin) } },
  { LR"=(X:\?..)=", { EPT(PathWin) } },
  { LR"=(X:\.?)=", { EPT(PathWin) } },
  { LR"=(X:\.?.)=", { EPT(PathWin) } },
  { LR"=(X:\..?)=", { EPT(PathWin) } },

  { LR"=(X:\*.)=", { EPT(PathWin) } },
  { LR"=(X:\*..)=", { EPT(PathWin) } },
  { LR"=(X:\.*)=", { EPT(PathWin) } },
  { LR"=(X:\.*.)=", { EPT(PathWin) } },
  { LR"=(X:\..*)=", { EPT(PathWin) } },

  { LR"=(X:\?.*)=", { EPT(PathWin) } },
  { LR"=(X:\?..*)=", { EPT(PathWin) } },
  { LR"=(X:\.?*)=", { EPT(PathWin) } },
  { LR"=(X:\.?.*)=", { EPT(PathWin) } },
  { LR"=(X:\..?*)=", { EPT(PathWin) } },

  { LR"=(X:\?.?)=", { EPT(PathWin) } },
  { LR"=(X:\?..?)=", { EPT(PathWin) } },
  { LR"=(X:\.??)=", { EPT(PathWin) } },
  { LR"=(X:\.?.?)=", { EPT(PathWin) } },
  { LR"=(X:\..??)=", { EPT(PathWin) } },

  { LR"=(X:\*.?)=", { EPT(PathWin) } },
  { LR"=(X:\*..?)=", { EPT(PathWin) } },
  { LR"=(X:\.*?)=", { EPT(PathWin) } },
  { LR"=(X:\.*.?)=", { EPT(PathWin) } },
  { LR"=(X:\..*?)=", { EPT(PathWin) } },

  { LR"=(X:\?.b)=", { EPT(PathWin) } },
  { LR"=(X:\?..cc)=", { EPT(PathWin) } },
  { LR"=(X:\*.d)=", { EPT(PathWin) } },
  { LR"=(X:\*..ee)=", { EPT(PathWin) } },
  { LR"=(X:\*?.f)=", { EPT(PathWin) } },
  { LR"=(X:\*?..gg)=", { EPT(PathWin) } },
  { LR"=(X:\?*.f)=", { EPT(PathWin) } },
  { LR"=(X:\?*..hh)=", { EPT(PathWin) } },
  { LR"=(X:\??.i)=", { EPT(PathWin) } },
  { LR"=(X:\??..jj)=", { EPT(PathWin) } },

  { LR"=(X:\.?l)=", { EPT(PathWin) } },
  { LR"=(X:\.?.ll)=", { EPT(PathWin) } },
  { LR"=(X:\..?lll)=", { EPT(PathWin) } },
  { LR"=(X:\..l?ll)=", { EPT(PathWin) } },
  { LR"=(X:\.*m)=", { EPT(PathWin) } },
  { LR"=(X:\.*.mm)=", { EPT(PathWin) } },
  { LR"=(X:\..*mmm)=", { EPT(PathWin) } },
  { LR"=(X:\..m*mm)=", { EPT(PathWin) } },
  { LR"=(X:\..mmm*)=", { EPT(PathWin) } },
  { LR"=(X:\.*?n)=", { EPT(PathWin) } },
  { LR"=(X:\.*?.nn)=", { EPT(PathWin) } },
  { LR"=(X:\..*?nnn)=", { EPT(PathWin) } },
  { LR"=(X:\..n*?nn)=", { EPT(PathWin) } },
  { LR"=(X:\..nnn*?)=", { EPT(PathWin) } },
  { LR"=(X:\.?*o)=", { EPT(PathWin) } },
  { LR"=(X:\.?*.oo)=", { EPT(PathWin) } },
  { LR"=(X:\..?*ooo)=", { EPT(PathWin) } },
  { LR"=(X:\..o?*oo)=", { EPT(PathWin) } },
  { LR"=(X:\..ooo?*)=", { EPT(PathWin) } },
  { LR"=(X:\.??p)=", { EPT(PathWin) } },
  { LR"=(X:\.??.pp)=", { EPT(PathWin) } },
  { LR"=(X:\..??ppp)=", { EPT(PathWin) } },
  { LR"=(X:\..p??pp)=", { EPT(PathWin) } },
  { LR"=(X:\..ppp??)=", { EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\r?)=", { EPT(PathWin) } },
  { LR"=(X:\r*)=", { EPT(PathWin) } },

  { LR"=(X:\r.a)=", { EPT(PathWin) } },
  { LR"=(X:\r..aa)=", { EPT(PathWin) } },

  { LR"=(X:\r?.)=", { EPT(PathWin) } },
  { LR"=(X:\r?..)=", { EPT(PathWin) } },
  { LR"=(X:\r.?)=", { EPT(PathWin) } },
  { LR"=(X:\r.?.)=", { EPT(PathWin) } },
  { LR"=(X:\r..?)=", { EPT(PathWin) } },

  { LR"=(X:\r*.)=", { EPT(PathWin) } },
  { LR"=(X:\r*..)=", { EPT(PathWin) } },
  { LR"=(X:\r.*)=", { EPT(PathWin) } },
  { LR"=(X:\r.*.)=", { EPT(PathWin) } },
  { LR"=(X:\r..*)=", { EPT(PathWin) } },

  { LR"=(X:\r?.*)=", { EPT(PathWin) } },
  { LR"=(X:\r?..*)=", { EPT(PathWin) } },
  { LR"=(X:\r.?*)=", { EPT(PathWin) } },
  { LR"=(X:\r.?.*)=", { EPT(PathWin) } },
  { LR"=(X:\r..?*)=", { EPT(PathWin) } },

  { LR"=(X:\r?.?)=", { EPT(PathWin) } },
  { LR"=(X:\r?..?)=", { EPT(PathWin) } },
  { LR"=(X:\r.??)=", { EPT(PathWin) } },
  { LR"=(X:\r.?.?)=", { EPT(PathWin) } },
  { LR"=(X:\r..??)=", { EPT(PathWin) } },

  { LR"=(X:\r*.?)=", { EPT(PathWin) } },
  { LR"=(X:\r*..?)=", { EPT(PathWin) } },
  { LR"=(X:\r.*?)=", { EPT(PathWin) } },
  { LR"=(X:\r.*.?)=", { EPT(PathWin) } },
  { LR"=(X:\r..*?)=", { EPT(PathWin) } },

  { LR"=(X:\r?.b)=", { EPT(PathWin) } },
  { LR"=(X:\r?..cc)=", { EPT(PathWin) } },
  { LR"=(X:\r*.d)=", { EPT(PathWin) } },
  { LR"=(X:\r*..ee)=", { EPT(PathWin) } },
  { LR"=(X:\r*?.f)=", { EPT(PathWin) } },
  { LR"=(X:\r*?..gg)=", { EPT(PathWin) } },
  { LR"=(X:\r?*.f)=", { EPT(PathWin) } },
  { LR"=(X:\r?*..hh)=", { EPT(PathWin) } },
  { LR"=(X:\r??.i)=", { EPT(PathWin) } },
  { LR"=(X:\r??..jj)=", { EPT(PathWin) } },

  { LR"=(X:\r.?l)=", { EPT(PathWin) } },
  { LR"=(X:\r.?.ll)=", { EPT(PathWin) } },
  { LR"=(X:\r..?lll)=", { EPT(PathWin) } },
  { LR"=(X:\r..l?ll)=", { EPT(PathWin) } },
  { LR"=(X:\r.*m)=", { EPT(PathWin) } },
  { LR"=(X:\r.*.mm)=", { EPT(PathWin) } },
  { LR"=(X:\r..*mmm)=", { EPT(PathWin) } },
  { LR"=(X:\r..m*mm)=", { EPT(PathWin) } },
  { LR"=(X:\r..mmm*)=", { EPT(PathWin) } },
  { LR"=(X:\r.*?n)=", { EPT(PathWin) } },
  { LR"=(X:\r.*?.nn)=", { EPT(PathWin) } },
  { LR"=(X:\r..*?nnn)=", { EPT(PathWin) } },
  { LR"=(X:\r..n*?nn)=", { EPT(PathWin) } },
  { LR"=(X:\r..nnn*?)=", { EPT(PathWin) } },
  { LR"=(X:\r.?*o)=", { EPT(PathWin) } },
  { LR"=(X:\r.?*.oo)=", { EPT(PathWin) } },
  { LR"=(X:\r..?*ooo)=", { EPT(PathWin) } },
  { LR"=(X:\r..o?*oo)=", { EPT(PathWin) } },
  { LR"=(X:\r..ooo?*)=", { EPT(PathWin) } },
  { LR"=(X:\r.??p)=", { EPT(PathWin) } },
  { LR"=(X:\r.??.pp)=", { EPT(PathWin) } },
  { LR"=(X:\r..??ppp)=", { EPT(PathWin) } },
  { LR"=(X:\r..p??pp)=", { EPT(PathWin) } },
  { LR"=(X:\r..ppp??)=", { EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?s)=", { EPT(PathWin) } },
  { LR"=(X:\*s)=", { EPT(PathWin) } },

  { LR"=(X:\.s.aa)=", { EPT(PathWin) } },

  { LR"=(X:\?s.)=", { EPT(PathWin) } },
  { LR"=(X:\?s..)=", { EPT(PathWin) } },
  { LR"=(X:\.s?)=", { EPT(PathWin) } },
  { LR"=(X:\.s?.)=", { EPT(PathWin) } },
  { LR"=(X:\.s.?)=", { EPT(PathWin) } },

  { LR"=(X:\*s.)=", { EPT(PathWin) } },
  { LR"=(X:\*s..)=", { EPT(PathWin) } },
  { LR"=(X:\.s*)=", { EPT(PathWin) } },
  { LR"=(X:\.s*.)=", { EPT(PathWin) } },
  { LR"=(X:\.s.*)=", { EPT(PathWin) } },

  { LR"=(X:\?s.*)=", { EPT(PathWin) } },
  { LR"=(X:\?s..*)=", { EPT(PathWin) } },
  { LR"=(X:\.s?*)=", { EPT(PathWin) } },
  { LR"=(X:\.s?.*)=", { EPT(PathWin) } },
  { LR"=(X:\.s.?*)=", { EPT(PathWin) } },

  { LR"=(X:\?s.?)=", { EPT(PathWin) } },
  { LR"=(X:\?s..?)=", { EPT(PathWin) } },
  { LR"=(X:\.s??)=", { EPT(PathWin) } },
  { LR"=(X:\.s?.?)=", { EPT(PathWin) } },
  { LR"=(X:\.s.??)=", { EPT(PathWin) } },

  { LR"=(X:\*s.?)=", { EPT(PathWin) } },
  { LR"=(X:\*s..?)=", { EPT(PathWin) } },
  { LR"=(X:\.s*?)=", { EPT(PathWin) } },
  { LR"=(X:\.s*.?)=", { EPT(PathWin) } },
  { LR"=(X:\.s.*?)=", { EPT(PathWin) } },

  { LR"=(X:\?s.b)=", { EPT(PathWin) } },
  { LR"=(X:\?s..cc)=", { EPT(PathWin) } },
  { LR"=(X:\*s.d)=", { EPT(PathWin) } },
  { LR"=(X:\*s..ee)=", { EPT(PathWin) } },
  { LR"=(X:\*s?.f)=", { EPT(PathWin) } },
  { LR"=(X:\*s?..gg)=", { EPT(PathWin) } },
  { LR"=(X:\?s*.f)=", { EPT(PathWin) } },
  { LR"=(X:\?s*..hh)=", { EPT(PathWin) } },
  { LR"=(X:\?s?.i)=", { EPT(PathWin) } },
  { LR"=(X:\?s?..jj)=", { EPT(PathWin) } },

  { LR"=(X:\.s?l)=", { EPT(PathWin) } },
  { LR"=(X:\.s?.ll)=", { EPT(PathWin) } },
  { LR"=(X:\.s.?lll)=", { EPT(PathWin) } },
  { LR"=(X:\.s.l?ll)=", { EPT(PathWin) } },
  { LR"=(X:\.s*m)=", { EPT(PathWin) } },
  { LR"=(X:\.s*.mm)=", { EPT(PathWin) } },
  { LR"=(X:\.s.*mmm)=", { EPT(PathWin) } },
  { LR"=(X:\.s.m*mm)=", { EPT(PathWin) } },
  { LR"=(X:\.s.mmm*)=", { EPT(PathWin) } },
  { LR"=(X:\.s*?n)=", { EPT(PathWin) } },
  { LR"=(X:\.s*?.nn)=", { EPT(PathWin) } },
  { LR"=(X:\.s.*?nnn)=", { EPT(PathWin) } },
  { LR"=(X:\.s.n*?nn)=", { EPT(PathWin) } },
  { LR"=(X:\.s.nnn*?)=", { EPT(PathWin) } },
  { LR"=(X:\.s?*o)=", { EPT(PathWin) } },
  { LR"=(X:\.s?*.oo)=", { EPT(PathWin) } },
  { LR"=(X:\.s.?*ooo)=", { EPT(PathWin) } },
  { LR"=(X:\.s.o?*oo)=", { EPT(PathWin) } },
  { LR"=(X:\.s.ooo?*)=", { EPT(PathWin) } },
  { LR"=(X:\.s??p)=", { EPT(PathWin) } },
  { LR"=(X:\.s??.pp)=", { EPT(PathWin) } },
  { LR"=(X:\.s.??ppp)=", { EPT(PathWin) } },
  { LR"=(X:\.s.p??pp)=", { EPT(PathWin) } },
  { LR"=(X:\.s.ppp??)=", { EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?.t.)=", { EPT(PathWin) } },
  { LR"=(X:\.?t)=", { EPT(PathWin) } },
  { LR"=(X:\.?t.)=", { EPT(PathWin) } },
  { LR"=(X:\..t?)=", { EPT(PathWin) } },

  { LR"=(X:\*.t.)=", { EPT(PathWin) } },
  { LR"=(X:\.*t)=", { EPT(PathWin) } },
  { LR"=(X:\.*t.)=", { EPT(PathWin) } },
  { LR"=(X:\..t*)=", { EPT(PathWin) } },

  { LR"=(X:\?.t*)=", { EPT(PathWin) } },
  { LR"=(X:\?.t.*)=", { EPT(PathWin) } },
  { LR"=(X:\.?t*)=", { EPT(PathWin) } },
  { LR"=(X:\.?t.*)=", { EPT(PathWin) } },
  { LR"=(X:\..t?*)=", { EPT(PathWin) } },

  { LR"=(X:\?.t?)=", { EPT(PathWin) } },
  { LR"=(X:\?.t.?)=", { EPT(PathWin) } },
  { LR"=(X:\.?t?)=", { EPT(PathWin) } },
  { LR"=(X:\.?t.?)=", { EPT(PathWin) } },
  { LR"=(X:\..t??)=", { EPT(PathWin) } },

  { LR"=(X:\*.t?)=", { EPT(PathWin) } },
  { LR"=(X:\*.t.?)=", { EPT(PathWin) } },
  { LR"=(X:\.*t?)=", { EPT(PathWin) } },
  { LR"=(X:\.*t.?)=", { EPT(PathWin) } },
  { LR"=(X:\..t*?)=", { EPT(PathWin) } },

  { LR"=(X:\?.yb)=", { EPT(PathWin) } },
  { LR"=(X:\?.t.cc)=", { EPT(PathWin) } },
  { LR"=(X:\*.yd)=", { EPT(PathWin) } },
  { LR"=(X:\*.t.ee)=", { EPT(PathWin) } },
  { LR"=(X:\*?t.f)=", { EPT(PathWin) } },
  { LR"=(X:\*?t..gg)=", { EPT(PathWin) } },
  { LR"=(X:\?*t.f)=", { EPT(PathWin) } },
  { LR"=(X:\?*t..hh)=", { EPT(PathWin) } },
  { LR"=(X:\??t.i)=", { EPT(PathWin) } },
  { LR"=(X:\??t..jj)=", { EPT(PathWin) } },

  { LR"=(X:\.?yl)=", { EPT(PathWin) } },
  { LR"=(X:\.?t.ll)=", { EPT(PathWin) } },
  { LR"=(X:\.*t.mm)=", { EPT(PathWin) } },
  { LR"=(X:\.*t?n)=", { EPT(PathWin) } },
  { LR"=(X:\.*t?.nn)=", { EPT(PathWin) } },
  { LR"=(X:\.?t*o)=", { EPT(PathWin) } },
  { LR"=(X:\.?t*.oo)=", { EPT(PathWin) } },
  { LR"=(X:\.?t?p)=", { EPT(PathWin) } },
  { LR"=(X:\.?t?.pp)=", { EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\.?.u)=", { EPT(PathWin) } },
  { LR"=(X:\..?u)=", { EPT(PathWin) } },

  { LR"=(X:\.*.u)=", { EPT(PathWin) } },
  { LR"=(X:\..*u)=", { EPT(PathWin) } },

  { LR"=(X:\?.*u)=", { EPT(PathWin) } },
  { LR"=(X:\?..u*)=", { EPT(PathWin) } },
  { LR"=(X:\.?*u)=", { EPT(PathWin) } },
  { LR"=(X:\.?.u*)=", { EPT(PathWin) } },
  { LR"=(X:\..?u*)=", { EPT(PathWin) } },

  { LR"=(X:\?.?u)=", { EPT(PathWin) } },
  { LR"=(X:\?..u?)=", { EPT(PathWin) } },
  { LR"=(X:\.??u)=", { EPT(PathWin) } },
  { LR"=(X:\.?.u?)=", { EPT(PathWin) } },
  { LR"=(X:\..?u?)=", { EPT(PathWin) } },

  { LR"=(X:\*.?u)=", { EPT(PathWin) } },
  { LR"=(X:\*..u?)=", { EPT(PathWin) } },
  { LR"=(X:\.*?u)=", { EPT(PathWin) } },
  { LR"=(X:\.*.u?)=", { EPT(PathWin) } },
  { LR"=(X:\..*u?)=", { EPT(PathWin) } },

  { LR"=(X:\*?.u.gg)=", { EPT(PathWin) } },
  { LR"=(X:\?*.u.hh)=", { EPT(PathWin) } },
  { LR"=(X:\??.u.jj)=", { EPT(PathWin) } },

  { LR"=(X:\.*?u.nn)=", { EPT(PathWin) } },
  { LR"=(X:\.?*u.oo)=", { EPT(PathWin) } },
  { LR"=(X:\.??u.pp)=", { EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?..v*)=", { EPT(PathWin) } },
  { LR"=(X:\.?*v)=", { EPT(PathWin) } },
  { LR"=(X:\.?.v*)=", { EPT(PathWin) } },
  { LR"=(X:\..?v*)=", { EPT(PathWin) } },

  { LR"=(X:\?.?v)=", { EPT(PathWin) } },
  { LR"=(X:\?..v?)=", { EPT(PathWin) } },
  { LR"=(X:\.??v)=", { EPT(PathWin) } },
  { LR"=(X:\.?.v?)=", { EPT(PathWin) } },
  { LR"=(X:\..?v?)=", { EPT(PathWin) } },

  { LR"=(X:\*.?v)=", { EPT(PathWin) } },
  { LR"=(X:\*..v?)=", { EPT(PathWin) } },
  { LR"=(X:\.*?v)=", { EPT(PathWin) } },
  { LR"=(X:\.*.v?)=", { EPT(PathWin) } },
  { LR"=(X:\..*v?)=", { EPT(PathWin) } },

  { LR"=(X:\*?.v.gg)=", { EPT(PathWin) } },
  { LR"=(X:\?*.v.hh)=", { EPT(PathWin) } },
  { LR"=(X:\??.v.jj)=", { EPT(PathWin) } },

  { LR"=(X:\.*?v.nn)=", { EPT(PathWin) } },
  { LR"=(X:\.?*v.oo)=", { EPT(PathWin) } },
  { LR"=(X:\.??v.pp)=", { EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\*.?.w)=", { EPT(PathWin) } },
  { LR"=(X:\*..?w)=", { EPT(PathWin) } },

  { LR"=(X:\??..w)=", { EPT(PathWin) } },
  { LR"=(X:\?.?.w)=", { EPT(PathWin) } },
  { LR"=(X:\?..?w)=", { EPT(PathWin) } },

  { LR"=(X:\?.*.w)=", { EPT(PathWin) } },
  { LR"=(X:\?..*w)=", { EPT(PathWin) } },

  { LR"=(?*.\f)=", { EPT(PathWin) } },

  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_win_dir
  { {
  { LR"=(?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.a\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*?\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(??.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(??..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(r?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r.a\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r?..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r?..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r?..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r?.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r?..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r*?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r?*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r?*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r??.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r??..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(r.?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r.??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(r..ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?s\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.s.aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?s.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*s.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?s.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?s.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*s.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?s.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*s?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s?.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?s?..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.s?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.s.ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?.t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..t?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*.t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..t*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.t*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?.t.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..t?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?.t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..t??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*.t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*.t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..t*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.yb\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?.t.cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*.yd\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*.t.ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*?t.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*?t..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?*t.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?*t..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(??t.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(??t..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.?yl\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*t?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t?p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?t?.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(.?.u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?u\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.*.u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*u\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.*u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..u*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.u*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?u*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?u?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*.?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*..u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*.u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*u?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*?.u.gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?*.u.hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(??.u.jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.*?u.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*u.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??u.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(?..v*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.v*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?v*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?.v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..?v?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*.?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*..v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.*.v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(..*v?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(*?.v.gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?*.v.hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(??.v.jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(.*?v.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.?*v.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(.??v.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(*.?.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(*..?w\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(??..w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?.?.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..?w\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(?.*.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(?..*w\)=", { EPT(Directory), EPT(PathWin) } },
  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_win_drive_dir
  { {
  { LR"=(X:\?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.a\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*?\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\r?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r.a\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r??.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r??..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r.?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?s\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.s.aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*s.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*s.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s?.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s?..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.s?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?.t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.t*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.t.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.yb\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.t.cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.yd\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.t.ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?t.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?t..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*t.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*t..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??t.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??t..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.?yl\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t?p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t?.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\.?.u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?u\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.*.u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*u\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.*u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..u*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.u*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?u*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?u?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*u?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*?.u.gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*.u.hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??.u.jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.*?u.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*u.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??u.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?..v*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.v*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?v*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?v?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*v?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*?.v.gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*.v.hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??.v.jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.*?v.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*v.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??v.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\*.?.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..?w\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\??..w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.?.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..?w\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.*.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..*w\)=", { EPT(Directory), EPT(PathWin) } },
  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_win_drive_path_dir
  { {
  { LR"=(X:\?\?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*\*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.a\.a)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..aa\..aa)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.\?.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..\?..)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?\.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.\.?.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?\..?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.\*.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..\*..)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*\.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.\.*.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*\..*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.*\?.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..*\?..*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*\.?*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.*\.?.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?*\..?*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.?\?.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..?\?..?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??\.??)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.?\.?.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..??\..??)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.?\*.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..?\*..?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?\.*?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.?\.*.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*?\..*?)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?.b\?.b)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..cc\?..cc)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.d\*.d)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..ee\*..ee)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?.f\*?.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?..gg\*?..gg)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*.f\?*.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*..hh\?*..hh)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??.i\??.i)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??..jj\??..jj)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.?l\.?l)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.ll\.?.ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?lll\..?lll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..l?ll\..l?ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*m\.*m)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.mm\.*.mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*mmm\..*mmm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..m*mm\..m*mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..mmm*\..mmm*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?n\.*?n)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?.nn\.*?.nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*?nnn\..*?nnn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..n*?nn\..n*?nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..nnn*?\..nnn*?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*o\.?*o)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*.oo\.?*.oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?*ooo\..?*ooo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..o?*oo\..o?*oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..ooo?*\..ooo?*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??p\.??p)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??.pp\.??.pp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..??ppp\..??ppp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..p??pp\..p??pp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..ppp??\..ppp??)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\r?\r?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*\r*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r.a\r.a)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..aa\r..aa)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.\r?.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..\r?..)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?\r.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.\r.?.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?\r..?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r*.\r*.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*..\r*..)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*\r.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*.\r.*.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*\r..*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.*\r?.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..*\r?..*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?*\r.?*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.*\r.?.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?*\r..?*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.?\r?.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..?\r?..?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.??\r.??)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.?\r.?.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..??\r..??)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r*.?\r*.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*..?\r*..?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*?\r.*?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*.?\r.*.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*?\r..*?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r?.b\r?.b)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?..cc\r?..cc)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*.d\r*.d)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*..ee\r*..ee)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*?.f\r*?.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r*?..gg\r*?..gg)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?*.f\r?*.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r?*..hh\r?*..hh)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r??.i\r??.i)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r??..jj\r??..jj)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\r.?l\r.?l)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?.ll\r.?.ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?lll\r..?lll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..l?ll\r..l?ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*m\r.*m)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*.mm\r.*.mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*mmm\r..*mmm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..m*mm\r..m*mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..mmm*\r..mmm*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*?n\r.*?n)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.*?.nn\r.*?.nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..*?nnn\r..*?nnn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..n*?nn\r..n*?nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..nnn*?\r..nnn*?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?*o\r.?*o)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.?*.oo\r.?*.oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..?*ooo\r..?*ooo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..o?*oo\r..o?*oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..ooo?*\r..ooo?*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.??p\r.??p)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r.??.pp\r.??.pp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..??ppp\r..??ppp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..p??pp\r..p??pp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\r..ppp??\r..ppp??)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?s\?s)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s\*s)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.s.aa\.s.aa)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.\?s.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..\?s..)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?\.s?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.\.s?.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?\.s.?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*s.\*s.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s..\*s..)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*\.s*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*.\.s*.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*\.s.*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.*\?s.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..*\?s..*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?*\.s?*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.*\.s?.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?*\.s.?*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.?\?s.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..?\?s..?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s??\.s??)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.?\.s?.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.??\.s.??)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*s.?\*s.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s..?\*s..?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*?\.s*?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*.?\.s*.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*?\.s.*?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?s.b\?s.b)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s..cc\?s..cc)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s.d\*s.d)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s..ee\*s..ee)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s?.f\*s?.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*s?..gg\*s?..gg)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s*.f\?s*.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s*..hh\?s*..hh)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s?.i\?s?.i)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?s?..jj\?s?..jj)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.s?l\.s?l)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?.ll\.s?.ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?lll\.s.?lll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.l?ll\.s.l?ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*m\.s*m)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*.mm\.s*.mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*mmm\.s.*mmm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.m*mm\.s.m*mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.mmm*\.s.mmm*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*?n\.s*?n)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s*?.nn\.s*?.nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.*?nnn\.s.*?nnn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.n*?nn\.s.n*?nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.nnn*?\.s.nnn*?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?*o\.s?*o)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s?*.oo\.s?*.oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.?*ooo\.s.?*ooo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.o?*oo\.s.o?*oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.ooo?*\.s.ooo?*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s??p\.s??p)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s??.pp\.s??.pp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.??ppp\.s.??ppp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.p??pp\.s.p??pp)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.s.ppp??\.s.ppp??)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?.t.\?.t.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t\.?t)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.\.?t.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t?\..t?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.t.\*.t.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t\.*t)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t.\.*t.)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t*\..t*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.t*\?.t*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.t.*\?.t.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t*\.?t*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.*\.?t.*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t?*\..t?*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.t?\?.t?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.t.?\?.t.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t?\.?t?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.?\.?t.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t??\..t??)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.t?\*.t?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.t.?\*.t.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t?\.*t?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t.?\.*t.?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..t*?\..t*?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.yb\?.yb)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.t.cc\?.t.cc)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.yd\*.yd)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*.t.ee\*.t.ee)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?t.f\*?t.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*?t..gg\*?t..gg)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*t.f\?*t.f)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*t..hh\?*t..hh)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??t.i\??t.i)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??t..jj\??t..jj)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.?yl\.?yl)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t.ll\.?t.ll)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t.mm\.*t.mm)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t?n\.*t?n)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*t?.nn\.*t?.nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t*o\.?t*o)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t*.oo\.?t*.oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t?p\.?t?p)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?t?.pp\.?t?.pp)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\.?.u\.?.u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?u\..?u)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.*.u\.*.u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*u\..*u)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.*u\?.*u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..u*\?..u*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*u\.?*u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.u*\.?.u*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?u*\..?u*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.?u\?.?u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..u?\?..u?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??u\.??u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.u?\.?.u?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?u?\..?u?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.?u\*.?u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..u?\*..u?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?u\.*?u)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.u?\.*.u?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*u?\..*u?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*?.u.gg\*?.u.gg)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*.u.hh\?*.u.hh)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??.u.jj\??.u.jj)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.*?u.nn\.*?u.nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*u.oo\.?*u.oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??u.pp\.??u.pp)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\?..v*\?..v*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*v\.?*v)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.v*\.?.v*)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?v*\..?v*)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.?v\?.?v)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..v?\?..v?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??v\.??v)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?.v?\.?.v?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..?v?\..?v?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*.?v\*.?v)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..v?\*..v?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*?v\.*?v)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.*.v?\.*.v?)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\..*v?\..*v?)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\*?.v.gg\*?.v.gg)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?*.v.hh\?*.v.hh)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\??.v.jj\??.v.jj)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\.*?v.nn\.*?v.nn)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.?*v.oo\.?*v.oo)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\.??v.pp\.??v.pp)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\*.?.w\*.?.w)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\*..?w\*..?w)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\??..w\??..w)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?.?.w\?.?.w)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..?w\?..?w)\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\?.*.w\?.*.w)\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\?..*w\?..*w)\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  { LR"=(X:\anydir\?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.a\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*?\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\?.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\??.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\??..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\r?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r.a\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r?..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r?..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r?..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r?.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r?..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r*?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r?*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r?*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r??.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r??..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\r.?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r.??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\r..ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\?s\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.s.aa\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?s.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*s.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s..\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?s.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s..*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?s.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s??\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*s.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s..?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?s.b\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s..cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s.d\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s..ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s?.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*s?..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s*.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s*..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s?.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?s?..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.s?l\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.?lll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.l?ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*m\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.*mmm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.m*mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.mmm*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s*?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.*?nnn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.n*?nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.nnn*?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s?*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.?*ooo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.o?*oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.ooo?*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s??p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s??.pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.??ppp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.p??pp\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.s.ppp??\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\?.t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..t?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*.t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t.\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..t*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.t*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?.t.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t.*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..t?*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?.t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..t??\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*.t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*.t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t.?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..t*?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.yb\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?.t.cc\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*.yd\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*.t.ee\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*?t.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*?t..gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?*t.f\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?*t..hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\??t.i\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\??t..jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.?yl\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t.ll\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t.mm\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t?n\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*t?.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t*o\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t*.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t?p\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?t?.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\.?.u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?u\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.*.u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*u\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.*u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..u*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.u*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?u*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?u?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*.?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*..u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*?u\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*.u?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*u?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*?.u.gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?*.u.hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\??.u.jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.*?u.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*u.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??u.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\?..v*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.v*\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?v*\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?.v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..?v?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*.?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*..v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*?v\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.*.v?\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\..*v?\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\*?.v.gg\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?*.v.hh\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\??.v.jj\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\.*?v.nn\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.?*v.oo\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\.??v.pp\)=", { EPT(Directory), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(X:\anydir\*.?.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\*..?w\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\??..w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?.?.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..?w\)=", { EPT(Directory), EPT(PathWin) } },

  { LR"=(X:\anydir\?.*.w\)=", { EPT(Directory), EPT(PathWin) } },
  { LR"=(X:\anydir\?..*w\)=", { EPT(Directory), EPT(PathWin) } },
  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_windows_error
  { {
  { LR"=(\?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.a)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..aa)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..??)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*..?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.b)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..cc)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*.d)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*..ee)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*?.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*?..gg)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?*.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?*..hh)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\??.i)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\??..jj)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.?l)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?lll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..l?ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*m)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*.mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*mmm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..m*mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..mmm*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*?n)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*?.nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*?nnn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..n*?nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..nnn*?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*o)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*.oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?*ooo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..o?*oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..ooo?*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??p)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??.pp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..??ppp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..p??pp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..ppp??)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(\r?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r.a)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..aa)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r?.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r?..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r*.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r?.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r?..*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..?*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r?.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r?..?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.??)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..??)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r*.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*..?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..*?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r?.b)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r?..cc)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*.d)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*..ee)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*?.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r*?..gg)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r?*.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r?*..hh)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r??.i)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r??..jj)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\r.?l)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?.ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..?lll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..l?ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*m)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*.mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..*mmm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..m*mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..mmm*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*?n)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.*?.nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..*?nnn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..n*?nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..nnn*?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?*o)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.?*.oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..?*ooo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..o?*oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..ooo?*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.??p)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r.??.pp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..??ppp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..p??pp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\r..ppp??)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(\?s)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.s.aa)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?s.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*s.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s..)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?s.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s..*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.?*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?s.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s..?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s??)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.??)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*s.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s..?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.*?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?s.b)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s..cc)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s.d)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s..ee)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s?.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*s?..gg)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s*.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s*..hh)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s?.i)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?s?..jj)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.s?l)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?.ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.?lll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.l?ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*m)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*.mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.*mmm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.m*mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.mmm*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*?n)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s*?.nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.*?nnn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.n*?nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.nnn*?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?*o)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s?*.oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.?*ooo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.o?*oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.ooo?*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s??p)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s??.pp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.??ppp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.p??pp)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.s.ppp??)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(\?.t.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..t?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*.t.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t.)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..t*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.t*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?.t.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t.*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..t?*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.t?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?.t.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..t??)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*.t?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*.t.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t.?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..t*?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.yb)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?.t.cc)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*.yd)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*.t.ee)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*?t.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*?t..gg)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?*t.f)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?*t..hh)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\??t.i)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\??t..jj)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.?yl)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t.ll)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t.mm)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t?n)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*t?.nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t*o)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t*.oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t?p)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?t?.pp)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(\.?.u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?u)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.*.u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*u)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.*u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..u*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.u*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?u*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.?u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..u?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.u?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?u?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*.?u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*..u?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*?u)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*.u?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*u?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*?.u.gg)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?*.u.hh)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\??.u.jj)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.*?u.nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*u.oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??u.pp)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(\?..v*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*v)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.v*)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?v*)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.?v)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..v?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??v)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?.v?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..?v?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*.?v)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*..v?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*?v)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.*.v?)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\..*v?)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\*?.v.gg)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?*.v.hh)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\??.v.jj)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\.*?v.nn)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.?*v.oo)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\.??v.pp)=", { EPT(Error), EPT(PathWin) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(\*.?.w)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\*..?w)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\??..w)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?.?.w)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..?w)=", { EPT(Error), EPT(PathWin) } },

  { LR"=(\?.*.w)=", { EPT(Error), EPT(PathWin) } },
  { LR"=(\?..*w)=", { EPT(Error), EPT(PathWin) } },
  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_nix_file
  { {
  { LR"=(/?)=", { EPT(PathNix) } },
  { LR"=(/*)=", { EPT(PathNix) } },

  { LR"=(/.a)=", { EPT(PathNix) } },
  { LR"=(/..aa)=", { EPT(PathNix) } },

  { LR"=(/?.)=", { EPT(PathNix) } },
  { LR"=(/?..)=", { EPT(PathNix) } },
  { LR"=(/.?)=", { EPT(PathNix) } },
  { LR"=(/.?.)=", { EPT(PathNix) } },
  { LR"=(/..?)=", { EPT(PathNix) } },

  { LR"=(/*.)=", { EPT(PathNix) } },
  { LR"=(/*..)=", { EPT(PathNix) } },
  { LR"=(/.*)=", { EPT(PathNix) } },
  { LR"=(/.*.)=", { EPT(PathNix) } },
  { LR"=(/..*)=", { EPT(PathNix) } },

  { LR"=(/?.*)=", { EPT(PathNix) } },
  { LR"=(/?..*)=", { EPT(PathNix) } },
  { LR"=(/.?*)=", { EPT(PathNix) } },
  { LR"=(/.?.*)=", { EPT(PathNix) } },
  { LR"=(/..?*)=", { EPT(PathNix) } },

  { LR"=(/?.?)=", { EPT(PathNix) } },
  { LR"=(/?..?)=", { EPT(PathNix) } },
  { LR"=(/.??)=", { EPT(PathNix) } },
  { LR"=(/.?.?)=", { EPT(PathNix) } },
  { LR"=(/..??)=", { EPT(PathNix) } },

  { LR"=(/*.?)=", { EPT(PathNix) } },
  { LR"=(/*..?)=", { EPT(PathNix) } },
  { LR"=(/.*?)=", { EPT(PathNix) } },
  { LR"=(/.*.?)=", { EPT(PathNix) } },
  { LR"=(/..*?)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?.b)=", { EPT(PathNix) } },
  { LR"=(/?..cc)=", { EPT(PathNix) } },
  { LR"=(/*.d)=", { EPT(PathNix) } },
  { LR"=(/*..ee)=", { EPT(PathNix) } },
  { LR"=(/*?.f)=", { EPT(PathNix) } },
  { LR"=(/*?..gg)=", { EPT(PathNix) } },
  { LR"=(/?*.f)=", { EPT(PathNix) } },
  { LR"=(/?*..hh)=", { EPT(PathNix) } },
  { LR"=(/??.i)=", { EPT(PathNix) } },
  { LR"=(/??..jj)=", { EPT(PathNix) } },

  { LR"=(/.?l)=", { EPT(PathNix) } },
  { LR"=(/.?.ll)=", { EPT(PathNix) } },
  { LR"=(/..?lll)=", { EPT(PathNix) } },
  { LR"=(/..l?ll)=", { EPT(PathNix) } },
  { LR"=(/.*m)=", { EPT(PathNix) } },
  { LR"=(/.*.mm)=", { EPT(PathNix) } },
  { LR"=(/..*mmm)=", { EPT(PathNix) } },
  { LR"=(/..m*mm)=", { EPT(PathNix) } },
  { LR"=(/..mmm*)=", { EPT(PathNix) } },
  { LR"=(/.*?n)=", { EPT(PathNix) } },
  { LR"=(/.*?.nn)=", { EPT(PathNix) } },
  { LR"=(/..*?nnn)=", { EPT(PathNix) } },
  { LR"=(/..n*?nn)=", { EPT(PathNix) } },
  { LR"=(/..nnn*?)=", { EPT(PathNix) } },
  { LR"=(/.?*o)=", { EPT(PathNix) } },
  { LR"=(/.?*.oo)=", { EPT(PathNix) } },
  { LR"=(/..?*ooo)=", { EPT(PathNix) } },
  { LR"=(/..o?*oo)=", { EPT(PathNix) } },
  { LR"=(/..ooo?*)=", { EPT(PathNix) } },
  { LR"=(/.??p)=", { EPT(PathNix) } },
  { LR"=(/.??.pp)=", { EPT(PathNix) } },
  { LR"=(/..??ppp)=", { EPT(PathNix) } },
  { LR"=(/..p??pp)=", { EPT(PathNix) } },
  { LR"=(/..ppp??)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/r?)=", { EPT(PathNix) } },
  { LR"=(/r*)=", { EPT(PathNix) } },

  { LR"=(/r.a)=", { EPT(PathNix) } },
  { LR"=(/r..aa)=", { EPT(PathNix) } },

  { LR"=(/r?.)=", { EPT(PathNix) } },
  { LR"=(/r?..)=", { EPT(PathNix) } },
  { LR"=(/r.?)=", { EPT(PathNix) } },
  { LR"=(/r.?.)=", { EPT(PathNix) } },
  { LR"=(/r..?)=", { EPT(PathNix) } },

  { LR"=(/r*.)=", { EPT(PathNix) } },
  { LR"=(/r*..)=", { EPT(PathNix) } },
  { LR"=(/r.*)=", { EPT(PathNix) } },
  { LR"=(/r.*.)=", { EPT(PathNix) } },
  { LR"=(/r..*)=", { EPT(PathNix) } },

  { LR"=(/r?.*)=", { EPT(PathNix) } },
  { LR"=(/r?..*)=", { EPT(PathNix) } },
  { LR"=(/r.?*)=", { EPT(PathNix) } },
  { LR"=(/r.?.*)=", { EPT(PathNix) } },
  { LR"=(/r..?*)=", { EPT(PathNix) } },

  { LR"=(/r?.?)=", { EPT(PathNix) } },
  { LR"=(/r?..?)=", { EPT(PathNix) } },
  { LR"=(/r.??)=", { EPT(PathNix) } },
  { LR"=(/r.?.?)=", { EPT(PathNix) } },
  { LR"=(/r..??)=", { EPT(PathNix) } },

  { LR"=(/r*.?)=", { EPT(PathNix) } },
  { LR"=(/r*..?)=", { EPT(PathNix) } },
  { LR"=(/r.*?)=", { EPT(PathNix) } },
  { LR"=(/r.*.?)=", { EPT(PathNix) } },
  { LR"=(/r..*?)=", { EPT(PathNix) } },

  { LR"=(/r?.b)=", { EPT(PathNix) } },
  { LR"=(/r?..cc)=", { EPT(PathNix) } },
  { LR"=(/r*.d)=", { EPT(PathNix) } },
  { LR"=(/r*..ee)=", { EPT(PathNix) } },
  { LR"=(/r*?.f)=", { EPT(PathNix) } },
  { LR"=(/r*?..gg)=", { EPT(PathNix) } },
  { LR"=(/r?*.f)=", { EPT(PathNix) } },
  { LR"=(/r?*..hh)=", { EPT(PathNix) } },
  { LR"=(/r??.i)=", { EPT(PathNix) } },
  { LR"=(/r??..jj)=", { EPT(PathNix) } },

  { LR"=(/r.?l)=", { EPT(PathNix) } },
  { LR"=(/r.?.ll)=", { EPT(PathNix) } },
  { LR"=(/r..?lll)=", { EPT(PathNix) } },
  { LR"=(/r..l?ll)=", { EPT(PathNix) } },
  { LR"=(/r.*m)=", { EPT(PathNix) } },
  { LR"=(/r.*.mm)=", { EPT(PathNix) } },
  { LR"=(/r..*mmm)=", { EPT(PathNix) } },
  { LR"=(/r..m*mm)=", { EPT(PathNix) } },
  { LR"=(/r..mmm*)=", { EPT(PathNix) } },
  { LR"=(/r.*?n)=", { EPT(PathNix) } },
  { LR"=(/r.*?.nn)=", { EPT(PathNix) } },
  { LR"=(/r..*?nnn)=", { EPT(PathNix) } },
  { LR"=(/r..n*?nn)=", { EPT(PathNix) } },
  { LR"=(/r..nnn*?)=", { EPT(PathNix) } },
  { LR"=(/r.?*o)=", { EPT(PathNix) } },
  { LR"=(/r.?*.oo)=", { EPT(PathNix) } },
  { LR"=(/r..?*ooo)=", { EPT(PathNix) } },
  { LR"=(/r..o?*oo)=", { EPT(PathNix) } },
  { LR"=(/r..ooo?*)=", { EPT(PathNix) } },
  { LR"=(/r.??p)=", { EPT(PathNix) } },
  { LR"=(/r.??.pp)=", { EPT(PathNix) } },
  { LR"=(/r..??ppp)=", { EPT(PathNix) } },
  { LR"=(/r..p??pp)=", { EPT(PathNix) } },
  { LR"=(/r..ppp??)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?s)=", { EPT(PathNix) } },
  { LR"=(/*s)=", { EPT(PathNix) } },

  { LR"=(/.xa)=", { EPT(PathNix) } },
  { LR"=(/.s.aa)=", { EPT(PathNix) } },

  { LR"=(/?s.)=", { EPT(PathNix) } },
  { LR"=(/?s..)=", { EPT(PathNix) } },
  { LR"=(/.s?)=", { EPT(PathNix) } },
  { LR"=(/.s?.)=", { EPT(PathNix) } },
  { LR"=(/.s.?)=", { EPT(PathNix) } },

  { LR"=(/*s.)=", { EPT(PathNix) } },
  { LR"=(/*s..)=", { EPT(PathNix) } },
  { LR"=(/.s*)=", { EPT(PathNix) } },
  { LR"=(/.s*.)=", { EPT(PathNix) } },
  { LR"=(/.s.*)=", { EPT(PathNix) } },

  { LR"=(/?s.*)=", { EPT(PathNix) } },
  { LR"=(/?s..*)=", { EPT(PathNix) } },
  { LR"=(/.s?*)=", { EPT(PathNix) } },
  { LR"=(/.s?.*)=", { EPT(PathNix) } },
  { LR"=(/.s.?*)=", { EPT(PathNix) } },

  { LR"=(/?s.?)=", { EPT(PathNix) } },
  { LR"=(/?s..?)=", { EPT(PathNix) } },
  { LR"=(/.s??)=", { EPT(PathNix) } },
  { LR"=(/.s?.?)=", { EPT(PathNix) } },
  { LR"=(/.s.??)=", { EPT(PathNix) } },

  { LR"=(/*s.?)=", { EPT(PathNix) } },
  { LR"=(/*s..?)=", { EPT(PathNix) } },
  { LR"=(/.s*?)=", { EPT(PathNix) } },
  { LR"=(/.s*.?)=", { EPT(PathNix) } },
  { LR"=(/.s.*?)=", { EPT(PathNix) } },

  { LR"=(/?s.b)=", { EPT(PathNix) } },
  { LR"=(/?s..cc)=", { EPT(PathNix) } },
  { LR"=(/*s.d)=", { EPT(PathNix) } },
  { LR"=(/*s..ee)=", { EPT(PathNix) } },
  { LR"=(/*s?.f)=", { EPT(PathNix) } },
  { LR"=(/*s?..gg)=", { EPT(PathNix) } },
  { LR"=(/?s*.f)=", { EPT(PathNix) } },
  { LR"=(/?s*..hh)=", { EPT(PathNix) } },
  { LR"=(/?s?.i)=", { EPT(PathNix) } },
  { LR"=(/?s?..jj)=", { EPT(PathNix) } },

  { LR"=(/.s?l)=", { EPT(PathNix) } },
  { LR"=(/.s?.ll)=", { EPT(PathNix) } },
  { LR"=(/.s.?lll)=", { EPT(PathNix) } },
  { LR"=(/.s.l?ll)=", { EPT(PathNix) } },
  { LR"=(/.s*m)=", { EPT(PathNix) } },
  { LR"=(/.s*.mm)=", { EPT(PathNix) } },
  { LR"=(/.s.*mmm)=", { EPT(PathNix) } },
  { LR"=(/.s.m*mm)=", { EPT(PathNix) } },
  { LR"=(/.s.mmm*)=", { EPT(PathNix) } },
  { LR"=(/.s*?n)=", { EPT(PathNix) } },
  { LR"=(/.s*?.nn)=", { EPT(PathNix) } },
  { LR"=(/.s.*?nnn)=", { EPT(PathNix) } },
  { LR"=(/.s.n*?nn)=", { EPT(PathNix) } },
  { LR"=(/.s.nnn*?)=", { EPT(PathNix) } },
  { LR"=(/.s?*o)=", { EPT(PathNix) } },
  { LR"=(/.s?*.oo)=", { EPT(PathNix) } },
  { LR"=(/.s.?*ooo)=", { EPT(PathNix) } },
  { LR"=(/.s.o?*oo)=", { EPT(PathNix) } },
  { LR"=(/.s.ooo?*)=", { EPT(PathNix) } },
  { LR"=(/.s??p)=", { EPT(PathNix) } },
  { LR"=(/.s??.pp)=", { EPT(PathNix) } },
  { LR"=(/.s.??ppp)=", { EPT(PathNix) } },
  { LR"=(/.s.p??pp)=", { EPT(PathNix) } },
  { LR"=(/.s.ppp??)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?.t.)=", { EPT(PathNix) } },
  { LR"=(/.?t)=", { EPT(PathNix) } },
  { LR"=(/.?t.)=", { EPT(PathNix) } },
  { LR"=(/..t?)=", { EPT(PathNix) } },

  { LR"=(/*.t.)=", { EPT(PathNix) } },
  { LR"=(/.*t)=", { EPT(PathNix) } },
  { LR"=(/.*t.)=", { EPT(PathNix) } },
  { LR"=(/..t*)=", { EPT(PathNix) } },

  { LR"=(/?.t*)=", { EPT(PathNix) } },
  { LR"=(/?.t.*)=", { EPT(PathNix) } },
  { LR"=(/.?t*)=", { EPT(PathNix) } },
  { LR"=(/.?t.*)=", { EPT(PathNix) } },
  { LR"=(/..t?*)=", { EPT(PathNix) } },

  { LR"=(/?.t?)=", { EPT(PathNix) } },
  { LR"=(/?.t.?)=", { EPT(PathNix) } },
  { LR"=(/.?t?)=", { EPT(PathNix) } },
  { LR"=(/.?t.?)=", { EPT(PathNix) } },
  { LR"=(/..t??)=", { EPT(PathNix) } },

  { LR"=(/*.t?)=", { EPT(PathNix) } },
  { LR"=(/*.t.?)=", { EPT(PathNix) } },
  { LR"=(/.*t?)=", { EPT(PathNix) } },
  { LR"=(/.*t.?)=", { EPT(PathNix) } },
  { LR"=(/..t*?)=", { EPT(PathNix) } },

  { LR"=(/?.yb)=", { EPT(PathNix) } },
  { LR"=(/?.t.cc)=", { EPT(PathNix) } },
  { LR"=(/*.yd)=", { EPT(PathNix) } },
  { LR"=(/*.t.ee)=", { EPT(PathNix) } },
  { LR"=(/*?t.f)=", { EPT(PathNix) } },
  { LR"=(/*?t..gg)=", { EPT(PathNix) } },
  { LR"=(/?*t.f)=", { EPT(PathNix) } },
  { LR"=(/?*t..hh)=", { EPT(PathNix) } },
  { LR"=(/??t.i)=", { EPT(PathNix) } },
  { LR"=(/??t..jj)=", { EPT(PathNix) } },

  { LR"=(/.?yl)=", { EPT(PathNix) } },
  { LR"=(/.?t.ll)=", { EPT(PathNix) } },
  { LR"=(/.*t.mm)=", { EPT(PathNix) } },
  { LR"=(/.*t?n)=", { EPT(PathNix) } },
  { LR"=(/.*t?.nn)=", { EPT(PathNix) } },
  { LR"=(/.?t*o)=", { EPT(PathNix) } },
  { LR"=(/.?t*.oo)=", { EPT(PathNix) } },
  { LR"=(/.?t?p)=", { EPT(PathNix) } },
  { LR"=(/.?t?.pp)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/.?.u)=", { EPT(PathNix) } },
  { LR"=(/..?u)=", { EPT(PathNix) } },

  { LR"=(/.*.u)=", { EPT(PathNix) } },
  { LR"=(/..*u)=", { EPT(PathNix) } },

  { LR"=(/?.*u)=", { EPT(PathNix) } },
  { LR"=(/?..u*)=", { EPT(PathNix) } },
  { LR"=(/.?*u)=", { EPT(PathNix) } },
  { LR"=(/.?.u*)=", { EPT(PathNix) } },
  { LR"=(/..?u*)=", { EPT(PathNix) } },

  { LR"=(/?.?u)=", { EPT(PathNix) } },
  { LR"=(/?..u?)=", { EPT(PathNix) } },
  { LR"=(/.??u)=", { EPT(PathNix) } },
  { LR"=(/.?.u?)=", { EPT(PathNix) } },
  { LR"=(/..?u?)=", { EPT(PathNix) } },

  { LR"=(/*.?u)=", { EPT(PathNix) } },
  { LR"=(/*..u?)=", { EPT(PathNix) } },
  { LR"=(/.*?u)=", { EPT(PathNix) } },
  { LR"=(/.*.u?)=", { EPT(PathNix) } },
  { LR"=(/..*u?)=", { EPT(PathNix) } },

  { LR"=(/*?.u.gg)=", { EPT(PathNix) } },
  { LR"=(/?*.u.hh)=", { EPT(PathNix) } },
  { LR"=(/??.u.jj)=", { EPT(PathNix) } },

  { LR"=(/.*?u.nn)=", { EPT(PathNix) } },
  { LR"=(/.?*u.oo)=", { EPT(PathNix) } },
  { LR"=(/.??u.pp)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?..v*)=", { EPT(PathNix) } },
  { LR"=(/.?*v)=", { EPT(PathNix) } },
  { LR"=(/.?.v*)=", { EPT(PathNix) } },
  { LR"=(/..?v*)=", { EPT(PathNix) } },

  { LR"=(/?.?v)=", { EPT(PathNix) } },
  { LR"=(/?..v?)=", { EPT(PathNix) } },
  { LR"=(/.??v)=", { EPT(PathNix) } },
  { LR"=(/.?.v?)=", { EPT(PathNix) } },
  { LR"=(/..?v?)=", { EPT(PathNix) } },

  { LR"=(/*.?v)=", { EPT(PathNix) } },
  { LR"=(/*..v?)=", { EPT(PathNix) } },
  { LR"=(/.*?v)=", { EPT(PathNix) } },
  { LR"=(/.*.v?)=", { EPT(PathNix) } },
  { LR"=(/..*v?)=", { EPT(PathNix) } },

  { LR"=(/*?.v.gg)=", { EPT(PathNix) } },
  { LR"=(/?*.v.hh)=", { EPT(PathNix) } },
  { LR"=(/??.v.jj)=", { EPT(PathNix) } },

  { LR"=(/.*?v.nn)=", { EPT(PathNix) } },
  { LR"=(/.?*v.oo)=", { EPT(PathNix) } },
  { LR"=(/.??v.pp)=", { EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/*.?.w)=", { EPT(PathNix) } },
  { LR"=(/*..?w)=", { EPT(PathNix) } },

  { LR"=(/??..w)=", { EPT(PathNix) } },
  { LR"=(/?.?.w)=", { EPT(PathNix) } },
  { LR"=(/?..?w)=", { EPT(PathNix) } },

  { LR"=(/?.*.w)=", { EPT(PathNix) } },
  { LR"=(/?..*w)=", { EPT(PathNix) } },
  } };

  TestPatternDiscriminatorBase::TestPatternDescriptorsV
    TestPatternDiscriminatorBase::pattern_descriptor_tests_nix_dir
  { {
  { LR"=(/?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.a/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..aa/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?../)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*../)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..??/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*..?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*?/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?.b/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..cc/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*.d/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*..ee/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*?.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*?..gg/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?*.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?*..hh/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/??.i/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/??..jj/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.?l/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?lll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..l?ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*m/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*.mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*mmm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..m*mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..mmm*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*?n/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*?.nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*?nnn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..n*?nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..nnn*?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*o/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*.oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?*ooo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..o?*oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..ooo?*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??p/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??.pp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..??ppp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..p??pp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..ppp??/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/r?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r.a/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..aa/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r?./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r?../)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r*./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*../)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r?.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r?..*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..?*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r?.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r?..?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.??/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..??/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r*.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*..?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..*?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r?.b/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r?..cc/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*.d/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*..ee/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*?.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r*?..gg/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r?*.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r?*..hh/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r??.i/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r??..jj/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/r.?l/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?.ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..?lll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..l?ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*m/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*.mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..*mmm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..m*mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..mmm*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*?n/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.*?.nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..*?nnn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..n*?nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..nnn*?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?*o/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.?*.oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..?*ooo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..o?*oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..ooo?*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.??p/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r.??.pp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..??ppp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..p??pp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/r..ppp??/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?s/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.xa/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.aa/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?s./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s../)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*s./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s../)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?s.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s..*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.?*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?s.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s..?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s??/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.??/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*s.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s..?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.*?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?s.b/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s..cc/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s.d/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s..ee/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s?.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*s?..gg/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s*.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s*..hh/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s?.i/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?s?..jj/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.s?l/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?.ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.?lll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.l?ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*m/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*.mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.*mmm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.m*mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.mmm*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*?n/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s*?.nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.*?nnn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.n*?nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.nnn*?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?*o/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s?*.oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.?*ooo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.o?*oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.ooo?*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s??p/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s??.pp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.??ppp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.p??pp/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.s.ppp??/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?.t./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..t?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*.t./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t./)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..t*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.t*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?.t.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t.*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..t?*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.t?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?.t.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..t??/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*.t?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*.t.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t.?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..t*?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.yb/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?.t.cc/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*.yd/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*.t.ee/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*?t.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*?t..gg/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?*t.f/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?*t..hh/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/??t.i/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/??t..jj/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.?yl/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t.ll/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t.mm/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t?n/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*t?.nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t*o/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t*.oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t?p/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?t?.pp/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/.?.u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?u/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.*.u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*u/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.*u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..u*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.u*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?u*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.?u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..u?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.u?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?u?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*.?u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*..u?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*?u/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*.u?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*u?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*?.u.gg/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?*.u.hh/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/??.u.jj/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.*?u.nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*u.oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??u.pp/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/?..v*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*v/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.v*/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?v*/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.?v/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..v?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??v/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?.v?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..?v?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*.?v/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*..v?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*?v/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.*.v?/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/..*v?/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/*?.v.gg/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?*.v.hh/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/??.v.jj/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/.*?v.nn/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.?*v.oo/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/.??v.pp/)=", { EPT(Directory), EPT(PathNix) } },
  }, { // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  { LR"=(/*.?.w/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/*..?w/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/??..w/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?.?.w/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..?w/)=", { EPT(Directory), EPT(PathNix) } },

  { LR"=(/?.*.w/)=", { EPT(Directory), EPT(PathNix) } },
  { LR"=(/?..*w/)=", { EPT(Directory), EPT(PathNix) } },
  } };


  TEST_CLASS(TestPatternDiscriminator),
    TestPatternDiscriminatorBase
  {
  public:

    PatternDiscriminator patternDiscriminator;

    TEST_METHOD(TestDiscriminator_Error)
    {
      for (auto const& pdt : pattern_descriptor_tests_errors)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_File)
    {
      for (auto const& pdt : pattern_descriptor_tests_file)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_WinFile)
    {
      for (auto const& pdt : pattern_descriptor_tests_win_file)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_WinDir)
    {
      for (auto const& pdt : pattern_descriptor_tests_win_dir)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_WinDriveDir)
    {
      for (auto const& pdt : pattern_descriptor_tests_win_drive_dir)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_WinDrivePathDir)
    {
      for (auto const& pdt : pattern_descriptor_tests_win_drive_path_dir)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_WinErr)
    {
      for (auto const& pdt : pattern_descriptor_tests_windows_error)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_Nix)
    {
      for (auto const& pdt : pattern_descriptor_tests_nix_file)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestDiscriminator_NixDir)
    {
      for (auto const& pdt : pattern_descriptor_tests_nix_dir)
      {
        for (auto const& test : pdt)
        {
#if defined(_DEBUG)
          if (test.stop) _CrtDbgBreak();
#endif
          PatternDescriptor result =
            patternDiscriminator.detect(test.pattern);

          Assert::AreEqual(
            make_pid(make_pattern_descriptor(test.patternDesc)),
            make_pid(result),
            (L"<" + test.pattern + L">").c_str());

          if (test.finish)
          {
            break;
          }
        }
      }
    }

    TEST_METHOD(TestPatternDescriptorOperator)
    {
      PatternDescriptor patternDescriptor{
        {ePatternType::eptError, true}
      };
      Assert::AreEqual(patternDescriptor & ePatternType::eptError, PatternId(2));
    }

    TEST_METHOD(Test_make_pid)
    {
      PatternDescriptor patternDescriptor{
        {ePatternType::eptFile, true},
        {ePatternType::eptPathWin, true},
      };
      PatternId patternId(make_pid(patternDescriptor));
      Assert::AreEqual(patternId, PatternId(20));
    }
  };

} // namespace RETest
