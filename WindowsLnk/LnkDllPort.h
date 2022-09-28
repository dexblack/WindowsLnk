#pragma once

#include <obeds/configure.hpp>

#if defined(LNK_DLL)
# define LnkDllPort OBEDS_SYMBOL_EXPORT
#else
# define LnkDllPort OBEDS_SYMBOL_IMPORT
#endif
