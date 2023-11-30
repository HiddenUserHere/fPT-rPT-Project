#pragma once

#include "X3DPlane.h"

#include "X3DVector3.h"
#include "X3DLine.h"
#include "X3DAABB.h"
#include "X3DSphere.h"

namespace X3D
{

class Frustum {
public:
	Frustum();
	Frustum(const Plane &near, const Plane &far, const Plane &left, const Plane &right, const Plane &top, const Plane &bottom);
	Frustum(const Frustum &c);

	void scale(float scaling);
	void perspective(float angle, float aspectRatio, float nearDistance, float farDistance);
	void lookAt(const Vector3 &eye, const Vector3 &lookAt, const Vector3 &up);

	void build();
	void reset();
	
	bool contains(const Vector3 &point) const;
	bool contains(const Line &line) const;
	bool contains(const Triangle &triangle) const;
	bool contains(const AABB &aabb) const;
	bool contains(const Sphere &sphere) const;
	bool contains(const Quad &quad) const;

private:
	Plane p[6];

	bool ready;

	float scaling = 1.0f;

	float nearDistance;
	float farDistance;

	float nearWidth;
	float nearHeight;
	float farWidth;
	float farHeight;

public:
	Vector3 ntl;
	Vector3 ntr;
	Vector3 nbl;
	Vector3 nbr;
	Vector3 ftl;
	Vector3 ftr;
	Vector3 fbl;
	Vector3 fbr;
};

}