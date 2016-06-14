#pragma once

// C/C++
#include <stdexcept>
#include <string>

namespace tensy {

typedef std::runtime_error Exception;

#define TENSY_ASSERT(__condition, __msg) \
	if(!(__condition)) { \
  	throw std::runtime_error( \
    	std::string("assertion failed: ")+ \
      std::string(__FILE__)+" "+ \
      std::to_string(__LINE__)+" "+ \
      std::string(__msg));}
};  // tensy
