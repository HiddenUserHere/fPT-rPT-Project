#pragma once

#include "X3DCubicBezierCurve.h"

namespace X3D
{

class CubicBezierCurveBuilder
{
public:
	void setEndPoint1(const Vector3& v) { endPoint1 = v; }
	void setEndPoint2(const Vector3& v) { endPoint2 = v; }

	void setPassThroughPoint1(const Vector3 & v, float t) { passThroughPoint1 = v; passThroughPoint1TimeOffset = t; }
	void setPassThroughPoint2(const Vector3 & v, float t) { passThroughPoint2 = v; passThroughPoint2TimeOffset = t; }

	CubicBezierCurve build();

private:
	void buildControlPoints();

private:
	Vector3 endPoint1;
	Vector3 endPoint2;

	Vector3 passThroughPoint1;
	float passThroughPoint1TimeOffset = 0.f;
	Vector3 passThroughPoint2;
	float passThroughPoint2TimeOffset = 1.f;

	Vector3 controlPoint1;
	Vector3 controlPoint2;
};

}