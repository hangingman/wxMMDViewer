#define _USE_MATH_DEFINES
#include <cmath>
#include <iconv.h>
#include <memory>

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
     static std::string output;
     output.clear();
     if (!sjis || *sjis == '\0') return "";

     iconv_t cd = iconv_open("UTF-8", "SHIFT-JIS");
     if (cd == (iconv_t)-1) return sjis;

     size_t inlen = strlen(sjis);
     size_t outlen = inlen * 3 + 1;
     std::unique_ptr<char[]> outbuf(new char[outlen]);
     char* inptr = const_cast<char*>(sjis);
     char* outptr = outbuf.get();
     size_t outleft = outlen - 1;

     if (iconv(cd, &inptr, &inlen, &outptr, &outleft) == (size_t)-1) {
          iconv_close(cd);
          return sjis;
     }
     *outptr = '\0';
     iconv_close(cd);
     output = outbuf.get();
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
