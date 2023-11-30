#pragma once

#include "user.h"
#include "character.h"
#include "item.h"

#define MAX_BELLATRA_PLAYERS		10
#define MAX_BELLATRA_ENDLASTROUND	3
#define MAX_BELLATRA_LEAVELASTROUND	(MAX_BELLATRA_ENDLASTROUND + 1)


namespace SOD
{
struct BellatraMonster
{
	int						iID;
	char					szName[32];

	int						iChance;
};

struct BellatraArea
{
	int						iRoomID;

	Point3D					sPosition;

	int						iNorth;
	int						iSouth;

	int						iEast;
	int						iWest;

	BellatraArea() {};

	BellatraArea( int _iRoomID, int _iX, int _iZ, int _iNorth, int _iSouth, int _iEast, int _iWest )
	{
		iRoomID = _iRoomID;

		sPosition.iX = _iX;
		sPosition.iY = 0;
		sPosition.iZ = _iZ;

		iNorth = _iNorth;
		iSouth = _iSouth;
		iEast = _iEast;
		iWest = _iWest;
	};

	~BellatraArea() {};
};



struct BellatraItemDrop
{
	char					szItemCode[32];
	int						iChance;

	BellatraItemDrop() {};
	BellatraItemDrop( char _szItemCode[32], int _Chance )
	{
		STRINGCOPY( szItemCode, _szItemCode );
		iChance = _Chance;
	};

	~BellatraItemDrop() {};
};

#define MAX_BELLATRA_ITEMS			9

enum ESealType : int
{
	SEALTYPE_Invalid = -1,
	SEALTYPE_Quake,
	SEALTYPE_Stun,
	SEALTYPE_IceFreeze,
	SEALTYPE_Rabie,
	SEALTYPE_Stygian,
	SEALTYPE_GuardianSaint,
	SEALTYPE_Score,
	SEALTYPE_NewItem1,
	SEALTYPE_NewItem2,

	SEALTYPE_Count,
};

struct BellatraPlayer
{
	bool					bActive;

	int						iID;
	int						iCharacterID;

	bool					bInvalid;

	int						iScore;

	int						iKills;

	int						iaBellatraItemCount[ESealType::SEALTYPE_Count];
};

struct BellatraTeam
{
	bool					bActive;

	bool					bInvalid;
	bool					bKick;

	int						iID;

	int						iRoomID;
	int						iAreaID;

	int						iKills;
	int						iScore;

	int						iUsersCount;
	int						iSuccessUsersCount;
	int						iFailUsersCount;

	int						iMonstersAlive;

	bool					bBoss;

	BOOL					bInParty;
	BOOL					bIsClan;

	BellatraPlayer			saPlayers[MAX_BELLATRA_PLAYERS];
};

class CBellatraRound
{
private:
	int						iID;
	UINT					iRoundNumber;

	int						iGameTime;

	int						iMinimumScore;

	int						iMaxTeams;

	int						iStartDelay;

	int						iMaxSpawnMonsters;

	std::vector<BellatraMonster *> vMonsters;
	int						iTotalMonstersChance;

	std::vector<BellatraMonster *> vBosses;
	int						iTotalBossesChance;

	std::vector<BellatraItemDrop *> vItemsDrop;
	int						iTotalItemsDropChance;

	EMapID					eMapID;

	std::vector<BellatraArea *>	vAreas;

	int						iDivScore;

public:
	CBellatraRound();
	~CBellatraRound();

	int						GetID() { return iID; }
	void					SetID( int _iID ) { iID = _iID; }

	UINT					GetRound() { return iRoundNumber; }
	void					SetRound( UINT _iRoundNumber ) { iRoundNumber = _iRoundNumber; }

	int						GetGameTime() { return iGameTime; }
	void					SetGameTime( int _iGameTime ) { iGameTime = _iGameTime; }

	int						GetMinimumScore() { return iMinimumScore; }
	void					SetMinimumScore( int _iMinimumScore ) { iMinimumScore = _iMinimumScore; }

	int						GetMaxTeams() { return iMaxTeams; }
	void					SetMaxTeams( int _iMaxTeams ) { iMaxTeams = _iMaxTeams; }

	int						GetDelay() { return iStartDelay; }
	void					SetDelay( int _iStartDelay ) { iStartDelay = _iStartDelay; }

	int						GetMaxSpawnMonsters() { return iMaxSpawnMonsters; }
	void					SetMaxSpawnMonsters( int _iMaxSpawnMonsters ) { iMaxSpawnMonsters = _iMaxSpawnMonsters; }

	std::vector<BellatraMonster *> & GetMonsters() { return vMonsters; }
	int						GetTotalMonstersChance() { return iTotalMonstersChance; }
	void					SetTotalMonstersChance( int _iTotalMonstersChance ) { iTotalMonstersChance = _iTotalMonstersChance; }

	std::vector<BellatraMonster *> & GetBosses() { return vBosses; }
	int						GetTotalBossesChance() { return iTotalBossesChance; }
	void					SetTotalBossesChance( int _iTotalBossesChance ) { iTotalBossesChance = _iTotalBossesChance; }

	std::vector<BellatraItemDrop *> & GetItemsDrop() { return vItemsDrop; }
	int						GetTotalItemsDropChance() { return iTotalItemsDropChance; }
	void					SetTotalItemsDropChance( int _iTotalItemsDropChance ) { iTotalItemsDropChance = _iTotalItemsDropChance; }

	std::vector<BellatraArea *> & GetAreas() { return vAreas; }

	EMapID					GetMapID() { return eMapID; }
	void					SetMapID( EMapID _eMapID ) { eMapID = _eMapID; }

	int						GetDivScore() { return iDivScore; }
	void					SetDivScore( int i ) { iDivScore = i; }
};

class CBellatra
{
private:
	int						iID;
	std::string				strName;

	int						iEntranceTime;

	std::vector<CBellatraRound *> vRounds;

	int						iKilledMonstersBoss;

	IMinMax					sRequiredLevel;

	EMapID					eLeaveMapID;

	Point3D					sLeavePosition;

	BOOL					bIsRanked;

public:
	CBellatra();
	~CBellatra();

	int						GetID() { return iID; }
	void					SetID( int _iID ) { iID = _iID; }

	std::string				GetName() { return strName; }
	void					SetName( const std::string _strName ) { strName = _strName; }

	int						GetEntranceTime() { return iEntranceTime; }
	void					SetEntranceTime( int _iEntranceTime ) { iEntranceTime = _iEntranceTime; }

	std::vector<CBellatraRound *> & GetRounds() { return vRounds; }

	int						GetKilledMonstersBoss() { return iKilledMonstersBoss; }
	void					SetKilledMonstersBoss( int _iKilledMonstersBoss ) { iKilledMonstersBoss = _iKilledMonstersBoss; }

	IMinMax					& GetRequiredLevel() { return sRequiredLevel; }

	EMapID					GetLeaveMapID() { return eLeaveMapID; }
	void					SetLeaveMapID( EMapID _eLeaveMapID ) { eLeaveMapID = _eLeaveMapID; }

	Point3D					& GetLeavePosition() { return sLeavePosition; }

	BOOL					IsRanked() { return bIsRanked; }
	void					SetIsRanked( BOOL _bIsRanked ) { bIsRanked = _bIsRanked; }
};
};

struct PacketBellatraInformation : Packet
{
	enum EBellatraInformationType
	{
		BELLATRAINFOTYPE_None,
		BELLATRAINFOTYPE_StageComplete,
		BELLATRAINFOTYPE_UpdateRound,
		BELLATRAINFOTYPE_StageFail,
		BELLATRAINFOTYPE_GoldYahoo,
		BELLATRAINFOTYPE_Effect = 100,
	};

	int					iRound;
	EBellatraInformationType	iType;
	union
	{
		int				iValue1;
		int				iRoomID;
	};
	int					iValue2;
};


struct PacketBellatraScore : PacketBellatraInformation
{
	int				iScore;
	int				iTeamRoomID;

	int				iaScoreTeams[4];

	int				iaPadding001[4];
};

struct PacketBellatraEffectCast : Packet
{
	int				iID;
	int				iBellatraCastID;
	int				iBellatraSealID;
	int				iPadding001 = 0;
};

#define MAX_PACKET_BELLATRA_RESULT		32

struct PacketBellatraRoomScore : Packet
{
	struct BellatraResult
	{
		char			szCharacterName[32];
		UserData		* pcUserData; //Unused
		ECharacterClass	iCharacterClass;
		ECharacterRank	iCharacterRank;

		int				iScore;
		
		int				iKills;

		int				iaPadding001[4];
	};

	UINT				uGameCode;
	UINT				uGameTime;
	BOOL				bShowScore;

	short				saTeamRound[2];

	int					iaPadding002[15];

	int					iResultCount;

	BellatraResult		saResult[MAX_PACKET_BELLATRA_RESULT];
};
