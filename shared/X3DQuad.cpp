#include "stdafx.h"
#include "X3DQuad.h"

namespace X3D
{

Quad::Quad() {
}

Quad::Quad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4): p1(p1), p2(p2), p3(p3), p4(p4) {
}

Quad::Quad(const Vector3 p[4]): p1(p[0]), p2(p[1]), p3(p[2]), p4(p[3]) {
}

Quad::Quad(const Quad &c): p1(c.p1), p2(c.p2), p3(c.p3), p4(c.p4) {
}

Vector3 Quad::computeCenter() const {
	Vector3 result(p1.x + p2.x + p3.x + p4.x, p1.y + p2.y + p3.y + p4.y, p1.z + p2.z + p3.z + p4.z);

	result /= 4.0f;

	return result;
}

}