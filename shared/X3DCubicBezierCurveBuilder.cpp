#include "stdafx.h"
#include "X3DCubicBezierCurveBuilder.h"

namespace X3D
{

CubicBezierCurve CubicBezierCurveBuilder::build() {
	buildControlPoints();

	CubicBezierCurve curve;
	curve.setEndPoint1(endPoint1);
	curve.setEndPoint2(endPoint2);
	curve.setControlPoint1(controlPoint1);
	curve.setControlPoint2(controlPoint2);
	return curve;
}

void CubicBezierCurveBuilder::buildControlPoints() {
	const float u = passThroughPoint1TimeOffset;
	const float v = passThroughPoint2TimeOffset;

	const float a = 3.f * (1.f - u) * (1.f - u) * u;
	const float b = 3.f * (1.f - u) * u * u;
	const float c = 3.f * (1.f - v) * (1.f - v) * v;
	const float d = 3.f * (1.f - v) * v * v;

	const float det = (a * d) - (b * c);

	if (det == 0.f) {
		return;
	}

	const Vector3& p0 = endPoint1;
	const Vector3& p1 = passThroughPoint1;
	const Vector3& p2 = passThroughPoint2;
	const Vector3& p3 = endPoint2;

	Vector3 q1;
	q1.x = p1.x - (((1.f - u) * (1.f - u) * (1.f - u) * p0.x) + (u*u*u*p3.x));
	q1.y = p1.y - (((1.f - u) * (1.f - u) * (1.f - u) * p0.y) + (u*u*u*p3.y));
	q1.z = p1.z - (((1.f - u) * (1.f - u) * (1.f - u) * p0.z) + (u*u*u*p3.z));
	
	Vector3 q2;
	q2.x = p2.x - (((1.f - v) * (1.f - v) * (1.f - v) * p0.x) + (v*v*v*p3.x));
	q2.y = p2.y - (((1.f - v) * (1.f - v) * (1.f - v) * p0.y) + (v*v*v*p3.y));
	q2.z = p2.z - (((1.f - v) * (1.f - v) * (1.f - v) * p0.z) + (v*v*v*p3.z));

	controlPoint1.x = ((d * q1.x) - (b * q2.x)) / det;
	controlPoint1.y = ((d * q1.y) - (b * q2.y)) / det;
	controlPoint1.z = ((d * q1.z) - (b * q2.z)) / det;

	controlPoint2.x = (((-c) * q1.x) + (a * q2.x)) / det;
	controlPoint2.y = (((-c) * q1.y) + (a * q2.y)) / det;
	controlPoint2.z = (((-c) * q1.z) + (a * q2.z)) / det;
}

}