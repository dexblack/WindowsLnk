#include "pch.h"

#include "IDList.hpp"
#include "istream_reader.hpp"


LnkDllPort std::istream& operator>>(std::istream& input, IDList& my)
{
  istream_reader isr(input);
  uint16_t n = 0U;
  isr.read(n);
  if (n > IDList::min_size)
  {
    for (isr.read(n); input.good() && n > sizeof(uint16_t); isr.read(n))
    {
      IDList::ItemID item(n, IDList::ItemID::value_type());
      input.read(reinterpret_cast<char*>(item.data()), n);
      my.itemIDs.push_back(item);
    }
  }
  return input;
}

