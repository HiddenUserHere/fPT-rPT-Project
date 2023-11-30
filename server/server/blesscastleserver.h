#pragma once

#define MAX_GOLD_PERBOX						400000000

#define	MAX_SOLDIER_BLESSCASTLE				20

static const Point3D saBlessCastleTowerPosition[] =
{
	{ 34224 , -25857 , 646 },
	{ 34891 , -25857 , 646 },
	{ 33755 , -28297 , 686 },
	{ 35351 , -28297 , 686 },
	{ 34151 , -30313 , 754 },
	{ 34952 , -30313 , 754 },
	{ 0,0,0 }
};

static const Point3D saCastleGuardPositionA[MAX_SOLDIER_BLESSCASTLE] = 
{
	{ 33251, -27968, 688 },
	{ 33339, -27968, 688 },
	{ 33428, -27968, 688 },
	{ 33516, -27968, 688 },
	{ 33605, -27968, 688 },

	{ 33251, -28052, 688 },
	{ 33339, -28052, 688 },
	{ 33428, -28052, 688 },
	{ 33516, -28052, 688 },
	{ 33605, -28052, 688 },


	{ 35497, -27968, 688 },
	{ 35586, -27968, 688 },
	{ 35674, -27968, 688 },
	{ 35763, -27968, 688 },
	{ 35851, -27968, 688 },

	{ 35497, -28052, 688 },
	{ 35586, -28052, 688 },
	{ 35674, -28052, 688 },
	{ 35763, -28052, 688 },
	{ 35851, -28052, 688 }
};


static const Point3D saCastleGuardPositionB[MAX_SOLDIER_BLESSCASTLE] =
{
	{ 33590, -29534, 706 },
	{ 33679, -29534, 706 },
	{ 33767, -29534, 706 },
	{ 33856, -29534, 706 },
	{ 33945, -29534, 706 },

	{ 33590, -29618, 706 },
	{ 33679, -29618, 706 },
	{ 33767, -29618, 706 },
	{ 33856, -29618, 706 },
	{ 33945, -29618, 706 },


	{ 35159, -29534, 706 },
	{ 35248, -29534, 706 },
	{ 35336, -29534, 706 },
	{ 35425, -29534, 706 },
	{ 35513, -29534, 706 },

	{ 35159, -29618, 706 },
	{ 35248, -29618, 706 },
	{ 35336, -29618, 706 },
	{ 35425, -29618, 706 },
	{ 35513, -29618, 706 }
};


static const Point3D saCastleGuardPositionC[MAX_SOLDIER_BLESSCASTLE] =
{
	{ 33898, -30882, 754 },
	{ 33987, -30882, 754 },
	{ 34075, -30882, 754 },
	{ 34164, -30882, 754 },
	{ 34252, -30882, 754 },

	{ 33898, -30967, 754 },
	{ 33987, -30967, 754 },
	{ 34075, -30967, 754 },
	{ 34164, -30967, 754 },
	{ 34252, -30967, 754 },


	{ 34852, -30882, 754 },
	{ 34941, -30882, 754 },
	{ 35029, -30882, 754 },
	{ 35118, -30882, 754 },
	{ 35206, -30882, 754 },

	{ 34852, -30967, 754 },
	{ 34941, -30967, 754 },
	{ 35029, -30967, 754 },
	{ 35118, -30967, 754 },
	{ 35206, -30967, 754 }
};



static const Point3D sCastleValhallaPosition	= { 34559, -31375, 800 };

static const Point3D sCastleDoorPosition		= { 34552, -25545, 680 };

class BlessCastleServer
{
protected:
	BOOL												bReadSQL = FALSE;

	DWORD												dwTickUpdateUsersScore = 0;

	UnitData											* pcBlessCastleTower = nullptr;

public:
	BlessCastleServer();
	virtual ~BlessCastleServer();

	struct BlessCastlePVPLeague							* psBlessCastlePVPLeague = NULL;
	struct BlessCastleStatusData						* psBlessCastleStatusData = NULL;

	void												NetSendBlessCastleStatus( struct NetConnection * psNetConnection = NULL );

	void												SQLReadBlessCastleStatus();
	void												SQLUpdateUserScore( User * pcUser );

	bool												HasCrystalsTowersAlive();

	BOOL												SQLWriteBlessCastleStatus();

	void												NetRecvBlessCastleStatus( struct PacketNetBCStatus * psPacket );

	void												SetBlessCastleOwnerClanID( DWORD iClanID );

	void												NetSendBlessCastleOwnerClanID( DWORD iClanID );

	void												UpdateIndividualScores( BOOL bReset = FALSE );

	void												HandleIndividualScore();

	void												SendIndividualScore();

	BOOL												Main();

	void												SendUserScore( User * pcUser );

	static int											GetClanCodeByID( int iID );

	ESiegeWarMode										GetSiegeWarMode();
	void												SetSiegeWarMode( ESiegeWarMode iNewMode );

	BOOL												Start();
	BOOL												End( BOOL bUnitsAlive );

	static BOOL											EasySiegeWar();

	void												ResetUserScore();

	BOOL												OnUnitKilled( UnitData * pcUnitData );

private:
	void												EndBattle();

	static void											GetTop10ClanDamage( UnitData * pcUnitData, PacketBlessCastleTopData * psPacket );

	static void											SendBlessCastleDataToLoginServer( UserData * pcUserData );

	static void											MainFountain();

	void												KillBuffs();

	BOOL												GetClanLeaderAccount( int iClanID, char szLeaderAccount[32] );
	BOOL												GetClanName( int iClanID, char szName[32] );

	BOOL												CanShowScore();

};

