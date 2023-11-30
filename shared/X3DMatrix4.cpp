#include "stdafx.h"
#include "X3DMatrix4.h"

namespace X3D
{

Matrix4::Matrix4(const InitializerOption e) {
	if (e == Identity) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[i][j] = (i == j ? 1.0f : 0.0f);
			}
		}
	}
}

Matrix4::Matrix4(const float *matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = matrix[(i*4)+j];
		}
	}
}

Matrix4::Matrix4(const float matrix[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = matrix[i][j];
		}
	}
}

Matrix4::Matrix4(const Vector3 &right, const Vector3 &forward, const Vector3 &up) {
	m[0][0] = right.x;
	m[0][1] = up.x;
	m[0][2] = -forward.x;
	m[0][3] = 0.0f;
	m[1][0] = right.y;
	m[1][1] = up.y;
	m[1][2] = -forward.y;
	m[1][3] = 0.0f;
	m[2][0] = right.z;
	m[2][1] = up.z;
	m[2][2] = -forward.z;
	m[2][3] = 0.0f;
	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

Matrix4::Matrix4(const Matrix4 &c) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = c.m[i][j];
		}
	}
}

Matrix4& Matrix4::translate(const Vector3 &translation) {
	m[3][0] = translation.x;
	m[3][1] = translation.y;
	m[3][2] = translation.z;

	return *this;
}

Matrix4& Matrix4::rotateAxis(const Vector3 &axis, const float angle) {
	Quaternion q(axis, angle);

	*this = q.toMatrix();

	return *this;
}

Matrix4& Matrix4::rotateX(const float angle) {
	m[1][1] = cosf(angle);
	m[2][2] = m[1][1];
	m[1][2] = sinf(angle);
	m[2][1] = -m[1][2];

	return *this;
}

Matrix4& Matrix4::rotateY(const float angle) {
	m[0][0] = cosf(angle);
	m[2][2] = m[0][0];
	m[2][0] = sinf(angle);
	m[0][2] = -m[2][0];

	return *this;
}

Matrix4& Matrix4::rotateZ(const float angle) {
	m[0][0] = cosf(angle);
	m[1][1] = m[0][0];
	m[0][1] = sinf(angle);
	m[1][0] = -m[0][1];

	return *this;
}

Matrix4& Matrix4::scale(const Vector3 &scaling) {
	m[0][0] = scaling.x;
	m[1][1] = scaling.y;
	m[2][2] = scaling.z;

	return *this;
}

Matrix4& Matrix4::scale(const float scaling) {
	m[0][0] = scaling;
	m[1][1] = scaling;
	m[2][2] = scaling;

	return *this;
}

Matrix4 Matrix4::inverse() {
	float inv[16];
	float * tmp = (float*)m;

	inv[0] = 	tmp[5]  * tmp[10] * tmp[15] - 
				tmp[5]  * tmp[11] * tmp[14] - 
				tmp[9]  * tmp[6]  * tmp[15] + 
				tmp[9]  * tmp[7]  * tmp[14] +
				tmp[13] * tmp[6]  * tmp[11] - 
				tmp[13] * tmp[7]  * tmp[10];

	inv[4] = 	-tmp[4]  * tmp[10] * tmp[15] + 
				tmp[4]  * tmp[11] * tmp[14] + 
				tmp[8]  * tmp[6]  * tmp[15] - 
				tmp[8]  * tmp[7]  * tmp[14] - 
				tmp[12] * tmp[6]  * tmp[11] + 
				tmp[12] * tmp[7]  * tmp[10];

	inv[8] = 	tmp[4]  * tmp[9] * tmp[15] - 
				tmp[4]  * tmp[11] * tmp[13] - 
				tmp[8]  * tmp[5] * tmp[15] + 
				tmp[8]  * tmp[7] * tmp[13] + 
				tmp[12] * tmp[5] * tmp[11] - 
				tmp[12] * tmp[7] * tmp[9];

	inv[12] = 	-tmp[4]  * tmp[9] * tmp[14] + 
				tmp[4]  * tmp[10] * tmp[13] +
				tmp[8]  * tmp[5] * tmp[14] - 
				tmp[8]  * tmp[6] * tmp[13] - 
				tmp[12] * tmp[5] * tmp[10] + 
				tmp[12] * tmp[6] * tmp[9];

	inv[1] = 	-tmp[1]  * tmp[10] * tmp[15] + 
				tmp[1]  * tmp[11] * tmp[14] + 
				tmp[9]  * tmp[2] * tmp[15] - 
				tmp[9]  * tmp[3] * tmp[14] - 
				tmp[13] * tmp[2] * tmp[11] + 
				tmp[13] * tmp[3] * tmp[10];

	inv[5] = 	tmp[0]  * tmp[10] * tmp[15] - 
				tmp[0]  * tmp[11] * tmp[14] - 
				tmp[8]  * tmp[2] * tmp[15] + 
				tmp[8]  * tmp[3] * tmp[14] + 
				tmp[12] * tmp[2] * tmp[11] - 
				tmp[12] * tmp[3] * tmp[10];

	inv[9] = 	-tmp[0]  * tmp[9] * tmp[15] + 
				tmp[0]  * tmp[11] * tmp[13] + 
				tmp[8]  * tmp[1] * tmp[15] - 
				tmp[8]  * tmp[3] * tmp[13] - 
				tmp[12] * tmp[1] * tmp[11] + 
				tmp[12] * tmp[3] * tmp[9];

	inv[13] = 	tmp[0]  * tmp[9] * tmp[14] - 
				tmp[0]  * tmp[10] * tmp[13] - 
				tmp[8]  * tmp[1] * tmp[14] + 
				tmp[8]  * tmp[2] * tmp[13] + 
				tmp[12] * tmp[1] * tmp[10] - 
				tmp[12] * tmp[2] * tmp[9];

	inv[2] = 	tmp[1]  * tmp[6] * tmp[15] - 
				tmp[1]  * tmp[7] * tmp[14] - 
				tmp[5]  * tmp[2] * tmp[15] + 
				tmp[5]  * tmp[3] * tmp[14] + 
				tmp[13] * tmp[2] * tmp[7] - 
				tmp[13] * tmp[3] * tmp[6];

	inv[6] = 	-tmp[0]  * tmp[6] * tmp[15] + 
				tmp[0]  * tmp[7] * tmp[14] + 
				tmp[4]  * tmp[2] * tmp[15] - 
				tmp[4]  * tmp[3] * tmp[14] - 
				tmp[12] * tmp[2] * tmp[7] + 
				tmp[12] * tmp[3] * tmp[6];

	inv[10] = 	tmp[0]  * tmp[5] * tmp[15] - 
				tmp[0]  * tmp[7] * tmp[13] - 
				tmp[4]  * tmp[1] * tmp[15] + 
				tmp[4]  * tmp[3] * tmp[13] + 
				tmp[12] * tmp[1] * tmp[7] - 
				tmp[12] * tmp[3] * tmp[5];

	inv[14] = 	-tmp[0]  * tmp[5] * tmp[14] + 
				tmp[0]  * tmp[6] * tmp[13] + 
				tmp[4]  * tmp[1] * tmp[14] - 
				tmp[4]  * tmp[2] * tmp[13] - 
				tmp[12] * tmp[1] * tmp[6] + 
				tmp[12] * tmp[2] * tmp[5];

	inv[3] = 	-tmp[1] * tmp[6] * tmp[11] + 
				tmp[1] * tmp[7] * tmp[10] + 
				tmp[5] * tmp[2] * tmp[11] - 
				tmp[5] * tmp[3] * tmp[10] - 
				tmp[9] * tmp[2] * tmp[7] + 
				tmp[9] * tmp[3] * tmp[6];

	inv[7] = 	tmp[0] * tmp[6] * tmp[11] - 
				tmp[0] * tmp[7] * tmp[10] - 
				tmp[4] * tmp[2] * tmp[11] + 
				tmp[4] * tmp[3] * tmp[10] + 
				tmp[8] * tmp[2] * tmp[7] - 
				tmp[8] * tmp[3] * tmp[6];

	inv[11] = 	-tmp[0] * tmp[5] * tmp[11] + 
				tmp[0] * tmp[7] * tmp[9] + 
				tmp[4] * tmp[1] * tmp[11] - 
				tmp[4] * tmp[3] * tmp[9] - 
				tmp[8] * tmp[1] * tmp[7] + 
				tmp[8] * tmp[3] * tmp[5];

	inv[15] =	tmp[0] * tmp[5] * tmp[10] - 
				tmp[0] * tmp[6] * tmp[9] - 
				tmp[4] * tmp[1] * tmp[10] + 
				tmp[4] * tmp[2] * tmp[9] + 
				tmp[8] * tmp[1] * tmp[6] - 
				tmp[8] * tmp[2] * tmp[5];

	float det = tmp[0] * inv[0] + tmp[1] * inv[4] + tmp[2] * inv[8] + tmp[3] * inv[12];

	if (det == 0.f)
		return X3D::Matrix4();

	det = 1.f / det;

	X3D::Matrix4 mat(X3D::Matrix4::Uninitialized);
	tmp = (float*)mat.m;

	for (int i = 0; i < 16; i++) {
		tmp[i] = inv[i] * det;
	}

	return mat;
}

Matrix4& Matrix4::invert() {
	*this = inverse();
	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const {
	Matrix4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float total = 0.0f;

			for (int k = 0; k < 4; k++) {
				total += m[i][k] * other.m[k][j];
			}

			result.m[i][j] = total;
		}
	}

	return result;
}

Vector3 Matrix4::operator*(const Vector3 &other) const {
	Vector3 result;

	result.x = (m[0][0] * other.x) + (m[1][0] * other.y) + (m[2][0] * other.z) + m[3][0];
	result.y = (m[0][1] * other.x) + (m[1][1] * other.y) + (m[2][1] * other.z) + m[3][1];
	result.z = (m[0][2] * other.x) + (m[1][2] * other.y) + (m[2][2] * other.z) + m[3][2];

	return result;
}

Vector4 Matrix4::operator*(const Vector4 &other) const {
	Vector4 result;

	result.x = (m[0][0] * other.x) + (m[1][0] * other.y) + (m[2][0] * other.z) + (m[3][0] * other.w);
	result.y = (m[0][1] * other.x) + (m[1][1] * other.y) + (m[2][1] * other.z) + (m[3][1] * other.w);
	result.z = (m[0][2] * other.x) + (m[1][2] * other.y) + (m[2][2] * other.z) + (m[3][2] * other.w);
	result.w = (m[0][3] * other.x) + (m[1][3] * other.y) + (m[2][3] * other.z) + (m[3][3] * other.w);

	return result;
}

}