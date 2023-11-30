#include "stdafx.h"
#include "X3DTriangle.h"

namespace X3D
{

Triangle::Triangle() {
}

Triangle::Triangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3): p1(p1), p2(p2), p3(p3) {
}

Triangle::Triangle(const Triangle &c): p1(c.p1), p2(c.p2), p3(c.p3) {
}

Vector3 Triangle::computeCenter() const {
	Vector3 result(p1.x + p2.x + p3.x, p1.y + p2.y + p3.y, p1.z + p2.z + p3.z);

	result /= 3.0f;

	return result;
}

Vector3 Triangle::computeNormal() const {
	//Compute CCW (Counter Clockwise)
	auto normal = (p2 - p1).cross(p3 - p1).normalize();

	//Convert to CW (Clockwise)
	return normal.inverse();
}

bool Triangle::intersects(const Plane &plane, Line *intersectionLine) const {
	float d1 = plane.distance(p1);
	float d2 = plane.distance(p2);
	float d3 = plane.distance(p3);

	//Determine on which side each point is..
	const Vector3 *negSide[3], *posSide[3];
	int negSideLen = 0, posSideLen = 0;

	if (d1 < 0.0f) {
		negSide[negSideLen] = &p1;
		negSideLen++;
	} else {
		posSide[posSideLen] = &p1;
		posSideLen++;
	}

	if (d2 < 0.0f) {
		negSide[negSideLen] = &p2;
		negSideLen++;
	} else {
		posSide[posSideLen] = &p2;
		posSideLen++;
	}

	if (d3 < 0.0f) {
		negSide[negSideLen] = &p3;
		negSideLen++;
	} else {
		posSide[posSideLen] = &p3;
		posSideLen++;
	}

	//All points on one side?
	if ((negSideLen == 3) || (posSideLen == 3)) {
		return false;
	}

	//No request for Intersection Line?
	if (!intersectionLine) {
		return true;
	}

	//Group the two points one side together
	const Vector3 *twoSide[2], *oneSide;

	if (negSideLen == 2) {
		twoSide[0] = negSide[0];
		twoSide[1] = negSide[1];

		oneSide = posSide[0];
	} else {
		twoSide[0] = posSide[0];
		twoSide[1] = posSide[1];

		oneSide = negSide[0];
	}

	Vector3 pi1, pi2;
	plane.intersects(Line(*twoSide[0], *oneSide), pi1);
	plane.intersects(Line(*twoSide[1], *oneSide), pi2);

	*intersectionLine = Line(pi1, pi2);

	return true;
}

bool Triangle::isPointInTriangle(const Vector3 &point) const {
	if (sameSide(point, p1, p2, p3)) {
		if (sameSide(point, p2, p1, p3)) {
			if (sameSide(point, p3, p1, p2)) {
				return true;
			}
		}
	}

	return false;
}

bool Triangle::sameSide(const Vector3 &p1, const Vector3 &p2, const Vector3 &a, const Vector3 &b) const {
	Vector3 cp1 = (b - a).cross(p1 - a);
	Vector3 cp2 = (b - a).cross(p2 - a);

	return (cp1.dot(cp2) >= (-EPSILON));
}

}