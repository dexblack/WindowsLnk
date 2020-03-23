#include "pch.h"

#include <optional>

#include "RegexIncludeExclude.h"


RegexIncludeExclude::RegexIncludeExclude()
  : include()
  , exclude()
{}


void RegexIncludeExclude::configure(
  strings_type const& include_,
  strings_type const& exclude_
)
{
  auto xform_clude = [](strings_type const& strs, regexes& ie)
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


// If path matches nothing in either include or exclude
// regex patterns lists, assume it should be kept.
std::optional<bool> RegexIncludeExclude::matched(
  string_type const& path, regexes const& res) const
{
  RegEx::match_type m;
  std::optional<bool> wasMatched;
  for (auto const& re : res)
  {
    wasMatched = re(path, m);
    if (*wasMatched)
      break;
  }
  return wasMatched;
}


// If path matches nothing in either include or exclude
// regex patterns lists, assume it should be kept.
bool RegexIncludeExclude::keep(string_type const& path) const
{
  // If include list was empty or nothing matched path,
  // move on to checking the exclusions list.
  return
    matched(path, include)
    .value_or(!
      matched(path, exclude)
      .value_or(false));
}

// If path matches nothing in either exclude or include
// regex patterns lists, assume it should be kept.
// NOTE: This method implements the opposite priority.
// It is absolutely not equivalent to !keep().
bool RegexIncludeExclude::drop(string_type const& path) const
{
  // If exclude list was empty or nothing matched path,
  // move on to checking the inclusions list.
  return
    matched(path, exclude)
    .value_or(!
      matched(path, include)
      .value_or(false));
}
