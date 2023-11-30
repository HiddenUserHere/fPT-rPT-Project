#include "stdafx.h"
#include "DLL.h"

extern int __cdecl RecvSkillSingleTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageNormalEx * psPacketEx );
extern int __cdecl RecvSkillAreaTarget( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacket, PacketSkillDamageRange * psPacketRange, PacketSkillDamageRangeEx * psPacketRangeEx );
extern int __cdecl RecvBuffSkill( UserData * pcUserData, UINT packet );
extern void __cdecl SendAttackRatting( UserData * pcUserData, UINT iAttackRating, UINT idEnemy );

void __cdecl InitServer()
{
	
}

void __cdecl InitSocketServer( HWND hWnd )
{
	
}

void __cdecl CreateUnitEnemyData()
{
	UNITINFODATA->CreateUnitEnemyData();
}

void __cdecl CreateItemMemoryTable()
{
	UNITINFODATA->CreateItemMemoryTable();
}

void __cdecl ReadUnitNpcStand( Map * pStageData )
{
	UNITINFODATA->ReadUnitNpcStand( pStageData );
}

void __cdecl ReadUnitNpcData( CharacterData * pCharInfo, UnitInfo *pMonInfo, char *pDialogMessage )
{
	UNITINFODATA->ReadUnitNpcData( pCharInfo, pMonInfo, pDialogMessage );
}

void __cdecl LoadAreaStages()
{
	MAPSERVER->Load();
}

void __cdecl ReadUnitSpawnData( Map * pStageData )
{
	UNITINFODATA->ReadUnitSpawnData( pStageData );
}

void __cdecl ReadUnitEnemyStage( Map::SpawnSetting * pMonsterStageList )
{
	UNITINFODATA->ReadUnitEnemyStage( pMonsterStageList );
}

BOOL __stdcall AddMonsterSpawn( UnitData * pcUnitData )
{
	Map * pcMap = NULL;
	READTHIS( pcMap );
	return MAPSERVER->AddMonsterSpawn( pcMap, pcUnitData );
}

BOOL __stdcall DelMonsterSpawn( UnitData * pcUnitData )
{
	Map * pcMap = NULL;
	READTHIS( pcMap );
	return MAPSERVER->DelMonsterSpawn( pcMap, pcUnitData );
}

int __cdecl OnLoginAccount( UserData * pcUserData, const char * pszUserID, const char * pszPassword )
{
	return 0;
}

BOOL __cdecl OnEvade( UserData * pcUserData, DWORD dwObjectID )
{
	if( CAssassin::OnAlasEvade( pcUserData, dwObjectID ) )
		return TRUE;

	return FALSE;
}

BOOL __cdecl OnLoginSuccess( UserData * pcUserData )
{
	return ACCOUNTSERVER->OnLoginSuccess( pcUserData );
}

void __cdecl SendDebugChat( UserData * pcUserData, const char * pszText )
{
	CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, pszText );
}

void __cdecl SendBossMessage( BaseMap * pcBaseMap, UINT pUnit )
{
	UNITSERVER->SendBossMessage( pcBaseMap, pUnit );
}
BOOL __cdecl OnUnitNpcClick( UserData * pcUserData, UnitData * pcUnitData )
{
	return UNITSERVER->OnUnitNpcClick( pcUserData, UNITDATATOUNIT( pcUnitData ) );
}

void __cdecl OnSendExp( UserData * pcUserData, UnitData * pcUnitData )
{
	UNITSERVER->OnSendExp( pcUserData, pcUnitData );
}

void __cdecl OnSetItemPerfect( Item * pItem )
{
	ITEMSERVER->OnSetItemPerfect( pItem );
}

void __cdecl OnReadPostBox( UserData * pcUserData )
{
	ACCOUNTSERVER->OnReadPostBox( pcUserData );
}

void __cdecl OnDeletePostBox( UINT ItemID )
{
	ACCOUNTSERVER->OnDeletePostBox( ItemID );
}

BOOL __cdecl ReadUnitAnimation( char * pszModelFile, ModelDataOld * pModelData )
{
	BOOL bResult = UNITINFODATA->ReadUnitAnimation( pszModelFile, pModelData );
	return bResult;
}

UINT __cdecl ReadModelSize( const char * pszModelFile )
{
	UINT uResult = UNITINFODATA->ReadModelSize( pszModelFile );
	return uResult;
}

int __cdecl UseDistortionMonster( UserData * pcUserData, PacketSkillDamageRange * psPacket, int iLevelSkill )
{
	return CMagician::UseDistortionMonster( pcUserData, psPacket, iLevelSkill );
}

void __cdecl SendMixDescription( UserData * pcUserData, UINT puMix )
{
	ITEMSERVER->SendMixDescription( pcUserData, puMix );
}

BOOL __cdecl OnSaveItemData( Item * psItem )
{
	return ITEMSERVER->OnSaveItemData( psItem );
}

BOOL __cdecl OnLoadItemData( ItemLoadData * psItem, const char * pszCode )
{
	return ITEMSERVER->OnLoadItemData( psItem, pszCode );
}

BOOL __cdecl RecordPoint( UserData * pcUserData, int iLevel, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode )
{
	return BELLATRASERVER->RecordPoint( pcUserData, iLevel, SodScore, SodKillCount, SodTotalScore, SodTotalSucessUser, SodTotalUser, SodCode );
}

BOOL __cdecl AddUserOnline( UserData * pcUserData, BOOL bOffline )
{
	return USERSERVER->AddUserOnline( pcUserData, bOffline );
}

int __cdecl AgingChanceHandler( UserData * pcUserData )
{
	return AGEHANDLER->AgingChanceHandler( USERDATATOUSER( pcUserData ) );
}

void __cdecl OnLogWrite( UserData * pcUserData, int iLogID, const char * pszLog, int iLogType )
{
	LOGSERVER->OnLogWrite( pcUserData, iLogID, pszLog, ( LogServer::ELogServerType )iLogType );
}

void __cdecl OnLogItem( int iType, int iSize, void * pLog )
{
	LOGSERVER->OnLogItem( iType, iSize, pLog );
}

void __cdecl SetupMonsterBellatra( UINT BellatraMemory, UINT ConfigMemory )
{
	UNITINFODATA->SetupMonsterBellatra( BellatraMemory, ConfigMemory );
}

BOOL __cdecl OnItemPremium( DWORD dwCode, int iX, int iY, int iZ, UserData * pcUserData )
{
	typedef BOOL( __cdecl *t_PremiumUse )( DWORD dwCode, int iX, int iY, int iZ, UserData * pcUserData );
	t_PremiumUse PremiumUse = (t_PremiumUse)0x00558140;
	if( !ITEMSERVER->OnPremiumItem( dwCode, iX, iY, iZ, pcUserData ) )
		return PremiumUse( dwCode, iX, iY, iZ, pcUserData );
	return TRUE;
}

void __cdecl OnSetDrop( UserData * pcUserData, UnitData * pcUnitData )
{
	UNITSERVER->OnSetDrop( pcUserData, pcUnitData );
}

BOOL __cdecl SendChatServer( SocketData * pcSocketData, void * pData )
{
	CHATSERVER->SendChatSpyMember( USERSERVER->GetUserdataSocket( pcSocketData ), pData );
	return TRUE;
}

BOOL __cdecl SocketAccept( SOCKET s, sockaddr_in * addr )
{
	//SOCKETSERVER->SocketAccept( s, addr );
	return TRUE;
}

void __cdecl SocketPacket( SocketData * sd, PacketReceiving * p )
{
	SOCKETSERVER->SocketPacket( sd, p );
}

void __stdcall SocketSend( PacketSending * p, BOOL bEncrypt )
{
	SocketData * sd = NULL;
	READTHIS( sd );
	if ( sd )
		SENDPACKETSOCKET( sd, (Packet *)p, bEncrypt );
}

void __cdecl ReadSocketHandler( DWORD s, DWORD dwParam )
{
	SOCKETSERVER->OnReadHandler( (SOCKET)s, dwParam );
}

void __cdecl OnLoadCharacterInventory( UserData * pcUserData )
{
	LOGSERVER->OnLogLoadItemCharacter( pcUserData );
}

BOOL __cdecl InventoryServerCheck( UserData * pcUserData, int iUnused )
{
	return USERSERVER->InventoryServerCheck( pcUserData, iUnused );
}

void __cdecl SendChatTrade( UserData * pcUserData, PacketChatBoxMessage * pPacket )
{
}

void __cdecl OnDisconnectUser( SocketData * pcSocketData )
{
	if( pcSocketData )
	{
		LOGSERVER->LogSocketClose( pcSocketData, "OnDisconnectUser [0x0055C390]", _ReturnAddress() );

		if( pcSocketData->u && pcSocketData->u->bNetServer )
		{
			WRITEERR( "NET SERVER TRIED DC %p", _ReturnAddress() );
			return;
		}
		if( NETSERVER->GetNetConnectionBySocketData( pcSocketData ) != NULL )
		{
			WRITEERR( "NET SERVER TRIED DC %p", _ReturnAddress() );
			return;
		}
		WNDPROC_SOCKETCLOSE( SOCKETSERVER->GetHWND(), pcSocketData );
	}
}

BOOL __cdecl OnDisconnectUserEx( UserData * pcUserData )
{
	if( pcUserData )
		LOGSERVER->LogSocketClose( pcUserData->pcSocketData, "OnDisconnectUserEx (Hook)", _ReturnAddress() );

	if( pcUserData )
		pcUserData->bDisconnectFunction = TRUE;
	ACCOUNTSERVER->OnDisconnectUser( pcUserData );
	return FALSE;
}

BOOL __cdecl CheatLog( UserData * pcUserData, void * pCheatLog )
{
	return LOGSERVER->OnLogCheat( pcUserData, pCheatLog );
}

void __cdecl OnHandlerFunctions( DWORD dwCode, DWORD * pMemory )
{
	return USERSERVER->OnHandlerFunctions( dwCode, pMemory );
}

void __cdecl OnSetTargetPosition( UserData * pcUserData, UnitData * pcUnitData )
{
	UNITSERVER->OnSetTargetPosition( pcUserData, pcUnitData );
}

UINT __cdecl AllocMemoryPT( UINT uSize )
{
	UINT uMemory = (UINT)VirtualAlloc( NULL, uSize + 4, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
	*( (UINT*)uMemory ) = uSize;
	return ( uMemory + 4 );
}

void __cdecl DeallocMemoryPT( UINT uMemory )
{
	VirtualFree( (void*)( uMemory ), *(UINT*)( uMemory - 4 ), MEM_RELEASE );
}

void __cdecl OnOutCharacter( UserData * pcUserData )
{
	CHARACTERSERVER->OnOutCharacterData( pcUserData );
}

BOOL __stdcall SetSpawnUnitData( UnitData * pcUnitData )
{
	Map * pcMap;
	READTHIS( pcMap );
	return MAPSERVER->SetSpawnMonster( pcMap, pcUnitData );
}

BOOL __cdecl UnitAutoMain( UnitData * pcUnitData )
{
	return UNITSERVER->UpdateUnitData( pcUnitData );
}

DWORD __cdecl OnSetClassJobItem( int iClass, DefinitionItem * ps )
{
	return (DWORD)ItemServer::GetItemSpecClassJob( iClass, ps );
}

BOOL __cdecl UpdateBellatra( Map * pcStage, MonsterMapSpawnHandle * psMonsterData )
{
	return BELLATRASERVER->UpdateServer( pcStage, psMonsterData );
}

BOOL __cdecl OnDownAgeHandler( Item * psItem, int iCount )
{//0x004BD8F0
	return AGEHANDLER->OnDownAgeHandler( psItem, iCount );
}

BOOL __cdecl OnUpAgeHandler( Item * psItem )
{//0x004BD540
	return AGEHANDLER->OnUpAgeHandler( psItem );
}

BOOL __cdecl OnCrystalUse( Item * psItem, int iX, int iY, int iZ, UserData * pcUserData )
{
	return MAPSERVER->SpawnCrystal( psItem, iX, iY, iZ, pcUserData );
}

BOOL __cdecl UpdateDamageDataPVP( UserData * pcKiller, UserData * pcVictim, int iDamage )
{
	return DAMAGEHANDLER->UpdateDamageDataPVP( pcKiller, pcVictim, iDamage );
}

BOOL __cdecl IsAreaDamage()
{
	return FALSE;
}

BOOL __cdecl SendPvPDamage( UserData * pcUserData, UserData * pcTarget, int iDamage, int iID, int iParam1, int iParam2 )
{
	return DAMAGEHANDLER->SendDamagePvPData( pcUserData, pcTarget, iDamage, iID, iParam1, iParam2 );
}

int __cdecl IsFreezedUnitFollow( UnitData * pcUnitData )
{
	return UNITSERVER->IsFreezedUnitFollow( pcUnitData );
}

BOOL __cdecl CanHitTargetAreaDamage( UserData * pcUserData, UnitData * pcUnitData )
{
	return DAMAGEHANDLER->CanHitTargetAreaDamage( pcUserData, pcUnitData );
}

int __stdcall MakeFaceArea( int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom )
{
	Stage * pcThis;
	READTHIS( pcThis );

	return MAPSERVER->MakeFaceArea( pcThis, iX, iZ, iWidth, iHeight, iTop, iBottom );
}

BOOL __stdcall SetupPolygonArea()
{
	Stage * pcThis;
	READTHIS( pcThis );

	return MAPSERVER->SetupPolygonArea( pcThis );
}

BOOL __stdcall SendItemNearUsers( Map::Loot * psItemStage )
{
	Map * pcMap;
	READTHIS( pcMap );

	return MAPSERVER->SendItemStageNearUsers( pcMap, psItemStage );
}

BOOL __cdecl ValidateCharacterStatusPoint( UserData * pcUserData, PacketCharacterRecordData * psPacket )
{
	return CCharacterDataHandler::ValidateStatusPoints( USERDATATOUSER( pcUserData ), psPacket->sCharacterData, psPacket->sCharacterSaveData.iQuestFlag );
}

BOOL __stdcall MoveUnitDataGate( int iDistance )
{
	UnitData * pcUnitData;
	READTHIS( pcUnitData );

	return pcUnitData->Move( iDistance );
}

int __cdecl FindInventoryItemServer( UserData * pcUserData, int iItemID, int iChk1, int iChk2 )
{
	return ITEMSERVER->FindItemInventory( pcUserData, iItemID, iChk1, iChk2 );
}

int __cdecl DelInventoryItemServer( UserData * pcUserData, int iItemID, int iChk1, int iChk2 )
{
	return ITEMSERVER->UseItem( pcUserData, iItemID, iChk1, iChk2 );
}

int __cdecl AddInventoryItemServer( UserData * pcUserData, int iItemID, int iChk1, int iChk2 )
{
	return ITEMSERVER->AddItemInventory( pcUserData, iItemID, iChk1, iChk2 );
}

BOOL __cdecl IsItemSafeGate( UserData * pcUserData, int iItemID, int iChk1, int iChk2, int iGold, DWORD dwSec1, DWORD dwSec2, DWORD dwSec3, DWORD dwSec4, int * piaPotionCount )
{
	return ITEMSERVER->IsItemSafe( pcUserData, iItemID, iChk1, iChk2, iGold, dwSec1, dwSec2, dwSec3, dwSec4, piaPotionCount );
}

int __cdecl AddPotionGate( UserData * pcUserData, EItemID iItemID, int iCount )
{
	return ITEMSERVER->AddPotion( pcUserData, iItemID, iCount );
}

BOOL __cdecl OnCharacterLoginLogoutGate( int iAction, const char * pszIP, const char * pszAccountName, const char * pszCharacterName, const char * pszCode, int iCharacterRank, int iCharacterClass, INT64 iCharacterEXP, UINT uCharacterLevel, int iCharacterGold )
{
	AccountServer::SQLCharacterLog( pszAccountName, iCharacterClass, pszCharacterName, pszCode, pszIP, ((char *)0x006E2660), uCharacterLevel, iCharacterEXP, iCharacterGold, iAction );
	return TRUE;
}

BOOL __cdecl OnUserOnlineGate( UserData * pcUserData, BOOL bOffline )
{
	const char * pszIP = pcUserData->pcSocketData ? pcUserData->pcSocketData->szIP : "";

	AccountServer::SQLUserOnline( pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUserData ), pszIP, pcUserData->iTicket, bOffline, pcUserData->sCharacterData.iClass, pcUserData->sCharacterData.iLevel );
	return TRUE;
}

void __cdecl AddItemServerStatusGate( Item * psItem, DefinitionItem * psDefinitionItem )
{
	bool bPerfectItem = (READDWORD( 0x08B70264 ) == 1);

	if ( psDefinitionItem->sItem.sItemID.ToItemID() == ITEMID_DraxosBoots )
	{
		psItem->bEvade = (BYTE)RandomI( 1, 3 );

		if ( bPerfectItem )
			psItem->bEvade = 3;
	}

	EItemID eItemID = psDefinitionItem->sItem.sItemID.ToItemID();

	switch ( eItemID & 0xFFFFFF00 )
	{
		case ITEMID_DefaultCostumeSkinAxe:
		case ITEMID_DefaultCostumeSkinClaw:
		case ITEMID_DefaultCostumeSkinDagger:
		case ITEMID_DefaultCostumeSkinHammer:
		case ITEMID_DefaultCostumeSkinWand:
		case ITEMID_DefaultCostumeSkinPhantom:
		case ITEMID_DefaultCostumeSkinScythe:
		case ITEMID_DefaultCostumeSkinBow:
		case ITEMID_DefaultCostumeSkinSword:
		case ITEMID_DefaultCostumeSkinJavelin:
		case ITEMID_DefaultCostumeSkinShield:
			psItem->bCostumeItem = 1;
			break;

		case ITEMID_DefaultCostumeSkinAxeTime:
		case ITEMID_DefaultCostumeSkinClawTime:
		case ITEMID_DefaultCostumeSkinDaggerTime:
		case ITEMID_DefaultCostumeSkinHammerTime:
		case ITEMID_DefaultCostumeSkinWandTime:
		case ITEMID_DefaultCostumeSkinPhantomTime:
		case ITEMID_DefaultCostumeSkinScytheTime:
		case ITEMID_DefaultCostumeSkinBowTime:
		case ITEMID_DefaultCostumeSkinSwordTime:
		case ITEMID_DefaultCostumeSkinJavelinTime:
		case ITEMID_DefaultCostumeSkinShieldTime:
			psItem->bCostumeItem = 2;
			break;
	}

	psItem->bLockedItemType = 0;


	if ( auto psDefinitionItemEx = UNITINFODATA->GetItemDefinitionExByItemID( eItemID ) )
	{
		psItem->sSpecData.sSpecPenetration = bPerfectItem ? psDefinitionItemEx->sSpecPenetration.sMax : RandomS( psDefinitionItemEx->sSpecPenetration.sMin, psDefinitionItemEx->sSpecPenetration.sMax );

		psItem->sSpecData.sSpecHPPotionEfficience = bPerfectItem ? psDefinitionItemEx->sSpecHPPotionEfficience.sMax : RandomS( psDefinitionItemEx->sSpecHPPotionEfficience.sMin, psDefinitionItemEx->sSpecHPPotionEfficience.sMax );
		psItem->sSpecData.sSpecMPPotionEfficience = bPerfectItem ? psDefinitionItemEx->sSpecMPPotionEfficience.sMax : RandomS( psDefinitionItemEx->sSpecMPPotionEfficience.sMin, psDefinitionItemEx->sSpecMPPotionEfficience.sMax );
		psItem->sSpecData.sSpecSPPotionEfficience = bPerfectItem ? psDefinitionItemEx->sSpecSPPotionEfficience.sMax : RandomS( psDefinitionItemEx->sSpecSPPotionEfficience.sMin, psDefinitionItemEx->sSpecSPPotionEfficience.sMax );
	}
}

void __cdecl SetMixStatusGate( Item * psItem, int iIndex )
{
	return MIXHANDLER->SetMixStatusNPC( psItem, iIndex );
}

BOOL __cdecl KillUnitNearBoss( UnitData * pcUnitData1, UnitData * pcUnitData2 )
{
	auto VerifyKill = []( UnitData * _pcUnitData1, UnitData * _pcUnitData2 ) -> BOOL
	{
		//Both Crystal? Can't kill...
		if ( _pcUnitData1->sCharacterData.iMonsterType == MONSTERTYPE_Friendly )
			if ( _pcUnitData2->sCharacterData.iMonsterType == MONSTERTYPE_Friendly )
				return TRUE;

		//Is Monster and Boss?
		if ( (_pcUnitData1->sCharacterData.iMonsterType != MONSTERTYPE_Friendly) && (_pcUnitData1->sCharacterData.sGlow == TRUE) )
		{
			//Crystal and isn't Boss?
			if ( (_pcUnitData2->sCharacterData.iMonsterType == MONSTERTYPE_Friendly) && (_pcUnitData2->sCharacterData.sGlow != TRUE) )
			{
				if ( _pcUnitData1->sPosition.WithinPTDistance( &_pcUnitData2->sPosition, 1500 ) )
				{
					_pcUnitData2->Animate( ANIMATIONTYPE_Die );
					_pcUnitData2->sCharacterData.sHP.sCur = 0;

					return TRUE;
				}
			}
		}

		return FALSE;
	};

	if ( pcUnitData1->sCharacterData.iType == CHARACTERTYPE_NPC )
		return FALSE;

	if ( pcUnitData2->sCharacterData.iType == CHARACTERTYPE_NPC )
		return FALSE;

	if ( VerifyKill( pcUnitData1, pcUnitData2 ) )
		return FALSE;

	if ( VerifyKill( pcUnitData2, pcUnitData1 ) )
		return FALSE;

	return TRUE;
}

BOOL __cdecl HandleUnitDamageTop( UserData * pcUserData, UnitData * pcUnitData, int iDamage )
{
	return UNITSERVER->HandleUnitDamageTop( pcUserData, pcUnitData, iDamage );
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		(*(DWORD*)0x08B64330) = (DWORD)&RecvSkillSingleTarget;
		(*(DWORD*)0x08B64334) = (DWORD)&RecvSkillAreaTarget;
		(*(DWORD*)0x08B64338) = (DWORD)&SocketSend;
		(*(DWORD*)0x08B6433C) = (DWORD)&OnLoginAccount;
		(*(DWORD*)0x08B64340) = (DWORD)&OnLoginSuccess;
		(*(DWORD*)0x08B64344) = (DWORD)&CreateItemMemoryTable;
		(*(DWORD*)0x08B64348) = (DWORD)&LoadAreaStages;
		(*(DWORD*)0x08B6434C) = (DWORD)&OnCrystalUse;
		(*(DWORD*)0x08B64350) = (DWORD)&OnSetItemPerfect;
		(*(DWORD*)0x08B64354) = (DWORD)&CreateUnitEnemyData;
		(*(DWORD*)0x08B64358) = (DWORD)&OnLogWrite;
		(*(DWORD*)0x08B6435C) = (DWORD)&ReadUnitNpcData;
		(*(DWORD*)0x08B64360) = (DWORD)&ReadUnitSpawnData;
		(*(DWORD*)0x08B64364) = (DWORD)&ReadUnitEnemyStage;
		(*(DWORD*)0x08B64368) = (DWORD)&ReadUnitNpcStand;
		(*(DWORD*)0x08B6436C) = (DWORD)&SendBossMessage;
		(*(DWORD*)0x08B64370) = (DWORD)&SetSpawnUnitData;
		(*(DWORD*)0x08B64374) = (DWORD)&InitSocketServer;
		(*(DWORD*)0x08B64378) = (DWORD)&OnItemPremium;
		(*(DWORD*)0x08B6437C) = (DWORD)&OnUnitNpcClick;
		(*(DWORD*)0x08B64380) = (DWORD)&RecvBuffSkill;
		(*(DWORD*)0x08B64384) = (DWORD)&OnReadPostBox;
		(*(DWORD*)0x08B64388) = (DWORD)&OnDeletePostBox;
		(*(DWORD*)0x08B6438C) = (DWORD)&SendAttackRatting;
		(*(DWORD*)0x08B64390) = (DWORD)&InitServer;
		(*(DWORD*)0x08B64394) = (DWORD)&UseDistortionMonster;
		(*(DWORD*)0x08B64398) = (DWORD)&OnLogItem;
		(*(DWORD*)0x08B6439C) = (DWORD)&AgingChanceHandler;
		(*(DWORD*)0x08B643A0) = (DWORD)&SetupMonsterBellatra;
		(*(DWORD*)0x08B643A4) = (DWORD)&SendDebugChat;
		(*(DWORD*)0x08B643A8) = (DWORD)&SendMixDescription;
		(*(DWORD*)0x08B643B4) = (DWORD)&OnOutCharacter;
		(*(DWORD*)0x08B643B8) = (DWORD)&DeallocMemoryPT;
		(*(DWORD*)0x08B643BC) = (DWORD)&OnSendExp;
		(*(DWORD*)0x08B643C0) = (DWORD)&RandomI;
		(*(DWORD*)0x08B643C4) = (DWORD)&ReadUnitAnimation;
		(*(DWORD*)0x08B643C8) = (DWORD)&ReadModelSize;
		(*(DWORD*)0x08B643CC) = (DWORD)&OnSetDrop;
		(*(DWORD*)0x08B643D0) = (DWORD)&CheatLog;
		(*(DWORD*)0x08B643D4) = (DWORD)&OnHandlerFunctions;
		(*(DWORD*)0x08B643D8) = (DWORD)&SendChatServer;
		(*(DWORD*)0x08B643DC) = (DWORD)&OnLoadCharacterInventory;
		(*(DWORD*)0x08B643E0) = (DWORD)&OnDisconnectUser;
		(*(DWORD*)0x08B643E4) = (DWORD)&InventoryServerCheck;
		(*(DWORD*)0x08B643E8) = (DWORD)&SendChatTrade;
		(*(DWORD*)0x08B643EC) = (DWORD)&ValidateCharacterStatusPoint;
		(*(DWORD*)0x08B643F0) = (DWORD)&IsItemSafeGate;
		(*(DWORD*)0x08B643F4) = (DWORD)&AddPotionGate;
		(*(DWORD*)0x08B643F8) = (DWORD)&OnSetTargetPosition;
		(*(DWORD*)0x08B643FC) = (DWORD)NULL;
		(*(DWORD*)0x08B64400) = (DWORD)&OnSaveItemData;
		(*(DWORD*)0x08B64404) = (DWORD)&OnLoadItemData;
		(*(DWORD*)0x08B64408) = (DWORD)&RecordPoint;
		(*(DWORD*)0x08B6440C) = (DWORD)&AddUserOnline;
		(*(DWORD*)0x08B64410) = (DWORD)&USERSERVER->UsersUpdate;
		(*(DWORD*)0x08B64414) = (DWORD)&SendItemNearUsers;
		(*(DWORD*)0x08B64418) = (DWORD)&SocketAccept;
		(*(DWORD*)0x08B6441C) = (DWORD)&ReadSocketHandler;
		(*(DWORD*)0x08B64420) = (DWORD)&SocketPacket;
		(*(DWORD*)0x08B64424) = (DWORD)&AddMonsterSpawn;
		(*(DWORD*)0x08B64428) = (DWORD)&DelMonsterSpawn;
		(*(DWORD*)0x08B6442C) = (DWORD)&OnEvade;
		(*(DWORD*)0x08B64430) = (DWORD)&MapServer::FindItemStage;
		(*(DWORD*)0x08B64434) = (DWORD)&UnitAutoMain;
		(*(DWORD*)0x08B64438) = (DWORD)&OnSetClassJobItem;
		(*(DWORD*)0x08B6443C) = (DWORD)&UpdateBellatra;
		(*(DWORD*)0x08B64440) = (DWORD)&OnDownAgeHandler;
		(*(DWORD*)0x08B64444) = (DWORD)&OnUpAgeHandler;
		(*(DWORD*)0x08B64448) = (DWORD)&UpdateDamageDataPVP;
		(*(DWORD*)0x08B6444C) = (DWORD)&SendPvPDamage;
		(*(DWORD*)0x08B64450) = (DWORD)&IsFreezedUnitFollow;
		(*(DWORD*)0x08B64454) = (DWORD)&CanHitTargetAreaDamage;
		(*(DWORD*)0x08B64458) = (DWORD)&MakeFaceArea;
		(*(DWORD*)0x08B6445C) = (DWORD)&SetupPolygonArea;
		(*(DWORD*)0x08B64460) = (DWORD)&MoveUnitDataGate;
		(*(DWORD*)0x08B64464) = (DWORD)&FindInventoryItemServer;
		(*(DWORD*)0x08B64468) = (DWORD)&DelInventoryItemServer;
		(*(DWORD*)0x08B6446C) = (DWORD)&AddInventoryItemServer;
		(*(DWORD*)0x08B64470) = (DWORD)&OnCharacterLoginLogoutGate;
		(*(DWORD *)0x08B64474) = (DWORD)&OnUserOnlineGate;
		(*(DWORD *)0x08B64478) = (DWORD)&AddItemServerStatusGate;
		(*(DWORD *)0x08B6447C) = (DWORD)&SetMixStatusGate;
		(*(DWORD *)0x08B64480) = (DWORD)&KillUnitNearBoss;
		(*(DWORD *)0x08B64484) = (DWORD)&HandleUnitDamageTop;
		break;
	}

	return TRUE;
}