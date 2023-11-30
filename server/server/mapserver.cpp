#include "stdafx.h"
#include "mapserver.h"

EMapID * MapServer::piBabelStormMapID = (EMapID *)0x006E2C30;
DWORD * MapServer::pdwBabelStormEndTime = (DWORD *)0x07AC9D54;

typedef int( __thiscall *t_SetNewSpawn ) ( int point, int X, int Z, int v );
t_SetNewSpawn SetNewSpawn = ( t_SetNewSpawn )0x0054F280;

typedef void( __thiscall *t_DeleteFlagPoint )(int iPoint, int iX, int iZ);
t_DeleteFlagPoint fnDeleteFlagPoint = (t_DeleteFlagPoint)0x0054F300;

typedef void( __thiscall *t_SendFlagPoint )(int iPoint);
t_SendFlagPoint SendFlagPoint = (t_SendFlagPoint)0x0054F090;

BOOL bTestDebugDamageUnit = TRUE;

BOOL * pbIsCrystalMonster		= (BOOL*)(0x08B81830);
BOOL * pbIsSummonedGMMonster	= (BOOL*)(0x08B81834);
BOOL * pbIsSummonedMonster		= (BOOL*)(0x08B81838);


MapServer::MapServer()
{
	pcaBaseMap = (BaseMap*)0x0075B038;

	for ( int i = 0; i < NUM_MAPS; i++ )
		vSpawnMapsInfo.push_back( new SpawnDataInfo( i ) );
}

MapServer::~MapServer()
{
	for ( auto ps : vBossMapPosition )
		DELET( ps );
	
	vBossMapPosition.clear();

	for ( auto ps : vSpawnMapsInfo )
		DELET( ps );
	
	vSpawnMapsInfo.clear();
}

void MapServer::AddMapSpawnData( int iMapID, const char * pszName, int iID, int iX, int iZ )
{
	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		SpawnDataInfo::SpawnData * psData = new SpawnDataInfo::SpawnData;
		psData->iID = iID;
		psData->iX	= iX;
		psData->iZ	= iZ;
		STRINGCOPY( psData->szName, pszName );
		vSpawnMapsInfo[iMapID]->vSpawnData.push_back( psData );
	}
}

void MapServer::DelMapSpawnData( int iMapID, int iX, int iZ )
{
	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		for ( std::vector<SpawnDataInfo::SpawnData*>::iterator it = vSpawnMapsInfo[iMapID]->vSpawnData.begin(); it != vSpawnMapsInfo[iMapID]->vSpawnData.end(); )
		{
			SpawnDataInfo::SpawnData * ps = (*it);

			if ( ps && (ps->iX == iX) && (ps->iZ == iZ) )
			{
				DELET( ps );
				it = vSpawnMapsInfo[iMapID]->vSpawnData.erase( it );
			}
			else
				it++;
		}
	}
}

SpawnDataInfo::SpawnData * MapServer::GetMapSpawnData( int iMapID, int iX, int iZ )
{
	SpawnDataInfo::SpawnData * psRet = NULL;

	if ( (iMapID >= 0) && ((UINT)iMapID < vSpawnMapsInfo.size()) )
	{
		for ( std::vector<SpawnDataInfo::SpawnData*>::iterator it = vSpawnMapsInfo[iMapID]->vSpawnData.begin(); it != vSpawnMapsInfo[iMapID]->vSpawnData.end(); it++ )
		{
			SpawnDataInfo::SpawnData * ps = (*it);

			if ( ps && (ps->iX == iX) && (ps->iZ == iZ) )
			{
				psRet = ps;
				break;
			}
		}
	}

	return psRet;
}

Map * MapServer::GetMap( int iMapID )
{
	if ( (iMapID >= 0) && (iMapID <= 80) )
		return MAPSDATA + iMapID;

	return MAPSDATA;
}

NAKED UINT MapServer::GetMinuteOfDay()
{
	JMP( pfnGetMinuteOfDay );
}

NAKED BOOL MapServer::HandleUnitQuestMap( void * pcMap, UnitData * pcUnitData )
{
	JMP( pfnQuestUnitMapHandler );
}

void MapServer::SetFlagPoint( UserData * pcUserData, const char * pszDescription )
{
	if ( pcUserData && lstrlenA( pszDescription ) )
	{
		int iX = pcUserData->sPosition.iX;
		int iZ = pcUserData->sPosition.iZ;
		int iStageID = pcUserData->iMapID;

		int iPoint = (int)(MAPSDATA + pcUserData->iMapID);

		if ( iPoint != 0 )
		{

			// Get Last ID
			int iID = 0;
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT TOP 1 ID FROM MapSpawnPoint ORDER BY ID DESC" ) )
				{
					if ( pcDB->Execute() && pcDB->Fetch() )
						pcDB->GetData( 1, PARAMTYPE_Integer, &iID, 0 );
				}
				pcDB->Close();
			}

			iID++;
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO MapSpawnPoint([ID],[Stage],[X],[Z],[Description]) VALUES(?,?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iStageID );
					int ishrX = iX >> 8;
					int ishrZ = iZ >> 8;
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &ishrX );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &ishrZ );
					pcDB->BindParameterInput( 5, PARAMTYPE_String, (void*)pszDescription );
					pcDB->Execute();
				}
				pcDB->Close();
			}

			SetNewSpawn( iPoint, iX >> 8, iZ >> 8, 1 );
			SendFlagPoint( iPoint );

			AddMapSpawnData( iStageID, pszDescription, iID, iX >> 8, iZ >> 8 );

			CHATSERVER->SendDebugChat( pcUserData, CHATCOLOR_Error, "> Flag Point '%s' Added! [X: %d   Z: %d]", pszDescription, iX >> 8, iZ >> 8 );
		}
		else
			CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "> Map not found!" );
	}
}

void MapServer::DeleteFlagPoint( UserData * pcUserData, PacketDeleteFlagPoint * psPacket )
{
	if ( pcUserData && pcUserData->iGameLevel > 3 )
	{
		Map * pcMap = MAPSDATA + pcUserData->iMapID;
		if ( pcMap )
		{
			if ( pcMap->pcBaseMap )
			{
				fnDeleteFlagPoint( (int)pcMap, psPacket->iX >> 8, psPacket->iZ >> 8 );
				SendFlagPoint( (int)pcMap );


				SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

				int iX = psPacket->iX >> 8;
				int iZ = psPacket->iZ >> 8;

				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "DELETE FROM MapSpawnPoint WHERE (X=?) AND (Z=?)" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iX );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iZ );
						pcDB->Execute();
					}
					pcDB->Close();
				}

				DelMapSpawnData( pcMap->pcBaseMap->GetID(), iX, iZ );
			}
		}
	}
}

void MapServer::OnSendDebugDamage( User * pcUser, PacketDebugDamageInfo * psPacket )
{
	if ( pcUser->GetMapID() != MAPID_Bellatra )
	{
		psPacket->dwObjectID = pcUser->GetID();

		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUserOther = USERSERVER->pcaUserInGame[i];
			if ( (pcUser != pcUserOther) && (pcUser->GetMapID() == pcUserOther->GetMapID()) )
			{
				int iX = (pcUser->pcUserData->sPosition.iX - pcUserOther->pcUserData->sPosition.iX) >> 8;
				int iY = (pcUser->pcUserData->sPosition.iY - pcUserOther->pcUserData->sPosition.iY) >> 8;
				int iZ = (pcUser->pcUserData->sPosition.iZ - pcUserOther->pcUserData->sPosition.iZ) >> 8;
				int iXZ = abs( (iX * iX) - (iZ * iZ) );
				
				if ( iXZ <= 90000 && iY <= 80 )
					SENDPACKET( pcUserOther, psPacket );
			}
		}
	}
}

void MapServer::OnSendDebugDamageUnit( UnitData * pcUnitData, int iDamage )
{
	if ( pcUnitData && bTestDebugDamageUnit && pcUnitData->sCharacterData.iType != 0 && *(int*)(0x08B8176C) > 0 )
	{
		PacketDebugDamageInfo sPacket;
		sPacket.iLength		= sizeof( PacketDebugDamageInfo );
		sPacket.iHeader		= PKTHDR_DamageDebugInfo;
		sPacket.dwObjectID	= pcUnitData->iID;
		sPacket.iTypeAction = *(BOOL*)(0x08B81260) == TRUE ? 6 : 5;
		*(BOOL*)(0x08B81260) = FALSE;
		sPacket.iValue		= *(int*)0x08B8176C;
		*(int*)0x08B8176C	= 0;

		for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
		{
			User * pcUserOther = USERSERVER->pcaUserInGame[i];
			if ( pcUserOther->GetMapID() != MAPID_Bellatra )
			{
				int iX = (pcUnitData->sPosition.iX - pcUserOther->pcUserData->sPosition.iX) >> 8;
				int iY = (pcUnitData->sPosition.iY - pcUserOther->pcUserData->sPosition.iY) >> 8;
				int iZ = (pcUnitData->sPosition.iZ - pcUserOther->pcUserData->sPosition.iZ) >> 8;
				int iXZ = abs( (iX * iX) - (iZ * iZ) );
				
				if ( iXZ <= 90000 && iY <= 80 )
					SENDPACKET( pcUserOther, &sPacket );
			}
		}
	}
}

BOOL MapServer::AddMonsterSpawn( Map * pcMap, UnitData * pcUnitData )
{
	//Get Index of empyt Unit in Stage
    int iIndex = GetEmptyIndexUnitData( pcMap );
 
	if ( iIndex >= 0 )
	{
		//Set Unit to stage
		pcMap->pcaUnitData[iIndex] = pcUnitData;

		//Set Pointer of stage to Unit
		pcUnitData->pcMapSpawn = (void*)pcMap;

		//Not a	Special Unit?
        if ( (*pbIsCrystalMonster) == FALSE && (*pbIsSummonedGMMonster) == FALSE && (*pbIsSummonedMonster) == FALSE )
        {
			if ( pcUnitData->iSpawnPointIndex >= 0 )
			{
				//Count It in Spawn.
				pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex]++;
			}
        }
        else //Otherwise Set No Counter Flag
            pcUnitData->bNoCountInSpawn = TRUE;

		//Is Special Unit?
		if ( pcUnitData->bNoCountInSpawn )
		{
			//Clear all flags
			if ( (*pbIsSummonedMonster) )
				(*pbIsSummonedMonster)		= FALSE;

			if ( (*pbIsSummonedGMMonster) )
				(*pbIsSummonedGMMonster)	= FALSE;

			if ( (*pbIsCrystalMonster) )
				(*pbIsCrystalMonster)		= FALSE;
		}


		//Set last Unit Pointer at Stage
		for ( int i = iIndex; i < MAX_ALIVEMONSTERS; i++ )
		{
			if ( pcMap->pcaUnitData[i] != NULL )
				pcMap->iNumUsingUnitData = i + 1;
		}

		//Success
        return TRUE;
    }

	//Fail
    return FALSE;
}

BOOL MapServer::DelMonsterSpawn( Map * pcMap, UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	//Not a Special Unit? then no need to decrase Counter on Spawn Flag
	if ( (pcUnitData->iSpawnPointIndex >= 0) && (pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex] > 0) )
		pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex]--;
	else
		bRet = TRUE;

	//Off Flag
	pcUnitData->bNoCountInSpawn = FALSE;

	//Clear Unit Pointer on Stage (killed or closed Unit)
	for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
	{
		if ( pcMap->pcaUnitData[i] == pcUnitData )
		{
			pcMap->pcaUnitData[i] = NULL;
			
			//Success
			bRet					= TRUE;
			break;
		}
	}
	return bRet;
}

BOOL MapServer::DisableMonsterSpawn( Map * pcMap, UnitData * pcUnitData )
{
	if (pcMap && pcUnitData)
	{
		//Not a Special Unit? then no need to decrase Counter on Spawn Flag
		if ((pcUnitData->iSpawnPointIndex >= 0) && (pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex] > 0))
		{
			pcUnitData->bNoCountInSpawn = TRUE;
			pcMap->iaSpawnMonsterActive[pcUnitData->iSpawnPointIndex]--;

			//Debug: if (pcUnitData->pcOwner)
			//	CHATSERVER->SendChatEx(pcUnitData->pcOwner->pcSocketData->u, CHATCOLOR_Error, "> Monster `%s` disabled from spawn!", pcUnitData->sCharacterData.szName);

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL MapServer::DelItem( Map * pcMap, Map::Loot * psItem )
{
	if( psItem )
	{
		psItem->bInUse = FALSE;

		PacketGetItem sPacket;
		sPacket.iHeader = PKTHDR_DeleteItem;
		sPacket.iLength = sizeof(PacketGetItem);
		sPacket.sPosition = Point3D( psItem->sPosition.iX << 8, psItem->sPosition.iY << 8, psItem->sPosition.iZ << 8 );
		sPacket.pcMap = pcMap;

		for( size_t i = 0; i < PLAYERS_MAX; i++ )
		{
			UserData * pcUserData = USERSDATA + i;
			if( pcUserData->pcSocketData && pcUserData->iID && pcUserData->iMapID == pcMap->pcBaseMap->iMapID )
			{
				int iX = psItem->sPosition.iX - ( pcUserData->sPosition.iX >> 8 );
				int iZ = psItem->sPosition.iZ - ( pcUserData->sPosition.iZ >> 8 );
				int iDistance = iX * iX + iZ * iZ;

				if( iDistance < 0xC8000 && abs( iX ) < 4096 && abs( iZ ) < 4096 )
					SENDPACKET( USERDATATOUSER(pcUserData), &sPacket );
			}
		}
	}

	return TRUE;
}

void MapServer::Load()
{
	ZeroMemory( pcaBaseMap, sizeof( BaseMap ) * 64 );

	for( int i = 0; i < 256; i++ )
		pcaBaseMap[i].SetID( (EMapID)i );

	//Open SQL Connection
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	//Get Total Maps in SQL
	int iCountMaps = 0;
	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "SELECT COUNT(*) FROM MapList" ) )
		{
			if( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCountMaps, 0 );
		}
		pcDB->Close();
	}

	if( pcDB->Open() )
	{
		if( pcDB->Prepare( "SELECT * FROM MapList ORDER BY ID ASC" ) )
		{
			if( pcDB->Execute() && pcDB->Fetch() )
			{
				for( int i = 0; i < iCountMaps; i++ )
				{
					//Get Parameters
					char szShortName[256] = { 0 };
					int iLevelMap = 0;
					char szFilePath[256] = { 0 };

					pcDB->GetData( 3, PARAMTYPE_String, szShortName, 256 );
					pcDB->GetData( 5, PARAMTYPE_Integer, &iLevelMap, 0 );
					pcDB->GetData( 7, PARAMTYPE_String, szFilePath, 256 );

					BaseMap * pc = ( pcaBaseMap + i );
					pc->SetLevelRequirement( MAPLEVEL( i ) );
					pc->SetFileName( szFilePath, szShortName );

					//Navisko Town?
					if( i == MAPID_NaviskoTown )
					{
						pc->SetType( MAPTYPE_Desert );
						pc->SetLevelRequirement( MAPLEVEL( MAPID_NaviskoTown ) );
						pc->SetNightSpawn( TRUE );
						pc->SetCenter( 21928, -2335 );
						pc->AddSpawn( 21928, -2335 );
					}

					//Bless Castle?
					if( i == MAPID_BlessCastle )
					{
						pc->SetType( MAPTYPE_BlessCastle );
						pc->SetLevelRequirement( MAPLEVEL( MAPID_BlessCastle ) );
						pc->SetCenter( 35225, -23847 );
						pc->AddSpawn( 33920, -23479 );
					}

					*(UINT*)(0x08B80008 + (i * 4)) = MAPLEVEL( i );

					if( !pcDB->Fetch() )
						break;
				}
			}
		}
		pcDB->Close();
	}

	SQLLoadMapBossPosition();
}

BOOL MapServer::IsPVPMap( int iMapID )
{
	if ( (*(int*)0x0084831C) != (-1) && (*(int*)0x0084831C) == iMapID )
		return TRUE;

	if ( BATTLEROYALEHANDLER->IsPVPMap( (EMapID)iMapID ) )
		return TRUE;

	if ( iMapID == MAPID_BlessCastle )
		return TRUE;

	//Restricted Maps
	if ( EVENT_PVPMODE )
	{
		BOOL bRes = TRUE;

		switch ( iMapID )
		{
			case MAPID_QuestArena:
			case MAPID_GhostCastle:
			case MAPID_OldRuinen2:
			case MAPID_RoyalDesert:
			case MAPID_ForestDungeon:
			case MAPID_RicartenTown:
			case MAPID_PhillaiTown:
			case MAPID_BattleTown:
			case MAPID_Swamp:
			case MAPID_DistortedForest:
			case MAPID_T5QuestArena:
			case MAPID_Atlantis:
			case MAPID_Bellatra:
			case MAPID_DragonsDungeon:
				bRes = FALSE;
				break;

			default:
				break;
		}

		return bRes;
	}

	return FALSE;
}

BOOL MapServer::IsPKMap( int iMapID )
{
	BOOL bResult = TRUE;
	switch ( iMapID )
	{
		case MAPID_QuestArena:
		case MAPID_GhostCastle:
		case MAPID_OldRuinen2:
		case MAPID_RoyalDesert:
		case MAPID_ForestDungeon:
		case MAPID_RicartenTown:
		case MAPID_PhillaiTown:
		case MAPID_BattleTown:
		case MAPID_Swamp:
		case MAPID_DistortedForest:
		case MAPID_T5QuestArena:
		case MAPID_Atlantis:
		case MAPID_Bellatra:
		case MAPID_DragonsDungeon:
			bResult = FALSE;
			break;
		default:
			break;
	}
	return bResult;
}

BOOL MapServer::IsEXPMap( int iMapID )
{
	BOOL bResult = FALSE;
	switch ( iMapID )
	{
		case MAPID_MysteryForest1:
		case MAPID_MysteryForest2:
		case MAPID_MysteryForest3:
		case MAPID_MysteryDesert1:
		case MAPID_MysteryDesert2:
		case MAPID_MysteryDesert3:
		case MAPID_ForgottenTempleF1:
		case MAPID_ForgottenTempleF2:
		case MAPID_AncientDungeonF1:
		case MAPID_AncientDungeonF2:
		case MAPID_AncientDungeonF3:
		case MAPID_IronCore:
			bResult = TRUE;
			break;
		default:
			break;
	}
	return bResult;
}

BOOL MapServer::IsBellatraMapArea( int iX, int iY, int iZ )
{
	BOOL bRes = FALSE;
	__asm
	{
		PUSH iZ;
		PUSH iY;
		PUSH iX;
		MOV EAX, 0x0054DF10;
		CALL EAX;
		ADD ESP, 0x0C;
		MOV bRes, EAX;
	}
	return bRes;
}

BOOL MapServer::IsSummonMap( int iMapID )
{
	if ( iMapID == MAPID_T5QuestArena )
		return FALSE;

	return TRUE;
}

BOOL MapServer::IsValidMap( Map * pcMap )
{
	for( int i = 0; i < 80; i++ )
		if( pcMap == MAPSDATA + i )
			return TRUE;

	return FALSE;
}

int MapServer::GetEmptyIndexUnitData( Map * pcMap )
{
	for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
	{
		//Empty Pointer? Found.
		if ( pcMap->pcaUnitData[i] == NULL )
			return i;
	}

	//Not Found
	return (-1);
}

void MapServer::SendStageItem( Map * pcMap, UserData * pcUserData )
{
	if ( !pcUserData )
		return;

	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );

	int dX = 0, dZ = 0, iDistance = 0;
	for ( int i = 0; i < MAX_LOOT; i++ )
	{
		if ( sPacket.iCount == 50 )
			break;

		Map::Loot * p = pcMap->saLoot + i;
		if ( p->bInUse && p->dwDropTime < TICKCOUNT && (p->dwEndTime - 10000) > TICKCOUNT )
		{
			if ( p->bExclusive && (p->lExclusiveOwnerID != 0 && p->lExclusiveOwnerID != 1) )
			{
				if ( p->lExclusiveOwnerID != pcUserData->iID )
				{
					BOOL bExclusive = BATTLEROYALEHANDLER->IsExclusiveDrop( USERDATATOUSER( pcUserData ), p->lExclusiveOwnerID );
					
					if ( bExclusive == FALSE )
						bExclusive = BELLATRASERVERHANDLER->IsExclusiveDrop( USERDATATOUSER( pcUserData ), p->lExclusiveOwnerID );

					if ( bExclusive == FALSE )
						bExclusive = DROPAREAHANDLER->IsExclusiveDrop( p );

					if ( bExclusive == FALSE )
						continue;
				}
			}

			dX = p->sPosition.iX - (pcUserData->sPosition.iX >> 8);
			dZ = p->sPosition.iZ - (pcUserData->sPosition.iZ >> 8);

			iDistance = (dX*dX+dZ*dZ);

			if ( iDistance < 0xC8000 && abs( dX ) < 4096 && abs( dZ ) < 4096 )
			{
				DWORD dwCode = p->sLootItem.sItem.sItemID.ToItemID();

				sPacket.saItems[sPacket.iCount].bActive = p->bInUse;
				sPacket.saItems[sPacket.iCount].iX		= p->sPosition.iX << 8;
				sPacket.saItems[sPacket.iCount].iY		= p->sPosition.iY << 8;
				sPacket.saItems[sPacket.iCount].iZ		= p->sPosition.iZ << 8;
				sPacket.saItems[sPacket.iCount].iItemID = dwCode;
				sPacket.saItems[sPacket.iCount].iClass	= ClassFlagToCharacterClass( (EClassFlag)p->sLootItem.sItem.eSpecialization );

				dwCode &= 0xFFFF0000;

				if( p->sLootItem.sItem.iPotionCount > 1 && ( dwCode & 0xFF000000 ) == 0x04000000 )
					STRINGFORMAT( sPacket.saItems[sPacket.iCount].szName, "%sx%d", p->sLootItem.sItem.szItemName, p->sLootItem.sItem.iPotionCount );
				else
					STRINGCOPY( sPacket.saItems[sPacket.iCount].szName, p->sLootItem.sItem.szItemName );

				sPacket.iCount++;
			}
		}
	}
	if ( sPacket.iCount > 0 )
	{
		sPacket.iLength = (sizeof( PacketStageItemData::StageItemData )* sPacket.iCount) + 16;
		sPacket.iHeader = PKTHDR_PlayItem;
		sPacket.bActive = TRUE;
		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
	}
}

void MapServer::SendStageSpawn( Map * pcMap, UserData * pcUserData )
{
	if ( !pcUserData )
		return;

	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );

	int dX = 0, dZ = 0, iDistance = 0;
	for ( int i = 0; i < 200; i++ )
	{
		if ( sPacket.iCount == 50 && pcUserData->iGameLevel >= GAMELEVEL_Three )
		{
			sPacket.iLength = (sizeof( PacketStageItemData::StageItemData )* sPacket.iCount) + 16;
			sPacket.iHeader = PKTHDR_PlayItem;
			sPacket.bActive = TRUE;
			SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
			ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );
		}

		Map::SpawnFlag * p = pcMap->saSpawnFlag + i;
		if ( p->bActive )
		{
			dX = p->iX - (pcUserData->sPosition.iX >> 8);
			dZ = p->iZ - (pcUserData->sPosition.iZ >> 8);

			iDistance = (dX*dX+dZ*dZ);

			if ( iDistance < 0xC8000 && abs( dX ) < 4096 && abs( dZ ) < 4096 )
			{
				if ( pcUserData->iGameLevel >= GAMELEVEL_Three && (pcMap->uCounter & 0x1FF) == 0x80 )
				{
					sPacket.saItems[sPacket.iCount].bActive = p->bActive;
					sPacket.saItems[sPacket.iCount].iX = p->iX << 8;
					sPacket.saItems[sPacket.iCount].iY = 0;
					sPacket.saItems[sPacket.iCount].iZ = p->iZ << 8;
					sPacket.saItems[sPacket.iCount].iItemID = 0;
					sPacket.saItems[sPacket.iCount].iClass = 0;

					if ( pcMap->pcBaseMap )
						UNITINFODATA->OnDescriptionSpawn( pcMap->pcBaseMap->GetID(), p->iX, p->iZ, sPacket.saItems[sPacket.iCount].szName );

					sPacket.iCount++;
				}
			}

			if ( iDistance < 0x1C2000 && abs( dX ) < 4096 && abs( dZ ) < 4096 )
			{
				pcMap->baSpawnFlagActive[i]++;
			}

			if ( pcMap->baSpawnFlagActive[i] )
				pcMap->iNumActiveSpawnFlag++;
			pcMap->iNumSpawnFlag++;
		}
	}
	if ( sPacket.iCount > 0 && pcUserData->iGameLevel >= GAMELEVEL_Three )
	{
		sPacket.iLength = (sizeof( PacketStageItemData::StageItemData )* sPacket.iCount) + 16;
		sPacket.iHeader = PKTHDR_PlayItem;
		sPacket.bActive = TRUE;
		SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );
	}
}

BOOL MapServer::SetSpawnMonster( Map * pcMap, UnitData * pcUnitData )
{
    if ( pcMap == NULL )
        return FALSE;

    int iFlag = 0, iX = 0, iZ = 0;

    for ( int i = pcMap->iNumActiveSpawns; i < 200; i++ )
    {
        Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

        if ( p->bActive && pcMap->baSpawnFlagActive[i] &&
             pcMap->iaSpawnMonsterActive[i] < pcMap->sSpawnSetting.iSpawnMonsters &&
             pcMap->dwaTimeLastMonsterSpawned[i] < TICKCOUNT )
        {
            iX = p->iX << 8;
            iZ = p->iZ << 8;
            pcMap->iNumActiveSpawns = i + 1;
            pcMap->iLastActiveSpawn = i;
            iFlag++;
            break;
        }
    }

    if ( iFlag == 0 )
    {
        for ( int i = 0; i < 200; i++ )
        {
            Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

            if ( p->bActive && pcMap->baSpawnFlagActive[i] &&
                 pcMap->iaSpawnMonsterActive[i] < pcMap->sSpawnSetting.iSpawnMonsters &&
                 pcMap->dwaTimeLastMonsterSpawned[i] < TICKCOUNT )
            {
                iX = p->iX << 8;
                iZ = p->iZ << 8;
                pcMap->iNumActiveSpawns = i + 1;
                pcMap->iLastActiveSpawn = i;
                iFlag++;
                break;
            }
        }
	}

	if ( iFlag == 0 && pcMap->bForceCreateMonster )
	{
		for ( int i = pcMap->iNumActiveSpawns; i < 200; i++ )
		{
			Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

			if ( p->bActive && pcMap->baSpawnFlagActive[i] )
			{
				iX = p->iX << 8;
				iZ = p->iZ << 8;
				iFlag++;
				break;
			}
		}

		if ( iFlag == 0 && pcMap->iNumActiveSpawns < 200 )
		{
			for ( int i = 0; i < pcMap->iNumActiveSpawns; i++ )
			{
				Map::SpawnFlag * p = pcMap->saSpawnFlag + i;
				if ( p->bActive && pcMap->baSpawnFlagActive[i] )
				{
					iX = p->iX << 8;
					iZ = p->iZ << 8;
					iFlag++;
					break;
				}
			}
		}

		if ( iFlag == 0 )
		{
			for ( int i = 0; i < 200; i++ )
			{
				Map::SpawnFlag * p = pcMap->saSpawnFlag + i;

				if ( p->bActive )
				{
					iX = p->iX << 8;
					iZ = p->iZ << 8;
					iFlag++;
					break;
				}
			}
		}
	}

	if ( iFlag > 0 )
	{
		if ( (*(DWORD*)0x07AC9E24) )
		{
			DWORD p = (*(DWORD*)0x07AC9E24);
			iX = (*(DWORD*)(p));
			iZ = (*(DWORD*)(p + 8));
		}

		__asm
		{
			MOV ECX, pcUnitData;
			MOV EAX, 0x004163E0;
			CALL EAX;
		}
		pcUnitData->pcStage = pcMap->pcStage;

		int iXRand = 0;
		int iZRand = 0;
		iXRand = rand() % (128 << 8);
		iZRand = rand() % (128 << 8);
		iXRand -= 64 << 8;
		iZRand -= 64 << 8;

		iX += iXRand;
		iZ += iZRand;

		__asm
		{
			PUSH 0;
			PUSH 0;
			PUSH 0;
			PUSH iZ;
			PUSH 0;
			PUSH iX;
			MOV ECX, pcUnitData;
			MOV EAX, 0x00416970;
			CALL EAX;
		}

		pcUnitData->sSpawnPosition.iX = iX;
		pcUnitData->sSpawnPosition.iY = pcUnitData->sPosition.iY;
		pcUnitData->sSpawnPosition.iZ = iZ;
		pcUnitData->iLureDistance = (256 * 256);

		if ( pcMap->bForceCreateMonster == FALSE )
		{
			pcUnitData->iSpawnPointIndex = pcMap->iLastActiveSpawn;

			pcMap->dwaTimeLastMonsterSpawned[pcMap->iLastActiveSpawn] = TICKCOUNT + pcMap->sSpawnSetting.iSpawnIntervalMax;
		}
		else
			pcUnitData->iSpawnPointIndex = -1;

		pcMap->bForceCreateMonster = FALSE;

		return TRUE;
	}

	pcMap->bForceCreateMonster = FALSE;

	return FALSE;
}

void * MapServer::FindItemStage( Map * pcMap, int iX, int iY, int iZ )
{
	Map::Loot * ps = NULL;

	for ( int i = 0; i < 1024; i++ )
	{
		Map::Loot * p = pcMap->saLoot + i;
		if ( p->bInUse && p->sPosition.iX == iX && p->sPosition.iY == iY && p->sPosition.iZ == iZ )
		{
			ps = p;
			break;
		}
	}
	return ps;
}

BOOL MapServer::SendItemStageNearUsers( Map * pcMap, Map::Loot * psItemStage )
{
	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );
	sPacket.iLength = sizeof( PacketStageItemData::StageItemData ) + 16;
	sPacket.iHeader = PKTHDR_PlayItem;
	sPacket.bActive = TRUE;

	DWORD dwCode = psItemStage->sLootItem.sItem.sItemID.ToItemID();

	sPacket.saItems[sPacket.iCount].bActive = psItemStage->bInUse;
	sPacket.saItems[sPacket.iCount].iX = psItemStage->sPosition.iX << 8;
	sPacket.saItems[sPacket.iCount].iY = psItemStage->sPosition.iY << 8;
	sPacket.saItems[sPacket.iCount].iZ = psItemStage->sPosition.iZ << 8;
	sPacket.saItems[sPacket.iCount].iItemID = dwCode;
	sPacket.saItems[sPacket.iCount].iClass = ClassFlagToCharacterClass( (EClassFlag)psItemStage->sLootItem.sItem.eSpecialization );

	dwCode &= 0xFFFF0000;

	if ( psItemStage->sLootItem.sItem.iPotionCount > 1 && (dwCode & 0xFF000000) == 0x04000000 )
		STRINGFORMAT( sPacket.saItems[sPacket.iCount].szName, "%sx%d", psItemStage->sLootItem.sItem.szItemName, psItemStage->sLootItem.sItem.iPotionCount );
	else
		STRINGCOPY( sPacket.saItems[sPacket.iCount].szName, psItemStage->sLootItem.sItem.szItemName );

	sPacket.iCount++;

	Point3D sPosition = psItemStage->sPosition;
	sPosition.iX <<= 8;
	sPosition.iY <<= 8;
	sPosition.iZ <<= 8;


	for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
	{
		User * pcUser = USERSERVER->pcaUserInGame[i];

		if ( sPosition.WithinPTDistance( &pcUser->pcUserData->sPosition, 544 ) )
			SENDPACKET( pcUser, &sPacket );
	}

	return TRUE;
}

void MapServer::SendItemStageUser( Map * pcMap, UserData * pcUserData, Map::Loot * psItemStage )
{
	PacketStageItemData sPacket;
	ZeroMemory( &sPacket, sizeof( PacketStageItemData ) );
	sPacket.iLength = sizeof( PacketStageItemData::StageItemData ) + 16;
	sPacket.iHeader = PKTHDR_PlayItem;
	sPacket.bActive = TRUE;

	DWORD dwCode = psItemStage->sLootItem.sItem.sItemID.ToItemID();

	sPacket.saItems[sPacket.iCount].bActive = psItemStage->bInUse;
	sPacket.saItems[sPacket.iCount].iX = psItemStage->sPosition.iX << 8;
	sPacket.saItems[sPacket.iCount].iY = psItemStage->sPosition.iY << 8;
	sPacket.saItems[sPacket.iCount].iZ = psItemStage->sPosition.iZ << 8;
	sPacket.saItems[sPacket.iCount].iItemID = dwCode;
	sPacket.saItems[sPacket.iCount].iClass = ClassFlagToCharacterClass( (EClassFlag)psItemStage->sLootItem.sItem.eSpecialization );

	dwCode &= 0xFFFF0000;

	if ( psItemStage->sLootItem.sItem.iPotionCount > 1 && (dwCode & 0xFF000000) == 0x04000000 )
		STRINGFORMAT( sPacket.saItems[sPacket.iCount].szName, "%sx%d", psItemStage->sLootItem.sItem.szItemName, psItemStage->sLootItem.sItem.iPotionCount );
	else
		STRINGCOPY( sPacket.saItems[sPacket.iCount].szName, psItemStage->sLootItem.sItem.szItemName );

	sPacket.iCount++;

	SENDPACKET( USERDATATOUSER( pcUserData ), &sPacket );
}

BOOL MapServer::OnThrowItem( UserData * pcUserData, struct PacketThrowItem * psPacket )
{
	auto pcUser = USERDATATOUSER( pcUserData );
	if ( pcUser == nullptr )
		return FALSE;

	SENDPACKETBLANK( pcUser, PKTHDR_Save );

	if ( (psPacket->sItem.sItemID.ToItemID() & 0xFFFF0000) == 0x08020000 )
	{
		// XMas Event Crystal?
		if ( psPacket->sItem.sItemID.ToItemID() == ITEMID_SnowManCrystal )
		{
			if ( auto pcUnitData = MAPSERVER->SpawnMonster( "Snowman", pcUserData->iMapID, Point3D( psPacket->sPosition.iX, pcUserData->sPosition.iY, psPacket->sPosition.iZ ), pcUser ) )
			{
				// User Warp Effect summon crystal
				PacketUserWarpEffect sPacketEffect;
				ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
				sPacketEffect.iID = 0x100;
				sPacketEffect.iX = psPacket->sPosition.iX;
				sPacketEffect.iY = psPacket->sPosition.iY;
				sPacketEffect.iZ = psPacket->sPosition.iZ;
				CALL_WITH_ARG2( 0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect );

				ITEMSERVER->OnUseCrystal( pcUser->pcUserData, pcUnitData->iID, ITEMID_SnowManCrystal, pcUnitData->dwLastUpdateTime );
			}

			return TRUE;
		}
	}

	if ( ITEMSERVER->IsDropableItem( psPacket->sItem.sItemID.ToItemID() ) == FALSE )
		return TRUE;

	if ( BATTLEROYALEHANDLER->IsDropableItemUser( pcUser ) == FALSE )
		return TRUE;

	if ( pcUser->bInBattleRoyale )
	{
		if ( BATTLEROYALEHANDLER->DropItemBattle( pcUser, psPacket->sItem ) )
			return TRUE;
	}

	return FALSE;
}

BOOL MapServer::UseUnionCore( UserData * pcUserData, struct PacketUseUnionCore * psPacket )
{
	// Online?
	if ( pcUserData && pcUserData->pcSocketData )
	{
		// Get User
		User * pcUser = USERDATATOUSER( pcUserData );
		if ( pcUser )
		{
			// Item Fake? dont teleport
			if ( !ITEMSERVER->CheckValidItem( &psPacket->sItem ) )
				return FALSE;

			psPacket->sItem.szItemName[31] = 0;
			
			// Get Character name to teleport to him
			int iLen		= lstrlenA( psPacket->sItem.szItemName );			
			char * pszName	= &psPacket->sItem.szItemName[iLen + 1];

			// Don't have name? don't teleport
			if ( pszName[0] == 0 )
				return FALSE;

			// Get Player
			UserData * pcUserDataTarget = USERDATABYNAME( pszName );
			User * pcUserTarget = USERDATATOUSER( pcUserDataTarget );

			if ( !pcUserDataTarget || !pcUserDataTarget->iID || pcUserData->dwExclusiveNum || pcUserDataTarget->dwExclusiveNum ||
				IsBellatraMapArea( pcUserDataTarget->sPosition.iX, pcUserDataTarget->sPosition.iY, pcUserDataTarget->sPosition.iZ ) ||
				pcUserTarget->bInBattleRoyale ||
				FURYARENAHANDLER->InsideArena( &pcUserDataTarget->sPosition ) || // Inside Fury Arena?
				pcUserDataTarget->iMapID == MAPID_QuestArena || pcUserDataTarget->iMapID == MAPID_T5QuestArena || pcUserDataTarget->iMapID == MAPID_BlessCastle ||
				pcUserDataTarget->iMapID == MAPID_OldRuinen2 || pcUserDataTarget->iMapID == MAPID_RoyalDesert )
			{
				CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "You cannot teleport to player" );
				return FALSE;
			}
			// Mismatch clan?
		/*	if ( pcUserData->dwClanID != pcUserDataTarget->dwClanID )
			{
				// Don't teleport
				CHATSERVER->SendChatMessageBox( pcUserData, "You need to be on the same clan" );
				return FALSE;
			}
			*/
			Map * pcMap = MAPSDATA + pcUserDataTarget->iMapID;
			if ( pcMap )
			{
				if ( pcMap->pcBaseMap )
				{
					if ( pcUserData->sCharacterData.iLevel < pcMap->pcBaseMap->iLevel )
					{
						CHATSERVER->SendChatMessageBox( USERDATATOUSER( pcUserData ), "Level does not match" );
						return FALSE;
					}
				
					// Success, Send coordinates to teleport to player
					PacketSendUseUnionCore sPacket;
					sPacket.iLength			= sizeof( PacketSendUseUnionCore );
					sPacket.iHeader			= PKTHDR_UseUnionCore;
					sPacket.iMapID			= pcMap->pcBaseMap->GetID();
					sPacket.iX				= pcUserDataTarget->sPosition.iX + (rand() % 256);
					sPacket.iY				= pcUserDataTarget->sPosition.iY;
					sPacket.iZ				= pcUserDataTarget->sPosition.iZ + (rand() % 256);
					sPacket.dwItemID		= psPacket->sItem.sItemID.ToItemID();
					sPacket.dwItemHead		= psPacket->sItem.iChk1;
					sPacket.dwItemChecksum	= psPacket->sItem.iChk2;
					sPacket.dwUnk			= 0;
					SENDPACKET( USERDATATOUSER(pcUserData), &sPacket, TRUE );

					pcUserData->iMapID = (EMapID)pcMap->pcBaseMap->GetID();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL MapServer::SetBossPosition( Map * pcMap, CharacterData * psCharacterData, Point3D * psPosition )
{
	BOOL bUpdate = FALSE;

	if ( pcMap && pcMap->pcBaseMap && psCharacterData && psPosition )
	{
		int iMapID			= pcMap->pcBaseMap->GetID();

		DWORD dwEffectID	= psCharacterData->iMonsterEffectID;
		
		for ( auto psBossPosition : vBossMapPosition )
		{
			if ( iMapID == psBossPosition->eMapID )
			{
				if ( ((psBossPosition->iEffectID > 0) && (psCharacterData->iMonsterEffectID == psBossPosition->iEffectID)) ||
					((psBossPosition->szName[0] != 0) && STRINGCOMPAREI( psCharacterData->szName, psBossPosition->szName )) )
				{
					psPosition->iX = psBossPosition->sPosition.iX << 8;
					psPosition->iY = 0;
					psPosition->iZ = psBossPosition->sPosition.iZ << 8;

					bUpdate = TRUE;

					break;
				}
			}
		}
	}

	return bUpdate;
}

BOOL MapServer::SpawnMonsterBoss( Map * pcMap, Map::SpawnSetting::Boss * psBoss )
{
	MonsterMapSpawnHandle sCharacter;

	UnitData * pcUnitData = NULL;

	if ( psBoss->cdBoss->iMonsterEffectID == MONSTEREFFECTID_BABEL )
	{
		if ( READDWORD( 0x07AC9D54 ) > TICKCOUNT )
			return FALSE;

		WRITEDWORD( 0x006E2C30, pcMap->pcBaseMap->iMapID );

		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pc = pcMap->pcaUnitData[i];

			if ( pc )
			{
				if ( pc->bActive && pc->GetAnimation() != ANIMATIONTYPE_Die )
				{
					pc->Animate( ANIMATIONTYPE_Flinch );
					pc->iStunTimeLeft = 16 * 8;
				}
			}
		}
	}

	static Point3D sPosition;

	if( psBoss->cdBoss->bExclusiveBoss = SetBossPosition( pcMap, psBoss->cdBoss, &sPosition ) )
		(*(Point3D**)0x07AC9E24 ) = &sPosition;

	if ( psBoss->cdBoss )
	{
		CopyMemory( &sCharacter.sCharacterData, psBoss->cdBoss, sizeof( CharacterData ) );
		pcMap->bForceCreateMonster = TRUE;
		pcUnitData = CreateUnit( pcMap, &sCharacter, 0 );
		pcMap->iNumActiveSpawns = pcMap->iLastActiveSpawn;

		if ( (*(Point3D**)0x07AC9E24) )
		{
			pcUnitData->sSpawnPosition = pcUnitData->sPosition;
		}
	}

	if ( psBoss->cdMinion )
	{
		CopyMemory( &sCharacter.sCharacterData, psBoss->cdMinion, sizeof( CharacterData ) );
		pcMap->bForceCreateMonster = TRUE;
		CreateUnit( pcMap, &sCharacter, psBoss->iMinions );
	}

	if ( (*(Point3D**)0x07AC9E24) )
	{
		(*(Point3D**)0x07AC9E24) = NULL;

		if ( pcUnitData )
		{
			pcUnitData->psaDamageUsersData = new AttackDamageData[ATTACK_DAMAGE_MAX];
			if ( pcUnitData->psaDamageUsersData )
				ZeroMemory( pcUnitData->psaDamageUsersData, sizeof( AttackDamageData ) * ATTACK_DAMAGE_MAX );
		}
	}

	return TRUE;
}

BOOL MapServer::UpdateUnitPosition( UnitData * pcUnitData, int iMapID, int iX, int iY, int iZ )
{
	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE MapNPC SET X=?, Y=?, Z=? WHERE (IDNPC=(SELECT ID FROM NPCList WHERE Name=?)) AND (Stage=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iX );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iY );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iZ );
					pcDB->BindParameterInput( 4, PARAMTYPE_String, pcUnitData->sCharacterData.szName, 32 );
					pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iMapID );

					pcDB->Execute();
				}
				pcDB->Close();
			}
		}

		pcUnitData->sPosition.iX = iX << 8;
		pcUnitData->sPosition.iY = iY << 8;
		pcUnitData->sPosition.iZ = iZ << 8;

		return TRUE;
	}

	return FALSE;
}

BOOL MapServer::UpdateUnitAngle( UnitData * pcUnitData, int iMapID, int iAngle )
{
	if ( pcUnitData )
	{
		if ( pcUnitData->sCharacterData.iType == ECharacterType::CHARACTERTYPE_NPC )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE MapNPC SET Angle=? WHERE (IDNPC=(SELECT ID FROM NPCList WHERE Name=?)) AND (Stage=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iAngle );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUnitData->sCharacterData.szName, 32 );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iMapID );

					pcDB->Execute();
				}
				pcDB->Close();
			}
		}

		pcUnitData->sAngle.iY = iAngle;

		return TRUE;
	}

	return FALSE;
}

UnitData * MapServer::CreateUnit( Map * pcMap, MonsterMapSpawnHandle * pcMonster, int iGroup )
{
	typedef UnitData * (__thiscall *tfnCreateUnit)(Map * pcMap, MonsterMapSpawnHandle * pcMonster, int iGroup);
	tfnCreateUnit fnCreateUnit = (tfnCreateUnit)0x08B8100C;
	return fnCreateUnit( pcMap, pcMonster, iGroup );
}


void MapServer::UpdateMap( Map * pcMap )
{ //0x00566220
	DWORD dwTickCount = TICKCOUNT;

	if ( pcMap->pcStage != NULL )
	{
		pcMap->uCounter++;

		//Spawn a Monster
		if ( (pcMap->uCounter % pcMap->sSpawnSetting.iSpawnIntervalMin) == 0 )
		{
			for ( int i = 0; i < pcMap->iNumActiveSpawnFlag; i++ )
			{
				if ( pcMap->iNumAliveUnitData < pcMap->sSpawnSetting.iSpawnMaxMonsters )
				{
					if ( (pcMap->pcBaseMap->bNightSpawn == FALSE) || (pcMap->bNight == TRUE) )
					{
						if ( pcMap->sSpawnSetting.iTotalMonsters > 0 )
						{
							MonsterMapSpawnHandle sMonster;
							sMonster.sCharacterData.szName[0] = 0;

							int iSpawnRate = 0;
							int iRandomSpawnRate = RandomI( 0, pcMap->sSpawnSetting.iTotalMonsterSpawnRate - 1 );

                            CopyMemory( &sMonster.sCharacterData, pcMap->sSpawnSetting.saMonster[0].cd, sizeof( CharacterData ) );

							for ( UINT j = 0; j < pcMap->sSpawnSetting.iTotalMonsters; j++ )
							{
								iSpawnRate += pcMap->sSpawnSetting.saMonster[j].iSpawnRate;

								if ( iSpawnRate > iRandomSpawnRate )
								{
									CopyMemory( &sMonster.sCharacterData, pcMap->sSpawnSetting.saMonster[j].cd, sizeof( CharacterData ) );

									break;
								}
							}

                            if ( sMonster.sCharacterData.szName[0] != 0 )
                            {
                                UnitInfo * psNewUnitInfo = sMonster.sCharacterData.psUnitInfo;

                                int iSpawnCount = RandomI( psNewUnitInfo->iGroupLimitMin, psNewUnitInfo->iGroupLimitMax );

                                if ( iSpawnCount <= 0 )
                                    iSpawnCount = 1;

                                HALLOWEENHANDLER->OnSpawnEventMonster( &sMonster.sCharacterData );
                                XMASHANDLER->OnSpawnMonsterEvent( &sMonster.sCharacterData, pcMap );
                                EVENTSERVER->OnSpawnMonsterEvent( &sMonster.sCharacterData, pcMap );
                                MONSTERHORDEHANDLER->HandleSpawnMonster( &sMonster.sCharacterData, pcMap );

								pcMap->bForceCreateMonster = FALSE;

                                if ( auto pcUnitData = pcMap->CreateMonster( &sMonster.sCharacterData, iSpawnCount ) )
                                {
                                    FixUnitDataName( pcUnitData );
                                }
                            }
						}
					}
				}
			}
		}

		//Update Spawn Activity based on Users
		switch ( pcMap->uCounter % 512 )
		{
			case 128:
			{
				//Reset Active Spawn
				ZeroMemory( pcMap->baSpawnFlagActive, sizeof( pcMap->baSpawnFlagActive ) );

                pcMap->iNumActiveSpawnFlag = 0;
                pcMap->iNumSpawnFlag = 0;

				//Update Spawns to Active (And Show Spawn Flags)
				UINT uUsersInGame = USERSERVER->uUsersInGame;
				User ** pcaUserInGame = USERSERVER->pcaUserInGame;
				for ( UINT i = 0; i < uUsersInGame; i++ )
				{
					User * pcUser = pcaUserInGame[i];

					if ( pcMap->pcBaseMap )
						if ( (pcMap->pcBaseMap->GetID() == pcUser->GetMapID()) || (pcMap->pcBaseMap->GetID() == pcUser->GetSecondMapID()) )
							SendStageSpawn( pcMap, pcUser->pcUserData );
				}
			}
			break;
			case 192:
			{
				//Show Loot
				UINT uUsersInGame = USERSERVER->uUsersInGame;
				User ** pcaUserInGame = USERSERVER->pcaUserInGame;
				for ( UINT i = 0; i < uUsersInGame; i++ )
				{
					User * pcUser = pcaUserInGame[i];

					if ( (dwTickCount - pcUser->pcUserData->dwTimeLastPacket) < 1500 )
						SendStageItem( pcMap, pcUser->pcUserData );
				}
			}
			break;
		}

		//Remove Timed Out Loot
		for ( UINT i = 0; i < MAX_LOOT; i++ )
		{
			Map::Loot * pLoot = pcMap->saLoot + i;

			if ( (pLoot->bInUse) && (pLoot->dwEndTime < TICKCOUNT) )
			{
				if ( DROPAREAHANDLER->IsExclusiveDrop( pLoot ) == FALSE )
					pLoot->bInUse = FALSE;
				else
					pLoot->dwEndTime = TICKCOUNT + 10000;
			}
		}

		//Bellatra Map Update
		if ( pcMap->pcBaseMap->iMapID == MAPID_Bellatra )
		{
			if ( pcMap->sSpawnSetting.iTotalMonsters > 0 )
			{
				CharacterData sNewCharacterData;
				CopyMemory( &sNewCharacterData, pcMap->sSpawnSetting.saMonster[0].cd, sizeof( CharacterData ) );

				MonsterMapSpawnHandle s;
				CopyMemory( &s.sCharacterData, &sNewCharacterData, sizeof( CharacterData ) );

				BELLATRASERVER->UpdateServer( pcMap, &s );
			}
		}

		//Bless Castle Map Update
		if ( pcMap->pcBaseMap->iMapID == MAPID_BlessCastle )
		{
			//Update BC
			BLESSCASTLEHANDLER->UpdateSiegeWar();
		}

		//Swifts in Map from Night to Day or vice versa
		if ( pcMap->bNight != IsNight() )
		{
			//DayNight swift in maps with Night Spawn on (Navisko Town)
			if ( pcMap->pcBaseMap->bNightSpawn )
			{
				if ( IsNight() )
				{
					//Curse NPCs into Monsters (except Bellatra NPCs)
					CharacterData * psZombie = *(CharacterData **)0x07AC9D5C;

					UINT uUsersInGame = UNITSERVER->uUnitsInGame;
					Unit ** pcaUnitInGame = UNITSERVER->pcaUnitInGame;
					for ( UINT i = 0; i < uUsersInGame; i++ )
					{
						Unit * pcUnit = pcaUnitInGame[i];
						UnitData * pcUnitData = pcUnit->pcUnitData;

						//In Use AND Same Map AND NPC AND Not Bellatra NPC?
						if ( pcUnitData->bActive == TRUE &&
							pcUnitData->pcMap == pcMap &&
							pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC &&
							(pcUnitData->sUnitInfo.iBellatraTypeNPC == 0) && (FOLLOWUNITHANDLER->IsFollowUnitType( pcUnit ) == false) )
						{
							//Reset Unit Data
							pcUnitData->Free();

							//Set in Use again
							pcUnitData->bActive = TRUE;

							//Copy Zombie Data
							CopyMemory( &pcUnitData->sCharacterData, psZombie, sizeof( CharacterData ) );
							CopyMemory( &pcUnitData->sUnitInfo, psZombie->psUnitInfo, sizeof( UnitInfo ) );

							//Set Mode
							pcUnitData->eUpdateMode = UPDATEMODE_ServerMonster;

							//Update Num Alive
							pcMap->iNumAliveUnitData++;

							//Load Model INI
							pcUnitData->LoadModel( pcUnitData->sCharacterData.Monster.szBodyModel );

							//Set Lure Distance
							pcUnitData->iLureDistance = UNIT_DEFAULT_LURE_DISTANCE_FAR;

							//Add Unit Data to Map
							pcMap->AddUnitData( pcUnitData );

							//Set Death Timer
							pcUnitData->iDeathCounter = 256;

							//Animate to Idle
							pcUnitData->Animate( ANIMATIONTYPE_Idle );

							//Make Unit appear as New Load
							pcUnitData->sCharacterData.cNewLoad++;

							//Load Unit
							pcUnit->Load();
						}
					}
				}
				else
				{
					//Kill Alive Monsters
					UINT uUsersInGame = UNITSERVER->uUnitsInGame;
					Unit ** pcaUnitInGame = UNITSERVER->pcaUnitInGame;
					for ( UINT i = 0; i < uUsersInGame; i++ )
					{
						Unit * pcUnit = pcaUnitInGame[i];
						UnitData * pcUnitData = pcUnit->pcUnitData;

						//InUse AND Same Map AND Monster?
						if ( pcUnitData->bActive == TRUE &&
							pcUnitData->pcMap == pcMap &&
							pcUnitData->sCharacterData.iType == CHARACTERTYPE_Monster )
						{
							//Kill Unit
							pcUnitData->Animate( EAnimationType::ANIMATIONTYPE_Die );
							pcUnitData->sCharacterData.sHP.sCur = 0;
						}
					}

					//Spawn NPCs
					for ( UINT i = 0; i < MAX_NPCINMAP; i++ )
					{
						PacketUnitInfo * psNPC = pcMap->saNPC + i;

						if ( (psNPC->iHeader != 0) )
							SpawnNPC( pcMap, psNPC );
					}
				}
			}

			//DayNight swift in Ricarten
			if ( pcMap->pcBaseMap->iMapID == MAPID_RicartenTown )
			{
				CALL_WITH_ARG1( 0x00552060, 100 ); //Removes Wolverines from World

				if ( IsNight() )
				{
					//Switching to Night.. spawn Wolverines
					CALL_WITH_ARG4( 0x0055BB00, 100, 0, 0, 0 );
				}
				else
				{
					//Switching to Day.. spawn Wolverine NPCs
					for ( UINT i = 0; i < MAX_NPCINMAP; i++ )
					{
						PacketUnitInfo * psNPC = pcMap->saNPC + i;

						if ( (psNPC->iHeader != 0) && (psNPC->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_WOLVERLIN) )
							SpawnNPC( pcMap, psNPC );
					}
				}
			}

			//Swift DayNight Time in Map
			pcMap->bNight = IsNight();
		}
	}
}

void MapServer::Update()
{
	//Only GameServers

	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	//Update each Map
	for ( int i = 0; i < 80; i++ )
	{
		Map * pcMap = MAPSDATA + i;

		UpdateMap( pcMap );
	}
	
	//Boss Time Update?
	if ( READWORD( 0x07AC9F54 ) != GetServerTime()->wDay )
	{
		BOSS_TIME = (rand() % 45) + 1;
		WRITEWORD( 0x07AC9F54, GetServerTime()->wDay );

		UpdateUsersBossTime();

		SQLUpdateBossTime( BOSS_TIME, SERVER_CODE, (int)(*(WORD *)0x07AC9F54) );
	}

	//Update Time
	UpdateTime();
}

void MapServer::UpdateTime()
{
	//Update PT time and check if it's night!
	SetTime( GetMinuteOfDay() );
	if ( (GetTime() < 4) || (GetTime() >= 23) )
	{
		//It's night!
		IsNight( TRUE );
	}
	else
	{
		//Not night!
		IsNight( FALSE );
	}
}


void MapServer::SpawnNPC( Map * pcMap, PacketUnitInfo * psPacket )
{
	CALLT_WITH_ARG1( 0x0054E100, (DWORD)pcMap, (DWORD)psPacket );
}

BOOL MapServer::SpawnCrystal( Item * psItem, int iX, int iY, int iZ, UserData * pcUserData )
{
	EItemID iItemID = psItem->sItemID.ToItemID();

	if ( (iItemID == ITEMID_CaravanHopyCrystal) || (iItemID == ITEMID_CaravanArmaCrystal) || (iItemID == ITEMID_CaravanBumaCrystal) || 
		(iItemID == ITEMID_CaravanCuepyCrystal) || (iItemID == ITEMID_CaravanHungkyCrystal) || (iItemID == ITEMID_CaravanRabieCrystal) ||
		 (iItemID == ITEMID_CaravanTobieCrystal) || (iItemID == ITEMID_CaravanRudolphCrystal) || (iItemID == ITEMID_CaravanSirKnightCrystal) || 
		 (iItemID == ITEMID_CaravanPinguCrystal) || (iItemID == ITEMID_CaravanHopyCheapCrystal) || (iItemID == ITEMID_CaravanYetiCrystal) || (iItemID == ITEMID_CaravanGoldenHopyCrystal) )
		return TRUE;

	if ( iItemID == ITEMID_SoulCrystal )
	{
		pcUserData->eSoulCrystalItemID = iItemID;
		pcUserData->dwSoulCrystalTimeOut = TICKCOUNT + (30 * 1000);

		ITEMSERVER->OnUseCrystal( pcUserData, 0x7FFFABBB, iItemID, pcUserData->dwSoulCrystalTimeOut );
		return TRUE;
	}

	struct CrystalData
	{
		EItemID			iItemID;

		int				iChance; //0~99%

		int				iTime;

		char			* pszName;
	};

	static const CrystalData saNormalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 5, "Hopy_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 5, "Hobgoblin_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 5, "Decoy_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Bargon_Crystal" },
		{ ITEMID_HeadCutterCrystal, 12, 5, "Head Cutter_Crystal" },
		{ ITEMID_FigonCrystal, 10, 5, "Figon_Crystal" },
		{ ITEMID_KingHopyCrystal, 8, 5, "King Hopy_Crystal" },
		{ ITEMID_HulkCrystal, 2, 5, "Hulk_Crystal" },
		{ ITEMID_GuardianSaintCrystal, 0, 5, "Guardian Saint_Crystal" },
		{ ITEMID_WebCrystal, 0, 5, "Web_Crystal" },
		{ ITEMID_DarkSpecterCrystal, 0, 5, "Dark Specter_Crystal" },
		{ ITEMID_IronGuardCrystal, 0, 5, "Iron Guard_Crystal" },
		{ ITEMID_RicartenMilitiaGuardCrystal, 0, 5, "Ricarten Militia" },
		{ ITEMID_RicartenGuardCrystal, 0, 5, "Ricarten Guard" },
		{ ITEMID_RoyalBlessGuardCrystal, 0, 5, "Royal Bless Guard" },
		{ ITEMID_ValentoCrystal, 0, 10, "Valento_Crystal" },
		{ ITEMID_KelvezuCrystal, 0, 10, "Kelvezu_Crystal" },
		{ ITEMID_TullaCrystal, 0, 10, "Tulla_Crystal" },
	};

	static const CrystalData saDuskCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 7, "Typhoon_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 7, "Grotesque_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 7, "Stygian_Crystal" },
		{ ITEMID_BargonCrystal, 14, 7, "Dark Specter_Crystal" },
	};

	static const CrystalData saDoomedCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 8, "Dark Phalanx_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 8, "King Spider_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 8, "Seto_Crystal" },
		{ ITEMID_BargonCrystal, 14, 8, "Hell Hound_Crystal" },
	};

	static const CrystalData saOccultCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 9, "Atalanta Spirit_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 9, "Archer Spirit_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 9, "Pikeman Spirit_Crystal" },
		{ ITEMID_BargonCrystal, 14, 9, "Mechanician Spirit_Crystal" },
	};

	static const CrystalData saArticCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 100, 10, "Chaos Cara_Crystal" },
	};

	static const CrystalData saAbysmalCrystalData[] =
	{
		{ ITEMID_HopyCrystal, 20, 10, "Dey_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 10, "Koon_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 10, "Sathla_Crystal" },
	};

	static const CrystalData saBossData[] =
	{
		{ ITEMID_HopyCrystal, 20, 5, "Zzangpy_Crystal" },
		{ ITEMID_HobgoblinCrystal, 18, 5, "Wild Goblin_Crystal" },
		{ ITEMID_DecoyCrystal, 16, 5, "Maximum Decoy_Crystal" },
		{ ITEMID_BargonCrystal, 14, 5, "Bauton_Crystal" },
		{ ITEMID_HeadCutterCrystal, 12, 5, "Vega Demir_Crystal" },
		{ ITEMID_FigonCrystal, 10, 5, "HellSing_Crystal" },
		{ ITEMID_KingHopyCrystal, 8, 5, "Horropion_Crystal" },
		{ ITEMID_HulkCrystal, 2, 5, "Typhoon_Crystal" },
		{ ITEMID_GuardianSaintCrystal, 0, 5, "Guardian King_Crystal" },
		{ ITEMID_WebCrystal, 0, 5, "Cursed Web_Crystal" },
		{ ITEMID_DarkSpecterCrystal, 0, 5, "Dark Phantom_Crystal" },
		{ ITEMID_IronGuardCrystal, 0, 5, "Imitir Guard_Crystal" },
		{ ITEMID_RicartenMilitiaGuardCrystal, 0, 5, "Ricarten Militia" },
		{ ITEMID_RicartenGuardCrystal, 0, 5, "Ricarten Guard" },
		{ ITEMID_RoyalBlessGuardCrystal, 0, 5, "Royal Bless Guard" },
		{ ITEMID_ValentoCrystal, 0, 10, "Valento_Crystal" },
		{ ITEMID_KelvezuCrystal, 0, 10, "Kelvezu_Crystal" },
		{ ITEMID_TullaCrystal, 0, 10, "Tulla_Crystal" },
	};

	BOOL bBossCrystal = (*(BOOL*)0x07AC9D50);

	BOOL bBlessCastleCrystal = FALSE;

	const CrystalData * psCrystal = NULL;

	const CrystalData * psaCrystals = saNormalData;
	int iCount						= _countof( saNormalData );

	if ( bBossCrystal )
	{
		psaCrystals					= saBossData;
		iCount						= _countof( saBossData );
	}

	if ( iItemID == ITEMID_DuskyCrystal )
	{
		psaCrystals					= saDuskCrystalData;
		iCount						= _countof( saDuskCrystalData );
	}

	if ( iItemID == ITEMID_DoomedCrystal )
	{
		psaCrystals					= saDoomedCrystalData;
		iCount						= _countof( saDoomedCrystalData );
	}

	if ( iItemID == ITEMID_OccultCrystal )
	{
		psaCrystals					= saOccultCrystalData;
		iCount						= _countof( saOccultCrystalData );
	}

	if ( iItemID == ITEMID_ArcticCrystal )
	{
		psaCrystals					= saArticCrystalData;
		iCount						= _countof( saArticCrystalData );
	}
	if ( iItemID == ITEMID_AbysmalCrystal )
	{
		psaCrystals					= saAbysmalCrystalData;
		iCount						= _countof( saAbysmalCrystalData );
	}
	if ( iItemID == ITEMID_SoulSummonCrystal )
	{
		CrystalData sData	= { ITEMID_SoulSummonCrystal, 100, 10, psItem->szItemName };

		psaCrystals			= &sData;
		iCount				= 1;
	}

	//Mystic Crystal
	if ( iItemID == ITEMID_MysticCrystal ||
		iItemID == ITEMID_DuskyCrystal ||
		iItemID == ITEMID_DoomedCrystal ||
		iItemID == ITEMID_OccultCrystal ||
		iItemID == ITEMID_ArcticCrystal ||
		iItemID == ITEMID_AbysmalCrystal )
	{
		int iRandom = RandomI( 0, 100 );

		int iRandomCount = 0;

		for ( int i = 0; i < iCount; i++ )
		{
			iRandomCount += psaCrystals[i].iChance;

			if ( iRandom < iRandomCount )
			{
				psCrystal = psaCrystals + i;
				break;
			}
		}

		if ( psCrystal == NULL )
			psCrystal = &psaCrystals[0];
	}

	//Normal Crystal
	if ( psCrystal == NULL )
	{
		for ( int i = 0; i < iCount; i++ )
		{
			if ( psaCrystals[i].iItemID == iItemID )
			{
				psCrystal = psaCrystals + i;
				break;
			}
		}
	}

	if ( psCrystal != NULL )
	{
		Map * pcMap = MAPSDATA + pcUserData->iMapID;
		if ( pcMap )
		{
			if ( pcMap->pcBaseMap->iMapType == MAPTYPE_Town )
				return FALSE;

			//Bless Castle Crystal?
			if ( (psCrystal->iItemID == ITEMID_RicartenGuardCrystal) ||
				(psCrystal->iItemID == ITEMID_RicartenMilitiaGuardCrystal) ||
				(psCrystal->iItemID == ITEMID_RoyalBlessGuardCrystal) )
			{
				if ( (*(BOOL*)0x07AB1EE8) == FALSE || pcUserData->iClanID == 0 || pcUserData->iClanID != (*(int*)0x07AB1EEC) || pcUserData->iMapID != MAPID_BlessCastle )
				{
					CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "You can't summon a Bless Castle guard now!" );

					return FALSE;
				}


				int iaBCGuard[3] = { 0,0,0 };
				int iClanMonster = 0;
				int iSelfMonster = 0;

				for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
				{
					UnitData * pcUnitData = pcMap->pcaUnitData[i];

					if ( pcUnitData && pcUnitData->iClanID == pcUserData->iClanID )
					{
						iClanMonster++;

						if ( pcUnitData->sCharacterData.iOwnerID == pcUserData->iID )
							iSelfMonster++;

						switch ( pcUnitData->sCharacterData.iMonsterEffectID )
						{
							case MONSTEREFFECTID_CASTLE_SOLDER_A:
								iaBCGuard[0]++;
								break;
							case MONSTEREFFECTID_CASTLE_SOLDER_B:
								iaBCGuard[1]++;
								break;
							case MONSTEREFFECTID_CASTLE_SOLDER_C:
								iaBCGuard[2]++;
								break;
						}
					}
				}

				int iBlessCrystalID = 0;

				if ( iItemID == ITEMID_RicartenMilitiaGuardCrystal )
					iBlessCrystalID = 0;
				else if ( iItemID == ITEMID_RicartenGuardCrystal )
					iBlessCrystalID = 1;
				else if ( iItemID == ITEMID_RoyalBlessGuardCrystal )
					iBlessCrystalID = 2;

				if ( iClanMonster >= 20 || iSelfMonster >= 1 || iaBCGuard[iBlessCrystalID] >= 5 )
				{
					CHATSERVER->SendChat( USERDATATOUSER( pcUserData ), CHATCOLOR_Error, "You can't summon a Bless Castle guard now!" );

					return FALSE;
				}

				bBlessCastleCrystal = TRUE;
			}

			//Position
			Point2D * psaPoint = (Point2D*)0x006E2BF0;
			Point3D sNewPosition( 0, 0, 0 );

			int iPos	= 0;
			int iHeight = 0;
			for ( int i = 0; i < 8; i++ )
			{
				int j = RandomI( 0, 8 );

				Point2D * ps = psaPoint + (j & 7);

				sNewPosition.iX = iX + (ps->iX << 8);
				sNewPosition.iY = iY;
				sNewPosition.iZ = iZ + (ps->iY << 8);

				j++;

				iHeight = pcMap->pcStage->GetHighestPoint( sNewPosition.iX, sNewPosition.iY, sNewPosition.iZ, 0x2000 );

				if ( iHeight != 0xFFFF8001 )
				{
					if ( abs( iHeight - sNewPosition.iY ) < 0x2000 )
						break;
				}

				iPos++;
			}

			if ( iPos < 8 )
			{
				iX = sNewPosition.iX;
				iY = iHeight;
				iZ = sNewPosition.iZ;
			}

			WRITEDWORD( 0x08B81830, TRUE );

			UnitData * pcUnitData = MAPSERVER->SpawnMonster( psCrystal->pszName, pcUserData->iMapID, Point3D( iX, iY, iZ ), USERDATATOUSER( pcUserData ) );
			if ( pcUnitData )
			{
				int iLengthName = STRLEN( pcUnitData->GetName() );
				int iLength		= STRLEN( "_Crystal" );

				if ( std::string( pcUnitData->GetName() ).find( "_Crystal" ) != std::string::npos )
					ZeroMemory( &pcUnitData->sCharacterData.szName[iLengthName - iLength], iLength );

				STRINGCOPY( pcUnitData->sCharacterData.Pet.szOwnerName, CHARACTERSERVER->GetCharacterName( pcUserData ) );
				
				pcUnitData->sUnitInfo.dwHashCharacterName	= 0;
				pcUnitData->sCharacterData.iMonsterType		= MONSTERTYPE_Friendly;
				pcUnitData->sCharacterData.iOwnerID			= pcUserData->iID;
				
				pcUnitData->pcOwner							= pcUserData;
				pcUnitData->dwLastUpdateTime				= TICKCOUNT + (psCrystal->iTime * 60 * 1000);

				ITEMSERVER->OnUseCrystal( pcUserData, pcUnitData->iID, iItemID, pcUnitData->dwLastUpdateTime );

				//User Warp Effect summon crystal
				PacketUserWarpEffect sPacketEffect;
				ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
				sPacketEffect.iID	= 0x100;
				sPacketEffect.iX	= iX;
				sPacketEffect.iY	= iY;
				sPacketEffect.iZ	= iZ;
				CALL_WITH_ARG2( 0x00552130, (DWORD)pcUserData, (DWORD)&sPacketEffect );

				return TRUE;
			}
		}
	}

	return FALSE;
}

UnitData * MapServer::SpawnMonsterBC( Map * pcMap, CharacterData * psCharacterData, int iX, int iY, int iZ )
{
	return (UnitData *)CALL_WITH_ARG5( 0x0055CC90, (DWORD)pcMap, (DWORD)psCharacterData, iX, iZ, iY );
}

UnitData * MapServer::SpawnMonster( const char * pszMonsterName, int iMapID, Point3D sPosition, User * pcUser )
{
	if ( (pszMonsterName == nullptr) || (pszMonsterName[0] == 0) )
		return nullptr;

	Map * pcMap = MAPSDATA + iMapID;
	MonsterMapSpawnHandle sMonster;

	sMonster.iPositionState = CHARACTERTYPE_Monster;
	sMonster.sCharacterData.sHP.sCur = 100;
	sMonster.sCharacterData.sMP.sCur = 100;
	sMonster.sCharacterData.sHP.sMax = 100;
	sMonster.sCharacterData.iType = CHARACTERTYPE_Monster;
	if ( auto psCharacterData = UNITSERVER->GetCharacterDataByName( pszMonsterName ) )
	{
		CopyMemory( &sMonster.sCharacterData, psCharacterData, sizeof( CharacterData ) );

		pcMap->bForceCreateMonster = TRUE;

		if ( auto pcUnitData = CreateUnit( pcMap, &sMonster, 0 ) )
		{
			pcUnitData->sPosition = sPosition;
			pcUnitData->iLureDistance = 0;
			pcUnitData->Animate( ANIMATIONTYPE_Idle );
			pcUnitData->iTargetMoveCount = 0;

			if ( pcUser )
			{
				pcUnitData->pcOwner = pcUser->pcUserData;
				pcUnitData->sUnitInfo.dwHashCharacterName = 0;
				pcUnitData->sCharacterData.iMonsterType = MONSTERTYPE_Friendly;
				STRINGCOPY( pcUnitData->sCharacterData.Pet.szOwnerName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
				pcUnitData->sCharacterData.iOwnerID = pcUser->GetID();
				pcUnitData->dwLastUpdateTime = TICKCOUNT + (4 * 60 * 1000);
			}

			return pcUnitData;
		}
	}

	return nullptr;
}

UINT MapServer::GetWorldTimeHour()
{
	return CALL( 0x0054CBC0 );
}

int MapServer::MakeFaceArea( Stage * pcThis, int iX, int iZ, int iWidth, int iHeight, int iTop, int iBottom )
{
	int iFaceCount = 0;

	StageFace ** psaFaceList = (StageFace**)0x08B818F0;

	pcThis->dwSumCount++;
	int iCalcSum = pcThis->dwSumCount;

	int isX = (iX >> (8 + 6)) & 0xFF;
	int isZ = (iZ >> (8 + 6)) & 0xFF;

	int iwX = isX + (iWidth >> (8 + 6));
	int iwZ = isZ + (iHeight >> (8 + 6));

	for ( int cx = isX; cx < iwX; cx++ )
	{
		for ( int cz = isZ; cz < iwZ; cz++ )
		{
			int x = cx & 0xFF;
			int y = cz & 0xFF;

			if ( pcThis->pdwaArea[x][y] )
			{
				int iPos = pcThis->pdwaArea[x][y][0];

				for ( int i = 1; i < (iPos + 1); i++ )
				{
					int FaceNum = pcThis->pdwaArea[x][y][i];
					
					StageFace * psFace = pcThis->pFace + FaceNum;
					if ( psFace->iCalcSum != iCalcSum )
					{
						psFace->iCalcSum = iCalcSum;

						int iY1 = pcThis->pVertex[psFace->wVertex[0]].sPosition.iY;
						int iY2 = pcThis->pVertex[psFace->wVertex[1]].sPosition.iY;
						int iY3 = pcThis->pVertex[psFace->wVertex[2]].sPosition.iY;

						if ( (iY1 < iTop && iY1 > iBottom) || (iY2 < iTop && iY2 > iBottom) || (iY3 < iTop && iY3 > iBottom) )
						{
							psaFaceList[iFaceCount++]	= psFace;
							psaFaceList[iFaceCount]		= NULL;
						}
					}
				}
			}
		}
	}

	WRITEDWORD( 0x0736660C, iFaceCount );

	pcThis->iCalcSum++;

	return iFaceCount;
}

BOOL MapServer::SetupPolygonArea( Stage * pcThis )
{
	//CLEAR AREA
	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 256; j++ )
		{
			pcThis->pdwaArea[j][i] = NULL;
		}
	}

	pcThis->psaAreaList = new Point2D[4096];
	ZeroMemory( pcThis->psaAreaList, sizeof( Point2D ) * 4096 );

	Point3D saVertexP[3];

	for ( int i = 0; i < pcThis->iFaceCount; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			saVertexP[j].iX = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iX >> 8;
			saVertexP[j].iY = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iY >> 8;
			saVertexP[j].iZ = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iZ >> 8;
		}
	}

	//TODO: GET POLY AREAS
	CALLT_WITH_ARG3( 0x00469590, (DWORD)pcThis, (DWORD)&saVertexP[0], (DWORD)&saVertexP[1], (DWORD)&saVertexP[2] );

	if ( pcThis->iAreaCount > 0 )
	{
		for ( int i = 0; i < pcThis->iAreaCount; i++ )
		{
			pcThis->pdwaArea[pcThis->psaAreaList[i].iX][pcThis->psaAreaList[i].iY]++;
		}
	}

	DELETA( pcThis->psaAreaList );

	int iBufferCount = 0;

	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 256; j++ )
		{
			if ( (int)pcThis->pdwaArea[j][i] > 0 )
				iBufferCount += (int)(pcThis->pdwaArea[j][i]) + 1;
		}
	}

	pcThis->iAreaBufferSize = iBufferCount;
	pcThis->pdwAreaBuffer = new DWORD[iBufferCount];

	iBufferCount = 0;

	for ( int i = 0; i < 256; i++ )
	{
		for ( int j = 0; j < 256; j++ )
		{
			if ( (int)pcThis->pdwaArea[j][i] > 0 )
			{
				pcThis->pdwaArea[j][i] = pcThis->pdwAreaBuffer + iBufferCount;
				pcThis->pdwaArea[j][i][0] = 0;

				iBufferCount += (int)(pcThis->pdwaArea[j][i]) + 1;
			}
		}
	}

	pcThis->psaAreaList = new Point2D[4096];
	ZeroMemory( pcThis->psaAreaList, sizeof( Point2D ) * 4096 );

	for ( int i = 0; i < pcThis->iFaceCount; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			saVertexP[j].iX = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iX >> 8;
			saVertexP[j].iY = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iY >> 8;
			saVertexP[j].iZ = pcThis->pVertex[pcThis->pFace[i].wVertex[j]].sPosition.iZ >> 8;
		}

		//TODO: GET POLY AREAS
		CALLT_WITH_ARG3( 0x00469590, (DWORD)pcThis, (DWORD)&saVertexP[0], (DWORD)&saVertexP[1], (DWORD)&saVertexP[2] );


		if ( pcThis->iAreaCount > 0 )
		{
			for ( int j = 0; j < pcThis->iAreaCount; j++ )
			{
				if ( (pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY]) && ((int)pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY] != 4) )
				{
					pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY][0]++;
					int iPos = pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY][0];

					pcThis->pdwaArea[pcThis->psaAreaList[j].iX][pcThis->psaAreaList[j].iY][iPos] = i;
				}
			}
		}
	}

	DELETA( pcThis->psaAreaList );

	return TRUE;
}

int MapServer::GetHeightArea( Stage * pcThis, int iAX, int iAZ, int iX, int iZ )
{
	int iHeight = 0;

	int cx = iAX & 0xFF;
	int cz = iAZ & 0xFF;

	int iCalcSum = pcThis->iCalcSum;

	if ( pcThis->pdwaArea[cx][cz] )
	{
		int iPos = pcThis->pdwaArea[cx][cz][0];

		for ( int i = 1; i < (iPos + 1); i++ )
		{
			int iFaceNum = pcThis->pdwaArea[cx][cz][i];

			StageFace * psFace = pcThis->pFace + iFaceNum;

			if ( psFace->iCalcSum != iCalcSum /* MESH */ )
			{
				psFace->iCalcSum = iCalcSum;

				//TODO: GETPOLYHEIGHT
				int iH = 0;

				if ( iH != (-32767) && iH > iHeight )
					iHeight = iH;
			}
		}
	}

	return iHeight;
}

void MapServer::UpdateUserBossTime( User * pcUser, int iMapID )
{
	if ( (iMapID >= 0) && (iMapID < NUM_MAPS) )
	{
		Map * pcMap = MAPSDATA + iMapID;
		if ( pcMap )
		{
			SYSTEMTIME sBossTime;
			CopyMemory( &sBossTime, GetServerTime(), sizeof( SYSTEMTIME ) );

			DWORD dwLessTimeBoss = 0;
			char * pszBossName = NULL;

			//Get Boss Time
			for ( int i = 0; i < pcMap->sSpawnSetting.iTotalBosses; i++ )
			{
				for ( int j = 0; j < pcMap->sSpawnSetting.saBoss[i].iBossHours; j++ )
				{
					//Get Boss Time
					sBossTime.wHour		= pcMap->sSpawnSetting.saBoss[i].caBossHour[j];
					sBossTime.wMinute	= BOSS_TIME;
					sBossTime.wSecond	= 0;

					//After the Time?
					if ( sBossTime.wMinute >= GetServerTime()->wMinute )
					{
						//If Boss was spawned at the current boss time on loop, go to the next day...
						if ( sBossTime.wHour < GetServerTime()->wHour )
							sBossTime.wDay++;
					}

					UINT64 uBossTime	= SystemTimeToQWORD( &sBossTime );
					UINT64 uServerTime	= SystemTimeToQWORD( GetServerTime() );
					if ( uBossTime >= uServerTime )
					{
						DWORD dwDifference = (DWORD)(uBossTime - uServerTime);
						if ( (dwLessTimeBoss > dwDifference) || (dwLessTimeBoss == 0) )
						{
							pszBossName		= pcMap->sSpawnSetting.saBoss[i].cdBoss->szName;
							dwLessTimeBoss	= dwDifference;
						}
					}

					//Backup Time
					CopyMemory( &sBossTime, GetServerTime(), sizeof( SYSTEMTIME ) );
				}
			}

			PacketBossTimeUpdate sPacket;
			ZeroMemory( &sPacket, sizeof( PacketBossTimeUpdate ) );
			sPacket.iLength = sizeof( PacketBossTimeUpdate );
			sPacket.iHeader = PKTHDR_BossTimeUpdate;

			if ( pszBossName )
			{
				STRINGCOPY( sPacket.szBossName, pszBossName );
				sPacket.dwTime = dwLessTimeBoss;
			}

			SENDPACKET( pcUser, &sPacket );
		}
	}
}

void MapServer::UpdateUsersBossTime()
{
	//Update Users about Boss Time
	UINT uUsersInGame = USERSERVER->uUsersInGame;
	User ** pcaUserInGame = USERSERVER->pcaUserInGame;
	for ( UINT i = 0; i < uUsersInGame; i++ )
	{
		User * pcUser = pcaUserInGame[i];
		UpdateUserBossTime( pcUser, pcUser->GetMapID() );
	}
}

void MapServer::HandlePacket( User * pcUser, PacketBossTimeUpdate * psPacket )
{
	if ( (psPacket->iMapID >= 0) && (psPacket->iMapID < NUM_MAPS) )
		UpdateUserBossTime( pcUser, psPacket->iMapID );
}

NAKED int MapServer::GetAngle( int iX, int iZ, int iTargetX, int iTargetZ )
{
	JMP( pfnGetAngle );
}

void MapServer::SQLUpdateBossTime( int iMinute, int iServerID, int iDay, BOOL bGameMaster )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_ServerDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO BossTime([Minute],[Day],[ServerID],[IsGameMasterCommand],[Date]) VALUES(?,?,?,?,GETDATE())" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iMinute );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iDay );
			pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iServerID );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &bGameMaster );

			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void MapServer::SQLLoadMapBossPosition()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [Name], [MonsterEffectName], [MapID], [MapX], [MapZ] FROM MapBossPosition WHERE Enabled=1" ) )
		{
			pcDB->Execute();
			
			while ( pcDB->Fetch() )
			{
				char szMonsterEffectName[32] = {0};

				MapBossPositionData * psBossPosition = new MapBossPositionData();
				psBossPosition->sPosition.iY = 0;
				psBossPosition->iEffectID = 0;

				pcDB->GetData( 1, PARAMTYPE_String, psBossPosition->szName, _countof( psBossPosition->szName ) );
				pcDB->GetData( 2, PARAMTYPE_String, szMonsterEffectName, _countof( szMonsterEffectName ) );
				pcDB->GetData( 3, PARAMTYPE_Integer, &psBossPosition->eMapID );
				pcDB->GetData( 4, PARAMTYPE_Integer, &psBossPosition->sPosition.iX );
				pcDB->GetData( 5, PARAMTYPE_Integer, &psBossPosition->sPosition.iZ );

				if ( szMonsterEffectName[0] != 0 )
				{
					for ( auto & sEffect : saCharacterSoundEffect )
					{
						if ( STRINGCOMPAREI( sEffect.szCodeName, szMonsterEffectName ) )
						{
							psBossPosition->iEffectID = sEffect.iMonsterEffectID;
							break;
						}
					}
				}

				vBossMapPosition.push_back( psBossPosition );
			}
		}
		pcDB->Close();
	}
}

bool MapServer::IsTownMap( int iMapID )
{
	for ( int i = 0; i < _countof( aStageCity ); i++ )
	{
		if ( iMapID == aStageCity[i] )
			return true;
	}

	return false;
}
