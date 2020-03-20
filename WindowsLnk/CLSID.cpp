#include "pch.h"
#include "clsid.hpp"

std::istream& operator>>(std::istream& input, CLSID& clsid)
{
  input >> clsid.Data1 >> clsid.Data2 >> clsid.Data3;
  for (unsigned char& byte : clsid.Data4)
  {
    input >> byte;
  }
  return input;
}
