#pragma once

#include <string>
#include <vector>


class RegexIncludeExclude
{
public:
  typedef std::wstring string_type;
  typedef std::vector<string_type> collection_type;
    
  RegexIncludeExclude();

  void configure(
    collection_type const& include_,
    collection_type const& exclude_);

  bool keep(string_type const& path) const;
  bool drop(string_type const& path) const
  {
    return !keep(path);
  }

private:
  collection_type include; // kept items regex patterns.
  collection_type exclude; // dropped items regex patterns.
};

