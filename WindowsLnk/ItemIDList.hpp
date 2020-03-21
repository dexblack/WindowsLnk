#pragma once

#include <string>
#include <vector>

#include <combaseapi.h>
#include <shtypes.h>

#include "LnkDllPort.h"
//#include "base_types.h"


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
enum class ShItemType
{
  itIsUnknown,
  itIsCLSID,   // refer to CLSID_Name
  itIsGUID,
  itIsDrive,
  itIsPath,
  itIsWPath,
  itIsBLOB
};

// Often both types are stored and required.
struct FileSystemPath
{
  char* path;
  wchar_t* wpath; // UTF16
};

struct ShItemIDs;

// Discriminated union type for Shell Items.
//
struct ShItemID
{
  ShItemID();
  ShItemID(ShItemID const& that);
  ShItemID& operator=(ShItemID const& that);
  ~ShItemID();

  ShItemType itIs; // Union type selector. Not using variant type here (yet).

  friend ShItemType parseShItemID(ItemID const& ids, ShItemID& shItemId);
  friend ShItemType parsePIDL(IDList const& ids, ShItemIDs& shItemIds);

private:
  union {
    CLSID clsid;
    GUID  guid;
    FileSystemPath file;
  };

};

#pragma warning(push)
#pragma warning(disable : 4251)

struct LnkDllPort ShItemIDs
  : public std::vector<ShItemID>
{};

#pragma warning(pop)

// Parser functions to generate a collection of ShItemIDs.
//
bool getPathFromIDList(std::istream& input, std::streampos size, std::wstring& wsPath);

ShItemType parseShItemID(ItemID const& ids, ShItemID& shItemId);
ShItemType parsePIDL(IDList const& ids, ShItemIDs& shItemIds);
