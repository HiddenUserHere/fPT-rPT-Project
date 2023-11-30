#pragma once

#include "EXEVertex.h"

struct EXEFace
{
	USHORT				uVertex1;	//1st Vertex
	USHORT				uVertex2;	//2nd Vertex
	USHORT				uVertex3;	//3rd Vertex
	USHORT				uMatID;		//Material Index

	Point2D				sUv[3];		//UV Coordinates

	void			  * pcTextureLink;
};

struct EXEFacePosition
{
	float			  fU1;
	float			  fU2;
	float			  fU3;

	float			  fV1;
	float			  fV2;
	float			  fV3;

	void			* pcTexture;
	DWORD			* pcNextTexture;
};

