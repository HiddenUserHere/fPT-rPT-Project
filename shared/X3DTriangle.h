#pragma once

#include "X3DVector3.h"

namespace X3D
{

class Plane;
class Line;

class Triangle {
public:
	Triangle();
	Triangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);
	Triangle(const Triangle &c);

	Vector3 computeCenter() const;
	Vector3 computeNormal() const;

	bool intersects(const Plane &plane, Line *intersectionLine = 0) const;

	bool isPointInTriangle(const Vector3 &point) const;

private:
	bool sameSide(const Vector3 &p1, const Vector3 &p2, const Vector3 &a, const Vector3 &b) const;

public:
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
};

}