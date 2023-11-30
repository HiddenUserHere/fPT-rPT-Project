#pragma once

#define MAX_PVPCLANSCORES 1000

struct PvPClanScore
{
    int		iClanID;
    int		iExperience;
    int		iKills;
    int		iDeaths;
    int		iStreak;
};

class PvPServer
{
private:
	DWORD												dwTick = 0;
	WORD												sLastDay = 0;

	struct PvPClassData
	{
		int     							            iaCharacterID[10];

        char                                            szCharacterName[10][32];
	};

    PvPClanScore			                            saPvPClanScore[MAX_PVPCLANSCORES];

	void												SQLKillLog( User * pcKiller, User * pcVictim, EPvPKillType iKillType );

public:
	PvPServer();
	virtual ~PvPServer();

    void                                                Init();

    void                                                Tick( ETickType iTickType );

	BOOL												SQLGetTopPvP( PvPClassData & s );
	BOOL												SQLSetTopPvP( PvPClassData & s );

	void												ResetPvPBuffer();

	void												HandlePvPBuffers( bool bNotifyReset, bool bAddBuff );

	void												Update();

	BOOL												SQLSelectCharacterPvP( User * pcUser );
	BOOL												IsTopPVP( User * pcUser );
	void												SetTopPVP( User * pcUser, BOOL bPvP );

	void												ProcessPvPData( User * pcUser, int iID, BOOL bKill );
	void												ProcessPvPKill( User * pcKiller, User * pcVictim, int iType );

	BOOL												OnLoadUser( User * pcUser );
	BOOL												OnUnLoadUser( User * pcUser, bool bDateChange = true );

	void												OnKilled( User * pcKiller, User * pcVictim );

	void												ResetTick() { dwTick = 0; sLastDay = 0; }

	void												SetPvPBuffs( std::vector<int> vID );

    int						                            CalculateExperience( User * pcKiller, int iKillerLevel, ECharacterClass iKillerClass, User * pcVictim, int iVictimLevel, ECharacterClass iVictimClass );
    int						                            CalculateBaseExperience( int iBaseExp, BOOL bPositiveExp, BOOL bHigherExp, BOOL bPositiveAddedExp );

    void					                            ProcessClanKill( User * pcKiller, User * pcVictim, int iExperience );
    int						                            GetPvPClanScoreIndex( int iClanID );
    inline PvPClanScore                                 * GetPvPClanScore( int iIndex ) { return &saPvPClanScore[iIndex]; }

    void					                            SQLLoadPvPClanScores();
    void					                            SQLSavePvPClanScores();
};

