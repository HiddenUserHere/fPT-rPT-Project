#pragma once

#include "X3DVector3.h"

namespace X3D
{

class Quad {
public:
	Quad();
	Quad(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &p4);
	Quad(const Vector3 p[4]);
	Quad(const Quad &c);

	Vector3 computeCenter() const;

public:
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 p4;
};

}