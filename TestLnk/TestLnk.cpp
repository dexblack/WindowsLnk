#include "pch.h"
#include <fstream>
#include "Lnk.hpp"
#include "SolutionDir.h"
#include <share.h>

namespace TestLnk
{
  TEST(Lnk, isValid)
  {
    Lnk lnk;
    EXPECT_TRUE(lnk.isValid());
  }

  TEST(Lnk, parse)
  {
    std::string const testDir{ TestData::SolutionDir + "TestData\\" };
    std::string const testFile{ testDir + "20151027_095211.jpg.lnk" };

    std::ifstream ifsLnk(testFile, std::ifstream::binary);
    ASSERT_TRUE(ifsLnk.good()) << "Failed to open test file: " << testFile;

    Lnk lnk;
    ifsLnk >> lnk;

    EXPECT_TRUE(lnk.isValid());
  }
}
