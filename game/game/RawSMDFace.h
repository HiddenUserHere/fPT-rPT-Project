#pragma once

#include "RawSMDTextureLink.h"

struct RawSMDFace
{
	unsigned short		  saVertex[3];
	unsigned short		  sMaterial;
	int				  faUnknown[3][2];

	RawSMDTextureLink	* pTextureLink;
};

struct RawSMDTerrainFace
{
	int					  iUnknown1;
	int					  iUnknown2;
	
	unsigned short		  saVertex[3];
	unsigned short		  sMaterial;

	RawSMDTextureLink	* pTextureLink;

	short				  saNormal[4];
};