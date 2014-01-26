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
#include "clsPMDFile.hpp"
#include "wx/wxnkf.h"

int main()
{
     std::unique_ptr<clsPMDFile> pmdFile(new clsPMDFile());
     std::string s_cwd = "data/HatsuneMiku.pmd";
     int ret = 0;

     try {
	  std::cout << "Loading file is:" << s_cwd << std::endl;
	  bool vmdRet = pmdFile->Open(s_cwd.c_str());

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

     // header1[]
     const char* header1 = pmdFile->GetHeaderString1();
     if ( strstr(header1, "Pmd") )
     {
	  std::cout << "header1 [ヘッダ１]: " << header1 << " ...OK!" << std::endl;
     }
     else
     {
	  std::cout << "header1 [ヘッダ１]: " << header1 << " ...NG" << std::endl;
	  return -2;
     }
     
     // header2[]
     const char* header2 = pmdFile->GetHeaderString2();
     const std::string header2SJIS = header2;
     std::string header2UTF8;

     std::unique_ptr<wxNKF> nkf(new wxNKF());
     nkf->ConvertSTDString(header2SJIS, header2UTF8, "--oc=UTF-8 --ic=CP932");

     if ( strstr(header2, "CRYPTON FUTURE MEDIA") )
     {
	  std::cout << "header2 [ヘッダ２]: " << header2UTF8 << " ...OK!" << std::endl;
     }
     else
     {
	  std::cout << "header2 [ヘッダ２]: " << header2UTF8 << " ...NG" << std::endl;
	  return -2;
     }

     // nVertices[]
     const DWORD nVertices = pmdFile->GetVertexChunkSize();
     if ( nVertices == 9036 )
     {
	  std::cout << "nVertices [頂点数]: " << nVertices << " ...OK!" << std::endl;
     }
     else
     {
	  std::cout << "nVertices [頂点数]: " << nVertices << " ...NG" << std::endl;
	  return -2;
     }

     // nIndices[]
     const DWORD nIndices = pmdFile->GetIndexChunkSize();
     if ( nIndices == 44991 )
     {
	  std::cout << "nIndices [頂点番号]: " << nIndices << " ...OK!" << std::endl;
     }
     else
     {
	  std::cout << "nIndices [頂点番号]: " << nIndices << " ...NG" << std::endl;
	  return -2;
     }     

     return ret;
}
