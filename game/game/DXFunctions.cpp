#include "stdafx.h"
#include "DXFunctions.h"

namespace DX
{

D3DXVECTOR3 X3DVector3ToD3DXVector3( const X3D::Vector3 & v )
{
	D3DXVECTOR3 nv;
	nv.x = v.x;
	nv.y = v.y;
	nv.z = v.z;
	return nv;
}

D3DXMATRIX X3DMatrixToD3DXMatrix( const X3D::Matrix4 & m )
{
	D3DXMATRIX nm;
	memcpy( nm.m, m.m, sizeof( nm.m ) );
	return nm;
}

X3D::Matrix4 D3DXMatrixToX3DMatrix( const D3DXMATRIX & m )
{
	X3D::Matrix4 nm;
	memcpy( nm.m, m.m, sizeof( nm.m ) );
	return nm;
}

X3D::Vector3 D3DXVector3ToX3DVector3( const D3DXVECTOR3 & v )
{
	X3D::Vector3 nv;
	nv.x = v.x;
	nv.y = v.y;
	nv.z = v.z;
	return nv;
}

}