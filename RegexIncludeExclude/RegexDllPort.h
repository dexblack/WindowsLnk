// 
// 
//! @Copyright 2020 David 'dex' Schwartz dex@dexblack.net
//
#pragma once

#if defined(REGEXINCLUDEEXCLUDE_EXPORTS)
# define RegexDllPort OBEDS_SYMBOL_EXPORT
#else
# define RegexDllPort OBEDS_SYMBOL_IMPORT
#endif
