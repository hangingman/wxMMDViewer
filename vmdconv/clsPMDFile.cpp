#include "common.hpp"
#include "MMD_IO.hpp"

#include "clsPMDFile.hpp"

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
	  INFO("%s\n", "File open failed");
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

     // header1 => magic
     std::advance(mid, sizeof(m_header.magic));
     std::copy(fst, mid, reinterpret_cast<char*>(&m_header.magic) );
     fst = mid;
     // header1 => version
     for (int index = 0; index < sizeof(m_header.version); ++index ) {
	  m_Float[index] = *mid;
	  mid++;
     }
     m_header.version = MakeFloatChunk();
     fst = mid;
     // header1 => modelName
     std::advance(mid, sizeof(m_header.modelName));
     std::copy(fst, mid, reinterpret_cast<char*>(&m_header.modelName) );

     fst = mid;
     // header2
     std::advance(mid, sizeof(m_header.header2));
     std::copy(fst, mid, reinterpret_cast<char*>(&m_header.header2) );

#ifdef __WXMSW__
     INFO("header1 %s %f %s \n", m_header.magic, m_header.version, m_header.modelName);
     INFO("header2 %s\n", m_header.header2);
#else
     INFO("header1 %s %f %s \n", m_header.magic, m_header.version, ToUTF8(m_header.modelName));
     INFO("header2 %s\n", ToUTF8(m_header.header2));
#endif

     // set PMD_VERTEX_RECORD size
     unsigned char vertexHex[SIZEOF_DWORD];
     fst = mid;
     std::advance(mid, SIZEOF_DWORD);
     std::copy(fst, mid, &vertexHex[0]);

     const DWORD nVertices = clsPMDFile::GetDWORDSizeFromBin(vertexHex);
     SetVertexChunkSize(nVertices);
     DEBUG("VertexChunkSize %lu\n", nVertices);

     // set PMD_VERTEX_CHUNK
     fst = mid;
     std::advance(mid, nVertices * sizeof(PMD_VERTEX_RECORD) );
     MakeVertexChunk(fst, mid);

     // set PMD_INDEX_CHUNK size
     unsigned char indexHex[SIZEOF_DWORD];
     fst = mid;
     std::advance(mid, SIZEOF_DWORD );
     std::copy(fst, mid, &indexHex[0]);

     const DWORD nIndices = clsPMDFile::GetDWORDSizeFromBin(indexHex);
     SetIndexChunkSize(nIndices);
     DEBUG("IndexChunkSize %lu\n", nIndices);

     // set PMD_INDEX_CHUNK
     fst = mid;
     std::advance(mid, nIndices * SIZEOF_WORD );
     MakeIndexChunk(fst, mid);

     // set PMD_MATERIAL_CHUNK size
     unsigned char materialHex[SIZEOF_DWORD];
     fst = mid;
     std::advance(mid, SIZEOF_DWORD );
     std::copy(fst, mid, &materialHex[0]);

     const DWORD nMaterials = clsPMDFile::GetDWORDSizeFromBin(materialHex);
     SetMaterialChunkSize(nMaterials);
     DEBUG("MaterialChunkSize %lu\n", nMaterials);

     // set PMD_MATERIAL_CHUNK
     fst = mid;
     std::advance(mid, nMaterials * SIZEOF_MATERIAL_RECORD );
     MakeMaterialChunk(fst, mid);

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

float clsPMDFile::GetVersion()
{
     return m_header.version;
}

void clsPMDFile::SetVersion(float ver)
{
     // TODO
}

const char* clsPMDFile::GetHeaderString1()
{
     const std::string header =
	  std::string(m_header.magic)       +
	  std::string(" ")                  +
	  std::to_string(m_header.version)  +
	  std::string(" ")                  +
	  babel::sjis_to_utf8(std::string(m_header.modelName));

     return header.c_str();
}

const char* clsPMDFile::GetHeaderString2()
{
     return m_header.header2;
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
     return m_header.modelName;
}

void clsPMDFile::SetActor(const char* name )
{

#ifdef _WIN32
     strncpy_s( m_header.modelName, _countof(m_header.modelName), name, _TRUNCATE );
#else
     //strncpy( m_header.modelName, name, countof(m_header.modelName));
#endif
}

DWORD clsPMDFile::GetVertexChunkSize()
{
     return m_vertexs.size();
}
void clsPMDFile::SetVertexChunkSize(DWORD size)
{
     m_vertexs.reserve(size);
}
PMD_VERTEX_CHUNK& clsPMDFile::GetVertexChunk()
{
     return m_vertexs;
}

DWORD clsPMDFile::GetIndexChunkSize()
{
     return m_indexs.size();
}
void clsPMDFile::SetIndexChunkSize(DWORD size)
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

DWORD clsPMDFile::GetMaterialChunkSize()
{
     return m_materials.size();
}

void clsPMDFile::SetMaterialChunkSize(DWORD size)
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

DWORD clsPMDFile::GetDWORDSizeFromBin(unsigned char hex[SIZEOF_DWORD])
{
     // bin to hex
     std::string sizeStr;
     for (int i = 0; i < SIZEOF_DWORD; i++)
     {
	  if ( hex[3-i] == '0' )
	  {
	       continue;
	  }
	  else
	  {
	       std::stringstream ss;
	       ss << std::hex << (int)hex[3-i];
	       if ( ss.str().size() == 1 )
	       {
		    sizeStr += "0";
		    sizeStr += ss.str();
	       }
	       else
	       {
		    sizeStr += ss.str();
	       }
	  }
     }

     DEBUG("hex: %s\n", sizeStr.c_str());

     std::stringstream ss(sizeStr);
     DWORD size_d;
     ss >> std::hex >> size_d;
     DEBUG("num: %lu\n", size_d);

     return size_d;
}

void clsPMDFile::MakeVertexChunk(std::vector<BYTE>::const_iterator& fst, std::vector<BYTE>::const_iterator& mid)
{
     int index = 0;
     PMD_VERTEX_RECORD pvr;

     for (auto it = fst; it != mid;  ++it, ++index )
     {
	  auto & value = *it;
	  size_t offset = (index + 1) % SIZEOF_VERTEX_RECORD;
	  DEBUG("value: %x, index: %d, offset %lu, data index %d\n",
		value, index, offset, index/SIZEOF_VERTEX_RECORD);

	  if ( 1 <= offset && offset <= 32 )
	  {
	       // float x,y,z,nx,ny,nz,tx,ty...
	       AddFloatChunk(value, offset % 4);
	       if ( offset % 4 == 0 && offset != 0 )
	       {
		    float f = MakeFloatChunk();

		    switch (offset)
		    {
		    case 4:
			 pvr.x = f;
			 break;
		    case 8:
			 pvr.y = f;
			 break;
		    case 12:
			 pvr.z = f;
			 break;
		    case 16:
			 pvr.nx = f;
			 break;
		    case 20:
			 pvr.ny = f;
			 break;
		    case 24:
			 pvr.nz = f;
			 break;
		    case 28:
			 pvr.tx = f;
			 break;
		    case 32:
			 pvr.ty = f;
			 break;
		    default:
			 DEBUG("some data is out of VertexChunkSize index %d\n", index);
			 break;
		    }
	       }
	  }
	  else if ( 33 <= offset && offset <= 36 )
	  {
	       // WORD b1,b2
	       AddWordChunk(value, offset % 2);
	       if ( offset % 2 == 0 )
	       {
		    WORD w = MakeWordChunk();

		    switch (offset)
		    {
		    case 34:
			 pvr.b1 = w;
			 break;
		    case 36:
			 pvr.b2 = w;
			 break;
		    }
	       }
	  }
	  else if ( offset == 37 || offset == 0 )
	  {
	       // BYTE bw,unknown
	       if ( offset == 37 )
	       {
		    DEBUG("BYTE bw HEX %x\n", value);
		    pvr.bw = value;
	       }
	       else if ( offset == 0 )
	       {
		    DEBUG("BYTE unknown HEX %x\n", value);
		    pvr.unknown = value;
	       }
	  }
	  else
	  {
	       DEBUG("some data is out of VertexChunkSize index %d\n", index);
	       break; // ERROR
	  }

	  if ( index != 0 && offset == 0 )
	  {
	       m_vertexs.push_back(pvr);
	  }
     }

#if defined(DEBUG_BUILD) && defined(__GNUC__)
     for (auto it = m_vertexs.begin(); it != m_vertexs.end(); ++it)
     {
	  DEBUG("x:%f, y:%f, z:%f, nx:%f, ny:%f, nz:%f, tx:%f, ty:%f,"
		"b1:%d, b2:%d, bw:%x, uk:%x\n",
		it->x, it->y, it->z, it->nx, it->ny, it->nz, it->tx, it->ty,
		it->b1, it->b2, it->bw, it->unknown);
     }
#endif

}

void clsPMDFile::MakeIndexChunk(std::vector<BYTE>::const_iterator& fst, std::vector<BYTE>::const_iterator& mid)
{
     int index = 0;

     for (auto it = fst; it != mid;  ++it, ++index )
     {
	  auto & value = *it;
	  size_t offset = (index + 1) % SIZEOF_WORD;
	  DEBUG("value: %x, index: %d, offset %lu\n", value, index, offset);

	  AddWordChunk(value, offset % 2);
	  if ( index != 0 && offset % 2 == 0 )
	  {
	       WORD w = MakeWordChunk();
	       m_indexs.push_back(w);
	  }
     }
}

void clsPMDFile::MakeMaterialChunk(std::vector<BYTE>::const_iterator& fst, std::vector<BYTE>::const_iterator& mid)
{
     int index = 0;
     PMD_MATERIAL_RECORD pmr;

     for (auto it = fst; it != mid;  ++it, ++index )
     {
	  auto & value = *it;
	  size_t offset = (index + 1) % SIZEOF_MATERIAL_RECORD;
	  DEBUG("value: %x, index: %d, offset %lu, data index %d\n",
		value, index, offset, index/SIZEOF_MATERIAL_RECORD);

	  if ( 1 <= offset && offset <= 44 )
	  {
	       AddFloatChunk(value, offset % 4);
	       if ( offset % 4 == 0 && offset != 0 )
	       {
		    float f = MakeFloatChunk();

		    switch (offset)
		    {
		    case 4:
			 pmr.diffuse.r = f;
			 break;
		    case 8:
			 pmr.diffuse.g = f;
			 break;
		    case 12:
			 pmr.diffuse.b = f;
			 break;
		    case 16:
			 pmr.diffuse.a = f;
			 break;
		    case 20:
			 pmr.shininess = f;
			 break;
		    case 24:
			 pmr.specular.r = f;
			 break;
		    case 28:
			 pmr.specular.g = f;
			 break;
		    case 32:
			 pmr.specular.b = f;
			 break;
		    case 36:
			 pmr.ambient.r = f;
			 break;
		    case 40:
			 pmr.ambient.g = f;
			 break;
		    case 44:
			 pmr.ambient.b = f;
			 break;
		    default:
			 DEBUG("some data is out of VertexChunkSize index %d\n", index);
			 break;
		    }
	       }
	  }
	  else if ( 45 <= offset && offset <= 46 )
	  {
	       // WORD p12
	       AddWordChunk(value, offset % 2);
	       if ( offset % 2 == 0 )
	       {
		    WORD w = MakeWordChunk();

		    switch (offset)
		    {
		    case 46:
			 pmr.p12 = w;
			 break;
		    }
	       }
	  }
	  else if ( 47 <= offset && offset <= 50 )
	  {
	       // DWORD nEdges
	       AddDwordChunk(value, offset % 2);
	       if ( offset % 2 == 0 )
	       {
		    DWORD d = MakeDwordChunk();

		    switch (offset)
		    {
		    case 50:
			 pmr.nEdges = d;
			 break;
		    }
	       }
	  }
	  else if ( 51 <= offset && offset <= 69 || offset == 0 )
	  {
	       const int fIndex = offset == 0 ? 19 : offset - 51;
	       pmr.textureFileName[fIndex] = value;
	  }

	  if ( index != 0 && offset == 0 )
	  {
	       m_materials.push_back(pmr);
	  }
     }
}

void  clsPMDFile::AddFloatChunk(BYTE b, int index)
{
     if ( 0 <= index && index <= SIZEOF_FLOAT -1 )
     {
	  if ( index == 0)
	  {
	       index = SIZEOF_FLOAT;
	       m_Float[SIZEOF_FLOAT] = 0x00;
	  }

	  m_Float[index-1] = b;
     }
}

float clsPMDFile::MakeFloatChunk(bool debug)
{
     std::string floatHex;
     for (int i = 0; i < SIZEOF_FLOAT; i++)
     {
	  if ( m_Float[SIZEOF_FLOAT -1 -i] == 0x00 )
	  {
	       floatHex += "00";
	  }
	  else
	  {
	       std::stringstream ss;
	       ss << std::hex << (int)m_Float[SIZEOF_FLOAT -1 -i];
	       if ( ss.str().size() == 1 )
	       {
		    floatHex += "0";
		    floatHex += ss.str();
	       }
	       else
	       {
		    floatHex += ss.str();
	       }
	  }
     }

     union IntFloat val;
     std::stringstream ss(floatHex);
     ss.setf(std::ios::hex, std::ios::basefield);
     if (debug)
     {
	  INFO("Float HEX %s\n", floatHex.c_str());
     }

     ss >> std::hex >> val.i;

     return val.f;
}

void  clsPMDFile::AddWordChunk(BYTE b, int index)
{
     if ( 0 <= index && index <= SIZEOF_WORD -1 )
     {
	  if ( index == 0)
	  {
	       index = SIZEOF_WORD;
	       m_Word[SIZEOF_WORD] = 0x00;
	  }

	  m_Word[index-1] = b;
     }
}

WORD clsPMDFile::MakeWordChunk()
{
     std::string wordHex;
     for (int i = 0; i < SIZEOF_WORD; i++)
     {
	  if ( m_Word[SIZEOF_WORD -1 -i] == 0x00 )
	  {
	       wordHex += "00";
	  }
	  else
	  {
	       std::stringstream ss;
	       ss << std::hex << (int)m_Word[SIZEOF_WORD -1 -i];
	       if ( ss.str().size() == 1 )
	       {
		    wordHex += "0";
		    wordHex += ss.str();
	       }
	       else
	       {
		    wordHex += ss.str();
	       }
	  }
     }

     WORD w;
     std::stringstream ss(wordHex);
     ss.setf(std::ios::hex, std::ios::basefield);
     DEBUG("WORD HEX %s\n", wordHex.c_str());
     ss >> std::hex >> w;

     return w;
}

void  clsPMDFile::AddDwordChunk(BYTE b, int index)
{
     if ( 0 <= index && index <= SIZEOF_DWORD -1 )
     {
	  if ( index == 0)
	  {
	       index = SIZEOF_DWORD;
	       m_Dword[SIZEOF_DWORD] = 0x00;
	  }

	  m_Dword[index-1] = b;
     }
}

DWORD clsPMDFile::MakeDwordChunk()
{
     std::string dwordHex;
     for (int i = 0; i < SIZEOF_DWORD; i++)
     {
	  if ( m_Dword[SIZEOF_DWORD -1 -i] == 0x00 )
	  {
	       dwordHex += "00";
	  }
	  else
	  {
	       std::stringstream ss;
	       ss << std::hex << (int)m_Dword[SIZEOF_DWORD -1 -i];
	       if ( ss.str().size() == 1 )
	       {
		    dwordHex += "0";
		    dwordHex += ss.str();
	       }
	       else
	       {
		    dwordHex += ss.str();
	       }
	  }
     }

     union IntDword val;
     std::stringstream ss(dwordHex);
     ss.setf(std::ios::hex, std::ios::basefield);
     DEBUG("Dword HEX %s\n", dwordHex.c_str());
     ss >> std::hex >> val.i;

     return val.d;
}


std::string clsPMDFile::StringToHex(const std::string& input)
{
     static const char* const lut = "0123456789ABCDEF";
     size_t len = input.length();

     std::string output;
     output.reserve(5 * len);
     for (size_t i = 0; i < len; ++i)
     {
	  const unsigned char c = input[i];
	  output.push_back('0');
	  output.push_back('x');
	  output.push_back(lut[c >> 4]);
	  output.push_back(lut[c & 15]);
	  output.push_back(' ');
     }
     return output;
}
