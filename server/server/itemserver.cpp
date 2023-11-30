#include "stdafx.h"
#include "itemserver.h"

DWORD pTableNewItem = 0;
DWORD pTableNewItemSoma = 0x10;

extern ItemData								* pAllocItemTable;
extern UINT									iTableItemCompressPosition;

CSocketSystemHandler * ItemServer::pcSocketSystemHandler;

LoadItemDataHandler * ItemServer::psRecoveryQueue;
HANDLE ItemServer::hThreadRecovery;
HANDLE ItemServer::hEventRecoveryQueue;


ItemServer::ItemServer()
{
	ItemServer::pcSocketSystemHandler = new CSocketSystemHandler();

	DWORD dwThreadID = 0;
	ItemServer::hEventRecoveryQueue = CreateEvent( NULL, FALSE, FALSE, NULL );
	ItemServer::hThreadRecovery = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&ItemServer::UpdateQueueRecovery, NULL, 0, &dwThreadID );
}


ItemServer::~ItemServer()
{
	ItemServer::DeleteQueue();
}

float RandomFloat( float a, float b )
{
	float random = ( ( float )rand( ) ) / ( float )RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

ItemData * ItemServer::FindItemPointerTable( const char * szCode )
{
	if ( !pAllocItemTable )
		return NULL;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( STRINGCOMPAREI( pAllocItemTable[ u ].szInventoryName, szCode ) )
			return &pAllocItemTable[ u ];
	}

	return NULL;
}

ItemData * ItemServer::FindItemPointerTable( UINT uCode )
{
	if ( !pAllocItemTable )
		return NULL;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( pAllocItemTable[ u ].sBaseItemID.ToItemID() == uCode )
			return &pAllocItemTable[ u ];
	}

	return NULL;
}

BOOL ItemServer::FindItemName( UINT uCode, char * szBufName )
{
	STRINGCOPYLEN( szBufName, 32, "NULL" );
	
	char szCharSpecial[2] = { 0x27, 0 };
	
	if ( !pAllocItemTable )
		return FALSE;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( pAllocItemTable[u].sBaseItemID.ToItemID() == uCode )
		{
			STRINGCOPYLEN( szBufName, 32, pAllocItemTable[u].szBaseName );
			
			char * pszCharSpecial = strstr( szBufName, szCharSpecial );
			
			if ( pszCharSpecial )
				*(char*)&pszCharSpecial[0] = ' ';

			return TRUE;
		}
	}

	return FALSE;
}
BOOL ItemServer::FindItemName( char * pszCode, char * szBufName )
{
	STRINGCOPYLEN( szBufName, 32, "NULL" );
	char szCharSpecial[2] = { 0x27, 0 };
	if ( !pAllocItemTable )
		return FALSE;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( STRINGCOMPAREI(pAllocItemTable[u].szInventoryName, pszCode) )
		{
			STRINGCOPYLEN( szBufName, 32, pAllocItemTable[u].szBaseName );
			char * pszCharSpecial = strstr( szBufName, szCharSpecial );
			if ( pszCharSpecial )
				*(char*)&pszCharSpecial[0] = ' ';
			return TRUE;
		}
	}

	return FALSE;
}

int ItemServer::GetItemIDByItemCode( char * pszCode )
{
	if ( !pAllocItemTable )
		return 0;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( STRINGCOMPAREI(pAllocItemTable[u].szInventoryName, pszCode) )
		{
			return pAllocItemTable[u].sBaseItemID.ToItemID();
		}
	}
	return 0;
}

BOOL ItemServer::IsDropableItem( int iItemID )
{
	BOOL bRet = TRUE;

	switch ( iItemID )
	{
		case ITEMID_HPBooster:
		case ITEMID_MPBooster:
		case ITEMID_STMBooster:

		case ITEMID_MechanicCostumeM:
		case ITEMID_MechanicCostumeF:
		case ITEMID_BroyaleCostumeMale:
		case ITEMID_BroyaleCostumeFemale:

		case ITEMID_BeginnerAxe:
		case ITEMID_BeginnerClaw:
		case ITEMID_BeginnerHammer:
		case ITEMID_BeginnerJavelin:
		case ITEMID_BeginnerScythe:
		case ITEMID_BeginnerBoots:
		case ITEMID_BeginnerGauntlet:
		case ITEMID_BeginnerBracelet:
		case ITEMID_BeginnerBow:
		case ITEMID_BeginnerDagger:
		case ITEMID_BeginnerPhantom:
		case ITEMID_BeginnerStaff:
		case ITEMID_BeginnerSword:
		case ITEMID_GrumpyArmor:
		case ITEMID_GrumpyRobe:
		case ITEMID_ChestKey:
		case ITEMID_SilverLockedChest:
		case ITEMID_BlueLockedChest:
		case ITEMID_PurpleLockedChest:
		case ITEMID_GoldenLockedChest:
			bRet = FALSE;
			break;

		default:
			break;
	}

	return bRet;
}

int ItemServer::SpecStoneToCharacterSpec( int iItemID )
{
	int iSpec = 0;
	switch ( iItemID )
	{
		case ITEMID_DeluxeSpecStoneFS:
		case ITEMID_SpecStoneFS:
			iSpec = CHARACTERCLASS_Fighter;
			break;

		case ITEMID_DeluxeSpecStoneMS:
		case ITEMID_SpecStoneMS:
			iSpec = CHARACTERCLASS_Mechanician;
			break;

		case ITEMID_DeluxeSpecStoneAS:
		case ITEMID_SpecStoneAS:
			iSpec = CHARACTERCLASS_Archer;
			break;

		case ITEMID_DeluxeSpecStonePS:
		case ITEMID_SpecStonePS:
			iSpec = CHARACTERCLASS_Pikeman;
			break;

		case ITEMID_DeluxeSpecStoneATA:
		case ITEMID_SpecStoneATA:
			iSpec = CHARACTERCLASS_Atalanta;
			break;

		case ITEMID_DeluxeSpecStoneKS:
		case ITEMID_SpecStoneKS:
			iSpec = CHARACTERCLASS_Knight;
			break;

		case ITEMID_DeluxeSpecStoneMG:
		case ITEMID_SpecStoneMG:
			iSpec = CHARACTERCLASS_Magician;
			break;

		case ITEMID_DeluxeSpecStonePRS:
		case ITEMID_SpecStonePRS:
			iSpec = CHARACTERCLASS_Priestess;
			break;

		case ITEMID_DeluxeSpecStoneASS:
		case ITEMID_SpecStoneASS:
			iSpec = CHARACTERCLASS_Assassin;
			break;

		case ITEMID_DeluxeSpecStoneSHA:
		case ITEMID_SpecStoneSHA:
			iSpec = CHARACTERCLASS_Shaman;
			break;

		default:
			break;
	}
	return iSpec;
}

BOOL ItemServer::IsDeluxeStone( int iItemID )
{
	BOOL bRet = FALSE;

	switch ( iItemID )
	{
		case ITEMID_DeluxeSpecStoneFS:
		case ITEMID_DeluxeSpecStoneMS:
		case ITEMID_DeluxeSpecStoneAS:
		case ITEMID_DeluxeSpecStonePS:
		case ITEMID_DeluxeSpecStoneATA:
		case ITEMID_DeluxeSpecStoneKS:
		case ITEMID_DeluxeSpecStoneMG:
		case ITEMID_DeluxeSpecStonePRS:
		case ITEMID_DeluxeSpecStoneASS:
		case ITEMID_DeluxeSpecStoneSHA:
			bRet = TRUE;
			break;

		default:
			break;
	}
	return bRet;
}

CharacterClassData * ItemServer::GetItemSpecClassJob( int iClass, DefinitionItem * psDefItem )
{
	int iCounter = _countof( saCharacterClassData );

	int iJobID = iClass;


	if ( (iJobID >= 100 || iJobID == 0) && RandomI(1,100) < 50 )
	{
		if ( psDefItem->JobBitCodeRandomCount > 0 )
			iJobID = ClassFlagToCharacterClass( (EClassFlag)psDefItem->JobBitCodeRandom[rand() % psDefItem->JobBitCodeRandomCount] );
	}


	for ( int i = 0; i < iCounter; i++ )
	{
		CharacterClassData * ps = saCharacterClassData + i;
		
		if ( ps->iClass == iJobID )
			return ps;
	}

	return NULL;
}

DefinitionItem *	ItemServer::FindItemDefByCode( DWORD dwCode )
{
	DefinitionItem * pDefItem = *( DefinitionItem** )0x07A9EB10;

	int iCount = (*(int*)0x07AAC890);

	for ( int i = 0; i < iCount; i++ )
	{
		if ( pDefItem[i].sItem.sItemID.ToItemID() == dwCode )
			return &pDefItem[i];
	}
	return NULL;
}

DefinitionItem *	ItemServer::FindItemDefByCode( const char * pszCode )
{
	DefinitionItem * pDefItem = *( DefinitionItem** )0x07A9EB10;

	int iCount = (*(int*)0x07AAC890);

	ItemData * psItemData = FindItemPointerTable( pszCode );
	if ( psItemData )
	{
		for ( int i = 0; i < iCount; i++ )
		{
			if ( pDefItem[i].sItem.sItemID.ToItemID() == psItemData->sBaseItemID.ToItemID() )
				 return &pDefItem[i];
		}
	}
	return NULL;
}

DefinitionItem * ItemServer::FindItemDefByLevelBase( int iItemBase, int iLevel )
{
	DefinitionItem * pDefItem = *(DefinitionItem**)0x07A9EB10;

	int iCount = (*(int*)0x07AAC890);

	for ( int i = 0; i < iCount; i++ )
	{
		if ( pDefItem[i].sItem.sItemID.ToItemType() == iItemBase && pDefItem[i].sItem.iLevel == iLevel )
			return &pDefItem[i];
	}
	return NULL;
}

int ItemServer::FindItemPositionTable( char * pszName )
{
	if ( !pAllocItemTable )
		return -1;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( STRINGCOMPAREI( pAllocItemTable[u].szBaseName, pszName ) )
			return u;
	}
	return -1;
}

int ItemServer::UseItem( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUser )
		if ( pcUser->bInBattleRoyale )
			return 300;

	if ( READDWORD( 0x0084609C ) == FALSE )
		return 300;

	ItemID sItemID( dwCode );
	if ( (sItemID.ToItemBase() == ITEMBASE_Potion) || (sItemID.ToItemType() == ITEMTYPE_GoldAndExp) )
		return 300;

	int iRet = -1;

	for ( int i = 0; i < 300; i++ )
	{
		if ( pcUserData->sIntentoryItems[i].iItemID )
		{
			if ( (pcUserData->sIntentoryItems[i].iItemID == sItemID.ToInt()) &&
				(pcUserData->sIntentoryItems[i].iChk1 == dwHead) &&
				(pcUserData->sIntentoryItems[i].iChk2 == dwChkSum) )
			{
				pcUserData->sIntentoryItems[i].iItemID = 0;
				iRet = i;
				break;
			}
		}
	}

	return iRet;
}

int ItemServer::UseItem( User * pcUser, DWORD dwCode, DWORD dwHead, DWORD dwChkSum )
{
	return UseItem( pcUser->pcUserData, dwCode, dwHead, dwChkSum );
}

int ItemServer::AddItemInventory( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUser )
		if ( pcUser->bInBattleRoyale )
			return 300;

	if ( READDWORD( 0x0084609C ) == FALSE )
		return 300;

	ItemID sItemID( dwCode );
	if ( (sItemID.ToItemBase() == ITEMBASE_Potion) || (sItemID.ToItemType() == ITEMTYPE_GoldAndExp) )
		return 300;

	int iRet = -1;
	int iNewIndex = -1;

	for ( int i = 0; i < 300; i++ )
	{
		if ( pcUserData->sIntentoryItems[i].iItemID )
		{
			if ( (pcUserData->sIntentoryItems[i].iItemID == sItemID.ToInt()) &&
				(pcUserData->sIntentoryItems[i].iChk1 == dwHead) &&
				(pcUserData->sIntentoryItems[i].iChk2 == dwChkSum) )
			{
				iRet = i;
				break;
			}
		}
		else
			iNewIndex = i;
	}

	if ( iRet < 0 )
	{
		if ( iNewIndex >= 0 )
		{
			pcUserData->sIntentoryItems[iNewIndex].iItemID	= sItemID.ToInt();
			pcUserData->sIntentoryItems[iNewIndex].iChk1	= dwHead;
			pcUserData->sIntentoryItems[iNewIndex].iChk2	= dwChkSum;

			iRet = iNewIndex;
		}
	}
	else
	{
		PacketLogCheat sPacket;
		sPacket.iCheatID	= CHEATLOGID_AccountCharacterError;
		sPacket.LParam		= sItemID.ToInt();
		sPacket.SParam		= iNewIndex;
		sPacket.EParam		= pcUserData->bWarehouseOpen;
		LOGSERVER->OnLogCheat( pcUserData, &sPacket );

		iRet = -1;
	}

	return iRet;
}

int ItemServer::FindItemInventory( UserData * pcUserData, DWORD dwCode, DWORD dwHead, DWORD dwChkSum )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUser )
		if ( pcUser->bInBattleRoyale )
			return 300;

	if ( READDWORD( 0x0084609C ) == FALSE )
		return 300;

	ItemID sItemID( dwCode );
	if ( (sItemID.ToItemBase() == ITEMBASE_Potion) || (sItemID.ToItemType() == ITEMTYPE_GoldAndExp) )
		return 300;

	int iRet = -1;

	for ( int i = 0; i < 300; i++ )
	{
		if ( pcUserData->sIntentoryItems[i].iItemID )
		{
			if ( (pcUserData->sIntentoryItems[i].iItemID == sItemID.ToInt()) &&
				(pcUserData->sIntentoryItems[i].iChk1 == dwHead) &&
				(pcUserData->sIntentoryItems[i].iChk2 == dwChkSum) )
			{
				iRet = i;
				break;
			}
		}
	}

	return iRet;
}

BOOL ItemServer::GetItemInventoryCheksum( User * pcUser, EItemID iItemID, int & iChk1, int & iChk2 )
{
	BOOL bRet = FALSE;
	for ( int i = 0; i < 300; i++ )
	{
		if ( pcUser->pcUserData->sIntentoryItems[i].iItemID )
		{
			if ( pcUser->pcUserData->sIntentoryItems[i].iItemID == ItemID( iItemID ).ToInt() )
			{
				iChk1 = pcUser->pcUserData->sIntentoryItems[i].iChk1;
				iChk2 = pcUser->pcUserData->sIntentoryItems[i].iChk2;

				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

void ItemServer::SendDeleteItem( User * pcUser, EItemID iItemID, int iChk1, int iChk2 )
{
	PacketDeleteItem sPacket;
	sPacket.iLength = sizeof( PacketDeleteItem );
	sPacket.iHeader = PKTHDR_DeleteItemData;
	sPacket.iItemID = iItemID;
	sPacket.iChk1 = iChk1;
	sPacket.iChk2 = iChk2;
	SENDPACKET( pcUser, &sPacket );
}

BOOL ItemServer::IsItemSafe( UserData * pcUserData, int iItemID, int iChk1, int iChk2, int iGold, DWORD dwSec1, DWORD dwSec2, DWORD dwSec3, DWORD dwSec4, int * piaPotionCount )
{
	if ( User * pcUser = USERDATATOUSER( pcUserData ) )
	{
		if ( pcUser->bInBattleRoyale )
			return TRUE;
	}

	return IsItemSafeServer( pcUserData, iItemID, iChk1, iChk2, iGold, dwSec1, dwSec2, dwSec3, dwSec4, piaPotionCount );
}

NAKED BOOL ItemServer::IsItemSafeServer( UserData * pcUserData, int iItemID, int iChk1, int iChk2, int iGold, DWORD dwSec1, DWORD dwSec2, DWORD dwSec3, DWORD dwSec4, int * piaPotionCount )
{
	JMP( 0x0056A070 );
}

BOOL ItemServer::CheckValidItem( Item * psItem )
{
	tfnCheckItem fnCheckItem = (tfnCheckItem)pfnCheckItem;
	return fnCheckItem( psItem );
}

void ItemServer::CreateItem( Item * psItem, DefinitionItem * psDefItem, int iSpeck )
{
	tfnCreateDefItem fnCreateItem = (tfnCreateDefItem)pfnCreateDefItem;
	fnCreateItem( psItem, psDefItem, iSpeck );
}

void ItemServer::CreateItem( UserData * pcUserdata, EItemID eItem, int iCount )
{
	tfnCreateItemCount fnCreateItemCount = (tfnCreateItemCount)pfnCreateItemCount;
	fnCreateItemCount( pcUserdata, eItem, iCount );
}

void ItemServer::CreatePerfectItem( Item * psOut, DefinitionItem * psIn, int iSpeck )
{
	*( UINT* )0x8B70264 = 1;
	*( UINT* )0x8B70268 = iSpeck;

	CreateItem( psOut, psIn, iSpeck );
		
	*( UINT* )0x8B70264 = 0;
	*( UINT* )0x8B70268 = 0;
	psOut->bPerfectItem = TRUE;
}

int ItemServer::AddPotion( UserData * pcUserData, EItemID iItemID, int iCount )
{
	if ( READDWORD( 0x00845874 ) == FALSE )
		return 0;

	if ( User * pcUser = USERDATATOUSER( pcUserData ) )
		if ( pcUser->bInBattleRoyale )
			return 0;

	if ( (iItemID == ITEMID_PotionSuperbHP) ||
		(iItemID == ITEMID_PotionSuperbMP) ||
		(iItemID == ITEMID_PotionSuperbSP) )
		return 0;

	int iRet = -1;
	int iPotionSlot = 0;

	switch ( ItemID(iItemID).ToItemType() )
	{
		case ITEMTYPE_HealthPotion:
			iPotionSlot = 0;
			break;
		case ITEMTYPE_ManaPotion:
			iPotionSlot = 1;
			break;
		case ITEMTYPE_StaminaPotion:
			iPotionSlot = 2;
			break;
	}

	int iPotionIndex = ((iItemID & 0xFFFF) >> 8) - 1;
	if ( (iPotionIndex >= 0) && (iPotionIndex < 4) )
	{
		iRet = pcUserData->iaServerPotion[iPotionSlot][iPotionIndex] + iCount;

		if ( (iRet < 0) && (iCount < 0) )
		{
			pcUserData->iaServerPotion[iPotionSlot][iPotionIndex] = 0;
			return iRet;
		}

		pcUserData->iaServerPotion[iPotionSlot][iPotionIndex] = iRet;
	}

	return iRet;
}

BOOL ItemServer::GetDropPotion( UserData * pcUserData, int iItemID )
{
	for ( int i = 0; i < pcUserData->iTradePotion; i++ )
	{
		if ( pcUserData->iaItemTradePotionID[i] == iItemID )
			return TRUE;
	}

	return FALSE;
}

BOOL ItemServer::AddDropPotion( UserData * pcUserData, int iItemID )
{
	if ( pcUserData->iTradePotion >= 16 )
		return FALSE;

	if ( GetDropPotion( pcUserData, iItemID ) == TRUE )
		return TRUE;

	pcUserData->iaItemTradePotionID[pcUserData->iTradePotion++] = iItemID;

	return TRUE;
}

BOOL ItemServer::CreatePremium( UserData * pcUserData, EItemID iItemID, int iType, DWORD dwTime )
{
	BOOL bRet = FALSE;

	PacketNewItemTimer s;
	s.iLength		= sizeof( PacketNewItemTimer );
	s.iHeader		= PKTHDR_SetItemTimer;
	s.iItemID		= iItemID;
	s.iType			= (EItemTimerType)iType;
	s.dwTimeLeft	= dwTime;
	s.dwTimeTotal	= dwTime;

	DWORD i1, i2;

	EItemID e;

	if ( SQLGetItemTimer( USERDATATOUSER( pcUserData ), s.iType, e, i1, i2 ) == FALSE )
	{
		if ( SQLAddItemTimer( USERDATATOUSER( pcUserData ), s.iType, iItemID, dwTime ) )
		{
			SENDPACKET( USERDATATOUSER( pcUserData ), &s, TRUE );
			bRet = TRUE;
		}
	}

	return bRet;
}

void ItemServer::SetAgingItem( ItemData * psItemData )
{
	tfnIncreaseAging fnIncreaseAging = (tfnIncreaseAging)pfnIncreaseAging;
	fnIncreaseAging( &psItemData->sItem );
	psItemData->sItem.eCraftType = EItemCraftType::ITEMCRAFTTYPE_Aging;
	tfnSetAgingChecksum fnSetAgingChecksum = (tfnSetAgingChecksum)pfnSetAgingChecksum;
	fnSetAgingChecksum( &psItemData->sItem );
}

int ItemServer::DecodeItemsData( void * pItemData, void * pItemDest, int iSize )
{
	typedef int( __cdecl *tfnDecodeItems ) (void * pItemData, void * pItemDest, int iSize);
	tfnDecodeItems fnDecodeItems = (tfnDecodeItems)0x004453F0;
	return fnDecodeItems( pItemData, pItemDest, iSize );
}

int ItemServer::EncodeItemsData( void * pItemData, void * pItemDest, int iSize )
{
	typedef int( __cdecl *tfnEncodeItems ) (void * pItemData, void * pItemDest, int iSize);
	tfnEncodeItems fnEncodeItems = (tfnEncodeItems)0x00445480;
	return fnEncodeItems( pItemData, pItemDest, iSize );
}

int ItemServer::EncodeItemsData( void * pItemData, void * pItemDest, int iSize, int iSizeDest )
{
	typedef int( __cdecl *tfnEncodeItems ) (void * pItemData, void * pItemDest, int iSize, int iSizeDest);
	tfnEncodeItems fnEncodeItems = (tfnEncodeItems)0x004451E0;
	return fnEncodeItems( pItemData, pItemDest, iSize, iSizeDest );
}

void ItemServer::SaveItemData( Item * psItem )
{
	tfnSaveItemData fnSaveItemData = (tfnCheckItem)pfnSaveItemData;
	fnSaveItemData( psItem );
}

EItemTimerType ItemServer::GetPremiumType( DWORD dwCode )
{

	return (EItemTimerType)NULL;
}

PremiumItem * ItemServer::GetPremiumInfo( DWORD dwCode )
{

	return NULL;
}

void ItemServer::SendItemTimer( User * pcUser, EItemID iItemID, EItemTimerType iType, DWORD dwTimeLeft, DWORD dwTimeTotal )
{
	if ( pcUser && pcUser->pcUserData )
	{
		PacketNewItemTimer s;
		s.iLength		= sizeof( PacketNewItemTimer );
		s.iHeader		= PKTHDR_SetItemTimer;
		s.iItemID		= iItemID;
		s.iType			= iType;
		s.dwTimeLeft	= dwTimeLeft;
		s.dwTimeTotal	= dwTimeTotal;
		SENDPACKET( pcUser, &s );
	}
}

BOOL ItemServer::CheckQuestItemAging( const char * pszItemName, int iItemID )
{
	if ( iItemID != 0 )
	{
		if ( iItemID == ITEMID_PotionAgingQuestBee )
			return TRUE;
		if ( iItemID == ITEMID_DemonHunter )
			return TRUE;
		if ( iItemID == ITEMID_AncientRevenge )
			return TRUE;
		if ( iItemID == ITEMID_DarkSkull )
			return TRUE;
		if ( iItemID == ITEMID_DevilScythe )
			return TRUE;
		if ( iItemID == ITEMID_Mist )
			return TRUE;
		if ( iItemID == ITEMID_PlatinumSword )
			return TRUE;
		if ( iItemID == ITEMID_Anaconda )
			return TRUE;
		if ( iItemID == ITEMID_PlatinumDagger )
			return TRUE;
		if ( iItemID == ITEMID_HutanPhantom )
			return TRUE;
	}

	if ( (pszItemName != NULL) && (pszItemName[0] != 0) )
	{
		if ( STRINGCOMPAREI( pszItemName, "Demon Hunter" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Ancient Revenge" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Dark Skull" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Devil Scythe" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Mist" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Platinum Sword" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Anaconda" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Platinum Dagger" ) )
			return TRUE;
		if ( STRINGCOMPAREI( pszItemName, "Hutan Phantom" ) )
			return TRUE;
	}

	return FALSE;
}

void ItemServer::OnSetItemPerfect( Item * pItem )
{
	if ( !pItem )
		return;

	int iCharacterClass = ClassFlagToCharacterClass((EClassFlag)pItem->eSpecialization);

	// Set Status
	{
		// Get Minor
		int iLevel		= low( saItemRequeriments[iCharacterClass].sLevel[0], saItemRequeriments[iCharacterClass].sLevel[1] );
		int iStrength	= low( saItemRequeriments[iCharacterClass].sStrength[0], saItemRequeriments[iCharacterClass].sStrength[1] );
		int iSpirit		= low( saItemRequeriments[iCharacterClass].sSpirit[0], saItemRequeriments[iCharacterClass].sSpirit[1] );
		int iTalent		= low( saItemRequeriments[iCharacterClass].sTalent[0], saItemRequeriments[iCharacterClass].sTalent[1] );
		int iAgility	= low( saItemRequeriments[iCharacterClass].sAgility[0], saItemRequeriments[iCharacterClass].sAgility[1] );
		int iHealth		= low( saItemRequeriments[iCharacterClass].sHealth[0], saItemRequeriments[iCharacterClass].sHealth[1] );


		if ( pItem->iLevel && iLevel )
			pItem->iLevel += ( pItem->iLevel * iLevel ) / 100;
	
		if ( pItem->iStrength && iStrength )
			pItem->iStrength += ( pItem->iStrength * iStrength ) / 100;
		
		if ( pItem->iSpirit && iSpirit )
			pItem->iSpirit += ( pItem->iSpirit * iSpirit ) / 100;
		
		if ( pItem->iTalent && iTalent )
			pItem->iTalent += ( pItem->iTalent * iTalent ) / 100;
		
		if ( pItem->iAgility && iAgility )
			pItem->iAgility += ( pItem->iAgility * iAgility ) / 100;

		if ( pItem->iHealth && iHealth )
			pItem->iHealth += ( pItem->iHealth * iHealth ) / 100;
	}
}

void ItemServer::SendMixDescription( UserData * pcUserData, UINT puMix )
{
	if ( pcUserData )
	{
		// Save Mix
		SaveItemData( (Item*)0x7878950 );
		
		char * pszDescrition = (char*)((puMix * 0x1A4) + 0x006BB6B0 + 0x0BC);
		int iMixName = *(int*)((puMix * 0x1A4) + 0x006BB6B0 + 0x0BC + 64);
		((Item*)0x7878950)->eMixTypeName = (EMixTypeName)iMixName;

		PacketMixDescription sPacket;
		sPacket.iLength	= sizeof( PacketMixDescription );
		sPacket.iHeader = PKTHDR_MixDescription;
		STRINGCOPY( sPacket.szMixDescription, pszDescrition );

		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}

int ItemServer::GetAgingType( UserData * pcUserData )
{
	if ( pcUserData )
	{

	User * pcUser = USERDATATOUSER( pcUserData );

	UINT uStone = *( UINT* )0x786BAE4;

	*( int* )0x0789066C = 0;

	switch ( uStone )
	{
		case AgeStoneList::AgingStone:
		{
			*( int* )0x789066C = 1;
		} break;

		case AgeStoneList::CooperOre:
		{
			*( int* )0x789066C = 2;
		} break;

		case AgeStoneList::MaxAge:
		{
			*( int* )0x789066C = 3;
		} break;

		case ITEMID_RipeStone:
			*(int*)0x789066C = 4;
			break;

		case ITEMID_FluenceStone:
			*(int *)0x789066C = 5;
			break;

		case ITEMID_ArcaneStone:
			*(int *)0x789066C = 6;
			break;

		default:
			break;
	}

		Item * pItem = (Item*)0x0786B838;

		int iAgingLevel = (int)pItem->sAgeLevel + 1;
		if ( iAgingLevel > MAX_AGING )
			iAgingLevel = MAX_AGING;

		int iAgeTotal = MAX_AGING;

		(*(WORD*)0x8B80DA4) = 0;
		if ( *(int*)0x789066C == 3 )
		{
			(*(WORD*)0x8B80DA4) = 20;
			return 5;
		}

		std::random_device rd;
		std::uniform_int_distribution<int> dist( 0, 1000 );
		int iRandChance = dist( rd );

		int iFailChance = iaAgingChance[iAgingLevel - 1];
		if ( *(int*)0x789066C == 1 ) // if have AgeStone
			iFailChance -= 10; // change to new chance
		if ( *(int*)0x789066C == 4 ) // if have Ripe
			iFailChance -= 20; // change to new chance
		if ( *(int*)0x789066C == 5 ) // if have Fluence
			iFailChance -= 30; // change to new chance


		iFailChance *= 10;

		if ( iFailChance < 0 )
			iFailChance = 0;

		if ( pcUser->bUnbrokeAge )
		{
			ITEMSERVER->SaveItemData( pItem );
			return 3;
		}
		// If failed item
		if ( (iRandChance < iFailChance) && (iFailChance > 0) )
		{

			int iChanceMode = RandomI( 0, 100 );

			bool bPassed = false;

			if( iChanceMode < 25 ) // ( -1 age )
			{
				if( iAgingLevel <= 9 )
					ITEMSERVER->UpdateIntegrity( pItem, -3 );
				else
					ITEMSERVER->UpdateIntegrity( pItem, -5 );

				return 1;
			}
			else if ( iChanceMode < 50 ) // ( -2 age )
			{
				if( iAgingLevel <= 9 )
					ITEMSERVER->UpdateIntegrity( pItem, -3 );
				else
					ITEMSERVER->UpdateIntegrity( pItem, -5 );

				return 2;
			}
			else if ( iChanceMode < 75 ) // broken age
			{
				if ( *(int*)0x789066C != 2 ) // if dont have CopperOre
				{
					CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s broke '%s' going to +%d! Better luck next time!",  pcUserData->sCharacterData.szName, (char *)0x0786B854, iAgingLevel );
					AGEHANDLER->OnBrokeItemHandler( USERDATATOUSER( pcUserData ), pItem );

					return -1;
				}
			}
			// gelinho do cu canhoto
			return 0;
		}
		// 
		else
		{
			// Chance to increase +2
			iRandChance = RandomI( 0, 100 );
			if ( iRandChance <= 5 )
			{
				if ( (iAgingLevel + 1) <= iAgeTotal )
				{
					if ( ((iAgingLevel + 1) >= 18) || ((iAgingLevel) >= 18) )
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s aged %s to +%d successfully! Congratz!", pcUserData->sCharacterData.szName, (char *)0x0786B854, iAgingLevel );

					ITEMSERVER->SaveItemData( pItem );
					ITEMSERVER->UpdateIntegrity( pItem, -1 );

					return 4;
				}
				else
				{
					if ( (iAgingLevel) >= 18 )
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s aged %s to +%d successfully! Congratz!", pcUserData->sCharacterData.szName, (char *)0x0786B854, iAgingLevel );

					ITEMSERVER->SaveItemData( pItem );
					ITEMSERVER->UpdateIntegrity( pItem, -1 );

					return 3;
				}
			}

			//Just increase +1
			if ( (iAgingLevel) >= 18 )
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s aged %s to +%d successfully! Congratz!", pcUserData->sCharacterData.szName, (char *)0x0786B854, iAgingLevel );
		
			//Update Item Integrity
			ITEMSERVER->UpdateIntegrity( pItem, -1 );
			ITEMSERVER->SaveItemData( pItem );

			return 3;
		}

	}
	else
		// gelinho do cu canhoto
		return 0;
}

void ItemServer::CreateMixingList()
{
	UINT MixList = 0x006BB6B0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM MixList ORDER BY ID ASC" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{ 
				for ( int i = 0; i < 300; i++ )
				{
					int TypeMix = 0;
					pcDB->GetData( 2, PARAMTYPE_Integer, &TypeMix, 0 );


					// Sheltom List
					for ( int j = 0; j < 16; j++ )
					{
						UINT uSheltomCode = 0;
						pcDB->GetData( (j + 3), PARAMTYPE_Integer, &uSheltomCode, 0 );
						*(UINT*)(MixList + 0x20 + (j * 4)) = uSheltomCode;
					}

					// Atributtes
					int iAttrPosition = 0;
					for ( int j = 0; j < 8; j++ )
					{
						int TypeAtr = 0;
						float Atr = 0;
						int PerAtr = 0;
						pcDB->GetData( (j * 3) + 19, PARAMTYPE_Integer, &TypeAtr, 0 );
						pcDB->GetData( (j * 3) + 20, PARAMTYPE_Float, &Atr, 0 );
						pcDB->GetData( (j * 3) + 21, PARAMTYPE_Integer, &PerAtr, 0 );


						if ( (TypeAtr == 0) || ((float)Atr == 0.0f) )
							continue;

						*(UINT*)(MixList + 0x5C + (iAttrPosition * 4)) = TypeAtr;
						*(float*)(MixList + 0x7C + (iAttrPosition * 4)) = (float)Atr;
						*(UINT*)(MixList + 0x9C + (iAttrPosition * 4)) = PerAtr;
						iAttrPosition++;
					}

					// Description
					char szDescription[64] = { 0 };
					pcDB->GetData( 43, PARAMTYPE_String, szDescription, 64 );
					STRINGCOPYLEN( (char*)(MixList + 0xBC), 64, szDescription );

					// Mix Name
					int iMixName = 0;
					pcDB->GetData( 44, PARAMTYPE_Integer, &iMixName, 0 );
					*(int*)(MixList + 0xBC + 64) = iMixName;


					*(int*)(MixList + 0xBC + 68) = TypeMix;

					// Weapons
					if ( TypeMix == 1 )
					{
						*(UINT*)(MixList) = 0x01010000;
						*(UINT*)(MixList + 4) = 0x01020000;
						*(UINT*)(MixList + 8) = 0x01030000;
						*(UINT*)(MixList + 0x0C) = 0x01040000;
						*(UINT*)(MixList + 0x10) = 0x01050000;
						*(UINT*)(MixList + 0x14) = 0x01060000;
						*(UINT*)(MixList + 0x18) = 0x01070000;
						*(UINT*)(MixList + 0x1C) = 0x01080000;

					}
					// Assassin & Shaman mix weapon
					else if ( TypeMix == 10 )
					{
						*(UINT*)(MixList) = 0x01090000;
						*(UINT*)(MixList + 4) = 0x010A0000;

					}
					// Armors and Robes
					else if ( TypeMix == 2 )
					{
						*(UINT*)(MixList) = 0x02010000;
						*(UINT*)(MixList + 4) = 0x02050000;
					}
					// Shields
					else if ( TypeMix == 3 )
					{
						*(UINT*)(MixList) = 0x02040000;
					}
					// Orbs
					else if ( TypeMix == 4 )
					{
						*(UINT*)(MixList) = 0x03030000;
					}
					// Bracelets
					else if ( TypeMix == 5 )
					{
						*(UINT*)(MixList) = 0x03020000;
					}
					// Gauntlets
					else if ( TypeMix == 6 )
					{
						*(UINT*)(MixList) = 0x02030000;
					}
					// Boots
					else if ( TypeMix == 7 )
					{
						*(UINT*)(MixList) = 0x02020000;
					}
					// Amulets
					else if ( TypeMix == 8 )
					{
						*(UINT*)(MixList) = 0x03010000;
					}
					// Rings
					else if ( TypeMix == 9 )
					{
						*(UINT*)(MixList) = 0x03040000;
					}
					MixList += 0x1A4;

					if ( !pcDB->Fetch() )
						break;
				}
			}
		}
		pcDB->Close();
	}
}

BOOL ItemServer::OnUpgradeAgingItem( UserData * pcUserData, PacketItem * psPacket )
{
	Item sItem;

	Item * psItem = &psPacket->sItem;

	BOOL bMatureHack = FALSE;

	if ( pcUserData )
	{
		if ( !CheckValidItem( psItem ) )
			return FALSE;

		if ( UseItem( pcUserData, psItem->sItemID.ToItemID(), psItem->iChk1, psItem->iChk2 ) < 0 )
			return FALSE;

		CopyMemory( &sItem, psItem, sizeof( Item ) );

		// Check Mature Hack
		if ( !CheckQuestItemAging( psItem->szItemName, psItem->sItemID.ToInt() ) )
		{
			// Not correct? Hack
			if ( psItem->bMaturing != TRUE )
				bMatureHack = TRUE;

			// Not mature? Hack
			if ( psItem->sMatureBar.sCur == 0 || (psItem->sMatureBar.sCur != psItem->sMatureBar.sMax) )
				bMatureHack = TRUE;

			//Hack?
			if ( bMatureHack )
			{
				if ( (psItem->sItemID.ToItemType() == ITEMTYPE_Orb) && (abs( psItem->sMatureBar.sMax - psItem->sMatureBar.sCur ) <= 5) )
					bMatureHack = FALSE;
			}

			// More aging or mature hack flag? 
			if ( (pcUserData->iGameLevel == GAMELEVEL_None) && (psItem->sAgeLevel >= MAX_AGING || psItem->sAgeLevel < 0 || bMatureHack) )
			{
				// Set Log
				PacketLogCheat sPacket;
				sPacket.iCheatID	= CHEATLOGID_MatureHack;
				sPacket.SParam		= psItem->bMaturing;
				sPacket.EParam		= psItem->sMatureBar.sCur;
				sPacket.LxParam		= psItem->sMatureBar.sMax;
				sPacket.WxParam		= psItem->sAgeLevel;
				sPacket.LParam		= psItem->sItemID.ToItemID();
				sPacket.SxParam		= psItem->iChk1;
				sPacket.ExParam		= psItem->iChk2;
				SaveItemData( psItem );
				LOGSERVER->OnLogCheat( pcUserData, &sPacket );
				return FALSE;
			}
		}
		psItem->tTime = 0;
		tfnIncreaseAgingState fnIncreaseAgingState = (tfnIncreaseAgingState)pfnIncreaseAgingState;
		fnIncreaseAgingState( psItem );
		
		AddItemInventory( pcUserData, psItem->sItemID.ToItemID(), psItem->iChk1, psItem->iChk2 );

		tfnRecordAgingLog fnRecordAgingLog = (tfnRecordAgingLog)pfnRecordAgingLog;
		fnRecordAgingLog( pcUserData, &sItem, &psPacket->sItem );

		tfnRecordItemChangeLog fnRecordItemChangeLog = (tfnRecordItemChangeLog)pfnRecordItemChangeLog;
		fnRecordItemChangeLog( pcUserData, sItem.sItemID.ToItemID(), sItem.iChk1, sItem.iChk2, psItem->sItemID.ToItemID(), psItem->iChk1, psItem->iChk2, 0x0C );

		SaveItemData( psItem );

		SENDPACKET( USERDATATOUSER(pcUserData), psPacket );
		return TRUE;
	}
	return FALSE;
}

BOOL ItemServer::OnAgingItem( UserData * pcUserData, PacketAgingItem * psPacket )
{
	if ( psPacket->iLength != sizeof( PacketAgingItem ) )
		return FALSE;

	ItemData * psItem	= &psPacket->sItem;
	DWORD * pdwSheltom	= psPacket->SheltomCode;

	BOOL bAgeHack		= FALSE;
	int iSheltomCount	= 0;

	short sAgeLevel		= psItem->sItem.sAgeLevel;

	BOOL bSheltom[12]	= { 0,0,0,0,0,0,0,0,0,0,0,0 };

	// Check Sheltom Hack
	{
		// Hack Level?
		if ( sAgeLevel >= MAX_AGING || sAgeLevel < 0 )
			bAgeHack = TRUE;

		for ( int i = 0; i < 12; i++ )
		{
			for ( int j = 0; j < 12; j++ )
			{
				if ( !bSheltom[j] && pdwSheltom[j] == (iaSheltomAgingList[sAgeLevel][i] ? ((iaSheltomAgingList[sAgeLevel][i]*256) | 0x02350000) : 0) )
				{
					if ( pdwSheltom[j] )
						iSheltomCount++;

					bSheltom[j] = TRUE;
					break;
				}
			}
		}

		for ( int i = 0; i < 12; i++ )
		{
			if ( bSheltom[i] != TRUE )
			{
				bAgeHack = TRUE;
				break;
			}
		}

		if ( CheckQuestItemAging( psItem->sItem.szItemName, psItem->sItem.sItemID.ToInt() ) )
			bAgeHack = FALSE;

		if ( bAgeHack && pcUserData->iGameLevel == GAMELEVEL_None )
		{
			PacketLogCheat sPacket;
			sPacket.iCheatID	= CHEATLOGID_AgingHack;
			sPacket.LParam		= sAgeLevel;
			sPacket.SParam		= iSheltomCount;
			sPacket.EParam		= psItem->sItem.sItemID.ToItemID();
			sPacket.LxParam		= psItem->sItem.iChk1;
			sPacket.WxParam		= psItem->sItem.iChk2;
			SaveItemData( &psItem->sItem );
			LOGSERVER->OnLogCheat( pcUserData, &sPacket );
			return FALSE;
		}
	}

	if ( !EVENT_AGINGFREE )
	{
		if ( psItem->sItem.eCraftType != ITEMCRAFTTYPE_QuestWeapon )
			CHARACTERSERVER->GiveGOLD( USERDATATOUSER( pcUserData ), -(((sAgeLevel + 1) * psItem->sItem.iSalePrice) / 2), WHEREID_Aging );
	}

	typedef void( __cdecl *tfnRecvAgingItem )(UserData * pcUserData, PacketAgingItem * psPacket);
	tfnRecvAgingItem fnRecvAgingItem = (tfnRecvAgingItem)0x005690B0;
	fnRecvAgingItem( pcUserData, psPacket );

	return TRUE;
}

void ItemServer::ProcessIntegrity( User * pcUser, EIntegrityUpdate eUpdate )
{
	PacketUpdateIntegrity sPacket;
	sPacket.iHeader = PKTHDR_IntegrityUpdateItem;
	sPacket.iLength = sizeof(PacketUpdateIntegrity);
	sPacket.eUpdate = eUpdate;
	SENDPACKET( pcUser, &sPacket );
}

void ItemServer::UpdateIntegrity( Item * psItem, int iValueCur, int iValueMax )
{
	if( psItem )
	{
		if( psItem->sIntegrity.sMax <= 0 )
			return;

		if( iValueCur )
		{
			psItem->sIntegrity.sCur += iValueCur;
			if( psItem->sIntegrity.sCur <= 0 )
			{
				psItem->sIntegrity.sCur = 0;
				psItem->bCanNotUse = TRUE;
			}
			else if( psItem->sIntegrity.sCur >= psItem->sIntegrity.sMax )
				psItem->sIntegrity.sCur = psItem->sIntegrity.sMax;
		}

		if( iValueMax )
		{
			psItem->sIntegrity.sMax += iValueMax;
			if( psItem->sIntegrity.sMax <= 0 )
				psItem->sIntegrity.sMax = 0;
		}
	}
}

DWORD ItemServer::OnPhoenixPet( UserData * pcUserData, DWORD dwCode, BOOL bDelete, int iElementType )
{
	if ( pcUserData )
	{
		PacketActivePremium sPacket;
		sPacket.iLength			= sizeof( PacketActivePremium );
		sPacket.iHeader			= PKTHDR_SetItemTimer;
		sPacket.sPremium.dwCode = dwCode;
		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
	}
	return dwCode;
}

BOOL ItemServer::ValidateItem( ItemData * psItem )
{
	typedef void( __cdecl *tfnValidateItem )(Item * psItem);
	tfnValidateItem ValidateItem = (tfnValidateItem)0x0044A8A0;
	ValidateItem( &psItem->sItem );
	return TRUE;
}

BOOL ItemServer::ValidateItem( Item * psItem )
{
	typedef void( __cdecl *tfnValidateItem )(Item * psItem);
	tfnValidateItem ValidateItem = (tfnValidateItem)0x0044A8A0;
	ValidateItem( psItem );
	return TRUE;
}

void ItemServer::ValidateItemTime( Item * psItem )
{
	typedef void( __cdecl *tfnValidateItemTime )(Item * ps);
	tfnValidateItemTime fnValidateItemTime = (tfnValidateItemTime)0x00445AD0;
	fnValidateItemTime( psItem );
}

void ItemServer::RegisterItemSecCode( UserData * pcUserData, PacketItem * psPacket )
{
	typedef void( __cdecl *tfnRegisItemSecCode )(UserData * pcUserData, PacketItem * psPacket, int v);
	tfnRegisItemSecCode RegisItemSecCode = (tfnRegisItemSecCode)0x00551AB0;
	RegisItemSecCode( pcUserData, psPacket, TRUE );
}

void ItemServer::SendItemData( UserData * pcUserData, Item * psItem )
{
	//Is user alive and have item?
	if( pcUserData && psItem )
	{
		PacketItem sPacket;
		ZeroMemory( &sPacket, sizeof(PacketItem) );
		sPacket.iLength	= sizeof(PacketItem);
		sPacket.iHeader = PKTHDR_NewItem;
		CopyMemory( &sPacket.sItem, psItem, sizeof( Item ) ); 

		if( psItem->sItemID.ToItemID() == ITEMID_Gold )
		{
			pcUserData->iGoldOut += psItem->iGold;
			sPacket.iX = TICKCOUNT;
			sPacket.iY = sPacket.sItem.iChk2;
			sPacket.iZ = pcUserData->iGoldOut ^(sPacket.iX + sPacket.iY);
		}

		RegisterItemSecCode( pcUserData, &sPacket );
		SaveItemData( &sPacket.sItem );
		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
	}
}

BOOL ItemServer::OnSaveItemData( Item * psItem )
{
	EItemBase iItemBase = psItem->sItemID.ToItemBase();

	if ( ((psItem->iChk1 == 0) && (psItem->iChk2 == 0)) || (psItem->sItemID.ToItemID() == ITEMID_None) )
		return FALSE;

	if ( (iItemBase != ITEMBASE_Weapon) && (iItemBase != ITEMBASE_Defense) && (iItemBase != ITEMBASE_Accessory) )
		return FALSE;

	if ( psItem->iLevel <= 75 )
		return FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO ItemCreateLog([ItemID],[Code1],[Code2],[Item],[Date]) VALUES(?,?,?,?,GETDATE())" ) )
		{
			int iItemID = psItem->sItemID.ToInt();

			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iItemID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &psItem->iChk1 );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &psItem->iChk2 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Binary, psItem, sizeof( Item ) );

			pcDB->Execute();
		}
		pcDB->Close();

		return TRUE;
	}

	return FALSE;
}

BOOL ItemServer::OnLoadItemData( ItemLoadData * psItem, const char * pszCode )
{
	BOOL bRet = FALSE;
	std::vector<std::string> v = split( pszCode, '@' );

	if ( v.size() == 2 )
	{
		int iCode1 = atoi( v[0].c_str() );
		int iCode2 = atoi( v[1].c_str() );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if( pcDB->Open() )
		{
			if( pcDB->Prepare( "SELECT TOP 1 Item FROM ItemCreateLog WHERE (Code1=?) AND (Code2=?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCode1 );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iCode2 );

				if( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Binary, &psItem->sItem, sizeof( Item ) );
					bRet = TRUE;
				}
			}
			pcDB->Close();
		}
	}

	return bRet;
}


ItemPremium	* ItemServer::GetPremiumUse( UserData * pcUserData, EItemTimerType eType )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUserData )
	{
		for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
		{
			if ( pcUser->sPremiumData.saItemList[i].eType == eType )
			{
				if ( pcUser->sPremiumData.saItemList[i].dwTimeLeft )
					return &pcUser->sPremiumData.saItemList[i];
				else
					return NULL;
			}
		}
	}
	return NULL;
}

ItemPremium * ItemServer::GetPremiumUse( UserData * pcUserData, EItemID eID )
{
	User * pcUser = USERDATATOUSER( pcUserData );
	if ( pcUserData )
	{
		for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
		{
			if ( pcUser->sPremiumData.saItemList[ i ].dwCode == eID )
				return &pcUser->sPremiumData.saItemList[ i ];
		}
	}
	return NULL;
}

void ItemServer::OnLoadUser( User * pcUser )
{
	ZeroMemory( &pcUser->sPremiumData, sizeof( PacketItemPremiumData ) );
	pcUser->sPremiumData.iSize = sizeof( PacketItemPremiumData );
	pcUser->sPremiumData.iHeader = PKTHDR_PremiumData;

	EItemID iItemID = ITEMID_None;
	int iType = 0;
	DWORD dwTimeLeft = 0, dwTimeTotal = 0;
	int i = 0;

	pcUser->iEXPPotion = 0;

	iType = ITEMTIMERTYPE_ExpUp;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;

		pcUser->iEXPPotion += 30;
	}
	iType = ITEMTIMERTYPE_ThirdEye;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_MPDown;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_MPUp;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_HPUp;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_SPDown;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHead;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_CartolaHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_HalloweenHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_WitchHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_GoldenHopyHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_ObscureHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_PhoenixFire;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_PhoenixIce;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_PhoenixLightning;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_PhoenixHeal;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_WeightStone;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_GiraffeHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_SheepHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_SoccerHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_XMasGreenHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_XMasRedHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadLove;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadLove;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadHappiness;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadLove;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadSadness;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadShyness;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadAngry;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadSurprised;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_BigHeadSensual;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_BCBuff;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanHopy;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanRudolph;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanArma;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanRabie;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanTobie;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanCuepy;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanHungky;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanBuma;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanSirKnight;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanPingu;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanYeti;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_CaravanGoldenHopy;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_SkillCooldownReduceT1;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_SkillCooldownReduceT2;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_SkillCooldownReduceT3;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_SkillCooldownReduceT4;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_SkillCooldownReduceT5;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_HPBooster;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_MPBooster;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_STMBooster;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_TuttiFruttiHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;

		pcUser->iEXPPotion += 6;
	}

	iType = ITEMTIMERTYPE_EasterHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;

		pcUser->iEXPPotion += 5;
	}

	iType = ITEMTIMERTYPE_ChemistHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}
	iType = ITEMTIMERTYPE_PatronumHat;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	iType = ITEMTIMERTYPE_MountPinguMysterious;
	if ( SQLGetItemTimer( pcUser, iType, iItemID, dwTimeLeft, dwTimeTotal ) )
	{
		pcUser->sPremiumData.saItemList[i].eType = (EItemTimerType)iType;
		pcUser->sPremiumData.saItemList[i].dwCode = iItemID;
		pcUser->sPremiumData.saItemList[i].dwTimeLeft = dwTimeLeft;
		pcUser->sPremiumData.saItemList[i].dwTimeTotal = dwTimeTotal;
		i++;
	}

	if ( pcUser->iEXPPotion > 0 )
		NETSERVER->SendEXPPotion( pcUser->pcUserData, pcUser->iEXPPotion );

	SENDPACKET( pcUser, &pcUser->sPremiumData );

	//Top Pvp
	if ( PVPSERVER->IsTopPVP( pcUser ) )
	{
	//	ITEMSERVER->SQLDeleteItemTimer( pcUser, ITEMID_BCBuff );
	//	ITEMSERVER->CreatePremium( pcUser->pcUserData, ITEMID_BCBuff, ITEMTIMERTYPE_BCBuff, ITEMTIMERTIME_72H );

	//	PVPSERVER->SetTopPVP( pcUser, FALSE );
	}
}

void ItemServer::OnUnLoadUser( User * pcUser )
{
	UpdateItemTimer( pcUser );
}

void ItemServer::UpdateItemTimer( User * pcUser )
{
	if ( GAME_SERVER )
	{
		for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
		{
			ItemPremium * psItem = &pcUser->sPremiumData.saItemList[i];

			if ( psItem->eType != ITEMTIMERTYPE_None )
			{
				if ( psItem->dwTimeLeft > 2 )
					SQLUpdateItemTimer( pcUser, psItem->eType, psItem->dwTimeLeft );
				else
				{
					SQLDeleteItemTimer( pcUser, psItem->dwCode );

					ITEMSERVER->UpdateItemTimerEXP( pcUser, psItem->eType, true );

					LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_PremiumTimerOver, "[%s](%s) Item Premium Over [%d][0x%08X][%d]", pcUser->pcUserData->szAccountName,
						CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), psItem->eType, psItem->dwCode, psItem->dwTimeTotal );

					if ( (psItem->eType == ITEMTIMERTYPE_CaravanHopy)   || (psItem->eType == ITEMTIMERTYPE_CaravanRudolph) || (psItem->eType == ITEMTIMERTYPE_CaravanArma)  ||
						 (psItem->eType == ITEMTIMERTYPE_CaravanRabie)  || (psItem->eType == ITEMTIMERTYPE_CaravanTobie)  || (psItem->eType == ITEMTIMERTYPE_CaravanCuepy) ||
						 (psItem->eType == ITEMTIMERTYPE_CaravanHungky) || (psItem->eType == ITEMTIMERTYPE_CaravanBuma) || (psItem->eType == ITEMTIMERTYPE_CaravanSirKnight) ||
						 (psItem->eType == ITEMTIMERTYPE_CaravanPingu)  || (psItem->eType == ITEMTIMERTYPE_CaravanGoldenHopy) )
						FOLLOWUNITHANDLER->OnItemTimerEnd( pcUser );

					psItem->eType = ITEMTIMERTYPE_None;
				}
			}
		}
	}
}

void ItemServer::OnUseCrystal( UserData * pcUserData, int iID, DWORD dwItemID, DWORD dwTime )
{
	PacketCrystalUse sPacket;
	sPacket.iLength		= sizeof( PacketCrystalUse );
	sPacket.iHeader		= PKTHDR_CrystalUse;
	sPacket.dwObjectID	= iID;
	sPacket.dwItemID	= dwItemID;
	sPacket.dwTime = (dwTime - TICKCOUNT) / 1000;
	SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
}

void ItemServer::OnUseCrystalDie( UserData * pcUserData, int iID )
{
	PacketCrystalUseDie sPacket;
	sPacket.iLength		= sizeof( PacketCrystalUseDie );
	sPacket.iHeader		= PKTHDR_CrystalUseDie;
	sPacket.dwObjectID	= iID;
	SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
}

BOOL ItemServer::SQLGetItemTimer( User * pcUser, int iType, EItemID & iItemID, DWORD & dwTimeLeft, DWORD & dwTimeTotal )
{
	if ( pcUser == NULL || pcUser->pcUserData == NULL )
		return FALSE;

	BOOL bRet = FALSE;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "SELECT ItemID, TimeLeft, TimeTotal FROM CharacterItemTimer WHERE CharacterName=? AND ItemTimerType=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iType );

			if( (pcDB->Execute()) && (pcDB->Fetch()) )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iItemID, 0 );
				pcDB->GetData( 2, PARAMTYPE_Integer, &dwTimeLeft, 0 );
				pcDB->GetData( 3, PARAMTYPE_Integer, &dwTimeTotal, 0 );

				bRet = TRUE;
			}
		}
		pcDB->Close();
	}

	return bRet;
}

void ItemServer::SQLUpdateItemTimer( User * pcUser, int iType, DWORD dwTimeLeft )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "UPDATE CharacterItemTimer SET TimeLeft=? WHERE CharacterName=? AND ItemTimerType=? AND TimeLeft>?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &dwTimeLeft );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName(pcUser->pcUserData) );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iType );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &dwTimeLeft );
			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void ItemServer::SQLDeleteItemTimer( User * pcUser, int iItemID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "DELETE FROM CharacterItemTimer WHERE CharacterName=? AND ItemID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iItemID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

BOOL ItemServer::SQLAddItemTimer( User * pcUser, int iType, EItemID iItemID, DWORD dwTimeTotal )
{
	if ( pcUser == NULL || pcUser->pcUserData == NULL )
		return FALSE;

	if ( LOGIN_SERVER )
		return TRUE;

	//New Item Index
	int iIndex = -1;

	for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
	{
		//Find New Item Slot
		if ( pcUser->sPremiumData.saItemList[i].eType == ITEMTIMERTYPE_None )
		{
			if ( iIndex == (-1) )
				iIndex = i;
		}
		else
		{
			//Find if can Stack
			if ( (pcUser->sPremiumData.saItemList[i].eType == iType) || (ItemTimerIsSame( pcUser->sPremiumData.saItemList[i].eType, iType ) == TRUE) )
			{
				//Can stack?
				if ( ItemTimerCanStack( iType ) )
				{
					//Set Index
					iIndex = i;

					//Delete Old
					SQLDeleteItemTimer( pcUser, pcUser->sPremiumData.saItemList[i].dwCode );

					ITEMSERVER->UpdateItemTimerEXP( pcUser, iType, true );

					break;
				}
			}
		}
	}

	if ( iIndex != -1 )
	{
		pcUser->sPremiumData.saItemList[iIndex].dwCode		= iItemID;
		pcUser->sPremiumData.saItemList[iIndex].dwTimeLeft	= dwTimeTotal;
		pcUser->sPremiumData.saItemList[iIndex].dwTimeTotal	= dwTimeTotal;
		pcUser->sPremiumData.saItemList[iIndex].eType		= (EItemTimerType)iType;

		ITEMSERVER->UpdateItemTimerEXP( pcUser, iType, false );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "INSERT INTO CharacterItemTimer([CharacterName],[ItemTimerType],[ItemID],[TimeLeft],[TimeTotal]) VALUES(?,?,?,?,?)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iType );
				pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iItemID );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &dwTimeTotal );
				pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &dwTimeTotal );

				pcDB->Execute();
			}

			pcDB->Close();
		}

		return TRUE;
	}

	return FALSE;
}

void ItemServer::UpdateItemTimerEXP( User * pcUser, int iType, bool bRemove )
{
	if ( ((EItemTimerType)iType == ITEMTIMERTYPE_EasterHat) || ((EItemTimerType)iType == ITEMTIMERTYPE_ExpUp) || ((EItemTimerType)iType == ITEMTIMERTYPE_TuttiFruttiHat) )
	{
		if ( bRemove )
		{
			if ( (EItemTimerType)iType == ITEMTIMERTYPE_TuttiFruttiHat )
				pcUser->iEXPPotion -= 6;
			else if ( (EItemTimerType)iType == ITEMTIMERTYPE_EasterHat )
				pcUser->iEXPPotion -= 5;
			else
				pcUser->iEXPPotion -= 30;
		}
		else
		{
			if ( (EItemTimerType)iType == ITEMTIMERTYPE_TuttiFruttiHat )
				pcUser->iEXPPotion += 6;
			else if ( (EItemTimerType)iType == ITEMTIMERTYPE_EasterHat )
				pcUser->iEXPPotion += 5;
			else
				pcUser->iEXPPotion += 30;
		}

		NETSERVER->SendEXPPotion( pcUser->pcUserData, pcUser->iEXPPotion );
	}
}

BOOL ItemServer::OnPremiumItem( DWORD dwCode, int iX, int iY, int iZ, UserData * pcUserData )
{
	EItemTimerType eType = (EItemTimerType)0;

	if ( eType == ITEMTIMERTYPE_SkillStone )
		return FALSE;

	if ( eType == ITEMTIMERTYPE_MatureStone )
		return FALSE;

	User * pcUser = USERDATATOUSER( pcUserData );

	DefinitionItem * psItemDef = NULL;
	Item sItem;
	int iRandom = -1;

	DWORD dwTotalTime = 0;

	PacketPartyPotion sPacket;

	switch ( (EItemID)dwCode )
	{
		case ITEMID_XmasCane:
			XMASTREEHANDLER->SpawnTree( pcUser->iCharacterID, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), pcUser->GetMapID(), pcUserData->sPosition.iX, pcUserData->sPosition.iZ, TRUE );
			break;

		case ITEMID_Gingerbread:
			ZeroMemory( &sPacket, sizeof( PacketPartyPotion ) );
			sPacket.iLength = sizeof( PacketPartyPotion );
			sPacket.iHeader = PKTHDR_PartyUsePot;
			sPacket.iHP = RandomI( 700, 2000 );
			sPacket.iMP = RandomI( 700, 2000 );
			sPacket.iSTM = RandomI( 700, 2000 );
			SENDPACKET( USERDATATOUSER( pcUserData ), &sPacket, TRUE );

			//Is in Party?
			if ( ((pcUser) && pcUser->bParty && pcUser->psParty) && pcUser->psParty->iMembersCount > 1 )
			{
				for ( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
				{
					User * p = i == 0 ? pcUser->psParty->pcLeader : pcUser->psParty->pcaMembers[i - 1];
					if ( p && p->pcUserData->pcSocketData && p != pcUser )
					{
						int dX = 0, dZ = 0;
						dX = (p->pcUserData->sPosition.iX - pcUserData->sPosition.iX) >> 8;
						dZ = (p->pcUserData->sPosition.iZ - pcUserData->sPosition.iZ) >> 8;

						int iDist = dX * dX + dZ * dZ;
						if ( iDist < 0x144000 )
							SENDPACKET( p, &sPacket, TRUE );
					}
				}
			}
			break;

		case ITEMID_RottenLantern:
			iRandom = rand() % 100;
			if ( iRandom < 15 )
				psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_MirageForce );
			else if ( iRandom < 30 )
				psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_TurquoiseStone );
			else if ( iRandom < 38 )
				psItemDef = ITEMSERVER->FindItemDefByCode( RandomI( 0, 1 ) ? ITEMID_SuperJera : ITEMID_Jera );
			else if ( iRandom < 45 )
				ITEMSERVER->CreateItem( pcUserData, ITEMID_ManaMystic, 25 );
			else if ( iRandom < 55 )
				psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_InfernaForce );
			else if ( iRandom < 65 )
				psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_EmeraldStone );

			if ( !psItemDef )
				return FALSE;

			ITEMSERVER->CreateItem( &sItem, psItemDef );
			ITEMSERVER->SendItemData( pcUserData, &sItem );
			break;

		case ITEMID_EvilLantern:
			iRandom = rand() % 100;
			if ( iRandom < 35 )
				psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_CartolaHat );
			else if ( iRandom < 45 )
				psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_HalloweenHat );
			else
			{
				iRandom = rand() % 100;
				if ( iRandom < 45 )
					psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_WitchHat );
				else
				{
					iRandom = rand() % 100;
					if ( iRandom < 20 )
						psItemDef = ITEMSERVER->FindItemDefByCode( ITEMID_EnigmaForce );
					if ( iRandom < 40 )
						psItemDef = ITEMSERVER->FindItemDefByCode( RandomI( 0, 1 ) ? ITEMID_SuperJera : ITEMID_Jera );
				}
			}
			if ( !psItemDef )
				return FALSE;

			ITEMSERVER->CreateItem( &sItem, psItemDef );
			ITEMSERVER->SendItemData( pcUserData, &sItem );
			break;

		case ITEMID_InfernalLantern:
		{
			int iRandom = RandomI( 0, 999 );
			if ( iRandom < 800 )
			{
				EItemID eaItemID[] =
				{
					ITEMID_CartolaHat,
					ITEMID_WitchHat,
					ITEMID_HalloweenHat,
					ITEMID_BellumForce,
				};

				psItemDef = FindItemDefByCode( eaItemID[RandomI( 0, (_countof( eaItemID ) - 1) )] );
			}
			else if ( iRandom < 850 )
			{
				EItemID eaItemID[] =
				{
					ITEMID_VampireSuit,
					ITEMID_Succubus,
					ITEMID_HalloweenShield,
				};

				psItemDef = FindItemDefByCode( eaItemID[RandomI( 0, (_countof( eaItemID ) - 1) )] );
			}
			else if ( iRandom < 950 )
			{
				EItemID eaItemID[] =
				{
					ITEMID_HalloweenAxe,
					ITEMID_HalloweenClaw,
					ITEMID_HalloweenDagger,
					ITEMID_HalloweenHammer,
					ITEMID_HalloweenStaff,
					ITEMID_HalloweenWand,
					ITEMID_HalloweenPhantom,
					ITEMID_HalloweenScythe,
					ITEMID_HalloweenBow,
					ITEMID_HalloweenSword,
					ITEMID_HalloweenBladeSword,
					ITEMID_HalloweenJavelin,
					ITEMID_OredoForce,
					ITEMID_SapphireForce,
					ITEMID_TopazStone,
				};

				psItemDef = FindItemDefByCode( eaItemID[RandomI( 0, (_countof( eaItemID ) - 1) )] );
			}
			else
			{
				EItemID eaItemID[] =
				{
					ITEMID_PinguMountMysterious,
					ITEMID_Oredo,
					ITEMID_Sapphire,
				};

				psItemDef = FindItemDefByCode( eaItemID[RandomI( 0, (_countof( eaItemID ) - 1) )] );
			}

			if ( !psItemDef )
				return FALSE;

			ITEMSERVER->CreateItem( &sItem, psItemDef );
			ITEMSERVER->SendItemData( pcUserData, &sItem );
		}
		break;

		default:
			break;
	}

	if ( dwTotalTime > 0 )
		SQLAddItemTimer( USERDATATOUSER( pcUserData ), eType, (EItemID)dwCode, dwTotalTime );
	
	return FALSE;
}

BOOL ItemServer::OnMixingItem( User * pcUser, struct PacketMixServer * psPacket )
{
	BOOL bResult = FALSE;
	UserData * pcUserData = pcUser->pcUserData;
	Item sItem;

	if( psPacket->sItemData.sItem.sItemID.ToInt() > 0 )
		ITEMSERVER->UpdateIntegrity( &psPacket->sItemData.sItem, -5 );

	//Socket Mix?
	if ( psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_PurpleDiamond || psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_SilverDiamond || psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_GoldDiamond ||
		 psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_RedMarquiseGem ||
		 psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_GreenMarquiseGem ||
		 psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_BlueMarquiseGem ||
		 psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_YellowMarquiseGem ||
		 psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_PurpleMarquiseGem )
	{
		SOCKETSYSTEMHANDLER->OnMixingItem( pcUser, psPacket );
		return TRUE;
	}

	//Item Respec
	if ( psPacket->iDocIndex == 2000 )
	{
		int iFound = 0;
		EItemID eSpecStone = ITEMID_None;

		for( int i = 0; i < 12; i++ )
		{
			if( (psPacket->dwCode[i]) )
			{
				if( (psPacket->dwCode[i] & 0xFFFF0000) == 0x03070000 )
					iFound++;
			}
		}

		if( (psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_ValentoRing) || (psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_DraxosBoots) || 
			(psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_KelvezuAmulet) || (psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_DarkGuardianSheltom) ||
			(psPacket->sItemData.sItem.sItemID.ToItemID() == ITEMID_AbyssRing) )
		{
			if( iFound < 2 )
			{
				//Mixing Failed
				psPacket->iResult = 0;
				SENDPACKET( USERDATATOUSER(pcUserData), psPacket );

				CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "You must use 2 spec stones to respec!" );

				return FALSE;
			}
		}

		//Number of sheltoms is right?
		if( iFound > 1 )
		{

			if ( ITEMSERVER->CheckValidItem( &psPacket->sItemData.sItem ) == FALSE )
			{
				PacketLogCheat sPacket;
				sPacket.iCheatID = CHEATLOGID_RespecFakeItem;
				sPacket.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
				sPacket.SParam = psPacket->sItemData.sItem.iChk1;
				sPacket.EParam = psPacket->sItemData.sItem.iChk2;
				sPacket.LxParam = (int)psPacket->sItemData.sItem.szItemName;
				LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
				return FALSE;
			}

			//Delete Sheltoms
			for ( int i = 0; i < 12; i++ )
			{
				if ( (psPacket->dwCode[i]) )
				{
					//Delete Sheltoms
					if ( ITEMSERVER->UseItem( pcUserData, psPacket->dwCode[i], psPacket->dwHead[i], psPacket->dwCheckSum[i] ) < 0 )
					{
						PacketLogCheat sPacket;

						char szItemName[32] = { 0 };
						FindItemName( psPacket->dwCode[i], szItemName );

						sPacket.iCheatID = CHEATLOGID_RespecFakeItem;
						sPacket.LParam = psPacket->dwCode[i];
						sPacket.SParam = psPacket->dwHead[i];
						sPacket.EParam = psPacket->dwCheckSum[i];
						sPacket.LxParam = (int)szItemName;
						LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
						return FALSE;
					}
				}
			}

			//Get Item Timer Create
			DWORD dwItemTimerCreate = psPacket->sItemData.sItem.tTime;

			//Mixing Log
			LogItem sLog;
			sLog.ItemCount = 1;
			sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
			sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
			sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
			sLog.Flag = ITEMLOGID_LostRecon;
			sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
			STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
			STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

			USERSERVER->SubServerUserGold( pcUserData, psPacket->iGold, EWhereAction::WHEREID_MixItem );

			//Delete Item from Inventory
			if ( ITEMSERVER->UseItem( pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 ) < 0 )
			{
				PacketLogCheat sPacket;
				sPacket.iCheatID = CHEATLOGID_RespecFakeItem;
				sPacket.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
				sPacket.SParam = psPacket->sItemData.sItem.iChk1;
				sPacket.EParam = psPacket->sItemData.sItem.iChk2;
				sPacket.LxParam = (int)psPacket->sItemData.sItem.szItemName;
				LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
				return FALSE;
			}

			//Get Item Data and Spec
			int iSpec = 0;
			DefinitionItem * psItem = ITEMSERVER->FindItemDefByCode( psPacket->sItemData.sItem.sItemID.ToItemID() );

			if ( psItem )
			{
				ItemData * psItemData = ITEMSERVER->FindItemPointerTable( psPacket->sItemData.sItem.sItemID.ToItemID() );
				if ( psItemData )
				{

					//Respecing with Spec Stone?
					iSpec = 0;
					if ( RandomI(1,100) < 30 )
						iSpec = pcUser->pcUserData->sCharacterData.iClass + 100;

					ITEMSERVER->SaveItemData( &psPacket->sItemData.sItem );

					// Get Old Code
					DWORD dwHeadOld = 0, dwCheckSumOld = 0;
					dwHeadOld = psPacket->sItemData.sItem.iChk1;
					dwCheckSumOld = psPacket->sItemData.sItem.iChk2;

					//Successful?
					ITEMSERVER->CreateItem( &sItem, psItem, iSpec );

					SpecItemData sSpecData;
					CopyMemory( &sSpecData, &psPacket->sItemData.sItem.sSpecData, sizeof( SpecItemData ) );

					//Validate New Item
					ZeroMemory( &psPacket->sItemData, sizeof( ItemData ) );
					CopyMemory( &psPacket->sItemData, psItemData, sizeof( ItemData ) );
					CopyMemory( &psPacket->sItemData.sItem, &sItem, sizeof( Item ) );

					ITEMSERVER->ValidateItem(&psPacket->sItemData); 
					psPacket->sItemData.sItem.tTime = dwItemTimerCreate;
					ITEMSERVER->ValidateItemTime( &psPacket->sItemData.sItem );

					//Save New Item Data
					ITEMSERVER->SaveItemData( &psPacket->sItemData.sItem );
					{
						LogItem sLog;
						sLog.ItemCount = 1;
						sLog.Item[1].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
						sLog.Item[1].dwHead = psPacket->sItemData.sItem.iChk1;
						sLog.Item[1].dwCheckSum = psPacket->sItemData.sItem.iChk2;
						sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
						sLog.Item[0].dwHead = dwHeadOld;
						sLog.Item[0].dwCheckSum = dwCheckSumOld;
						sLog.Flag = ITEMLOGID_GetRecon;
						sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
						STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
						STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
						LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
					}

					//Create New Item
					if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 ) < 0 )
					{
						psPacket->iResult = 0;
						SENDPACKET( pcUser, psPacket );
						return FALSE;
					}

					psPacket->iResult = TRUE;
					psPacket->iDocIndex = 2000;
					SENDPACKET( USERDATATOUSER(pcUserData), psPacket );

					return TRUE;
				}
			}

			//Mixing Failed
			psPacket->iResult = 0;
			SENDPACKET( USERDATATOUSER(pcUserData), psPacket );
			return FALSE;
		}
		else
		{
			//Mixing Failed
			psPacket->iResult = 0;
			SENDPACKET( USERDATATOUSER( pcUserData ), psPacket );

			CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "You must use 2 spec stones to respec!" );

			return FALSE;
		}
	}
	
	__asm
	{
		PUSH psPacket;
		PUSH pcUserData;
		MOV EAX, 0x00569550;
		CALL EAX;
		ADD ESP, 0x08;
		MOV bResult, EAX;
	}

	return bResult;
}

BOOL ItemServer::OnResetItem( User * pcUser, struct PacketResetItem * psPacket )
{
	Item sItem;
	if ( psPacket->dwCode == ITEMID_CleanItem )
	{
		if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->dwCode, psPacket->dwHead, psPacket->dwCheckSum ) < 0 )
		{
			return FALSE;
		}
		if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->sItemData.sBaseItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 ) < 0 )
		{
			return FALSE;
		}
		DefinitionItem * psItem = ITEMSERVER->FindItemDefByCode( psPacket->sItemData.sBaseItemID.ToItemID() );
		if ( psItem )
		{
			ItemData * psItemData = ITEMSERVER->FindItemPointerTable( psPacket->sItemData.sBaseItemID.ToItemID() );
			if ( psItemData )
			{
				ITEMSERVER->SaveItemData( &psPacket->sItemData.sItem );
				{
					LogItem sLog;
					sLog.ItemCount = 1;
					sLog.Item[0].dwCode = psPacket->sItemData.sBaseItemID.ToItemID();
					sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
					sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
					sLog.Flag = ITEMLOGID_MixtureResetLost;
					sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
					STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
					STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
					LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
				}
				// Send Success
				if ( psPacket->sItemData.sItem.bPerfectItem )
					ITEMSERVER->CreatePerfectItem( &sItem, psItem, ClassFlagToCharacterClass( (EClassFlag)psPacket->sItemData.sItem.eSpecialization ) );
				else
					ITEMSERVER->CreateItem( &sItem, psItem, ClassFlagToCharacterClass( (EClassFlag)psPacket->sItemData.sItem.eSpecialization ) );

				CopyMemory( &sItem.sSpecData, &psPacket->sItemData.sItem.sSpecData, sizeof( SpecItemData ) );
				ZeroMemory( &psPacket->sItemData, sizeof( ItemData ) );
				CopyMemory( &psPacket->sItemData, psItemData, sizeof( ItemData ) );
				CopyMemory( &psPacket->sItemData.sItem, &sItem, sizeof( Item ) );
				ITEMSERVER->ValidateItem( &psPacket->sItemData );
				if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sBaseItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 ) < 0 )
				{
					psPacket->iResult = 0;
					SENDPACKET( pcUser, psPacket );
					return FALSE;
				}
				ITEMSERVER->SaveItemData( &psPacket->sItemData.sItem );
				{
					LogItem sLog;
					sLog.ItemCount = 1;
					sLog.Item[0].dwCode = psPacket->sItemData.sBaseItemID.ToItemID();
					sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
					sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
					sLog.Flag = ITEMLOGID_MixtureResetGet;
					sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
					STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
					STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
					LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
				}
				psPacket->iResult = TRUE;
				psPacket->dwCode = 0;
				SENDPACKET( pcUser, psPacket );
				return TRUE;
			}
		}
	}

	LogItem sLog;
	sLog.ItemCount = 1;
	sLog.Item[0].dwCode = psPacket->sItemData.sBaseItemID.ToItemID();
	sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
	sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
	sLog.Flag = ITEMLOGID_MixtureResetFail;
	sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
	STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
	STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
	psPacket->iResult = 0;
	SENDPACKET( pcUser, psPacket );
	return FALSE;
}

BOOL ItemServer::OnSocketItem( User * pcUser, PacketSocketSystem * psPacket )
{
	SOCKETSYSTEMHANDLER->HandlePacket( pcUser, psPacket );
	return TRUE;
}

void ItemServer::OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket )
{
	
	//Price
	int iPrice = psPacket->sItemData.sItem.iSalePrice;


	for ( int i = 0; i < 3; i++ )
	{
		if ( psPacket->iaRuneID[i] && ((psPacket->iaRuneID[i] & 0xFFFF0000) != ITEMID_RuneCodeManufacture1 &&
			(psPacket->iaRuneID[i] & 0xFFFFFF00) != ITEMID_SpecStoneCode && psPacket->iaRuneID[i] != ITEMID_Jera && psPacket->iaRuneID[i] != ITEMID_SuperJera) )
			return;
	}

	//Respec
	{
		int iLevelItem = psPacket->sItemData.sItem.iLevel;

		int iJera = 0;
		int iItemID = 0;

		for ( int i = 0; i < 3; i++ )
		{
			if ( iLevelItem >= 128 )
			{
				if ( psPacket->iaRuneID[i] == ITEMID_SuperJera )
					iJera++;
			}
			else if ( psPacket->iaRuneID[i] == ITEMID_Jera )
				iJera++;
			if ( (psPacket->iaRuneID[i] & 0xFFFFFF00) == ITEMID_SpecStoneCode )
				iItemID = psPacket->iaRuneID[i];

		}

		if ( iItemID != 0 && iJera == 2 )
		{
			int iSpec		= SpecStoneToCharacterSpec( iItemID );
			BOOL bDeluxe	= IsDeluxeStone( iItemID );

			BYTE bLockItemGM = psPacket->sItemData.sItem.bLockedItemType;

			if ( iSpec > 0 )
			{
				// Get Item Data and Spec
				DefinitionItem * psItem = ITEMSERVER->FindItemDefByCode( psPacket->sItemData.sItem.sItemID.ToItemID() );

				Item sItem;

				if ( psItem )
				{
					ItemData * psItemData = ITEMSERVER->FindItemPointerTable( psPacket->sItemData.sItem.sItemID.ToItemID() );
					if ( psItemData )
					{
						DWORD dwItemTimerCreate = psPacket->sItemData.sItem.tTime;

						// Get Old Code
						DWORD dwHeadOld = 0, dwCheckSumOld = 0;
						dwHeadOld = psPacket->sItemData.sItem.iChk1;
						dwCheckSumOld = psPacket->sItemData.sItem.iChk2;

						//Successful?
						if ( (psPacket->sItemData.sItem.bPerfectItem) )
							ITEMSERVER->CreatePerfectItem( &sItem, psItem, iSpec );
						else
							ITEMSERVER->CreateItem( &sItem, psItem, iSpec );

						SpecItemData sSpecData;
						if ( (psPacket->sItemData.sItem.eSpecialization == 0) )
							CopyMemory( &sSpecData, &sItem.sSpecData, sizeof( SpecItemData ) );
						else
							CopyMemory( &sSpecData, &psPacket->sItemData.sItem.sSpecData, sizeof( SpecItemData ) );

						int iStrength	= sItem.iStrength;
						int iSpirit		= sItem.iSpirit;
						int iTalent		= sItem.iTalent;
						int iAgility	= sItem.iAgility;
						int iHealth		= sItem.iHealth;

						if ( bDeluxe == FALSE )
						{
							//Upgrade Status
							ITEMSERVER->UpgradeNewItem( &psPacket->sItemData.sItem, &sItem );

							ZeroMemory( &psPacket->sItemData, sizeof( ItemData ) );
							CopyMemory( &psPacket->sItemData, psItemData, sizeof( ItemData ) );
							CopyMemory( &psPacket->sItemData.sItem, &sItem, sizeof( Item ) );
						}
						else
						{
							psPacket->sItemData.sItem.iStrength = iStrength;
							psPacket->sItemData.sItem.iSpirit	= iSpirit;
							psPacket->sItemData.sItem.iTalent	= iTalent;
							psPacket->sItemData.sItem.iAgility	= iAgility;
							psPacket->sItemData.sItem.iHealth	= iHealth;

							ZeroMemory( &sItem, sizeof( Item ) );

							ITEMSERVER->CreatePerfectItem( &sItem, psItem, iSpec );

							CopyMemory( &psPacket->sItemData.sItem.sSpecData, &sItem.sSpecData, sizeof( SpecItemData ) );

							psPacket->sItemData.sItem.eSpecialization = sItem.eSpecialization;
						}

						ITEMSERVER->ValidateItem( &psPacket->sItemData );
						psPacket->sItemData.sItem.tTime = dwItemTimerCreate;
						ITEMSERVER->ValidateItemTime( &psPacket->sItemData.sItem );

						//Save New Item Data
						ITEMSERVER->SaveItemData( &psPacket->sItemData.sItem );
						{
							LogItem sLog;
							sLog.ItemCount = 1;
							sLog.Item[1].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
							sLog.Item[1].dwHead = psPacket->sItemData.sItem.iChk1;
							sLog.Item[1].dwCheckSum = psPacket->sItemData.sItem.iChk2;
							sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToItemID();
							sLog.Item[0].dwHead = dwHeadOld;
							sLog.Item[0].dwCheckSum = dwCheckSumOld;
							sLog.Flag = ITEMLOGID_GetRecon;
							sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
							STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
							STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
							LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );
						}

						LogItem sLog;
						ZeroMemory( &sLog, sizeof( LogItem ) );
						sLog.ItemCount = 3;
						sLog.Flag = ITEMLOGID_ManufactureLost;
						sLog.IP = pcUser->pcUserData->pcSocketData->lIP;
						STRINGCOPY( sLog.UserID, pcUser->pcUserData->szAccountName );
						STRINGCOPY( sLog.CharName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

						// Checksum
						for ( int i = 0; i < 3; i++ )
						{
							if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->iaRuneID[i], psPacket->iaChk1[i], psPacket->iaChk2[i] ) < 0 )
							{
								// TODO
								sLog.ItemCount = 1;
								sLog.Flag = ITEMLOGID_ManufactureFail;
								sLog.Item[0].dwCode = psPacket->iaRuneID[i];
								sLog.Item[0].dwHead = psPacket->iaChk1[i];
								sLog.Item[0].dwCheckSum = psPacket->iaChk2[i];
								LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );

								PacketLogCheat sCheat;
								sCheat.iCheatID = CHEATLOGID_ManufactureItemFake;
								sCheat.LParam = psPacket->iaRuneID[i];
								sCheat.SParam = psPacket->iaChk1[i];
								sCheat.EParam = psPacket->iaChk2[i];
								LOGSERVER->OnLogCheat( pcUser->pcUserData, &sCheat );
								return;
							}

							{
								sLog.Item[i].dwCode = psPacket->iaRuneID[i];
								sLog.Item[i].dwHead = psPacket->iaChk1[i];
								sLog.Item[i].dwCheckSum = psPacket->iaChk2[i];
							}
						}

						if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToInt(), dwHeadOld, dwCheckSumOld ) < 0 )
						{
							// TODO
							sLog.ItemCount = 1;
							sLog.Flag = ITEMLOGID_ManufactureFail;
							sLog.Item[0].dwCode = psPacket->sItemData.sItem.sItemID.ToInt();
							sLog.Item[0].dwHead = psPacket->sItemData.sItem.iChk1;
							sLog.Item[0].dwCheckSum = psPacket->sItemData.sItem.iChk2;
							LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );

							PacketLogCheat sCheat;
							sCheat.iCheatID = CHEATLOGID_ManufactureItemFake;
							sCheat.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
							sCheat.SParam = psPacket->sItemData.sItem.iChk1;
							sCheat.EParam = psPacket->sItemData.sItem.iChk2;
							LOGSERVER->OnLogCheat( pcUser->pcUserData, &sCheat );
							return;
						}

						LOGSERVER->OnLogItem( LogServer::LOGTYPEID_LogItem, 1, &sLog );

						//Create New Item
						if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sItem.sItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 ) < 0 )
						{

						}

						psPacket->iResult = TRUE;
						if ( iPrice >= 0 )
							CHARACTERSERVER->GiveGOLD( pcUser, -iPrice );
					}
				}
			}

			psPacket->iLength = sizeof( PacketManufactureItem );
			psPacket->iHeader = PKTHDR_ManufactureItem;
			
			if ( psPacket->iResult == TRUE )
				psPacket->sItemData.sItem.bLockedItemType = bLockItemGM;

			SENDPACKET( pcUser, psPacket );
			return;
		}

	}

	if ( psPacket->iRecipeID && ((psPacket->iRecipeID & 0xFFFF0000) != ITEMID_RecipeCode) )
		return;

	HALLOWEENHANDLER->OnManufactureItem( pcUser, psPacket );
	XMASHANDLER->OnManufactureItem( pcUser, psPacket );
}

void ItemServer::OnManufactureWeightPrice( User * pcUser )
{


}

void ItemServer::PHGetPerfectItem( User * pcUser, struct PacketGetItemPerfectView * psPacket )
{
	PacketItemPerfectView sPacket;

	ItemData * psItemData			= FindItemPointerTable( psPacket->iItemID );
	DefinitionItem * psItemDefData	= FindItemDefByCode( psPacket->iItemID );

	if ( psItemData && psItemDefData )
	{
		CopyMemory( &sPacket.cItemData, psItemData, sizeof( ItemData ) );
		*( UINT* )0x8B70264 = 1;
		*( UINT* )0x8B70268 = pcUser->pcUserData->sCharacterData.iClass;
		ITEMSERVER->CreateItem( &sPacket.cItemData.sItem, psItemDefData, pcUser->pcUserData->sCharacterData.iClass );
		*( UINT* )0x8B70264 = 0;
		*( UINT* )0x8B70268 = 0;

		Item sItemUp;
		CopyMemory( &sItemUp, &sPacket.cItemData.sItem, sizeof( Item ) );

		if ( psPacket->iMixID != 0 )
		{
			sItemUp.eCraftType = ITEMCRAFTTYPE_Mixing;
			sItemUp.eMixTypeName = (EMixTypeName)psPacket->iMixID;
		}
		else if ( psPacket->iAgeLevel > 0 )
		{
			sItemUp.eCraftType = ITEMCRAFTTYPE_Aging;
			sItemUp.sAgeLevel = (short)psPacket->iAgeLevel;
		}
		

		ITEMSERVER->UpgradeNewItem( &sItemUp, &sPacket.cItemData.sItem );

		sPacket.iLength = sizeof( PacketItemPerfectView );
		sPacket.iHeader = PKTHDR_PerfectItemData;
		SENDPACKET( pcUser, &sPacket );
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketGetItem * psPacket )
{
	if( psPacket->iLength != sizeof(PacketGetItem) )
		return;

	//Valid Pointer?
	Map * pcMap = MAPSDATA + pcUser->GetMapID();
	if( !pcMap )
		return;

	//It's a valid map?
	if( !MAPSERVER->IsValidMap( pcMap ) )
		return;

	//Is Shared Account?
	if ( pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyPickupAndDropItems ) )
	{
		CHATSERVER->SendChatMessageBox( pcUser, "You can't Pickup Items, because you're logged in with the Share PW!" );
		return;
	}

	//Item Found?
	Map::Loot * pcLootItem = (Map::Loot*)MAPSERVER->FindItemStage( pcMap, psPacket->sPosition.iX >> 8, psPacket->sPosition.iY >> 8, psPacket->sPosition.iZ >> 8 );
	if( pcLootItem )
	{
		if ( SERVEREVENTHANDLER->OnUserGetMapItem( pcUser, pcLootItem->sLootItem.sItem.sItemID.ToItemID() ) )
			return;

		//Bellatra Item
		if( pcLootItem->sLootItem.sItem.sItemID.ToItemType() == ITEMTYPE_Bellatra )
		{
			BELLATRASERVERHANDLER->HandleItem( pcUser, pcLootItem->sLootItem.sItem.sItemID.ToItemID() );
			MAPSERVER->DelItem( pcMap, pcLootItem );
			return;
		}

		//Item Drop
		SendItemData( pcUser->pcUserData, &pcLootItem->sLootItem.sItem );

		if( pcLootItem->bExclusive && pcLootItem->sLootItem.sItem.sItemID.ToItemID() != ITEMID_None )
		{
			if( (pcUser->psParty && pcUser->bParty) && pcUser->psParty->pcLeader )
			{
				char szMessage[256] = { 0 };
				STRINGFORMAT( szMessage, "> %s gained %s!", pcUser->pcUserData->sCharacterData.szName, pcLootItem->sLootItem.sItem.sItemID.ToItemID() == ITEMID_Gold ? FormatString( "%s Gold", FormatNumber( pcLootItem->sLootItem.sItem.iGold ) ) : pcLootItem->sLootItem.sItem.szItemName );
				PARTYHANDLER->SendChatParty( pcUser, szMessage, CHATCOLOR_Notice, true );
			}
			else
				CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Notice, "> %s gained %s!", pcUser->pcUserData->sCharacterData.szName, pcLootItem->sLootItem.sItem.sItemID.ToItemID() == ITEMID_Gold ? FormatString("%s Gold", FormatNumber(pcLootItem->sLootItem.sItem.iGold)) : pcLootItem->sLootItem.sItem.szItemName );
		}

		MAPSERVER->DelItem( pcMap, pcLootItem );
	}
}

BOOL ItemServer::SQLLoadItemBox( User * pcUser )
{
	if( LOGIN_SERVER )
	{
		UserData * pcUserData = pcUser->pcUserData;

		if( (pcUserData != NULL) && (pcUserData->szAccountName[0] != 0) )
		{
			ZeroMemory( &pcUser->sItemBox, sizeof( ItemBoxSlot ) );

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

			if( pcDB->Open() != FALSE )
			{
				if( pcDB->Prepare( "SELECT ID, SenderName, ItemCode, ItemSpec, IsItem, Item, Date FROM ItemBox WHERE AccountName=? AND DateReceived IS NULL AND DateDiscarded IS NULL" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUserData->szAccountName, _countof( pcUserData->szAccountName ) );

					if( pcDB->Execute() )
					{
						int i = 0;
						while( (i < MAX_ITEMSINITEMBOX) && (pcDB->Fetch()) )
						{
							ItemBoxItem * psItemBoxItem = &pcUser->sItemBox.saItemBox[i];

							pcDB->GetData( 1, PARAMTYPE_Integer, &psItemBoxItem->iID );
							pcDB->GetData( 2, PARAMTYPE_String, psItemBoxItem->szSenderName, _countof( psItemBoxItem->szSenderName ) );

							char szItemCode[32] = { 0 };
							pcDB->GetData( 3, PARAMTYPE_String, szItemCode, _countof( szItemCode ) );
							psItemBoxItem->iItemID = ITEMSERVER->GetItemIDByItemCode( szItemCode );
							
							//Item error check
							if ( psItemBoxItem->iItemID == 0 )
								continue;

							pcDB->GetData( 4, PARAMTYPE_Integer, &psItemBoxItem->iSpecialization );
							pcDB->GetData( 5, PARAMTYPE_Integer, &psItemBoxItem->bItem );
							pcDB->GetData( 6, PARAMTYPE_Binary, &psItemBoxItem->sItem, sizeof( Item ) );
							pcDB->GetData( 7, PARAMTYPE_Time, &psItemBoxItem->sDate );

							psItemBoxItem->bValid = TRUE;

							i++;
						}

						pcUser->sItemBox.iNumItems = i;
					}
				}

				pcDB->Close();
			}

			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

void ItemServer::SQLItemLogBox( char * pszAccountName, ItemLogBox * psItemLogBox, BOOL bAccepted )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

	if( pcDB->Open() != FALSE )
	{
		if ( bAccepted )
		{
			if ( pcDB->Prepare( "UPDATE ItemBox SET DateReceived=GETDATE() WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psItemLogBox->iID );

				pcDB->Execute();
			}
		}
		else
		{
			if ( pcDB->Prepare( "UPDATE ItemBox SET DateReceived=GETDATE(), DateDiscarded=GETDATE() WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &psItemLogBox->iID );

				pcDB->Execute();
			}
		}

		pcDB->Close();
	}
}

void ItemServer::SQLItemLogBox( User * pcUser, Item * psItem, int iID, int iAmount, BOOL bAccepted )
{

	ItemLogBox l = { 0 };
	l.iSize = sizeof( l );

	StringCchCopyA( l.sPlayer.szAccountName, _countof( l.sPlayer.szAccountName ), pcUser->pcUserData->szAccountName );
	StringCchCopyA( l.sPlayer.szCharacterName, _countof( l.sPlayer.szCharacterName ), pcUser->pcUserData->szCharacterName );
	l.sPlayer.iIP.S_un.S_addr = StringIPToNumberIPV4( pcUser->pcUserData->pcSocketData->szIP );

	l.iActionCode = ITEMLOGID_BuyShop;

	l.iID = iID;
	l.sItem.iItemID = psItem->sItemID.ToItemID();
	l.sItem.iChecksum1 = psItem->iChk1;

	if( (l.sItem.iItemID == ITEMID_Gold) || (l.sItem.iItemID == ITEMID_Experience) )
		l.sItem.iAmount = iAmount;
	else
		l.sItem.iChecksum2 = psItem->iChk2;

	SQLItemLogBox( pcUser->pcUserData->szAccountName, &l, bAccepted );

	LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_ItemBoxReceived, "AccountName[%s] CharacterName[%s] Item Box Received ID[%d] Accepted[%s] Item([0x%08X][%s][%d x %d] Count[%d])",
		pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), iID, bAccepted ? "Yes" : "No", l.sItem.iItemID, psItem->szItemName, l.sItem.iChecksum1, l.sItem.iChecksum2, l.sItem.iAmount );
}

int ItemServer::SQLItemBoxCount( char * pszAccountName )
{
	if( (pszAccountName == NULL) || (pszAccountName[0] == 0) )
		return 0;

	int i = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() != FALSE )
	{
		if ( pcDB->Prepare( "SELECT COUNT(ID) FROM ItemBox WHERE AccountName=? AND DateReceived IS NULL AND DateDiscarded IS NULL" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, 32 );
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &i );
		}
		pcDB->Close();
	}
	return i;
}

int ItemServer::SQLItemBoxCount( char * pszAccountName, EItemID eItemID )
{
	if ( (pszAccountName == NULL) || (pszAccountName[0] == 0) )
		return 0;

	char szItemCode[32] = {0};
	if ( auto pcItemData = ITEMSERVER->FindItemPointerTable( eItemID ) )
		STRINGCOPY( szItemCode, pcItemData->szInventoryName );

	int i = 0;

	if ( szItemCode[0] != 0 )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() != FALSE )
		{
			if ( pcDB->Prepare( "SELECT COUNT(ID) FROM ItemBox WHERE AccountName=? AND ItemCode=? AND DateReceived IS NULL AND DateDiscarded IS NULL" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, szItemCode, STRLEN( szItemCode ) );
				if ( pcDB->Execute() && pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &i );
			}
			pcDB->Close();
		}
	}

	return i;
}

BOOL ItemServer::OpenItemBox( User * pcUser, PacketNPCItemBox * psPacket )
{
	if( LOGIN_SERVER )
	{
		if ( pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyOpenItemBox ) )
		{
			CHATSERVER->SendChatMessageBox( pcUser, "You can't open Distributor, because you're logged in with the Share PW!" );
			return FALSE;
		}

		if( (SQLLoadItemBox( pcUser ) != FALSE) && (pcUser->pcUserData->pcSocketData != NULL) )
		{
			PacketNPCItemBox p;
			ZeroMemory( &p, sizeof( PacketNPCItemBox ) );
			p.iLength = sizeof( p );
			p.iHeader = PKTHDR_NPCItemBox;

			//Retrieve Item
			if( psPacket->bItem )
			{
				for( int i = 0; i < pcUser->sItemBox.iNumItems; i++ )
				{
					ItemBoxItem * psItemBoxItem = &pcUser->sItemBox.saItemBox[i];

					if ( psItemBoxItem->bValid != FALSE )
					{
						EItemID iItemID = psPacket->cItemData.sItem.sItemID.ToItemID();
						int iChk1 = psPacket->cItemData.sItem.iChk1 ^ 0x7C6D;
						int iChk2 = psPacket->cItemData.sItem.iChk2 ^ 0x7C6E;

						if ( ((psItemBoxItem->bItem == FALSE) && (psItemBoxItem->iItemID == iItemID)) || ((psItemBoxItem->sItem.sItemID.ToItemID() == iItemID) && (psItemBoxItem->sItem.iChk1 == iChk1) && (psItemBoxItem->sItem.iChk2 == iChk2)) )
						{
							psItemBoxItem->bValid = FALSE;

							if ( psPacket->bAccepted )
							{
								if ( psItemBoxItem->iItemID == ITEMID_Gold )
								{
									int iGold = psItemBoxItem->iSpecialization;

									if ( (iGold > 0) && (iGold <= 1000000000) )
										CHARACTERSERVER->GiveGOLD( pcUser, iGold );

									p.bItem = TRUE;
								}
								else if ( psItemBoxItem->iItemID == ITEMID_Experience )
								{
									int iEXP = psItemBoxItem->iSpecialization;

									if ( iEXP > 0 )
										CHARACTERSERVER->GiveEXP( pcUser, iEXP );

									p.bItem = TRUE;
								}
								else
								{
									if ( psItemBoxItem->bItem == FALSE )
									{
										DefinitionItem * itm = ITEMSERVER->FindItemDefByCode( psItemBoxItem->iItemID );
										if ( itm != NULL )
										{
											p.bItem = TRUE;
											p.sWeight = (short)itm->sItem.iWeight;

											if ( (psItemBoxItem->iItemID & 0xFF000000) == 0x04000000 )
											{
												CreateItem( &p.cItemData.sItem, itm, 0 );
												p.cItemData.sItem.iPotionCount = psItemBoxItem->iSpecialization;
												p.sWeight = p.cItemData.sItem.iPotionCount;
											}
											else if ( psItemBoxItem->iSpecialization )
												CreatePerfectItem( &p.cItemData.sItem, itm, psItemBoxItem->iSpecialization );
											else
												CreateItem( &p.cItemData.sItem, itm, psItemBoxItem->iSpecialization );

											//Put Item In Server Inventory
											ITEMSERVER->AddItemInventory( pcUser->pcUserData, p.cItemData.sItem.sItemID.ToItemID(), p.cItemData.sItem.iChk1, p.cItemData.sItem.iChk2 );
										}
									}
									else
									{
										p.bItem = TRUE;
										p.sWeight = (short)psItemBoxItem->sItem.iWeight;

										ItemData * pcItemData = FindItemPointerTable( psItemBoxItem->sItem.sItemID.ToInt() );
										if ( pcItemData )
											CopyMemory( &p.cItemData, pcItemData, sizeof( ItemData ) );

										CopyMemory( &p.cItemData.sItem, &psItemBoxItem->sItem, sizeof( Item ) );

										//Put Item In Server Inventory
										ITEMSERVER->AddItemInventory( pcUser->pcUserData, p.cItemData.sItem.sItemID.ToItemID(), p.cItemData.sItem.iChk1, p.cItemData.sItem.iChk2 );
									}
								}

								SENDPACKET( pcUser, &p );
							}

							//Log and check received in DB
							SQLItemLogBox( pcUser, &p.cItemData.sItem, psItemBoxItem->iID, psItemBoxItem->iSpecialization, psPacket->bAccepted );

							p.cItemData.sItem.sItemID.SetItemID( ITEMID_None );
							p.bItem = FALSE;

							break;
						}
					}
				}
			}

			//Show next Item
			for( int i = 0; i < pcUser->sItemBox.iNumItems; i++ )
			{
				ItemBoxItem * psItemBoxItem = &pcUser->sItemBox.saItemBox[i];

				if( psItemBoxItem->bValid != FALSE )
				{
					if ( psItemBoxItem->bItem )
						CopyMemory( &p.cItemData.sItem, &psItemBoxItem->sItem, sizeof( Item ) );
					else
					{
						DefinitionItem * itm = ITEMSERVER->FindItemDefByCode( psItemBoxItem->iItemID );
						if ( itm != NULL )
						{
							p.sWeight = (short)itm->sItem.iWeight;
							if ( psItemBoxItem->iSpecialization )
								CreatePerfectItem( &p.cItemData.sItem, itm, psItemBoxItem->iSpecialization );
							else
								CreateItem( &p.cItemData.sItem, itm, psItemBoxItem->iSpecialization );

							p.cItemData.sItem.iChk1 = 15418561 + i;
							p.cItemData.sItem.iChk2 = 15818181 + i;

						}

						if ( psItemBoxItem->iItemID == ITEMID_Gold )
							p.Gold.iValue = psItemBoxItem->iSpecialization;
						else if ( psItemBoxItem->iItemID == ITEMID_Experience )
							p.EXP.liEXP.QuadPart = (INT64)psItemBoxItem->iSpecialization;
					}

					p.cItemData.sItem.iChk1 ^= 0x7C6D;
					p.cItemData.sItem.iChk2 ^= 0x7C6E;

					CopyMemory( &p.sDate, &psItemBoxItem->sDate, sizeof( SYSTEMTIME ) );
					STRINGCOPY( p.szSenderName, psItemBoxItem->szSenderName );

					break;
				}
			}

			SENDPACKET( pcUser, &p );

			return TRUE;
		}
	}

	return FALSE;
}

BOOL ItemServer::AddItemOpenBox( UserData * pcUserData, char * pszItemName, int iSpec, int iQuantity )
{
	return AddItemOpenBox( pcUserData->szAccountName, pszItemName, iSpec, iQuantity );
}

BOOL ItemServer::AddItemOpenBox( char * pszAccountName, char * pszItemName, int iSpec, int iQuantity )
{
	char szItemName[32] = { 0 };
	STRINGCOPY( szItemName, pszItemName );
	upper( szItemName );

	if ( STRLEN( pszItemName ) >= 2 )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() != FALSE )
		{
			BOOL bQuantitySpec = FALSE;

			if ( szItemName[0] == 'P' && (szItemName[1] == 'L' || szItemName[1] == 'M' || szItemName[1] == 'S') )
				bQuantitySpec = TRUE;

			if ( STRINGCOMPAREI( szItemName, "GG102" ) || STRINGCOMPAREI( szItemName, "GG101" ) )
				bQuantitySpec = TRUE;

			if ( bQuantitySpec )
			{
				if ( pcDB->Prepare( "INSERT INTO ItemBox([AccountName],[SenderName],[ItemCode],[ItemSpec],[IsItem],[Item],[DateReceived],[DateDiscarded],[Date]) VALUES(?,?,?,?,0,NULL,NULL,NULL,GETDATE())" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)"Server", STRLEN( "Server" ) );
					pcDB->BindParameterInput( 3, PARAMTYPE_String, szItemName, STRLEN( szItemName ) );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iQuantity );
					pcDB->Execute();
				}
				pcDB->Clear();
			}
			else
			{
				for ( int i = 0; i < iQuantity; i++ )
				{
					if ( pcDB->Prepare( "INSERT INTO ItemBox([AccountName],[SenderName],[ItemCode],[ItemSpec],[IsItem],[Item],[DateReceived],[DateDiscarded],[Date]) VALUES(?,?,?,?,0,NULL,NULL,NULL,GETDATE())" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );
						pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)"Server", STRLEN( "Server" ) );
						pcDB->BindParameterInput( 3, PARAMTYPE_String, szItemName, STRLEN( szItemName ) );
						pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iSpec );
						pcDB->Execute();
					}
					pcDB->Clear();
				}
			}
			pcDB->Close();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ItemServer::AddItemOpenBox( char * pszAccountName, Item * psItem, const char * pszSender )
{
	ItemData * pcItemData = FindItemPointerTable( psItem->sItemID.ToInt() );
	if ( pcItemData )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
		if ( pcDB->Open() != FALSE )
		{
			if ( pcDB->Prepare( "INSERT INTO ItemBox([AccountName],[SenderName],[ItemCode],[ItemSpec],[IsItem],[Item],[DateReceived],[DateDiscarded],[Date]) VALUES(?,?,?,?,1,?,NULL,NULL,GETDATE())" ) )
			{
				int iSpec = 0;
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_String, (char*)pszSender, STRLEN( pszSender ) );
				pcDB->BindParameterInput( 3, PARAMTYPE_String, pcItemData->szInventoryName, STRLEN( pcItemData->szInventoryName ) );
				pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &psItem->eSpecialization );
				pcDB->BindParameterInput( 5, PARAMTYPE_Binary, psItem, sizeof( Item ) );
				pcDB->Execute();
			}
			pcDB->Close();
			return TRUE;
		}
	}
	return FALSE;
}

void ItemServer::PHCheckItemUsing( UserData * pcUserData, PacketListUsingItem * psPacket )
{
	BOOL bCanUse = TRUE;
	DWORD dwCode = 0;
	int iPosition = 0;

	if ( psPacket->iItemListCount > _countof( psPacket->sItemList ) )
		psPacket->iItemListCount = _countof( psPacket->sItemList );

	for ( int i = 0; i < psPacket->iItemListCount; i++ )
	{
		dwCode		= psPacket->sItemList[i].iItemID & 0xFFFF0000;
		iPosition	= (int)(psPacket->sItemList[i].sPerformance[0] & 0xFF);
		bCanUse		= TRUE;

		if ( psPacket->sItemList[i].sPerformance[1] > 4 )
			psPacket->sItemList[i].sPerformance[1] = 4;

		// Weapon?
		if ( (dwCode & 0xFF000000) == 0x01000000 )
			dwCode = 0x01010000;
		// Potion?
		if ( (dwCode & 0xFF000000) == 0x01000000 )
			dwCode = 0x01010000;

		switch ( dwCode )
		{
			// Weapon
			case 0x01010000:
				if ( iPosition != ITEMSLOT_RightHand )
					bCanUse = FALSE;
				break;

			// Armor, Robe
			case 0x02010000:
			case 0x02050000:
				if ( iPosition == ITEMSLOT_Costume )
					break;

				if ( iPosition != ITEMSLOT_Armor )
					bCanUse = FALSE;
				break;

			// Boots
			case 0x02020000:
				if ( iPosition != ITEMSLOT_Boots )
					bCanUse = FALSE;
				break;

			// Gauntles
			case 0x02030000:
				if ( iPosition != ITEMSLOT_Gauntlets )
					bCanUse = FALSE;
				break;

			// Shield, Orb
			case 0x02040000:
			case 0x03030000:
				if ( iPosition != ITEMSLOT_LeftHand )
					bCanUse = FALSE;
				break;

			// Potion
			case 0x04010000:
				if ( iPosition < ITEMSLOT_PotionOne || iPosition > ITEMSLOT_PotionThree )
					bCanUse = FALSE;
				break;

			// Sheltom
			case 0x02350000:
				if ( iPosition != ITEMSLOT_Sheltom )
					bCanUse = FALSE;
				break;

			// Amulet
			case 0x03010000:
				if ( iPosition != ITEMSLOT_Amulet )
					bCanUse = FALSE;
				break;

			// Bracelets
			case 0x03020000:
				if ( iPosition != ITEMSLOT_Bracelets )
					bCanUse = FALSE;
				break;

			// Ring
			case 0x03040000:
			case 0x03050000:
				if ( iPosition != ITEMSLOT_RingRight && iPosition != ITEMSLOT_RingLeft )
					bCanUse = FALSE;
				break;

			// Costume
			case 0x02100000:
			case 0x02110000:
			case 0x02060000:
			case 0x02070000:
				if ( (iPosition != ITEMSLOT_Costume) && (iPosition != ITEMSLOT_Armor) )
					bCanUse = FALSE;
				break;

			default:
				break;
		}

		// Cannot use on that slot?
		if ( bCanUse == FALSE )
		{
			// Log it
			PacketLogCheat sPacket;
			sPacket.iCheatID	= CHEATLOGID_ItemPositionError;
			sPacket.LParam		= psPacket->sItemList[i].iItemID;
			sPacket.SParam		= 0;
			sPacket.EParam		= (int)psPacket->sItemList[i].sPerformance[0];
			LOGSERVER->OnLogCheat( pcUserData, &sPacket );
			return;
		}

		for ( int j = 0; j < psPacket->iItemListCount; j++ )
		{
			if ( i != j )
			{
				bool bSameSlot = false;
				// Same item in 2 or more slots in use?
				if ( psPacket->sItemList[i].sPerformance[0] == psPacket->sItemList[j].sPerformance[0] )
					bSameSlot = true;

				if ( bSameSlot )
				{
					if ( ((ItemID( psPacket->sItemList[j].iItemID ).ToItemType() == ITEMTYPE_Armor) ||
						  (ItemID( psPacket->sItemList[j].iItemID ).ToItemType() == ITEMTYPE_Robe)) &&
						 ((ItemID( psPacket->sItemList[i].iItemID ).ToItemType() == ITEMTYPE_Armor) ||
						  (ItemID( psPacket->sItemList[i].iItemID ).ToItemType() == ITEMTYPE_Robe)) )
						bSameSlot = false;
				}

				if ( bSameSlot )
				{
					// Log it
					PacketLogCheat sPacket;
					sPacket.iCheatID	= CHEATLOGID_ItemPositionError;
					sPacket.LParam		= psPacket->sItemList[i].iItemID;
					sPacket.SParam		= psPacket->sItemList[j].iItemID;
					sPacket.EParam		= (int)psPacket->sItemList[j].sPerformance[0];
					LOGSERVER->OnLogCheat( pcUserData, &sPacket );
					return;
				}
			}
		}
	}
}

void ItemServer::PHCheckItemUsingEx( UserData * pcUserData, PacketListUsingItem * psPacket )
{
	BOOL bCanUse = TRUE;
	DWORD dwCode = 0;
	int iWPCount = 0, iDSCount = 0;
	DWORD dwWPCode = 0, dwDSCode = 0;
	int iPosition = 0;

	if ( psPacket->iItemListCount > _countof( psPacket->sItemList ) )
		psPacket->iItemListCount = _countof( psPacket->sItemList );

	for ( int i = 0; i < psPacket->iItemListCount; i++ )
	{
		dwCode		= psPacket->sItemList[i].iItemID & 0xFFFF0000;
		iPosition	= (int)(psPacket->sItemList[i].sPerformance[0] & 0xFF);
		bCanUse		= TRUE;

		if ( psPacket->sItemList[i].sPerformance[1] > 4 )
			psPacket->sItemList[i].sPerformance[1] = 4;

		// Weapon?
		if ( (dwCode & 0xFF000000) == 0x01000000 )
			dwCode = 0x01010000;
		// Potion?
		if ( (dwCode & 0xFF000000) == 0x01000000 )
			dwCode = 0x01010000;

		switch ( dwCode )
		{
			// Weapon
			case 0x01010000:
				if ( iPosition != ITEMSLOT_RightHand )
					bCanUse = FALSE;

				iWPCount++;
				dwWPCode = psPacket->sItemList[i].iItemID;
				break;

			// Armor, Robe
			case 0x02010000:
			case 0x02050000:
				if ( iPosition == ITEMSLOT_Costume )
					break;

				if ( iPosition != ITEMSLOT_Armor )
					bCanUse = FALSE;
				break;

			// Boots
			case 0x02020000:
				if ( iPosition != ITEMSLOT_Boots )
					bCanUse = FALSE;
				break;

			// Gauntles
			case 0x02030000:
				if ( iPosition != ITEMSLOT_Gauntlets )
					bCanUse = FALSE;
				break;

			// Shield, Orb
			case 0x02040000:
			case 0x03030000:
				if ( iPosition != ITEMSLOT_LeftHand )
					bCanUse = FALSE;

				iDSCount++;
				dwDSCode = psPacket->sItemList[i].iItemID;
				break;

			// Potion
			case 0x04010000:
				if ( iPosition < ITEMSLOT_PotionOne || iPosition > ITEMSLOT_PotionThree )
					bCanUse = FALSE;
				break;

			// Sheltom
			case 0x02350000:
				if ( iPosition != ITEMSLOT_Sheltom )
					bCanUse = FALSE;
				break;

			// Amulet
			case 0x03010000:
				if ( iPosition != ITEMSLOT_Amulet )
					bCanUse = FALSE;
				break;

			// Bracelets
			case 0x03020000:
				if ( iPosition != ITEMSLOT_Bracelets )
					bCanUse = FALSE;
				break;

			// Ring
			case 0x03040000:
			case 0x03050000:
				if ( iPosition != ITEMSLOT_RingRight && iPosition != ITEMSLOT_RingLeft )
					bCanUse = FALSE;
				break;

			// Costume
			case 0x02100000:
			case 0x02110000:
			case 0x02060000:
			case 0x02070000:
				if ( (iPosition != ITEMSLOT_Costume) && (iPosition != ITEMSLOT_Armor) )
					bCanUse = FALSE;
				break;

			default:
				break;
		}

		// Cannot use on that slot?
		if ( bCanUse == FALSE )
		{
			// Log it
			PacketLogCheat sPacket;
			sPacket.iCheatID	= CHEATLOGID_ItemPositionError;
			sPacket.LParam		= psPacket->sItemList[i].iItemID;
			sPacket.SParam		= 0;
			sPacket.EParam		= (int)psPacket->sItemList[i].sPerformance[0];
			LOGSERVER->OnLogCheat( pcUserData, &sPacket );
			return;
		}

		for ( int j = 0; j < psPacket->iItemListCount; j++ )
		{
			if ( i != j )
			{
				bool bSameSlot = false;
				// Same item in 2 or more slots in use?
				if ( psPacket->sItemList[i].sPerformance[0] == psPacket->sItemList[j].sPerformance[0] )
					bSameSlot = true;

				if ( bSameSlot )
				{
					if ( ((ItemID( psPacket->sItemList[j].iItemID ).ToItemType() == ITEMTYPE_Armor) ||
   						  (ItemID( psPacket->sItemList[j].iItemID ).ToItemType() == ITEMTYPE_Robe)) &&
						 ((ItemID( psPacket->sItemList[i].iItemID ).ToItemType() == ITEMTYPE_Armor) ||
						  (ItemID( psPacket->sItemList[i].iItemID ).ToItemType() == ITEMTYPE_Robe)) )
						bSameSlot = false;
				}

				if ( bSameSlot )
				{
					// Log it
					PacketLogCheat sPacket;
					sPacket.iCheatID	= CHEATLOGID_ItemPositionError;
					sPacket.LParam		= psPacket->sItemList[i].iItemID;
					sPacket.SParam		= psPacket->sItemList[j].iItemID;
					sPacket.EParam		= (int)psPacket->sItemList[j].sPerformance[0];
					LOGSERVER->OnLogCheat( pcUserData, &sPacket );
					return;
				}
			}
		}
	}

	if ( iWPCount && iDSCount )
	{
		for ( UINT i = 0; i < (*(UINT*)0x07AAC890); i++ )
		{
			if ( pAllocItemTable[i].sBaseItemID.ToItemID() == dwWPCode )
			{
				if ( pAllocItemTable[i].IsTwoHandedWeapon() )
				{
					// Log it
					PacketLogCheat sPacket;
					sPacket.iCheatID	= CHEATLOGID_ItemPositionError;
					sPacket.LParam		= dwWPCode;
					sPacket.SParam		= dwDSCode;
					sPacket.EParam		= -1;
					LOGSERVER->OnLogCheat( pcUserData, &sPacket );
					return;
				}
			}
		}
	}
}

void ItemServer::UpgradeNewItem( Item * psItemOld, Item * psItemNew )
{
	if ( psItemOld->eCraftType == ITEMCRAFTTYPE_Mixing )
	{
		psItemNew->eCraftType = ITEMCRAFTTYPE_Mixing;
		MIXHANDLER->SetItemMixByMixName( psItemNew, psItemOld->eMixTypeName );
		psItemNew->eMixEffect = psItemOld->eMixEffect;
		psItemNew->sMixID = psItemOld->sMixID;
	}
	else if ( psItemOld->eCraftType == ITEMCRAFTTYPE_Aging )
	{
		if ( psItemNew->sItemID.ToItemType() == ITEMTYPE_Boots || psItemNew->sItemID.ToItemType() == ITEMTYPE_Gauntlets || psItemNew->sItemID.ToItemType() == ITEMTYPE_Bracelets )
			return;


		psItemNew->eCraftType = ITEMCRAFTTYPE_Aging;
		AGEHANDLER->OnUpAgeHandler( psItemNew, (int)psItemOld->sAgeLevel );
		psItemNew->bMaturing = psItemOld->bMaturing;
		psItemNew->sMatureBar = psItemOld->sMatureBar;
		ITEMSERVER->ValidateItem( psItemNew );
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketBuyNPCShop * psPacket )
{
	if ( pcUser && pcUser->pcUserData )
	{
		if ( GAME_SERVER )
		{
			if ( psPacket->iUniqueID != (pcUser->GetID() ^ CRC_ITEMSHOP_UNIQUEID) )
			{
				//Log It...
				PacketLogCheat sPacket;
				sPacket.iCheatID = CHEATLOGID_ItemFakeBoughtShopID;
				sPacket.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
				sPacket.SParam = psPacket->sItemData.sItem.iChk1;
				sPacket.EParam = psPacket->sItemData.sItem.iChk2;
				STRINGCOPY( sPacket.szBuffer1, psPacket->sItemData.sItem.szItemName );
				sPacket.WxParam = psPacket->iUniqueID;
				sPacket.SxParam = (pcUser->GetID() ^ CRC_ITEMSHOP_UNIQUEID);
				LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
				psPacket->iHeader = 0;
				return;
			}

			bool bValidItem = false;

			int iNPCID = psPacket->iNPCID ^ CRC_ITEMSHOP_UNIQUEID;
			UnitData * pcUnitData = UNITSERVER->GetUnit( iNPCID );
			if ( pcUnitData && pcUnitData->bActive )
			{
				if ( pcUnitData->sUnitInfo.iWeaponShopCounter )
				{
					for ( int i = 0; i < pcUnitData->sUnitInfo.iWeaponShopCounter; i++ )
					{
						DefinitionItem * psDefinitionItem = (DefinitionItem*)pcUnitData->sUnitInfo.dwaWeaponShop[i];
						if ( psDefinitionItem )
						{
							if ( psDefinitionItem->sItem.sItemID.ToItemID() == psPacket->sItemData.sItem.sItemID.ToItemID() )
							{
								bValidItem = true;
								break;
							}
						}
					}
				}

				if ( bValidItem == false )
				{
					if ( pcUnitData->sUnitInfo.iDefenseShopCounter )
					{
						for ( int i = 0; i < pcUnitData->sUnitInfo.iDefenseShopCounter; i++ )
						{
							DefinitionItem * psDefinitionItem = (DefinitionItem*)pcUnitData->sUnitInfo.dwaDefenseShop[i];
							if ( psDefinitionItem )
							{
								if ( psDefinitionItem->sItem.sItemID.ToItemID() == psPacket->sItemData.sItem.sItemID.ToItemID() )
								{
									bValidItem = true;
									break;
								}
							}
						}
					}
				}

				if ( bValidItem == false )
				{
					if ( pcUnitData->sUnitInfo.iMiscShopCounter )
					{
						for ( int i = 0; i < pcUnitData->sUnitInfo.iMiscShopCounter; i++ )
						{
							DefinitionItem * psDefinitionItem = (DefinitionItem*)pcUnitData->sUnitInfo.dwaMiscShop[i];
							if ( psDefinitionItem )
							{
								if ( psDefinitionItem->sItem.sItemID.ToItemID() == psPacket->sItemData.sItem.sItemID.ToItemID() )
								{
									bValidItem = true;
									break;
								}
							}
						}
					}
				}

				if ( bValidItem == false )
					if ( (psPacket->sItemData.sItem.sItemID.ToItemBase() == ITEMBASE_Potion) || (psPacket->sItemData.sItem.sItemID.ToItemBase() == ITEMBASE_Core) )
						bValidItem = true;
			}

			if ( bValidItem == false )
			{
				PacketLogCheat sPacket;
				sPacket.iCheatID = CHEATLOGID_ItemFakeBoughtShopItem;
				sPacket.LParam = psPacket->sItemData.sItem.sItemID.ToInt();
				sPacket.SParam = psPacket->sItemData.sItem.iChk1;
				sPacket.EParam = psPacket->sItemData.sItem.iChk2;
				STRINGCOPY( sPacket.szBuffer1, psPacket->sItemData.sItem.szItemName );
				LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
				psPacket->iHeader = 0;
				return;
			}

			if ( LOGIN_SERVER == FALSE )
				psPacket->iHeader = 0;
		}
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketNewItemTimer * psPacket )
{
	DWORD dwTime = 0;

	psPacket->dwTimeLeft = psPacket->dwTimeTotal = 0;

	//For Delete...
	int iOldItemID = psPacket->iItemID;

	if ( MOUNTSERVERHANDLER->HandleItem( pcUser, psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 ) )
		return;

	//Hour
	switch ( psPacket->iItemID )
	{
		case ITEMID_ThirdEye1H:
		case ITEMID_ExpUp1H:
			psPacket->dwTimeTotal = ITEMTIMERTIME_1H;
			break;

		case ITEMID_ThirdEye3H:
		case ITEMID_ExpUp3H:
			psPacket->dwTimeTotal = ITEMTIMERTIME_3H;
			break;

		case ITEMID_BigHeadLove:
		case ITEMID_BigHeadSadness:
		case ITEMID_BigHeadHappyness:
		case ITEMID_BigHeadShyness:
		case ITEMID_BigHeadAngry:
		case ITEMID_BigHeadSurprised:
		case ITEMID_BigHeadSensual:
		case ITEMID_HPBooster:
		case ITEMID_MPBooster:
		case ITEMID_STMBooster:
			psPacket->dwTimeTotal = ITEMTIMERTIME_3H;
			break;

		case ITEMID_SheepHat:
		case ITEMID_GiraffeHat:
		case ITEMID_SoccerHat:
		case ITEMID_CaravanYetiCrystal:
		case ITEMID_CaravanGoldenHopyCrystal:
			psPacket->dwTimeTotal = ITEMTIMERTIME_6H;
			break;

		case ITEMID_MPDown12H:
		case ITEMID_MPUp12H:
		case ITEMID_SPDown12H:
		case ITEMID_HPUp12H:
		case ITEMID_CartolaHat:
		case ITEMID_WitchHat:
		case ITEMID_HalloweenHat:
		case ITEMID_XMasGreenHat:
		case ITEMID_XMasRedHat:
		case ITEMID_EasterHat:
		case ITEMID_ChemistHat:
		case ITEMID_PatronumHat:
		case ITEMID_GoldenHopyHat:
		case ITEMID_ObscureHat:
		case ITEMID_PinguMountMysterious:
			psPacket->dwTimeTotal = ITEMTIMERTIME_12H;
			break;

		case ITEMID_MPDown24H:
		case ITEMID_MPUp24H:
		case ITEMID_SPDown24H:
		case ITEMID_HPUp24H:
		case ITEMID_ExpUp24H:
		case ITEMID_ThirdEye24H:
		case ITEMID_FeatherTier1:
		case ITEMID_FeatherTier2:
		case ITEMID_FeatherTier3:
		case ITEMID_FeatherTier4:
		case ITEMID_FeatherTier5:
		case ITEMID_TuttiFruttiHat:
		case ITEMID_CaravanHopyCheapCrystal:
			psPacket->dwTimeTotal = ITEMTIMERTIME_24H;
			break;

		case ITEMID_MPDown48H:
		case ITEMID_MPUp48H:
		case ITEMID_SPDown48H:
		case ITEMID_HPUp48H:
		case ITEMID_ExpUp48H:
		case ITEMID_ThirdEye48H:
			psPacket->dwTimeTotal = ITEMTIMERTIME_48H;
			break;

		case ITEMID_MPDown72H:
		case ITEMID_MPUp72H:
		case ITEMID_SPDown72H:
		case ITEMID_HPUp72H:
		case ITEMID_ExpUp72H:
		case ITEMID_ThirdEye72H:
		case ITEMID_GravityStone72H:
		case ITEMID_PhoenixEgg:
			psPacket->dwTimeTotal = ITEMTIMERTIME_72H;
			break;

		case ITEMID_CaravanHopy:
		case ITEMID_CaravanHopyCrystal:
		case ITEMID_CaravanArmaCrystal:
		case ITEMID_CaravanBumaCrystal:
		case ITEMID_CaravanCuepyCrystal:
		case ITEMID_CaravanHungkyCrystal:
		case ITEMID_CaravanRabieCrystal:
		case ITEMID_CaravanTobieCrystal:
		case ITEMID_CaravanRudolphCrystal:
		case ITEMID_CaravanSirKnightCrystal:
		case ITEMID_CaravanPinguCrystal:
			psPacket->dwTimeTotal = ITEMTIMERTIME_168H;
			break;

		default:
			break;
	}

	//Used for PHPet...
	int iRandom = RandomI( 0, 100 );

	//Type
	switch ( psPacket->iItemID )
	{
		case ITEMID_ExpUp1H:
		case ITEMID_ExpUp3H:
		case ITEMID_ExpUp24H:
		case ITEMID_ExpUp48H:
		case ITEMID_ExpUp72H:
			psPacket->iType = ITEMTIMERTYPE_ExpUp;
			break;

		case ITEMID_ThirdEye1H:
		case ITEMID_ThirdEye3H:
		case ITEMID_ThirdEye24H:
		case ITEMID_ThirdEye48H:
		case ITEMID_ThirdEye72H:
			psPacket->iType = ITEMTIMERTYPE_ThirdEye;
			break;

		case ITEMID_MPDown12H:
		case ITEMID_MPDown24H:
		case ITEMID_MPDown48H:
		case ITEMID_MPDown72H:
			psPacket->iType = ITEMTIMERTYPE_MPDown;
			break;

		case ITEMID_SPDown12H:
		case ITEMID_SPDown24H:
		case ITEMID_SPDown48H:
		case ITEMID_SPDown72H:
			psPacket->iType = ITEMTIMERTYPE_SPDown;
			break;

		case ITEMID_HPUp12H:
		case ITEMID_HPUp24H:
		case ITEMID_HPUp48H:
		case ITEMID_HPUp72H:
			psPacket->iType = ITEMTIMERTYPE_HPUp;
			break;

		case ITEMID_MPUp12H:
		case ITEMID_MPUp24H:
		case ITEMID_MPUp48H:
		case ITEMID_MPUp72H:
			psPacket->iType = ITEMTIMERTYPE_MPUp;
			break;

		case ITEMID_GravityStone72H:
			psPacket->iType = ITEMTIMERTYPE_WeightStone;
			break;

		case ITEMID_CartolaHat:
			psPacket->iType = ITEMTIMERTYPE_CartolaHat;
			break;

		case ITEMID_WitchHat:
			psPacket->iType = ITEMTIMERTYPE_WitchHat;
			break;

		case ITEMID_HalloweenHat:
			psPacket->iType = ITEMTIMERTYPE_HalloweenHat;
			break;

		case ITEMID_SheepHat:
			psPacket->iType = ITEMTIMERTYPE_SheepHat;
			break;

		case ITEMID_GiraffeHat:
			psPacket->iType = ITEMTIMERTYPE_GiraffeHat;
			break;

		case ITEMID_SoccerHat:
			psPacket->iType = ITEMTIMERTYPE_SoccerHat;
			break;

		case ITEMID_XMasRedHat:
			psPacket->iType = ITEMTIMERTYPE_XMasRedHat;
			break;

		case ITEMID_XMasGreenHat:
			psPacket->iType = ITEMTIMERTYPE_XMasGreenHat;
			break;

		case ITEMID_EasterHat:
			psPacket->iType = ITEMTIMERTYPE_EasterHat;
			break;

		case ITEMID_GoldenHopyHat:
			psPacket->iType = ITEMTIMERTYPE_GoldenHopyHat;
			break;

		case ITEMID_ObscureHat:
			psPacket->iType = ITEMTIMERTYPE_ObscureHat;
			break;

		case ITEMID_BigHeadLove:
			psPacket->iType = ITEMTIMERTYPE_BigHeadLove;
			break;

		case ITEMID_BigHeadSadness:
			psPacket->iType = ITEMTIMERTYPE_BigHeadSadness;
			break;

		case ITEMID_BigHeadHappyness:
			psPacket->iType = ITEMTIMERTYPE_BigHeadHappiness;
			break;

		case ITEMID_BigHeadShyness:
			psPacket->iType = ITEMTIMERTYPE_BigHeadShyness;
			break;

		case ITEMID_BigHeadAngry:
			psPacket->iType = ITEMTIMERTYPE_BigHeadAngry;
			break;

		case ITEMID_BigHeadSurprised:
			psPacket->iType = ITEMTIMERTYPE_BigHeadSurprised;
			break;

		case ITEMID_BigHeadSensual:
			psPacket->iType = ITEMTIMERTYPE_BigHeadSensual;
			break;

		case ITEMID_PhoenixEgg:
			psPacket->iType			= ITEMTIMERTYPE_PhoenixFire;
			psPacket->iItemID		= ITEMID_PhoenixFire;

			if ( iRandom < 25 )
			{
				psPacket->iType		= ITEMTIMERTYPE_PhoenixIce;
				psPacket->iItemID	= ITEMID_PhoenixIce;
			}
			else if ( iRandom < 50 )
			{
				psPacket->iType		= ITEMTIMERTYPE_PhoenixLightning;
				psPacket->iItemID	= ITEMID_PhoenixLightning;
			}
			else if ( iRandom < 75 )
			{
				psPacket->iType		= ITEMTIMERTYPE_PhoenixHeal;
				psPacket->iItemID	= ITEMID_PhoenixHealing;
			}
			break;

		case ITEMID_CaravanHopyCrystal:
		case ITEMID_CaravanHopyCheapCrystal:
		case ITEMID_CaravanHopy:
			psPacket->iType = ITEMTIMERTYPE_CaravanHopy;
			break;

		case ITEMID_CaravanRudolphCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanRudolph;
			break;

		case ITEMID_CaravanArmaCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanArma;
			break;

		case ITEMID_CaravanBumaCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanBuma;
			break;

		case ITEMID_CaravanCuepyCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanCuepy;
			break;

		case ITEMID_CaravanHungkyCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanHungky;
			break;

		case ITEMID_CaravanRabieCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanRabie;
			break;

		case ITEMID_CaravanTobieCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanTobie;
			break;

		case ITEMID_CaravanSirKnightCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanSirKnight;
			break;

		case ITEMID_CaravanPinguCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanPingu;
			break;

		case ITEMID_CaravanYetiCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanYeti;
			break;

		case ITEMID_CaravanGoldenHopyCrystal:
			psPacket->iType = ITEMTIMERTYPE_CaravanGoldenHopy;
			break;

		case ITEMID_FeatherTier1:
			psPacket->iType = ITEMTIMERTYPE_SkillCooldownReduceT1;
			break;

		case ITEMID_FeatherTier2:
			psPacket->iType = ITEMTIMERTYPE_SkillCooldownReduceT2;
			break;

		case ITEMID_FeatherTier3:
			psPacket->iType = ITEMTIMERTYPE_SkillCooldownReduceT3;
			break;

		case ITEMID_FeatherTier4:
			psPacket->iType = ITEMTIMERTYPE_SkillCooldownReduceT4;
			break;

		case ITEMID_FeatherTier5:
			psPacket->iType = ITEMTIMERTYPE_SkillCooldownReduceT5;
			break;


		case ITEMID_HPBooster:
			psPacket->iType = ITEMTIMERTYPE_HPBooster;
			break;

		case ITEMID_MPBooster:
			psPacket->iType = ITEMTIMERTYPE_MPBooster;
			break;

		case ITEMID_STMBooster:
			psPacket->iType = ITEMTIMERTYPE_STMBooster;
			break;

		case ITEMID_TuttiFruttiHat:
			psPacket->iType = ITEMTIMERTYPE_TuttiFruttiHat;
			break;

		case ITEMID_ChemistHat:
			psPacket->iType = ITEMTIMERTYPE_ChemistHat;
			break;

		case ITEMID_PatronumHat:
			psPacket->iType = ITEMTIMERTYPE_PatronumHat;
			break;

		case ITEMID_PinguMountMysterious:
			psPacket->iType = ITEMTIMERTYPE_MountPinguMysterious;
			break;

		default:
			break;
	}

	//Have Time?
	if ( psPacket->dwTimeTotal > 0 )
	{
		//Update Time Left
		psPacket->dwTimeLeft = psPacket->dwTimeTotal;

		//Delete Item From Server
		/*if ( DeleteItemInventory( pcUser->pcUserData, iOldItemID, psPacket->iChk1, psPacket->iChk2 ) < 0 )
		{
			//Item Name
			char szItemName[32] = { 0 };
			if ( FindItemName( iOldItemID, szItemName ) )
			{
				//Log It...
				PacketLogCheat sPacket;
				sPacket.iCheatID	= CHEATLOGID_ItemTimerFakeInventory;
				sPacket.LParam		= iOldItemID;
				sPacket.SParam		= psPacket->iChk1;
				sPacket.EParam		= psPacket->iChk2;
				sPacket.LxParam		= (int)szItemName;
				LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
			}

			return;
		}*/

		//Add To User
		if ( SQLAddItemTimer( pcUser, psPacket->iType, psPacket->iItemID, psPacket->dwTimeTotal ) )
		{
			if ( psPacket->iType == ITEMTIMERTYPE_CaravanHopy )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Hopy" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanRudolph )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Rudolph" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanArma )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Arma" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanBuma )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Buma" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanCuepy )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Cuepy" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanHungky )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Hungky" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanRabie )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Rabie" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanTobie )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Tobie" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanSirKnight )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Sir Knight" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanPingu )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Pingu" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanYeti )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Yeti" );
			else if ( psPacket->iType == ITEMTIMERTYPE_CaravanGoldenHopy )
				FOLLOWUNITHANDLER->SpawnUnit( pcUser, "Caravan Golden Hopy" );

			//Notify New Item to User...
			SENDPACKET( pcUser, psPacket );
		}
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketCancelItemTimer * psPacket )
{
	for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
	{
		ItemPremium * psItem = &pcUser->sPremiumData.saItemList[i];

		if ( (psItem->eType == psPacket->iType) && (psItem->dwCode == psPacket->iItemID) )
		{
			if ( (psItem->eType == ITEMTIMERTYPE_CaravanHopy)   || (psItem->eType == ITEMTIMERTYPE_CaravanRudolph) || (psItem->eType == ITEMTIMERTYPE_CaravanArma)  ||
				 (psItem->eType == ITEMTIMERTYPE_CaravanRabie)  || (psItem->eType == ITEMTIMERTYPE_CaravanTobie)  || (psItem->eType == ITEMTIMERTYPE_CaravanCuepy) ||
				 (psItem->eType == ITEMTIMERTYPE_CaravanHungky) || (psItem->eType == ITEMTIMERTYPE_CaravanBuma) || (psItem->eType == ITEMTIMERTYPE_CaravanSirKnight) ||
				 (psItem->eType == ITEMTIMERTYPE_CaravanPingu)  || (psItem->eType == ITEMTIMERTYPE_CaravanYeti) || (psItem->eType == ITEMTIMERTYPE_CaravanGoldenHopy) )
				FOLLOWUNITHANDLER->OnItemTimerEnd( pcUser );

			SQLDeleteItemTimer( pcUser, psPacket->iItemID );

			ITEMSERVER->UpdateItemTimerEXP( pcUser, psItem->eType, true );

			psItem->eType = ITEMTIMERTYPE_None;
			break;
		}
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketOldHeadUse * psPacket )
{
	if ( psPacket->bUpdate )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE CharacterInfo SET OldHead=? WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->sCharacterData.Player.szHeadModel );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
				pcDB->Execute();
			}

			pcDB->Close();
		}
	}
	else
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT OldHead FROM CharacterInfo WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
				
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					char szHead[64] = { 0 };
					pcDB->GetData( 1, PARAMTYPE_String, szHead, _countof( szHead ) );

					if ( IsNull( szHead ) )
						ZeroMemory( szHead, _countof( szHead ) );

					STRINGCOPY( pcUser->pcUserData->sCharacterData.Player.szHeadModel, szHead );
					STRINGCOPY( psPacket->szHeadModel, szHead );

					SENDPACKET( pcUser, psPacket );
				}
			}

			pcDB->Close();
		}
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketSlotItemHandle * psPacket )
{
	char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );

	char * pszAction = "Get";
	char * pszWhere = "null";

	int iLogID = ITEMLOGID_GetPerfectize;

	BOOL bPut = (psPacket->iTypeHandle == ESlotTypeHandle::ITEMSLOTHANDLE_Put) ? TRUE : FALSE;

	if ( psPacket->iTypeHandle == ESlotTypeHandle::ITEMSLOTHANDLE_Put )
		pszAction = "Put";

	if ( psPacket->iTypeWhere == ESlotTypeHandleWhere::ITEMSLOTHANDLEWHERE_Perfectize )
	{
		pszWhere = "Perfectize";

		iLogID = ITEMLOGID_GetPerfectize;
		if ( bPut )
			iLogID = ITEMLOGID_PutPerfectize;
	}
	else if ( psPacket->iTypeWhere == ESlotTypeHandleWhere::ITEMSLOTHANDLEWHERE_Swap )
	{
		pszWhere = "Swap";
		
		iLogID = ITEMLOGID_GetSwap;
		if ( bPut )
			iLogID = ITEMLOGID_PutSwap;
	}

	char szItemName[32] = { 0 };
	ITEMSERVER->FindItemName( psPacket->iItemID, szItemName );

	LOGSERVER->OnLogItemEx( pcUser, iLogID, "[%s](%s) %s item on %s ( [%s][0x%08X] %d x %d )",
		pcUser->pcUserData->szAccountName, pszCharacterName, pszAction, pszWhere, szItemName, psPacket->iItemID, psPacket->iChk1, psPacket->iChk2 );
}

void ItemServer::HandlePacket( User * pcUser, PacketNPCItemBoxSend * psPacket )
{
	if ( psPacket->iCount <= MAX_ITEMBOX_SEND_QUEUE )
	{
		PacketNPCItemBoxSendResponse sPacket;
		ZeroMemory( &sPacket, sizeof( PacketNPCItemBoxSendResponse ) );

		sPacket.iLength = sizeof( PacketNPCItemBoxSendResponse );
		sPacket.iHeader = PKTHDR_NPCItemBoxSendResponse;
		sPacket.iResult = 1;

		int iGold = 0;

		char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUser->pcUserData );

		SQLCharacter s;
		if ( CHARACTERSERVER->SQLGetCharacter( psPacket->szCharacterName, &s ) )
		{
			if ( (pcUser->GetGameLevel() > GAMELEVEL_Two) || ((STRINGCOMPAREI( psPacket->szCharacterName, pszCharacterName ) == FALSE) && (STRINGCOMPAREI( s.szAccountName, pcUser->pcUserData->szAccountName ) == FALSE)) )
			{
				int iItemsBox = ITEMSERVER->SQLItemBoxCount( s.szAccountName );
				if ( (iItemsBox + psPacket->iCount) < MAX_ITEMSINITEMBOX )
				{
					SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
					if ( pcDB->Open() )
					{
						for ( int i = 0; i < psPacket->iCount; i++ )
						{
							Item * psItem = &psPacket->cItemData[i].sItem;
							if ( STRLEN( psItem->szItemName ) >= 2 )
							{
								if ( CheckValidItem( psItem ) )
								{
									if ( ITEMSERVER->UseItem( pcUser->pcUserData, psItem->sItemID.ToInt(), psItem->iChk1, psItem->iChk2 ) >= 0 )
									{
										if ( pcDB->Prepare( "INSERT INTO ItemBox([AccountName],[SenderName],[ItemCode],[ItemSpec],[IsItem],[Item],[DateReceived],[DateDiscarded],[Date]) VALUES(?,?,?,?,1,?,NULL,NULL,GETDATE())" ) )
										{
											pcDB->BindParameterInput( 1, PARAMTYPE_String, s.szAccountName, STRLEN( s.szAccountName ) );
											pcDB->BindParameterInput( 2, PARAMTYPE_String, pszCharacterName, STRLEN( pszCharacterName ) );
											pcDB->BindParameterInput( 3, PARAMTYPE_String, psPacket->cItemData[i].szInventoryName, STRLEN( psPacket->cItemData[i].szInventoryName ) );
											pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &psItem->eSpecialization );
											pcDB->BindParameterInput( 5, PARAMTYPE_Binary, psItem, sizeof( Item ) );
											pcDB->Execute();

											sPacket.iaItemID[sPacket.iCount] = psItem->sItemID.ToInt();
											sPacket.iaChk1[sPacket.iCount] = psItem->iChk1;
											sPacket.iaChk2[sPacket.iCount] = psItem->iChk2;
											sPacket.iCount++;

											iGold += 75000;
										}
										pcDB->Clear();
									}
									else
									{
										//Item Name
										char szItemName[32] = { 0 };
										if ( ITEMSERVER->FindItemName( psItem->sItemID.ToInt(), szItemName ) )
										{
											//Log It...
											PacketLogCheat sPacket;
											sPacket.iCheatID = CHEATLOGID_ItemBoxItemDupe;
											sPacket.LParam = psItem->sItemID.ToInt();
											sPacket.SParam = psItem->iChk1;
											sPacket.EParam = psItem->iChk2;
											STRINGCOPY( sPacket.szBuffer1, szItemName );
											LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
										}
									}
								}
							}
						}
						pcDB->Close();
					}
				}
				else
					sPacket.iResult = -1;
			}
			else
				sPacket.iResult = -2;
		}
		else
			sPacket.iResult = -3;

		if ( sPacket.iResult == 1 )
		{
			if ( iGold > 0 )
				CHARACTERSERVER->GiveGOLD( pcUser, -iGold, WHEREID_Distributor );
		}

		SENDPACKET( pcUser, &sPacket, TRUE );
	}
}

void ItemServer::HandlePacket( User * pcUser, PacketRequestBeginnerItemsTimer * psPacket )
{
	if ( pcUser->pcUserData->sCharacterData.iLevel <= MAX_LEVEL_BEGINNERSBUFF )
	{
		EItemID iItemID = ITEMID_None;
		DWORD dwTimeLeft = 0, dwTimeTotal = 0;
		if ( (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_ExpUp, iItemID, dwTimeLeft, dwTimeTotal )	== FALSE) &&
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_HPUp, iItemID, dwTimeLeft, dwTimeTotal )	== FALSE) &&
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_MPDown, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE) &&
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_SPDown, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE) &&
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_SheepHat, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE) &&
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_HPBooster, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE) && 
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_STMBooster, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE) && 
			 (SQLGetItemTimer( pcUser, ITEMTIMERTYPE_MPBooster, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE) )
		{
			CreatePremium( pcUser->pcUserData, ITEMID_ExpUp24H, ITEMTIMERTYPE_ExpUp, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_HPUp12H, ITEMTIMERTYPE_HPUp, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_MPDown12H, ITEMTIMERTYPE_MPDown, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_SPDown12H, ITEMTIMERTYPE_SPDown, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_SheepHat, ITEMTIMERTYPE_SheepHat, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_HPBooster, ITEMTIMERTYPE_HPBooster, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_STMBooster, ITEMTIMERTYPE_STMBooster, ITEMTIMERTIME_3H );
			CreatePremium( pcUser->pcUserData, ITEMID_MPBooster, ITEMTIMERTYPE_MPBooster, ITEMTIMERTIME_3H );

			LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterItemTimerBeginner, "Character[%s] Received the Beginners Buff", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
		}
		else
			CHATSERVER->SendChatMessageBox( pcUser, "You already have an activated item" );

		//Add level
		if ( pcUser->pcUserData->sCharacterData.iLevel < 130 )
			CHARACTERSERVER->GiveEXP( pcUser, CHARACTERSERVER->GetExpFromLevel( 130 ) - CHARACTERSERVER->GetExp( pcUser->pcUserData ) );

		QUESTSERVER->SkipTier5Quests(pcUser);
		SENDPACKETBLANK(pcUser, PKTHDR_YurinaSkipQuests);
	}
	else
		CHATSERVER->SendChatMessageBox( pcUser, "You must be lv.1~%d to get the beginner's buff", MAX_LEVEL_BEGINNERSBUFF );
}

void ItemServer::HandlePacket( User * pcUser, PacketRequestChemistItemTimer * psPacket )
{
	EItemID iItemID = ITEMID_None;
	DWORD dwTimeLeft = 0, dwTimeTotal = 0;
	if ( SQLGetItemTimer( pcUser, ITEMTIMERTYPE_ChemistHat, iItemID, dwTimeLeft, dwTimeTotal ) == FALSE )
	{
		CreatePremium( pcUser->pcUserData, ITEMID_ChemistHat, ITEMTIMERTYPE_ChemistHat, ITEMTIMERTIME_12H );

		LOGSERVER->OnLogEx( pcUser, ACCLOGID_CharacterItemTimerChemist, "Character[%s] Received the Chemist Buff", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
	}
	else
		CHATSERVER->SendChatMessageBox( pcUser, "You already have activated the item" );
}

void ItemServer::UpdateQueueRecovery()
{
	ItemServer::psRecoveryQueue = new LoadItemDataHandler();
	LoadItemDataHandler * ps = ItemServer::psRecoveryQueue;

	ps->bLoaded = FALSE;
	ps->iID = -1;
	ZeroMemory( &ps->sItem, sizeof( ItemLoadData ) );
	ZeroMemory( ps->szCode, _countof( ps->szCode ) );

	//Wait
	while( true )
	{
		if ( (ps->bLoaded == FALSE) && (ps->iID >= 0) && (ps->szCode[0] != 0) )
		{
			if ( ITEMSERVER )
			{
				if ( ITEMSERVER->OnLoadItemData( &ps->sItem, ps->szCode ) )
				{
					ps->szCode[0] = 0;
					ps->bLoaded = TRUE;

				}
			}
		}

		WaitForSingleObject( ItemServer::hEventRecoveryQueue, 1500 );
	}
}

void ItemServer::DeleteQueue()
{
	//Exit Thread
	TerminateThread( ItemServer::hThreadRecovery, 0 );
	CloseHandle( ItemServer::hEventRecoveryQueue );

	DELET( ItemServer::psRecoveryQueue );
}

void ItemServer::CheckItemRecovery( User * pcUser )
{
	LoadItemDataHandler * ps = ItemServer::psRecoveryQueue;
	if ( ps->bLoaded && (ps->iID >= 0) && (pcUser->GetID() == ps->iID) )
	{
		ITEMSERVER->SendItemData( pcUser->pcUserData, &ps->sItem.sItem );
		ps->bLoaded = FALSE;
		ps->iID		= -1;

		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "ItemServer> Recovered!" );
	}
}

void ItemServer::AddItemRecovery( User * pcUser, const char * pszCode )
{
	LoadItemDataHandler * ps = ItemServer::psRecoveryQueue;
	if ( ps->bLoaded == FALSE )
	{
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "ItemServer> Recovering..." );
		STRINGCOPY( ps->szCode, pszCode );
		if ( (ps->szCode[0] == '@') && (STRLEN(pszCode) > 2) )
			STRINGCOPY( ps->szCode, pszCode + 1 );

		ps->iID		= pcUser->GetID();
	}
	else
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "ItemServer> Wait for previous recovery queue..." );
}

bool ItemServer::IsItemTimerFreezedMap( EMapID iMapID, EItemTimerType iType )
{
	bool bRet = false;

	switch ( iType )
	{
		case ITEMTIMERTYPE_ExpUp:
		case ITEMTIMERTYPE_ThirdEye:
			if ( (iMapID == MAPID_RicartenTown) || (iMapID == MAPID_PhillaiTown) || (iMapID == MAPID_Eura) ||
				(iMapID == MAPID_BattleTown) || (iMapID == MAPID_Atlantis) || (iMapID == MAPID_BlessCastle) ||
				(iMapID == MAPID_GhostCastle) || (iMapID == MAPID_ForestDungeon) || (iMapID == MAPID_DragonsDungeon) || (iMapID == MAPID_Bellatra) )
				bRet = true;
			break;
		
		case ITEMTIMERTYPE_Phoenix:
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			if ( (iMapID == MAPID_RicartenTown) || (iMapID == MAPID_PhillaiTown) || (iMapID == MAPID_Eura) ||
				(iMapID == MAPID_BattleTown) || (iMapID == MAPID_Atlantis) || (iMapID == MAPID_BlessCastle) || 
				(iMapID == MAPID_Bellatra) )
				bRet = true;
			break;
	
		case ITEMTIMERTYPE_MPDown:
		case ITEMTIMERTYPE_SPDown:
		case ITEMTIMERTYPE_HPUp:
		case ITEMTIMERTYPE_MPUp:
		case ITEMTIMERTYPE_BigHead:
		case ITEMTIMERTYPE_WeightStone:
		case ITEMTIMERTYPE_MatureStone:
		case ITEMTIMERTYPE_SkillStone:
		case ITEMTIMERTYPE_CartolaHat:
		case ITEMTIMERTYPE_HalloweenHat:
		case ITEMTIMERTYPE_WitchHat:
		case ITEMTIMERTYPE_SheepHat:
		case ITEMTIMERTYPE_GiraffeHat:
		case ITEMTIMERTYPE_SoccerHat:
		case ITEMTIMERTYPE_XMasGreenHat:
		case ITEMTIMERTYPE_XMasRedHat:
		case ITEMTIMERTYPE_BigHeadHappiness:
		case ITEMTIMERTYPE_BigHeadLove:
		case ITEMTIMERTYPE_BigHeadSadness:
		case ITEMTIMERTYPE_BigHeadShyness:
		case ITEMTIMERTYPE_BigHeadAngry:
		case ITEMTIMERTYPE_BigHeadSurprised:
		case ITEMTIMERTYPE_BigHeadSensual:
		case ITEMTIMERTYPE_BCBuff:
		case ITEMTIMERTYPE_PumpkinHat:
		case ITEMTIMERTYPE_TuttiFruttiHat:
		case ITEMTIMERTYPE_EasterHat:
		case ITEMTIMERTYPE_ChemistHat:
		case ITEMTIMERTYPE_PatronumHat:
		case ITEMTIMERTYPE_GoldenHopyHat:
		case ITEMTIMERTYPE_ObscureHat:
			if ( (iMapID == MAPID_RicartenTown) || (iMapID == MAPID_PhillaiTown) || (iMapID == MAPID_Atlantis) ||
				(iMapID == MAPID_Bellatra) )
				bRet = true;
			break;

		case ITEMTIMERTYPE_CaravanHopy:
		case ITEMTIMERTYPE_CaravanRudolph:
		case ITEMTIMERTYPE_CaravanArma:
		case ITEMTIMERTYPE_CaravanBuma:
		case ITEMTIMERTYPE_CaravanCuepy:
		case ITEMTIMERTYPE_CaravanHungky:
		case ITEMTIMERTYPE_CaravanRabie:
		case ITEMTIMERTYPE_CaravanTobie:
		case ITEMTIMERTYPE_CaravanSirKnight:
		case ITEMTIMERTYPE_CaravanPingu:
		case ITEMTIMERTYPE_CaravanYeti:
		case ITEMTIMERTYPE_CaravanGoldenHopy:
		case ITEMTIMERTYPE_SkillCooldownReduceT1:
		case ITEMTIMERTYPE_SkillCooldownReduceT2:
		case ITEMTIMERTYPE_SkillCooldownReduceT3:
		case ITEMTIMERTYPE_SkillCooldownReduceT4:
		case ITEMTIMERTYPE_SkillCooldownReduceT5:
		case ITEMTIMERTYPE_HPBooster:
		case ITEMTIMERTYPE_MPBooster:
		case ITEMTIMERTYPE_STMBooster:
			if ( (iMapID == MAPID_RicartenTown) || (iMapID == MAPID_PhillaiTown) || (iMapID == MAPID_Eura) ||
				(iMapID == MAPID_BattleTown) || (iMapID == MAPID_Atlantis) || (iMapID == MAPID_BlessCastle) ||
				(iMapID == MAPID_Bellatra) )
				bRet = true;
			break;


		default:
			break;
	}

	return bRet;
}

BOOL ItemServer::SendItemShopList( User * pcUser, UnitData * pcUnitData )
{
	PacketItemShopList sPacket;
	sPacket.iLength = sizeof( PacketItemShopList );
	sPacket.iHeader = PKTHDR_ItemShopList;
	sPacket.iUniqueID = (pcUser->GetID() ^ CRC_ITEMSHOP_UNIQUEID);
	sPacket.iNPCID = (pcUnitData->iID ^ CRC_ITEMSHOP_UNIQUEID);
	
	if ( pcUnitData->sUnitInfo.iWeaponShopCounter )
	{
		int iLength = 32;
		sPacket.iItemCount = 0;

		int iBufferLength = 0;

		for ( int i = 0; i < pcUnitData->sUnitInfo.iWeaponShopCounter; i++ )
		{
			DefinitionItem * psDefinitionItem = (DefinitionItem*)pcUnitData->sUnitInfo.dwaWeaponShop[i];
			if ( psDefinitionItem )
			{
				if ( (psDefinitionItem->DefCompressDataLen + iLength) < 0x2000 )
				{
					CopyMemory( sPacket.szBuffer + iBufferLength, psDefinitionItem->DefCompressData, psDefinitionItem->DefCompressDataLen );
					iLength += psDefinitionItem->DefCompressDataLen;
					iBufferLength += psDefinitionItem->DefCompressDataLen;
					sPacket.iItemCount++;
				}
			}
		}

		sPacket.iLength = iLength;
		sPacket.iShopType = 1;
		SENDPACKET( pcUser, &sPacket );
	}

	if ( pcUnitData->sUnitInfo.iDefenseShopCounter )
	{
		int iLength = 32;
		sPacket.iItemCount = 0;

		int iBufferLength = 0;

		for ( int i = 0; i < pcUnitData->sUnitInfo.iDefenseShopCounter; i++ )
		{
			DefinitionItem * psDefinitionItem = (DefinitionItem*)pcUnitData->sUnitInfo.dwaDefenseShop[i];
			if ( psDefinitionItem )
			{
				if ( (psDefinitionItem->DefCompressDataLen + iLength) < 0x2000 )
				{
					CopyMemory( sPacket.szBuffer + iBufferLength, psDefinitionItem->DefCompressData, psDefinitionItem->DefCompressDataLen );
					iLength += psDefinitionItem->DefCompressDataLen;
					iBufferLength += psDefinitionItem->DefCompressDataLen;
					sPacket.iItemCount++;
				}
			}
		}

		sPacket.iLength = iLength;
		sPacket.iShopType = 2;
		SENDPACKET( pcUser, &sPacket );
	}

	if ( pcUnitData->sUnitInfo.iMiscShopCounter )
	{
		int iLength = 32;
		sPacket.iItemCount = 0;

		int iBufferLength = 0;

		for ( int i = 0; i < pcUnitData->sUnitInfo.iMiscShopCounter; i++ )
		{
			DefinitionItem * psDefinitionItem = (DefinitionItem*)pcUnitData->sUnitInfo.dwaMiscShop[i];
			if ( psDefinitionItem )
			{
				if ( (psDefinitionItem->DefCompressDataLen + iLength) < 0x2000 )
				{
					CopyMemory( sPacket.szBuffer + iBufferLength, psDefinitionItem->DefCompressData, psDefinitionItem->DefCompressDataLen );
					iLength += psDefinitionItem->DefCompressDataLen;
					iBufferLength += psDefinitionItem->DefCompressDataLen;
					sPacket.iItemCount++;
				}
			}
		}

		sPacket.iLength = iLength;
		sPacket.iShopType = 3;
		SENDPACKET( pcUser, &sPacket );
	}

	return TRUE;
}