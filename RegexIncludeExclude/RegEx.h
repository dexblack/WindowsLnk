#pragma once

#include <string>
#include <regex>

#include "RegexDllPort.h"

#pragma warning(push)
#pragma warning(disable : 4251)


class RegexDllPort RegEx
{
  typedef wchar_t value_type;
  typedef std::basic_regex<value_type> regex_type;

  regex_type const regex;

public:
  typedef std::basic_string<value_type> string_type;
  typedef std::pmr::match_results<string_type::const_iterator> match_type;
  typedef std::regex_constants::match_flag_type match_flag_type;
  typedef std::regex_constants::syntax_option_type syntax_option_type;

  RegEx(
    string_type const& pattern,
    syntax_option_type const syntax_options = syntax_option_type::ECMAScript
  );

  bool operator()(
    string_type const&&,
    match_type&,
    match_flag_type flags = std::regex_constants::match_any) = delete;

  bool operator()(
    string_type const& str,
    match_type& match_results,
    match_flag_type flags = std::regex_constants::match_any);
};

#pragma warning(pop)
