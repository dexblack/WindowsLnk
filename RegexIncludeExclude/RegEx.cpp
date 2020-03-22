#include "pch.h"

#include "RegEx.h"


RegEx::RegEx(string_type const& pattern, syntax_option_type const syntax_options)
  : regex(pattern, syntax_options)
{}



bool RegEx::operator()(
  string_type const& str,     // String to be searched.
  match_type& match_results,  // Output matches.
  match_flag_type flags       // Search mode.
  )
{
  std::regex_search(str, match_results, regex, flags);
  return match_results.ready() && !match_results.empty();
}

