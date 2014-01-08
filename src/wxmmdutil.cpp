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

#include "wxmmdutil.hpp"

/**
 * CSVファイルからVMDへの変換
 * @param const char* ファイル名
 */
int wxMMDViewerUtil::CSV2VMD( const char*name )
{
     size_t line = 0,size;
     FIELDS fields;
     clsVMDFile vmd;
     clsCSVFile csv;

     std::string in_name(name);
     std::string out_name(name);

     std::string::size_type pos = out_name.find_last_of(".");
     if ( pos == std::string::npos )
	  return -1;
     out_name.replace(pos,4,".vmd");
	
     if ( !csv.Open( in_name.c_str() ) )
	  return -1;
     PARSE_LINE_BUFFER& buff = csv.GetBuffer();
	
     if ( buff.size() < 3 ) 
	  return -2;
     buff[line++].Export( fields );
     vmd.GetHeader().Import( fields ) ;
     buff[line++].Export( fields );
     if ( fields.size() )
	  vmd.SetActor( fields[0].c_str() );
     else
	  return -2;

     buff[line++].Export( fields );
     fields >> size;
     if ( buff.size()<=line )
	  return -2;

     vmd.SetMotionChunkSize( size );

     for ( size_t i=0;i<size;i++ ) 
     {
	  buff[line++].Export( fields );
	  vmd.GetMotionChunk()[i].Import( fields );
	  if ( buff.size()<=line )
	       return -2;
     }
     buff[line++].Export( fields );
     fields >> size;
     vmd.SetMorpChunkSize( size );
     for ( size_t i=0;i<size;i++ ) 
     {
	  buff[line++].Export( fields );
	  vmd.GetMorpChunk()[i].Import( fields );
	  if ( buff.size()<=line )
	       return -2;
     }
     buff[line++].Export( fields );
     fields >> size;
     vmd.SetCameraChunkSize( size );
     for ( size_t i=0;i<size;i++ ) 
     {
	  buff[line++].Export( fields );
	  vmd.GetCameraChunk()[i].Import( fields );
	  if ( buff.size()<=line )
	       return -2;
     }
     buff[line++].Export( fields );
     fields >> size;
     vmd.SetLightChunkSize( size );
     for ( size_t i=0;i<size;i++ ) 
     {
	  buff[line++].Export( fields );
	  vmd.GetLightChunk()[i].Import( fields );
	  if ( buff.size()<=line )
	       return -2;
     }

     if ( !vmd.Commit( out_name.c_str() ) )
	  return -3;
     return 0;
}

/**
 * VMDファイルからCSVへの変換
 * @param const char* ファイル名
 */
int wxMMDViewerUtil::VMD2CSV( const char*name )
{
     FIELDS fields;
     clsVMDFile vmd;
     clsCSVFile csv;

     std::string in_name(name);
     std::string out_name(name);

     std::string::size_type pos = out_name.find_last_of(".");
     if ( pos == std::string::npos )
	  return -1;
	
     out_name.replace(pos,4,".csv");

     if ( !vmd.Open( in_name.c_str() ) )
	  return -1;

     if ( !vmd.GetActor() ) 
     {
	  return -1;
     }

     vmd.GetHeader().Export( fields );
     csv.AddFields( fields );

     NEW_FIELD(fields);
     fields << vmd.GetActor();
     csv.AddFields( fields );
     {
	  VMD_MOTION_CHUNK &buff = vmd.GetMotionChunk();

	  NEW_FIELD(fields);
	  fields << vmd.GetMotionChunkSize();
	  csv.AddFields( fields );

	  for (size_t i=0;i<vmd.GetMotionChunkSize();i++) 
	  {
	       VMD_MOTION_RECORD &rec = buff[i];
	       rec.Export( fields );
	       csv.AddFields( fields );
	  }
     }
     {
	  VMD_MORP_CHUNK &buff = vmd.GetMorpChunk();

	  NEW_FIELD(fields);
	  fields << vmd.GetMorpChunkSize();
	  csv.AddFields( fields );

	  for (size_t i=0;i<vmd.GetMorpChunkSize();i++) 
	  {
	       VMD_MORP_RECORD &rec = buff[i];
	       rec.Export( fields );
	       csv.AddFields( fields );
	  }
     }
     {
	  VMD_CAMERA_CHUNK &buff = vmd.GetCameraChunk();

	  NEW_FIELD(fields);
	  fields << vmd.GetCameraChunkSize();
	  csv.AddFields( fields );

	  for (size_t i=0;i<vmd.GetCameraChunkSize();i++) 
	  {
	       VMD_CAMERA_RECORD &rec = buff[i];
	       rec.Export( fields );
	       csv.AddFields( fields );
	  }
     }
     {
	  VMD_LIGHT_CHUNK &buff = vmd.GetLightChunk();

	  NEW_FIELD(fields);
	  fields << vmd.GetLightChunkSize();
	  csv.AddFields( fields );

	  for (size_t i=0;i<vmd.GetLightChunkSize();i++) 
	  {
	       VMD_LIGHT_RECORD &rec = buff[i];
	       rec.Export( fields );
	       csv.AddFields( fields );
	  }
     }
     if ( !csv.Commit( out_name.c_str() ) )
	  return -3;
     return 0;
}
