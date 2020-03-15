//
// Reference - [MS-SHLLINK]: 
// Shell Link(.LNK) Binary File Format
// 9/12/2018	5.0	Major
//
#include "pch.h"
#include <stdexcept>
#include <vector>
#include "Lnk.hpp"


// Base class LnkHeader takes care of basic initialisation.
//
Lnk::Lnk()
  : header()
{}

// Fixed size required.
//
bool Lnk::isValidHeader() const
{
  return LnkHeader::cRequiredSize == sizeof(header);
}

// Validation conditions for checking object state.
//
bool Lnk::isValid() const
{
  struct validity_condition
  {
    bool valid; char const* error;
  };

  std::vector<validity_condition> validity
  {
    {isValidHeader(), "Invalid HeaderSize"}
  };
  for (validity_condition const& vc : validity)
  {
    if (!vc.valid)
    {
      throw std::runtime_error(vc.error);
    }
  }

  return true;
}
