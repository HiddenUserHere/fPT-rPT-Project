#pragma once

#include "X3DVector3.h"
#include "X3DVector4.h"

namespace X3D
{

class Matrix4 {
public:
	enum InitializerOption {
		Uninitialized = -1,
		Identity = 0,
	};

public:
	/**
	 * Constructor
	 * @param e initializer option, default Identity
	 */
	Matrix4(const InitializerOption e = Identity);
	/**
	 * Constructor
	 */
	Matrix4(const float *matrix);
	/**
	 * Constructor
	 */
	Matrix4(const float matrix[4][4]);
	/**
	 * Constructor
	 */
	Matrix4(const Vector3 &right, const Vector3 &forward, const Vector3 &up);
	/**
	 * Copy Constructor
	 */
	Matrix4(const Matrix4 &c);

	/**
	 * Translate Matrix
	 * @param t translation vector
	 * @return this reference
	 */
	Matrix4& translate(const Vector3 &translation);

	/**
	 * Rotate Matrix around arbitrary axis
	 * Angle is clockwise when viewed from the positive side of the arbitrary axis towards the origin
	 * @param angle in radians
	 * @return this reference
	 */
	Matrix4& rotateAxis(const Vector3 &axis, const float angle);

	/**
	 * Rotate Matrix around X
	 * Angle is clockwise when viewed from the positive side of the X axis towards the origin
	 * @param angle in radians
	 * @return this reference
	 */
	Matrix4& rotateX(const float angle);

	/**
	 * Rotate Matrix around Y
	 * Angle is clockwise when viewed from the positive side of the Y axis towards the origin
	 * @param angle in radians
	 * @return this reference
	 */
	Matrix4& rotateY(const float angle);

	/**
	 * Rotate Matrix around Z
	 * Angle is clockwise when viewed from the positive side of the Z axis towards the origin
	 * @param angle in radians
	 * @return this reference
	 */
	Matrix4& rotateZ(const float angle);

	/**
	 * Scale Matrix
	 * @param scaling vector
	 * @return this reference
	 */
	Matrix4& scale(const Vector3 &scaling);

	/**
	 * Scale Matrix
	 * @param scaling across all axes
	 * @return this reference
	 */
	Matrix4& scale(const float scaling);

	/**
	 * Gets the Inverse Matrix
	 * @return inverse
	 */
	Matrix4 inverse();

	/**
	 * Inverts the Matrix
	 * @return this reference
	 */
	Matrix4& invert();

	/**
	 * Operator *
	 * @param other matrix
	 * @return multiplied matrix
	 */
	Matrix4 operator*(const Matrix4 &other) const;

	/**
	 * Operator *
	 * @param other vector
	 * @return transformed vector
	 */
	Vector3 operator*(const Vector3 &other) const;

	/**
	 * Operator *
	 * @param other vector
	 * @return transformed vector
	 */
	Vector4 operator*(const Vector4 &other) const;

public:
	/**
	 * The Matrix Definition
	 */
	float m[4][4];
};

}