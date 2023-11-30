#pragma once

#include "X3DVector3.h"

namespace X3D
{

class CubicBezierCurve
{
public:
	CubicBezierCurve();
	CubicBezierCurve(const Vector3 & endPoint1, const Vector3 & endPoint2, const Vector3 & controlPoint1, const Vector3 & controlPoint2);

	const Vector3& getEndPoint1() const { return endPoint1; }
	void setEndPoint1(const Vector3& v) { endPoint1 = v; }
	const Vector3& getEndPoint2() const { return endPoint2; }
	void setEndPoint2(const Vector3& v) { endPoint2 = v; }
	
	const Vector3& getControlPoint1() const { return controlPoint1; }
	void setControlPoint1(const Vector3& v) { controlPoint1 = v; }
	const Vector3& getControlPoint2() const { return controlPoint2; }
	void setControlPoint2(const Vector3& v) { controlPoint2 = v; }

	Vector3 computeBezierPoint(float t) const;

private:
	Vector3 endPoint1;
	Vector3 endPoint2;

	Vector3 controlPoint1;
	Vector3 controlPoint2;
};

}