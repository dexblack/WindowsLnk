#include "pch.h"

#include "FilePathRegExPatterns.h"

namespace RE
{
  str_const ESCAPE(1, PC::PATH_SEP_WIN);
  str_const BEGIN{ L"^" };
  str_const ANY_CHAR{ L"." };
  str_const ONE_OR_MORE{ L"+" };
  str_const ZERO_OR_MORE(1, PC::ASTERISK);
  str_const END{ L"$" };
  str_const PATH_SEP(2, PC::PATH_SEP_WIN);
  str_const NAME_PART{ LR"=([^"*:<>?/\\|])=" };
  str_const PATH_NAME{ NAME_PART + ONE_OR_MORE };
  str_const DIRECTORY{ PATH_SEP + PATH_NAME };
  str_const SERVER{ L"(" + PATH_SEP + DIRECTORY + L")" };
  str_const DRIVE_OR_SERVER{ L"(([A-Z]:)|" + SERVER + L")" };
  str_const DIRECTORIES_OPTIONAL{ L"(" + DIRECTORY + L")" + ZERO_OR_MORE };
  str_const PATH_NAME_OPTIONAL{ NAME_PART + ZERO_OR_MORE };
  str_const DIR_END{ PATH_SEP + ANY_CHAR + ZERO_OR_MORE };
}

