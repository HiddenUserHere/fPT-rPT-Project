#pragma once

#include "X3DVector2.h"
#include "X3DVector3.h"
#include "X3DMatrix4.h"

namespace DX
{

D3DXVECTOR3 X3DVector3ToD3DXVector3( const X3D::Vector3 & v );
D3DXMATRIX X3DMatrixToD3DXMatrix( const X3D::Matrix4 & m );

X3D::Matrix4 D3DXMatrixToX3DMatrix( const D3DXMATRIX & m );
X3D::Vector3 D3DXVector3ToX3DVector3( const D3DXVECTOR3 & v );

}