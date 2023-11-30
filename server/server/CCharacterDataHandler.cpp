#include "stdafx.h"
#include "CCharacterDataHandler.h"


CCharacterDataHandler::CCharacterDataHandler()
{
}


CCharacterDataHandler::~CCharacterDataHandler()
{
}

BOOL CCharacterDataHandler::LoadCharacterInventory( User * pcUser, PacketCharacterRecordData & sPacket )
{
	ZeroMemory( pcUser->pcUserData->iaServerPotion, sizeof( int ) * 3 * 4 );



	return 0;
}

BOOL CCharacterDataHandler::LoadCharacterDataFile( std::string strFile, PacketCharacterRecordData & sPacket )
{
	BOOL bRet = FALSE;

	FILE * fp = NULL;
	fopen_s( &fp, strFile.c_str(), "rb" );
	if ( fp )
	{
		bRet = TRUE;

		fread( &sPacket, sizeof( PacketCharacterRecordData ), 1, fp );
		fclose( fp );
	}

	if ( bRet )
	{
		if ( (sPacket.iLength == 0) && (sPacket.iHeader == 0) )
			bRet = FALSE;
		else if ( (sPacket.iLength <= 0) || (sPacket.iLength > sizeof( PacketCharacterRecordData )) )
			bRet = FALSE;
	}

	return bRet;
}

void CCharacterDataHandler::HandlePacket( User * pcUser, PacketSelectCharacter * psPacket )
{
	if ( pcUser->pcUserData->szAccountName[0] == 0 )
		return;

	if ( (pcUser->GetGameLevel() == GAMELEVEL_None) && pcUser->pcUserData->bBlock )
		return;

	psPacket->szName[_countof( psPacket->szName ) - 1] = 0;

	CCharacterSaveData * pcSaveData = new CCharacterSaveData();
	if ( LoadCharacterDataFile( FormatString( "Data\\Character\\%s.dat", psPacket->szName ), pcSaveData->sPacket ) )
	{
		BOOL bDisconnect = FALSE;

		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pc = USERSERVER->pcaUserInGame[i];
			if ( pc != pcUser )
			{
				if ( pcUser->pcUserData->dwHashAccountName == pc->pcUserData->dwHashAccountName )
				{
					if ( STRINGCOMPAREI( pcUser->pcUserData->szAccountName, pc->pcUserData->szAccountName ) )
						bDisconnect = TRUE;
				}
				else if ( STRINGCOMPAREI(psPacket->szName, pc->pcUserData->szCharacterName) )
					bDisconnect = TRUE;

				if ( bDisconnect )
				{
					WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcUser->pcUserData->pcSocketData );
					WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pc->pcUserData->pcSocketData );
					break;
				}
			}
		}

		if ( bDisconnect == FALSE )
		{
			CharacterData & sCharacterData = pcSaveData->sPacket.sCharacterData;
			CharacterSave & sCharacterSaveData = pcSaveData->sPacket.sCharacterSaveData;

			pcUser->pcUserData->dwCharacterUpdateTime = TICKCOUNT;

			//Set EXP Verification
			LARGE_INTEGER liEXP;
			liEXP.LowPart = sCharacterData.iCurrentExpLow;
			liEXP.HighPart = sCharacterData.iCurrentExpHigh;
			if ( liEXP.QuadPart < 0 )
				liEXP.QuadPart = 0;
			sCharacterData.iCurrentExpLow = liEXP.LowPart;
			sCharacterData.iCurrentExpHigh = liEXP.HighPart;

			//Level
			sCharacterData.iLevel = CHARACTERSERVER->GetLevelFromEXP( liEXP.QuadPart );
			sCharacterData.iOwnerID = (int)CHARACTERSERVER->GetExpFromLevel( sCharacterData.iLevel + 1 );

			ValidateStatusPoints( pcUser, sCharacterData, sCharacterSaveData.iQuestFlag );

			sCharacterData.iClanID = 0;

			if ( (sCharacterData.iLevel < 20) && (sCharacterData.iRank != CHARACTERRANK_Rank1) )
				sCharacterData.iRank = CHARACTERRANK_Rank1;

			sCharacterSaveData.sPotionUpdate[1] = 1;

			sCharacterSaveData.iBlessCastleTax = BLESSCASTLESERVER->psBlessCastleStatusData->iTax;
			sCharacterSaveData.iBlessCastleClanID = BLESSCASTLESERVER->psBlessCastleStatusData->iClanID;

			//Map Check
			if ( sCharacterSaveData.iMapID == MAPID_Bellatra )
				sCharacterSaveData.iMapID = MAPID_NaviskoTown;
			else if ( sCharacterSaveData.iMapID == MAPID_QuestArena )
			{
				ECharacterRace iRace = GetCharacterRace( sCharacterData.iClass );;
				if ( iRace == CHARACTERRACE_Tempskron )
					sCharacterSaveData.iMapID = MAPID_RicartenTown;
				else if ( iRace == CHARACTERRACE_Morion )
					sCharacterSaveData.iMapID = MAPID_PhillaiTown;
			}
			else if ( sCharacterSaveData.iMapID == MAPID_T5QuestArena )
				sCharacterSaveData.iMapID = MAPID_Atlantis;
			else if ( sCharacterSaveData.iMapID == MAPID_GhostCastle )
				sCharacterSaveData.iMapID = MAPID_Atlantis;
			else if ( sCharacterSaveData.iMapID == MAPID_ForestDungeon )
				sCharacterSaveData.iMapID = MAPID_Atlantis;
			else if ( sCharacterSaveData.iMapID == MAPID_DragonsDungeon )
				sCharacterSaveData.iMapID = MAPID_RicartenTown;

			if ( pcUser->GetID() == 0 )
				sCharacterData.iID = USERSERVER->GetNewUserID();
			else
				sCharacterData.iID = pcUser->GetID();

			CHARACTERSERVER->Validate( &sCharacterData );

			sCharacterData.bResetStatistics = 0;

			CCharacterDataHandler::SetLoginKey( &sCharacterData );

			ProcessPacket( pcUser, &pcSaveData->sPacket );

			STRINGCOPY( pcUser->pcUserData->szCharacterName, psPacket->szName );

			pcUser->pcUserData->dwHashCharacterName = CHARACTERSERVER->GetHashString( pcUser->pcUserData->szCharacterName );

			if ( pcSaveData->sPacket.iDropItemCount > 0 )
				if ( pcSaveData->sPacket.iDropItemCount > _countof( pcSaveData->sPacket.saDropItemData ) )
					pcSaveData->sPacket.iDropItemCount = _countof( pcSaveData->sPacket.saDropItemData );

			pcUser->pcUserData->iSaveItems = pcSaveData->sPacket.iDropItemCount;
			CopyMemory( pcUser->pcUserData->sDropItems, pcSaveData->sPacket.saDropItemData, sizeof( DropItemData ) * pcSaveData->sPacket.iDropItemCount );

			if ( sCharacterSaveData.iLastGold > 0 )
				pcUser->pcUserData->iSaveGold = sCharacterSaveData.iLastGold;

			if ( pcUser->pcUserData->sCharacterData.szName[0] != 0 )
				CopyMemory( &pcUser->pcUserData->sCharacterData, &sCharacterData, sizeof( CharacterData ) );

			CopyMemory( pcUser->pcUserData->baSkillPoint, sCharacterSaveData.sSkillInfo.baSkillPoints, sizeof( BYTE ) * _countof( pcUser->pcUserData->baSkillPoint ) );

			pcUser->pcUserData->sCharacterData.iLevel = sCharacterData.iLevel;
			pcUser->pcUserData->iLevelStart = sCharacterData.iLevel;

			pcUser->pcUserData->sCharacterData.iCurrentExpLow = sCharacterData.iCurrentExpLow;
			pcUser->pcUserData->sCharacterData.iCurrentExpHigh = sCharacterData.iCurrentExpHigh;

			STRINGCOPY( pcUser->pcUserData->sCharacterData.szName, sCharacterData.szName );

			pcUser->pcUserData->sCharacterData.iClass = sCharacterData.iClass;

			pcUser->pcUserData->sCharacterData.iGold = sCharacterData.iGold;

			liEXP.LowPart						= sCharacterData.iCurrentExpLow;
			liEXP.HighPart						= sCharacterData.iCurrentExpHigh;
			pcUser->pcUserData->liEXP.QuadPart	= liEXP.QuadPart;

			if ( sCharacterSaveData.iLastGold > 0 )
				pcUser->pcUserData->iGoldStart = sCharacterSaveData.iLastGold;
			else
				pcUser->pcUserData->iGoldStart = sCharacterData.iGold;

			USERSERVER->SetServerUserGold( pcUser->pcUserData, pcUser->pcUserData->iGoldStart, WHEREID_LoadPlayer );

			pcUser->pcUserData->sCharacterData.iClanID = sCharacterData.iClanID;

			if ( sCharacterSaveData.szAccountName[0] != 0 )
			{
				if ( STRINGCOMPAREI( sCharacterSaveData.szAccountName, pcUser->pcUserData->szAccountName ) == FALSE )
				{
					PacketLogCheat s;
					s.iCheatID	= CHEATLOGID_CharacterAccountMismatch;
					s.LParam	= (DWORD)sCharacterSaveData.szAccountName;
					s.SParam	= (DWORD)pcUser->pcUserData->szAccountName;
					LOGSERVER->OnLogCheat( pcUser->pcUserData, &s );
				}
			}

			if ( pcUser->pcUserData->psCharacterFile )
			{

			}
		}
	}
	else
	{
		psPacket->iHeader = PKTHDR_SaveDataFail;
		if ( pcUser->GetID() == 0 )
			pcUser->pcUserData->iID = USERSERVER->GetNewUserID();

		psPacket->dwReserved2 = pcUser->GetID();
		psPacket->dwReserved1 = GetLoginKey( &pcUser->pcUserData->sCharacterData, CHARACTERSERVER->GetHashString( pcUser->pcUserData->szCharacterName ) );

		SENDPACKET( pcUser, psPacket );

		pcUser->pcUserData->psCharacterFile->iLength = 0;
		pcUser->pcUserData->psCharacterFile->iHeader = 0;

		pcUser->pcUserData->iLevelStart = 1;

		ZeroMemory( &pcUser->pcUserData->baSkillPoint, sizeof( BYTE ) * _countof( pcUser->pcUserData->baSkillPoint ) );
		
		pcUser->pcUserData->sCharacterData.iRank = CHARACTERRANK_Rank1;

		SENDPACKETBLANK( pcUser, PKTHDR_Save );
	}

	DELET( pcSaveData );
}

void CCharacterDataHandler::ProcessPacket( User * pcUser, PacketCharacterRecordData * psPacket )
{
	PacketCharacterRecordDataSplit sPacket;
	sPacket.iHeader			= PKTHDR_SaveData;
	sPacket.iCurrentPacket	= 0;
	sPacket.iSize			= 0;

	sPacket.iMaxPackets = psPacket->iLength / MAX_PKTSIZE_SAVEDATA;
	if ( psPacket->iLength % MAX_PKTSIZE_SAVEDATA )
		if ( psPacket->iLength > MAX_PKTSIZE_SAVEDATA )
			sPacket.iMaxPackets++;

	int iCurrentLength = 0;
	while ( iCurrentLength < psPacket->iLength )
	{
		sPacket.iSize = psPacket->iLength - iCurrentLength;
		if ( sPacket.iSize > MAX_PKTSIZE_SAVEDATA )
			sPacket.iSize = MAX_PKTSIZE_SAVEDATA;

		sPacket.iLength = sPacket.iSize + sizeof( Packet ) + 12;

		CopyMemory( &sPacket.szData, (BYTE*)psPacket + iCurrentLength, sPacket.iSize );

		SENDPACKET( pcUser, &sPacket );

		sPacket.iCurrentPacket++;

		iCurrentLength += sPacket.iSize;
	}
}

NAKED BOOL CCharacterDataHandler::SetLoginKey( CharacterData * psCharacterData )
{
	JMP( 0x0044E990 );
}

DWORD CCharacterDataHandler::GetLoginKey( CharacterData * psCharacterData, DWORD dwHashName )
{
	return CALL_WITH_ARG2( 0x0044E950, (DWORD)psCharacterData, dwHashName );
}

BOOL CCharacterDataHandler::ValidateStatusPoints( User * pcUser, CharacterData & sCharacterData, UINT iQuestFlag )
{
	BOOL bRet = TRUE;

	int iBaseStatusCount = abs( sCharacterData.iStrength ) +
		abs( sCharacterData.iSpirit ) +
		abs( sCharacterData.iTalent ) +
		abs( sCharacterData.iAgility ) +
		abs( sCharacterData.iHealth ) +
		abs( sCharacterData.iStatPoints );

	int iAddStatusPoints = ((sCharacterData.iLevel - 1) * 5) + 99;
	iAddStatusPoints += GetStatusPointQuest( pcUser, sCharacterData.iLevel, iQuestFlag );

	sCharacterData.iStatPoints += (iAddStatusPoints - iBaseStatusCount);

	if ( sCharacterData.iStatPoints < 0 )
	{
		if ( sCharacterData.iStatPoints <= (-10) )
		{
			sCharacterData.iStrength = 1;
			sCharacterData.iSpirit = 8;
			sCharacterData.iTalent = 1;
			sCharacterData.iAgility = 1;
			sCharacterData.iHealth = 30;

			bRet = FALSE;
		}

		sCharacterData.iStatPoints = 0;
	}

	return bRet;
}

int CCharacterDataHandler::GetSkillPointQuest( int iLevel, UINT iQuestFlag )
{
	int iPoints = 0;

	//The Cave
	if ( (iQuestFlag & 2) && (iLevel >= 55) )
		iPoints++;

	//Help Michelle
	if ( (iQuestFlag & 4) && (iLevel >= 70) )
		iPoints++;

	//Closed Fury
	if ( (iQuestFlag & 8) && (iLevel >= 80) )
		iPoints += 2;

	return iPoints;
}

int CCharacterDataHandler::GetStatusPointQuest( User * pcUser, int iLevel, UINT iQuestFlag )
{
	int iPoints = 0;

	//For Her
	if ( (iQuestFlag & 1) && (iLevel >= 30) )
		iPoints += 5;

	//Help Michelle
	if ( (iQuestFlag & 4) && (iLevel >= 70) )
		iPoints += 5;

	//Closed Fury
	if ( (iQuestFlag & 8) && (iLevel >= 80) )
		iPoints += 5;

	//Kingdom Test
	if ( (iQuestFlag & 64) && (iLevel >= 80) )
		iPoints += (iLevel - 79) * 2;

	//Bitter Ordeal
	if ( (iQuestFlag & 128) && (iLevel >= 90) )
		iPoints += (iLevel - 89) * 3;

	//Custom Quests
	if ( (iQuestFlag & QUESTSAVEFLAG_TheLoyaltyTest) && (iLevel >= 120) )
		iPoints += (iLevel - 119) * 3;

	return iPoints;
}
