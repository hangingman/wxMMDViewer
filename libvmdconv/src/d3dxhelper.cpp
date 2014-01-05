
#pragma comment( lib , "d3dx9.lib" )

#include "d3dx9.hpp"
#include "d3dxhelper.hpp"

namespace D3DX_HELPER {



void SetAngle( float q[4] ,float x,float y,float z )
{
	D3DXQuaternionRotationYawPitchRoll((D3DXQUATERNION*)q, y,x,z );
}
/*
void GetAngle( const float q[4],float &x,float &y,float&z )
{
	D3DXMATRIX mt;

	D3DXMatrixRotationQuaternion( &mt,(D3DXQUATERNION*)q );
	x = asin( -mt._32 );
	float rcx = cos( x );
	y = atan2( mt._31 / rcx,mt._33 / rcx );
	z = atan2( mt._12 / rcx,mt._22 / rcx );
	// x = Å}2/ÉŒÇÃÇ∆Ç´y,zÇÕÇ«Ç§Ç»ÇÈÇÃ???
}
void mat2ypr(float &yaw, float &pitch, float &roll, const D3DXMATRIX &m)
{
  roll = atan2f(m._12, m._22);
  pitch = asinf(-m._32);
  yaw = atan2f(m._31, m._33);
    
  if (fabsf(cosf(pitch)) < 1.0e-6f) {
    roll += m._12 > 0.0f ? D3DX_PI : -D3DX_PI;
    yaw += m._31 > 0.0f ? D3DX_PI : -D3DX_PI;
  }
}
*/
void GetAngle( const float q[4],float &pitch,float &yaw,float&roll )
{
	D3DXMATRIX m;

	D3DXMatrixRotationQuaternion( &m,(D3DXQUATERNION*)q );
	roll = atan2f(m._12, m._22);
	pitch = asinf(-m._32);
	yaw = atan2f(m._31, m._33);
    
	if (fabsf(cosf(pitch)) < 1.0e-6f) {
	  roll += m._12 > 0.0f ? D3DX_PI : -D3DX_PI;
	  yaw += m._31 > 0.0f ? D3DX_PI : -D3DX_PI;
	}
}
void GetAxisAngle( const float q[4],float vec[3],float&theta ){
	D3DXQuaternionToAxisAngle( (D3DXQUATERNION*)q,(D3DXVECTOR3*)vec,&theta);
	D3DXVec3Normalize( (D3DXVECTOR3*)vec,(D3DXVECTOR3*)vec );
}

}