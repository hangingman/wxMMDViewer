#ifndef D3DXHELPER_HPP
#define D3DXHELPER_HPP

namespace D3DX_HELPER {

extern void SetAngle( float q[4] ,float x,float y,float z );
extern void GetAngle( const float q[4],float &x,float &y,float&z );
extern void GetAxisAngle( const float q[4],float vec[3],float&theta );

}

#endif /** D3DXHELPER_HPP */
