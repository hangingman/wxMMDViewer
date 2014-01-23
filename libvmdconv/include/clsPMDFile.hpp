#ifndef CLSPMDFILE_HPP
#define CLSPMDFILE_HPP

#include "common.hpp"

#pragma pack(1)
struct PMD_HEADER {
     PMD_HEADER(){
	  memset(header1,0xFD,sizeof(header1));
	  memset(header2,0xFD,sizeof(header2));
     }
     char header1[0x1b];
     char header2[0x100];
};
struct PMD_VERTEX_RECORD {
     float x;
     float y;
     float z;
     float nx;
     float ny;
     float nz;
     float tx;
     float ty;
     WORD  b1;
     WORD  b2;
     BYTE  bw;
     BYTE  unknown;
};
struct PMD_INDEX_RECORD {
     WORD  id;
};
struct PMD_MATERIAL_RECORD {
     struct { 
	  float r;
	  float g;
	  float b;
	  float a;
     } diffuse;
     float shininess;
     struct { 
	  float r;
	  float g;
	  float b;
     } specular, ambient;
     WORD p12;
     DWORD nEdges;
     char textureFileName[20]; 
};
struct PMD_BONE_RECORD {
     char  name[20];
     WORD  parent;
     WORD  to;
     BYTE  kind;
     WORD  knum;
     float pos[3];
};
struct PMD_IK_RECORD {
     void Read(std::istream &is) {
	  is.read( (char*)&parent,sizeof(parent) );
	  is.read( (char*)&to,sizeof(to) );
	  is.read( (char*)&num_link,sizeof(num_link) );
	  is.read( (char*)&count,sizeof(count) );
	  is.read( (char*)&fact,sizeof(fact) );
	  link.resize(num_link);
	  is.read( (char*)&link[0],num_link*sizeof(WORD) );
     };
     void Write(std::ostream &os) {
	  os.write( (char*)&parent,sizeof(parent) );
	  os.write( (char*)&to,sizeof(to) );
	  os.write( (char*)&num_link,sizeof(num_link) );
	  os.write( (char*)&count,sizeof(count) );
	  os.write( (char*)&fact,sizeof(fact) );
	  os.write( (char*)&link[0],num_link*sizeof(WORD) );
     };
     WORD  parent;
     WORD  to;
     BYTE  num_link;
     WORD  count;
     float fact;
     std::vector<WORD>link;
};
struct PMD_MORP_VERTEX_RECORD {
     int no;
     float vec[3];
};

struct PMD_MORP_RECORD {
     void Read(std::istream &is) {
	  is.read( name,sizeof(name) );
	  is.read( (char*)&vnum,sizeof(vnum) );
	  is.read( (char*)&grp,sizeof(grp) );
	  mv.resize(vnum);
	  is.read( (char*)&mv[0],vnum*sizeof(PMD_MORP_VERTEX_RECORD) );
     }
     void Write(std::ostream &os) {
	  os.write( name,sizeof(name) );
	  os.write( (char*)&vnum,sizeof(vnum) );
	  os.write( (char*)&grp,sizeof(grp) );
	  os.write( (char*)&mv[0],vnum*sizeof(PMD_MORP_VERTEX_RECORD) ); 
     }
     char  name[20];
     DWORD vnum;
     BYTE grp;
     std::vector<PMD_MORP_VERTEX_RECORD>mv;
};
	
struct PMD_GRP_NAME_RECORD {
     char name[50];
};

struct PMD_GRP_RECORD {
     WORD bone_no;
     BYTE grp;
};

/*
  struct PMD_MORP_RECORD {
  void Read(std::istream &is) {
  is.read( (char*)&nnum,sizeof(nnum) );
  mt.resize( nnum );
  for (int i=0;i<nnum;i++){
  mt[i].Read( is );
  }
  };
  void Write(std::ostream &os) {
  os.write( (char*)&nnum,sizeof(nnum) );
  for (int i=0;i<nnum;i++){
  mt[i].Write( os );
  }
  };
  WORD nnum;
  std::vector<PMD_MORP_TARGET_RECORD>mt;
  };
*/

#pragma pack(8)

typedef std::vector<PMD_VERTEX_RECORD>   PMD_VERTEX_CHUNK;
typedef std::vector<WORD>                PMD_INDEX_CHUNK;
typedef std::vector<PMD_MATERIAL_RECORD> PMD_MATERIAL_CHUNK;
typedef std::vector<PMD_BONE_RECORD>     PMD_BONE_CHUNK;
typedef std::vector<PMD_IK_RECORD>	 PMD_IK_CHUNK;
typedef std::vector<PMD_MORP_RECORD>     PMD_MORP_CHUNK;
typedef std::vector<WORD>		 PMD_CTRL_CHUNK;
typedef std::vector<PMD_GRP_NAME_RECORD> PMD_GRP_NAME_CHUNK;
typedef std::vector<PMD_GRP_RECORD>      PMD_GRP_CHUNK;

#define SIZEOF_WORD           2
#define SIZEOF_FLOAT          4
#define SIZEOF_DWORD          4
#define SIZEOF_VERTEX_RECORD 38

union IntFloat {
     uint32_t i;
     float f;
};

class clsPMDFile
{
public:

     BOOL Open(const char* name);
     BOOL Commit(const char* name);

     int  GetVersion();
     void SetVersion(int ver);

     const char* GetHeaderString1();
     const char* GetHeaderString2();
     void	 SetHeaderString(const char* name);

     const char* GetActor();
     void	 SetActor(const char* name );

     DWORD		GetVertexChunkSize();
     void	        SetVertexChunkSize(DWORD size);
     PMD_VERTEX_CHUNK&  GetVertexChunk();

     DWORD		GetIndexChunkSize();
     void	        SetIndexChunkSize(DWORD size);
     PMD_INDEX_CHUNK&   GetIndexChunk();

     int		GetBoneChunkSize();
     void	SetBoneChunkSize(int size);
     PMD_BONE_CHUNK& GetBoneChunk();

     int		GetIKChunkSize();
     void	SetIKChunkSize(int size);
     PMD_IK_CHUNK& GetIKChunk();

     int		GetMaterialChunkSize();
     void	SetMaterialChunkSize(int size);
     PMD_MATERIAL_CHUNK& GetMaterialChunk();

     int		GetMorpChunkSize();
     void	SetMorpChunkSize(int size);
     PMD_MORP_CHUNK& GetMorpChunk();

     int		GetCtrlChunkSize();
     void	SetCtrlChunkSize(int size);
     PMD_CTRL_CHUNK& GetCtrlChunk();

     int		GetGrpNameChunkSize();
     void	SetGrpNameChunkSize(int size);
     PMD_GRP_NAME_CHUNK& GetGrpNameChunk();

     int		GetGrpChunkSize();
     void	SetGrpChunkSize(int size);
     PMD_GRP_CHUNK& GetGrpChunk();

public:
     PMD_HEADER			m_header;
     PMD_VERTEX_CHUNK		m_vertexs;
     PMD_INDEX_CHUNK		m_indexs;
     PMD_MATERIAL_CHUNK		m_materials;
     PMD_BONE_CHUNK		m_bones;
     PMD_IK_CHUNK		m_ikbones;
     PMD_MORP_CHUNK		m_morps;
     PMD_CTRL_CHUNK		m_ctrls;
     PMD_GRP_NAME_CHUNK         m_grp_name;
     PMD_GRP_CHUNK		m_grp;

private:
     
     // PMDファイルはDWORD部分にサイズ情報を持っている
     // バイナリ情報からDWORDの情報を取り出す
     static DWORD GetDWORDSizeFromBin(unsigned char vertexHex[SIZEOF_DWORD]);

     // バイナリから型つきの値へ変換
     unsigned char m_Float[SIZEOF_FLOAT];
     unsigned char m_Word[SIZEOF_WORD];

     void  AddFloatChunk(BYTE b, int index);
     float MakeFloatChunk();
     void  AddWordChunk(BYTE b, int index);
     WORD  MakeWordChunk();

     /**
      * PMD_VERTEX_CHUNK: 頂点座標取得用
      */

     void  MakeVertexChunk(std::vector<BYTE>::const_iterator& fst, std::vector<BYTE>::const_iterator& mid);

     /**
      * PMD_INDEX_CHUNK: 頂点番号取得用
      */
     void  MakeIndexChunk(std::vector<BYTE>::const_iterator& fst, std::vector<BYTE>::const_iterator& mid);

};

#endif /** CLSPMDFILE_HPP */
