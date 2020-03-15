//
// Reference - [MS-SHLLINK]: 
// Shell Link(.LNK) Binary File Format
// 9/12/2018	5.0	Major
//
#include "pch.h"
#include <stdexcept>
#include <vector>
#include "Lnk.hpp"
#include "CLSID.hpp"


// 00021401-0000-0000-C000-000000000046
const CLSID LnkHeader::cLnkCLSID
{ 0x00021401, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46} };


// Base class LnkHeader takes care of basic initialisation.
//
Lnk::Lnk()
  : header()
{}

// Fixed size required.
//
bool Lnk::isValidHeaderSize() const
{
  return LnkHeader::cRequiredSize == sizeof(header);
}

// Must a fixed CLSID.
bool Lnk::isValidCLSID() const
{
  return LnkHeader::cLnkCLSID == header.clsid;
}


bool Lnk::isValidReserved() const
{
  return
    header.Reserved1 == 0u &&
    header.Reserved2 == 0U &&
    header.Reserved3 == 0U;
}


bool Lnk::isValidShowCommand() const
{
  switch (header.show_command)
  {
  case SW_SHOWNORMAL:
  case SW_SHOWMAXIMIZED:
  case SW_SHOWMINNOACTIVE:
    return true;
  }
  return false;
}


// Validation conditions for checking object state.
//
bool Lnk::isValid() const
{
  struct validity_condition
  {
    bool valid; char const* error;
  };

  std::vector<validity_condition> validity_conditions
  {
    { isValidHeaderSize(), "Invalid HeaderSize" },
    { isValidCLSID(), "Invalid Lnk CLSID" },
    { isValidShowCommand(), "Invalid Show Command value" },
    { isValidReserved(), "Invalid Reserved bytes" },
  };

  for (validity_condition const& vc : validity_conditions)
  {
    if (!vc.valid)
    {
      throw std::runtime_error(vc.error);
    }
  }

  return true;
}

std::istream& operator>>(std::istream& input, FILETIME& ft)
{
  input >> ft.dwLowDateTime >> ft.dwHighDateTime;
  return input;
}

class istream_reader
{
  std::istream& rInput;
public:
  istream_reader(std::istream& input)
    : rInput(input)
  {}

  template <typename T> istream_reader& read(T& t)
  {
    rInput.read(reinterpret_cast<char*>(&t), sizeof(T));
    return *this;
  }
};

// Read .LNK data from stream.
// Parse and validate.
//
std::istream& operator>>(std::istream& input, Lnk& lnk)
{
  LnkHeader& rLnkHdr(lnk.header);
  istream_reader isr(input);
  isr
    .read(rLnkHdr.size)
    .read(rLnkHdr.clsid)
    .read(rLnkHdr.link_flags)
    .read(rLnkHdr.file_attrib)
    .read(rLnkHdr.creation_time)
    .read(rLnkHdr.access_time)
    .read(rLnkHdr.write_time)
    .read(rLnkHdr.file_size)
    .read(rLnkHdr.icon_index)
    .read(rLnkHdr.show_command)
    .read(rLnkHdr.hot_key)
    .read(rLnkHdr.Reserved1)
    .read(rLnkHdr.Reserved2)
    .read(rLnkHdr.Reserved3);

  return input;
}

