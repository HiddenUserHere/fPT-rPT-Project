#include "stdafx.h"
#include "CMonsterHordeHandler.h"


CMonsterHordeHandler::CMonsterHordeHandler()
{
}


CMonsterHordeHandler::~CMonsterHordeHandler()
{
	for ( auto & p : vInstances )
		DELET( p );

	vInstances.clear();
}

void CMonsterHordeHandler::Init()
{
	LoadHorde();
}

void CMonsterHordeHandler::LoadHorde( int iHordeID )
{
	if ( LOGIN_SERVER )
		return;

	if ( iHordeID >= 0 )
	{
		for ( std::vector<CHordeInstance*>::iterator it = vInstances.begin(); it != vInstances.end(); )
		{
			CHordeInstance * pcInstance = (*it);
			if ( pcInstance->GetID() == iHordeID )
			{
				DELET( pcInstance );
				it = vInstances.erase( it );
			}
			else
				it++;
		}

		CHordeInstance * pcInstance = new CHordeInstance();
		if ( SQLLoadHorde( iHordeID, pcInstance ) )
			vInstances.push_back( pcInstance );
		else
			DELET( pcInstance );
	}
	else
	{
		for ( auto & pcInstance : vInstances )
		{
			DELET( pcInstance );
		}

		vInstances.clear();

		std::vector<int> vHordes = SQLGetHordeList();
		for ( auto & i : vHordes )
		{
			CHordeInstance * pcInstance = new CHordeInstance();
			if ( SQLLoadHorde( i, pcInstance ) )
				vInstances.push_back( pcInstance );
			else
				DELET( pcInstance );
		}

		vHordes.clear();
	}
}

BOOL CMonsterHordeHandler::HandleSpawnMonster( CharacterData * psCharacterData, Map * pcMap )
{
	BOOL bRet = FALSE;

	for ( auto & pcInstance : vInstances )
	{
		if ( pcInstance->IsActive() )
		{
			if ( pcInstance->IsHour( GetServerTime()->wHour ) )
			{
				for ( auto eMapID : pcInstance->GetMapIDs() )
				{
					if ( eMapID == pcMap->pcBaseMap->iMapID )
					{
						int iTotal = 0;
						int iChance = RandomI( 0, 1000 );
						for ( const auto pcMonster : pcInstance->GetMonsters() )
						{
							if ( iChance < (pcMonster->GetChance() + iTotal) )
							{
								CharacterData * psCharacterDataMonster = &pcMonster->GetCharacterData();
								if ( psCharacterDataMonster )
								{
									LARGE_INTEGER liEXP;
									liEXP.QuadPart = pcInstance->GetUseEXPMap() ? UNITSERVER->GetExp( psCharacterData ) : UNITSERVER->GetExp( psCharacterDataMonster );

									int iLevel = pcInstance->GetUseEXPMap() ? psCharacterData->iLevel : psCharacterDataMonster->iLevel;

									static UnitInfo sUnitInfo;
									CopyMemory( &sUnitInfo, psCharacterDataMonster->psUnitInfo, sizeof( UnitInfo ) );

									if ( pcInstance->GetUseDropMap() )
									{
										CopyMemory( &sUnitInfo.saDropData, &psCharacterData->psUnitInfo->saDropData, sizeof( DropSettingsData ) * _countof( sUnitInfo.saDropData ) );
										CopyMemory( &sUnitInfo.saDropDataCustom, &psCharacterData->psUnitInfo->saDropDataCustom, sizeof( DropSettingsData ) * _countof( sUnitInfo.saDropDataCustom ) );

										sUnitInfo.iDropCount = psCharacterData->psUnitInfo->iDropCount;
										sUnitInfo.iDropPercentNum = psCharacterData->psUnitInfo->iDropPercentNum;
										sUnitInfo.iDropPlusCount = psCharacterData->psUnitInfo->iDropPlusCount;
										sUnitInfo.iNumDrops = psCharacterData->psUnitInfo->iNumDrops;
									}

									for ( const auto & drop : pcInstance->GetExtraDrops() )
									{
										if ( eMapID == drop.first )
										{
											sUnitInfo.iNumDrops += drop.second;
											break;
										}
									}

									CopyMemory( psCharacterData, psCharacterDataMonster, sizeof( CharacterData ) );

									psCharacterData->iLevel = iLevel;
									psCharacterData->psUnitInfo = &sUnitInfo;

									if ( pcInstance->GetEXPMultiplier() > 0.0f )
									{
										double dEXP = (double)liEXP.QuadPart;
										dEXP *= (double)pcInstance->GetEXPMultiplier();

										liEXP.QuadPart = (INT64)dEXP;

										psCharacterData->iCurrentExpLow = liEXP.LowPart;
										psCharacterData->iCurrentExpHigh = liEXP.HighPart;
									}

									int iNameLength = STRLEN( psCharacterData->szName );

									STRINGCOPY( psCharacterData->szName, FixUnitDataName( psCharacterData->szName ).c_str() );

									bRet = TRUE;
									break;
								}
							}

							iTotal += pcMonster->GetChance();
						}

						if ( bRet )
							break;
					}
				}

				if ( bRet )
					break;
			}
		}
	}

	return bRet;
}

void CMonsterHordeHandler::Update()
{
	if ( LOGIN_SERVER )
		return;

	if ( wLastHour != GetServerTime()->wHour )
	{
		UpdateStatus( FALSE );

		wLastHour = GetServerTime()->wHour;
	}
}

CHordeInstance * CMonsterHordeHandler::GetInstance( int iID )
{
	for ( auto & pc : vInstances )
		if ( pc->GetID() == iID )
			return pc;

	return nullptr;
}

bool CMonsterHordeHandler::DisableInstance( int iID )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE HordeList SET Enabled=0 WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
			pcDB->Execute();
		}

		pcDB->Close();
	}

	for ( std::vector<CHordeInstance*>::iterator it = vInstances.begin(); it != vInstances.end(); )
	{
		CHordeInstance * pc = (*it);
		if ( pc->GetID() == iID )
		{
			DELET( pc );
			it = vInstances.erase( it );

			bRet = true;
		}
		else
			it++;
	}

	return bRet;
}

void CMonsterHordeHandler::UpdateStatus( BOOL bKillUnits )
{
	for ( auto & pcInstance : vInstances )
	{
		if ( pcInstance->IsActive() )
		{
			if ( pcInstance->IsHour( GetServerTime()->wHour ) == false )
			{
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Horde> %s event ended!", pcInstance->GetName().c_str() );

				pcInstance->SetActive( FALSE );

				if ( bKillUnits )
				{
					for ( auto eMapID : pcInstance->GetMapIDs() )
						KillUnits( eMapID );
				}
			}
		}
		else
		{
			if ( pcInstance->IsHour( GetServerTime()->wHour ) )
			{
				CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Horde> %s event started!", pcInstance->GetName().c_str() );

				pcInstance->SetActive( TRUE );
			}
		}
	}
}

bool CMonsterHordeHandler::SQLLoadHorde( int iHordeID, CHordeInstance * pcInstance )
{
	bool bRet = false;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		BOOL bLoaded = FALSE;

		if ( pcDB->Prepare( "SELECT Name, MapIDs, ExtraDrops, Hours, UseEXPMap, EXPMultiplier, UseDropMap FROM HordeList WHERE (ID=?) AND (Enabled=1)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iHordeID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				char szName[32]		= { 0 };
				char szHours[64]	= { 0 };
				char szMapIDs[128]		= {0};
				char szExtraDrops[128]	= {0};
				BOOL bUseEXPMap		= FALSE;
				BOOL bUseDropMap	= FALSE;
				float fEXPMultipler = 0.0f;

				pcDB->GetData( 1, PARAMTYPE_String, szName, _countof( szName ) );
				pcDB->GetData( 2, PARAMTYPE_String, &szMapIDs, _countof( szMapIDs ) );
				pcDB->GetData( 3, PARAMTYPE_String, &szExtraDrops, _countof( szExtraDrops ) );
				pcDB->GetData( 4, PARAMTYPE_String, szHours, _countof( szHours ) );
				pcDB->GetData( 5, PARAMTYPE_Integer, &bUseEXPMap );
				pcDB->GetData( 6, PARAMTYPE_Float, &fEXPMultipler );
				pcDB->GetData( 7, PARAMTYPE_Integer, &bUseDropMap );

				pcInstance->Init( iHordeID, szName );
				pcInstance->SetUseEXPMap( bUseEXPMap );
				pcInstance->SetUseDropMap( bUseDropMap );
				pcInstance->SetEXPMultiplier( fEXPMultipler );

				std::vector<std::string> vHours = split( szHours, ' ' );
				for ( const auto & v : vHours )
					pcInstance->AddHour( atoi( v.c_str() ) );

				std::vector<std::string> vMapIDs = split( szMapIDs, ' ' );
				std::vector<std::string> vExtraDrops = split( szExtraDrops, ' ' );
				for ( size_t i = 0; i < vMapIDs.size(); i++ )
				{
					pcInstance->AddMapID( (EMapID)atoi( vMapIDs[i].c_str() ) );

					if ( vMapIDs.size() == vExtraDrops.size() )
						pcInstance->AddExtraDrop( (EMapID)atoi( vMapIDs[i].c_str() ), atoi( vExtraDrops[i].c_str() ) );
				}

				bLoaded = TRUE;
			}
		}

		if ( bLoaded )
		{
			pcDB->Clear();

			if ( pcDB->Prepare( "SELECT Name, Chance, MonsterID, UseCustomStats, IsPercentageStats, Level, HP, HPMultiplier, AttackPowerMin, AttackPowerMax, SpecialSkillType, SpecialSkillHit, SpecialSkillHitRate, SpecialSkillHitPowerMin, SpecialSkillHitPowerMax, SpecialSkillHitScope, Absorb, Block, Defense, Organic, Lightning, Ice, Fire, Poison, Magic FROM MonsterHorde WHERE HordeID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iHordeID );
				if ( pcDB->Execute() )
				{
					while ( pcDB->Fetch() )
					{
						int iChance = 0;
						BOOL bIsPercentage = FALSE;
						BOOL bUseCustomStats = FALSE;

						int iMonsterID = 0;

						auto pcMonster = new CMonsterHorde();

						pcDB->GetData( 1, PARAMTYPE_String, pcMonster->GetCharacterData().szName, _countof( pcMonster->GetCharacterData().szName ) );
						pcDB->GetData( 2, PARAMTYPE_Integer, &iChance );
						pcDB->GetData( 3, PARAMTYPE_Integer, &iMonsterID );
						pcDB->GetData( 4, PARAMTYPE_Integer, &bUseCustomStats );
						pcDB->GetData( 5, PARAMTYPE_Integer, &bIsPercentage );

						pcMonster->SetChance( iChance );
						pcMonster->SetPercentageStats( bIsPercentage );

						CharacterData * psCharacterDataMonster = UNITSERVER->GetCharacterDataByName( pcMonster->GetCharacterData().szName );
						if ( psCharacterDataMonster )
						{
							CopyMemory( &pcMonster->GetCharacterData(), psCharacterDataMonster, sizeof( CharacterData ) );
							CopyMemory( &pcMonster->GetUnitInfo(), psCharacterDataMonster->psUnitInfo, sizeof( UnitInfo ) );

							pcMonster->GetCharacterData().psUnitInfo = &pcMonster->GetUnitInfo();

							if ( iMonsterID > 0 )
								pcMonster->GetUnitInfo().iRank = iMonsterID;

							if ( bUseCustomStats )
							{
								int iValue = 0;
								short sValue = 0;
								float fValue = 0.0f;

								pcDB->GetData( 6, PARAMTYPE_Integer, &iValue );
								if ( iValue > 0 )
									pcMonster->GetCharacterData().iLevel = iValue;
								
								pcDB->GetData( 7, PARAMTYPE_Short, &sValue );
								if ( sValue > 0 )
									pcMonster->GetCharacterData().sHP.sCur = pcMonster->GetCharacterData().sHP.sMax = sValue;

								pcDB->GetData( 8, PARAMTYPE_Short, &sValue );
								if ( sValue > 0 )
									pcMonster->GetCharacterData().sHPBlocks.sCur = pcMonster->GetCharacterData().sHPBlocks.sMax = sValue;

								pcDB->GetData( 9, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().iMinDamage = bIsPercentage ? (int)((float)pcMonster->GetCharacterData().iMinDamage * fValue) : (int)fValue;

								pcDB->GetData( 10, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().iMaxDamage = bIsPercentage ? (int)((float)pcMonster->GetCharacterData().iMaxDamage * fValue) : (int)fValue;

								pcDB->GetData( 11, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetUnitInfo().iSkillType = bIsPercentage ? (int)((float)pcMonster->GetUnitInfo().iSkillType * fValue) : (int)fValue;

								pcDB->GetData( 12, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetUnitInfo().iSkillPierceRange = bIsPercentage ? (int)((float)pcMonster->GetUnitInfo().iSkillPierceRange * fValue) : (int)fValue;

								pcDB->GetData( 13, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetUnitInfo().iSkillChance = bIsPercentage ? (int)((float)pcMonster->GetUnitInfo().iSkillChance * fValue) : (int)fValue;

								pcDB->GetData( 14, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetUnitInfo().sSkillPower.sMin = bIsPercentage ? (short)((float)pcMonster->GetUnitInfo().sSkillPower.sMin * fValue) : (short)fValue;

								pcDB->GetData( 15, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetUnitInfo().sSkillPower.sMax = bIsPercentage ? (short)((float)pcMonster->GetUnitInfo().sSkillPower.sMax * fValue) : (short)fValue;

								pcDB->GetData( 16, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetUnitInfo().iSkillArea = bIsPercentage ? (int)((float)pcMonster->GetUnitInfo().iSkillArea * fValue) : (int)fValue;

								pcDB->GetData( 17, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().iAbsorbRating = bIsPercentage ? (int)((float)pcMonster->GetCharacterData().iAbsorbRating * fValue) : (int)fValue;

								pcDB->GetData( 18, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().iBlockRating = bIsPercentage ? (int)((float)pcMonster->GetCharacterData().iBlockRating * fValue) : (int)fValue;

								pcDB->GetData( 19, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().iDefenseRating = bIsPercentage ? (int)((float)pcMonster->GetCharacterData().iDefenseRating * fValue) : (int)fValue;

								pcDB->GetData( 20, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Organic] = bIsPercentage ? (short)((float)pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Organic] * fValue) : (short)fValue;

								pcDB->GetData( 21, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Lighting] = bIsPercentage ? (short)((float)pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Lighting] * fValue) : (short)fValue;

								pcDB->GetData( 22, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Ice] = bIsPercentage ? (short)((float)pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Ice] * fValue) : (short)fValue;

								pcDB->GetData( 23, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Fire] = bIsPercentage ? (short)((float)pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Fire] * fValue) : (short)fValue;

								pcDB->GetData( 24, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Poison] = bIsPercentage ? (short)((float)pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Poison] * fValue) : (short)fValue;

								pcDB->GetData( 25, PARAMTYPE_Float, &fValue );
								if ( fValue != 0.0f )
									pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Wind] = bIsPercentage ? (short)((float)pcMonster->GetCharacterData().sElementalDef[ELEMENTID_Wind] * fValue) : (short)fValue;
							}

							pcInstance->AddMonster( pcMonster );

							bRet = true;
						}
						else
							DELET( pcMonster );
					}
				}
			}
		}

		pcDB->Close();
	}

	return bRet;
}

std::vector<int> CMonsterHordeHandler::SQLGetHordeList()
{
	std::vector<int> v;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM HordeList WHERE Enabled=1" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iID = -1;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );

					v.push_back( iID );
				}
			}
		}

		pcDB->Close();
	}

	return v;
}

void CMonsterHordeHandler::KillUnits( int iMapID )
{
	Map * pcMap = MAPSDATA + iMapID;
	if ( pcMap )
	{
		for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
		{
			UnitData * pcUnitData = pcMap->pcaUnitData[i];

			// Is Unit Alive?
			if ( pcUnitData && pcUnitData->sCharacterData.sHP.sCur > 0 )
			{
				// Kill Unit
				pcUnitData->sCharacterData.sHP.sCur = pcUnitData->sCharacterData.sHP.sMax = 0;

				pcUnitData->Animate( ANIMATIONTYPE_Die );
			}
		}
	}
}
