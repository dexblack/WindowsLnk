//
// Reference - [MS-SHLLINK]: 
// Shell Link(.LNK) Binary File Format
// 9/12/2018	5.0	Major
//
#include "pch.h"

#include <stdexcept>
#include <iomanip>
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
  : lnkPath()
  , header()
  , idList()
  , hasShellPath(false)
  , targetPath()
  , shItemIds()
{}

// Fixed size required.
//
bool Lnk::isValidHeaderSize() const
{
  return LnkHeader::cRequiredSize == sizeof(header);
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
    { isShortCut(), "Invalid Lnk CLSID" },
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

std::istream& LnkHeader::operator>>(std::istream& input)
{
  istream_reader ir(input);
  ir(size)
    (clsid)
    (link_flags)
    (file_attributes)
    (creation_time)
    (access_time)
    (write_time)
    (file_size)
    (icon_index)
    (show_command)
    (hot_key)
    (Reserved1)
    (Reserved2)
    (Reserved3);

  return input;
}

std::istream& operator>>(std::istream& input, LnkHeader& lnkHeader)
{
  return lnkHeader.operator>>(input);
}

// Read .LNK data from stream.
// Parse and validate.
//
LnkDllPort std::istream& operator>>(std::istream& input, Lnk& lnk)
{
  input >> lnk.header;
  if (lnk.header.flags.hasLinkTargetIDList)
  {
    input >> lnk.idList;
    lnk.hasShellPath = getPathFromIDList(input, lnk.idList.total_size, lnk.targetPath);
    if (!lnk.hasShellPath)
    {
      ShItemType const itIs = parsePIDL(lnk.idList, lnk.shItemIds);
    }
  }
  //if (rLnkHdr.flags.)
  //{

  //}
  return input;
}


// A hex dump of the raw bytes in the SHITEMID data.
//
std::wostream& operator<<(std::wostream& output, ItemID const& id)
{
  output << std::setw(5) << id.size() << L": "
    << std::hex << std::uppercase << std::endl;

  for (auto const b : id)
  {
    output << int((b & 0xF0) >> 4) << int(b & 0xF) << L" ";
  }
  output << std::endl << std::dec << std::nouppercase;
  for (auto const b : id)
  {
    output << std::setw(2) << b << L" ";
  }
  output << std::endl << L"----" << std::endl;
  return output;
}


LnkDllPort std::wostream& operator<<(std::wostream& output, Lnk& lnk)
{
  if (lnk.hasShellPath)
  {
    output << lnk.targetPath << std::endl;
  }
  else
  {
    for (auto const& id : lnk.idList)
    {
      output << id;
    }
  }
  return output;
}


CLSID const& Lnk::getCLSID() const
{
  return header.clsid;
}


bool Lnk::isShortCut() const
{
  return getCLSID() == LnkHeader::cLnkCLSID;
}

void Lnk::setLnkPath(std::wstring const& wsLnkPath_)
{
  lnkPath = wsLnkPath_;
}

std::wstring const& Lnk::getLnkPath() const
{
  return lnkPath;
}
