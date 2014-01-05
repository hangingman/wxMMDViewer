#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef _WIN32
   #include <windows.h>
#else
   #include <cstring>
   typedef unsigned char      BYTE;
   typedef bool               BOOL;
   typedef unsigned short int WORD;
   typedef unsigned long int  DWORD;
   #define TRUE          true
   #define FALSE         false

   /** MSW _countof */
   #include <type_traits>
   template < typename T, size_t N >
   size_t countof( T ( & arr )[ N ] )
   {
       return std::extent< T[ N ] >::value;
   }

#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include <functional> 

typedef std::vector<std::string> FIELDS;

float ToRadian(float x);
float ToDegree(float x);

#define NEW_FIELD(x) (x).clear()  

#endif /** COMMON_HPP */
