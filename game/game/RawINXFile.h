#pragma once

#include "RawINXLODGroup.h"
#include "RawINXAnimation.h"

struct RawINXFile
{
	char			  szModelFileSMD[64];			//ASE
	char			  szAnimationFileSMB[64];		//ASE

	char			  szDeathFileINX[64];			//INI

	RawINXLODGroup	  sHighLODGroup;
	RawINXLODGroup	  sMediumLODGroup;
	RawINXLODGroup	  sLowLODGroup;

	RawINXAnimation	  saAnimation[512];	//Starts at index 10
	unsigned int	  uAnimationCount;	//Starts at 10

	int				  iKey1;
	int				  iKey2;
	char			  szAnimationFileINX[64];		//IN

	char			  szTalkFileINX[64];			//IN
	char			  szTalkAnimationFileSMB[64];	//ASE
	RawINXAnimation	  saTalkAnimation[30];
	unsigned int	  uTalkAnimationCount;

	int				  iaTalkUnknown1[30];
	int				  iaTalkUnknown2[100];
	int				  iaTalkUnknown3[30];
	int				  iaTalkUnknown4[2][100];
}; //Size = 0x1060C