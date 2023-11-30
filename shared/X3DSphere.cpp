#include "stdafx.h"
#include "X3DSphere.h"

#include "X3DLine.h"

namespace X3D
{

Sphere::Sphere(): radius(0.0f) {
}

Sphere::Sphere(const Vector3 &center, const float radius): center(center), radius(radius) {
}

Sphere::Sphere(const Sphere &c): center(c.center), radius(c.radius) {
}

Sphere& Sphere::reset() {
	center = Vector3(0.0f, 0.0f, 0.0f);
	radius = 0.0f;

	return *this;
}

Sphere& Sphere::mergeWith(const Sphere &other) {
	Line line(center, other.center);

	center = line.computeCenter();

	float biggestRadius = radius > other.radius ? radius : other.radius;
	float distanceToCenter = (other.center - center).length();

	radius = distanceToCenter + biggestRadius;

	return *this;
}

bool Sphere::intersects(const Ray &ray) const {
	Vector3 oc = ray.location - center;

	//Solve with ABC formula
	float A = ray.direction.dot(ray.direction);
	float B = ray.direction.dot(oc * 2.0f);
	float C = (oc).dot(oc) - (radius * radius);

	//Discriminant
	float D = (B * B) - (4.0f * A * C);

	//No intersection if D is below 0
	if (D < 0.0f) {
		return false;
	}

	float Dsqrt = sqrt(D);

	float t0 = (-B + Dsqrt) / (2 * A); //First Intersection
	float t1 = (-B - Dsqrt) / (2 * A); //Second Intersection

	if (t1 < t0) {
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	//Intersection Behind Ray Start Point?
	if (t1 < 0.0f) {
		return false;
	}

	//If Ray starts inside object, then t0 is below zero, and thus first intersection point is t1
	if (t0 < 0.0f) {
		//Intersection on t1
		return true;
	} else {
		//Intersection on t0
		return true;
	}
}

bool Sphere::intersects(const Sphere &sphere) const {
	float distance = (sphere.center - center).length();
	float touchDistance = sphere.radius + radius;

	return (distance <= touchDistance);
}

}