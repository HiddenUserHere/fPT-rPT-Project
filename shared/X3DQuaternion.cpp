#include "stdafx.h"
#include "X3DQuaternion.h"

namespace X3D
{

Quaternion::Quaternion(): w(1.0f), x(0.0f), y(0.0f), z(0.0f) {
}

Quaternion::Quaternion(const float w, const float x, const float y, const float z): w(w), x(x), y(y), z(z) {
	if (lengthSquared() != 0.0f) {
		normalize();
	}
}

Quaternion::Quaternion(const float x, const float y, const float z) {
	fromEuler(x, y, z);
}

Quaternion::Quaternion(const Vector3 &axis, const float angle) {
	fromAxisAngle(axis, angle);
}

Quaternion::Quaternion(const Matrix4 &matrix) {
	fromMatrix(matrix);
}

Quaternion::Quaternion(const Quaternion &c): w(c.w), x(c.x), y(c.y), z(c.z) {
}

float Quaternion::length() const {
	return sqrt(w*w + x*x + y*y + z*z);
}

float Quaternion::lengthSquared() const {
	return w*w + x*x + y*y + z*z;
}

Quaternion& Quaternion::normalize() {
	float len = length();

	w /= len;
	x /= len;
	y /= len;
	z /= len;

	return *this;
}

Quaternion& Quaternion::conjugate() {
	x = -x;
	y = -y;
	z = -z;

	return *this;
}

Quaternion& Quaternion::invert() {
	conjugate();

	float lenSq = lengthSquared();

	w /= lenSq;
	x /= lenSq;
	y /= lenSq;
	z /= lenSq;

	return *this;
}

Quaternion& Quaternion::fromEuler(const float ax, const float ay, const float az) {
	const float ex = ax / 2.0f;
	const float ey = ay / 2.0f;
	const float ez = az / 2.0f;

	const float cr = cosf(ex);
	const float cp = cosf(ey);
	const float cy = cosf(ez);

	const float sr = sinf(ex);
	const float sp = sinf(ey);
	const float sy = sinf(ez);

	const float cpcy = cp * cy;
	const float spsy = sp * sy;

	w = cr * cpcy + sr * spsy;
	x = sr * cpcy - cr * spsy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;

	normalize();

	return *this;
}

Quaternion& Quaternion::fromAxisAngle(Vector3 axis, const float angle) {
	axis.normalize();

	const float angleDiv2 = angle / 2.0f;
	const float sinAngleDiv2 = sinf(angleDiv2);

	w = cosf(angleDiv2);
	x = axis.x * sinAngleDiv2;
	y = axis.y * sinAngleDiv2;
	z = axis.z * sinAngleDiv2;

	return *this;
}

Quaternion& Quaternion::fromMatrix(const Matrix4 &matrix) {
	float trace = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];

	if (trace > 0.0f) {
		float s = 0.5f / sqrtf(trace + 1.0f);

		w = 0.25f / s;
		x = (matrix.m[2][1] - matrix.m[1][2]) * s;
		y = (matrix.m[0][2] - matrix.m[2][0]) * s;
		z = (matrix.m[1][0] - matrix.m[0][1]) * s;
	} else {
		if ((matrix.m[0][0] > matrix.m[1][1]) && (matrix.m[0][0] > matrix.m[2][2])) {
			float s = sqrtf(1.0f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]) * 2.0f;

			w = (matrix.m[2][1] - matrix.m[1][2]) / s;
			x = 0.25f * s;
			y = (matrix.m[0][1] + matrix.m[1][0]) / s;
			z = (matrix.m[0][2] + matrix.m[2][0]) / s;
		} else if (matrix.m[1][1] > matrix.m[2][2]) {
			float s = sqrtf(1.0f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]) * 2.0f;

			w = (matrix.m[0][2] - matrix.m[2][0]) / s;
			x = (matrix.m[0][1] + matrix.m[1][0]) / s;
			y = 0.25f * s;
			z = (matrix.m[1][2] + matrix.m[2][1]) / s;
		} else {
			float s = sqrtf(1.0f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]) * 2.0f;

			w = (matrix.m[1][0] - matrix.m[0][1]) / s;
			x = (matrix.m[0][2] + matrix.m[2][0]) / s;
			y = (matrix.m[1][2] + matrix.m[2][1]) / s;
			z = 0.25f * s;
		}
	}

	normalize();

	return *this;
}

Vector3 Quaternion::toEuler() const {
	const float sqw = w * w;
	const float sqx = x * x;
	const float sqy = y * y;
	const float sqz = z * z;

	Vector3 euler;

	euler.x = atan2f(2.0f * (z * y + x * w), 1 - 2.0f * (sqx + sqy));
	euler.y = asinf(-2.0f * (x * z - y * w));
	euler.z = atan2f(2.0f * (x * y + z * w), 1 - 2.0f * (sqy + sqz));

	return euler;
}

float Quaternion::toAxisAngle(Vector3& axis) const {
	const float angle = acosf(w);
	const float sinThetaInv = 1.0f / sinf(angle);

	axis.x = x * sinThetaInv;
	axis.y = y * sinThetaInv;
	axis.z = z * sinThetaInv;

	return (angle * 2.0f);
}

Matrix4 Quaternion::toMatrix() const {
	Matrix4 result;

	//First Row (LH)
	result.m[0][0] = 1.0f - (2.0f * ((y * y) + (z * z)));
	result.m[0][1] =        (2.0f * ((x * y) - (z * w)));
	result.m[0][2] =        (2.0f * ((x * z) + (y * w)));

	//Second Row (LH)
	result.m[1][0] =        (2.0f * ((x * y) + (z * w)));
	result.m[1][1] = 1.0f - (2.0f * ((x * x) + (z * z)));
	result.m[1][2] =        (2.0f * ((y * z) - (x * w)));

	//Third Row (LH)
	result.m[2][0] =        (2.0f * ((x * z) - (y * w)));
	result.m[2][1] =        (2.0f * ((y * z) + (x * w)));
	result.m[2][2] = 1.0f - (2.0f * ((x * x) + (y * y)));

	return result;
}

float Quaternion::dot(const Quaternion &other) const {
	return (w * other.w) + (x * other.x) + (y * other.y) + (z * other.z);
}

Quaternion Quaternion::lerp(const Quaternion &other, const float t) const {
	Quaternion result;

	result = ((*this * (1 - t)) + (other * t)).normalize();

	return result;
}

Quaternion Quaternion::slerp(const Quaternion &other, const float t) const {
	Quaternion n;
	float d = dot(other);

	if (d < 0) {
		d = -d;
		n = -other;
	} else
		n = other;

	if (d < 0.95f) {
		const float angle = acosf(d);

		return ((*this * sinf(angle * (1 - t))) + (n * sinf(angle * t))) / sinf(angle);
	} else
		return lerp(n, t);
}

Quaternion Quaternion::operator*(const Quaternion &other) const {
	Quaternion result;

	result.w = (w*other.w) - (x*other.x) - (y*other.y) - (z*other.z);
	result.x = (w*other.x) + (x*other.w) + (y*other.z) - (z*other.y);
	result.y = (w*other.y) - (x*other.z) + (y*other.w) + (z*other.x);
	result.z = (w*other.z) + (x*other.y) - (y*other.x) + (z*other.w);

	return result;
}

Quaternion Quaternion::operator-() const {
	Quaternion result;

	result.w = -w;
	result.x = -x;
	result.y = -y;
	result.z = -z;

	return result;
}

Quaternion Quaternion::operator/(const Quaternion &other) const {
	Quaternion result;

	result = *this * (Quaternion(other).invert());

	return result;
}

Quaternion Quaternion::operator+(const Quaternion &other) const {
	Quaternion result;

	result.w = w + other.w;
	result.x = x + other.x;
	result.y = y + other.y;
	result.z = z + other.z;

	return result;
}

Quaternion Quaternion::operator-(const Quaternion &other) const {
	Quaternion result;
	
	result.w = w - other.w;
	result.x = x - other.x;
	result.y = y - other.y;
	result.z = z - other.z;

	return result;
}

Vector3 Quaternion::operator*(const Vector3 &vector) const {
	Matrix4 matrix = toMatrix();

	Vector3 result = matrix * vector;

	return result;
}

Quaternion Quaternion::operator*(const float scaling) const {
	Quaternion result;

	result.w = w * scaling;
	result.x = x * scaling;
	result.y = y * scaling;
	result.z = z * scaling;

	return result;
}

Quaternion Quaternion::operator/(const float scaling) const {
	Quaternion result;

	result.w = w / scaling;
	result.x = x / scaling;
	result.y = y / scaling;
	result.z = z / scaling;

	return result;
}

Quaternion& Quaternion::operator*=(const float scaling) {
	*this = *this * scaling;

	return *this;
}

Quaternion& Quaternion::operator/=(const float scaling) {
	*this = *this / scaling;

	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion &other) {
	*this = *this * other;

	return *this;
}

}