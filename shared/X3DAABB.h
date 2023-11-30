#pragma once

#include "X3DVector3.h"

namespace X3D
{

class AABB {
public:
	AABB();
	AABB(const Vector3 &vector);
	AABB(const Vector3 &min, const Vector3 &max);
	AABB(const AABB &c);

	AABB& reset();
	AABB& recompute();
	AABB& mergeWith(const AABB &other);
	AABB& mergeWith(const Vector3 &vector);

	Vector3* computeCorners(Vector3 *array) const;
	Vector3 computeCenter() const;
	AABB* octSplit(AABB *array) const;

	bool intersects(const AABB &aabb) const;

	bool contains(const Vector3 &point) const;

public:
	Vector3 min;
	Vector3 max;
};

}