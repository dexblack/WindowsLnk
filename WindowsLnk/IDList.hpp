#pragma once
#include <vector>
#include <inttypes.h>

#include "base_types.h"

#include "LnkDllPort.h"


#pragma warning(push)
#pragma warning(disable : 4251)

// Stream reader wrapper class.
// This is a small intermediate step implementing
// the basic SHITEMID block reading rules.
// Parsing the data in detail comes later.
//
struct LnkDllPort IDList
  : public std::vector<ItemID>
{
  static uint16_t const min_size = sizeof(uint16_t) * 2;

  IDList()
    : total_size()
  {}

  uint16_t total_size;

  friend std::istream& operator>>(std::istream& input, IDList& idlist);
};

#pragma warning(pop)
