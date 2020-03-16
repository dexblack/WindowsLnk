#pragma once
#include <inttypes.h>
#include <vector>
#include "base_types.h"
#include "LnkDllPort.h"

#pragma pack(push, 1)


struct IDList
{
  typedef std::vector<byte> ItemID;
  typedef std::vector<ItemID> ItemIDs;

  // sizeof(uint16_t) * 2 + sum of all item sizes
  static uint16_t const min_size{ sizeof(uint16_t) * 2 };

  ItemIDs itemIDs;

  LnkDllPort friend std::istream& operator>>(std::istream& input, IDList& idlist);
};

#pragma pack(pop)
