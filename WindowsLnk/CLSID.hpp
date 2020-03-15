#pragma once
#include <istream>
#include <combaseapi.h>

std::istream& operator>>(std::istream& input, CLSID& clsid);
