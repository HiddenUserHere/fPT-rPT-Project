#include "stdafx.h"
#include "X3DLine.h"

namespace X3D
{

Line::Line() {
}

Line::Line(const Vector3 &begin, const Vector3 &end): begin(begin), end(end) {
}

Line::Line(const Line &c): begin(c.begin), end(c.end) {
}

float Line::length() const {
	return (end - begin).length();
}

float Line::lengthSquared() const {
	return (end - begin).lengthSquared();
}

Vector3 Line::computeCenter() {
	Vector3 result(begin.x + end.x, begin.y + end.y, begin.z + end.z);

	result /= 2.0f;

	return result;
}

bool Line::overlaps(const Line &line) const {
	if (overlapProjected(begin.x, end.x, line.begin.x, line.end.x)) {
		if (overlapProjected(begin.y, end.y, line.begin.y, line.end.y)) {
			if (overlapProjected(begin.z, end.z, line.begin.z, line.end.z)) {
				return true;
			}
		}
	}

	return false;
}

Vector3 Line::getPointOnLine(const float t) const {
	return begin + ((end - begin) * t);
}

float Line::projectPointOnLine(const Vector3 &point) const {
	const float ebLength = length();
	return (end - begin).dot(point - begin) / ebLength / ebLength;
}

float Line::distanceToPoint(const Vector3 &point) const {
	const float ebLength = length();
	const float t = (end - begin).dot(point - begin) / ebLength;

	if( t < 0.f )
		return (begin - point).length();

	if( t > ebLength )
		return (end - point).length();

	return (getPointOnLine(t) - point).length();
}

bool Line::overlapProjected(float a1, float a2, float b1, float b2) const {
	float min1, max1;

	if (a1 < a2) {
		min1 = a1;
		max1 = a2;
	} else {
		min1 = a2;
		max1 = a1;
	}

	float min2, max2;

	if (b1 < b2) {
		min2 = b1;
		max2 = b2;
	} else {
		min2 = b2;
		max2 = b1;
	}

	return (((min1 >= min2) && (min1 <= max2)) || ((max1 >= min2) && (max1 <= max2)));
}

}