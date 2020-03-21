#include "pch.h"

#include "IDList.hpp"
#include "istream_reader.hpp"

std::istream& operator>>(std::istream& input, IDList& idList)
{
  istream_reader ir(input);
  ir(idList.total_size);
  if (idList.total_size > IDList::min_size)
  {
    uint16_t n = 0U, sum = 0U;
    for (ir(n); input.good() && n > sizeof(n) && sum <= idList.total_size; ir(n))
    {
      sum += n;
      n -= sizeof(n);
      ItemID item(n, ItemID::value_type());
      input.read(reinterpret_cast<char*>(item.data()), n);
      idList.push_back(item);
    }
  }
  return input;
}

