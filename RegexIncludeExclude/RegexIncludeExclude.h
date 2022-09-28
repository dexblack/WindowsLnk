#pragma once

#include <vector>
#include <optional>

#include "Regex.h"

#pragma warning(push)
#pragma warning(disable : 4251)

//! Wrapper for a pair of regular expression lists.
//! Implements either an inclusion or exclusion 
//! prioritised regular expression pattern search.
//
class RegexDllPort RegexIncludeExclude
{
public:
  typedef std::vector<string_type> strings_type;
  typedef std::vector<RegEx> regexes_type;

  RegexIncludeExclude();

  RegexIncludeExclude(
    strings_type const&& include_,
    strings_type const&& exclude_);

  void configure(
    strings_type const& include_,
    strings_type const& exclude_);

  bool keep(string_type const& path) const;
  bool drop(string_type const& path) const;

private:
  std::optional<bool> matched(
    string_type const& path, regexes_type const& res) const;

private:
  regexes_type include; // kept items regex patterns.
  regexes_type exclude; // dropped items regex patterns.
};

#pragma warning(pop)
