#include "pch.h"
#include "Lnk.hpp"


namespace TestLnk
{
  TEST(Lnk, isValid) {
    Lnk lnk;
    EXPECT_TRUE(lnk.isValid());
  }

}
