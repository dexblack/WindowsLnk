//
// Reference - [MS-SHLLINK]: 
// Shell Link(.LNK) Binary File Format
// 9/12/2018	5.0	Major
//
#include <inttypes.h>
#if defined(LNK_DLL)
# define LnkDllPort   __declspec( dllexport )
#else
# define LnkDllPort __declspec( dllimport )
#endif


// 
struct LnkDllPort LnkHeader
{
  // Required header size.
  static const uint16_t cRequiredSize = 0x4C;
  static const CLSID cLnkCLSID;

  uint32_t size = cRequiredSize;
  CLSID    clsid = cLnkCLSID;
  uint32_t link_flags;
  uint32_t file_attrib;
  FILETIME creation_time;
  FILETIME access_time;
  FILETIME write_time;
  uint32_t file_size;
  uint32_t icon_index = 0;
  uint32_t show_command = SW_SHOWNORMAL;
  uint16_t hot_key = 0;
  uint16_t Reserved1 = 0;
  uint32_t Reserved2 = 0;
  uint32_t Reserved3 = 0;
};


// Contains all the potential instantiated elements
// resulting from parsing an arbitrary .LNK file.
// Accessor and validation functions are added here
// as necessary/appropriate.
//
class LnkDllPort Lnk
{
private:
  LnkHeader header;

public:
  Lnk();
  bool isValid() const;

private:
  bool isValidHeaderSize() const;
  bool isValidCLSID() const;
  bool isValidShowCommand() const;
  bool isValidReserved() const;
};
