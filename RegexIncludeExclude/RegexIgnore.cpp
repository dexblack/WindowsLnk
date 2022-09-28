#include "pch.h"

#include <optional>

#include "RegexIgnore.h"
#include "FilePathRegExPatterns.h"


RegexIgnore::RegexIgnoreItemStr::RegexIgnoreItemStr(
  string_type const& pattern_,
  bool const ignore_
  )
  : ignore(ignore_)
  , pattern(pattern_)
{}


RegexIgnore::RegexIgnore(
  RegexIgnoreItemStrs const& reIgnoreItemStrs,
  ConfigureMatching const& config)
  : configuration(config)
{
  for (auto const& reIgnoreItemStr : reIgnoreItemStrs)
  {
    reIgnoreItems.push_back(RegexIgnoreItem(reIgnoreItemStr));
  }
}

// Caveat: This may throw an exception if the RegEx is invalid.
// The assumption is that pattern_to_regex_str().second
// has already been checked at least once.
//
RegexIgnore::RegexIgnoreItem::RegexIgnoreItem(
  RegexIgnoreItemStr const& reIgnoreItemStr
  )
  : ignore(reIgnoreItemStr.ignore)
  , regEx(pattern_to_regex_str(reIgnoreItemStr.pattern))
{}


void RegexIgnore::analyse(
  string_type const& that,
  MatchResults& matchResults) const
{
  RegEx::match_type remt;
  for (auto const& reIgnoreItem : reIgnoreItems)
  {
    bool const matched = reIgnoreItem.regEx(that, remt);
    if (matched)
    {
      bool const completely = remt.suffix().length() == 0;
      matchResults.push_back(
        MatchResult(matched, completely, reIgnoreItem.ignore));
    }
  }
}


bool RegexIgnore::ignore(string_type const& that) const
{
  MatchResults matchResults;
  analyse(that, matchResults);
  // Now scan through the match results and make a decision.
  for (auto const& match : matchResults)
  {
    if (match.matched && match.completely)
    {
      //!TODO Implement git style matching rule(s)
      //! based upon configuration setting(s).
      if (match.ignore && configuration.first_match_only)
      {
        return true;
      }
    }
  }
  return false;
}
