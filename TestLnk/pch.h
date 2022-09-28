//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <obeds/configure.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX

#include <windows.h>
#include <combaseapi.h>

#include <gtest/gtest.h>
#include <fstream>
#include <limits>
