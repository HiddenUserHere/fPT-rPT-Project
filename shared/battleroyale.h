#pragma once

#include "user.h"
#include "character.h"
#include "item.h"

#define MAX_BATTLEROYALE_ITEMDROP		4
#define MAX_BATTLEROYALE_ITEMSPAWN		250
#define MAX_BATTLEROYALE_DROPTIME		(2 * 60 * 1000)

#define MAX_BATTLEROYALE_TIMEDEATH		5

#define MAX_BATTLEROYALE_PLAYERS		60
#define MAX_BATTLEROYALE_WAITTIME		(5 * 60 * 1000)
#define MAX_BATTLEROYALE_WAITTIMEAFTER	(MAX_BATTLEROYALE_WAITTIME + 11000)
#define MAX_BATTLEROYALE_WAITPVP		(5 * 60 * 1000)
#define MAX_BATTLEROYALE_WAITPVPAFTER	(MAX_BATTLEROYALE_WAITPVP + 11000)
#define MAX_BATTLEROYALE_AFTERBATTLE	(60 * 1000)
#define MAX_BATTLEROYALE_ENDBATTLE		(5 * 1000)

#define MAX_BATTLEROYALE_RADIUS			10000
#define RADIUS_BATTLEROYALE_NEXT		75
#define TIME_BATTLEROYALE_METEORITE		(20 * 1000)
#define RADIUS_BATTLEROYALE_METEORITE	700

#define MAX_BATTLEROYALE_PLAYERTEAM		100

#define MAX_BATTLEROYALE_SPAWNCOORDS	256

enum EBattleRoyaleID : int
{
	BATTLEROYALE_None,
	BATTLEROYALE_Normal,
	BATTLEROYALE_PvPFast,
	BATTLEROYALE_TeamRoyal,
	BATTLEROYALE_TeamRoyalTicketKills,
	BATTLEROYALE_TeamRoyalOasis,
};


namespace BattleRoyale
{

enum EGameState : int
{
	GAMESTATE_None,
	GAMESTATE_WaitingForPlayers,
	GAMESTATE_WaitingPvP,
	GAMESTATE_InBattle = 500,
	GAMESTATE_End = 600,
};

enum EPlayerTeam : int
{
	PLAYERTEAM_None,
	PLAYERTEAM_Red,
	PLAYERTEAM_Green,
	PLAYERTEAM_Blue,
	PLAYERTEAM_White,

	PLAYERTEAM_Party = 500,

	PLAYERTEAM_Solo = 1500,
};

struct TeamsAliveData
{
	EPlayerTeam		eTeamID;

	int				iKills;

	int				iCount;
};

static std::string BroyaleTeamToString( EPlayerTeam eTeamID )
{
	std::string str = "None";

	switch ( eTeamID )
	{
		case PLAYERTEAM_Red:
			str = "Red";
			break;
		case PLAYERTEAM_Green:
			str = "Green";
			break;
		case PLAYERTEAM_Blue:
			str = "Blue";
			break;
		case PLAYERTEAM_White:
			str = "White";
			break;
	}

	return str;
}

#define ENC_BATTLEROYALE_CHARACTERID		0x708950

struct PlayerTeam
{
	int				iID;
	EPlayerTeam		eTeamID;
	int				iKills;

	int				iEncryptedCharacterID; //0x78950
};


struct State
{
	int					iBattleID = BATTLEROYALE_None;

	EGameState			eGameState = GAMESTATE_None;

	int					iKillsNeeded = 0;

	int					iPlayersCount;
	int					iPlayersMax;

	int					iEndTime	= 0;
	int					iTimeUpdate = 0;
	int					iDuration	= 0;

	bool				bCountdown		= false;
	int					iCountdownEnd	= 0;

	int					iRadius		= 0;
	int					iNextRadius = 0;
	Point3D				sPosition;
	
	int					iMeteoriteTime = 0;
	Point3D				sMeteoritePosition;

	bool				bTeam;

	int					iPlayerTeamCount;
	PlayerTeam			saPlayerTeams[MAX_BATTLEROYALE_PLAYERTEAM];

	EPlayerTeam			eaTeamID[2];
	int					iaTeamKills[2];
};

enum EPlayerState : int
{
	PLAYERSTATE_None,
	PLAYERSTATE_Alive,
	PLAYERSTATE_Dead,
	PLAYERSTATE_Disconnected,
};

enum EAnnouncementType : int
{
	ANNOUNCEMENTTYPE_None,
	ANNOUNCEMENTTYPE_15Minutes,
	ANNOUNCEMENTTYPE_10Minutes,
	ANNOUNCEMENTTYPE_5Minutes,
};

enum EHandicapType : int
{
	HANDICAPTYPE_None,
	HANDICAPTYPE_AttackRating,
	HANDICAPTYPE_Damage,
};

struct PlayerState
{
	int					iID;
	int					iCharacterID;

	EPlayerState		eState;

	int					iTimeDeath;

	int					iKills;

	int					iDamageDealt;
	int					iDamageReceived;

	EPlayerTeam			eTeamID;
};

struct BattleTimes
{
	int iDayWeek;

	std::vector<std::pair<int, int>> vTimes;

	BattleTimes() {};
	~BattleTimes() { vTimes.clear(); };

	void Add( int iHour, int iMinute ) { vTimes.push_back( std::make_pair( iHour, iMinute ) ); };
};

struct BattleHandicap
{
	ECharacterClass		iCharacterClass;
	float				fAttackRating;
	float				fDamage;

	BOOL				bItem;

	BattleHandicap( ECharacterClass	_iCharacterClass, float _fAttackRating, float _fDamage, BOOL _bItem ) 
	{
		iCharacterClass = _iCharacterClass;
		fAttackRating = _fAttackRating;
		fDamage = _fDamage;
		bItem = _bItem;
	};

	~BattleHandicap() {};
};

struct SpawnCoordinate
{
	int					iNorth;
	int					iSouth;

	int					iWest;
	int					iEast;

	SpawnCoordinate( int _iNorth, int _iSouth, int _iWest, int _iEast )
	{
		iNorth = _iNorth;
		iSouth = _iSouth;

		iWest = _iWest;
		iEast = _iEast;
	};

	SpawnCoordinate() {};

	~SpawnCoordinate() {};
};

static int ComputeBattleRoyaleRadius( int iTimeUpdate, int iDuration )
{
	return (int)((((INT64)iDuration - (INT64)iTimeUpdate) * MAX_BATTLEROYALE_RADIUS) / (INT64)(iDuration > 0 ? iDuration : 1));
}

static int ComputeBattleRoyaleNextTime( int iDuration, int iRadius, int iNextRadius )
{
	INT64 iDifference = (MAX_BATTLEROYALE_RADIUS * 1000) / iDuration;
	return (int)((INT64)((INT64)iRadius - (INT64)iNextRadius) / iDifference);
}

static BOOL IsBattleRoyaleInsideRadius( Point3D sPosition, Point3D sComparePosition, int iRadius )
{
	sPosition.iY = 0;

	int iDistanceA, iDistanceB, iDistanceC, iDistanceD;
	sPosition.GetPTDistanceXZY( &sComparePosition, iDistanceA, iDistanceB, iDistanceC, iDistanceD );
	if ( (INT64)iDistanceA >= ((INT64)iRadius * (INT64)iRadius) )
		return FALSE;

	return TRUE;
}

};

struct PacketBattleRoyaleUpdate : Packet
{
	BattleRoyale::State			sState;
};

struct PacketBattleRoyaleBattleTime : Packet
{
	PacketBattleRoyaleBattleTime( BOOL _bStart, BOOL _bRemoveItems = FALSE ) { bStart = _bStart; bRemoveItems = _bRemoveItems; };
	~PacketBattleRoyaleBattleTime() {};

	BOOL					bStart;
	BOOL					bRemoveItems;
};

struct PacketBattleRoyaleGasDeath : Packet
{
	BOOL					bMeteorite;

	int						iTime;
};

struct PacketNetBattleRoyaleStatus : Packet
{
	int						iID;
	bool					bInBattleRoyale;
};

struct PacketNetBattleRoyaleEnter : Packet
{
	int						iID;

	char					szCharacterName[32];

	PacketNetBattleRoyaleEnter() {};
	PacketNetBattleRoyaleEnter( int _iID ) { iID = _iID; };
	~PacketNetBattleRoyaleEnter() {};
};

struct PacketBattleRoyaleRequest : Packet
{
	int						iInstanceID;
	int						iBattleID;

	BOOL					bReward;

	int						iRewardID;

	BOOL					bGiveGoldEXP;

	PacketBattleRoyaleRequest( int _iInstanceID, int _iBattleID, BOOL _bReward, int _iRewardID = -1, BOOL _bGiveGoldEXP = FALSE )
	{ 
		iInstanceID		= _iInstanceID; 
		iBattleID		= _iBattleID; 
		bReward			= _bReward; 
		iRewardID		= _iRewardID;
		bGiveGoldEXP	= _bGiveGoldEXP;
	};

	~PacketBattleRoyaleRequest() {};
};

#define MAX_BATTLEROYALELIST		32

struct PacketBattleRoyaleList : Packet
{
	struct BattleRoyaleList
	{
		int					iID;
		char				szName[32];
		EMapID				iMapID;

		IMinMax				sRequiredLevel;

		bool				bOpen;

		INT64					iTimeLeft;
	};

	int						iCount;

	BattleRoyaleList		saList[MAX_BATTLEROYALELIST];
};

#define MAX_BATTLEROYALEREWARD		12

struct PacketBattleRoyaleRewardList : Packet
{
	struct BattleRoyaleReward
	{
		int					iID;
		char				szName[32];
		char				szImagePath[260];

		int					iChance;
	};

	int						iInstanceID;
	int						iBattleID;

	MinMax					sPacketCount;

	int						iCount;

	BattleRoyaleReward		saRewards[MAX_BATTLEROYALEREWARD];

	bool					IsFirst() { return sPacketCount.sMin == 1 ? true : false; }
	bool					IsOnly() { return sPacketCount.sMin == sPacketCount.sMax ? true : false; }
};

struct PacketBattleRoyaleResponse : Packet
{
	int						iRetCode;

	PacketBattleRoyaleResponse( int iRet ) { iRetCode = iRet; };
	~PacketBattleRoyaleResponse() {};
};

struct PacketBattleRoyaleTeleports : Packet
{
	int								iCount;
	BattleRoyale::SpawnCoordinate	saCoordinates[MAX_BATTLEROYALE_SPAWNCOORDS];
};

class CBattleRoyale
{
private:
	int							iID;

	EMapID						eMapID;
	std::string					strName;
	IMinMax						sRequiredUsers;
	IMinMax						sRequiredLevel;
	std::vector<BattleRoyale::BattleTimes*>	vTimes;

	std::vector<std::vector<DropSettingsData*>> vDrops;
	int							iTotalChance;

	Point3D						sWaitingPosition;

	int							iDuration;

	BattleRoyale::EAnnouncementType			eAnnouncement;

	int							iKillsNeeded;

	std::vector<BattleRoyale::SpawnCoordinate *> vSpawnCoordinates;

	std::vector<BattleRoyale::BattleHandicap*> vBattleHandicaps;

public:

	CBattleRoyale();
	CBattleRoyale( int _iID, const std::string _strName, EMapID _eMapID );
	~CBattleRoyale();

	int							GetID() { return iID; };
	void						SetID( int i ) { iID = i; }

	std::string					GetName() { return strName; }
	void						SetName( const std::string _strName ) { strName = _strName; }
	IMinMax						& GetRequiredUsers() { return sRequiredUsers; }
	IMinMax						& GetRequiredLevel() { return sRequiredLevel; }

	std::vector<std::vector<DropSettingsData*>> & GetDrops() { return vDrops; }
	int							GetTotalDropChance() { return iTotalChance; }
	void						SetTotalDropChance( int i ) { iTotalChance = i; }

	std::vector<BattleRoyale::SpawnCoordinate *> & GetCoordinates() { return vSpawnCoordinates; };

	std::vector<BattleRoyale::BattleHandicap*> & GetBattleHandicap() { return vBattleHandicaps; };

	Point3D						GetWaitingPosition() { return sWaitingPosition; }
	void						SetWaitingPosition( Point3D sPosition ) { sWaitingPosition = sPosition; }

	EMapID						GetMapID() { return eMapID; }
	void						SetMapID( EMapID _eMapID ) { eMapID = _eMapID; }
	void						AddTime( BattleRoyale::BattleTimes * psBattleTimes );

	bool						IsTime( int iDayWeek, int iHour, int iMinute, int iLeftMinute = 0 );

	INT64						GetTimeLeft( SYSTEMTIME sServerTime );

	int							GetDuration() { return iDuration; }
	void						SetDuration( int i ) { iDuration = i; }

	int							GetKillsNeeded() { return iKillsNeeded; }
	void						SetKillsNeeded( int i ) { iKillsNeeded = i; }

	BattleRoyale::EAnnouncementType		GetAnnouncement() { return eAnnouncement; }
	void								SetAnnouncement( BattleRoyale::EAnnouncementType eType ) { eAnnouncement = eType; }

	static inline bool			IsTeamRoyal( int iBattleID )
	{
		return ((iBattleID == BATTLEROYALE_TeamRoyal) || (iBattleID == BATTLEROYALE_TeamRoyalTicketKills) || (iBattleID == BATTLEROYALE_TeamRoyalOasis));
	};

	static inline bool			IsTeamRoyalTickets( int iBattleID )
	{
		return ((iBattleID == BATTLEROYALE_TeamRoyalTicketKills) || (iBattleID == BATTLEROYALE_TeamRoyalOasis));
	};

	inline bool			IsTeamRoyal()
	{
		return CBattleRoyale::IsTeamRoyal( GetID() );
	};

	inline bool			IsTeamRoyalTickets()
	{
		return CBattleRoyale::IsTeamRoyalTickets( GetID() );
	};
};
