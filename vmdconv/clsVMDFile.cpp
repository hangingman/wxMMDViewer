#include "common.hpp"
#include "MMD_IO.hpp"
#include "d3dxhelper.hpp"
#include "clsVMDFile.hpp"

const char clsVMDFile::c_hdr_string[]={"Vocaloid Motion Data 0002"};
const char clsVMDFile::c_actor_v2[]={"miku"};
const char clsVMDFile::c_actor_v3[]={"初音ミク"};

const BYTE clsVMDFile::c_hokan_data[]={
     0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 
     0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 
     0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x6B, 
     0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x01, 
     0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x6B, 0x6B, 
     0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x01, 0x00, 
     0x14, 0x14, 0x14, 0x14, 0x14, 0x6B, 0x6B, 0x6B, 
     0x6B, 0x6B, 0x6B, 0x6B, 0x6B, 0x01, 0x00, 0x00
};
const BYTE clsVMDFile::c_hokan_data2[]={
     0x14, 0x6B, 0x14, 0x6B, 0x14, 0x6B, 0x14, 0x6B,
     0x14, 0x6B, 0x14, 0x6B, 0x14, 0x6B, 0x14, 0x6B,
     0x14, 0x6B, 0x14, 0x6B, 0x14, 0x6B, 0x14, 0x6B,
     0x23, 0x00, 0x00, 0x00, 0x00
};

VMD_HEADER::VMD_HEADER()
{
     memcpy( hdr_string,clsVMDFile::c_hdr_string,sizeof(hdr_string) );
     memset( unknown,0,sizeof(unknown) );
}
void VMD_HEADER::Export(FIELDS&rec) const
{
     NEW_FIELD(rec);
     rec << std::pair<char*,size_t>((char*)hdr_string,sizeof(hdr_string));
     rec << *(int*)unknown ;
}

void VMD_HEADER::Import(FIELDS&rec)
{
     std::pair<char*,size_t> pair((char*)hdr_string,sizeof(hdr_string));
     rec >> pair;
     rec >> *(int*)unknown;
}

VMD_V2_HEADER::VMD_V2_HEADER()
{
     memcpy( actor,clsVMDFile::c_actor_v2,sizeof(actor) );
}

void VMD_V2_HEADER::Export(FIELDS&rec) const
{
     NEW_FIELD(rec);
     rec << std::pair<char*,size_t>((char*)actor,sizeof(actor));
}
void VMD_V2_HEADER::Import(FIELDS&rec)
{
     std::pair<char*,size_t> pair((char*)actor,sizeof(actor));
     rec >> pair;
}

VMD_V3_HEADER::VMD_V3_HEADER()
{
     memcpy( actor,clsVMDFile::c_actor_v3,sizeof(actor) );
}

void VMD_V3_HEADER::Export(FIELDS&rec) const
{
     NEW_FIELD(rec);
     rec << std::pair<char*,size_t>((char*)actor,sizeof(actor));
}
void VMD_V3_HEADER::Import(FIELDS&rec)
{
     std::pair<char*,size_t> pair((char*)actor,sizeof(actor));
     rec >> pair;
}

VMD_MOTION_RECORD::VMD_MOTION_RECORD()
 :frame_no(0)
{
     memset(name,0,sizeof(name) );
     memset(pos,0,sizeof(pos) );
     memset(qt,0,sizeof(qt) );
     memcpy( pad,clsVMDFile::c_hokan_data,sizeof(pad) );
}
void VMD_MOTION_RECORD::Export(FIELDS&rec)  const
{
     float rx,ry,rz,theta,vec[3];
     NEW_FIELD(rec);
     rec << std::pair<char*,size_t>((char*)name,sizeof(name));

     rec << frame_no ;

     rec << pos[0] ;
     rec << pos[1] ;
     rec << pos[2] ;

     D3DX_HELPER::GetAngle( qt,rx,ry,rz );

     rec << ToDegree(rx) ;
     rec << ToDegree(ry) ;
     rec << ToDegree(rz) ;

     rec << std::pair<BYTE*,size_t>((BYTE*)pad,sizeof(pad));
/*
  D3DX_HELPER::GetAxisAngle( qt,vec,theta );
  theta = ToDegree(theta);
  rec << vec[0];
  rec << vec[1];
  rec << vec[2];
  rec << theta;
*/
}
void VMD_MOTION_RECORD::Import(FIELDS&rec)
{
     float rx,ry,rz;
     std::pair<char*,size_t> pair1((char*)name,sizeof(name));
     rec >> pair1;

     rec >> frame_no ;

     rec >> pos[0] ;
     rec >> pos[1] ;
     rec >> pos[2] ;
     rec >> rx ;
     rec >> ry ;
     rec >> rz ;

     D3DX_HELPER::SetAngle( qt,ToRadian(rx),ToRadian(ry),ToRadian(rz) );
     std::pair<BYTE*,size_t> pair2((BYTE*)pad,sizeof(pad));
     rec >> pair2;
}

VMD_MORP_RECORD::VMD_MORP_RECORD()
 :frame_no(0)
 ,factor(0.0)
{
}
void VMD_MORP_RECORD::Export(FIELDS&rec)  const
{
     NEW_FIELD(rec);
     rec << name;
     rec << frame_no ;
     rec << factor ;
}
void VMD_MORP_RECORD::Import(FIELDS&rec)
{
     rec >> name;
     rec >> frame_no;
     rec >> factor;

}
VMD_CAMERA_RECORD::VMD_CAMERA_RECORD()
 :frame_no(0)
{
     memset(pos,0,sizeof(pos) );
     memset(angle,0,sizeof(angle) );
     memcpy( pad,clsVMDFile::c_hokan_data2,sizeof(pad) );
}
void VMD_CAMERA_RECORD::Export(FIELDS&rec)  const
{
     NEW_FIELD(rec);
     rec << frame_no ;

     rec << pos[0] ;
     rec << pos[1] ;
     rec << pos[2] ;
     rec << pos[3] ;

     rec << ToDegree(angle[0]);
     rec << ToDegree(angle[1]);
     rec << ToDegree(angle[2]);

     rec << std::pair<BYTE*,size_t>((BYTE*)pad,sizeof(pad));
}
void VMD_CAMERA_RECORD::Import(FIELDS&rec)
{
     rec >> frame_no ;

     rec >> pos[0] ;
     rec >> pos[1] ;
     rec >> pos[2] ;
     rec >> pos[3] ;

     rec >> angle[0];
     rec >> angle[1];
     rec >> angle[2];

     angle[0] = ToRadian( angle[0] );
     angle[1] = ToRadian( angle[1] );
     angle[2] = ToRadian( angle[2] );

     std::pair<BYTE*,size_t> pair((BYTE*)pad,sizeof(pad));
     rec >> pair;
}
VMD_LIGHT_RECORD::VMD_LIGHT_RECORD()
 :frame_no(0)
{
     memset(pos,0,sizeof(pos) );
     memset(direction,0,sizeof(direction) );
}
void VMD_LIGHT_RECORD::Export(FIELDS&rec)  const
{
     NEW_FIELD(rec);
     rec << frame_no ;

     rec << pos[0] ;
     rec << pos[1] ;
     rec << pos[2] ;
     rec << direction[0] ;
     rec << direction[1] ;
     rec << direction[2] ;
}
void VMD_LIGHT_RECORD::Import(FIELDS&rec)
{
     rec >> frame_no ;

     rec >> pos[0] ;
     rec >> pos[1] ;
     rec >> pos[2] ;
     rec >> direction[0] ;
     rec >> direction[1] ;
     rec >> direction[2] ;

}

clsVMDFile::clsVMDFile(void)
{
}

clsVMDFile::~clsVMDFile(void)
{
}
bool clsVMDFile::SortByBoneNameAndFrameNo(VMD_MOTION_RECORD rec1,VMD_MOTION_RECORD rec2 )
{
     if ( strcmp( rec1.name,rec2.name )<0 )
	  return true;
     else if ( !strcmp( rec1.name,rec2.name ) )
	  return rec1.frame_no < rec2.frame_no;
     else
	  return false;
}
bool clsVMDFile::SortByMorpNameAndFrameNo(VMD_MORP_RECORD rec1,VMD_MORP_RECORD rec2 )
{
     return rec1.frame_no < rec2.frame_no;
}
bool clsVMDFile::SortByCameraFrameNo(VMD_CAMERA_RECORD rec1,VMD_CAMERA_RECORD rec2 )
{
     return rec1.frame_no < rec2.frame_no;
}
bool clsVMDFile::SortByLightFrameNo(VMD_LIGHT_RECORD rec1,VMD_LIGHT_RECORD rec2 )
{
     return rec1.frame_no < rec2.frame_no;
}

BOOL clsVMDFile::Open(const char*name )
{
     size_t size;
     std::fstream fs;

#ifdef _WIN32
     fs.open( name,std::ios::in | std::ios::binary,_SH_SECURE );
#else
     fs.open( name,std::ios::in | std::ios::binary);
#endif
     if ( fs.bad() || fs.fail() )
	  return FALSE;

     fs.read( (char*)&m_vmd_header,sizeof(m_vmd_header) );
     if ( !strcmp( GetVersion(),"file" ) ){
	  fs.read( (char*)&m_vmd_v2_header,sizeof(m_vmd_v2_header) );
     } else if ( !strcmp( GetVersion(),"0002" ) ){
	  fs.read( (char*)&m_vmd_v3_header,sizeof(m_vmd_v3_header) );
     } else {
	  fs.close();
	  return FALSE;// Error
     }
     fs.read( (char*)&size,sizeof(size) );
     if ( size ){
	  SetMotionChunkSize( size );
	  VMD_MOTION_CHUNK& buff = GetMotionChunk();
	  fs.read( (char*)&buff[0],sizeof(VMD_MOTION_RECORD)*size );
	  sort( buff.begin(),buff.end(),SortByBoneNameAndFrameNo );

     }
     fs.read( (char*)&size,sizeof(size) );
     if ( size ){
	  SetMorpChunkSize( size );
	  VMD_MORP_CHUNK& buff = GetMorpChunk();
	  fs.read( (char*)&buff[0],sizeof(VMD_MORP_RECORD)*size );
	  sort( buff.begin(),buff.end(),SortByMorpNameAndFrameNo );
     }
     fs.read( (char*)&size,sizeof(size) );
     if ( size ){
	  SetCameraChunkSize( size );
	  VMD_CAMERA_CHUNK& buff = GetCameraChunk();
	  fs.read( (char*)&buff[0],sizeof(VMD_CAMERA_RECORD)*size );
	  sort( buff.begin(),buff.end(),SortByCameraFrameNo );
     }
     fs.read( (char*)&size,sizeof(size) );
     if ( size ){
	  SetLightChunkSize( size );
	  VMD_LIGHT_CHUNK& buff = GetLightChunk();
	  fs.read( (char*)&buff[0],sizeof(VMD_LIGHT_RECORD)*size );
	  sort( buff.begin(),buff.end(),SortByLightFrameNo );
     }
     fs.close();

     return TRUE;
}
BOOL clsVMDFile::Commit(const char*name )
{
     size_t size;
     std::fstream fs;
#ifdef _WIN32
     fs.open( name,std::ios::out | std::ios::binary ,_SH_SECURE);
#else
     fs.open( name,std::ios::out | std::ios::binary);
#endif

     fs.write( (char*)&m_vmd_header,sizeof(m_vmd_header) );
     if ( fs.bad() || fs.fail() )
	  return FALSE;

     if ( !strcmp( GetVersion(),"file" ) ){
	  fs.write( (char*)&m_vmd_v2_header,sizeof(m_vmd_v2_header) );
     } else if ( !strcmp( GetVersion(),"0002" ) ){
	  fs.write( (char*)&m_vmd_v3_header,sizeof(m_vmd_v3_header) );
     } else {
	  fs.close();
	  return FALSE;// Error
     }
     size = GetMotionChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size ){
	  VMD_MOTION_CHUNK& buff = GetMotionChunk();
	  fs.write( (char*)&buff[0],sizeof(VMD_MOTION_RECORD)*size );
     }
     size = GetMorpChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size ){
	  VMD_MORP_CHUNK& buff = GetMorpChunk();
	  fs.write( (char*)&buff[0],sizeof(VMD_MORP_RECORD)*size );
     }

     size = GetCameraChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size ){
	  VMD_CAMERA_CHUNK& buff = GetCameraChunk();
	  fs.write( (char*)&buff[0],sizeof(VMD_CAMERA_RECORD)*size );
     }
     size = GetLightChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size ){
	  VMD_LIGHT_CHUNK& buff = GetLightChunk();
	  fs.write( (char*)&buff[0],sizeof(VMD_LIGHT_RECORD)*size );
     }

     fs.close();
     return TRUE;
}
const char* clsVMDFile::GetVersion() const
{
     return &m_vmd_header.hdr_string[0x15];
}
void clsVMDFile::SetVersion(const char* ver)
{
#ifdef _WIN32
     strncpy_s(&m_vmd_header.hdr_string[0x15],5,ver,4);
#else
     strncpy(&m_vmd_header.hdr_string[0x15],ver,5);
#endif
}

const VMD_HEADER& clsVMDFile::GetHeader() const
{
     return m_vmd_header;
}
VMD_HEADER& clsVMDFile::GetHeader()
{
     return m_vmd_header;
}


void clsVMDFile::SetHeader(const VMD_HEADER& header)
{
     memcpy( &m_vmd_header,&header,sizeof(m_vmd_header) );
}

const char* clsVMDFile::GetActor() const
{
     if ( !strncmp( GetVersion(),"file",4 )){
	  return m_vmd_v2_header.actor;
     } else if ( !strcmp( GetVersion(),"0002" ) ){
	  return m_vmd_v3_header.actor;
     } else {
	  return NULL;
     }

}
void	clsVMDFile::SetActor(const char* name )
{
     if ( !strncmp( GetVersion(),"file",4 )){
#ifdef _WIN32
	  strncpy_s( m_vmd_v2_header.actor ,sizeof(m_vmd_v2_header.actor),name,_TRUNCATE );
#else
	  strncpy( m_vmd_v2_header.actor, name, sizeof(m_vmd_v2_header.actor));
#endif
     } else if ( !strcmp( GetVersion(),"0002" ) ){
#ifdef _WIN32
	  strncpy_s( m_vmd_v3_header.actor ,sizeof(m_vmd_v3_header.actor),name,_TRUNCATE );
#else
	  strncpy( m_vmd_v3_header.actor, name, sizeof(m_vmd_v3_header.actor));
#endif
     }

}

VMD_MOTION_CHUNK::size_type	clsVMDFile::GetMotionChunkSize()
{
     return m_motion_chunk.size();
}
void	clsVMDFile::SetMotionChunkSize(VMD_MOTION_CHUNK::size_type size)
{
     m_motion_chunk.resize(size);
}
VMD_MOTION_CHUNK& clsVMDFile::GetMotionChunk()
{
     return m_motion_chunk;
}

VMD_MORP_CHUNK::size_type		clsVMDFile::GetMorpChunkSize()
{
     return m_morp_chunk.size();
}
void	clsVMDFile::SetMorpChunkSize(VMD_MORP_CHUNK::size_type size)
{
     return m_morp_chunk.resize(size);
}
VMD_MORP_CHUNK& clsVMDFile::GetMorpChunk()
{
     return m_morp_chunk;
}

VMD_CAMERA_CHUNK::size_type		clsVMDFile::GetCameraChunkSize()
{
     return m_camera_chunk.size();
}
void	clsVMDFile::SetCameraChunkSize(VMD_CAMERA_CHUNK::size_type size)
{
     return m_camera_chunk.resize(size);
}
VMD_CAMERA_CHUNK& clsVMDFile::GetCameraChunk()
{
     return m_camera_chunk;
}

VMD_LIGHT_CHUNK::size_type		clsVMDFile::GetLightChunkSize()
{
     return m_light_chunk.size();
}
void	clsVMDFile::SetLightChunkSize(VMD_LIGHT_CHUNK::size_type size)
{
     return m_light_chunk.resize(size);
}
VMD_LIGHT_CHUNK& clsVMDFile::GetLightChunk()
{
     return m_light_chunk;
}
/*
  VMD_ACCESSORIES_CHUNK::size_type		clsVMDFile::GetAccessoriesChunkSize()
  {
  return m_accessories_chunk.size();
  }
  void	clsVMDFile::SetAccessoriesChunkSize(VMD_ACCESSORIES_CHUNK::size_type size)
  {
  return m_accessories_chunk.resize(size);
  }
  VMD_ACCESSORIES_CHUNK& clsVMDFile::GetAccessoriesChunk()
  {
  return m_accessories_chunk;
  }
*/
