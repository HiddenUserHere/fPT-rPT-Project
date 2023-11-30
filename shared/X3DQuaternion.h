#pragma once

#include "X3DMatrix4.h"

namespace X3D
{

class Quaternion {
public:
	/**
	 * Constructor
	 * Identity Quaternion
	 */
	Quaternion();
	/**
	 * Constructor
	 */
	Quaternion(const float w, const float x, const float y, const float z);
	/**
	 * Constructor
	 * From Euler angles in radians
	 */
	Quaternion(const float x, const float y, const float z);
	/**
	 * Constructor
	 * From Axis Angle in radians
	 */
	Quaternion(const Vector3 &axis, const float angle);
	/**
	 * Constructor
	 * From Matrix4
	 */
	Quaternion(const Matrix4 &matrix);
	/**
	 * Copy Constructor
	 */
	Quaternion(const Quaternion &c);

	/**
	 * Compute Quaternion Length
	 */
	float length() const;

	/**
	 * Compute Quaternion Length Squared
	 */
	float lengthSquared() const;

	/**
	 * Normalize Quaternion
	 * @return this reference
	 */
	Quaternion& normalize();

	/**
	 * Conjugates Quaternion
	 * @return this reference
	 */
	Quaternion& conjugate();

	/**
	 * Inverts Quaternion
	 * @return this reference
	 */
	Quaternion& invert();

	/**
	 * Rotation around Euler angles
	 * @param x in radians
	 * @param y in radians
	 * @param z in radians
	 * @return this reference
	 */
	Quaternion& fromEuler(const float ax, const float ay, const float az);

	/**
	 * Rotate around arbitrary axis
	 * @param axis axis to rotate about
	 * @param angle in radians
	 * @return this reference
	 */
	Quaternion& fromAxisAngle(Vector3 axis, const float angle);

	/**
	 * From Rotation Matrix
	 * @param matrix rotation matrix
	 * @return this reference
	 */
	Quaternion& fromMatrix(const Matrix4 &matrix);
	
	/**
	 * Convert to Euler
	 * @return euler angles as Vector3 in radians
	 */
	Vector3 toEuler() const;

	/**
	 * Convert to Axis Angle
	 * @param axis filled with axis data
	 * @return angle in radians
	 */
	float toAxisAngle(Vector3& axis) const;

	/**
	 * Convert to Matrix
	 * @return Matrix 4x4
	 */
	Matrix4 toMatrix() const;

	/**
	 * Dot Product
	 */
	float dot(const Quaternion &other) const;

	/**
	 * Lerp with another Quaternion
	 * @param other Quaternion to Lerp with
	 * @param t interpolation [0,1]
	 * @return new Quaternion
	 */
	Quaternion lerp(const Quaternion &other, const float t) const;

	/**
	 * Slerp with another Quaternion
	 * @param other Quaternion to Slerp with
	 * @param t interpolation [0,1]
	 * @return new Quaternion
	 */
	Quaternion slerp(const Quaternion &other, const float t) const;

	/**
	 * Operator *
	 * @param other
	 * @return new Quaternion
	 */
	Quaternion operator*(const Quaternion &other) const;

	/**
	 * Negates Quaternion
	 * @return new Quaternion
	 */
	Quaternion operator-() const;

	/**
	 * Operator /
	 * @param other
	 * @return new Quaternion
	 */
	Quaternion operator/(const Quaternion &other) const;

	/**
	 * Operator +
	 * @param other
	 * @return new Quaternion
	 */
	Quaternion operator+(const Quaternion &other) const;

	/**
	 * Operator -
	 * @param other
	 * @return new Quaternion
	 */
	Quaternion operator-(const Quaternion &other) const;

	/**
	 * Operator *
	 * @param vector to rotate
	 * @return new rotated Vector
	 */
	Vector3 operator*(const Vector3 &vector) const;
	
	/**
	 * Operator *
	 * @param scaling
	 * @return new Quaternion
	 */
	Quaternion operator*(const float scaling) const;

	/**
	 * Operator /
	 * @param scaling
	 * @return new Quaternion
	 */
	Quaternion operator/(const float scaling) const;
	
	/**
	 * Operator *=
	 * @param scaling
	 * @return this reference
	 */
	Quaternion& operator*=(const float scaling);

	/**
	 * Operator /=
	 * @param scaling
	 * @return this reference
	 */
	Quaternion& operator/=(const float scaling);

	/**
	 * Operator *= Quaternion
	 * @param other
	 * @return this reference
	 */
	Quaternion& operator*=(const Quaternion &other);

public:
	union
	{
		struct
		{
			float w;
			float x;
			float y;
			float z;
		};

		float f[4];
	};
};

}