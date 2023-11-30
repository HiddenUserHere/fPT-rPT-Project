#include "stdafx.h"
#include "servercore.h"

#include "CServer.h"
#include "CConfigFileReader.h"
#include "LevelTable.h"

DWORD * ServerCore::pdwTickCount = (DWORD *)0x07AC9D30;
DWORD * ServerCore::pdwUnitsOnline = (DWORD *)0x07AC9B94;
DWORD * ServerCore::pdwUsersOnline = (DWORD *)0x07AC9BC8;

extern void InitSkills();

void SetLevelTable()
{
	DWORD old;
	VirtualProtect((void*)0x00443980, 1000, PAGE_EXECUTE_READWRITE, &old);
	WRITEDWORD((DWORD)(0x00443980 + 1), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00443985 + 2), (DWORD)&LevelTable + 4);
	WRITEDWORD((DWORD)(0x004439AF + 3), (DWORD)&LevelTable + 4);
	WRITEDWORD((DWORD)(0x004439B8 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00443A09 + 3), (DWORD)&LevelTable + 4);
	WRITEDWORD((DWORD)(0x00443A14 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00443A40 + 3), (DWORD)&LevelTable + 4);
	WRITEDWORD((DWORD)(0x00443A4B + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00443A54 + 3), (DWORD)&LevelTable[1] + 0x4);
	WRITEDWORD((DWORD)(0x00443A5F + 3), (DWORD)&LevelTable[1]);
	WRITEDWORD((DWORD)(0x00443B2F + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00443B38 + 3), (DWORD)&LevelTable + 4);
	WRITEDWORD((DWORD)(0x00443B94 + 3), (DWORD)&LevelTable);
	WRITEDWORD((DWORD)(0x00443B9B + 3), (DWORD)&LevelTable + 4);
	WRITEDWORD((DWORD)(0x004439F1 + 3), (DWORD)&LevelTable - 0x4);
	WRITEDWORD((DWORD)(0x00443A00 + 3), (DWORD)&LevelTable - 0x8);


	VirtualProtect((void*)0x00443980, 1000, old, &old);
}
void ServerCore::LoadDirty()
{
	SetTickCounts( GetTickCount() );

	if ( GAME_SERVER )
		WRITEDWORD( 0x07AC9D3C, 1 ); //BOOL isGameServer

	string sName = "";
	DWORD dwIPKeyLogin = 0;
	BOOL bSiegeWar = FALSE;
	int iSiegeDay = 0;
	int iSiegeHour = 0;

	CConfigFileReader * pcConfigFileReader = new CConfigFileReader( "server.ini" );
	if ( pcConfigFileReader->Open() )
	{
		sName = pcConfigFileReader->ReadString( "LoginServer", "Name" );

		dwIPKeyLogin = (DWORD)StringIPToNumberIPV4( pcConfigFileReader->ReadString( "LoginServer", "IP" ).c_str() );

		pcConfigFileReader->Close();
	}
	DELET( pcConfigFileReader );


	//This partial with stage decompressing can and should be removed from server so these valeus can be removed safely anyway ;)
	ZeroMemory( (void *)0x008487B8, 7572 );
	ZeroMemory( (void *)0x00848320, 172 );
	ZeroMemory( (void *)0x0084A550, 908 );
	ZeroMemory( (void *)0x0084585C, 10948 );
	WRITEDWORD( 0x0088BCE4, 0 );
	// Open Server
	SERVER_WINDOW = -1;

	// Enable Record Save Character
	RECORD_SAVE_CHARACTER = TRUE;
	WRITEDWORD( 0x008487BC, 0x0320 );
	WRITEDWORD( 0x008487C0, 0x0258 );
	WRITEDWORD( 0x008487C4, 0x10 );
	WRITEDWORD( 0x0084A26C, 0 );
	WRITEDWORD( 0x008487C8, 0 );
	WRITEDWORD( 0x008487D0, 2 );
	WRITEDWORD( 0x008487CC, 1 );
	WRITEDWORD( 0x0084A400, 0xA0 );
	WRITEDWORD( 0x0084A404, 0x012C );
	WRITEDWORD( 0x0084A408, 0x0100 );
	WRITEDWORD( 0x0084A40C, -0x0100 );
	WRITEDWORD( 0x0084A410, 0x80 );

	*(int*)(0x0084831C) = -1;


	/* Core Server Defined Settings */
																//Server Code

	StringCchCopyA( SERVER_NAME, 32, sName.c_str() );											//Server Name

	WRITEDWORD( 0x0084609C, 1 );
	WRITEDWORD( 0x00845874, 1 );

	INI::CReader cReader( "server.ini" );

	if ( GAME_SERVER )
	{
		//Exp
		RATE_EXP = cReader.ReadInt( "Event", "RateExp" );
		EVENT_EXP = cReader.ReadInt( "Event", "EventExp" );

		//Bless Castle
		if ( cReader.ReadOnOff( "Event", "SiegeWar" ) )
		{
			EVENT_BLESSCASTLE = 1;
			EVENT_BLESSCASTLE_DAY = cReader.ReadInt( "Event", "SiegeWarDay" );
			EVENT_BLESSCASTLE_HOUR = cReader.ReadInt( "Event", "SiegeWarHour" );
		}

		CHARACTERSERVER->SetSiegeWarDay( cReader.ReadInt( "Event", "SiegeWarDay" ) );
		CHARACTERSERVER->SetSiegeWarHour( cReader.ReadInt( "Event", "SiegeWarHour" ) );

		//Bellatra
		if ( cReader.ReadOnOff( "Event", "Bellatra" ) )
		{
			EVENT_BELLATRA = 2;
			EVENT_BELLATRA_DIVSCORE = cReader.ReadInt( "Event", "BellatraDivScore" );
			EVENT_BELLATRA_TAX = cReader.ReadInt( "Event", "BellatraTax" );
			STRINGCOPYLEN( EVENT_BELLATRA_TEXT, 64, cReader.ReadString( "Event", "BellatraText" ).c_str() );
		}
		//Wanted Wolf
		if ( cReader.ReadOnOff( "Event", "WantedWolf" ) )
		{
			EVENT_WANTEDWOLF = cReader.ReadInt( "Event", "WantedWolfPercent" );
			EVENT_WANTEDWOLF_EXPMAP = cReader.ReadInt( "Event", "WantedWolfEXPMapPercent" );
		}

		//Wanted Moriph
		if ( cReader.ReadOnOff( "Event", "WantedMoriph" ) )
		{
			EVENT_WANTEDMORIPH = cReader.ReadInt( "Event", "WantedMoriphPercent" );
			EVENT_WANTEDMORIPH_EXPMAP = cReader.ReadInt( "Event", "WantedMoriphEXPMapPercent" );
		}
		//Easter
		if ( cReader.ReadOnOff( "Event", "Easter" ) )
		{
			EVENT_EASTER = TRUE;
			EVENT_EASTER_RATE = cReader.ReadInt( "Event", "EasterPercent" );
			EVENT_EASTER_RATEMAPEXP = cReader.ReadInt( "Event", "EasterEXPMapPercent" );
		}

		//Reduce Monster Damage
		if ( cReader.ReadOnOff( "Event", "MonsterDamageReduce" ) )
		{
			EVENT_REDUCE_DAMAGEMON = TRUE;
			EVENT_REDUCE_DAMAGEMON_LIVE = TRUE;
		}
	}

	//Carnival
	if ( cReader.ReadOnOff( "Event", "Carnival" ) )
		EVENT_CARNIVAL = TRUE;

	//PVPMode
	if ( cReader.ReadOnOff( "Event", "PvPServer" ) )
	{
		EVENT_PVPMODE = TRUE;
		RATE_EXPPVPMODE = cReader.ReadInt( "Event", "RateExpPvPMode" );
		RATE_DROPPVPMODE = cReader.ReadInt( "Event", "RateDropPvPMode" );
	}

	if ( cReader.ReadOnOff( "Event", "ActionField" ) )
		EVENT_ACTIONFIELD = TRUE;

	//Halloween
	if ( cReader.ReadOnOff( "Event", "Halloween" ) )
	{
		EVENT_HALLOWEEN = TRUE;
		RATE_HALLOWEEN = 50;
	}

	EVENT_BELLATRA_DIVSCORE = cReader.ReadInt( "Event", "BellatraDivScore" );
	EVENT_BELLATRA_TAX = cReader.ReadInt( "Event", "BellatraTax" );
	EVENT_BLESSCASTLE_MODE = cReader.ReadInt( "Event", "SiegeWarMode" );
	WRITEDWORD( 0x08B86750, EVENT_BLESSCASTLE_MODE );

	//Xmas
	if ( cReader.ReadOnOff( "Event", "Xmas" ) )
	{
		EVENT_XMAS = TRUE;
		RATE_XMAS = cReader.ReadInt( "Event", "XmasPercent" );
		RATE_XMAS_EXPMAP = cReader.ReadInt( "Event", "XmasEXPMapPercent" );
	}

	//The Grand Fury
	if ( cReader.ReadOnOff( "Event", "TheGrandFury" ) )
	{
		THEGRANDFURY_FLAG = TRUE;
	}

	WRITEDWORD( 0x0061A26C, 0x1400 );

	// Level Server
	*(int*)(0x08B80725) = 155;
	*(int*)(0x08B80739) = 155;
	*(int*)(0x08B80752) = 155;
	SetLevelTable();
	// Set Alloc Size
	{
		// Monsters
		*(UINT*)0x08B81328 = sizeof( CharacterData ) * (MAX_UNITS + 1);
		*(UINT*)0x08B8132C = sizeof( UnitInfo ) * (MAX_UNITS + 1);

		*(UINT*)0x08B81380 = sizeof( UnitData ) * (MAX_UNITS)+8;	// Alloc Size
		*(UINT*)0x08B81384 = (*(UINT*)0x08B81380) - 8;							    // Alloc Size
		*(UINT*)0x08B81388 = MAX_UNITS;								// Max pcUnitData online
		*(UINT*)0x08B8138C = ((*(UINT*)0x08B81380) - 8) / 4;						// Clear Memory size

																					// Items
		*(UINT*)0x08B81330 = sizeof( DefinitionItem ) * (ITEM_SERVER_MAX + 1);

		// Generate Item Form
		(*(WORD*)0x08B64170) = 27242;
	}


	// Item Requeriments
	{
		DWORD old;
		VirtualProtect( (void*)0x0043C22A, 4, PAGE_EXECUTE_READWRITE, &old );
		VirtualProtect( (void*)0x0043C3F1, 4, PAGE_EXECUTE_READWRITE, &old );
		*(UINT*)0x0043C22A = (UINT)&saItemRequeriments + 4;
		*(UINT*)0x0043C3F1 = (UINT)&saItemRequeriments[11];
		VirtualProtect( (void*)0x00439D9C, 4, PAGE_EXECUTE_READWRITE, &old );
		VirtualProtect( (void*)0x00439F3A, 4, PAGE_EXECUTE_READWRITE, &old );
		*(UINT*)0x00439D9C = (UINT)&saItemRequeriments + 4;
		*(UINT*)0x00439F3A = (UINT)&saItemRequeriments[11];
	}

	//Threading of these functions can be considered to be removed imo
	ZeroMemory( (void *)0x07A9EA60, 0xB0 );

	if ( LOGIN_SERVER )
	{
		// LOGS
		(*(BOOL*)0x07A9EA68) = 1;
		(*(DWORD*)(0x07A9EAA4)) = (DWORD)&AccountServer::SQLCharacterLog;
		(*(DWORD*)(0x07A9EB04)) = (DWORD)&AccountServer::SQLUserOnline;
	}

	CALL( 0x00467DC0 );

	InitSkills();

	ITEMSERVER->CreateMixingList();
	QUESTSERVER->UpdateQuestActiveMonsterKill();

	TEST( "Class Map", sizeof( BaseMap ), 0x0EA8 );
	
	int iMinute = (int)GetServerTime()->wMinute + 15;
	if ( iMinute > 59 )
		iMinute = iMinute - 60;

	BOSS_TIME = iMinute;
	(*(WORD*)0x07AC9F54) = GetServerTime()->wDay; //Boss Time

	MAPSERVER->SQLUpdateBossTime( BOSS_TIME, SERVER_CODE, (int)(*(WORD *)0x07AC9F54) );
}

void ServerCore::UnLoadDirty()
{
	CALL( 0x0054F6F0 ); //Unload .inx files
	CALL( 0x005492D0 ); //Unload Item Handling Thread Memory

	WRITEDWORD( 0x00726B38, 1 );  //Used by threads to check if server is shutting down, to ExitThread()
}

__declspec(naked) long ServerCore::Hash( char * s )
{
	JUMP( pfnHash );
}

void ServerCore::UpdateItemLevel( Item * psItem )
{
	int iLevel = psItem->iLevel;
	int iLevelAge = psItem->sAgeLevel;

	Item sItemOld;
	CopyMemory( &sItemOld, psItem, sizeof( Item ) );

	//Check Level
	if ( psItem->eCraftType == ITEMCRAFTTYPE_Aging )
	{
		if ( sItemOld.sAgeLevel > 0 )
			AGEHANDLER->OnDownAgeHandler( psItem, sItemOld.sAgeLevel - 1 );
	}
	else if ( psItem->eCraftType == ITEMCRAFTTYPE_Mixing )
	{
		MIXHANDLER->SetClearMixStatus( psItem, MIXHANDLER->GetIndexMixByNameID( sItemOld.eMixTypeName, sItemOld.sItemID.ToInt() ) );
	}

	BOOL bChanged = TRUE;

	switch ( psItem->sItemID.ToItemID() )
	{
		case ITEMID_InfernoAxe:
			psItem->iWeight += 1;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->sDamage.sMax += 1;
			break;
		case ITEMID_ZecramAxe:
			psItem->iStrength += 1;
			psItem->iSalePrice -= 16000;
			psItem->sDamage.sMin -= 2;
			psItem->sDamage.sMax -= 1;
			psItem->iAttackRating += 3;
			psItem->iCritical -= 1;
			break;

		case ITEMID_InfernoHammer:
			psItem->iStrength += 6;
			psItem->iWeight -= 4;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->sDamage.sMax += 1;
			psItem->iAttackRating += 6;
			break;
		case ITEMID_BaneHammer:
			psItem->iStrength += 12;
			psItem->iWeight -= 8;
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 2;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 12;
			break;

		case ITEMID_InfernoScythe:
			psItem->iStrength += 6;
			psItem->sIntegrity.sCur += 2;
			psItem->sIntegrity.sMax += 2;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->sDamage.sMax += 1;
			psItem->iAttackRating += 6;
			break;
		case ITEMID_ReaperScythe:
			psItem->iStrength += 12;
			psItem->sIntegrity.sCur += 4;
			psItem->sIntegrity.sMax += 4;
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 2;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 12;
			break;

		case ITEMID_EnigmaBow:
			psItem->iSpirit += 40;
			psItem->iAgility += 5;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->sDamage.sMax += 1;
			psItem->iAttackRating += 7;
			break;
		case ITEMID_ImmortalBow:
			psItem->iSpirit += 40;
			psItem->iAgility += 10;
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 2;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 14;
			break;

		case ITEMID_InfernoDagger:
			psItem->sIntegrity.sCur += 11;
			psItem->sIntegrity.sMax -= 3;
			psItem->iWeight += 2;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 2;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 8;
			break;
		case ITEMID_DevilianDagger:
			psItem->sIntegrity.sCur += 8;
			psItem->sIntegrity.sMax -= 1;
			psItem->iWeight += 4;
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 4;
			psItem->sDamage.sMax += 4;
			psItem->iAttackRating += 16;
			break;

		case ITEMID_InfernoClaw:
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->sDamage.sMax += 1;
			psItem->iAttackRating += 5;
			break;
		case ITEMID_InjusticeTalon:
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 2;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 10;
			break;

		case ITEMID_BlessedStaff:
			psItem->iSpirit -= 4;
			psItem->iWeight -= 3;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 3;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 7;
			break;
		case ITEMID_CelestialStaff:
			psItem->iSpirit += 2;
			psItem->iWeight -= 6;
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 4;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 13;
			break;

		case ITEMID_LutaPhantom:
			psItem->iStrength += 5;
			psItem->iSpirit += 5;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->iAttackRating += 5;
			psItem->iCritical -= 1;
			break;
		case ITEMID_AcalaPhantom:
			psItem->iStrength += 5;
			psItem->iSpirit += 10;
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 2;
			psItem->iAttackRating += 10;
			psItem->iCritical -= 1;
			break;

		case ITEMID_InfernoJavelin:
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->iAttackRating += 5;
			break;
		case ITEMID_HerecticJavelin:
			psItem->iSalePrice -= 44000;
			psItem->sDamage.sMin += 2;
			psItem->iAttackRating += 10;
			break;

		case ITEMID_InfernoSword:
			psItem->iStrength += 6;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 1;
			psItem->sDamage.sMax += 1;
			psItem->iAttackRating += 1;
			break;
		case ITEMID_ExtremeSword:
			psItem->iStrength -= 1;
			psItem->iSalePrice -= 88000;
			psItem->sDamage.sMin += 2;
			psItem->sDamage.sMax += 2;
			psItem->iAttackRating += 6;
			break;

		case ITEMID_InfernoArmor:
			psItem->fAbsorbRating += 1.2f;
			psItem->iDefenseRating += 16;
			if ( psItem->iDefenseRating > 485 )
				psItem->iDefenseRating = RandomI( 482, 485 );
			psItem->sSpecData.fSpecAbsorbRating += 0.2f;
			psItem->sSpecData.iSpecDefenseRating += 1;
			break;
		case ITEMID_FrenzyArmor:
			psItem->fAbsorbRating += 1.2f;
			psItem->iDefenseRating += 9;
			psItem->sSpecData.fSpecAbsorbRating += 0.2f;
			psItem->sSpecData.iSpecDefenseRating += 2;
			break;

		case ITEMID_InfernoRobe:
			psItem->fAbsorbRating += 0.7f;
			psItem->iDefenseRating += 5;
			if ( psItem->iDefenseRating > 395 )
				psItem->iDefenseRating = 395;
			psItem->sSpecData.fSpecAbsorbRating += 0.1f;
			psItem->sSpecData.iSpecDefenseRating += 3;
			break;
		case ITEMID_EternalRobe:
			psItem->fAbsorbRating += 1.8f;
			psItem->iDefenseRating += 15;
			if ( psItem->iDefenseRating > 420 )
				psItem->iDefenseRating = 419;
			psItem->sSpecData.fSpecAbsorbRating += 0.2f;
			psItem->sSpecData.iSpecDefenseRating += 6;
			break;

		case ITEMID_CrystalShield:
			psItem->sIntegrity.sCur += 5;
			psItem->fAbsorbRating -= 0.5f;
			psItem->iDefenseRating += 5;
			psItem->sSpecData.fSpecAbsorbRating += 0.1f;
			psItem->sSpecData.iSpecDefenseRating += 3;
			break;
		case ITEMID_DreadnaughtShield:
			psItem->sIntegrity.sCur += 10;
			psItem->sIntegrity.sMax += 5;
			psItem->fAbsorbRating += 0.8f;
			psItem->iDefenseRating += 4;
			psItem->sSpecData.fSpecAbsorbRating += 0.2f;
			psItem->sSpecData.iSpecDefenseRating += 6;
			break;

		case ITEMID_KelvezuClaw:
			psItem->iSpirit += 10;
			psItem->sIntegrity.sCur -= 4;
			psItem->sIntegrity.sMax -= 4;
			psItem->fAbsorbRating += 0.9f;
			psItem->iDefenseRating += 12;
			psItem->fAddMP = RandomF( 239.0f, 245.0f );
			psItem->sSpecData.iSpecDefenseRating += 3;
			psItem->sSpecData.fSpecMPRegen -= 0.1f;
			break;
		case ITEMID_AvernusBeads:
			psItem->iSpirit += 10;
			psItem->sIntegrity.sCur -= 4;
			psItem->sIntegrity.sMax -= 4;
			psItem->fAbsorbRating += 1.2f;
			psItem->iDefenseRating += 12;
			psItem->fAddMP = RandomF( 254.0f, 260.0f );
			psItem->sSpecData.iSpecDefenseRating += 6;
			psItem->sSpecData.fSpecMPRegen -= 0.2f;
			break;

		case ITEMID_GoldenBracelets:
			psItem->iAttackRating += 7;
			psItem->iDefenseRating = RandomI( 144, 150 );
			break;
		case ITEMID_PhoenixBracelets:
			psItem->iSalePrice -= 72000;
			psItem->iAttackRating += 14;
			psItem->iDefenseRating = RandomI( 160, 165 );
			break;

		case ITEMID_GoldenGauntlets:
			psItem->iStrength += 5;
			psItem->fAbsorbRating = RandomF( 11.5f, 11.7f );
			psItem->iDefenseRating = RandomI( 176, 184 );
			break;
		case ITEMID_PhoenixGauntlets:
			psItem->iStrength += 5;
			psItem->iSalePrice -= 72000;
			psItem->fAbsorbRating = RandomF( 12.9f, 13.3f );
			psItem->iDefenseRating = RandomI( 190, 199 );
			break;

		case ITEMID_MokovaBoots:
			psItem->iStrength -= 10;
			psItem->sIntegrity.sCur = 225;
			psItem->sIntegrity.sMax = 225;
			psItem->iWeight = 40;
			psItem->iSalePrice -= 100000;
			psItem->fAbsorbRating = RandomF( 13.2f, 14.9f );
			psItem->iDefenseRating = RandomI( 188, 200 );
			psItem->fMovementSpeed = RandomF( 12.0f, 13.8f );
			psItem->sSpecData.fSpecMovementSpeed = RandomF( 1.9f, 2.4f );
			psItem->sSpecData.fSpecAbsorbRating = RandomF( 1.0f, 1.5f );
			break;
		case ITEMID_GoldenBoots:
			psItem->fAbsorbRating = RandomF( 13.4f, 13.9f );
			psItem->iDefenseRating = RandomI( 178, 181 );
			psItem->fMovementSpeed = RandomF( 4.1f, 4.4f );
			psItem->sSpecData.fSpecMovementSpeed = RandomF( 1.7f, 2.3f );
			psItem->sSpecData.fSpecAbsorbRating = RandomF( 0.9f, 1.2f );
			break;
		case ITEMID_PhoenixBoots:
			psItem->iSpirit += 5;
			psItem->iAgility += 3;
			psItem->iSalePrice -= 72000;
			psItem->fAbsorbRating = RandomF( 14.9f, 15.4f );
			psItem->iDefenseRating = RandomI( 190, 196 );
			psItem->fMovementSpeed = RandomF( 4.3f, 4.6f );
			psItem->sSpecData.fSpecMovementSpeed = RandomF( 1.8f, 2.4f );
			psItem->sSpecData.fSpecAbsorbRating = RandomF( 1.0f, 1.3f );
			break;

		case ITEMID_SpiritAmulet:
			psItem->iLevel = 105;
			break;

		case ITEMID_SpiritRing:
			psItem->iLevel = 105;
			break;

		default:
			bChanged = FALSE;
			break;
	}

	
	ITEMSERVER->UpgradeNewItem( &sItemOld, psItem );

	ITEMSERVER->ValidateItem( psItem );
}

void ServerCore::UpdateCharacterItemsLevelData( const char * pszFile )
{
	static PacketCharacterRecordData sCharacterRecord;
	BOOL bRead = FALSE;

	FILE * fp = NULL;
	int iSizeFile = 0;
	fopen_s( &fp, pszFile, "rb" );
	if ( fp )
	{
		fseek( fp, 0, SEEK_END );
		iSizeFile = ftell( fp );
		
		fseek( fp, 0, SEEK_SET );
		fread( &sCharacterRecord, iSizeFile, 1, fp );
		fclose( fp );

		bRead = TRUE;
	}

	if ( bRead )
	{
		BYTE * pbaData = sCharacterRecord.baData;
		std::vector<RecordItem*> vRecordItem;

		for ( int i = 0; i < sCharacterRecord.iItemCount; i++ )
		{
			RecordItem * ps = new RecordItem;
			ITEMSERVER->DecodeItemsData( pbaData, ps, 0x7FFFF );

			//Check Throw
			int iIndex = -1;
			for ( int j = 0; j < sCharacterRecord.iDropItemCount; j++ )
			{
				DropItemData * psDrop = sCharacterRecord.saDropItemData + j;
				if ( (psDrop->iItemID == ps->sItem.sItemID.ToInt()) && (psDrop->iChk1 == ps->sItem.iChk1) && (psDrop->iChk2 == ps->sItem.iChk2) )
				{
					iIndex = j;
					break;
				}
			}

			ITEMSERVER->OnSaveItemData( &ps->sItem );

//			UpdateItemLevel( &ps->sItem );

			//ITEMSERVER->OnSaveItemData( &ps->sItem );

			if ( iIndex >= 0 )
			{
				DropItemData * psDrop	= sCharacterRecord.saDropItemData + iIndex;
				psDrop->iItemID			= ps->sItem.sItemID.ToInt();
				psDrop->iChk1			= ps->sItem.iChk1;
				psDrop->iChk2			= ps->sItem.iChk2;
			}

			vRecordItem.push_back( ps );

			//Next Item
			pbaData += ((int*)pbaData)[0];
		}

		pbaData = sCharacterRecord.baData;
		sCharacterRecord.iItemCount = 0;
		sCharacterRecord.iDataSize = 0;

		//Write Items
		for ( std::vector<RecordItem*>::iterator it  = vRecordItem.begin(); it != vRecordItem.end(); it++ )
		{
			RecordItem * ps = (*it);

			//Encode
			int iSize = ITEMSERVER->EncodeItemsData( ps, pbaData, sizeof( RecordItem ) );

			pbaData += iSize;
			sCharacterRecord.iDataSize += iSize;
			sCharacterRecord.iItemCount++;

			delete ps;
		}

		vRecordItem.clear();
	}

	if ( bRead )
	{
		return;

		FILE * fp = NULL;
		fopen_s( &fp, pszFile, "wb" );
		if ( fp )
		{
			fseek( fp, 0, SEEK_SET );
			fwrite( &sCharacterRecord, iSizeFile, 1, fp );
			fclose( fp );
		}
	}
}

void ServerCore::UpdateCharacterItemsLevelWarehouse( const char * pszFile )
{
	CWarehouseBase * pcWarehouse = WAREHOUSEHANDLER->Open( pszFile );
	if ( WAREHOUSEHANDLER->HaveFile( pcWarehouse ) )
	{
		if ( WAREHOUSEHANDLER->Decrypt( pcWarehouse ) )
		{
			for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
			{
				ItemData * pc = pcWarehouse->psWarehouse->saItem + i;

				if ( pc->bValid )
				{
					UpdateItemLevel( &pc->sItem );

					ITEMSERVER->OnSaveItemData( &pc->sItem );
				}
			}

			WAREHOUSEHANDLER->Encrypt( pcWarehouse );
			WAREHOUSEHANDLER->Save( pcWarehouse );
		}
	}
	WAREHOUSEHANDLER->Close( pcWarehouse );
}

void ServerCore::UpdateCharacterItemsLevel( const char * pszBaseFolder, const char * pszName )
{
	char szPath[MAX_PATH] = { 0 };

	STRINGFORMAT( szPath, "%s\\CharacterOld\\%s.chr", pszBaseFolder, pszName );
	ServerCore::UpdateCharacterItemsLevelData( szPath );
/*
	STRINGFORMAT( szPath, "%s\\CharacterBackup\\%s.chr", pszBaseFolder, pszName );
	ServerCore::UpdateCharacterItemsLevelData( szPath );

	STRINGFORMAT( szPath, "%s\\CharacterDelete\\%s.chr", pszBaseFolder, pszName );
	ServerCore::UpdateCharacterItemsLevelData( szPath );

	SQLCharacter s;
	if ( CharacterServer::SQLGetCharacter( (char*)pszName, &s ) )
	{
		STRINGFORMAT( szPath, "%s\\Warehouse\\%s.dat", pszBaseFolder, s.szAccountName );
		ServerCore::UpdateCharacterItemsLevelWarehouse( szPath );

		STRINGFORMAT( szPath, "%s\\WarehouseBackup\\%s.dat", pszBaseFolder, s.szAccountName );
		ServerCore::UpdateCharacterItemsLevelWarehouse( szPath );
	}
*/
}

void ServerCore::UpdateCharacterItemsLevelAll()
{
	//Character
	{
		char * pszFolder = "Data\\CharacterOld";

		WIN32_FIND_DATAA sFindData;
		char szFileName[MAX_PATH] = { 0 };
		HANDLE hFile = NULL;
		StringCchPrintfA( szFileName, MAX_PATH, "%s\\*.*", pszFolder );

		char szPath[512] = { 0 };

		if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
		{
			do
			{
				StringCchPrintfA( szPath, 512, "%s\\%s", pszFolder, sFindData.cFileName );
				ServerCore::UpdateCharacterItemsLevelData( szPath );
			}
			while ( FindNextFileA( hFile, &sFindData ) );

			FindClose( hFile );
		}
	}
/*
	//CharacterBackup
	{
		char * pszFolder = "Data\\CharacterBackup";

		WIN32_FIND_DATAA sFindData;
		char szFileName[MAX_PATH] = { 0 };
		HANDLE hFile = NULL;
		StringCchPrintfA( szFileName, MAX_PATH, "%s\\*.*", pszFolder );

		char szPath[512] = { 0 };

		if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
		{
			do
			{
				StringCchPrintfA( szPath, 512, "%s\\%s", pszFolder, sFindData.cFileName );
				ServerCore::UpdateCharacterItemsLevelData( szPath );
			}
			while ( FindNextFileA( hFile, &sFindData ) );

			FindClose( hFile );
		}
	}
	//CharacterDelete
	{
		char * pszFolder = "Data\\CharacterDelete";

		WIN32_FIND_DATAA sFindData;
		char szFileName[MAX_PATH] = { 0 };
		HANDLE hFile = NULL;
		StringCchPrintfA( szFileName, MAX_PATH, "%s\\*.*", pszFolder );

		char szPath[512] = { 0 };

		if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
		{
			do
			{
				StringCchPrintfA( szPath, 512, "%s\\%s", pszFolder, sFindData.cFileName );
				ServerCore::UpdateCharacterItemsLevelData( szPath );
			}
			while ( FindNextFileA( hFile, &sFindData ) );

			FindClose( hFile );
		}
	}
	//Warehouse
	{
		char * pszFolder = "Data\\Warehouse";

		WIN32_FIND_DATAA sFindData;
		char szFileName[MAX_PATH] = { 0 };
		HANDLE hFile = NULL;
		StringCchPrintfA( szFileName, MAX_PATH, "%s\\*.*", pszFolder );

		char szPath[512] = { 0 };

		if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
		{
			do
			{
				StringCchPrintfA( szPath, 512, "%s\\%s", pszFolder, sFindData.cFileName );
				ServerCore::UpdateCharacterItemsLevelWarehouse( szPath );
			}
			while ( FindNextFileA( hFile, &sFindData ) );

			FindClose( hFile );
		}
	}
	//WarehouseBackup
	{
		char * pszFolder = "Data\\WarehouseBackup";

		WIN32_FIND_DATAA sFindData;
		char szFileName[MAX_PATH] = { 0 };
		HANDLE hFile = NULL;
		StringCchPrintfA( szFileName, MAX_PATH, "%s\\*.*", pszFolder );

		char szPath[512] = { 0 };

		if ( hFile = FindFirstFileA( szFileName, &sFindData ) )
		{
			do
			{
				StringCchPrintfA( szPath, 512, "%s\\%s", pszFolder, sFindData.cFileName );
				ServerCore::UpdateCharacterItemsLevelWarehouse( szPath );
			}
			while ( FindNextFileA( hFile, &sFindData ) );

			FindClose( hFile );
		}
	}
	*/
}
