#pragma once

#include <stdexcept>

#include "RegexDllPort.h"
#include "RegExBaseTypes.h"

//! @file

#pragma warning(push)
#pragma warning(disable : 4251)

class PatternError : public std::runtime_error
{
public:
  PatternError(std::string const& msg)
    : runtime_error(msg)
  {}

  PatternError(const char* msg)
    : runtime_error(msg)
  {}
};

string_type RegexDllPort pattern_to_regex_str(string_type const& pattern);

#pragma warning(pop)


enum class Delimiter
{
  ANY_DIR,
  DRIVE,
  WIN_PATH,
  NIX_PATH,
  NAME,
};

struct DelimiterById
{
  Delimiter id;
  string_type::value_type const* pattern;
};
// delimiters[] order must match Delimiter enum.
inline constexpr DelimiterById const delimiters[]
{
  {Delimiter::ANY_DIR, LR"=((([\\\/])\*\*(?:\2))|(^\*\*[\\\/])|([\\\/]\*\*$))="},
  {Delimiter::DRIVE, L":"},
  {Delimiter::WIN_PATH, LR"=(\\)="},
  {Delimiter::NIX_PATH, LR"=(\/)="},
  {Delimiter::NAME, L"\\."},
};


template <Delimiter delim>
class TokenizerBase
{
  typedef std::basic_regex<string_type::value_type> regex_type;
  typedef std::regex_token_iterator<string_type::const_iterator> regex_token_iterator_type;

  regex_type delimiter_re;

public:
  TokenizerBase()
    : delimiter_re(delimiters[static_cast<int>(delim)].pattern)
  {
#ifdef _DEBUG
    Delimiter id = Delimiter::ANY_DIR;
    for (auto const& d : delimiters)
    {
      id = static_cast<Delimiter>(static_cast<int>(id) + 1);
      _ASSERTE(d.id == id); // delimiters[] order does not match Delimiter enum.
    }
#endif
  }

  void operator()(string_type pattern, std::vector<string_type>& tokens)
  {
    std::copy(regex_token_iterator_type(pattern.begin(), pattern.end(), delimiter_re, -1),
      regex_token_iterator_type(), std::back_inserter(tokens));
  }
};

typedef TokenizerBase<Delimiter::ANY_DIR> AnyDirTokenizer;
typedef TokenizerBase<Delimiter::WIN_PATH> WinPathTokenizer;
typedef TokenizerBase<Delimiter::NIX_PATH> NixPathTokenizer;
typedef TokenizerBase<Delimiter::DRIVE> DriveTokenizer;
typedef TokenizerBase<Delimiter::NAME> NameTokenizer;
