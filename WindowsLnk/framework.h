#pragma once

#include <obeds/configure.hpp>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <Knownfolders.h>
#include <guiddef.h>
#include <WTypesbase.h>
#include <combaseapi.h>

#if defined(_WIN32) && !defined(OLE2ANSI)
# define TESTLNK_WIDE_OLECHAR
#endif