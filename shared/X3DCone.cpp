#include "stdafx.h"
#include "X3DCone.h"

#include "X3DLine.h"

namespace X3D
{

Cone::Cone() {
}

Cone::Cone(const Sphere &begin, const Sphere &end): begin(begin), end(end) {
}

Cone::Cone(const Cone &c): begin(c.begin), end(c.end) {
}

bool Cone::contains(const Vector3 &point) const {
	const Line baseLine(end.center, begin.center);
	const float t = baseLine.projectPointOnLine(point);

	if (t < 0.f)
		return false;

	if (t > 1.f)
		return false;

	const Vector3 pointOnLine = baseLine.getPointOnLine(t);
	const float distanceSq = (point - pointOnLine).lengthSquared();

	float fRadiusAtPointOnLine = lerpf( begin.radius, end.radius, t );
	fRadiusAtPointOnLine *= fRadiusAtPointOnLine;

	if( distanceSq < fRadiusAtPointOnLine )
		return true;

	return false;
}

}