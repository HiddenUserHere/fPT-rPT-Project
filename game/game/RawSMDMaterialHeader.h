#pragma once

#include "RawSMDMaterial.h"

struct RawSMDMaterialHeader
{
	int				  iUnknown1;

	RawSMDMaterial	* paRawSMDMaterial;

	unsigned int	  uMaterialCount;

	int				  iUnknown3;

	unsigned int	  uMaxMaterialCount;

	int				  iUnknown4;
	char			  szUnknown5[64];
};