#pragma once

//! @file

#include <type_traits>

#include "RegExBaseTypes.h"
#include "RegexIncludeExclude.h"

#pragma warning(push)
#pragma warning(disable : 4251)

// shorthand for ePatternType names so the are displayed more like
// the output from the test results.
#define EPT(x) ePatternType::ept##x



enum class ePatternType : uint16_t
{
  eptNone,
  eptError,
  eptFile,
  eptDirectory,
  eptPathWin,
  eptPathNix,
};
typedef std::underlying_type<ePatternType>::type PatternId;
typedef std::pair<ePatternType, bool> PatternDiscriminatorResult;
typedef std::vector<ePatternType> PatternDescriptorInitialiser;
typedef std::vector<PatternDiscriminatorResult> PatternDescriptor;

class RegexDllPort PatternDiscriminator;


RegexDllPort PatternId make_pid(PatternDescriptor const& pd);


class RegexDllPort PatternDiscriminator
{
public:

  PatternDescriptor const detect(string_type const& pattern) const;

private:
  inline static constexpr bool exclude = true;
  inline static constexpr bool include = false;

  struct PatternType
  {
    RegexIncludeExclude reie; // The regex patterns determine the pattern type.
    ePatternType ept;  // Discriminator result.
  };
  typedef std::vector<PatternType> PatternTypes;

  static PatternTypes patternTypes;
};

RegexDllPort PatternDescriptor make_pattern_descriptor(
  PatternDescriptorInitialiser const& pdi);


RegexDllPort PatternId operator&(PatternDescriptor const& pd, ePatternType const ept);


#pragma warning(pop)
