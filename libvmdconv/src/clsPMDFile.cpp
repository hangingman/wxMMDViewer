#include "common.hpp"
#include "MMD_IO.hpp"

#include "clsPMDFile.hpp"

#define SIZEOF_VERTEX 4

BOOL clsPMDFile::Open(const char* name)
{

     BYTE   size_b;
     WORD   size_w;
     size_t size;

     // open the file:
     std::streampos fileSize;
     std::ifstream fs;
     
     fs.open(name, std::ios::binary);
     if ( fs.bad() || fs.fail() )
     {
	  DEBUG("%s\n", "File open failed");
	  return FALSE;
     }

     // get its size:
     fs.seekg(0, std::ios::end);
     fileSize = fs.tellg();
     fs.seekg(0, std::ios::beg);

     // read the data:
     std::vector<BYTE> temporary(fileSize);
     fs.read((char*) &temporary[0], fileSize);

     std::vector<BYTE>::const_iterator fst = temporary.begin();
     std::vector<BYTE>::const_iterator mid = temporary.begin();
     std::advance(mid, sizeof(m_header));
     // header
     std::copy(fst, mid, reinterpret_cast<char*>(&m_header) );

     DEBUG("header1 %s\n", &m_header.header1);
     DEBUG("header2 %s\n", &m_header.header2);

     // set PMD_VERTEX_RECORD size
     fst = mid;
     std::advance(mid, SIZEOF_VERTEX);
     char vertexHex[SIZEOF_VERTEX];
     std::copy(fst, mid, &vertexHex[0]);

     // bin to hex
     std::string vertexSize;
     for (int i = 0; i < SIZEOF_VERTEX; i++)
     {
	  if ( vertexHex[3-i] == '0' )
	  {
	       continue;
	  }
	  else
	  {
	       std::stringstream ss;
	       ss << std::hex << (int)vertexHex[3-i];
	       if ( ss.str().size() == 1 )
	       {
		    vertexSize += "0";
		    vertexSize += ss.str();
	       }
	       else
	       {
		    vertexSize += ss.str();
	       }
	  }
     }

     DEBUG("hex: %s\n", vertexSize.c_str());

     DWORD size_d;
     std::stringstream ss(vertexSize);
     ss >> std::hex >> size_d;
     DEBUG("num: %lu\n", size_d);

     SetVertexChunkSize(size_d);
     DEBUG("VertexChunkSize %lu\n", size_d);

     // set PMD_VERTEX_RECORD
     fst = mid;
     std::advance(mid, size_d * sizeof(PMD_VERTEX_RECORD));
     auto vertexFst = reinterpret_cast<PMD_VERTEX_CHUNK::const_iterator>(fst);

     /**
     PMD_VERTEX_CHUNK::const_iterator
	  vertexMid = static_cast< PMD_VERTEX_CHUNK::const_iterator >(mid);
     */
     /**
     std::transform(fst, mid, 
		    back_inserter( m_vertexs ),
		    [] () -> PMD_VERTEX_RECORD& { 
			 PMD_VERTEX_RECORD;
		    });*/
     //std::copy(fst, mid, reinterpret_cast<BYTE*>(&m_vertexs));

#if defined(DEBUG_BUILD) && defined(__GNUC__)
     for (auto it = m_vertexs.begin(); it != m_vertexs.end(); ++it)
     {
	  DEBUG("x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f, tx:%f, ty:%f", 
		it->x, it->y, it->z, it->nx, it->ny, it->nz, it->tx, it->ty);
     }
#endif

     return true;

/**
   fs.read( (char*)&m_header,sizeof(m_header) );
   DEBUG("header1 %s\n", &m_header.header1);
   DEBUG("header2 %s\n", &m_header.header2);
     
   fs.read( (char*)&size,sizeof(size) );
   SetVertexChunkSize(size);
   DEBUG("VertexChunkSize %d\n", size);

   DEBUG("sizeof(PMD_VERTEX_RECORD) %d\n", sizeof(PMD_VERTEX_RECORD));
   DEBUG("size*sizeof(PMD_VERTEX_RECORD) %d\n", size*sizeof(PMD_VERTEX_RECORD));
*/  
     if ( size )
     {
	  try 
	  {
	       fs.clear();
	       fs.seekg( sizeof(m_header) + sizeof(size), std::ios_base::beg);
	       fs.read( (char*)(&m_vertexs[0]), size*sizeof(PMD_VERTEX_RECORD) );
	       //inf.read( (char*)( &mDataBuffer[0] ), bytesAvailable ) ;
	       DEBUG("end get VertexChunkSize\n");
	  }
	  catch(std::exception& e)
	  {
	       std::cerr << typeid(e).name() << std::endl;
	       std::cerr << e.what() << std::endl;

	       return FALSE;
	  }
     }

     return true;

     fs.read( (char*)&size,sizeof(size) );
     SetIndexChunkSize(size);
     if ( size )
	  fs.read( (char*)&GetIndexChunk()[0],size*sizeof(WORD) );

     fs.read( (char*)&size,sizeof(size) );
     SetMaterialChunkSize(size);
     if ( size )
	  fs.read( (char*)&GetMaterialChunk()[0],size*sizeof(PMD_MATERIAL_RECORD) );

     fs.read( (char*)&size_w,sizeof(size_w) );
     SetBoneChunkSize( size_w );
     if ( size_w )
	  fs.read( (char*)&GetBoneChunk()[0],size_w*sizeof(PMD_BONE_RECORD) );

     fs.read( (char*)&size_w,sizeof(size_w) );
     SetIKChunkSize( size_w );
     for (int i=0;i<size_w;i++){
	  GetIKChunk()[i].Read(fs);
     }

     fs.read( (char*)&size_w,sizeof(size_w) );
     SetMorpChunkSize( size_w );
     for (int i=0;i<size_w;i++){
	  GetMorpChunk()[i].Read(fs);
     }

     fs.read( (char*)&size_b,sizeof(size_b) );
     SetCtrlChunkSize( size_b );
     if ( size_b )
	  fs.read( (char*)&GetCtrlChunk()[0],size_b*sizeof(WORD) );

     fs.read( (char*)&size_b,sizeof(size_b) );
     SetGrpNameChunkSize( size_b );
     if ( size_b )
	  fs.read( (char*)&GetGrpNameChunk()[0],size_b*sizeof(PMD_GRP_NAME_RECORD) );

     fs.read( (char*)&size,sizeof(size) );
     SetGrpChunkSize( size );
     if ( size )
	  fs.read( (char*)&GetGrpChunk()[0],size*sizeof(PMD_GRP_RECORD) );

     return TRUE;
}

BOOL clsPMDFile::Commit(const char* name)
{
     BYTE   size_b;
     WORD   size_w;
     size_t size;
     std::fstream fs;

#ifdef _WIN32
	fs.open( name,std::ios::in | std::ios::binary,_SH_SECURE );
#else
	fs.open( name,std::ios::in | std::ios::binary);
#endif
     if ( fs.bad() || fs.fail() )
	  return FALSE;

     fs.write( (char*)&m_header,sizeof(m_header) );

     size = GetVertexChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size )
	  fs.write( (char*)&GetVertexChunk()[0],size*sizeof(PMD_VERTEX_RECORD) );

     size = GetIndexChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size )
	  fs.write( (char*)&GetIndexChunk()[0],size*sizeof(WORD) );

     size=GetMaterialChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size )
	  fs.write( (char*)&GetMaterialChunk()[0],size*sizeof(PMD_MATERIAL_RECORD) );

     size_w = GetBoneChunkSize();
     fs.write( (char*)&size_w,sizeof(size_w) );
     if ( size_w )
	  fs.write( (char*)&GetBoneChunk()[0],size_w*sizeof(PMD_BONE_RECORD) );

     size_w = GetIKChunkSize();
     fs.write( (char*)&size_w,sizeof(size_w) );
     for (int i=0;i<size_w;i++){
	  GetIKChunk()[i].Write(fs);
     }

     size_w = GetMorpChunkSize();
     fs.write( (char*)&size_w,sizeof(size_w) );
     for (int i=0;i<size_w;i++){
	  GetMorpChunk()[i].Write(fs);
     }

     size_b = GetCtrlChunkSize();
     fs.write( (char*)&size_b,sizeof(size_b) );
     if ( size_b )
	  fs.write( (char*)&GetCtrlChunk()[0],size_b*sizeof(WORD) );

     size_b = GetGrpNameChunkSize(  );
     fs.write( (char*)&size_b,sizeof(size_b) );
     if ( size_b )
	  fs.write( (char*)&GetGrpNameChunk()[0],size_b*sizeof(PMD_GRP_NAME_RECORD) );

     size = GetGrpChunkSize();
     fs.write( (char*)&size,sizeof(size) );
     if ( size )
	  fs.write( (char*)&GetGrpChunk()[0],size*sizeof(PMD_GRP_RECORD) );

     return TRUE;
}

int clsPMDFile::GetVersion()
{
     return 0;
}

void clsPMDFile::SetVersion(int ver)
{
}

const char* clsPMDFile::GetHeaderString()
{
     return m_header.header1;
}

void clsPMDFile::SetHeaderString(const char* name)
{
/**---------------------------------------------------

void *memcpy(         errno_t memcpy_s(	    
   void *dest,		 void *dest,	    
   const void *src,	 size_t sizeInBytes,
   size_t count		 const void *src,   
);			 size_t count	      
		      );		   
------------------------------------------------------*/

#ifdef _WIN32
     memcpy_s( (char*)&m_header, sizeof(m_header)-1,name, _TRUNCATE );
#else
     memcpy( (char*)&m_header, name, sizeof(m_header)-1);
#endif
}

const char* clsPMDFile::GetActor()
{
     return &m_header.header1[0x7];
}
void clsPMDFile::SetActor(const char* name )
{

#ifdef _WIN32
     strncpy_s( &m_header.header1[0x7],_countof(m_header.header1)-0x7,name,_TRUNCATE );
#else
     strncpy( &m_header.header1[0x7],name, countof(m_header.header1) - 0x7 );
#endif
}

int clsPMDFile::GetVertexChunkSize()
{
     return m_vertexs.size();
}
void clsPMDFile::SetVertexChunkSize(int size)
{
     m_vertexs.reserve(size);
}
PMD_VERTEX_CHUNK& clsPMDFile::GetVertexChunk()
{
     return m_vertexs;
}

int clsPMDFile::GetIndexChunkSize()
{
     return m_indexs.size();
}
void clsPMDFile::SetIndexChunkSize(int size)
{
     m_indexs.reserve(size);
}
PMD_INDEX_CHUNK& clsPMDFile::GetIndexChunk()
{
     return m_indexs;
}

int clsPMDFile::GetBoneChunkSize()
{
     return m_bones.size();
}
void clsPMDFile::SetBoneChunkSize(int size)
{
     m_bones.reserve( size );
}
std::vector<PMD_BONE_RECORD>& clsPMDFile::GetBoneChunk()
{
     return m_bones;
}
int clsPMDFile::GetIKChunkSize()
{
     return m_ikbones.size();
}
void clsPMDFile::SetIKChunkSize(int size)
{
     m_ikbones.reserve(size);
}
PMD_IK_CHUNK& clsPMDFile::GetIKChunk()
{
     return m_ikbones;
}

int clsPMDFile::GetMaterialChunkSize()
{
     return m_materials.size();
}

void clsPMDFile::SetMaterialChunkSize(int size)
{
     m_materials.reserve( size );
}

PMD_MATERIAL_CHUNK& clsPMDFile::GetMaterialChunk()
{
     return m_materials;
}

int clsPMDFile::GetMorpChunkSize()
{
     return m_morps.size();
}

void clsPMDFile::SetMorpChunkSize(int size)
{
     m_morps.reserve(size);
}

PMD_MORP_CHUNK& clsPMDFile::GetMorpChunk()
{
     return m_morps;
}
int clsPMDFile::GetCtrlChunkSize()
{
     return m_ctrls.size();
}
void clsPMDFile::SetCtrlChunkSize(int size)
{
     m_ctrls.reserve(size);
}
std::vector<WORD>& clsPMDFile::GetCtrlChunk()
{
     return m_ctrls;
}

int clsPMDFile::GetGrpNameChunkSize()
{
     return m_grp_name.size();
}
void clsPMDFile::SetGrpNameChunkSize(int size)
{
     m_grp_name.reserve(size);
}
PMD_GRP_NAME_CHUNK& clsPMDFile::GetGrpNameChunk()
{
     return m_grp_name;
}

int clsPMDFile::GetGrpChunkSize()
{
     return m_grp.size();
}
void clsPMDFile::SetGrpChunkSize(int size)
{
     m_grp.reserve(size);
}
PMD_GRP_CHUNK& clsPMDFile::GetGrpChunk()
{
     return m_grp;
}
