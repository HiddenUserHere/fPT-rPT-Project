#pragma once

#include "X3DVector4.h"

namespace X3D
{

class Vector3 {
public:
	/**
	 * Constructor
	 * Zero Vector
	 */
	Vector3();
	/**
	 * Constructor
	 */
	Vector3(const float x, const float y, const float z);
	/**
	 * Constructor
	 */
	Vector3(const Vector4 &c);
	/**
	 * Constructor
	 */
	Vector3(const float *p);
	/**
	 * Copy Constructor
	 */
	Vector3(const Vector3 &c);

	/**
	 * Compute Vector Length
	 */
	float length() const;

	/**
	 * Compute Vector Length Squared
	 */
	float lengthSquared() const;

	/**
	 * Normalize Vector
	 */
	Vector3& normalize();

	/**
	 * Get Normal of Vector
	 * @return normal
	 */
	Vector3 normal() const;

	/**
	 * Get Inverse of Vector
	 * @return inversed Vector
	 */
	Vector3 inverse() const;

	/**
	 * Dot Product
	 */
	float dot(const Vector3 &other) const;

	/**
	 * Cross Product
	 */
	Vector3 cross(const Vector3 &other) const;

	/**
	 * Operator *
	 * @param scaling
	 * @return scaled vector
	 */
	Vector3 operator*(const float scaling) const;

	/**
	 * Operator /
	 * @param scaling
	 * @return scaled vector
	 */
	Vector3 operator/(const float scaling) const;

	/**
	 * Operator +
	 * @param other
	 * @return added vector
	 */
	Vector3 operator+(const Vector3 &other) const;

	/**
	 * Operator -
	 * @param other
	 * @return subtracted vector
	 */
	Vector3 operator-(const Vector3 &other) const;

	/**
	 * Operator *
	 * @param other
	 * @return multiplied vector
	 */
	Vector3 operator*(const Vector3 &other) const;

	/**
	 * Operator /
	 * @param other
	 * @return divided vector
	 */
	Vector3 operator/(const Vector3 &other) const;

	/**
	 * Operator +=
	 * @param other
	 * @return this reference
	 */
	Vector3& operator+=(const Vector3 &other);

	/**
	 * Operator +=
	 * @param difference
	 * @return this reference
	 */
	Vector3& operator+=(const float difference);

	/**
	 * Operator -=
	 * @param difference
	 * @return this reference
	 */
	Vector3& operator-=(const float difference);

	/**
	 * Operator *=
	 * @param other
	 * @return this reference
	 */
	Vector3& operator*=(const Vector3 &other);

	/**
	 * Operator *=
	 * @param scaling
	 * @return this reference
	 */
	Vector3& operator*=(const float scaling);

	/**
	 * Operator /=
	 * @param scaling
	 * @return this reference
	 */
	Vector3& operator/=(const float scaling);

	/**
	 * Operator ==
	 * @param other vector
	 * @return bool
	 */
	bool operator==(const Vector3 &other) const;

	/**
	 * Operator !=
	 * @param other vector
	 * @return bool
	 */
	bool operator!=(const Vector3 &other) const;

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		struct
		{
			float r;
			float g;
			float b;
		};

		float f[3];
	};
};

}