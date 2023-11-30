#pragma once

struct RawSMDMaterial
{
	enum EMaterialType : int
	{
		MATERIALTYPE_Move			= 0x0001,
		
		MATERIALTYPE_Animating2		= 0x0002,
		MATERIALTYPE_Animating4		= 0x0004,
		MATERIALTYPE_Animating8		= 0x0008,
		MATERIALTYPE_Animating16	= 0x0010,
		
		MATERIALTYPE_MoveZ1			= 0x0020,
		MATERIALTYPE_MoveZ2			= 0x0040,
		MATERIALTYPE_MoveX1			= 0x0080,
		MATERIALTYPE_MoveX2			= 0x0100,
		
		MATERIALTYPE_Water			= 0x0200,

		MATERIALTYPE_Invisible		= 0x0400,

		MATERIALTYPE_Solid			= 0x0800,
		MATERIALTYPE_WalkThrough	= 0x1000,

		MATERIALTYPE_RenderLast		= 0x2000,

		MATERIALTYPE_Blinking		= 0x4000,

		MATERIALTYPE_Ice			= 0x8000,
	};

	struct Texture
	{
		char		  szTextureFilePath[260];

					  Texture() { szTextureFilePath[0] = 0; }
	};

	int				  iInUse;

	unsigned int	  uTextureCount;
	Texture			* paTexture[8];
	int				  iaTextureStageState[8];
	int				  iaTextureFormState[8];

	int				  iUnknown2;

	int				  iHasTransparency;

	int				  iTextureType;
	int				  iBlendType;

	int				  iUnknown3;
	
	int				  iIsTwoSided;

	int				  iUnknown4;

	float			  fDiffuseRed;
	float			  fDiffuseGreen;
	float			  fDiffuseBlue;
	float			  fTransparency;
	float			  fSelfIllumination;

	int				  iUnknown5;
	int				  iUnknown6;
	int				  iUnknown7;

	EMaterialType	  eMaterialType1;
	EMaterialType	  eMaterialType2;
	EMaterialType	  eMaterialType3;

	Texture			* paAnimatedTexture[32];
	unsigned int	  uAnimatedTextureCount;
	int				  iUnknown13;
	int				  iUnknown14;
	int				  iUnknown15;
};