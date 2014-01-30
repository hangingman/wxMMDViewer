/* wxMMDViewer - VMD file or PMD file viewer with wxWidgets and OpenGL
 * Copyright (C) 2014 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#include <unistd.h>
#include <memory>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 
#include <chrono>
#include <thread>
#include "clsPMDFile.hpp"
#include "wx/wxnkf.h"

int compare(const PMD_VERTEX_CHUNK& left, const PMD_VERTEX_CHUNK& right);

int main()
{
     clsPMDFile pmdFile;
     std::string pmdPath = "data/HatsuneMiku.pmd";
     int ret = 0;

     try {
	  std::cout << "Loading file is:" << pmdPath << std::endl;
	  bool vmdRet = pmdFile.Open(pmdPath.c_str());

	  if (!vmdRet)
	  {
	       return -2;
	  }
	  
     }
     catch (const char* str)
     {
	  std::cout << str << std::endl;
	  ret = -2;
     }

     // load PMD file vertex info csv
     bool start = false;
     std::string csvPath = "data/HatsuneMiku_vertex.csv", str = "";
     std::ifstream ifs(csvPath);
 
     if( !ifs ) {
	  std::cout << "Error:Input data file not found" << std::endl;
	  return -2;
     }

     // data in CSV
     PMD_VERTEX_CHUNK pmdVertexChunkCSV;
 
     while( std::getline( ifs, str ) ){
	  std::string token; 
	  std::istringstream stream( str );

	  // line is vertex array first ?
	  if ( std::strncmp(str.c_str(), "0,", sizeof("0,") - 1) == 0 )
	  {
	       start = true;
	  }

	  while( start && std::getline( stream, token, ',' ) ) {

	       std::stringstream ss;

	       PMD_VERTEX_RECORD pmdVertexRecord;

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.x;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.y;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.z;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.nx;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.ny;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.nz;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.tx;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.ty;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.b1;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.b2;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.bw;
	       ss.clear(std::stringstream::goodbit);

	       if ( ! std::getline( stream, token, ',' )) { break; }
	       ss << token;
	       ss >> pmdVertexRecord.unknown;
	       ss.clear(std::stringstream::goodbit);

	       pmdVertexChunkCSV.push_back(pmdVertexRecord);
	  }
     }

     // compare clsPMDFile and CSV data
     ret = compare(pmdFile.m_vertexs, pmdVertexChunkCSV);

     return ret;
}

int compare(const PMD_VERTEX_CHUNK& left, const PMD_VERTEX_CHUNK& right) {

     auto leftIt = left.begin();
     auto rightIt = right.begin();
     std::vector<int> diffContainer;
     int diff = 0, ret = 0;
     std::chrono::milliseconds sleep( 50 );

     while (leftIt != left.end() && rightIt != right.end()) {

	  if (leftIt->x	 != rightIt->x	||
	      leftIt->y	 != rightIt->y	||
	      leftIt->z	 != rightIt->z	||
	      leftIt->nx != rightIt->nx ||
	      leftIt->ny != rightIt->ny ||
	      leftIt->nz != rightIt->nz	||
	      leftIt->tx != rightIt->tx ||
	      leftIt->ty != rightIt->ty) 
	  {
	       /** Data is somewhat wrong... */
	       std::cout << "Error: Data is not match at " << diff << std::endl;
	       std::cout << "clsPMDFile x:" << leftIt->x
			 << " y:"  <<  leftIt->y
			 << " z:"  <<  leftIt->z
			 << " nx:" <<  leftIt->nx
			 << " ny:" <<  leftIt->ny
			 << " nz:" <<  leftIt->nz
			 << " tx:" <<  leftIt->tx
			 << " ty:" <<  leftIt->ty
			 << std::endl;

	       std::cout << "csv file   x:" << rightIt->x
			 << " y:"  <<  rightIt->y
			 << " z:"  <<  rightIt->z
			 << " nx:" <<  rightIt->nx
			 << " ny:" <<  rightIt->ny
			 << " nz:" <<  rightIt->nz
			 << " tx:" <<  rightIt->tx
			 << " ty:" <<  rightIt->ty
			 << std::endl;

	       return -2;
	  }
	  else
	  {
	       std::cout << "index: " << diff << " is OK..." << std::endl;
	       std::this_thread::sleep_for( sleep );
	  }
	  
	  ++leftIt;
	  ++rightIt;
	  ++diff;
     }

     return ret;
}
