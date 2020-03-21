// Attribution notice:
// Parsing code abstracted from Google Code project.
// https://code.google.com/archive/p/lnk-parser/
// Thank you unknown author defeatthefreak@gmail.com
// The one and only commit message:
//  This is pretty rough code. I don't even want to touch it. So...uh...have fun.
//
// So, it's a start...
// here we go
// oh and my moniker?  dex@dexblack.net
//
#include "pch.h"

#include <iostream>
#include <iomanip>

#include <assert.h>
#include <ShlObj.h>

#include "IDList.hpp"
#include "ItemIDList.hpp"
#include "CLSID_Name.hpp"


// Parse ItemID objects from the raw bytes read from LNK.
//
ShItemType parsePIDL(
  IDList const& ids,   // Raw bytes input blocks.
  ShItemIDs& shItemIds // parsed Shell Items.
)
{
  ShItemType itIs = ShItemType::itIsUnknown;
  for (auto const& id : ids)
  {
    ShItemID shItemId;
    ShItemType const shItemType = parseShItemID(id, shItemId);
    shItemIds.push_back(shItemId);

    // Use the first ShItemType detected
    // as the defining type for the PIDL.
    //
    if (ShItemType::itIsUnknown == itIs)
    {
      //TODO: Revisit this later.
      // Maybe define an enumeration for LNK types
      // based upon the results of further research.
      //
      itIs = shItemType;
    }
    else
    {
      // Stop if an unknown ShItemID block is seen.
      // 
      if (ShItemType::itIsUnknown == shItemType)
      {
        break;
      }
    }
  }

  return itIs;
}


bool getPathFromIDList(
  std::istream &input, std::streampos size, std::wstring& wsPath)
{
  // Save read position, to restore it later.
  std::streampos pos = input.tellg();
  assert(pos > size);
  assert(size <= std::numeric_limits<std::streamoff>::max());
  input.seekg(pos - static_cast<std::streampos>(size));

  std::vector<char> file_buffer(size, 0);
  input.read(&file_buffer[0], size);
  wsPath.resize(size, std::wstring::value_type());
  BOOL result = SHGetPathFromIDListEx(
    reinterpret_cast<LPCITEMIDLIST>(&file_buffer[0]),
    wsPath.data(),
    static_cast<DWORD>(size),
    GPFIDL_DEFAULT
  );
  // Restore read position to where it was.
  input.seekg(pos);

  // Calculate the string's actual length and truncate.
  size = 0UL;
  for (auto it = wsPath.begin(); ; ++it)
  {
    if (*it == std::wstring::value_type())
    {
      size = it - wsPath.begin();
      break;
    }
  }
  wsPath.resize(size);
  return TRUE == result;
}

ShItemType parseShItemID(ItemID const& id, ShItemID& shItemId)
{
  auto const uint16_t_size(sizeof(uint16_t));
  ShItemType shItemType = ShItemType::itIsUnknown;
  // First step let's have a look at
  // the bytes of the first block data.
  // Decide what it might be from the first byte.
  if (id[1] == 0x50 && id[0] == 0x1F
    && id.size() == sizeof(CLSID) + uint16_t_size)
  {
    shItemId.itIs = ShItemType::itIsCLSID;

    ::memcpy_s(
      static_cast<void*>(&shItemId.clsid),
      sizeof(shItemId.clsid),
      id.data() + uint16_t_size,
      sizeof(shItemId.clsid));

    std::cout << getCLSID_Name(shItemId.clsid).name << std::endl;
  }
  else if (id[1] == 0x00 && id[0] == 0x1F)
  {
    shItemId.itIs = ShItemType::itIsCLSID;

    ::memcpy_s(
      static_cast<void*>(&shItemId.clsid),
      sizeof(shItemId.clsid),
      id.data() + uint16_t_size,
      sizeof(shItemId.clsid));

    std::cout << getCLSID_Name(shItemId.clsid).name << std::endl;
  }
  else if (id[0] == 0x2F)  // '/' character indicating a path starting.
  {
    // Paths always start with a drive specifier
    shItemId.itIs = ShItemType::itIsDrive;
    // Copy from second byte until 0x00 is found.
    auto b = id.begin();
    while (*b) ++b; // Count the first byte to allow for the null terminator.
    size_t const len = b - id.begin();
    shItemId.file.path = (char*)::malloc(len);
    ::memcpy_s(static_cast<void*>(shItemId.file.path), len, &b[1], len);
  }
  return shItemId.itIs;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ShItemID method implementations.
//

// ShItemID class ctor
ShItemID::ShItemID()
  : itIs(ShItemType::itIsUnknown)
  , clsid(GUID_NULL)
{
  file.path = nullptr;
  file.wpath = nullptr;
}

// ShItemID copy ctor.
ShItemID::ShItemID(ShItemID const& that)
  : itIs(ShItemType::itIsUnknown)
  , clsid(GUID_NULL)
{
  file.path = nullptr;
  file.wpath = nullptr;

  ShItemID::operator=(that);
}

// ShItemID assignment operator.
ShItemID& ShItemID::operator=(ShItemID const& that)
{
  if (this != &that)
  {
    itIs = that.itIs;

    switch (itIs)
    {
    case ShItemType::itIsDrive:
    case ShItemType::itIsPath:
      if (file.path)
      {
        ::free(file.path);
        file.path = nullptr;
      }
      if (that.file.path)
      {
        auto len = ::strlen(that.file.path) + 1;
        file.path = static_cast<char*>(::malloc(len));
        ::memcpy_s(file.path, len, that.file.path, len);
      }
      if (file.wpath)
      {
        ::free(file.wpath);
        file.wpath = nullptr;
      }
      if (that.file.wpath)
      {
        auto len = (::wcslen(that.file.wpath) + 1) * sizeof(wchar_t);
        file.wpath = static_cast<wchar_t*>(::malloc(len));
        ::memcpy_s(file.wpath, len, that.file.wpath, len);
      }
      break;

    case ShItemType::itIsCLSID:
    case ShItemType::itIsGUID:
      clsid = that.clsid;
      break;
    case ShItemType::itIsUnknown:
    case ShItemType::itIsBLOB:
    default:
      break;
    }
  }
  return *this;
}

// ShItemID dtor.
ShItemID::~ShItemID()
{
  switch (itIs)
  {
  case ShItemType::itIsDrive:
  case ShItemType::itIsPath:
    if (file.path)
    {
      ::free(file.path);
      file.path = nullptr;
    }
    if (file.wpath)
    {
      ::free(file.wpath);
      file.wpath = nullptr;
    }
    break;

  case ShItemType::itIsUnknown:
  case ShItemType::itIsCLSID:
  case ShItemType::itIsGUID:
  case ShItemType::itIsBLOB:
  default:
    break;
  }
}
