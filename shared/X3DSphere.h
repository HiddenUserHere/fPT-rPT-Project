#pragma once

#include "X3DVector3.h"
#include "X3DRay.h"

namespace X3D
{

class Sphere {
public:
	Sphere();
	Sphere(const Vector3 &center, const float radius);
	Sphere(const Sphere &c);

	Sphere& reset();
	Sphere& mergeWith(const Sphere &other);

	bool intersects(const Ray &ray) const;
	bool intersects(const Sphere &sphere) const;

public:
	Vector3 center;
	float radius;
};

}