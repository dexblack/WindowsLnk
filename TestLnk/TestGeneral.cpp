#include "pch.h"

namespace TestGeneral
{
  TEST(Platform, FILETIME)
  {
    EXPECT_EQ(sizeof(FILETIME), 8) << "FILETIME structure size breaks LNK header format";
  }

}
