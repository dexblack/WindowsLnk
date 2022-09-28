#include "pch.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>

#include <CppUnitTest.h>

#include <obeds/std/error.hpp>


namespace RETest
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;



  TEST_CLASS(obeds_std_error)
  {
    TEST_METHOD(Test_get_next_error_buffer)
    {
      obeds::error_buf& error = obeds::get_next_error_buffer();

      std::string const str = "test string longer";
      std::copy(str.begin(), str.end(), error);
      error[str.length()] = char();
      std::string const str2 = "shorter";
      std::copy(str2.begin(), str2.end(), error);
      error[str2.length()] = char();

      Assert::AreEqual(std::strlen(error), str2.length());
    };


    TEST_METHOD(TestWriteAnError)
    {
      static std::mt19937 mt19937{ static_cast<unsigned int>(time(0)) };
      static std::uniform_int_distribution<int> uid{ 50, 500 };

      struct write_an_error
      {
        int id;

        write_an_error(int id_)
          : id(id_)
        {
          auto N = uid(mt19937);
          for (int x = 0; x < (N%(id+1)); ++x)
            auto const discardThis = uid(mt19937);
        }

        void operator()()
        {
          auto delay = uid(mt19937);
          std::this_thread::sleep_for(std::chrono::milliseconds(delay));

          auto now = std::chrono::steady_clock::now();
          std::ostringstream ss;
          ss << "id:" << std::setw(5) << id
            << " delay:" << std::setw(4) << delay
            << " t:" << std::setw(16) << now.time_since_epoch().count();

          obeds::error_buf& error = obeds::get_next_error_buffer();
          Assert::IsTrue(ss.str().size() < sizeof(error));

          ss.seekp(0, std::ios::end);
          auto const size = static_cast<unsigned long long>(ss.tellp());

          // NB: std::stringstream::str() creates a new string every time.
          std::string const message = ss.str().substr(0, std::min(sizeof(error) - 1, size));
          std::copy(message.begin(), message.end(), error);
          error[std::min(sizeof(error)-1, message.length())] = char(); // Null terminate
        }
      };

      std::vector<std::thread> threads;
      for (int id = 0; id < 1024; ++id)
      {
        threads.push_back(std::thread(write_an_error(id)));
      }
      for (auto& thread : threads)
      {
        thread.join();
      }

      std::vector<std::string> here;
      obeds::dump_all_errors(here);
      for (auto& e : here)
      {
        Logger::WriteMessage(e.c_str());
        Logger::WriteMessage("\n");
      }
    }

    TEST_METHOD(TestThrowAnError)
    {
    }

    TEST_METHOD(Test_overflow_uchar)
    {
      unsigned char expected = 0;
      unsigned char x = std::numeric_limits<unsigned char>::max();
      Assert::AreEqual(expected, ++x);
    }


    TEST_METHOD(TestArrayIteration)
    {
      char bufs[3][7]
      {
        "one", "two", "three"
      };
      int i = 0;
      std::ostringstream oss;
      for (auto const& buf : bufs)
      {
        oss << ++i << ": " << buf;
        Logger::WriteMessage(oss.str().c_str());
        oss.str("");
        Logger::WriteMessage("\n");
      }
    }


  };

}