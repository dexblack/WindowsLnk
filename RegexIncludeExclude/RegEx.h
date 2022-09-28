#pragma once

#include <regex>

#include "RegexDllPort.h"
#include "RegExBaseTypes.h"


#pragma warning(push)
#pragma warning(disable : 4251)

//! Functor wrapper for \c std::regex. i.e. implements \c operator()()
//
class RegexDllPort RegEx
{
public:
  typedef std::basic_regex<string_type::value_type> regex_type;
  typedef std::pmr::match_results<string_type::const_iterator> match_type;
  typedef std::regex_constants::match_flag_type match_flag_type;
  typedef std::regex_constants::syntax_option_type syntax_option_type;

  //! The default Regular Expression parser \c ECMAScript is used.
  //
  RegEx(
    string_type const& pattern,
    syntax_option_type const syntax_options = syntax_option_type::ECMAScript
  );

  RegEx(RegEx const& that);
  RegEx& operator=(RegEx const& that);

  //! \b deleted method eliminates the possibility of referring to a temporary \c match_type value.
  //
  bool operator()(
    string_type const&&,
    match_type&,
    match_flag_type flags = std::regex_constants::match_any) = delete;

  //! The default \c std::regex_constants::match_any is used.
  //! If more than one match is possible, then any match is an acceptable result.
  //
  bool operator()(
    string_type const& str,
    match_type& match_results,
    match_flag_type flags = std::regex_constants::match_any) const;

private:
  regex_type regex;  //! Prepared regular expression.
};

#pragma warning(pop)
