#pragma once

#include <string>
#include <vector>

#include "Regex.h"
#include "RegexDllPort.h"

#pragma warning(push)
#pragma warning(disable : 4251)


class RegexDllPort RegexIncludeExclude
{
public:
  typedef std::wstring string_type;
  typedef std::vector<string_type> strings_type;
  typedef std::vector<RegEx> regexes;

  RegexIncludeExclude();

  void configure(
    strings_type const& include_,
    strings_type const& exclude_);

  bool keep(string_type const& path) const;
  bool drop(string_type const& path) const;

private:
  std::optional<bool> matched(
    string_type const& path, regexes const& res) const;

private:
  regexes include; // kept items regex patterns.
  regexes exclude; // dropped items regex patterns.
};

#pragma warning(pop)
