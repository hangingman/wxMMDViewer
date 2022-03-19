#ifndef MMD_IO_HPP
#define MMD_IO_HPP

#include "common.hpp"

/**
 * size_t is unsigned int or long unsigned int
 */
FIELDS& operator<<( FIELDS&fields,const std::string& name );
FIELDS& operator<<( FIELDS&fields,const std::pair<char*,size_t>& pchars );
FIELDS& operator<<( FIELDS&fields,const std::pair<char*,long unsigned int>& pchars );
FIELDS& operator<<( FIELDS&fields,const std::pair<BYTE*,size_t>& pbytes );

template<typename T> FIELDS& operator<<( FIELDS&fields,const T t)
{
     std::stringstream s;
     s.imbue( std::locale("C") );
     s << t << '\0';
     fields.push_back( s.str() );	
     return fields;
}

/**
 * size_t is unsigned int or long unsigned int
 */
FIELDS& operator>>( FIELDS&fields, std::string& name );
FIELDS& operator>>( FIELDS&fields, std::pair<char*,size_t>& pchars );
FIELDS& operator>>( FIELDS&fields, std::pair<char*,long unsigned int>& pchars );
FIELDS& operator>>( FIELDS&fields, std::pair<BYTE*,size_t>& pbytes );

template<typename T> FIELDS& operator>>( FIELDS&fields,T& t)
{
     if ( fields.size() ){
	  std::stringstream s;
	  s.imbue( std::locale("C") );
	  s << fields.front();
	  s >> t;
	  fields.erase( fields.begin() );
     }
     return fields;
}

#endif /** MMD_IO_HPP */
