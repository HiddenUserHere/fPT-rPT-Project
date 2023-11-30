#pragma once

#include "X3DSphere.h"
#include "X3DVector3.h"

namespace X3D
{

class Cone {
public:
	Cone();
	Cone(const Sphere &begin, const Sphere &end);
	Cone(const Cone &c);

	bool contains(const Vector3 &point) const;

public:
	Sphere begin;
	Sphere end;
};

}