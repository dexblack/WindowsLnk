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
#include "istream_reader.hpp"
#include "ItemIDList.hpp"


// 00021401-0000-0000-C000-000000000046
const CLSID LnkHeader::cLnkCLSID
{ 0x00021401, 0x0000, 0x0000, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46} };


// Base class LnkHeader takes care of basic initialisation.
//
Lnk::Lnk()
  : header()
  , idList()
  , shItemIds()
{}

// Fixed size required.
//
bool Lnk::isValidHeaderSize() const
{
  return LnkHeader::cRequiredSize == sizeof(header);
}

// Must be a fixed CLSID.
bool Lnk::isValidCLSID() const
{
  return LnkHeader::cLnkCLSID == header.clsid;
}


bool Lnk::isValidFileAttribs() const
{
  LinkFileAttributes const& flag = header.attributes;
  uint32_t value = header.file_attributes;
  const uint32_t ignored_flags =
    FILE_ATTRIBUTE_INTEGRITY_STREAM |
    FILE_ATTRIBUTE_VIRTUAL |
    FILE_ATTRIBUTE_NO_SCRUB_DATA |
    FILE_ATTRIBUTE_EA |
    FILE_ATTRIBUTE_PINNED |
    FILE_ATTRIBUTE_UNPINNED |
    FILE_ATTRIBUTE_RECALL_ON_OPEN |
    FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS;

  return
    flag.reserved1 == 0 &&
    flag.reserved2 == 0 &&
    flag.reserved3 == 0 &&
    (!flag.normal || (
      flag.normal && (value & ~ignored_flags) == FILE_ATTRIBUTE_NORMAL));
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


bool Lnk::isValidReserved() const
{
  return
    header.Reserved1 == 0u &&
    header.Reserved2 == 0U &&
    header.Reserved3 == 0U;
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
    { isValidFileAttribs(), "Invalid File Attributes" },
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

// FILETIME stream input.
std::istream& operator>>(std::istream& input, FILETIME& ft)
{
  input >> ft.dwLowDateTime >> ft.dwHighDateTime;
  return input;
}


// Read .LNK data from stream.
// Parse and validate.
//
std::istream& operator>>(std::istream& input, Lnk& lnk)
{
  LnkHeader& rLnkHdr(lnk.header);
  istream_reader ir(input);
  ir(rLnkHdr.size)
    (rLnkHdr.clsid)
    (rLnkHdr.link_flags)
    (rLnkHdr.file_attributes)
    (rLnkHdr.creation_time)
    (rLnkHdr.access_time)
    (rLnkHdr.write_time)
    (rLnkHdr.file_size)
    (rLnkHdr.icon_index)
    (rLnkHdr.show_command)
    (rLnkHdr.hot_key)
    (rLnkHdr.Reserved1)
    (rLnkHdr.Reserved2)
    (rLnkHdr.Reserved3);

  if (rLnkHdr.flags.hasLinkTargetIDList)
  {
    input >> lnk.idList;
    ShItemType itIs = parsePIDL(lnk.idList.itemIDs, lnk.shItemIds);
  }
  //if (rLnkHdr.flags.)
  //{

  //}
  return input;
}

