#pragma once

#include "X3DVector3.h"

namespace X3D
{

class Circle {
public:
	Circle();
	Circle(const Vector3 &center, const float radius, const Vector3 &normal);
	Circle(const Circle &c);

public:
	Vector3 center;
	float radius = 0.f;
	Vector3 normal;
};

}