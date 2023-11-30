#pragma once

struct EXEFaceTextureRect
{
	int				  iLeft;
	int				  iRight;
	int				  iTop;
	int				  iBottom;
};

struct EXEFace2D
{
	Point3D			  sPosition;
	int				  iWidth;
	int				  iHeight;

	EXEFaceTextureRect sTextureRect;

	int				  iMaterialID;
	int				  iTransparency;

	int				  iRed;
	int				  iGreen;
	int				  iBlue;
};

#define	GAMEEFFECTFOLLOW_INFINITE			(-1)

#define SETPARTICLEMESHFOLLOW				CGameEffectFollow::StartMacro

enum EMonsterFollowEffectID
{
	MONSTERFOLLOWEFFECT_GreedyBody = 0x0586,
	MONSTERFOLLOWEFFECT_FaugnBody = 0x058B,
	MONSTERFOLLOWEFFECT_ArhdyraBody = 0x058C,
	MONSTERFOLLOWEFFECT_IgniculusBody = 0x058E,
	MONSTERFOLLOWEFFECT_InsecBody = 0x058F,
	MONSTERFOLLOWEFFECT_FlameMaidenBody = 0x0590,
	MONSTERFOLLOWEFFECT_IceElementalBody = 0x6A01,
	MONSTERFOLLOWEFFECT_IceElementalLeftHand = 0x6A02,
	MONSTERFOLLOWEFFECT_IceElementalRightHand = 0x6A03,
	MONSTERFOLLOWEFFECT_DarkGuardianCenter = 0x6A04,
	MONSTERFOLLOWEFFECT_DarkGuardianCenter01 = 0x6A05,
	MONSTERFOLLOWEFFECT_DarkGuardianCenter02 = 0x6A06,
};

struct EffectBaseFollowColor
{
	int iRed;
	int iGreen;
	int iBlue;
	int iAlpha;
};

struct EffectBaseFollowSize
{
	int iSameSize;
	int iWidth;
	int iHeight;
};

class EffectBaseFollowTrace
{
public:
	std::list<Point3D> sTraceList;
private:
	unsigned int iLength;

public:
	EffectBaseFollowTrace()
	{
		iLength = 50;
	}

	~EffectBaseFollowTrace()
	{

	}

	void SetLength( int length )
	{
		iLength = length;
	}

	void AddData( Point3D * psPosition );

};


class EffectBaseFollow
{
	IMPFNC				pfnGetFreeIndex		= 0x005B4990;
	IMPFNC				pfnIsActiveEffect	= 0x005D2FD0;
public:	

	EffectBaseFollow()
	{
		ZeroMemory( this, sizeof( EffectBaseFollow ) );
		pcEffectTrace = new EffectBaseFollowTrace;
	}

	virtual ~EffectBaseFollow() 
	{
		DELET( pcEffectTrace );
	};

	virtual void Main() {};
	virtual void Draw(Point3D * psPosition, Point3D * psAngle) {};

	DWORD				dwUnk01;
	DWORD				dwUnk02;

	DWORD				dwID;

	int					iTime;
	int					iTimeMax;

	int					iBoneID;
	char				szFileName[128];
	int					iPower;

	EXEFace2D			sFace;
	PTModel			* pcModel;
	int					iMaterialID;
	PTMesh				* pcMeshEffect;

	int					iIndex;
	int					iZDepth;

	EffectBaseFollowTrace * pcEffectTrace;

	Point3D				sPosition;
	Point3D				sAngle;
	Point3D				sTempPosition;
	Point3D				sTempAngle;
	Point3D				sStartPosition;
	int					iAddHeight;
	Point3D				sTargetPosition;

	int					iAnimationCount;
	int					iAnimationDelayTime;
	int					iAnimationMaxCount;
	int					iAnimationMaterialIndex[10];
	int					iAnimationReverse;

	Point3D				sMovementSpeed;
	Point3D				sMovementPosition;
	int					iGatherSpeed;
	int					iGravity;

	Point3D				sRearDistance;
	int					iRearAngle;
	int					iRearAngleSpeed;
	Point3D				sRearPosition;
	Point3D				sCenterPosition;

	Point3D				sAngleRotation;
	Point3D				sAngleRotationSpeed;
	int					iFaceY;


	UnitData			* pcUnitData;				//0x001DC
	UnitData			* pcUnitDataDest;			//0x001E0

	int					iNotFadeIn;
	int					iNotFadeOut;
	int					iMaxAlphaAmount;
	float				fAlphaAmount;
	int					iAlphaStartTime;

	EffectBaseFollowColor sColor;
	EffectBaseFollowColor sColorOld;

	int iTypeEffect;

	Point3D sScalePosition;
	Point3D sScaleSpeed;

	BOOL bCenter;

	BOOL bCollision;

	EffectBaseFollowSize sSize;
	EffectBaseFollowSize sSizeChange;

	int     iType;

	int		iaActionIndex[10];
	int		iaActionTime[10];

	DWORD   dwaNextID[10];
	int     iaNextTime[10];


	BOOL	bUseZ;

	int		Temp[10];
	float	fTemp[10];

	BOOL     bSelfPointer;

	static int			GetFreeIndex();

	static BOOL			IsActiveEffect( UnitData * pcUnitData, DWORD dwID );

	static BOOL			DeleteEffect( UnitData * pcUnitData );
	static void			MainEffect();


	static EffectBaseFollow ** GetData()
	{
		return ((EffectBaseFollow**)0x037B80F0);
	}

};

class EffectBaseFollowShot : public EffectBaseFollow
{
private:
	void Constructor();
	void Deconstructor();

public:
	EffectBaseFollowShot() { Constructor(); };
	~EffectBaseFollowShot() { Deconstructor(); };

	void Start( Point3D * psCurrentPosition, Point3D * psTargetPosition, int iEffectType );

public:

	int		iWidthStep;
	int		iState;
	int		iEffectType;
	int		iTimeCount;

	EffectBaseFollow * pcEffectFollow;
	int			 iParticleID;
	X3D::Vector3	sVelocity;


	Point3D     sLocalPosition;
	Point3D		sLocalAngle;
	Point3D     sTargetAngle;
	Point3D     sShootingAngle;

	bool	bIsChainingLight;
	UnitData * pcaUnitData[20];
	int		iUnitCount;
};


class CGameEffectFollow : EffectBaseFollow
{
private:
	IMPFNC					pfnConstructor	= 0x005594B0;
	IMPFNC					pfnStart		= 0x005466C0;

	void					Constructor();

public:
	CGameEffectFollow();
	~CGameEffectFollow();

	void					Start( UnitData * pcUnitData, char * pszMeshName, char * pszParticleName, bool bAttack = false, int iTime = GAMEEFFECTFOLLOW_INFINITE );

	void					SetPositionLength( int i );
	void					SetScale( float f );

	static BOOL				SetEffectUnitSpawn( UnitData * pcUnitData );

	static void				LoadParticles();

	static BOOL				StartMacro( UnitData * pcUnitData, char * pszMeshName, char * pszParticleName, int iID );

private:
	BOOL					bVisible;

	int						iParticleID;

	PTMesh					* pcMesh;

	BOOL					bSelfUnit;
	bool					bAttack;

	int						iLength;
	float					fScale;

	int						iTime;
	int						iTimeEnd;
};

