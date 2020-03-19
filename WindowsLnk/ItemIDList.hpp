#pragma once

#include <string>
#include <vector>
#include <combaseapi.h>

#include "base_types.h"


// A PIDL can be many things:
// PIDL = Pointer to an Item iDentifier List
//
// 1. CLSID or GUID.
// 2. Binary data i.e. for custom shell extensions.
// 3. URL link info.
// 4. Disk drives.
// 5. File system path information.
// 6. etc.
//
//
class ItemIDList
{
  struct FileSystemPath
  {
    std::string path;
    std::wstring wpath; // UTF16
  };

  struct ShItemID
  {
    ~ShItemID()
    {}

    enum ItemType
    {
      itIsUnknown,
      itIsCLSID,   // refer to 
      itIsGUID,
      itIsDrive,
      itIsPath,
      itIsWPath
    } itIs;

    union {
      ItemID rawItemId;
      CLSID clsid;
      GUID  guid;
      FileSystemPath path;
    };
  };

public:

  ItemIDList(ItemIDs const& ids);

  std::vector<ItemID> items;
};
