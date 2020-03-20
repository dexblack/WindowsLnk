#pragma once
#include <vector>
#include <inttypes.h>

#include "base_types.h"

#include "LnkDllPort.h"


// Stream reader wrapper class.
// This is a small intermediate step implementing
// the basic SHITEMID block reading rules.
// Parsing the data in detail comes later.
//
struct IDList
{
  // sizeof(uint16_t) * 2 + sum of all item sizes
  static uint16_t const min_size{ sizeof(uint16_t) * 2 };

  ItemIDs itemIDs;

  LnkDllPort friend std::istream& operator>>(std::istream& input, IDList& idlist);
};
