#pragma once

#if defined(LNK_DLL)
# define LnkDllPort   __declspec( dllexport )
#else
# define LnkDllPort __declspec( dllimport )
#endif
