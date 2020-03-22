#pragma once

#if defined(REGEX_DLL)
# define RegexDllPort   __declspec( dllexport )
#else
# define RegexDllPort __declspec( dllimport )
#endif
