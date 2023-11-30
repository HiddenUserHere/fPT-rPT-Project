#include "stdafx.h"
#include "X3DCubicBezierCurve.h"

namespace X3D
{

CubicBezierCurve::CubicBezierCurve() {
}

CubicBezierCurve::CubicBezierCurve(const Vector3 & endPoint1, const Vector3 & endPoint2, const Vector3 & controlPoint1, const Vector3 & controlPoint2): endPoint1(endPoint1), endPoint2(endPoint2), controlPoint1(controlPoint1), controlPoint2(controlPoint2) {
}

Vector3 CubicBezierCurve::computeBezierPoint(float t) const {
	const float u = 1.f - t;
	const float tt = t * t;
	const float uu = u * u;
	const float uuu = uu * u;
	const float ttt = tt * t;

	Vector3 point = endPoint1 * uuu;

	point += controlPoint1 * 3 * uu * t;
	point += controlPoint2 * 3 * u * tt;
	point += endPoint2 * ttt;

	return point;
}

}