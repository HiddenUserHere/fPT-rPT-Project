#pragma once

struct EXEDrawVertex
{
	float		x;
	float		y;
	float		z;
	float		rhw;
	D3DCOLOR	d3dColor;
	D3DCOLOR	d3dSpecular;
	float		u;
	float		v;
};

struct EXEColoredVertex
{
	float		x;
	float		y;
	float		z;
	D3DCOLOR	d3dcolor;
};

struct EXEDrawVertexModel
{
	float		x;
	float		y;
	float		z;
	D3DCOLOR	d3dColor;
	D3DCOLOR	d3dSpecular;
};

struct EXEDrawVertexModelCoord
{
	float		u;
	float		v;
};

struct EXEDrawVertexMinor
{
	float		x;
	float		y;
	float		z;
	float		rhw;
	D3DCOLOR	d3dColor;
	float		u;
	float		v;
};

struct EXEVertex
{
	Point3D sPosition;
	Point3D sPosition2;
};

struct EXEVertexF
{
    X3D::Vector3 sPosition;
    X3D::Vector3 sPosition2;
};

struct EXEVertexTangentF
{
    X3D::Vector3 sPosition;
    X3D::Vector3 sPosition2;
	X3D::Vector3 sTangent;
	X3D::Vector3 sBitangent;
};

struct EXEVector4D
{
	float fX;
	float fY;
	float fZ;
	float fW;
};