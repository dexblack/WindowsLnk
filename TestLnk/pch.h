//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <combaseapi.h>

#include <gtest/gtest.h>
#include <fstream>
#include <limits>

#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
