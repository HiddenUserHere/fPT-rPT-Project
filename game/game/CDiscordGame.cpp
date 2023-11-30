#include "stdafx.h"
#include "CDiscordGame.h"


CDiscordGame::CDiscordGame()
{
}


CDiscordGame::~CDiscordGame()
{
}

void CDiscordGame::Init()
{
	DiscordEventHandlers sDiscordHandler;
	ZeroMemory( &sDiscordHandler, sizeof( DiscordEventHandlers ) );

	iStartTime = time( NULL );

	Discord_Initialize( "426454844249735178", &sDiscordHandler, 0, "0" );
}

void CDiscordGame::Shutdown()
{
	Discord_Shutdown();
}

void CDiscordGame::Update()
{
	struct DiscordRichPresenceLocal
	{
		char state[128]; /* max 128 bytes */
		char details[128]; /* max 128 bytes */
		int64_t startTimestamp;
		int64_t endTimestamp;
		char largeImageKey[32]; /* max 32 bytes */
		char largeImageText[128]; /* max 128 bytes */
		char smallImageKey[32]; /* max 32 bytes */
		char smallImageText[128]; /* max 128 bytes */
		char partyId[128]; /* max 128 bytes */
		int partySize;
		int partyMax;
		char matchSecret[128]; /* max 128 bytes */
		char joinSecret[128]; /* max 128 bytes */
		char spectateSecret[128]; /* max 128 bytes */
		int8_t instance;
	};


	DiscordRichPresenceLocal sLocal;
	ZeroMemory( &sLocal, sizeof( DiscordRichPresenceLocal ) );
	if ( PARTYHANDLER->GetPartyMembers().size() > 0 )
		STRINGFORMAT( sLocal.state, "Party Game (%d/%d) - %s (%d)", PARTYHANDLER->GetPartyMembers().size(), MAX_PARTY_MEMBERS,
			UNITDATA->sCharacterData.szName, UNITDATA->sCharacterData.iLevel );
	else
		STRINGFORMAT( sLocal.state, "Solo Game - %s (%d)", UNITDATA->sCharacterData.szName, UNITDATA->sCharacterData.iLevel );

	if ( MAP_ID >= 0 )
	{
		if ( GM_MODE )
			STRINGCOPY( sLocal.details, "Who knows?" );
		else
			STRINGCOPY( sLocal.details, MAPNAME( MAP_ID ) );
	}

	sLocal.startTimestamp = iStartTime;

	STRINGFORMAT( sLocal.largeImageKey, "%s_class", GetCharacterClassByFlag( CharacterClassToClassFlag( UNITDATA->sCharacterData.iClass ) ) );
	STRINGCOPY( sLocal.largeImageText, GetCharacterClassByFlag( CharacterClassToClassFlag( UNITDATA->sCharacterData.iClass ) ) );

	lower( sLocal.largeImageKey );

	DiscordRichPresence sPresence;
	ZeroMemory( &sPresence, sizeof( DiscordRichPresence ) );
	sPresence.state = sLocal.state;
	sPresence.details = sLocal.details;
	sPresence.startTimestamp = sLocal.startTimestamp;
	sPresence.endTimestamp = sLocal.endTimestamp;
	sPresence.largeImageKey = sLocal.largeImageKey;
	sPresence.largeImageText = sLocal.largeImageText;
	sPresence.largeImageKey = sLocal.largeImageKey;
	sPresence.smallImageText = sLocal.smallImageText;
	sPresence.partyId = sLocal.partyId;
	sPresence.partySize = sLocal.partySize;
	sPresence.partyMax = sLocal.partyMax;
	sPresence.matchSecret = sLocal.matchSecret;
	sPresence.spectateSecret = sLocal.spectateSecret;
	sPresence.instance = sLocal.instance;
	Discord_UpdatePresence( &sPresence );
}
