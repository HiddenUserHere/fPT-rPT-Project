#include "stdafx.h"
#include "X3DAABB.h"

namespace X3D
{

AABB::AABB() {
}

AABB::AABB(const Vector3 &vector): min(vector), max(vector) {
}

AABB::AABB(const Vector3 &min, const Vector3 &max): min(min), max(max) {
}

AABB::AABB(const AABB &c): min(c.min), max(c.max) {
}

AABB& AABB::reset() {
	min = Vector3(0.0f, 0.0f, 0.0f);
	max = Vector3(0.0f, 0.0f, 0.0f);

	return *this;
}

AABB& AABB::recompute() {
	Vector3 newMin, newMax;

	if( min.x < max.x )
	{
		newMin.x = min.x;
		newMax.x = max.x;
	}
	else
	{
		newMin.x = max.x;
		newMax.x = min.x;
	}

	if( min.y < max.y )
	{
		newMin.y = min.y;
		newMax.y = max.y;
	}
	else
	{
		newMin.y = max.y;
		newMax.y = min.y;
	}

	if( min.z < max.z )
	{
		newMin.z = min.z;
		newMax.z = max.z;
	}
	else
	{
		newMin.z = max.z;
		newMax.z = min.z;
	}

	min = newMin;
	max = newMax;

	return *this;
}

AABB& AABB::mergeWith(const AABB &other) {
	if (other.min.x < min.x) {
		min.x = other.min.x;
	}
	if (other.max.x > max.x) {
		max.x = other.max.x;
	}

	if (other.min.y < min.y) {
		min.y = other.min.y;
	}
	if (other.max.y > max.y) {
		max.y = other.max.y;
	}

	if (other.min.z < min.z) {
		min.z = other.min.z;
	}
	if (other.max.z > max.z) {
		max.z = other.max.z;
	}

	return *this;
}

AABB& AABB::mergeWith(const Vector3 &vector) {
	if (vector.x < min.x) {
		min.x = vector.x;
	}
	if (vector.x > max.x) {
		max.x = vector.x;
	}
	
	if (vector.y < min.y) {
		min.y = vector.y;
	}
	if (vector.y > max.y) {
		max.y = vector.y;
	}

	if (vector.z < min.z) {
		min.z = vector.z;
	}
	if (vector.z > max.z) {
		max.z = vector.z;
	}

	return *this;
}

Vector3* AABB::computeCorners(Vector3 *array) const {
	array[0] = Vector3(min.x, max.y, max.z); //Back top left
	array[1] = Vector3(max.x, max.y, max.z); //Back top right
	array[2] = Vector3(max.x, min.y, max.z); //Back bottom right
	array[3] = Vector3(min.x, min.y, max.z); //Back bottom left
	array[4] = Vector3(min.x, max.y, min.z); //Front top left
	array[5] = Vector3(max.x, max.y, min.z); //Front top right
	array[6] = Vector3(max.x, min.y, min.z); //Front bottom right
	array[7] = Vector3(min.x, min.y, min.z); //Front bottom left

	return array;
}

Vector3 AABB::computeCenter() const {
	return (max + min) / 2.0f;
}

AABB* AABB::octSplit(AABB *array) const {
	auto center = computeCenter();

	Vector3 corners[8];
	computeCorners(corners);

	array[0] = AABB(corners[0], center).recompute();
	array[1] = AABB(corners[1], center).recompute();
	array[2] = AABB(corners[2], center).recompute();
	array[3] = AABB(corners[3], center).recompute();
	array[4] = AABB(corners[4], center).recompute();
	array[5] = AABB(corners[5], center).recompute();
	array[6] = AABB(corners[6], center).recompute();
	array[7] = AABB(corners[7], center).recompute();

	return array;
}

bool AABB::intersects(const AABB &aabb) const {
	if (max.x < aabb.min.x ||
		max.y < aabb.min.y ||
		max.z < aabb.min.z ||
		min.x > aabb.max.x ||
		min.y > aabb.max.y ||
		min.z > aabb.max.z ) {
		return false;
	}

	return true;
}

bool AABB::contains(const Vector3 &point) const {
	if ((point.x >= min.x) && (point.x <= max.x)) {
		if ((point.y >= min.y) && (point.y <= max.y)) {
			if ((point.z >= min.z) && (point.z <= max.z)) {
				return true;
			}
		}
	}

	return false;
}

}