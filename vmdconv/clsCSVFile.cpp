#include "common.hpp"
#include "MMD_IO.hpp"
#include "clsCSVFile.hpp"

void PARSE_LINE::Import( const std::vector<std::string>&_fields ) 
{
     fields = _fields;
}
void PARSE_LINE::Export( std::vector<std::string>&_fields ) const
{
     _fields = fields;
}
const std::string PARSE_LINE::ToText()
{
     std::string text;
     fields >> text;
     return text;
}

clsCSVFile::clsCSVFile(void)
{
}

clsCSVFile::~clsCSVFile(void)
{
}

BOOL clsCSVFile::Open(const std::string&name )
{
     FIELDS fields;
     std::string text;
     std::fstream fs;

#ifdef _WIN32
     fs.open( name.c_str(), std::ios::in,_SH_SECURE );
#else
     fs.open( name.c_str(), std::ios::in);
#endif

     if ( fs.bad() || fs.fail() )
	  return FALSE;

     while ( !fs.eof() ){
	  getline( fs,text );
	  NEW_FIELD(fields);
	  fields << text;
	  AddFields( fields );
     }
     fs.close();
     return TRUE;
}
BOOL clsCSVFile::Commit(const std::string&name )
{
     std::fstream fs;

#ifdef _WIN32
     fs.open( name.c_str(), std::ios::in,_SH_SECURE );
#else
     fs.open( name.c_str(), std::ios::in);
#endif

     if ( fs.bad() || fs.fail() )
	  return FALSE;

     for ( PARSE_LINE_BUFFER::size_type i=0;i<m_csv.size();i++ ){
	  fs << m_csv[i].ToText() << std::endl;
     }
     fs.close();
     return TRUE;
}

PARSE_LINE_BUFFER& clsCSVFile::GetBuffer()
{
     return m_csv;
}
void clsCSVFile::AddFields( const std::vector<std::string>& fields ) 
{
     PARSE_LINE line;
     line.Import( fields );
     m_csv.push_back( line );
}
