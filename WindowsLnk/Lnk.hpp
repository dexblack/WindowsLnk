//
// Reference - [MS-SHLLINK]: 
// Shell Link(.LNK) Binary File Format
// 9/12/2018	5.0	Major
//

// Language headers
#include <istream>
#include <inttypes.h>

// Windows headers
#include <guiddef.h>
#include <minwindef.h>

// Project headers
#include "LnkDllPort.h"
#include "LinkFlags.h"
#include "LinkFileAttributes.h"
#include "IDList.hpp"
#include "ItemIDList.hpp"


#pragma pack(push, 1)
// Shell .LNK file binary format.
//
struct LnkDllPort LnkHeader
{
  // Required header size.
  static const uint16_t cRequiredSize = 0x4C;
  static const CLSID cLnkCLSID;

  std::istream& operator>>(std::istream& input);

  uint32_t size = cRequiredSize;
  CLSID    clsid = cLnkCLSID;
  union
  {
    uint32_t link_flags{ 0UL };
    LinkFlags flags;
  };
  union
  {
    uint32_t file_attributes{ 0UL };
    LinkFileAttributes attributes;
  };
  FILETIME creation_time{ 0UL, 0UL };
  FILETIME access_time{ 0UL, 0UL };
  FILETIME write_time{ 0UL, 0UL };
  uint32_t file_size{ 0UL };
  uint32_t icon_index = 0;
  uint32_t show_command = SW_SHOWNORMAL;
  uint16_t hot_key = 0;
  uint16_t Reserved1 = 0;
  uint32_t Reserved2 = 0;
  uint32_t Reserved3 = 0;
};

std::istream& operator>>(std::istream& input, LnkHeader& lnkHeader);

#pragma pack(pop)

#pragma warning(push)
#pragma warning(disable : 4251)


// Contains all the potential instantiated elements
// resulting from parsing an arbitrary .LNK file.
// Accessor and validation functions are added here
// as necessary/appropriate.
//
class LnkDllPort Lnk
{
private:
  LnkHeader header;       // Initial fixed size header block.
  IDList idList;          // Variable length chain of link details.
  bool hasShellPath;      // Parsing result.
  std::wstring wsPath;    // Parsed file system path.
  ShItemIDs shItemIds;    // Parsed IDList elements.

public:
  Lnk();

  // Validation of header block
  // and possibly some other elements.
  bool isValid() const;

  // Binary stream reader.
  LnkDllPort friend std::istream& operator>>(std::istream& input, Lnk& lnk);

  // Wide stream output.
  // Stick with UTF16 for all the output.
  LnkDllPort friend std::wostream& operator<<(std::wostream& output, Lnk& lnk);

  // Validation related member functions.
  CLSID const& getCLSID() const;
  bool isShortCut() const;

private:
  // Internal validation related member functions.
  bool isValidHeaderSize() const;
  bool isValidFileAttribs() const;
  bool isValidShowCommand() const;
  bool isValidReserved() const;
};

#pragma warning(pop)

// Stream I/O operations.
LnkDllPort std::istream& operator>>(std::istream& input, Lnk& lnk);
LnkDllPort std::wostream& operator<<(std::wostream& output, Lnk& lnk);
