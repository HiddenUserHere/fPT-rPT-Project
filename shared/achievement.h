#pragma once


enum EAchievementType : int
{
	ACHIEVEMENTTYPE_None,
	ACHIEVEMENTTYPE_GameMaster,
	ACHIEVEMENTTYPE_LegacyMember,
	ACHIEVEMENTTYPE_ReachLevel115,
	ACHIEVEMENTTYPE_ReachLevel125,
	ACHIEVEMENTTYPE_ReachLevel130,
	ACHIEVEMENTTYPE_BattleRoyaleLastStandingT,
	ACHIEVEMENTTYPE_BattleRoyaleLastStandingM,
	ACHIEVEMENTTYPE_ReachLevel145,
	ACHIEVEMENTTYPE_Streamer,
};

enum EAchievementTask
{
	ACHIEVEMENTTASK_None,
	ACHIEVEMENTTASK_LevelUP,
	ACHIEVEMENTTASK_BattleRoyaleWinner,
	ACHIEVEMENTTASK_BattleRoyaleKills,
	ACHIEVEMENTTASK_BattleRoyalePoints,
};

struct ValueTaskAchievement
{
	char			szValue[64] = { 0 };
	
	ValueTaskAchievement( int i ) { STRINGFORMAT( szValue, "%d", i ); }
	ValueTaskAchievement( float f ) { STRINGFORMAT( szValue, "%f", f ); }
	ValueTaskAchievement( const char * pszValue ) { STRINGCOPY( szValue, pszValue ); }
	~ValueTaskAchievement() {}

	int					GetInt() { return atoi( szValue ); }
	float				GetFloat() { return (float)atof( szValue ); }
	char				* GetString() { return szValue; }
};


#define MAX_ACHIEVEMENT_EVENT			5
#define MAX_ACHIEVEMENT_EVENT_VALUES	3

class CAchievementEvent
{
public:
	void				SetID( int iID ) { this->iID = iID; }
	void				SetType( int iType ) { this->iType = iType; }
	void				SetIsLevelAchievement( BOOL b ) { bIsLevelAchievement = b; }
	void				SetValue( int iIndex, const char * pszValue );
	void				SetValue( int iIndex, int iValue );
	void				SetValue( int iIndex, float fValue );
	void				SetIsUnique( BOOL b ) { bIsUnique = b; }
	void				SetIsCharacterAchievement( BOOL b ) { bIsCharacterAchievement = b; }

	int					GetID() { return iID; }
	int					GetType() { return iType; }
	BOOL				IsLevelAchievement() { return bIsLevelAchievement; }
	char				* GetValueString( int iIndex );
	int					GetValueInt( int iIndex );
	float				GetValueFloat( int iIndex );
	BOOL				IsUnique() { return bIsUnique; }
	BOOL				IsCharacterAchievement() { return bIsCharacterAchievement; }

private:
	int					iID						= 0;
	int					iType					= 0;
	BOOL				bIsLevelAchievement		= FALSE;
	char				szaValue[MAX_ACHIEVEMENT_EVENT_VALUES][64]		= { 0 };
	BOOL				bIsUnique				= FALSE;
	BOOL				bIsCharacterAchievement = FALSE;
};

class CAchievement
{
public:
	int					GetID() { return iID; }
	int					GetType() { return iType; }
	const char			* GetName() { return szName; }
	const char			* GetImagePath() { return szImagePath; }
	const char			* GetDescription() { return szDescription; }

	void				SetAchievement( int iID, int iType, const std::string & strName, const std::string & strImage, const std::string & strDescription );
	void				AddEvent( CAchievementEvent * pc ){ vAchievementEvent.push_back( pc ); }

	std::vector<CAchievementEvent*> & GetEvents() { return vAchievementEvent; }

	void				Shutdown();
private:
	int					iID						= 0;
	int					iType					= 0;
	char				szName[32]				= { 0 };
	char				szImagePath[MAX_PATH]	= { 0 };
	char				szDescription[256]		= { 0 };

	std::vector<CAchievementEvent*> vAchievementEvent;
};

class CAchievementUser
{
public:
	void				SetID( int iID ) { this->iID = iID; }
	void				SetLevel( int iLevel ) { this->iLevel = iLevel; }
	void				SetValue( int iIndex, const char * pszValue );
	void				SetValue( int iIndex, int iValue );
	void				SetValue( int iIndex, float fValue );
	void				SetDate( SYSTEMTIME & s );

	int					GetID() { return iID; }
	int					GetLevel() { return iLevel; }
	char				* GetValueString( int iIndex );
	int					GetValueInt( int iIndex );
	float				GetValueFloat( int iIndex );
	SYSTEMTIME			& GetDate(){ return sDate; }

private:
	int					iID = 0;
	int					iLevel = 0;
	char				szaValue[MAX_ACHIEVEMENT_EVENT_VALUES][64] = { 0 };

	SYSTEMTIME			sDate;
};

struct PacketAchievement : Packet
{
	int					iID;
	int					iType;
	char				szName[32];
	char				szImagePath[MAX_PATH];
	char				szDescription[256];

	struct AchievementEvent
	{
		int				iID;
		int				iType;
		BOOL			bIsLevelAchievement;
		char			szaValue[MAX_ACHIEVEMENT_EVENT_VALUES][64];
		BOOL			bIsUnique;
		BOOL			bIsCharacterAchievement;
	};

	int					iEventCount;
	AchievementEvent	saEvent[MAX_ACHIEVEMENT_EVENT];
};

struct PacketAchievementUser : Packet
{
	int					iID;
	int					iAchievementID;
	int					iLevel;
	char				szaValue[MAX_ACHIEVEMENT_EVENT_VALUES][64];

	SYSTEMTIME			sDate;
};
