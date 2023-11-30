#pragma once

struct RawSMDVertex
{
	int		  iX;
	int		  iY;
	int		  iZ;
	int		  iNormalX;
	int		  iNormalY;
	int		  iNormalZ;
};

struct RawSMDTerrainVertex
{
	int		  iUnknown1;
	int		  iUnknown2;

	int		  iX;
	int		  iY;
	int		  iZ;

	short	  sBlue;
	short	  sGreen;
	short	  sRed;
	short	  sAlpha;
};