#include "stdafx.h"
#include "X3DVector2.h"

namespace X3D
{

Vector2::Vector2(): x(0.0f), y(0.0f) {
}

Vector2::Vector2(const float x, const float y): x(x), y(y) {
}

Vector2::Vector2(const float *p): x(p[0]), y(p[1]) {
}

Vector2::Vector2(const Vector2 &c): x(c.x), y(c.y) {
}

Vector2 Vector2::operator-(const Vector2 &other) const {
	return Vector2(x - other.x, y - other.y);
}

Vector2& Vector2::operator*=(const float scaling) {
	x *= scaling;
	y *= scaling;

	return *this;
}

Vector2& Vector2::operator/=(const float scaling) {
	x /= scaling;
	y /= scaling;

	return *this;
}

bool Vector2::operator==(const Vector2 &other) const {
	return ((x == other.x) && (y == other.y));
}

bool Vector2::operator!=(const Vector2 &other) const {
	return !(*this == other);
}

}