#include "stdafx.h"
#include "userserver.h"
#include "HNSSkill.h"

extern void __cdecl AddFinalDamage( UserData * pcUserData, void * pPacket );

User * pcaUser;

UserServer::UserServer()
{
	pcaUserInGame = new User*[PLAYERS_MAX + 256];

	pcaUser = new User[PLAYERS_MAX + 256];

	pcaUserData = new UserData[PLAYERS_MAX + 256];

	USERSDATA = pcaUserData;

	TEST( "UserData", sizeof( UserData ), 0x0B510 );

	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		User * u = pcaUser + i;
		u->uIndex = i;
		u->pcUserData = pcaUserData + i;
	}
}

UserServer::~UserServer()
{
}

void UserServer::CreateData()
{
	
}

User * UserServer::GetFreeUser()
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		User * u = pcaUser + i;
		
		if ( !u->bInUse )
		{
			u->Init();
			return u;
		}
	}

	//Error
	return NULL;
}

User * UserServer::UserdatatoUser( UserData * pcUserData )
{
	if ( pcUserData == NULL )
		return NULL;

	return (pcaUser + (pcUserData - pcaUserData));
}

User * UserServer::GetUserByIndex( UINT uIndex )
{
	return pcaUser + uIndex;
}

UserData * UserServer::GetUserdata( int iEnemyID )
{
	typedef UserData *( __cdecl *t_GetPlayerPointer ) ( UINT iID );
	t_GetPlayerPointer GetPlayerPointer = ( t_GetPlayerPointer )0x0054CC10;
	return GetPlayerPointer( iEnemyID );
}

UserData * UserServer::GetUserdata( const char * pszCharacterName )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData )
		{
			if ( GAME_SERVER )
			{
				if ( STRINGCOMPAREI( USERSDATA[i].sCharacterData.szName, pszCharacterName ) )
					return &USERSDATA[i];
			}
			else
			{
				if ( STRINGCOMPAREI( USERSDATA[i].szCharacterName, pszCharacterName ) )
					return &USERSDATA[i];
			}
		}
	}
	return NULL;
}

UserData * UserServer::GetUserdataByAccount( const char * pszAccountName )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		User * pcUser = USERDATATOUSER( &USERSDATA[i] );

		if ( pcUser && pcUser->IsConnected() && pcUser->pcUserData && pcUser->pcUserData->szAccountName[0] )
		{
			if ( STRINGCOMPAREI( pcUser->pcUserData->szAccountName, pszAccountName ) )
				return pcUser->pcUserData;
		}
	}

	return nullptr;
}

UserData * UserServer::GetUserdataSocket( SocketData * pcSocketData )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		if ( USERSDATA[i].pcSocketData && USERSDATA[i].pcSocketData == pcSocketData )
		{
			return &USERSDATA[i];
		}
	}
	return NULL;
}

int UserServer::SQLGetAccountID( const char * pszAccountName )
{
	int iRet = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM UserInfo WHERE AccountName=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (char*)pszAccountName, STRLEN( pszAccountName ) );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iRet, 32 );
			}
		}
		pcDB->Close();
	}

	return iRet;
}

void UserServer::CancelForceOrb( User * pcUser )
{
	if ( pcUser->pcUserData->ForceOrbTime )
	{
		pcUser->pcUserData->ForceOrbTime = 1;
		pcUser->pcUserData->ForceOrbCode = 0;
		pcUser->dwLastForceTime = TICKCOUNT;
		NETSERVER->SendForceOrb( pcUser->pcUserData, TRUE );
		CHATSERVER->SendChat( pcUser, EChatColor::CHATCOLOR_Error, "> Force Orb is gone!" );
	}
}

int UserServer::GetTicket()
{
	srand( (DWORD)time( NULL ) );
	int iRand = rand() & 9999 + 1;
	return iRand;
}

BOOL UserServer::SetUserID( UserData * pcUserData )
{
	if ( pcUserData && !pcUserData->szAccountName[ 0 ] )
	{
		char szUserID[ 32 ] = { 0 };
		ZeroMemory( szUserID, 32 );
		char * pszCharName = GAME_SERVER ? pcUserData->sCharacterData.szName : pcUserData->szCharacterName;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT AccountName FROM CharacterInfo WHERE Name=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszCharName );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_String, szUserID, 32 );
				}
			}
			pcDB->Close();
		}

		if ( IsNull(szUserID) )
			return FALSE;
		
		STRINGCOPY( pcUserData->szAccountName, szUserID );
		return TRUE;
	}
	return FALSE;
}

void UserServer::ReconnectUser( UserData * pcUserData )
{
	CHATSERVER->SendChatEx( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> Reconnected to %s Server!", GAME_SERVER ? "Game" : "Login" );
	PacketSimple sPacket;
	sPacket.iLength = sizeof( PacketSimple );
	sPacket.iHeader = PKTHDR_ReconnectedServer;
	sPacket.iUnk = 0;
}

void UserServer::UpdateUnitStatus( User * pcUser )
{
	UserData * pcUserData = pcUser->pcUserData;

	//User Unit Status
	pcUser->sUnitStatusContainer.iHeader = PKTHDR_UnitStatusContainer;
	pcUser->sUnitStatusContainer.iCounter = pcUserData->iUnitStatusCounter & 0x00FFFFFF;
	//Monster Unit Status
	pcUser->sUnitStatusContainerM.iHeader = PKTHDR_UnitStatusContainer;
	pcUser->sUnitStatusContainerM.iCounter = (pcUserData->iUnitStatusCounter & 0x00FFFFFF) - 0x80000000;
	//User Skill Status
	pcUser->sSkillStatus.iHeader = PKTHDR_SkillStatusContainer;

	//Increment Status Counter
	pcUserData->iUnitStatusCounter++;

	//Priority
	if ( pcUserData->iMovingIndex != 0 )
	{
		pcUserData->iMovingIndex++;

		UINT uPriorityCounter = (pcUser->uPriorityCounter++);
		if ( pcUser->uPriority < 4 )
		{
			if ( uPriorityCounter > 1200 )
			{
				pcUser->uPriority = 2;
			}
			else if ( uPriorityCounter > 480 )
			{
				pcUser->uPriority = 2;
			}
			else if ( uPriorityCounter > 240 )
			{
				pcUser->uPriority = 2;
			}
			else if ( uPriorityCounter > 64 )
			{
				pcUser->uPriority = 1;
			}
			else
			{
				pcUser->uPriority = 0;
			}
		}

		//AFK check
		if ( uPriorityCounter > 1920 )
		{
			pcUser->pcUserData->bAFK = TRUE;
		}
		else
		{
			pcUser->pcUserData->bAFK = FALSE;
		}
	}
	else
	{
		pcUser->uPriorityCounter = 0;
		pcUser->uPriority = 0;

		pcUser->pcUserData->bAFK = FALSE;
	}
}

void UserServer::OnHandlerFunctions( DWORD dwCode, DWORD * pdwOptional )
{
	switch ( dwCode )
	{
		// Debug Unit
		case 1:
			UnitData * pcUnitData;
			pcUnitData = (UnitData*)(pdwOptional[0]);
			int iDamage;
			iDamage = (int)(pdwOptional[1]);
			MAPSERVER->OnSendDebugDamageUnit( pcUnitData, iDamage );
			break;

		// User Load
		case 2:
			USERSERVER->OnLoadUser( USERSERVER->UserdatatoUser( (UserData*)pdwOptional[0] ) );
			break;

		// User UnLoad
		case 3:
			USERSERVER->OnUnLoadUser( USERSERVER->UserdatatoUser( (UserData*)pdwOptional[0] ) );
			break;

		// Final Damage
		case 4:
			AddFinalDamage( (UserData*)pdwOptional[0], (void*)pdwOptional[1] );
			break;

		// Final Damage
		case 5:
			NETSERVER->SendSyncChar( (UserData*)pdwOptional[0] );
			CHARACTERSERVER->OnCharacterSyncData( (UserData*)pdwOptional[0] );
			break;

		case 6:
			//SOCKETSERVER->SocketClose( (SocketData*)pdwOptional[0] );
			break;

		case 7:

			break;

		case 8:
			BLESSCASTLEHANDLER->UpdateSiegeWar();
			break;

		case 9:
			BLESSCASTLESERVER->NetSendBlessCastleOwnerClanID( pdwOptional[0] );
			break;

		case 0x0A:
			BLESSCASTLESERVER->SQLReadBlessCastleStatus();
			break;

		case 0x0B:
			BLESSCASTLESERVER->SQLWriteBlessCastleStatus();
			break;

		case 0x0C:
			PRIESTESSSKILL->OnHolyMindHandler( (UserData*)pdwOptional[0], (int*)pdwOptional[1] );
			break;

		case 0x0D:
			ITEMSERVER->OnUseCrystal( (UserData*)pdwOptional[0], ((UnitData*)pdwOptional[1])->iID, (DWORD)pdwOptional[2], ((UnitData *)pdwOptional[1])->dwLastUpdateTime );
			break;

		case 0x0E:
			ITEMSERVER->OnUseCrystalDie( (UserData*)pdwOptional[0], ((UnitData*)pdwOptional[1])->iID );
			break;

		case 0x0F:
			HALLOWEENHANDLER->OnSpawnEventMonster( (CharacterData*)pdwOptional[0] );
			XMASHANDLER->OnSpawnMonsterEvent( (CharacterData*)pdwOptional[0], (Map*)pdwOptional[1] );
			EVENTSERVER->OnSpawnMonsterEvent( (CharacterData*)pdwOptional[0], (Map*)pdwOptional[1] );
			break;

		case 0x11:
			break;

		case 0x12:
			break;

		case 0x13:
			UserData * ud;
			ud = (UserData*)pdwOptional[1];
			if ( ud )
				MAPSERVER->SendStageItem( (Map*)pdwOptional[0], (UserData*)pdwOptional[1] );
			break;

		case 0x14:
			ud = (UserData*)pdwOptional[1];
			if ( ud )
				MAPSERVER->SendStageSpawn( (Map*)pdwOptional[0], (UserData*)pdwOptional[1] );
			break;

		case 0x15:
			if ( (UnitData*)pdwOptional[0] )
				XMASHANDLER->OnSayTime( (UnitData*)pdwOptional[0] );
			break;

		case 0x16:
			if ( (UnitData*)pdwOptional[0] )
				UNITSERVER->IsMultiplyHPMonter( (UnitData*)pdwOptional[0] );
			break;

		case 0x17:
			MAPSERVER->SpawnMonsterBoss( (Map*)pdwOptional[0], (Map::SpawnSetting::Boss*)pdwOptional[1] );
			break;

		case 0x18:
			MAGICIANSKILL->UseDistortionUser( (UserData*)pdwOptional[0], pdwOptional[1] );
			break;

		case 0x19:
			BLESSCASTLESERVER->Start();
			break;

		default:
			break;
	}
}

BOOL UserServer::AddServerUserGold( UserData * pcUserData, int iGold, EWhereAction eWhere )
{
	typedef BOOL( __thiscall *tfnAddGoldServer )(UserData * pcUserData, int iGold, EWhereAction eWhere);
	tfnAddGoldServer fnAddGoldServer = (tfnAddGoldServer)0x00575A30;
	return fnAddGoldServer( pcUserData, iGold, eWhere );
}

BOOL UserServer::SetServerUserGold( UserData * pcUserData, int iGold, EWhereAction eWhere )
{
	typedef BOOL( __thiscall *tfnSetGoldServer )(UserData * pcUserData, int iGold, EWhereAction eWhere);
	tfnSetGoldServer fnSetGoldServer = (tfnSetGoldServer)0x00575BB0;
	return fnSetGoldServer( pcUserData, iGold, eWhere );
}

BOOL UserServer::SubServerUserGold( UserData * pcUserData, int iGold, EWhereAction eWhere )
{
	typedef BOOL( __thiscall *tfnSubGoldServer )(UserData * pcUserData, int iGold, EWhereAction eWhere);
	tfnSubGoldServer fnSubGoldServer = (tfnSubGoldServer)0x00575AF0;
	return fnSubGoldServer( pcUserData, iGold, eWhere );
}

BOOL UserServer::InventoryServerCheck( UserData * pcUserData, int iUnused )
{
	User * pcUser = USERDATATOUSER( pcUserData );

	//Check
	if ( pcUserData->psCharacterFile == NULL )
		return FALSE;

	if ( pcUserData->iGameLevel >= GAMELEVEL_Three )
		return FALSE;

	if ( pcUser && pcUser->bInBattleRoyale )
		return FALSE;

	PacketCharacterRecordData * psCharacter = (PacketCharacterRecordData*)pcUserData->psCharacterFile;

	RecordItem sRecordItem;

	DropItemData sDropData[64];

	ZeroMemory( sDropData, sizeof( DropItemData ) * 64 );

	BYTE * pbaItems = psCharacter->baData;

	int iBufferWheel = 0;

	int iCount = 0;

	int iDropItemCount = 0;

	for ( int i = 0; i < psCharacter->iItemCount; i++ )
	{
		//Inventory Limit...
		if ( i >= 200 )
			break;

		ITEMSERVER->DecodeItemsData( pbaItems, &sRecordItem, 44 );

		if ( sRecordItem.sItem.sItemID.ToInt() && sRecordItem.sItem.iChk1 && sRecordItem.sItem.iChk2 )
		{
			iCount = 0;

			if ( sRecordItem.sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
			{
				if ( pcUserData->iTradePotion > 0 )
				{
					if ( ITEMSERVER->GetDropPotion( pcUserData, sRecordItem.sItem.sItemID.ToInt() ) )
						iCount = 1;
				}
				else
					iCount = (-1);
			}
			else
			{
				//Inventory 1 and 2, backup
				for ( int j = 0; j < 300; j++ )
				{
					DropItemData * ps = pcUserData->sIntentoryItems + j;

					if ( ps->iItemID && (ps->iChk1 == sRecordItem.sItem.iChk1) && (ps->iChk2 == sRecordItem.sItem.iChk2) )
					{
						ps->iItemID = ITEMID_None;

						iCount++;
					}
				}

				//Warehouse
				for ( int j = 0; j < WAREHOUSE_ITEM_MAX; j++ )
				{
					DropItemData * ps = pcUser->sWarehouseItems + j;

					if ( ps->iItemID && (ps->iChk1 == sRecordItem.sItem.iChk1) && (ps->iChk2 == sRecordItem.sItem.iChk2) )
					{
						PacketLogCheat s;
						s.iCheatID = CHEATLOGID_ItemErrorInventoryRecord;
						s.WxParam = 1;
						s.LxParam = (int)"*RECORD ITEM WAREHOUSE";
						s.LParam = sRecordItem.sItem.sItemID.ToInt();
						s.SParam = sRecordItem.sItem.iChk1;
						s.EParam = sRecordItem.sItem.iChk2;

						LOGSERVER->OnLogCheat( pcUserData, &s );
						iCount++;

						ITEMSERVER->SaveItemData( &sRecordItem.sItem );
					}
				}
			}

			//Not Found...
			if ( iCount == 0 )
			{
				int iLoop = 0;

				for ( iLoop = 0; iLoop < psCharacter->iDropItemCount; iLoop++ )
				{
					DropItemData * ps = psCharacter->saDropItemData + iLoop;

					if ( (ps->iItemID == sRecordItem.sItem.sItemID.ToInt()) && (ps->iChk1 == sRecordItem.sItem.iChk1) && (ps->iChk2 == sRecordItem.sItem.iChk2) )
						break;
				}


				if ( iDropItemCount < 64 )
				{
					int k = 0;

					for ( k = 0; k < iDropItemCount; k++ )
					{
						DropItemData * ps = sDropData + k;

						if ( (ps->iItemID == sRecordItem.sItem.sItemID.ToInt()) && (ps->iChk1 == sRecordItem.sItem.iChk1) && (ps->iChk2 == sRecordItem.sItem.iChk2) )
							break;
					}

					if ( k >= iDropItemCount )
					{
						sDropData[iDropItemCount].iItemID = sRecordItem.sItem.sItemID.ToInt();
						sDropData[iDropItemCount].iChk1 = sRecordItem.sItem.iChk1;
						sDropData[iDropItemCount].iChk2 = sRecordItem.sItem.iChk2;
						iDropItemCount++;
					}
				}

				if ( iLoop >= psCharacter->iDropItemCount )
				{
					PacketLogCheat s;
					s.iCheatID = CHEATLOGID_ItemErrorInventoryRecord;
					s.WxParam = 1;
					s.LxParam = (int)"*RECORD ITEM";
					s.LParam = sRecordItem.sItem.sItemID.ToInt();
					s.SParam = sRecordItem.sItem.iChk1;
					s.EParam = sRecordItem.sItem.iChk2;

					LOGSERVER->OnLogCheat( pcUserData, &s );

					ITEMSERVER->SaveItemData( &sRecordItem.sItem );
				}
			}
			else
			{
				if ( iCount > 0 )
				{
					for ( int j = 0; j < psCharacter->iDropItemCount; j++ )
					{
						DropItemData * ps = psCharacter->saDropItemData + j;

						if ( (ps->iItemID == sRecordItem.sItem.sItemID.ToInt()) && (ps->iChk1 == sRecordItem.sItem.iChk1) && (ps->iChk2 == sRecordItem.sItem.iChk2) )
						{
							if ( iDropItemCount < 64 )
							{
								sDropData[iDropItemCount].iItemID = sRecordItem.sItem.sItemID.ToInt();
								sDropData[iDropItemCount].iChk1 = sRecordItem.sItem.iChk1;
								sDropData[iDropItemCount].iChk2 = sRecordItem.sItem.iChk2;
								iDropItemCount++;
							}
							break;
						}
					}
				}
			}

			if ( iCount > 1 )
			{
				PacketLogCheat s;
				s.iCheatID = CHEATLOGID_ItemErrorInventoryRecord;
				s.WxParam = iCount;
				s.LxParam = (int)"*RECORD COPIED ITEM";
				s.LParam = sRecordItem.sItem.sItemID.ToInt();
				s.SParam = sRecordItem.sItem.iChk1;
				s.EParam = sRecordItem.sItem.iChk2;

				LOGSERVER->OnLogCheat( pcUserData, &s );

				ITEMSERVER->SaveItemData( &sRecordItem.sItem );
			}
		}

		int iSize = ((int*)pbaItems)[0];

		iBufferWheel += iSize;

		pbaItems += iSize;

		if ( iBufferWheel >= (sizeof( RecordItem ) * 200) )
			break;
	}

	if ( iDropItemCount > 0 )
	{
		CopyMemory( psCharacter->saDropItemData, sDropData, sizeof( DropItemData ) * iDropItemCount );
		psCharacter->iDropItemCount = iDropItemCount;
	}

	if ( psCharacter->sCharacterData.iGold > pcUserData->iInventoryGold )
	{
		PacketLogCheat s;
		s.iCheatID = CHEATLOGID_ServerMoneyErrorInventory;
		s.LParam = 1;
		s.SParam = pcUserData->iInventoryGold;
		s.EParam = psCharacter->sCharacterData.iGold;
		LOGSERVER->OnLogCheat( pcUserData, &s );

		psCharacter->sCharacterData.iGold = pcUserData->iInventoryGold;

		pcUserData->iSaveGold = (-1);

		if ( psCharacter->sCharacterData.iGold < 0 )
			psCharacter->sCharacterData.iGold = 0;
	}

	int iGold = psCharacter->sCharacterSaveData.iLastGold;

	if ( iGold && (iGold - 1) > pcUserData->iInventoryGold )
	{
		PacketLogCheat s;
		s.iCheatID = CHEATLOGID_ServerMoneyErrorInventory;
		s.LParam = 3;
		s.SParam = pcUserData->iInventoryGold;
		s.EParam = psCharacter->sCharacterSaveData.iLastGold - 1;
		LOGSERVER->OnLogCheat( pcUserData, &s );

		psCharacter->sCharacterSaveData.iLastGold = pcUserData->iInventoryGold + 1;

		if ( psCharacter->sCharacterSaveData.iLastGold < 0 )
			psCharacter->sCharacterSaveData.iLastGold = 0;
	}

	return TRUE;
}

BOOL UserServer::AddUnsavedItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 )
{
	return 0;
}

BOOL UserServer::AddUserOnline( UserData * pcUserData, BOOL bOffline )
{

	return TRUE;
}

BOOL UserServer::OnTradeRequest( UserData * pcUserData, PacketSimple * psPacket )
{
	UserData * pcUserDataTarget = GetUserdata( psPacket->iUnk2 );
	
	User * pcUserTarget = USERDATATOUSER( pcUserDataTarget );

	if ( pcUserDataTarget && pcUserDataTarget->pcSocketData && pcUserTarget )
	{
		if ( pcUserData->bBlockAction || pcUserDataTarget->bBlockAction )
			return FALSE;
		if ( pcUserData->szOwner[0] || *(BOOL*)(0x00848314) )
			return FALSE;
		if ( pcUserTarget->sSettingsData.bNoTradeRequest || pcUserTarget->IsAccountShareDenied( ACCOUNTSHARE_DenyTrade ) )
		{
			CHATSERVER->SendChatEx( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> %s disabled trade request!", CHARACTERSERVER->GetCharacterName(pcUserDataTarget) );
			return FALSE;
		}

		if ( auto pcUser = USERDATATOUSER( pcUserData ) )
		{
			if ( pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyTrade ) )
			{
				CHATSERVER->SendChatMessageBox( pcUser, "You can't start a Trade, because you're logged in with the Share PW!" );
				return FALSE;
			}
		}

		SENDPACKET( USERDATATOUSER( pcUserDataTarget ), psPacket, TRUE );
	}
	return TRUE;
}

BOOL UserServer::OnLoadUser( User * pcUser )
{
	USERSERVER->UpdateID( pcUser );

	QUESTSERVER->OnLoadUser( pcUser );
	PVPSERVER->OnLoadUser( pcUser );

	TIMEPOINTSHANDLER->OnLoadUser( pcUser );

	MOUNTSERVERHANDLER->OnLoadUser( pcUser );

	if ( LOGIN_SERVER )
	{
		PacketAgingFreeEvent s;
		s.iLength = sizeof( PacketAgingFreeEvent );
		s.iHeader = PKTHDR_AgingFreeEvent;
		s.bFree = EVENT_AGINGFREE;
		s.iReduceTax = EVENT_AGINGREDUCEFAIL;
		SENDPACKET( pcUser, &s );
	}

	CALL_WITH_ARG1( 0x00557F10, (DWORD)pcUser->pcUserData );
	CALL_WITH_ARG1( 0x00551EE0, (DWORD)pcUser->pcUserData );

	return TRUE;
}

BOOL UserServer::OnUnLoadUser( User * pcUser )
{
	// Load Quests
	QUESTSERVER->OnUnLoadUser( pcUser );
	PVPSERVER->OnUnLoadUser( pcUser );
	ITEMSERVER->OnUnLoadUser( pcUser );
	ACHIEVEMENTHANDLER->OnUnloadUser( pcUser );

	if ( GAME_SERVER )
	{
		CharacterServer::SQLUpdateDateLevelUP( pcUser );

		HONORHANDLER->OnUnloadUser( pcUser );

		TIMEPOINTSHANDLER->OnUnloadUser( pcUser );
	}
	if ( LOGIN_SERVER )
	{
		CHARACTERSERVER->OnCharacterDataExSave( pcUser );
		NETSERVER->OnUnloadUser( pcUser->pcUserData );
	}

	return TRUE;
}

void UserServer::ResetSkills( User * pcUser )
{
	pcUser->dwAlasTime							= 0;
	pcUser->dwAlasEvasion						= 0;

	pcUser->dwBlindTime							= 0;
	pcUser->dwBlindDamage						= 0;

	pcUser->dwAdventMigalTime					= 0;
	pcUser->dwAdventMigalDamage					= 0;

	pcUser->uMagneticDischargeLevel				= 0;
	pcUser->dwMagneticDischargeTimeOut			= 0;

	pcUser->pcUserData->dwAutomationEndTime		= 0;
	pcUser->pcUserData->iAutomationDamageBoost	= 0;

	pcUser->pcUserData->dwMaximizeEndTime		= 0;
	pcUser->pcUserData->iMaximizeMaxDamageBoost = 0;
	
	pcUser->pcUserData->dwPhysicalAbsorbEndTime = 0;
	pcUser->pcUserData->iPhysicalAbsorbBoost	= 0;
	
	pcUser->pcUserData->dwExtremeShieldEndTime	= 0;
	pcUser->pcUserData->iExtremeShield			= 0;

	pcUser->pcUserData->dwHolyBodyEndTime		= 0;
	pcUser->pcUserData->iHolyBodyAbsorb			= 0;

	pcUser->pcUserData->dwHolyValorEndTime		= 0;
	pcUser->pcUserData->iHolyValorBoost			= 0;

	pcUser->pcUserData->dwHolyReflectionEndTime = 0;
	pcUser->pcUserData->iHolyReflectionDamage	= 0;

	pcUser->pcUserData->dwEnchantWeaponEndTime	= 0;
	pcUser->pcUserData->iEnchantWeaponAttr		= 0;

	pcUser->pcUserData->dwMetalArmorEndTime		= 0;
	pcUser->pcUserData->iMetalArmorParam		= 0;

	pcUser->pcUserData->dwSparkShieldEndTime	= 0;
	pcUser->pcUserData->iSparkShieldDamage		= 0;

	pcUser->pcUserData->dwFalconEndTime			= 0;
	pcUser->pcUserData->saFalconDamage[0]		= 0;
	pcUser->pcUserData->saFalconDamage[1]		= 0;

	pcUser->pcUserData->dwDivineShieldEndTime		= 0;
	pcUser->pcUserData->sDivineShieldAbsorb			= 0;
	pcUser->pcUserData->sDivineShieldAttackRating	= 0;

	pcUser->pcUserData->dwTriumphOfValhallaEndTime	= 0;
	pcUser->pcUserData->iTriumphOfValhallaDamage	= 0;

	pcUser->pcUserData->dwVirtualLifeEndTime	= 0;
	pcUser->pcUserData->iVirtualLifeData		= 0;

	pcUser->pcUserData->dwEnergyShieldEndTime	= 0;
	pcUser->pcUserData->iEnergyShieldDamage		= 0;

	pcUser->pcUserData->dwSpiritElementalEndTime	= 0;
	pcUser->pcUserData->iSpiritElementalBoost		= 0;

	pcUser->pcUserData->dwDancingSwordEndTime	= 0;
	pcUser->pcUserData->iDancingSwordLevel		= 0;
	pcUser->pcUserData->dwDancingSwordDelayTime = 0;

	pcUser->pcUserData->dwVanishEndTime			= 0;
	pcUser->pcUserData->saVanishParam[0]		= 0;
	pcUser->pcUserData->saVanishParam[1]		= 0;

	pcUser->pcUserData->dwHookedTargetEndTime	= 0;
	pcUser->pcUserData->iCompulsionParam		= 0;

	pcUser->pcUserData->dwMagneticSphereEndTime = 0;
	pcUser->pcUserData->iMagneticSphereParam	= 0;

	pcUser->pcUserData->dwBerserkerEndTime		= 0;
	pcUser->pcUserData->iBerserkerParam			= 0;

	pcUser->pcUserData->dwAssassinEyeEndTime	= 0;
	pcUser->pcUserData->AssassinEyeParam		= 0;

	pcUser->pcUserData->dwVagueEndTime			= 0;
	pcUser->pcUserData->sVagueEvade				= 0;
	pcUser->pcUserData->sVagueLevel				= 0;

	pcUser->pcUserData->dwForceOfNatureEndTime	= 0;
	pcUser->pcUserData->saForceOfNatureParam[0] = 0;
	pcUser->pcUserData->saForceOfNatureParam[1] = 0;

	pcUser->pcUserData->dwGodlyShiedEndTime		= 0;
	pcUser->pcUserData->iGodlyShiedParam		= 0;

	pcUser->pcUserData->GodsBlessingTime		= 0;
	pcUser->pcUserData->GodsBlessingParam		= 0;

	pcUser->pcUserData->HallOfValhallaTime		= 0;
	pcUser->pcUserData->HallOfValhallaParam[0]	= 0;
	pcUser->pcUserData->HallOfValhallaParam[1]	= 0;

	pcUser->pcUserData->FrostJavelinTime		= 0;
	pcUser->pcUserData->FrostJavelinParam		= 0;

	pcUser->pcUserData->RegenerationFieldTime	= 0;

	pcUser->pcUserData->SummonMuspellTime		= 0;
	pcUser->pcUserData->SummonMuspellParam		= 0;

	pcUser->uFierceWindLevel					= 0;
	pcUser->dwFierceWindTimeOut					= 0;

	pcUser->uHolyConvictionLevel				= 0;
	pcUser->dwHolyConvictionTimeOut				= 0;

	pcUser->bWhirlWind							= false;

	pcUser->uMagneticDischargeLevel				= 0;
	pcUser->dwMagneticDischargeTimeOut			= 0;
}

void UserServer::UpdateSkillStatus( User * pcUser )
{
	PacketSkillStatus & p = pcUser->sSkillStatus;

	p.iLength	= sizeof( PacketSkillStatus );
	p.iHeader	= PKTHDR_SkillStatus;
	p.iID		= pcUser->GetID();

	//Lambda Function for repeatable usage
	auto SetSkillTime = []( short & sPacketLevel, DWORD & sPacketTime, UINT & uUserLevel, DWORD & dwUserTimeOut )-> void
	{
		if ( dwUserTimeOut )
		{
			if ( dwUserTimeOut > TICKCOUNT )
			{
				sPacketLevel	= (short)uUserLevel;
				sPacketTime		= (short)(dwUserTimeOut - TICKCOUNT);
			}
			else
			{
				uUserLevel		= 0;
				dwUserTimeOut	= 0;
				sPacketTime		= 0;
				sPacketLevel	= 0;
			}
		}
		else
		{
			uUserLevel			= 0;
			dwUserTimeOut		= 0;
			sPacketTime			= 0;
			sPacketLevel		= 0;
		}
	};

	//Lethal Sight
	SetSkillTime( p.sLethalSightLevel, p.dwLethalSightTimeLeft, pcUser->uLethalSightLevel, pcUser->dwLethalSightTimeOut );

	//Fierce Wind
	SetSkillTime( p.sFierceWindLevel, p.dwFierceWindTimeLeft, pcUser->uFierceWindLevel, pcUser->dwFierceWindTimeOut );

	//Holy Conviction
	SetSkillTime( p.sHolyConvictionLevel, p.dwHolyConvictionTimeLeft, pcUser->uHolyConvictionLevel, pcUser->dwHolyConvictionTimeOut );
	
	//Wizard Trance
	SetSkillTime( p.sWizardTranceLevel, p.dwWizardTranceTimeLeft, pcUser->uWizardTranceLevel, pcUser->dwWizardTranceTimeOut );

	//Magnetic Discharge
	SetSkillTime( p.sMagneticDischargeLevel, p.dwMagneticDischargeTimeLeft, pcUser->uMagneticDischargeLevel, pcUser->dwMagneticDischargeTimeOut );

	//Whirl Wind
	p.bWhirlWind = pcUser->bWhirlWind;

	//Kiss Viper
	SetSkillTime( p.sKissViperLevel, p.dwKissViperTimeLeft, pcUser->uKissViperLevel, pcUser->dwKissViperTimeOut );
}

void UserServer::HandlePacket( User * pcUser, PacketPlayData * psPacket )
{
	if ( psPacket->bDie != pcUser->bDieUpdate )
	{
		if ( psPacket->bDie )
			ResetSkills( pcUser );

		pcUser->bDieUpdate = psPacket->bDie;
	}

	if ( (int)psPacket->sMapID != pcUser->pcUserData->iMapID )
	{
		if ( CHARACTERSERVER->CharacterInCityStage( pcUser->pcUserData ) )
			ResetSkills( pcUser );

		//Boss Time
		if ( (psPacket->sMapID >= 0) && (psPacket->sMapID < NUM_MAPS) )
			MAPSERVER->UpdateUserBossTime( pcUser, psPacket->sMapID );

		//Action Field Leave Map
		ACTIONFIELDHANDLER->OnLeaveMap( pcUser, psPacket->sMapID );

		SERVEREVENTHANDLER->OnUserMapChanged( pcUser, (EMapID)psPacket->sMapID );

		if ( pcUser->pcCaravan )
		{
			//Update Pet position when player change the map
			if ( FOLLOWUNITHANDLER->IsFollowUnitType( pcUser->pcCaravan ) )
			{
				Point3D sUserPosition = Point3D( psPacket->iLastX, 0, psPacket->iLastZ );
				FOLLOWUNITHANDLER->UpdateMap( pcUser->pcCaravan, (EMapID)psPacket->sMapID, sUserPosition );
			}
		}
	}
	
	bool bFirstPacket = false;
	//First?
	if ( pcUser->eMapID == MAPID_Invalid )
		bFirstPacket = true;


	pcUser->pcUserData->iMapID = (EMapID)psPacket->sMapID;
	pcUser->eMapID = pcUser->pcUserData->iMapID;
	pcUser->eSecondMapID = (EMapID)psPacket->sSecondMapID;
	pcUser->pcUserData->sCharacterData.sMP = psPacket->sMP;
	pcUser->pcUserData->sCharacterData.sSP = psPacket->sSP;
	pcUser->pcUserData->sCharacterData.iAttackSpeed		= psPacket->saSpeed[0];
	pcUser->pcUserData->sCharacterData.iMovementSpeed	= psPacket->saSpeed[1];
	pcUser->pcUserData->sAngle.iY = psPacket->sAngleY;
	pcUser->pcUserData->sPosition.iX = psPacket->iLastX;
	pcUser->pcUserData->sPosition.iZ = psPacket->iLastZ;
	pcUser->pcUserData->sCharacterData.sSize = psPacket->sSize;
	pcUser->bPvPMode = psPacket->bPvPMode;

	if ( bFirstPacket )
	{
		if ( pcUser->pcCaravan == nullptr )
		{
			if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanHopy ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Hopy" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanRudolph ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Rudolph" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanArma ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Arma" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanBuma ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Buma" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanCuepy ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Cuepy" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanHungky ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Hungky" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanRabie ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Rabie" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanTobie ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Tobie" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanSirKnight ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Sir Knight" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanPingu ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Pingu" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanYeti ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Yeti" );
			else if ( ITEMSERVER->GetPremiumUse( pcUser->pcUserData, ITEMTIMERTYPE_CaravanGoldenHopy ) )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Golden Hopy" );
		}
	}

	psPacket->iLevel = pcUser->pcUserData->sCharacterData.iLevel;

	//Buffer
	if ( psPacket->iPlayBuffCount < 32 )
	{
		int iIndex = 0;

		if ( psPacket->iPlayBuffCount <= 1 )
		{
			if ( pcUser->pcUserData->iMovingIndex == 0 )
				pcUser->pcUserData->iMovingIndex = 1;

			iIndex = USER_STATUS_UPDATE_MAX;
		}
		else
		{
			iIndex = pcUser->pcUserData->uUnitStatusNum % USER_STATUS_UPDATE_MAX;
			pcUser->pcUserData->iMovingIndex = 0;
			pcUser->pcUserData->uUnitStatusNum++;

			int iNewIndex = (iIndex + 1) % USER_STATUS_UPDATE_MAX;
			pcUser->pcUserData->dwaTimeUnitStatus[iNewIndex] = 0;
		}

		CopyMemory( pcUser->baUnitStatusNew[iIndex], psPacket, psPacket->iLength );
		pcUser->pcUserData->dwaTimeUnitStatus[iIndex] = psPacket->iLength;
	}
}

void UserServer::OnKilled( User * pcUser, PacketDeadCharacter * psPacket )
{
	ResetSkills( pcUser );

	User * pcKiller = USERDATATOUSER( USERDATABYID( psPacket->dwObjectID ) );
	if( pcKiller )
	{
		if( LOGIN_SERVER )
			PVPSERVER->OnKilled( pcKiller, pcUser );
		else if( GAME_SERVER )
			ITEMSERVER->ProcessIntegrity( pcUser, INTEGRITYUPDATE_KilledUser );

		SERVEREVENTHANDLER->OnUserKilled( pcKiller, pcUser );

		return;
	}
	else if ( GAME_SERVER )
	{
		ITEMSERVER->ProcessIntegrity( pcUser, INTEGRITYUPDATE_KilledMonster );

		SERVEREVENTHANDLER->OnUserKilled( pcUser );
	}

	if( LOGIN_SERVER )
		psPacket->iHeader = 0;
}

void UserServer::SettingsDataSet( User * pcUser, PacketSettingsData * psPacket )
{
	if( LOGIN_SERVER )
	{
		if( pcUser->sSettingsData.bNoWhisper != psPacket->bNoWhisper )
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Whisper %s!", pcUser->sSettingsData.bNoWhisper ? "enabled" : "disabled" );

		if( pcUser->sSettingsData.bNoTradeChat != psPacket->bNoTradeChat )
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Trade Chat %s!", pcUser->sSettingsData.bNoTradeChat ? "enabled" : "disabled" );

		if( pcUser->sSettingsData.bNoPartyRequest != psPacket->bNoPartyRequest )
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Party Requests %s!", pcUser->sSettingsData.bNoPartyRequest ? "enabled" : "disabled" );

		if( pcUser->sSettingsData.bNoTradeRequest != psPacket->bNoTradeRequest )
			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Trade Requests %s!", pcUser->sSettingsData.bNoTradeRequest ? "enabled" : "disabled" );
	}

	CopyMemory( &pcUser->sSettingsData, psPacket, sizeof( PacketSettingsData ) );
	pcUser->pcUserData->bDisabledWhisper = *(bool*)&pcUser->sSettingsData.bNoWhisper;
}

void UserServer::UpdatePetData( User * pcUser, PacketUpdatePetData * psPacket )
{
	switch( psPacket->dwSkillID )
	{
		case SKILLID_RecallHestian:
		case SKILLID_MetalGolem:
		case SKILLID_RecallWolverine:
		case SKILLID_FireElemental:
		case SKILLID_IceElemental:
		case SKILLID_SummonTiger:
			if( pcUser->pcUserData->pcPetData && pcUser->pcUserData->pcPetData->iID == pcUser->pcUserData->dwObjectPetID && pcUser->pcUserData->pcPetData->iPetID )
			{
				pcUser->pcUserData->pcPetData->bAggressive		= psPacket->bAutoAttack;
				pcUser->pcUserData->pcPetData->pcFocusTarget	= NULL;
				pcUser->pcUserData->pcPetData->pcAttackTarget	= NULL;
				pcUser->pcUserData->pcPetData->pcFocusingTarget = NULL;

				if ( pcUser->pcUserData->pcPetData->iFocusingLastID != (-1) )
				{
					PacketUpdatePetDataEx sPacket;
					sPacket.iLength = sizeof( PacketUpdatePetDataEx );
					sPacket.iHeader = PKTHDR_UpdatePetDataEx;
					sPacket.iTargetID = -1;
					sPacket.iLastTargetID = pcUser->pcUserData->pcPetData->iFocusingLastID;
					SENDPACKET( pcUser, &sPacket );
				}

				pcUser->pcUserData->pcPetData->iFocusingLastID	= -1;
			}
			break;

		default:
			break;
	}
}

void UserServer::UpdatePetDataEx( User * pcUser, PacketUpdatePetDataEx * psPacket )
{
	if( pcUser->pcUserData->pcPetData && pcUser->pcUserData->pcPetData->iID == pcUser->pcUserData->dwObjectPetID && pcUser->pcUserData->pcPetData->iPetID )
	{
		UnitData * pcUnitData = UNITSERVER->GetUnit( psPacket->iTargetID );

		if( pcUnitData )
		{
			psPacket->iLastTargetID = pcUser->pcUserData->pcPetData->iFocusingLastID;

			pcUnitData->Animate( ANIMATIONTYPE_Idle );

			pcUser->pcUserData->pcLastAttack						= NULL;
			pcUser->pcUserData->pcPetData->pcFocusTarget			= NULL;
			pcUser->pcUserData->pcPetData->pcAttackTarget			= NULL;
			pcUser->pcUserData->pcPetData->pcFocusingTarget			= pcUnitData;
			pcUser->pcUserData->pcPetData->iFocusingLastID			= pcUnitData->iID;

			CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Pet is focusing on %s", pcUnitData->sCharacterData.szName );
			SENDPACKET( pcUser, psPacket );
		}
	}
}

void UserServer::DisconnectOldUser( User * pcUser )
{
	for ( int i = 0; i < PLAYERS_MAX; i++ )
	{
		UserData * ud = &USERSDATA[i];
		if ( ud != pcUser->pcUserData && (ud->iID == pcUser->GetID() || STRINGCOMPAREI( CHARACTERSERVER->GetCharacterName( ud ), CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) )) )
		{
			if ( ud->pcSocketData )
				WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), ud->pcSocketData );
			else
			{
				ZeroMemory( ud, 0xB510 );
			}
		}
	}
}

void UserServer::ReconnectUser( User * pcUser, struct PacketReconnectUser * psPacket )
{

}

void UserServer::AddReconnectUser( User * pcUser )
{
}

BOOL UserServer::DeleteReconnectUser( const char * pszAccountName )
{
	BOOL bRet = FALSE;

	return bRet;
}

DWORD UserServer::AllowPacket( User * pcUser )
{
	DWORD dwTickCount = TICKCOUNT;

	UserData * ud = pcUser->pcUserData;

	if( pcUser->bNetServer == FALSE )
	{
		//Flood protection
		if ( ud->iPacketCounter > MAX_PACKETFLOW )
		{
			PacketLogCheat s;
			s.LParam = ud->iPacketCounter;
			s.SParam = MAX_PACKETFLOW;
			s.EParam = TIM_PACKETFLOW;
			s.iCheatID = CHEATLOGID_TooManyPackets;
			LOGSERVER->OnLogCheat( ud, &s );
			return 0;
		}

		if( (dwTickCount - ud->dwTickPacketCounter) > TIM_PACKETFLOW )
		{
			ud->iPacketCounter		= 0;
			ud->dwTickPacketCounter	= dwTickCount;
		}
		else
			ud->iPacketCounter++;
	}

	ud->dwTimeLastPacket = dwTickCount;

	return dwTickCount;
}

void UserServer::Update()
{
	//LoginServers and GameServers
	static UINT iWheel = 0;

	USERSERVER->uUsersInGame = 0;

	USERS_ONLINE = 0;

	for( UINT i = 0; i < PLAYERS_MAX; i++ )
	{
		User * pcUser			= pcaUser + i;
		UserData * pcUserData	= pcUser->pcUserData;

		if( pcUser && pcUserData && pcUser->IsConnected() )
		{
			if( pcUser->IsInGame() )
			{
				USERS_ONLINE++;

				USERSERVER->pcaUserInGame[USERSERVER->uUsersInGame] = pcUser;
				USERSERVER->uUsersInGame++;

				if( GAME_SERVER )
				{
					UpdateUnitStatus( pcUser );
					UpdateSkillStatus( pcUser );

					if ( (i % 16) == (iWheel % 16) )
					{
						pcUser->b16 = TRUE;
					}
					else
					{
						pcUser->b16 = FALSE;
					}

					//Frame Check
					if( (i % 32) == (iWheel % 32) )
					{
						//Status Update
						pcUser->b32 = TRUE;

						if( (i % 64) == (iWheel % 64) )
						{
							//Deep Status Update
							pcUser->b64 = TRUE;
						}
						else
						{
							//No Deep Status Update
							pcUser->b64 = FALSE;
						}
					}
					else
					{
						//No Status Update
						pcUser->b32 = FALSE;
						pcUser->b64 = FALSE;
					}
				}
			}

			if( (i % 512) == (iWheel % 512) )
			{ //Every 512
				DWORD dwDisconnectTime;

				//Old Disconnect Time
				dwDisconnectTime = pcUserData->dwDisconnectTime;

				if( dwDisconnectTime != 0 )
				{
					if ( TICKCOUNT > dwDisconnectTime && pcUserData->pcSocketData )
					{
						SocketData * sd = pcUserData->pcSocketData;
						if ( (sd->bKeepAlive == FALSE) && (pcUser->bNetServer == FALSE) )
						{
							pcUserData->dwDisconnectTime = 0;
							LOGSERVER->LogSocketClose( sd, "UserServer::Update()::OldDisconnectTime", _ReturnAddress() );
							WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), sd );
						}
					}
				}

				//Lag Grace
				DWORD dwTimeLastPacketReceived = 0;

				if ( pcUserData && pcUserData->pcSocketData )
					dwTimeLastPacketReceived = pcUserData->dwTimeLastPacket;

				if( dwTimeLastPacketReceived != 0 )
				{
					SocketData * sd = pcUserData->pcSocketData;

					if( sd && (sd->bKeepAlive == FALSE) && (pcUser->bNetServer == FALSE) )
					{
						DWORD dwLag = TICKCOUNT - dwTimeLastPacketReceived;

						if( dwLag > (5 * 60 * 1000) )
						{
							LOGSERVER->LogSocketClose( sd, "UserServer::Update()::LagGrace", _ReturnAddress() );
							WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), sd );
						}
					}
				}
			}
		}
	}

	iWheel++;
}

DWORD WINAPI UserServer::UsersUpdate()
{
	UpdateServerTime();
	BELLATRASERVER->UpdateClanBellatra();
	BOTSERVER->Tick();
	NETSERVER->Tick();
	HALLOWEENHANDLER->Tick();
	return TRUE;
}


void UserServer::Loop()
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	for ( UINT i = 0, j = uUsersInGame; i < j; i++ )
	{
		User * pcUser = pcaUserInGame[i];
		UserData * pcUserData = pcUser->pcUserData;

		DWORD dwTimeDifference = TICKCOUNT - pcUserData->dwTimeLastPacket;
		if ( dwTimeDifference < USER_STATUS_UPDATE_GRACE )
		{
			if ( pcUser->b16 )
			{
				//Send Unit Status of other Users to this User
				//LoopUsers( pcUser );
			}

			if ( pcUser->b32 )
			{
				if ( pcUser->b64 )
				{
					//Send Unit Status of other Monsters to this User
					LoopUnits( pcUser );
				}

				//Send Unit Status of other Users to this User
				LoopUsers( pcUser );	
			}
		}
	}
}

void UserServer::LoopUnits( User * pcUser )
{ //0x0055A590
	UserData * pcUserData = pcUser->pcUserData;

	//Alive Unit array
	UINT uUnitsInGame = UNITSERVER->uUnitsInGame;
	Unit ** pcaUnitInGame = UNITSERVER->pcaUnitInGame;

	//Receive Unit Status
	for ( UINT i = 0, j = uUnitsInGame; i < j; i++ )
	{
		Unit * pcUnit = pcaUnitInGame[i];
		UnitData * pcUnitData = pcUnit->pcUnitData;

		if ( pcUnitData->uUnitInfoSize != 0 )
		{
			BOOL bCanSee = pcUser->CanSee( pcUnit ) && ROOMINSTANCEHANDLER->CanSee( pcUser->GetID(), pcUnit->GetID() );
			if ( bCanSee )
			{
				if ( FOLLOWUNITHANDLER->IsFollowUnitType( pcUnit ) )
				{
					if ( User * pcUserCaravan = USERDATATOUSER( pcUnit->pcUnitData->pcOwner ) )
					{
						if ( pcUserCaravan->bInBattleRoyale || pcUserCaravan->bInGrandFury )
							bCanSee = FALSE;

						if ( pcUser->sSettingsData.bHideCaravans )
						{
							if ( pcUser->GetID() != pcUserCaravan->GetID() )
							{
								if ( MAPSERVER->IsTownMap( pcUnit->GetMapID() ) == false )
									bCanSee = FALSE;
							}
						}
					}
				}
			}
			else
			{
				if ( pcUnit->pcUnitData->pcOwner == pcUser->pcUserData )
					bCanSee = TRUE;

				if ( pcUser->GetMapID() == MAPID_Bellatra )
					if ( pcUnit->pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
						bCanSee = TRUE;
			}

			if ( bCanSee )
			{
				int iDistanceA, iDistanceB, iDistanceC;
				pcUserData->sPosition.GetPTDistanceXZ( &pcUnitData->sPosition, iDistanceA, iDistanceB, iDistanceC );

				if ( (iDistanceA < USER_SIGHTRANGE_FAR) && (iDistanceB < MISC_DISTANCE) && (iDistanceC < MISC_DISTANCE) )
				{
					BOOL bForceUnitStatus = FALSE;

					if ( pcUnitData->iSpecialType != 0 )
					{
						switch ( pcUnitData->sCharacterData.iMonsterEffectID )
						{
							case MONSTEREFFECTID_CASTLE_DOOR:
							case MONSTEREFFECTID_CASTLE_TOWER_B:
								bForceUnitStatus = TRUE;
								break;
						}
					}

					if ( pcUnit->bFollowUnitType )
						bForceUnitStatus = TRUE;

					if ( (bForceUnitStatus) || (iDistanceA < USER_UPDATERANGE_FAR) )
					{
						if ( pcUserData->uaUpdateCounter5[pcUnit->uIndex] != pcUnit->uLastUpdate )
						{
							SendUnitStatusM( pcUser, (Packet *)(pcUnit->baUnitBufferNew) );

							pcUnitData->iAliveCounter = 256;

							pcUserData->uaUpdateCounter5[pcUnit->uIndex] = pcUnit->uLastUpdate;
						}
					}
					else
					{
						if ( pcUnitData->GetAnimation() != ANIMATIONTYPE_Die )
						{
							PacketUnitStatusMove sMovePacket;
							sMovePacket.iLength = sizeof( sMovePacket );
							sMovePacket.iHeader = PKTHDR_UnitStatusMove;
							sMovePacket.lID = pcUnitData->iID;
							sMovePacket.sPosition = pcUnitData->sPosition;
							sMovePacket.sAngleX = pcUnitData->sAngle.iX;
							sMovePacket.sAngleY = pcUnitData->sAngle.iY;
							sMovePacket.sAngleZ = pcUnitData->sAngle.iZ;
							sMovePacket.sAnimationType = pcUnitData->GetAnimation();
							sMovePacket.iFrame = pcUnitData->iFrame;
							sMovePacket.dwHashCharacterName = pcUnitData->sUnitInfo.dwHashCharacterName;

							SendUnitStatusM( pcUser, &sMovePacket );

							pcUnitData->iAliveCounter = 256;

							//if( pcUserData->bShowDebugInfo )
							//	ChatServer::SendChatEx( pcUser, CHATCOLOR_Error, "> Move Status [%s]", pcUnitData->sCharacterData.szName );
						}
					}
				}
			}
		}
	}

	//Game Status Babel Storm Map
	if ( (pcUser->GetMapID() == (*MapServer::piBabelStormMapID)) && ((*MapServer::pdwBabelStormEndTime) > TICKCOUNT) )
	{
		PacketGameStatusBabelStormMap sPacket;
		sPacket.iLength = sizeof( sPacket );
		sPacket.iHeader = PKTHDR_GameStatus;
		sPacket.iStatusID = 0x200;
		sPacket.lID = pcUser->GetID();
		sPacket.eBabelStormMapID = (*MapServer::piBabelStormMapID);
		SendUnitStatusM( pcUser, &sPacket, FALSE );
	}

	//Game Status Bless Castle
	if ( pcUser->GetMapID() == MAPID_BlessCastle )
	{
		PacketGameStatusBlessCastle sPacket;
		sPacket.iLength = sizeof( sPacket );
		sPacket.iHeader = PKTHDR_GameStatus;
		sPacket.iStatusID = 0x210;
		sPacket.lID = pcUser->GetID();
		sPacket.dwNum1 = ((*(DWORD*)0x07AB1EF0) & 0xFFFF) | (*((DWORD*)0x07AB1EE8) << 16);
		sPacket.dwNum2 = *(DWORD*)0x07AB1EEC;
		sPacket.dwNum3 = *(DWORD*)0x07AB2054;
		sPacket.dwNum4 = *(DWORD*)0x07AB1F1C;
		sPacket.sConRate = pcUserData->sNumDeath;
		sPacket.sBattlePoints = pcUserData->sPVPKills;
		sPacket.dwNum6 = 0;
		SendUnitStatusM( pcUser, &sPacket, FALSE );

		pcUserData->sNumDeath = 0;
		pcUserData->sPVPKills = 0;
	}

	//Game Status PvP Map
	EMapID ePvPMapID = (EMapID)(READDWORD( 0x0084831C ));

	if ( MAPSERVER->IsPVPMap( pcUser->GetMapID() ) )
	{
		if ( pcUser->GetMapID() == MAPID_BlessCastle )
		{
			//Not Massacre or in Bless castle?
			if ( (READDWORD( 0x08B86750 ) != 2) || (READDWORD( 0x07AB1EE8 ) == 0) )
			{
				//Not Inside Area? 1 = Inside Area, 2 = target still in area + caution zone
				if ( (pcUser->pcUserData->sPosition.iZ > 0xFFA21500) )
				{

				}
				else
					ePvPMapID = pcUser->GetMapID();
			}
			else
				ePvPMapID = pcUser->GetMapID();
		}
		else
			ePvPMapID = pcUser->GetMapID();
	}


	if ( (ePvPMapID != MAPID_Invalid) && (pcUser->GetMapID() == ePvPMapID) )
	{
		PacketGameStatusPvPMap sPacket;
		sPacket.iLength = sizeof( sPacket );
		sPacket.iHeader = PKTHDR_GameStatus;
		sPacket.iStatusID = 0x220;
		sPacket.lID = pcUser->GetID();
		sPacket.ePvPMapID = ePvPMapID;
		SendUnitStatusM( pcUser, &sPacket, FALSE );
	}

	ClearUnitStatusM( pcUser );
}

void UserServer::LoopUsers( User * pcUser )
{ //0x005501C0
	UserData * pcUserData = pcUser->pcUserData;

	//Update Party
	if( pcUser->bParty && pcUser->psParty )
		PARTYHANDLER->UpdatePartyData( pcUser, pcUser->psParty );

	//Recovery
	ITEMSERVER->CheckItemRecovery( pcUser );

	//Update Drop Area
	DROPAREAHANDLER->SendUserArea( pcUser );

	//Do Chat Update?
	BOOL bChat = pcUserData->szChat[0] != 0;

	//Do Status Update?
	BOOL bStatus = (pcUserData->dwTimeLastStatusUpdate + USER_USER_STATUS_UPDATE_GRACE) >= TICKCOUNT;

	//PvP
	DAMAGEHANDLER->UpdateDamagePvPData( pcUser );

	//Update Chat or Status?
	if ( bChat || bStatus )
	{
		//Fill Chat Packet
		PacketChatBoxMessage sPacketChat;

		if ( bChat )
		{
			sPacketChat.iHeader = PKTHDR_ChatMessage;
			sPacketChat.iChatColor = CHATCOLOR_Normal;
			sPacketChat.lID = pcUserData->iID;
			STRINGCOPY( sPacketChat.szChatBoxMessage, pcUserData->szChat );

			sPacketChat.iLength = sizeof( Packet ) + sizeof( sPacketChat.iChatColor ) + sizeof( sPacketChat.lID ) + STRLEN( sPacketChat.szChatBoxMessage ) + 1;

			pcUserData->szChat[0] = 0;

            CHATSERVER->SQLLogChat( pcUser->iAccountID, pcUser->iCharacterID, 0, 0, sPacketChat.iChatColor, sPacketChat.szChatBoxMessage, pcUser->pcUserData->pcSocketData->szIP, "127.0.0.1" );
		}

		//Send Chat Message and Receive Unit Status
		for ( UINT i = 0, j = uUsersInGame; i < j; i++ )
		{
			User * pcOtherUser = pcaUserInGame[i];

			if ( pcOtherUser != pcUser )
			{
				UserData * pcOtherUserData = pcOtherUser->pcUserData;

				if ( bChat )
				{
					if ( pcUserData->sPosition.WithinPTDistance( &pcOtherUserData->sPosition, USER_TALKRANGE ) )
						SENDPACKET( pcOtherUser, &sPacketChat );
				}

				if ( bStatus )
				{
					//Battle Royale Update
					if ( pcUserData->sPosition.WithinPTDistance( &pcOtherUserData->sPosition, USER_TALKRANGE >> 1 ) )
					{
						pcUserData->uBattleRoyaleUsersNearCount++;
						if ( pcUserData->dwBattleRoyaleUsersNearTime < TICKCOUNT )
						{
							//Over the limit? Log it...
							if ( pcUserData->uBattleRoyaleUsersNearCount >= MAX_BATTLEROYALE_USERSRANGE )
								BATTLEROYALEHANDLER->SQLLogBattleRoyaleUsersInRange( pcUser );

							//Reset Timer
							pcUserData->uBattleRoyaleUsersNearCount = 0;
							pcUserData->dwBattleRoyaleUsersNearTime = TICKCOUNT + MAX_BATTLEROYALE_TIMERANGE;
						}
					}

					BOOL bCanSee = pcUser->CanSee( pcOtherUser ) && ROOMINSTANCEHANDLER->CanSee( pcUser->GetID(), pcOtherUser->GetID() );
					if ( (pcOtherUserData->uUnitStatusNum > 0) && (bCanSee) )
					{
						UINT uOtherIndex = pcOtherUser->uIndex;

						UINT uMyUnitStatusNumOfOther = pcUserData->uaUpdateCounter1[uOtherIndex];
						UINT uUnitStatusNumber = pcOtherUserData->uUnitStatusNum;

						int iDistanceA, iDistanceB, iDistanceC;
						pcUserData->sPosition.GetPTDistanceXZ( &pcOtherUserData->sPosition, iDistanceA, iDistanceB, iDistanceC );

						if ( (iDistanceA < USER_SIGHTRANGE_FAR) && (iDistanceB < MISC_DISTANCE) && (iDistanceC < MISC_DISTANCE) )
						{
							ID lOtherID = pcOtherUserData->iID;
							int iOtherMovingIndex = pcOtherUserData->iMovingIndex;

							UINT uUnitStatusIndex = 0;

							if ( pcUserData->uaUpdateCounter2[uOtherIndex] == lOtherID )
							{
								if ( (iOtherMovingIndex == 0) || (uMyUnitStatusNumOfOther < uUnitStatusNumber) )
								{
									if ( (uMyUnitStatusNumOfOther == 0) || (uMyUnitStatusNumOfOther >= uUnitStatusNumber) )
									{
										pcUserData->uaUpdateCounter1[uOtherIndex] = uUnitStatusNumber;
										continue;
									}
									else
									{
										UINT uTemp = uUnitStatusNumber - uMyUnitStatusNumOfOther;

										if ( uTemp > (USER_STATUS_UPDATE_MAX-1) )
											uMyUnitStatusNumOfOther = uUnitStatusNumber - (USER_STATUS_UPDATE_MAX-1);

										uUnitStatusIndex = uMyUnitStatusNumOfOther % USER_STATUS_UPDATE_MAX;

										uMyUnitStatusNumOfOther++;
										pcUserData->uaUpdateCounter1[uOtherIndex] = uMyUnitStatusNumOfOther;
									}
								}
								else
								{
									if ( pcUserData->uaUpdateCounter4[uOtherIndex] == 0 )
									{
										if ( pcOtherUser->uPriority > 0 )
										{
											UINT uCounter3 = (pcUserData->uaUpdateCounter3[uOtherIndex]++);

											BOOL bContinue;
											switch ( pcOtherUser->uPriority )
											{
												case 1:
													if ( uCounter3 > 2 )
													{
														bContinue = FALSE;
														break;
													}

													bContinue = TRUE;
													break;
												case 2:
													if ( uCounter3 > 4 )
													{
														bContinue = FALSE;
														break;
													}

													bContinue = TRUE;
													break;
												case 3:
													if ( uCounter3 > 8 )
													{
														bContinue = FALSE;
														break;
													}

													bContinue = TRUE;
													break;
												default:
													if ( uCounter3 > 12 )
													{
														bContinue = FALSE;
														break;
													}

													bContinue = TRUE;
													break;
											}

											if ( bContinue )
												continue;
										}
									}

									uUnitStatusIndex = USER_STATUS_UPDATE_MAX;
								}

								pcUserData->uaUpdateCounter3[uOtherIndex] = 0;
							}
							else
							{
								pcUserData->uaUpdateCounter1[uOtherIndex] = uUnitStatusNumber;
								pcUserData->uaUpdateCounter2[uOtherIndex] = lOtherID;
								pcUserData->uaUpdateCounter3[uOtherIndex] = 0;
								pcUserData->uaUpdateCounter4[uOtherIndex] = USER_STATUS_UPDATE_MAX;

								uUnitStatusIndex = USER_STATUS_UPDATE_MAX;
							}

							if ( (iDistanceA < USER_UPDATERANGE_FAR) || (uUnitStatusIndex == USER_STATUS_UPDATE_MAX) )
							{
								if ( pcUserData->uaUpdateCounter4[uOtherIndex] > 0 )
									pcUserData->uaUpdateCounter4[uOtherIndex]--;

								if ( pcOtherUserData->dwaTimeUnitStatus[uUnitStatusIndex] != 0 )
									SendUnitStatus( pcUser, (Packet *)(pcOtherUser->baUnitStatusNew[uUnitStatusIndex]) );

								SendSkillStatus( pcUser, &pcOtherUser->sSkillStatus );

								//CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Unit Status [%s] [%d]", pcOtherUserData->sCharacterData.szName, uUnitStatusIndex );
							}
							else
							{
								PacketUnitStatusMove sMovePacket;
								sMovePacket.iLength = sizeof( sMovePacket );
								sMovePacket.iHeader = PKTHDR_UnitStatusMove;
								sMovePacket.lID = pcOtherUserData->iID;
								sMovePacket.sPosition = pcOtherUserData->sPosition;
								sMovePacket.sAngleX = 0;
								sMovePacket.sAngleY = pcOtherUserData->sAngle.iY;
								sMovePacket.sAngleZ = 0;
								sMovePacket.sAnimationType = ANIMATIONTYPE_None;
								sMovePacket.iFrame = 0;
								sMovePacket.dwHashCharacterName = 0;

								SendUnitStatus( pcUser, &sMovePacket );

								//if( pcUserData->bShowDebugInfo )
								//	ChatServer::SendChatEx( pcUser, CHATCOLOR_Error, "> Move Status [%s]", pcOtherUserData->sCharacterData.szName );
							}
						}
						else
						{
							//Reset so it will refresh again next time even if same User is still logged in
							pcUserData->uaUpdateCounter2[uOtherIndex] = 0;
						}
					}
				}
			}
		}

		ClearUnitStatus( pcUser );
		ClearSkillStatus( pcUser );
	}
}

void UserServer::SendUnitStatus( User * pcUser, Packet * psPacket, BOOL bIncrementAmount )
{
	if ( psPacket->iLength > (MAX_PKTSIZ - 0x10) )
		return;

	psPacket->iEncKeyIndex = 0;
	psPacket->iEncrypted = 0;

	UINT uFreeSpace = (MAX_PKTSIZ - 0x10) - pcUser->uBufferPosition;

	if ( psPacket->iLength > uFreeSpace )
		ClearUnitStatus( pcUser );

	BYTE * buf = ((BYTE*)pcUser->sUnitStatusContainer.baBuffer) + pcUser->uBufferPosition;

	CopyMemory( buf, psPacket, psPacket->iLength );

	pcUser->uBufferPosition += psPacket->iLength;

	if ( bIncrementAmount )
		pcUser->sUnitStatusContainer.iAmount++;
}

void UserServer::ClearUnitStatus( User * pcUser )
{
	if ( (pcUser->sUnitStatusContainer.iAmount > 0) || (pcUser->uBufferPosition > 0) )
	{
		pcUser->sUnitStatusContainer.iLength = sizeof( Packet ) + sizeof( int ) + sizeof( int ) + pcUser->uBufferPosition;

		SENDPACKET( pcUser, &pcUser->sUnitStatusContainer );

		pcUser->uBufferPosition = 0;
		pcUser->sUnitStatusContainer.iAmount = 0;
	}
}

void UserServer::SendUnitStatusM( User * pcUser, Packet * psPacket, BOOL bIncrementAmount )
{
	if ( psPacket->iLength > (MAX_PKTSIZ - 0x10) )
		return;

	psPacket->iEncKeyIndex = 0;
	psPacket->iEncrypted = 0;

	UINT uFreeSpace = (MAX_PKTSIZ - 0x10) - pcUser->uBufferPositionM;

	if ( psPacket->iLength > uFreeSpace )
		ClearUnitStatusM( pcUser );

	BYTE * buf = ((BYTE*)pcUser->sUnitStatusContainerM.baBuffer) + pcUser->uBufferPositionM;

	CopyMemory( buf, psPacket, psPacket->iLength );

	pcUser->uBufferPositionM += psPacket->iLength;
	if ( bIncrementAmount )
		pcUser->sUnitStatusContainerM.iAmount++;
}

void UserServer::SendSkillStatus( User * pcUser, PacketSkillStatus * psPacket )
{
	if ( (pcUser->uBufferSkillPosition + psPacket->iLength) >= SKILLCONTAINER_LENGTH )
		ClearSkillStatus( pcUser );

	CopyMemory( pcUser->sSkillContainer.baBuffer + pcUser->uBufferSkillPosition, psPacket, sizeof( PacketSkillStatus ) );

	pcUser->uBufferSkillPosition += psPacket->iLength;

	pcUser->sSkillContainer.iAmount++;
}

void UserServer::ClearSkillStatus( User * pcUser )
{
	if ( (pcUser->sSkillContainer.iAmount > 0) || (pcUser->uBufferSkillPosition > 0) )
	{
		pcUser->sSkillContainer.iLength = sizeof( Packet ) + sizeof( int ) + pcUser->uBufferSkillPosition;
		pcUser->sSkillContainer.iHeader = PKTHDR_SkillStatusContainer;

		SENDPACKET( pcUser, &pcUser->sSkillContainer, TRUE );

		ZeroMemory( &pcUser->sSkillContainer, sizeof( PacketSkillStatusContainer ) );

		pcUser->sSkillContainer.iAmount = 0;
		pcUser->uBufferSkillPosition = 0;
	}
}


void UserServer::ClearUnitStatusM( User * pcUser )
{
	if ( (pcUser->sUnitStatusContainerM.iAmount > 0) || (pcUser->uBufferPositionM > 0) )
	{
		pcUser->sUnitStatusContainerM.iLength = sizeof( Packet ) + sizeof( int ) + sizeof( int ) + pcUser->uBufferPositionM;

		SENDPACKET( pcUser, &pcUser->sUnitStatusContainerM );

		pcUser->uBufferPositionM = 0;
		pcUser->sUnitStatusContainerM.iAmount = 0;
	}
}


void UserServer::UpdateID( User * pcUser )
{
	if ( pcUser && pcUser->pcUserData )
	{
		pcUser->iCharacterID = CHARACTERSERVER->SQLGetCharacterID( CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	}
}

int UserServer::GetNewUserID()
{
	return CALL( 0x0054CEE0 );
}
