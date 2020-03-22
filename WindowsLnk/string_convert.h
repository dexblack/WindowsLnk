#pragma once
#include <string>

#include "LnkDllPort.h"

LnkDllPort std::wstring string_to_wstring(std::string const& s);
LnkDllPort std::string wstring_to_string(std::wstring const& ws);