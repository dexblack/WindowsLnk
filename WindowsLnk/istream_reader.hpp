#pragma once

#include <istream>

// Binary stream reader helper class.
//
class istream_reader
{
public:
  std::istream& rInput;

  istream_reader(std::istream& input)
    : rInput(input)
  {}

  template <typename T> istream_reader& operator()(T& t)
  {
    rInput.read(reinterpret_cast<char*>(&t), sizeof(T));
    return *this;
  }
};

