#pragma once


#include <string>
#include <algorithm>

#include <CppUnitTest.h>

#include <PatternDiscriminator.h>

#include "TestData.h" //!TODO break this up into separate data sets.


namespace RETest
{
  using namespace Microsoft::VisualStudio::CppUnitTestFramework;

  struct TestPatternDiscriminatorBase
  {
    ~TestPatternDiscriminatorBase() {}

    struct TestPatternDescriptor
    {
      ~TestPatternDescriptor() {}

      string_type const pattern;
      PatternDescriptorInitialiser const patternDesc;
      bool const stop = false;    // =true and set a breakpoint to stop on it.
      bool const finish = false;  // =true skip further tests.
    };
    typedef std::vector<TestPatternDescriptor> TestPatternDescriptors;
    typedef std::vector<TestPatternDescriptors> TestPatternDescriptorsV;

    static TestPatternDescriptorsV pattern_descriptor_tests_errors;
    static TestPatternDescriptorsV pattern_descriptor_tests_file;
    static TestPatternDescriptorsV pattern_descriptor_tests_win_file;
    static TestPatternDescriptorsV pattern_descriptor_tests_win_drive_dir;
    static TestPatternDescriptorsV pattern_descriptor_tests_win_drive_path_dir;
    static TestPatternDescriptorsV pattern_descriptor_tests_win_dir;
    static TestPatternDescriptorsV pattern_descriptor_tests_windows_error;
    static TestPatternDescriptorsV pattern_descriptor_tests_nix_file;
    static TestPatternDescriptorsV pattern_descriptor_tests_nix_dir;
  };

}
