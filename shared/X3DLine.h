#pragma once

#include "X3DVector3.h"

namespace X3D
{

class Line {
public:
	Line();
	Line(const Vector3 &begin, const Vector3 &end);
	Line(const Line &c);

	float length() const;
	float lengthSquared() const;

	Vector3 computeCenter();

	bool overlaps(const Line &line) const;

	Vector3 getPointOnLine(const float t) const;
	float projectPointOnLine(const Vector3 &point) const;
	float distanceToPoint(const Vector3 &point) const;

protected:
	bool overlapProjected(float a1, float a2, float b1, float b2) const;

public:
	Vector3 begin;
	Vector3 end;
};

}