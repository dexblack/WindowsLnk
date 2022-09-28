#include "pch.h"

#include <optional>

#include "RegexIncludeExclude.h"


RegexIncludeExclude::RegexIncludeExclude()
  : include()
  , exclude()
{}


RegexIncludeExclude::RegexIncludeExclude(
  strings_type const&& include_,
  strings_type const&& exclude_)
  : include()
  , exclude()
{
  configure(include_, exclude_);
}


//! Initialise the internal input ordered regular expression collections.
//
void RegexIncludeExclude::configure(
  strings_type const& include_,
  strings_type const& exclude_
)
{
  auto xform_clude = [](strings_type const& strs, regexes_type& ie)
  {
    ie.clear();
    std::transform(strs.begin(), strs.end(),
      std::back_inserter(ie),
      [](string_type const& s)
      {
        return RegEx(s);
      });
  };
  xform_clude(include_, include);
  xform_clude(exclude_, exclude);
}


//! If the item matches nothing in either include or exclude
//! regex patterns lists, assume it should be kept.
//! @return \c std::optional<bool>() if the list was empty
//! or nothing matched path;
//! \c std::optional<bool>(true) if a match was found;
//! \c std::optional<bool>(false) if no match was found.
//
std::optional<bool> RegexIncludeExclude::matched(
  string_type const& item, regexes_type const& res) const
{
  RegEx::match_type m;
  std::optional<bool> wasMatched;
  for (auto const& win_re : res)
  {
    if (win_re(item, m))
    {
      wasMatched = true;
      break;
    }
  }
  return wasMatched;
}


//! If the item matches nothing in either include or exclude
//! regex patterns lists, assume it should be kept.
//
bool RegexIncludeExclude::keep(string_type const& item) const
{
  return
    matched(item, include)
    .value_or(!
      matched(item, exclude)
      .value_or(false));
}

// If the item matches nothing in either exclude or include
// regex patterns lists, assume it should be kept.
// NOTE: This method implements the opposite priority.
// It is absolutely not equivalent to !keep().
bool RegexIncludeExclude::drop(string_type const& item) const
{
  // If exclude list was empty or nothing matched path,
  // move on to checking the inclusions list.
  return
    matched(item, exclude)
    .value_or(!
      matched(item, include)
      .value_or(false));
}
