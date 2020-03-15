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

  char buf[cRequiredSize];
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
  bool isValidHeader() const;
};
