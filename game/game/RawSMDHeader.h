#pragma once

#include "RawSMDFrame.h"

const UCHAR szModelHeader[] = { 0xA5, 0x82, 0xAC, 0x58, 0x15, 0x98, 0x29, 0x8C, 0x85, 0x9F, 0x12, 0xC8, 0x84, 0x1F, 0x74, 0x8C, 0x8F, 0x42, 0x8F, 0xA8, 0x1B, 0x84, 0xAA, 0x00 };
const char szModelHeaderOld[] = "SMD Model data Ver 0.62";
const char szTerrainHeader1[] = "SMD Stage data Ver 0.72";
const char szTerrainHeader2[] = "SMD Stage data Ver 0.73";

struct RawSMDHeader
{
	char		  szHeader[24];

	int			  iObjectCount;
	int			  iMaterialCount;

	int			  iFilePointerToMaterials;
	int			  iFilePointerToFirstObject;

	int			  iFrameCount;
	RawSMDFrame	  aFrame[32];
};