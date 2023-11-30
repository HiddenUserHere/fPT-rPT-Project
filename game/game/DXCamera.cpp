#include "stdafx.h"
#include "DXCamera.h"

namespace DX
{

X3D::Vector3 Camera::GetDirection() const
{
	X3D::Vector3 vResult = vLookAt;

	vResult.x -= vEye.x;
	vResult.y -= vEye.y;
	vResult.z -= vEye.z;

	return vResult.normalize();
}

}