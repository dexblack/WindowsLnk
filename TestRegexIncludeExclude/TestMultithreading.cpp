#include "pch.h"
#include <atomic>

#include <CppUnitTest.h>

namespace TestMultithreading
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;


  TEST_CLASS(TestAtomic)
  {

    TEST_METHOD(Test_std_atomic_flag)
    {
      std::atomic_flag af{};
      Assert::IsFalse(af._Storage.load());

      bool const result0 = af.test_and_set();
      Assert::IsFalse(result0);

      bool const result1 = af.test_and_set();
      Assert::IsTrue(result1);
    }

    TEST_METHOD(Test_std_atomic_bool_strong)
    {
      std::atomic<bool> ab{};

      Assert::IsFalse(ab.load());

      bool expected = true;
      bool const result0 = ab.compare_exchange_strong(expected, true);
      Assert::IsFalse(result0);
      Assert::IsFalse(expected);
      Assert::IsFalse(ab.load());

      bool const result1 = ab.compare_exchange_strong(expected, true);
      Assert::IsTrue(result1);
      Assert::IsFalse(expected);
      Assert::IsTrue(ab.load());

      bool const result2 = ab.compare_exchange_strong(expected, false);
      Assert::IsFalse(result2);
      Assert::IsTrue(expected);
      Assert::IsTrue(ab.load());

      bool const result3 = ab.compare_exchange_strong(expected, false);
      Assert::IsTrue(result3);
      Assert::IsTrue(expected);
      Assert::IsFalse(ab.load());
    }

    TEST_METHOD(Test_std_atomic_bool_weak)
    {
      std::atomic<bool> ab{};

      Assert::IsFalse(ab.load());

      bool expected = true;
      bool const result0 = ab.compare_exchange_weak(expected, true);
      Assert::IsFalse(result0);
      Assert::IsFalse(expected);
      Assert::IsFalse(ab.load());

      bool const result1 = ab.compare_exchange_weak(expected, true);
      Assert::IsTrue(result1);
      Assert::IsFalse(expected);
      Assert::IsTrue(ab.load());

      bool const result2 = ab.compare_exchange_weak(expected, false);
      Assert::IsFalse(result2);
      Assert::IsTrue(expected);
      Assert::IsTrue(ab.load());

      bool const result3 = ab.compare_exchange_weak(expected, false);
      Assert::IsTrue(result3);
      Assert::IsTrue(expected);
      Assert::IsFalse(ab.load());
    }
  };

}