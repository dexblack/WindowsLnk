#include "pch.h"

#include <vector>
#include <regex>

#include <obeds/string/convert.hpp>
#include <obeds/std/error.hpp>

#include "PatternToRegExStr.h"
#include "FilePathRegExPatterns.h"
#include "PatternDiscriminator.h"

using ::obeds::string::convert::to;

//! \brief Parse the input gitignore style pattern and generate
//! an equivalent ECMAScript style regular expression string.
//! \param pattern Input matching expression.
//! \return ECMAScript regular expression.
//! \exception PatternError.
//!
string_type RegexDllPort pattern_to_regex_str(
  string_type const& pattern)
{
  // Identify which overall pattern type to start with.
  static PatternDiscriminator patternDiscriminator;
  PatternDescriptor patternDescriptor = patternDiscriminator.detect(pattern);

  // Tokenize the pattern according to its type.
  if (patternDescriptor & ePatternType::eptError)
  {
    throw PatternError(obeds::store_error("Invalid pattern " + to<std::string>(pattern)));
  }
  else if (patternDescriptor & ePatternType::eptFile)
  {

  }
  else if (patternDescriptor & ePatternType::eptDirectory)
  {

  }

  // Identify special token sequence /**/ and replace it with
  // the form of the DIRECTORIES_OPTIONAL regular expression.

  // Select and execute the appropriate parsers for the tokens.

  // Stitch the errors together if need be.

  // Stitch the results back together.

  return string_type();
}
