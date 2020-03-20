#include "pch.h"

#include "IDList.hpp"
#include "istream_reader.hpp"

LnkDllPort std::istream& operator>>(std::istream& input, IDList& my)
{
  istream_reader ir(input);
  uint16_t n = 0U;
  ir(n);
  if (n > IDList::min_size)
  {
    for (ir(n); input.good() && n > sizeof(uint16_t); ir(n))
    {
      n -= sizeof(uint16_t);
      ItemID item(n, ItemID::value_type());
      input.read(reinterpret_cast<char*>(item.data()), n);
      my.itemIDs.push_back(item);
    }
  }
  return input;
}

