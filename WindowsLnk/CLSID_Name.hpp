#pragma once
#include <string>

#include "framework.h"

#include "LnkDllPort.h"

CLSID makeCLSID(char const* cszCLSID);

struct LnkDllPort CLSID_Name
{
  CLSID_Name(char const* const sCLSID, char const* const name1, char const* const name2 = nullptr);
  CLSID_Name(CLSID const& cCLSID, char const* const name1, char const* const name2 = nullptr);

  CLSID const clsid;
  char const* const name;

  operator CLSID const& () const { return clsid; }
};


// Convenience wrapper class for retrieving
// a CLSID's Descriptive name.
//
class LnkDllPort CLSID_Names
{
public:
  CLSID_Name const& operator[](CLSID const& clsid) const;
};

LnkDllPort extern CLSID_Name const gCLSID_Invalid;

LnkDllPort CLSID_Name const& getCLSID_Name(char const* const cszCLSID);
LnkDllPort CLSID_Name const& getCLSID_Name(CLSID const& clsid);
