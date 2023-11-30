#pragma once

struct RawINXAnimation
{
	int					  iType;

	int					  iKey1;
	int					  iStartFrame;
	int					  iKey2;
	int					  iEndFrame;

	int					  iaEventFrame[4];

	int					  iItemIndexCount;
	short				  saItemIndex[52];

	unsigned int		  uClassFlag;
	unsigned char		  uaSkill[8];

	int					  iAreaType;		//OR flag (1 = town, 2 = battlefield, 3 = both)
	int					  iLoop;
	unsigned char		  uaValue[4];
	int					  iSubFileFrameNumber;
};