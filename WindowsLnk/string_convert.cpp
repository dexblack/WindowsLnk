#include "pch.h"

#include <stdexcept>
#include <wchar.h>

#include "string_convert.h"


LnkDllPort std::wstring string_to_wstring(std::string const& src)
{
  size_t retval = 0;
  rsize_t len = src.size() + 1;
  rsize_t dstsz = len;
  wchar_t* dst = static_cast<wchar_t*>(
    calloc(dstsz, sizeof(std::wstring::value_type)));
  auto psrc = src.data();
  std::mbstate_t ps{ 0,0,0 };
  auto err = mbsrtowcs_s(&retval, dst, dstsz, &psrc, len, &ps);
  for (; ERANGE == err; err = mbsrtowcs_s(&retval, dst, dstsz, &psrc, len, &ps))
  {
    dstsz <<= 1;
    wchar_t* dst2 = static_cast<wchar_t*>(
      realloc(dst, dstsz * sizeof(std::string::value_type)));
    if (!dst2)
    {
      throw std::runtime_error("string_to_wstring(): Memory allocation error.");
    }
    dst = dst2;
  }
  if (NOERROR != err)
  {
    switch (err)
    {
    case EINVAL:
      throw std::runtime_error("string_to_wstring(): null pointer parameter.");
      break;
    case EILSEQ:
      throw std::runtime_error("string_to_wstring(): invalid multibyte sequence.");
      break;
    default:
      break;
    }
  }
  return std::wstring(dst ? dst : L"");
}


LnkDllPort std::string wstring_to_string(std::wstring const& src)
{
  size_t retval = 0;
  rsize_t len = src.size() + 1;
  rsize_t dstsz = len;
  char* dst = static_cast<char*>(
    calloc(dstsz, sizeof(std::string::value_type)));;
  auto psrc = src.data();
  std::mbstate_t ps{ 0,0,0 };
  errno_t err = wcsrtombs_s(&retval, dst, dstsz, &psrc, len, &ps);
  for (;ERANGE == err; err = wcsrtombs_s(&retval, dst, dstsz, &psrc, len, &ps))
  {
    dstsz <<= 1;
    char* dst2 = static_cast<char*>(
      realloc(dst, dstsz * sizeof(std::string::value_type)));
    if (!dst2)
    {
      throw std::runtime_error("wstring_to_string(): Memory allocation error.");
    }
    dst = dst2;
  }
  if (NOERROR != err)
  {
    switch (err)
    {
    case EINVAL:
      throw std::runtime_error("string_to_wstring(): null pointer parameter.");
      break;
    default:
      break;
    }
  }
  return std::string(dst ? dst : "");
}
