#include "pch.h"
#include <fstream>
#include "Lnk.hpp"
#include "SolutionDir.h"

namespace TestLnk
{
  TEST(LnkHeader, sizeCheck)
  {
    union
    {
      uint32_t file_attributes{ 0UL };
      LinkFileAttributes attributes;
    } x;
    EXPECT_EQ(sizeof(x.attributes), sizeof(x.file_attributes));
  }
  TEST(LnkHeader, correctSize)
  {
    EXPECT_EQ(sizeof(LnkHeader), LnkHeader::cRequiredSize)
      << "LnkHeader size changed?!";
  }

  TEST(Lnk, isValid)
  {
    Lnk lnk;
    EXPECT_TRUE(lnk.isValid());
  }

  TEST(Lnk, parse)
  {
    std::string const testDir{ TestData::SolutionDir + "TestData\\" };
    std::vector<std::string> const testFiles
    {
      "20151027_095211.jpg",
      "IEEE Web Renewal 2007-8.htm",
      "Loading.ico"
    };

    for (std::string const& testFile : testFiles)
    {
      std::ifstream ifsLnk(testDir + testFile + ".lnk", std::ifstream::binary);
      ASSERT_TRUE(ifsLnk.good()) << "Failed to open test file: " << testFile;

      Lnk lnk;
      ifsLnk >> lnk;

      EXPECT_TRUE(lnk.isValid());
    }
  }
}
