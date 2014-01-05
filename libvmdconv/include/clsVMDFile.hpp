#ifndef CLSVMDFILE_HPP
#define CLSVMDFILE_HPP

#pragma pack(1)

class VMD_HEADER {
public:
     VMD_HEADER();
     void Export( FIELDS&rec ) const;
     void Import( FIELDS&rec );

     char hdr_string[0x1A];
     BYTE unknown[0x04];
};
class VMD_V3_HEADER {
public:
     VMD_V3_HEADER();
     void Export( FIELDS&rec ) const;
     void Import( FIELDS&rec );

     char actor[20];
};
class VMD_V2_HEADER {
public:
     VMD_V2_HEADER();
     void Export( FIELDS&rec ) const;
     void Import( FIELDS&rec );

     char actor[10];
};

class VMD_MOTION_RECORD {
public:
     VMD_MOTION_RECORD();
     void Export( FIELDS&rec ) const;
     void Import( FIELDS&rec );

     char name[15];
     int   frame_no;
     float pos[3];
     float qt[4];
     BYTE pad[0x40];
};
class VMD_MORP_RECORD {
public:
     VMD_MORP_RECORD();
     void Export( FIELDS&rec )  const;
     void Import( FIELDS&rec );

     char name[15];
     int   frame_no;
     float factor;
};
class VMD_CAMERA_RECORD{
public:
     VMD_CAMERA_RECORD();
     void Export( FIELDS&rec )  const;
     void Import( FIELDS&rec );
     int   frame_no;
     float pos[4];
     float angle[3];
     BYTE pad[29];
};
class VMD_LIGHT_RECORD{
public:
     VMD_LIGHT_RECORD();
     void Export( FIELDS&rec )  const;
     void Import( FIELDS&rec );
     int   frame_no;
     float pos[3];
     float direction[3];
};

#pragma pack(8)

class clsVMDFile
{
     friend class VMD_HEADER;
     friend class VMD_V2_HEADER;
     friend class VMD_V3_HEADER;
     friend class VMD_MOTION_RECORD;
     friend class VMD_MORP_RECORD;
     friend class VMD_CAMERA_RECORD;
     friend class VMD_LIGHT_RECORD;
public:
     clsVMDFile(void);
     ~clsVMDFile(void);

     BOOL Open(const char* name);
     BOOL Commit(const char* name);

     const char*	GetVersion() const;
     void	SetVersion(const char* ver);

     VMD_HEADER&	GetHeader();
     const VMD_HEADER&	GetHeader() const ;
     void	SetHeader(const VMD_HEADER& header);

     const char*	GetActor() const ;
     void	SetActor(const char* name );

     std::vector<VMD_MOTION_RECORD>::size_type		GetMotionChunkSize();
     void	SetMotionChunkSize(std::vector<VMD_MOTION_RECORD>::size_type size);
     std::vector<VMD_MOTION_RECORD>& GetMotionChunk();

     std::vector<VMD_MORP_RECORD>::size_type		GetMorpChunkSize();
     void	SetMorpChunkSize(std::vector<VMD_MORP_RECORD>::size_type size);
     std::vector<VMD_MORP_RECORD>& GetMorpChunk();

     std::vector<VMD_CAMERA_RECORD>::size_type		GetCameraChunkSize();
     void	SetCameraChunkSize(std::vector<VMD_CAMERA_RECORD>::size_type size);
     std::vector<VMD_CAMERA_RECORD>& GetCameraChunk();

     std::vector<VMD_LIGHT_RECORD>::size_type		GetLightChunkSize();
     void	SetLightChunkSize(std::vector<VMD_LIGHT_RECORD>::size_type size);
     std::vector<VMD_LIGHT_RECORD>& GetLightChunk();

     static bool SortByBoneNameAndFrameNo(VMD_MOTION_RECORD rec1,VMD_MOTION_RECORD rec2 );
     static bool SortByMorpNameAndFrameNo(VMD_MORP_RECORD rec1,VMD_MORP_RECORD rec2 );
     static bool SortByCameraFrameNo(VMD_CAMERA_RECORD rec1,VMD_CAMERA_RECORD rec2 );
     static bool SortByLightFrameNo(VMD_LIGHT_RECORD rec1,VMD_LIGHT_RECORD rec2 );

private:
     static const char c_hdr_string[0x1A];
     static const char c_actor_v2[10];
     static const char c_actor_v3[20];
     static const BYTE c_hokan_data[0x40];
     static const BYTE c_hokan_data2[29];

     VMD_HEADER m_vmd_header;
     VMD_V2_HEADER m_vmd_v2_header;
     VMD_V3_HEADER m_vmd_v3_header;

     std::vector<VMD_MOTION_RECORD> m_motion_chunk;
     std::vector<VMD_MORP_RECORD> m_morp_chunk;
     std::vector<VMD_CAMERA_RECORD> m_camera_chunk;
     std::vector<VMD_LIGHT_RECORD> m_light_chunk;

};

typedef std::vector<VMD_MOTION_RECORD> VMD_MOTION_CHUNK;
typedef std::vector<VMD_MORP_RECORD> VMD_MORP_CHUNK;
typedef std::vector<VMD_CAMERA_RECORD> VMD_CAMERA_CHUNK;
typedef std::vector<VMD_LIGHT_RECORD> VMD_LIGHT_CHUNK;

#endif /** CLSVMDFILE_HPP */
