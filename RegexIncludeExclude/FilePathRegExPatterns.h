#pragma once

#include "RegExBaseTypes.h"


namespace RE
{
  typedef string_type const str_const;

  extern str_const ESCAPE;
  extern str_const BEGIN;
  extern str_const ANY_CHAR;
  extern str_const ONE_OR_MORE;
  extern str_const ZERO_OR_MORE;
  extern str_const END;
  extern str_const PATH_SEP;
  extern str_const NAME_PART;
  extern str_const PATH_NAME;
  extern str_const SERVER;
  extern str_const DIRECTORY;
  extern str_const DRIVE_OR_SERVER;
  extern str_const DIRECTORIES_OPTIONAL;
  extern str_const PATH_NAME_OPTIONAL;
  extern str_const DIR_END;
}

namespace PC
{
  typedef string_type const str_const;
  inline string_type::value_type const DRIVE_SEP{ L':' };
  inline string_type::value_type const PATH_SEP_WIN{ L'\\' };
  inline string_type::value_type const PATH_SEP_NIX{ L'/' };
  inline string_type::value_type const ASTERISK{ L'*' };
  inline string_type::value_type const QUESTION{ L'?' };
  inline string_type::value_type const DOT{ L'.' };
  inline str_const WHITE_SPACE{ L"\x20\x0C\x0A\x0D\x09\x0B" };
}
