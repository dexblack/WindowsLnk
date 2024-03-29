#include "pch.h"

#include <fstream>
#include <limits>

#include <assert.h>

#include <ShlObj.h>
#include <obeds/string/convert.hpp>

#include "Lnk.hpp"
#include "SolutionDir.h"


namespace TestLnk
{
  static std::vector<char const*> const testFiles
  {
    "b0rken",
    "1.80",
    "20151027_095211.jpg",
    "IEEE membership renewal",
    "IEEE Web Renewal 2007-8.htm",
    "Loading.ico",
    "ms-gamingoverlay--kglcheck- (3)",
    "System (8)",
    "System and Security",
    "WindowsLnk.code-workspace",
    "_DSC0294.JPG",
  };

  
  TEST(LnkHeader, SizeCheck)
  {
    union
    {
      uint32_t file_attributes{ 0UL };
      LinkFileAttributes attributes;
    } x;
    EXPECT_EQ(sizeof(x.attributes), sizeof(x.file_attributes));
    EXPECT_EQ(sizeof(x.attributes), sizeof(x));
  }
  TEST(LnkHeader, CorrectSize)
  {
    EXPECT_EQ(sizeof(LnkHeader), LnkHeader::cRequiredSize)
      << "LnkHeader size changed?!";
  }

  TEST(Lnk, IsValidDefaultCtor)
  {
    Lnk lnk;
    EXPECT_TRUE(lnk.isValid());
  }

  TEST(Lnk, ReadLnk)
  {
    std::string const testDir{ TestData::SolutionDir + "TestData\\" };

    for (auto const& testFile : testFiles)
    {
      Lnk lnk;

      lnk.setLnkPath(obeds::string::convert::to<std::wstring>(testDir + testFile + ".lnk"));
      std::ifstream ifsLnk(lnk.getLnkPath(), std::ifstream::binary);
      ASSERT_TRUE(ifsLnk.good()) << "Failed to open test file: " << testFile;

      ifsLnk >> lnk;

      EXPECT_TRUE(lnk.isValid());

#ifdef _DEBUG
      std::wcout << lnk;
#endif
    }
  }

}
