#pragma once

#include "X3DVector3.h"
#include "X3DMatrix4.h"

namespace DX
{

typedef std::shared_ptr<class Camera> Camera_ptr;

class Camera
{
public:
	void				  SetEye( const X3D::Vector3 & v ) { vEye = v; }
	const X3D::Vector3	& GetEye() const { return vEye; }

	void				  SetLookAt( const X3D::Vector3 & v ) { vLookAt = v; }
	const X3D::Vector3	& GetLookAt() const { return vLookAt; }

	void				  SetUp( const X3D::Vector3 & v ) { vUp = v; }
	const X3D::Vector3	& GetUp() const { return vUp; }

	X3D::Vector3		  GetDirection() const;

private:
	X3D::Vector3		  vEye;
	X3D::Vector3		  vLookAt;
	X3D::Vector3		  vUp;
};

}