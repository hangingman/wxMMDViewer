#define _USE_MATH_DEFINES
#include <cmath>

#include "common.hpp"

float ToRadian(float x)
{
     return x*M_PI / 180.0f;
}
float ToDegree(float x)
{
     return x/M_PI * 180.0f;
}
const char* ToUTF8(const char* sjis)
{
     const std::string input(sjis);
     const std::string output = babel::sjis_to_utf8(input);
     return output.c_str();
}
std::string StringToHex(const std::string& input)
{
     static const char* const lut = "0123456789ABCDEF";
     size_t len = input.length();

     std::string output;
     output.reserve(5 * len);
     for (size_t i = 0; i < len; ++i)
     {
	  const unsigned char c = input[i];
	  output.push_back('0');
	  output.push_back('x');
	  output.push_back(lut[c >> 4]);
	  output.push_back(lut[c & 15]);
	  output.push_back(' ');
     }
     return output;
}
