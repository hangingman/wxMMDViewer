#include "common.hpp"
#include "MMD_IO.hpp"

FIELDS& operator<<( FIELDS&fields,const std::string& name )
{
     std::string text = name;
     std::string::size_type pos;
     fields.clear();
     while ( text.length()>0 )
     {
	  pos = text.find_first_of(',');
	  if ( pos == std::string::npos )
	  {
	       fields.push_back(text);
	       break;
	  }
	  fields.push_back( text.substr( 0,pos ) );
	  text = text.substr( pos+1 );
     }
     return fields;
}
FIELDS& operator<<( FIELDS&fields,const std::pair<char*,size_t>& pchars )
{
     std::string s(pchars.first);
     fields.push_back( s );
     return fields;
}
FIELDS& operator<<( FIELDS&fields,const std::pair<BYTE*,size_t>& pbytes )
{
     std::string buff(pbytes.second*2+2,' ');
     buff[0]='0';
     buff[1]='x';
     for (size_t i=0;i<pbytes.second;i++)
     {
	  buff[i*2+2]="0123456789ABCDEF"[pbytes.first[i]>>4] ;
	  buff[i*2+3]="0123456789ABCDEF"[pbytes.first[i]&0xF];
     }

     fields.push_back( buff );
     return fields;
}

FIELDS& operator>>( FIELDS&fields,std::string& name )
{
     size_t i;
     name = "";
     if ( fields.size() )
     {
	  for ( i=0;i<fields.size()-1;i++)
	  {
	       name += fields[i] + (std::string)",";
	  }
	  name += fields[i];
     }
     return fields;
}

FIELDS& operator>>( FIELDS&fields, std::pair<char*,size_t>& pchars )
{
     if ( fields.size() )
     {
#ifdef _WIN32 
/**     MSW strncpy_s                  strncpy
	-----------------------------  -------------------------
	errno_t strncpy_s(             char* strncpy(
	   char *strDest,                 char *strDest,
	   size_t numberOfElements,       const char *strSource,
	   const char *strSource,         size_t n
	   size_t count                );
	);
*/
	  strncpy_s(pchars.first, pchars.second, fields.front().c_str(), _TRUNCATE);
#else
	  strncpy(pchars.first, fields.front().c_str(), pchars.second);
#endif
	  fields.erase( fields.begin() );
     }
     return fields;
}

FIELDS& operator>>( FIELDS&fields, std::pair<BYTE*,size_t>& pbytes )
{
     if ( fields.size() )
     {
	  std::string rec(fields.front());
	  std::string::size_type s = rec.find_first_of( 'x' );

	  if ( s != std::string::npos )
	  {
	       for ( size_t i=0;i<rec.length();i+=2 )
	       {
		    if ( i/2>= pbytes.second ) break;

		    if ( isxdigit(rec[i+s+1]) )
		    {
			 pbytes.first[i/2] =  (rec[i+s+1]<='9'?(rec[i+s+1]-'0'):((rec[i+s+1]-'A')+10))<<4 |
			      ( rec[i+s+2]<='9'?(rec[i+s+2]-'0'):(rec[i+s+2]-'A')+10);
		    }
		    else
		    {
			 pbytes.first[i/2] = 0;
		    }
	       }
	  }
	  fields.erase( fields.begin() );
     }
     return fields;
}
