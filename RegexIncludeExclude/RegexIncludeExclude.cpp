#include "pch.h"
#include "RegexIncludeExclude.h"


RegexIncludeExclude::RegexIncludeExclude()
  : include()
  , exclude()
{}


void RegexIncludeExclude::configure(
  collection_type const& include_,
  collection_type const& exclude_
)
{
  include = include_;
  exclude = exclude_;
}


bool RegexIncludeExclude::keep(string_type const& path) const
{
  return false;
}
