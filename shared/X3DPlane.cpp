#include "stdafx.h"
#include "X3DPlane.h"

namespace X3D
{

Plane::Plane(): A(1.0f), B(1.0f), C(1.0f), D(0.0f) {
}

Plane::Plane(const float A, const float B, const float C, const float D): A(A), B(B), C(C), D(D) {
}

Plane::Plane(const Vector3 &normal, const float distance): A(normal.x), B(normal.y), C(normal.z), D(distance) {
}

Plane::Plane(const Triangle &triangle) {
	const auto normal = triangle.computeNormal();

	A = normal.x;
	B = normal.y;
	C = normal.z;
	D = -normal.dot(triangle.p1);
}

Plane::Plane(const Plane &c): A(c.A), B(c.B), C(c.C), D(c.D) {
}

Vector3 Plane::computeNormal() const {
	return Vector3(A, B, C);
}

float Plane::distance(const Vector3 &point) const {
	return A*point.x + B*point.y + C*point.z + D;
}

Plane::Side Plane::side(const Vector3 &point) const {
	const float d = distance(point);

	if ((d > -EPSILON) && (d < EPSILON)) {
		return INTERSECT;
	}

	return d > 0.0f ? FRONT : BACK;
}

Plane::Side Plane::side(const AABB &aabb) const {
	Vector3 corners[8];
	aabb.computeCorners(corners);

	const float d1 = distance(corners[0]);

	if (fabs(d1) < EPSILON) {
		return INTERSECT;
	}

	const bool positiveSide = d1 > 0.0f;

	for (int i = 1; i < 8; i++) {
		const float dx = distance(corners[i]);

		if (fabs(dx) < EPSILON) {
			return INTERSECT;
		}

		if (positiveSide) {
			if (dx < 0.0f) {
				return INTERSECT;
			}
		} else {
			if (dx > 0.0f) {
				return INTERSECT;
			}
		}
	}

	return positiveSide ? FRONT : BACK;
}

Plane::Side Plane::side(const Sphere &sphere) const {
	const float d = distance(sphere.center);

	if (fabs(d) <= sphere.radius) {
		return INTERSECT;
	}

	return (d > 0.0f ? FRONT : BACK);
}

bool Plane::intersects(const Ray &ray, Vector3 &point) const {
	const Line line(ray.location, ray.location + ray.direction);

	const float denominator = (A * (line.begin.x - line.end.x)) + (B * (line.begin.y - line.end.y)) + (C * (line.begin.z - line.end.z));

	//Parallel to Plane?
	if (denominator == 0.0f) {
		//No intersection
		return false;
	}

	const float numerator = (A * line.begin.x) + (B * line.begin.y) + (C * line.begin.z) + D;
	const float t = numerator / denominator;

	//Behind Ray?
	if (t < (-EPSILON)) {
		//No intersection
		return false;
	}

	//Get Point on Line using the P = P1 + t(P2 - P1) formula
	point = line.getPointOnLine(t);

	//Intersection!
	return true;
}

bool Plane::intersects(const Line &line, Vector3 &point) const {
	const float denominator = (A * (line.begin.x - line.end.x)) + (B * (line.begin.y - line.end.y)) + (C * (line.begin.z - line.end.z));

	//Parallel to Plane?
	if (denominator == 0.0f) {
		//No intersection
		return false;
	}

	const float numerator = (A * line.begin.x) + (B * line.begin.y) + (C * line.begin.z) + D;
	const float t = numerator / denominator;

	//Behind Line?
	if (t < (-EPSILON)) {
		//No intersection
		return false;
	}

	//Further than Line?
	if (t > 1.0f) {
		//No intersection
		return false;
	}

	//Get Point on Line using the P = P1 + t(P2 - P1) formula
	point = line.getPointOnLine(t);

	//Intersection!
	return true;
}

}