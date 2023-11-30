#pragma once

#include "EXEMatrix.h"

#define MONSTER_SERVER_MAX			800

#define	ATTACK_DAMAGE_MAX			100

#define UNIT_DEFAULT_LURE_DISTANCE_FAR	1638400

#define _UNIT_PLAYER_MINIMUM

class EXEMesh;
class EXEModel;
class Map;

class UserData;

#if defined(_GAME)
namespace Delta3D::Graphics
{
struct ModelGroup;
}

struct ModelsGroup
{
	Delta3D::Graphics::ModelGroup HighModel;
	Delta3D::Graphics::ModelGroup DefaultModel;
	Delta3D::Graphics::ModelGroup LowModel;
};

#endif

#if defined(_SERVER)
typedef void ModelsGroup;
#endif

struct EXEModelData
{
	EXEModelData			* psUnused;

	EXEModelData			* psMotion;
	PTModel_ptr				 pcModel;

	char					szModelName[64];
	int						iCount;

	DWORD					dwCheckSum;

	struct ModelData		* psModelData;
	ModelsGroup				* m_ModelsGroup = nullptr;
};



struct AttackDamageData
{
	int					iID;
	int					iDamage;
	int					iCounter;
};

enum ENPCID : int
{
	NPCID_EventGirl							= 1,
	NPCID_ItemDistributor					= 2,
	NPCID_Warehouse							= 3,
	NPCID_Force								= 4,
	NPCID_CastleControl						= 5,
	NPCID_Aging								= 6,
	NPCID_Smelting							= 7,
	NPCID_ClanControl						= 8,
	NPCID_Teleport							= 9,
	NPCID_SkillMaster						= 10,
	NPCID_Mixing							= 11,
	NPCID_Manufacturing						= 12,
	NPCID_WarpGate							= 13,
	NPCID_Bellatra							= 14,
	NPCID_ResetItem							= 15,
	NPCID_CoinShop							= 16,
	NPCID_FuryArenaNPC						= 17,
	NPCID_FuryArenaNPCEnter					= 18,
	NPCID_SocketSystemNPCDrill				= 19,
	NPCID_SocketSystemNPCStone				= 20,

	NPCID_NPCEasterEgg						= 21,

	NPCID_LarryQuests						= 22,
	NPCID_IhinEvent							= 23,
	NPCID_MarinaQuests						= 24,
	NPCID_RudolphXmas						= 25,
	NPCID_ActionFieldEnter					= 26,
	NPCID_ActionFieldExchange				= 27,
	NPCID_BeginnerItemTimer					= 28,
	NPCID_XmasTreeGift						= 29,
	NPCID_BattleRoyale						= 30,
	NPCID_Carnival							= 31,
	NPCID_TimeShop							= 32,
	NPCID_ActionFieldFD1					= 33,
	NPCID_ActionFieldFD2					= 34,
	NPCID_ActionFieldFD3					= 35,
	NPCID_RankingLevel						= 36,
	NPCID_ActionFieldDG1					= 37,
};

enum ENPCEventCode : int
{
	NPCEVENTCODE_Force						= 200,
	NPCEVENTCODE_Smelting					= 2,
	NPCEVENTCODE_Manufacture				= 3,
	NPCEVENTCODE_ResetItem					= 4,
	NPCEVENTCODE_CoinShop					= 5,
	NPCEVENTCODE_FuryArenaNPCTeleportCT2	= 6,
	NPCEVENTCODE_FuryArenaNPCEnter			= 7,
	NPCEVENTCODE_SocketSystemNPCDrill		= 8,
	NPCEVENTCODE_SocketSystemNPCStone		= 9,
	NPCEVENTCODE_EasterNPC					= 10,
	NPCEVENTCODE_LarryQuestsNPC				= 11,
	NPCEVENTCODE_IhinEvent					= 12,
	NPCEVENTCODE_MarinaQuestsNPC			= 13,
	NPCEVENTCODE_RudolphXmasNPC				= 14,
	NPCEVENTCODE_ActionFieldEnter			= 15,
	NPCEVENTCODE_ActionFieldExchange		= 16,
	NPCEVENTCODE_CaravanNPC					= 17,
	NPCEVENTCODE_NPCBeginnerItemTimer		= 18,
	NPCEVENTCODE_XmasTreeNPC				= 19,
	NPCEVENTCODE_XmasTreeGiftNPC			= 20,
	NPCEVENTCODE_BattleRoyaleNPC			= 21,
	NPCEVENTCODE_CarnivalNPC				= 22,
	NPCEVENTCODE_TimeShop					= 23,
	NPCEVENTCODE_ActionFieldFD1				= 24,
	NPCEVENTCODE_ActionFieldFD2				= 25,
	NPCEVENTCODE_ActionFieldFD3				= 26,
	NPCEVENTCODE_RankingLevel				= 27,
	NPCEVENTCODE_ActionFieldDG1				= 28,
};

enum EAnimationType : int
{
	ANIMATIONTYPE_None					= 0x0000,	//0
	ANIMATIONTYPE_Idle					= 0x0040,	//64
	ANIMATIONTYPE_IdleMount				= 0x0041,	//65
	ANIMATIONTYPE_Walking				= 0x0050,	//80
	ANIMATIONTYPE_Running				= 0x0060,	//96
	ANIMATIONTYPE_RunningMount			= 0x0061,	//97
	ANIMATIONTYPE_Sprint				= 0x0070,	//112
	ANIMATIONTYPE_Falling				= 0x0080,	//128
	ANIMATIONTYPE_Attack				= 0x0100,	//256
	ANIMATIONTYPE_Flinch				= 0x0110,	//272
	ANIMATIONTYPE_Die					= 0x0120,	//288
	ANIMATIONTYPE_Potion				= 0x0140,	//320
	ANIMATIONTYPE_Skill					= 0x0150,	//336
	ANIMATIONTYPE_FallLow				= 0x0170,	//368
	ANIMATIONTYPE_FallHigh				= 0x0180,	//384
	ANIMATIONTYPE_Revive				= 0x0200,	//512
	ANIMATIONTYPE_Special				= 0x0210,	//528
	ANIMATIONTYPE_Yahoo					= 0x0220,	//544
	ANIMATIONTYPE_Taunt					= 0x0230,	//560
	ANIMATIONTYPE_Dance					= 0x0240,	//576
	ANIMATIONTYPE_Hammer				= 0x0300,	//768
};

enum EAnimationID : int
{
	ANIMATIONID_None					= 0,
};

enum EToolEquipModeTown : int
{
	TOOLEQUIPMODETOWN_None				= 0,	//Wearing Weapon as if in Battle
	TOOLEQUIPMODETOWN_Back				= 1,	//Wearing Weapon on the Back
	TOOLEQUIPMODETOWN_BackCrossBow		= 2,	//Wearing Weapon on the Back for CrossBows
	TOOLEQUIPMODETOWN_BackBow			= 3,	//Wearing Weapon on the Back for Bows
};

enum ECharacterGender
{
	CHARACTERGENDER_None,
	CHARACTERGENDER_Male,
	CHARACTERGENDER_Female,
};

enum EUpdateMode : int
{
	UPDATEMODE_ClientSelf				= 0,
	UPDATEMODE_ServerMonster			= 5,
	UPDATEMODE_ServerNPC				= 12,
	UPDATEMODE_ClientUnit				= 99,
	UPDATEMODE_ClientTarget				= 101,
};

enum EAnimationClassFlag : unsigned int
{
	ANIMATIONCLASSFLAG_None				= 0,

	ANIMATIONCLASSFLAG_Fighter			= 0x0001,
	ANIMATIONCLASSFLAG_Mechanician		= 0x0002,
	ANIMATIONCLASSFLAG_Archer			= 0x0004,
	ANIMATIONCLASSFLAG_Pikeman			= 0x0008,
	ANIMATIONCLASSFLAG_Atalanta			= 0x0010,
	ANIMATIONCLASSFLAG_Knight			= 0x0020,
	ANIMATIONCLASSFLAG_Magician			= 0x0040,
	ANIMATIONCLASSFLAG_Priestess		= 0x0080,
	ANIMATIONCLASSFLAG_Assassin			= 0x0100,
	ANIMATIONCLASSFLAG_Shaman			= 0x0200,
};

enum EPotionStatus : int
{
	POTIONSTATUS_HP						= 1,
	POTIONSTATUS_MP						= 2,
	POTIONSTATUS_SP						= 3,
	POTIONSTATUS_HPMPSP					= 4,
	POTIONSTATUS_EtherCore				= 0x40,

};

struct MoveSequence
{
	Point3D				  sPosition;				//0x00

	int					  iFrame;					//0x0C

	short				  saAngle[4];				//0x10

	short				  saEquippedItems[4];		//0x18

	ID					  lTargetID;				//0x20
}; //Size = 0x24

struct ModelAnimation
{
	enum EMapTypeFlag : unsigned int
	{
		MAPTYPEFLAG_None = 0,
		MAPTYPEFLAG_Town = 1,
		MAPTYPEFLAG_BattleField = 2,
		MAPTYPEFLAG_Unknown = 3,
	};

	EAnimationType		  iType;					//0x00

	int					  iKey1;					//0x04
	int					  iBeginFrame;				//0x08
	int					  iKey2;					//0x0C
	int					  iEndFrame;				//0x10

	int					  iaEventFrame[4];			//0x14

	int					  iItemIndexCount;			//0x24

	WORD				  eaItemIndex[52];			//0x28

	EAnimationClassFlag	  eClassFlag;				//0x5C
	unsigned char		  uaSkill[8];				//0x60

	EMapTypeFlag		  eMapTypeFlag;				//0x68 OR Flag
	BOOL				  bLoop;					//0x6C

	BYTE				  uaValue[4];				//0x70

	int					  iSubFileFrameNumber;		//0x74
}; //Size = 0x78

struct ModelAnimationOld
{
	enum EMapTypeFlag : unsigned int
	{
		MAPTYPEFLAG_None = 0,
		MAPTYPEFLAG_Town = 1,
		MAPTYPEFLAG_BattleField = 2,
	};

	EAnimationType		  iType;			//0x00

	int					  iKey1;					//0x04
	int					  iBeginFrame;				//0x08
	int					  iKey2;					//0x0C
	int					  iEndFrame;				//0x10

	int					  iaEventFrame[4];			//0x14

	int					  iItemIndexCount;			//0x24

	BYTE				  eaItemIndex[52];			//0x28

	EAnimationClassFlag	  eClassFlag;				//0x5C
	unsigned char		  uaSkill[8];				//0x60

	EMapTypeFlag		  eMapTypeFlag;				//0x68 OR Flag
	BOOL				  bLoop;					//0x6C

	BYTE				  uaValue[4];				//0x70

	int					  iSubFileFrameNumber;		//0x74
}; //Size = 0x78

struct ModelData
{
	struct MeshQuality
	{
		int				  iCount;

		union
		{
			struct
			{
				char			  szMeshName1[16];
				char			  szMeshName2[16];
				char			  szMeshName3[16];
				char			  szMeshName4[16];
			};

			char szaMeshName[4][16];
		};
	};

	char				  szModelPath[64];					//ASE
	union
	{
		struct
		{
			int			  iID;
			int			  iWidth;
			int			  iHeight;
			int			  iDeathModelID;
			ModelData	* pDeathModelData;
		};

		char			  szBoneModelFilePath[64];			//ASE
	};
	char				  szDeathModelDataFilePath[64];		//INI

	MeshQuality			  sHighMeshQuality;
	MeshQuality			  sMediumMeshQuality;
	MeshQuality			  sLowMeshQuality;

	ModelAnimation		  saModelAnimation[512];
	int					  iNumModelAnimation;

	DWORD				  dwObfuscator1;
	DWORD				  dwObfuscator2;
	char				  szModelDataAnimationFile[64];		//INI

	char				  szTalkModelDataFilePath[64];		//INI
	char				  szTalkBoneModelFilePath[64];		//ASE
	ModelAnimation		  saTalkModelAnimation[30];
	unsigned int		  iTalkNumModelAnimation;

	int					  iaTalkUnknown1[30];
	int					  iaTalkUnknown2[100];
	int					  iaTalkUnknown3[30];
	int					  iaTalkUnknown4[2][100];
}; //Size = 0x1060C

struct ModelDataOld
{
	struct MeshQuality
	{
		int				  iCount;
		char			  szMeshName1[16];
		char			  szMeshName2[16];
		char			  szMeshName3[16];
		char			  szMeshName4[16];
	};

	char				  szModelPath[64];					//ASE
	union
	{
		struct
		{
			int			  iID;
			int			  iWidth;
			int			  iHeight;
			int			  iDeathModelID;
			ModelData	* pDeathModelData;
		};

		char			  szBoneModelFilePath[64];			//ASE
	};
	char				  szDeathModelDataFilePath[64];		//INI

	MeshQuality			  sHighMeshQuality;
	MeshQuality			  sMediumMeshQuality;
	MeshQuality			  sLowMeshQuality;

	ModelAnimationOld	  saModelAnimation[512];
	int					  iNumModelAnimation;

	DWORD				  dwObfuscator1;
	DWORD				  dwObfuscator2;
	char				  szModelDataAnimationFile[64];		//INI

	char				  szTalkModelDataFilePath[64];		//INI
	char				  szTalkBoneModelFilePath[64];		//ASE
	ModelAnimationOld	  saTalkModelAnimation[30];
	unsigned int		  iTalkNumModelAnimation;

	int					  iaTalkUnknown1[30];
	int					  iaTalkUnknown2[100];
	int					  iaTalkUnknown3[30];
	int					  iaTalkUnknown4[2][100];
}; //Size = 0x1060C

namespace Delta3D::Graphics{ class Material; };

struct UnitTool
{
	PTMesh							* pBone;
	PTModel						* pModel;
	EItemID							  eItemID;

	int								  iSizeMax;
	int								  iSizeMin;

	int								  iBlinkSpeed;		//0 = disabled. [1,inf], higher is faster blink speed
	union
	{
		short						  saBlinkColor[4];	//BGRA

		struct
		{
			int iAgeLevel;
			int iMixID;
		};
	};
	BOOL							  bStarEffect;		//Stars circling around Weapon
	int								  iBlinkScale;		//Obsolete
	
	Delta3D::Graphics::Material		* pcBlendingMaterial;

	int								  iOverlayTextureIndex;
	int								  iOverlayTextureTransformationType;
}; //Size = 0x34

class UnitData
{
private:

#if defined(_GAME)
	IMPFNC									pfnCloseUnitData			= 0x0041F3C0;
	IMPFNC									pfnHandleBufferData			= 0x00423440;
	IMPFNC									pfnAnimate					= 0x00429680;
	IMPFNC									pfnSetNextTarget			= 0x00000000;
	IMPFNC									pfnMain						= 0x0043CB20;
	IMPFNC									pfnSetPosition				= 0x0041F600;
	IMPFNC									pfnSetAngleByPosition		= 0x00000000;
	IMPFNC									pfnLoadModel				= 0x00000000;
	IMPFNC									pfnRender					= 0x00429AF0;
	IMPFNC									pfnGetAnimAttackPosition	= 0x004294D0;
	IMPFNC									pfnInit						= 0x0041EF00;
	IMPFNC									pfnChangeAnimationID		= 0x0041FA04;
	IMPFNC									pfnUpdateAttack				= 0x0042B600;
	IMPFNC									pfnMove						= 0x0044FFB4;
	IMPFNC									pfnSetTarget				= 0x00433E20;
	IMPFNC									pfnSetTargetEx				= 0x00000000;
	IMPFNC									pfnSetHeadModelData			= 0x0041F830;
	IMPFNC									pfnSetBodyModelData			= 0x00434370;
#elif defined(_SERVER)
	IMPFNC									pfnCloseUnitData			= 0x004167A0;
	IMPFNC									pfnHandleBufferData			= 0x0041B780;
	IMPFNC									pfnAnimate					= 0x00416E30;
	IMPFNC									pfnSetNextTarget			= 0x00416A90;
	IMPFNC									pfnMain						= 0x00429930;
	IMPFNC									pfnSetPosition				= 0x00416970;
	IMPFNC									pfnSetAngleByPosition		= 0x00416A50;
	IMPFNC									pfnLoadModel				= 0x00429350;
	IMPFNC									pfnRender					= 0x00000000;
	IMPFNC									pfnGetAnimAttackPosition	= 0x00000000;
	IMPFNC									pfnInit						= 0x00000000;
	IMPFNC									pfnChangeAnimationID		= 0x00000000;
	IMPFNC									pfnUpdateAttack				= 0x00000000;
	IMPFNC									pfnMove						= 0x0041EB40;
	IMPFNC									pfnSetTarget				= 0x0041E670;
	IMPFNC									pfnSetTargetEx				= 0x004454A0;
	IMPFNC									pfnSetHeadModelData			= 0x00000000;
	IMPFNC									pfnSetBodyModelData			= 0x00000000;
#endif

public:
	//Attributes
	DWORD							dwHeader;	

	BOOL                            bVisible;
	BOOL                            bVisibleEquipment;
	class Stage                     * pcStage;
	ID		                        iID;
	BOOL                            bRemoteControlled;

	PTModel                        * pcBodyMeshContainer;
	PTModel                        * pcHeadMeshContainer;
	PTModel                        * pcBoneContainer;
	PTModel                        * pcTalkBoneContainer;

	EXEModelData                    * pcBody;
	EXEModelData                    * pcHead;

	EXEModelData                    * pcLeftHandTool;
	EXEModelData                    * pcRightHandTool;

	UnitTool						sLeftHandTool;
	UnitTool						sRightHandTool;

	EXEMatrixI						sLeftHandMatrix;
	EXEMatrixI						sRightHandMatrix;

	BOOL                            bRenderLeftHand;
	BOOL                            bRenderRightHand;

#if defined(_GAME)
	EXEMatrixI						sWingMatrix;
	EXEMatrixI						sWingSkillMatrix;
#endif

	PTMesh                         * pcaBackBone[3];
	PTMesh                         * pcAttackBone;
	PTMesh                         * pcShieldBone;

#if defined(_GAME)
	int								iPadding63[3];

	PTMesh							* pcaWeaponDaggerBone[2];
#endif

	int                             iRange;
	union
	{
		int							iFireOverlay;			
		int							iHits;				
	};
	int                             iIceOverlay;

	BOOL                            bActive;

	Point3D							sPosition;
	Point3D							sAttackPosition;
	int                             iFloorHeight;
	int                             iLastHeight;
	int                             iLoadedMapIndex;
	int                             iMapType;

	BOOL                            bNoMove;

	int                             iModelWidth;
	int                             iModelHeight;

	int                             iShadowSize;

	BOOL                            bCollision;
	BOOL                            bCheckCollision;
	UnitData						* pcCollisionUnitData;

	Point3D							sPosi;
	Point3D							sAngle;

	BOOL                            bRunMode;
	BOOL                            bWalking;
	int                             iMoveCount;	

	int                             iTargetX;
	int								iTargetZ;
	int                             iTargetMoveCount;

	int                             iWaterHeight;

	EItemID                         eWeaponItemID;
	EToolEquipModeTown              eToolEquipModeTown;	
	BOOL							bIsWeaponTwoHanded;

	short                           saEquippedItems[4];

	BOOL                            bProjectileMode;
	int                             iProjectileType;
	int                             iProjectileType2;
	Point3D                         sProjectilePosition;
	Point3D                         sProjectileAngle;
	int                             iProjectileCount;

	UnitData						* pcTarget;
	int                             iTargetCritical;
	int                             iTargetCriticalCount;
	int                             iTargetUnknown3;

	ActiveSkill                     sActiveSkill;
	int                             iAttackEffect;

	BOOL                            bChaosCaraDrainMana;
	BOOL                            bKilled;
	Point3D                         sMoveToPosition;
	Point3D                         sSpawnPosition;
	int                             iLureDistance;
	int                             iTargetTraceMode;

	Point3D                         sMoveBeginPosition;
	DWORD                           iMoveBeginCounter;

	EAnimationType                  eNextAnimationType;

	BOOL                            bIsCharging;

	int								iTalkFrame;
	int								iTalkSoundIndex;

	int 							iStartFrame;
	int 							iEndFrame;
	EAnimationID					iAnimationID;
	int 							iFrame;
	int								iFrameObsolete;
	int 							iFPS;
	int                             iMovementSpeed;
	int                             iWeaponSpeed;

#if defined(_GAME)
	int								iLastMovementSpeed;
#endif

	EUpdateMode                     eUpdateMode;

	int	                            iaEventFrame[4];

	int                             iAliveCounter;
	int                             iDeathCounter;
	int                             iMoveCounter;
	short                           saMoveStepCount[2];

	int                             iLoopTimer;
	int                             iFrameTimer;

	BOOL                            bRendered;
	RECT                           	rRenderRectangle;
	Point3D                         sRenderPoint;
	BOOL                            bFlagShow;

	char                            szMessage[256];
	DWORD                           dwMessageEndTime;

	char                            szPersonalShop[128];
	int								iPersonalShop;


	BYTE                            baUnitInfo[0x1000];
	UINT                            uUnitInfoSize;

	int                             iServerCode;

	int                             iUnknown[7];

	MoveSequence					saMoveSequence[256];
	UINT                            iMoveSequenceCounter;
	UINT                            iMoveSequenceEnd;

	DWORD                           dwLastActiveTime;
	int                             iLastUnitInfoPacketType;

#if defined(_GAME)
	CharacterData					sCharacterData;
	UnitInfo						sUnitInfo;
#else
	CharacterData					sCharacterData;
	UnitInfo						sUnitInfo;
#endif

	int                             bAnimation;
	int                             iAnimationMode;
	int                             iFrameType;

	ModelData						* psBody;
	ModelData						* psHead;
	ModelAnimation					* psModelAnimation;

	ModelData						* psTalkModelData;
	ModelAnimation					* psTalkModelAnimation;

	EAnimationType					iOldAnimationType;
	ID								lTargetID;

	UserData                        * pcOwner;
	UserData                        * pcFocusTarget;
	UnitData                        * pcAttackTarget;

	BOOL                            bMonsterAttacking;

	DWORD                           dwIgnoreOthersTime;
	ID                           	lExclusiveTargetID;
	UserData                        * pcExclusiveTarget;

	UnitData						* pcExclusiveUnitTarget;

	UserData                        * pcLastAttacker;
	int                             iDamageLastAttacker;
	DWORD                          	dwDamageLastTime;

	PacketSkillDamageNormal			sLastDamagePacket;
	DWORD                           dwLastDamageTime;

	EPotionStatus                   iPotionStatus;
	int                             iLevelStatus;
	int                             iCriticalStatus;

	BOOL                            bShowHP;

	int                             iMotionRecordCount;
	int                             iSpawnPointIndex;

	int                             iTransparencySelf;
	int                             iTransparency;
	BOOL                            bRenderLast;

	int                           	eSkillDisplayFlag;
	ModelAnimation::EMapTypeFlag    eLastMapTypeFlag;

	BOOL                            bHideRightWeapon;
	BOOL                            bOverrideFPS;
	int                             iNewFPS;
	int                             iAnimationEvent;

	int                             iEnchantWeaponType;
	DWORD                           dwEnchantWeaponTimeOut;
	int                             iEnchantWeaponLevel;

	UserData                        * pcHookedTarget;
	DWORD                           dwHookedTargetEndTime;

	DWORD                           dwAssassinEyeEndTime;
	int 							iAssassinEyeValue;

	int                             iClanInfoIndex;
	DWORD                           dwClanUpdateTime;

	int                           	iBellatraCrown;
	BOOL                            bBlessCastleCrown;

	Map								* pcMap;
	UserData						* pcKillerData;
	void							* pcBellatraKiller;

	union
	{
		BOOL						bWearingTwoHandedWeapon;
		void						* pcMapSpawn;
	};
	Point3D                         sPetPosition;
	int                             iPetID;
	BOOL                            bAggressive;

	DWORD                           dwExclusiveNum;
	DWORD                           iBellatraRoom;

	int                             iFreezeTimeLeft;
	int                             iUpdateMoveReduce;
	int                             iStunTimeLeft;
	int                             iStopMotionCounter;
	MinMax							sHolyMind;
	MinMax                          sSuccubus;
	MinMax                          sPoison;
	MinMax                          sHolyIncantation;

	int                             iVanishTimeLeft;
	int                             bBitterOrdeal;
	BOOL                            bNoShadow;
	BOOL                            bDistortion;
	BOOL                            bNoDeath;

#if defined(_GAME)
	int                             iBlessHealLimit;
	int                             iRivera;
	int                             iWisp;
	int                             iBlindSkill;
	int                             iCurseLazy;
#endif

	BOOL							bCheckFlagIce;

	int                             iPoison;

	DWORD                           dwForceOfNatureTimeOut;
#if defined(_GAME)
	DWORD                           dwHallOfValhallaTimeOut;


	DWORD                           dwAdventMigalTime;
	DWORD                           dwAdventMidrandaTime;
#endif

	int                             iAreaDamageArea;

	int                             iHammerAttackCount;
	DWORD                           lHammerUserID;

	short                           sAddColorEffect[4];
	int                             iCurHPBar;

	DWORD                           dwLastUpdateTime;
	DWORD                           dwLastRecvAttackTime;

	int                             iTryAttackCount;

#if defined(_GAME)
	EItemID							eForceOrbItemID;
#else
	BOOL							bNoCountInSpawn;
#endif

	DWORD							dwForceOrbTimeOut;

#if defined(_GAME)
	DWORD							dwLifeBoosterCode;
	DWORD                           dwLifeBoosterTime;

	DWORD                           dwManaBoosterCode;
	DWORD                           dwManaBoosterTime;

	DWORD                           dwStaminaBoosterCode;
	DWORD                           dwStaminaBoosterTime;

	BOOL							bAddFrameUnit;

	UINT							uMovementFrameCount;

	int								iPadding[25];
#endif

	ECharacterType                  iCharacterType;
	int                             iSpecialType;			//if 2 then angle = 0
	ID		                        iClanID;

	int                             iHPBlocks;
	CurMax                          sHPBlocks;
	CurMax                          sVirtualHP;

#if defined(_GAME)
	int								iUnknown70;
#endif

	short                           sDamageBC;
	short							sPVPKills;
	AttackDamageData                * psaDamageUsersData;
	AttackDamageData                * psaSiegeWarDataList;

#if defined(_GAME)
	int								iPlaySeal;

	int								iPlayBlind;

	void							* pcaUnitDataTargetList;

	int								iPadding13[185];
#else
	UnitData					  * pcFocusingTarget;
	int								iFocusingLastID;

	int								iPadding13[178];
#endif

	UnitData();
	~UnitData();

	ECharacterGender				GetCharacterGender();
	EAnimationType					GetAnimation() { return psModelAnimation ? psModelAnimation->iType : ANIMATIONTYPE_None; }
	BOOL							IsDead() { return (GetAnimation() == ANIMATIONTYPE_Die); }
	BOOL							IsAttackAnimation() { return ((GetAnimation() == ANIMATIONTYPE_Attack) || (GetAnimation() == ANIMATIONTYPE_Skill)); }
	BOOL							IsLowAnimationType() { return (GetAnimation() < ANIMATIONTYPE_Attack); }

    BOOL                            IsPlayer( bool bVerifyPvP = true );

	char							* GetName() { return this->sCharacterData.szName; };

	void							Main();
	void							Init();
	void							Free();

	BOOL							Move( int iDistance );

	void							SetPosition( int iX, int iY, int iZ, int iAX = 0, int iAY = 0, int iAZ = 0 );
	BOOL							Animate( EAnimationType eAnimationType );

	BOOL							ChangeAnimationID( int iID, int iModelType = 0 );

	int								HandleUnitDataBuffer( char * pBuffer, int iDelayTime, int iSleep );
	BOOL							NextTarget( int iX, int iY, int iZ );

	BOOL							SetTarget( int iX, int iZ );
	BOOL							SetTargetEx( int iX, int iY, int iZ );

	int								SetAngleByPosition( int iX, int iZ );

	BOOL							GetAnimationAttackPosition( int * iX, int * iY, int * iZ );

	void							LoadModel( char * pszModelFilePath );

	void							UpdateAttack( BOOL bUpdateTime = FALSE );

	BOOL							Render();

	void							SetHeadModelData( EXEModelData * psModelData );
	void							SetBodyModelData( EXEModelData * psModelData );


}; //Size = 0x4D98

class Unit
{
private:
	static void									Init( Unit * pcUnit, UnitData * pcUnitData );
public:

	UINT										uIndex;

	UnitData									* pcUnitData = NULL;
	int											iID;

	UINT										uLastUpdate;

	IMinMax										sHPLong;

	Point3D										sLastRunningPosition;
	DWORD										dwTimePositionUpdate;

	//Skills T5
	BOOL										bFierceWind;
	BOOL										bLethalSight;
	BOOL										bHolyConviction;
	BOOL										bWizardTrance;
	bool										bWhirlWind;
	bool										bMagneticDischarge;
	bool										bKissViper;

	int											iInertiaCount;

	//Items Element
	UINT										uElementWeaponID;

	//Party
	bool										bPartyLeader;

	BYTE										baUnitBufferNew[0x2000];

	std::vector<CAchievementUser*>				vAchievement;

	bool										bFollowUnitType;

	bool										bCaravanFollow;

	int											iCharacterXmasTreeID;

    int                                         iPvPKills;
    int                                         iPvPExperience;

    UnitData                                    * pcUnitDataMount = nullptr;

	bool										bSpecialEffect = false;

#if defined(_GAME)
	float										fAnimationBlendingTime;

	std::vector<Delta3D::Graphics::MeshAnimationDataOld> vMeshesAnimation;
#endif

	bool										bPvPMode;

	int											iMountType;

	void										Load();

	void										Load( UnitData * pcUnitData );

	Point3D										& GetPosition() { return pcUnitData->sPosition; }
	Point3D										& GetAngle() { return pcUnitData->sAngle; }

	int											GetID() { return iID; }

	char										* GetName() { return pcUnitData->sCharacterData.szName; }

	EMonsterEffectID							GetMonsterEffectID() { return pcUnitData->sCharacterData.iMonsterEffectID; }

	static UINT									GetArrayPosition( UnitData * pcUnitData );
	
	EMapID										GetMapID();
};

static const std::string FixUnitDataName( const std::string strName )
{
	std::string strNameFix = strName;

	if ( strNameFix.find( "_" ) != std::string::npos )
		strNameFix.erase( strNameFix.begin() + strNameFix.find( "_" ), strNameFix.end() );

	return strNameFix;
}


static bool FixUnitDataName( UnitData * pcUnitData )
{
	STRINGCOPY( pcUnitData->sCharacterData.szName, FixUnitDataName( pcUnitData->sCharacterData.szName ).c_str() );
	return true;
}


struct MonsterMapSpawnHandle
{
	CharacterData	sCharacterData;
	int				iPositionState;

	BOOL			bFixed;

	Point3D			sPosition;
};

struct PacketUnitStatusMove : Packet
{
	ID					  lID;
	Point3D				  sPosition;
	short				  sAngleX;
	short				  sAngleY;
	short				  sAngleZ;
	short				  sAnimationType;
	int					  iFrame;
	DWORD				  dwHashCharacterName;
}; //Size = 0x28

struct PacketGameStatusBase : Packet
{
	int					  iStatusID;
	ID					  lID;
};

struct PacketGameStatusBabelStormMap : PacketGameStatusBase
{
	EMapID				  eBabelStormMapID;
};

struct PacketGameStatusBlessCastle : PacketGameStatusBase
{
	DWORD				  dwNum1;
	DWORD				  dwNum2;
	DWORD				  dwNum3;
	DWORD				  dwNum4;
	short				  sConRate;
	short				  sBattlePoints;
	DWORD				  dwNum6;
};

struct PacketGameStatusPvPMap : PacketGameStatusBase
{
	EMapID				  ePvPMapID;
};

struct PacketUnitStatusContainer : Packet
{
	int					  iAmount;
	int					  iCounter;

	BYTE				  baBuffer[MAX_PKTSIZ - 0x10];
};
