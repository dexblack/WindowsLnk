#pragma once
#include <istream>
#include <guiddef.h>

std::istream& operator>>(std::istream& input, CLSID& clsid);
