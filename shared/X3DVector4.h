#pragma once

namespace X3D
{

class Vector3;

class Vector4 {
public:
	/**
	 * Constructor
	 * Zero Vector
	 */
	Vector4();
	/**
	 * Constructor
	 */
	Vector4(const float x, const float y, const float z, const float w);
	/**
	 * Constructor
	 */
	Vector4(const Vector3 &c);
	/**
	 * Constructor
	 */
	Vector4(const Vector3 &c, const float w);
	/**
	 * Constructor
	 */
	Vector4(const float *p);
	/**
	 * Copy Constructor
	 */
	Vector4(const Vector4 &c);

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
	Vector4& normalize();

	/**
	 * Operator *
	 * @param scaling
	 * @return scaled vector
	 */
	Vector4 operator*(const float scaling) const;

	/**
	 * Operator /
	 * @param scaling
	 * @return scaled vector
	 */
	Vector4 operator/(const float scaling) const;

	/**
	 * Operator +
	 * @param other
	 * @return added vector
	 */
	Vector4 operator+(const Vector4 &other) const;

	/**
	 * Operator -
	 * @param other
	 * @return subtracted vector
	 */
	Vector4 operator-(const Vector4 &other) const;

	/**
	 * Operator *
	 * @param other
	 * @return multiplied vector
	 */
	Vector4 operator*(const Vector4 &other) const;

	/**
	 * Operator /
	 * @param other
	 * @return divided vector
	 */
	Vector4 operator/(const Vector4 &other) const;

	/**
	 * Operator +=
	 * @param other
	 * @return this reference
	 */
	Vector4& operator+=(const Vector4 &other);

	/**
	 * Operator +=
	 * @param difference
	 * @return this reference
	 */
	Vector4& operator+=(const float difference);

	/**
	 * Operator -=
	 * @param difference
	 * @return this reference
	 */
	Vector4& operator-=(const float difference);

	/**
	 * Operator *=
	 * @param other
	 * @return this reference
	 */
	Vector4& operator*=(const Vector4 &other);

	/**
	 * Operator *=
	 * @param scaling
	 * @return this reference
	 */
	Vector4& operator*=(const float scaling);

	/**
	 * Operator /=
	 * @param scaling
	 * @return this reference
	 */
	Vector4& operator/=(const float scaling);

	/**
	 * Operator ==
	 * @param other vector
	 * @return bool
	 */
	bool operator==(const Vector4 &other) const;

	/**
	 * Operator !=
	 * @param other vector
	 * @return bool
	 */
	bool operator!=(const Vector4 &other) const;

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		float f[4];
	};
};

}