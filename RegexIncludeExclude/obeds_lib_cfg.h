#pragma once
//
//! @file
//! Windows Target Platform Version
//
// Copy this file, name it whatever you wish and include it first
// inside your project. Probably in your precompiled header.
//
// Uncomment and set whichever values you need for your project.
// Refer to sdkddkver.h for the appropriate NTDDI_* values

// Windows version setting.
// Generally you should only choose one of these.
// The others will be set accordingly.
//
// OBEDS_WINVER value may be changed as needed.
//
//#define OBEDS_WIN32_WINNT_DEFAULT
//#define OBEDS_NTDDI_VERSION_DEFAULT
#define OBEDS_WINVER 0x0A000000

// Assume both char* and unsigned char* are UTF-8.
// Causes both the following to be defined also.
#define OBEDS_ALWAYS_USE_MULTIBYTE_STRINGS

// UTF-8 handling for type: char*
//#define OBEDS_TREAT_SIGNED_CHAR_AS_UTF8

// UTF-8 handling for type: unsigned char*
//#define OBEDS_TREAT_UNSIGNED_CHAR_AS_UTF8

#include <obeds/configure.hpp>
