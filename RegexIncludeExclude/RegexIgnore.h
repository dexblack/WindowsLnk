#pragma once
#include <string>
#include <vector>

#include "Regex.h"
#include "RegexDllPort.h"
#include "PatternToRegExStr.h"


#pragma warning(push)
#pragma warning(disable : 4251)

//! Implementation of the equivalent of a gitignore
//! list processor for regular expressions.
//
class RegexDllPort RegexIgnore
{
public:
  //! Human readable regular expression strings.
  //! This class forms part of the public interface
  //! because these are used in the RegexIgnore ctor.
  //
  struct RegexDllPort RegexIgnoreItemStr
  {
    RegexIgnoreItemStr(string_type const& pattern_, bool const ignore_ = true);

    bool const ignore; //! true => Ignore matched items.
    string_type const pattern; //! gitignore style pattern to match items against.
  };

  typedef std::vector<RegexIgnoreItemStr> RegexIgnoreItemStrs;

  //! Alters the matching algorithm to be more like gitignore.
  //! This may get more interesting later depending on demand.
  //! For now let's assume the most obvious and simple default.
  //
  struct RegexDllPort ConfigureMatching
  {
    ConfigureMatching(bool basic = true)
      : first_match_only(basic)
    {}

    bool const first_match_only = true; // ==false => git style directory matching rule.
    //bool const git_negation_active = false; // ==true => git bang prefix allowed.
  };
  //! Construct RegexIgnore from RegexIgnoreItemStrs.
  //
  RegexIgnore(
    RegexIgnoreItemStrs const& reIgnoreItemStrs,
    ConfigureMatching const& config);


  //! Prepared regular expressions for matching strings,
  //! combined with a ignore flag which defaults to false.
  //! This class is internal to RegexIgnore.
  struct RegexIgnoreItem
  {
    //! Construct RegexIgnoreItem from RegexIgnoreItemStr.
    //
    RegexIgnoreItem(RegexIgnoreItemStr const&);

    bool ignore; //! true: string is to be ignored.
    RegEx regEx; //! Regular expression to match items against.
  };

  typedef std::vector<RegexIgnoreItem> RegexIgnoreItems;


  //! analyse() output vector element.
  //
  struct MatchResult
  {
    MatchResult(bool const m, bool const c, bool const i)
      : matched(m), completely(c), ignore(i)
    {}
    bool const matched;
    bool const completely;
    bool const ignore;
  };
  typedef std::vector<MatchResult> MatchResults;


  //! Analyse the matching of patterns against \var that
  //! \param that.
  //! \param 
  //! \returns A vector of results corresponding to the reIgnoreItems,
  //! corresponding to the output for each pattern against.
  void analyse(
    string_type const& that,
    MatchResults& matchResults) const;

public:
  //! Check that string against the configured list of regular expressions.
  //! configuration::first_match_only causes first match terminates the search.
  //! \return If no matching element is found the default is false.
  //! \retval true Ignore \b that.
  //! \retval false Keep \b that.
  bool ignore(string_type const& that) const;
  //! !ignore().
  bool keep(string_type const& that) const { return !ignore(that); }

private:
  ConfigureMatching configuration; //! Alters the matching algorithm.
  RegexIgnoreItems reIgnoreItems;  //! The list of patterns to ignore/ignore.
};

#pragma warning(pop)
