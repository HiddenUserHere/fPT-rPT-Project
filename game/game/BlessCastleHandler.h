#pragma once

struct ClanData
{
	BOOL					bFlag;
	int						iScore;
	int						iClanIndex;
	char					szClanName[40];

	int						iClanID;
	EXESurfaceOld		  * psTexture;
};

struct PvPKillInfoData
{
	char					szKillerName[32];
	char					szVictimName[32];

	int						iKillerClass;
	int						iVictimClass;

	int						iKillType;

	BOOL					bDelete;

	int						iAlpha;
};

class CBlessCastleHandler
{
enum EElementsRankingID
{
	TEXTID_BattlePointsValue,
	TEXTID_HitRateValue,
	TEXTID_ConRateValue,
	TEXTID_UserScoreValue,
	TEXTID_ClanName1,
	TEXTID_ClanName2,
	TEXTID_ClanName3,
	TEXTID_ClanName4,
	TEXTID_ClanName5,
	TEXTID_ClanValue1,
	TEXTID_ClanValue2,
	TEXTID_ClanValue3,
	TEXTID_ClanValue4,
	TEXTID_ClanValue5,
	TEXTID_CharacterName1,
	TEXTID_CharacterName2,
	TEXTID_CharacterName3,
	TEXTID_CharacterValue1,
	TEXTID_CharacterValue2,
	TEXTID_CharacterValue3,
	TEXTID_TimeLeft,
	TEXTID_Phase,

	IMAGEID_ClanIcon1,
	IMAGEID_ClanIcon2,
	IMAGEID_ClanIcon3,
	IMAGEID_ClanIcon4,
	IMAGEID_ClanIcon5,
	IMAGEID_CrystalTowerIcon1,
	IMAGEID_CrystalTowerIcon2,
	IMAGEID_CrystalTowerIcon3,
	IMAGEID_CrystalTowerIcon4,
	IMAGEID_CrystalTowerIcon5,
	IMAGEID_CrystalTowerIcon6,
};
protected:
    IMPFNC                          pfnCastleMode = 0x0392E088;
    IMPFNC                          pfnDefenseLevel = 0x0392E090;
    IMPFNC                          pfnBlessCastleClanID = 0x0392E08C;
    IMPFNC                          pfnGetClanIDByClass = 0x005EC420;

	int								iUserScoreTemp	= 0;

	int								iPvPKills		= 0;
	int								iPvPDeaths		= 0;

    int                             iPvPExperience  = 0;

	int								iPhaseID		= 0;


	std::vector<PvPKillInfoData*>	vPvPKillData;

	PvPKillInfoData					* psCurrentPvPKillInfo;

	DWORD							dwPvPKillInfoTimeOut;

	UI::Image						* pcaImageCharacterPvPKillInfo[10];
	UI::Image						* pcaImageKillPvPKillInfo[PVPKILLTYPE_COUNT-1];

	char							szCharacterName[3][32];

	int								iClass[3];

	int								iDamage[3];

	DX::Font_ptr					pFont;

	int								iMode;
	UI::Image					  * pcCrystalTowerImage;

	UI::Window_ptr					pWindowRanking;
	UI::Window_ptr					pWindowSiegeWarInfo;
public:
	CBlessCastleHandler();
	virtual ~CBlessCastleHandler();

    BOOL                                                IsCastleEvent() { return (*(BOOL *)pfnCastleMode); }
    int                                                 GetCastleDefenseLevel() { return READINT( pfnCastleMode ); }
    int                                                 GetCastleLeaderClanID() { return READINT( pfnBlessCastleClanID ); }

    static int                                          GetClanID( int iCodeClass );

	bool												InBattleZone();

	void												Init();

	BOOL												WarpField( int iFieldID, int * piX, int * piZ );

	BOOL												WarpCastle( BOOL bLeader, int * piX, int * piZ );

	void												Render();

	BOOL												OnMouseClick( class CMouse * pcMouse );
	void												OnMouseMove( class CMouse * pcMouse );
	void												OnResolutionChanged();

	void												PHUpdate( struct PacketPvPData * psPacket );
	void												PHUpdate( struct PacketSiegeWarPhase * psPacket );

	void												HandlePacket( struct PacketPvPKillType * psPacket );
	void												HandlePacket( struct PacketBlessCastleTopIndividual * psPacket );
	void												HandlePacket( struct PacketBlessCastleUserScoreUpdate * psPacket );

	void												Update( float fTime );

	void												UpdateObjectSettings();

	int													GetSiegeWarMode() { return iMode; };

	BOOL												InSiegeWar() { return *(BOOL*)(0x362BE1C); };

private:
	void												BuildWindowSiegeWarInfo();
	void												BuildWindowRanking();

    void                                                RenderBattleStatus();
};

