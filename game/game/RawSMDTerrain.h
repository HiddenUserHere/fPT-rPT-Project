#pragma once

#include "RawSMDVertex.h"
#include "RawSMDFace.h"
#include "RawSMDTextureLink.h"
#include "RawSMDLight.h"

struct RawSMDTerrain
{
	int				  iUnknown1;

	DWORD			* aaArea[256][256];
	int				  iUnknown3;
	int				  iUnknown4;

	int				  iObsolete1;

	int				  iUnknown6;
	int				  iUnknown7;
	
	RawSMDTerrainVertex		* paVertex = NULL;
	RawSMDTerrainFace		* paFace = NULL;
	RawSMDTextureLink		* paTextureLink = NULL;
	RawSMDLight				* paLight = NULL;

	EXETextureManager * pcTextureManager;

	void				* pcAnims; //Size = 0x4B020

	EXETextureMaterial * pcTextureMaterial;
	
	int				  iVertexCount;
	int				  iFaceCount;
	int				  iTextureLinkCount;
	int				  iLightCount;

	int				  iUnknown15;

	int				  iContrast;
	int				  iBrightness;

	int				  iSunLightDirectionX;
	int				  iSunLightDirectionY;
	int				  iSunLightDirectionZ;

	DWORD			* pAreaBuffer;
	int				  iAreaSize;

	int				  iTerrainRectangleLeft;
	int				  iTerrainRectangleTop;
	int				  iTerrainRectangleRight;
	int				  iTerrainRectangleBottom;
};