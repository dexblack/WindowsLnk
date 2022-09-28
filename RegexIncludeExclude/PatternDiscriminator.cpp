#include "pch.h"
#include "PatternDiscriminator.h"





PatternDiscriminator::PatternTypes
PatternDiscriminator::patternTypes
{
  {{{L"^([\r\t\f\v ])*$", LR"=(^\s*$)="},{}}, ePatternType::eptNone},
  {{{
    LR"=(^(\.|\s)+[\\\/]?$)=", // ".", "..", "...", ".\", "..\", "...\" etc.
    LR"=(^.*[\\\/]\.+$)=", // "___\.", "___\.." etc.
    LR"=(^.*[\\\/]\.+[\\\/].*$)=", // "___\.\___", "___\..\___", "___\.\", "___\..\", "___\.", "___\.." etc.
    LR"=(^((?:(?:[^\\:*<>"]:)|\\)\\)?(?:[^\\:<>"]+\\)*(AUX|CON|COM[0-9]|LPT[0-9]|NUL|PRN)\\?$)=", // ""
    LR"=(^\\[^:\\\/<>"|]+$)=", // "\?", "\*", "\a"
    LR"=(^.:[^\\].*$)=", // "Z:*.err"
    LR"=(^.:$)=", // "Z:", "?:", "*:", "\:"
    LR"=(^.:[\\]{2,}.*$)=", // "?:\\___"
    LR"=(^(.:\\|\\\\).*\/.*$)=", // "?:\___/___", "\\___/___"
    LR"=(^[^\\\/*]*\*{2,}[^\\\/*]*$)=", // "**.err" "..err**"
    LR"=(^.*(?:[^\\\/]\*{2,}[\\\/]).*$)=", // "A:\**.err"
    LR"=(^.*(?:[^\\\/]\*{2,}[^\\\/]).*$)=", // "M:\.**\.err"
    LR"=(^.*(?:[^\\\/]\*{2,})$)=", // "F:\..err**"
    LR"=(^.*(?:[\\\/]\*{2,}[^\\\/]).*$)=", // "C:\.**.err"
    LR"=(^.:\\.*\\\\.*$)=", // "?:\___\\___" e.g. "?:\*.\\an.err"
    LR"=(^\*\*([\\\/])(\*\*+(\1?|\1([^:\\\/<>"|*?][^:\\\/<>"|.*?]*\.*\1?)+))+$)=", // "___/**/**/___", "___\**\**\___"
    LR"=(^.*?([\\\/])\*\*+(\1\*\*+)+(\1?|\1([^:\\\/<>"|*?][^:\\\/<>"|.*?]*\.*\1?)+)$)=", // "W:\err\**\**"
    },
    {
    LR"=(^(\.+)([^:\\\/<>.]+\\)$)=", // ""
    LR"=(^(\.+)([^:\\\/<>.*]+)(?!(\*\*+))([^:\\\/<>.*]+)?$)=", // ""
    }}, ePatternType::eptError
  },
  {{{LR"=(^[^:\\\/<>"|]+$)="}, {}}, ePatternType::eptFile},
  {{{LR"=(^(([a-zA-Z]:\\|\\\\))?[^:<>"|]+[\\\/]$)="}, {LR"=(^[^\\\/:<>"]+$)="}}, ePatternType::eptDirectory},
  {{{
    LR"=(^([a-zA-Z]:|\\)\\([^:\\\/<>"|.*?][^:\\\/<>"|.*?]*\.*)+$)=", // ""
    LR"=(^(?!([a-zA-Z]:|\\))([^:\/<>"|.*?][^:\\\/<>"|.*?]*\.*)+$)=", // ""
    LR"=(^(([a-zA-Z]:|\\))?([^:\/<>"|]+[^:\\\/<>"|.]*\.*)+$)=", // ""
    LR"=(^[^:\\\/<>"|.]*(\\?[^:\\\/<>"|.]+\.*)+$)=", // "?*.err"
    LR"=(^.:[^:\/<>"|]+$)=", // "?:*.err"
  }, {
    LR"=(^.:\\\\[^:\/<>"|]+$)=", // "?:\\*.err"
    LR"=(^\\\\.*\\\\.*$)=", // "\\___\\___"
    LR"=(^(?!\\\\).*\\\\.*$)=", // "___\\___"
  }}, ePatternType::eptPathWin},
  {{{
    LR"=(^((\.\.\/)|(\.\/))([^:\\<>"|.*?][^:\\\/<>"|.*?]*\.*)+$)=", // ""
    LR"=(^(?!(\.\.\/)|(\.\/))([^:\\<>"|.*?][^:\\\/<>"|.*?]*\.*)+$)=", // ""
    LR"=(^((\.\.\/)|(\.\/))?([^:\\<>"|]+[^:\\\/<>"|.]*\.*)+$)=", // ""
    LR"=(^(\.*[^:\\<>"|.]+)+$)=", // ""
    LR"=(^(.:\\|\\\\).*\/.*$)=", // "___/___"
  },{}}, ePatternType::eptPathNix},
};

//! Build a PatternDescriptor from a PatternDescriptorInitialiser.
//
RegexDllPort PatternDescriptor make_pattern_descriptor(
  PatternDescriptorInitialiser const& pdi)
{
  PatternDescriptor pd
  {
    {EPT(None), false},
    {EPT(Error), false},
    {EPT(File), false},
    {EPT(Directory), false},
    {EPT(PathWin), false},
    {EPT(PathNix), false},
  };
  for (auto const& ptype : pdi)
  {
    for (auto& pditem : pd)
    {
      if (ptype == pditem.first)
      {
        pditem.second = true;
      }
    }
  }
  return pd;
}


//! Build an integer representation (PatternId) from a PatternDescriptor.
//! Treats the pattern flags as bits in an integer.
//! Assumes the ordering of the flags is fixed. It is.
//
RegexDllPort PatternId make_pid(PatternDescriptor const& pd)
{
  PatternId pid = 0;
  for (auto const& discriminatorResult : pd)
  {
    if (discriminatorResult.second)
    {
      pid += 1 << static_cast<PatternId>(
        discriminatorResult.first);
    }
  }
  return pid;
}


//! A handy bit flag check for the PatternDescriptor structure.
//! 
//
PatternId operator&(PatternDescriptor const& pd, ePatternType const ept)
{
  return make_pid(pd) & (1 << static_cast<PatternId>(ept));
}


//! Iterate through the collection of pattern matching patterns.
//! Flagging any correct Include/Exclude matches.
//! \return A possibly incomplete PatternDescriptor.
//
PatternDescriptor const
PatternDiscriminator::detect(string_type const& pattern) const
{
  PatternDescriptor patternDescriptor;
  ePatternType ept = ePatternType::eptNone;
  for (auto const& patternType : patternTypes)
  {
    patternDescriptor.push_back(
      PatternDiscriminatorResult(
        patternType.ept,
        !patternType.reie.drop(pattern)
      ));
  }
  return patternDescriptor;
}
