#include "stdafx.h"
#include "X3DFrustum.h"

namespace X3D
{

Frustum::Frustum() {
	reset();
}

Frustum::Frustum(const Plane &_near, const Plane &_far, const Plane &left, const Plane &right, const Plane &top, const Plane &bottom) {
	p[0] = _near;
	p[1] = _far;
	p[2] = left;
	p[3] = right;
	p[4] = top;
	p[5] = bottom;
}

Frustum::Frustum(const Frustum &c) {
	for (int i = 0; i < 6; i++) {
		p[i] = c.p[i];
	}
}

void Frustum::scale(float scaling) {
	this->scaling = scaling;
}

void Frustum::perspective(float angle, float aspectRatio, float nearDistance, float farDistance) {
	this->nearDistance = nearDistance;
	this->farDistance = farDistance;

	//Compute Width and Height of near and far planes (scaled)
	float tang = tanf(DEGTORAD(angle) * 0.5f);

	nearHeight = nearDistance * tang * scaling;
	nearWidth = nearHeight * aspectRatio;

	farHeight = farDistance * tang * scaling;
	farWidth = farHeight * aspectRatio;
}

void Frustum::lookAt(const Vector3 &eye, const Vector3 &lookAt, const Vector3 &up) {
	//Compute Z axis of Camera
	Vector3 Z = lookAt - eye;
	Z.normalize();

	//Compute X axis of Camera
	Vector3 X = up.cross(Z);
	X.normalize();

	//Compute Y axis of Camera
	Vector3 Y = Z.cross(X);

	//Compute centers of near and far plane
	Vector3 nc = eye + (Z * nearDistance);
	Vector3 fc = eye + (Z * farDistance);

	//Compute the 4 corners of the frustum on the near plane
	ntl = nc + Y * nearHeight - X * nearWidth;
	ntr = nc + Y * nearHeight + X * nearWidth;
	nbl = nc - Y * nearHeight - X * nearWidth;
	nbr = nc - Y * nearHeight + X * nearWidth;

	//Compute the 4 corners of the frustum on the far plane
	ftl = fc + Y * farHeight - X * farWidth;
	ftr = fc + Y * farHeight + X * farWidth;
	fbl = fc - Y * farHeight - X * farWidth;
	fbr = fc - Y * farHeight + X * farWidth;
}

void Frustum::build() {
	//Build Planes from Triangles facing inward (triangles are clockwise)
	p[0] = Plane(Triangle(ntl,ntr,nbr));
	p[1] = Plane(Triangle(ftr,ftl,fbl));
	p[2] = Plane(Triangle(ntl,nbl,fbl));
	p[3] = Plane(Triangle(nbr,ntr,fbr));
	p[4] = Plane(Triangle(ntr,ntl,ftl));
	p[5] = Plane(Triangle(nbl,nbr,fbr));

	ready = true;
}

void Frustum::reset() {
	ready = false;

	scaling = 1.0f;
}

bool Frustum::contains(const Vector3 &point) const {
	if (!ready) {
		return true;
	}

	for (int i = 0; i < 6; i++) {
		if (p[i].side(point) == Plane::BACK) {
			return false;
		}
	}

	return true;
}

bool Frustum::contains(const Line &line) const {
	if (!ready) {
		return true;
	}

	for (int i = 0; i < 6; i++) {
		const auto s1 = p[i].side(line.begin);
		const auto s2 = p[i].side(line.end);

		if ((s1 == Plane::BACK) && (s2 == Plane::BACK)) {
			return false;
		}
	}

	return true;
}

bool Frustum::contains(const Triangle &triangle) const {
	if (!ready) {
		return true;
	}

	if( contains( X3D::Line( triangle.p1, triangle.p2 ) ) )
		return true;

	if( contains( X3D::Line( triangle.p2, triangle.p3 ) ) )
		return true;

	if( contains( X3D::Line( triangle.p3, triangle.p1 ) ) )
		return true;

	if( contains( X3D::Line( triangle.p1, (triangle.p3 - triangle.p2) / 2.f ) ) )
		return true;

	return true;
}

bool Frustum::contains(const AABB &aabb) const {
	if (!ready) {
		return true;
	}

	for (int i = 0; i < 6; i++) {
		if (p[i].side(aabb) == Plane::BACK) {
			return false;
		}
	}

	return true;
}

bool Frustum::contains(const Sphere &sphere) const {
	if (!ready) {
		return true;
	}

	for (int i = 0; i < 6; i++) {
		if (p[i].side(sphere) == Plane::BACK) {
			return false;
		}
	}

	return true;
}

bool Frustum::contains(const Quad &quad) const {
	if (!ready) {
		return true;
	}

	if( contains( X3D::Line( quad.p2, quad.p3 ) ) )
		return true;

	if( contains( X3D::Line( quad.p1, quad.p4 ) ) )
		return true;

	if( contains( quad.p1 ) )
		return true;

	if( contains( quad.p2 ) )
		return true;

	if( contains( quad.p3 ) )
		return true;

	if( contains( quad.p4 ) )
		return true;

	return false;
}

}