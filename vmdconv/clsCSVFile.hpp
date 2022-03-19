#ifndef CLSCSVFILE_HPP
#define CLSCSVFILE_HPP

#include "common.hpp"

class PARSE_LINE 
{
public:
	void Import( const FIELDS&fields );
	void Export( FIELDS&fields ) const;
	const std::string ToText(); 
private:
	FIELDS fields;
};

class clsCSVFile
{
public:
	clsCSVFile(void);
	~clsCSVFile(void);

	BOOL Open(const std::string&name );
	BOOL Commit(const std::string&name );

	std::vector<PARSE_LINE>& GetBuffer();
	void AddFields( const FIELDS& fields ) ;

public:
	std::vector<PARSE_LINE> m_csv;
};


typedef std::vector<PARSE_LINE> PARSE_LINE_BUFFER;

#endif /** CLSCSVFILE_HPP */
