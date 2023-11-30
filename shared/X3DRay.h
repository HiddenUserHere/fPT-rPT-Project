#pragma once

#include "X3DVector3.h"

namespace X3D
{

class Ray {
public:
	Ray();
	Ray(const Vector3 &location, const Vector3 &direction);
	Ray(const Ray &c);

public:
	Vector3 location;
	Vector3 direction;
};

}