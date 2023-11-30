#include "stdafx.h"
#include "X3DVector3.h"

namespace X3D
{

Vector3::Vector3(): x(0.0f), y(0.0f), z(0.0f) {
}

Vector3::Vector3(const float x, const float y, const float z): x(x), y(y), z(z) {
}

Vector3::Vector3(const Vector4 &c): x(c.x), y(c.y), z(c.z) {
}

Vector3::Vector3(const float *p): x(p[0]), y(p[1]), z(p[2]) {
}

Vector3::Vector3(const Vector3 &c): x(c.x), y(c.y), z(c.z) {
}

float Vector3::length() const {
	return sqrt(x*x + y*y + z*z);
}

float Vector3::lengthSquared() const {
	return x*x + y*y + z*z;
}

Vector3& Vector3::normalize() {
	float len = length();

	x /= len;
	y /= len;
	z /= len;

	return *this;
}

Vector3 Vector3::normal() const {
	Vector3 result;

	float len = length();

	result.x = x / len;
	result.y = y / len;
	result.z = z / len;

	return result;
}

Vector3 Vector3::inverse() const {
	Vector3 result = *this;

	result *= -1.0f;

	return result;
}

float Vector3::dot(const Vector3 &other) const {
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::cross(const Vector3 &other) const {
	Vector3 result;
	result.x = (y * other.z) - (z * other.y);
	result.y = (z * other.x) - (x * other.z);
	result.z = (x * other.y) - (y * other.x);
	return result;
}

Vector3 Vector3::operator*(const float scaling) const {
	return Vector3(x * scaling, y * scaling, z * scaling);
}

Vector3 Vector3::operator/(const float scaling) const {
	return Vector3(x / scaling, y / scaling, z / scaling);
}

Vector3 Vector3::operator+(const Vector3 &other) const {
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3 &other) const {
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(const Vector3 &other) const {
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::operator/(const Vector3 &other) const {
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3& Vector3::operator+=(const Vector3 &other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vector3& Vector3::operator+=(const float difference) {
	x += difference;
	y += difference;
	z += difference;

	return *this;
}

Vector3& Vector3::operator-=(const float difference) {
	x -= difference;
	y -= difference;
	z -= difference;

	return *this;
}

Vector3& Vector3::operator*=(const Vector3 &other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

Vector3& Vector3::operator*=(const float scaling) {
	x *= scaling;
	y *= scaling;
	z *= scaling;

	return *this;
}

Vector3& Vector3::operator/=(const float scaling) {
	x /= scaling;
	y /= scaling;
	z /= scaling;

	return *this;
}

bool Vector3::operator==(const Vector3 &other) const {
	return ((x == other.x) && (y == other.y) && (z == other.z));
}

bool Vector3::operator!=(const Vector3 &other) const {
	return !(*this == other);
}

}