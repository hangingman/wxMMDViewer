#include "d3dxhelper.hpp"

/**
 * DirectX用の補助クラスだが、将来的にOpenGLで描画を行いたいため削除する予定
 */
namespace D3DX_HELPER {

     void SetAngle( float q[4] ,float x,float y,float z )
     {
	  // D3DXQuaternionRotationYawPitchRoll((D3DXQUATERNION*)q, y,x,z );
     }

     void GetAngle( const float q[4],float &pitch,float &yaw,float&roll )
     {
	  // D3DXMATRIX m;

	  // D3DXMatrixRotationQuaternion( &m,(D3DXQUATERNION*)q );
	  // roll = atan2f(m._12, m._22);
	  // pitch = asinf(-m._32);
	  // yaw = atan2f(m._31, m._33);
    
	  // if (fabsf(cosf(pitch)) < 1.0e-6f) {
	  //      roll += m._12 > 0.0f ? D3DX_PI : -D3DX_PI;
	  //      yaw += m._31 > 0.0f ? D3DX_PI : -D3DX_PI;
	  // }
     }
     void GetAxisAngle( const float q[4],float vec[3],float&theta )
     {
	  // D3DXQuaternionToAxisAngle( (D3DXQUATERNION*)q,(D3DXVECTOR3*)vec,&theta);
	  // D3DXVec3Normalize( (D3DXVECTOR3*)vec,(D3DXVECTOR3*)vec );
     }
}
