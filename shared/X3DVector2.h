#pragma once

namespace X3D
{

class Vector2 {
public:
	/**
	 * Constructor
	 * Zero Vector
	 */
	Vector2();
	/**
	 * Constructor
	 */
	Vector2(const float x, const float y);
	/**
	 * Constructor
	 */
	Vector2(const float *p);
	/**
	 * Copy Constructor
	 */
	Vector2(const Vector2 &c);

	/**
	 * Operator -
	 * @param other
	 * @return subtracted vector
	 */
	Vector2 operator-(const Vector2 &other) const;

	/**
	 * Operator /=
	 * @param scaling
	 * @return this reference
	 */
	Vector2& operator*=(const float scaling);

	/**
	 * Operator /=
	 * @param scaling
	 * @return this reference
	 */
	Vector2& operator/=(const float scaling);

	/**
	 * Operator ==
	 * @param other vector
	 * @return bool
	 */
	bool operator==(const Vector2 &other) const;

	/**
	 * Operator !=
	 * @param other vector
	 * @return bool
	 */
	bool operator!=(const Vector2 &other) const;

public:
	union
	{
		struct
		{
			float x;
			float y;
		};
		
		struct
		{
			float u;
			float v;
		};

		float f[2];
	};
};

}