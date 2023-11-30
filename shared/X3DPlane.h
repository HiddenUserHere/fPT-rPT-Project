#pragma once

#include "X3DVector3.h"
#include "X3DTriangle.h"
#include "X3DAABB.h"
#include "X3DSphere.h"
#include "X3DRay.h"
#include "X3DLine.h"

namespace X3D
{

class Plane {
public:
	enum Side: unsigned int {
		FRONT,
		BACK,
		INTERSECT,
	};

public:
	Plane();
	Plane(const float A, const float B, const float C, const float D);
	Plane(const Vector3 &normal, const float d);
	Plane(const Triangle &triangle);
	Plane(const Plane &c);

	Vector3 computeNormal() const;

	float distance(const Vector3 &point) const;

	Side side(const Vector3 &point) const;
	Side side(const AABB &aabb) const;
	Side side(const Sphere &sphere) const;

	bool intersects(const Ray &ray, Vector3 &point) const;
	bool intersects(const Line &line, Vector3 &point) const;

public:
	float A;
	float B;
	float C;
	float D;
};

}