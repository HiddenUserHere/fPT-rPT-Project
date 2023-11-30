#include "stdafx.h"
#include "X3DVector4.h"

#include "X3DVector3.h"

namespace X3D
{

Vector4::Vector4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f) {
}

Vector4::Vector4(const float x, const float y, const float z, const float w): x(x), y(y), z(z), w(w) {
}

Vector4::Vector4(const Vector3 &c): x(c.x), y(c.y), z(c.z), w(0.0f) {
}

Vector4::Vector4(const Vector3 &c, const float w): x(c.x), y(c.y), z(c.z), w(w) {
}

Vector4::Vector4(const float *p): x(p[0]), y(p[1]), z(p[2]), w(p[3]) {
}

Vector4::Vector4(const Vector4 &c): x(c.x), y(c.y), z(c.z), w(c.w) {
}

float Vector4::length() const {
	return sqrt(x*x + y*y + z*z + w*w);
}

float Vector4::lengthSquared() const {
	return x*x + y*y + z*z + w*w;
}

Vector4& Vector4::normalize() {
	float len = length();

	x /= len;
	y /= len;
	z /= len;
	w /= len;

	return *this;
}

Vector4 Vector4::operator*(const float scaling) const {
	return Vector4(x * scaling, y * scaling, z * scaling, w * scaling);
}

Vector4 Vector4::operator/(const float scaling) const {
	return Vector4(x / scaling, y / scaling, z / scaling, w / scaling);
}

Vector4 Vector4::operator+(const Vector4 &other) const {
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4 &other) const {
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(const Vector4 &other) const {
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4 Vector4::operator/(const Vector4 &other) const {
	return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vector4& Vector4::operator+=(const Vector4 &other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	return *this;
}

Vector4& Vector4::operator+=(const float difference) {
	x += difference;
	y += difference;
	z += difference;
	w += difference;

	return *this;
}

Vector4& Vector4::operator-=(const float difference) {
	x -= difference;
	y -= difference;
	z -= difference;
	w -= difference;

	return *this;
}

Vector4& Vector4::operator*=(const Vector4 &other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;

	return *this;
}

Vector4& Vector4::operator*=(const float scaling) {
	x *= scaling;
	y *= scaling;
	z *= scaling;
	w *= scaling;

	return *this;
}

Vector4& Vector4::operator/=(const float scaling) {
	x /= scaling;
	y /= scaling;
	z /= scaling;
	w /= scaling;

	return *this;
}

bool Vector4::operator==(const Vector4 &other) const {
	return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
}

bool Vector4::operator!=(const Vector4 &other) const {
	return !(*this == other);
}

}