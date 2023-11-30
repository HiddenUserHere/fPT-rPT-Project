#include "stdafx.h"
#include "QuestArenaHandler.h"


CQuestArenaHandler::CQuestArenaHandler()
{
}


CQuestArenaHandler::~CQuestArenaHandler()
{
}

void CQuestArenaHandler::UpdateQuestT5Unit()
{
    Map * pcMap = MAPSDATA + MAPID_T5QuestArena;

    if ( pcMap == NULL )
        return;


}

void CQuestArenaHandler::UpdateQuestT5Users()
{
    for ( int i = 0; i < PLAYERS_MAX; i++ )
    {
        User * pcUser = USERDATATOUSER( &USERSDATA[i] );

        if ( pcUser && pcUser->IsConnected() && pcUser->IsInGame() )
        {
            //Oasis Drop while 18h & 23h...
            if ( (pcUser->uQuestT5ID == QUESTID_CryMeARiver) && (pcUser->GetMapID() == MAPID_Oasis) && ((MAPSERVER->GetWorldTimeHour() >= 18) || (MAPSERVER->GetWorldTimeHour() <= 4)) )
            {
                if ( pcUser->sQuestT5ProgressValue[0] < 1 )
                {
                    if ( pcUser->dwTimeT5QuestArenaSpawnUpdate < TICKCOUNT )
                    {
                        Map::Loot::LootItem sLootItem;

                        //Get Item Data
                        DefinitionItem * ps = ITEMSERVER->FindItemDefByCode( ITEMID_DiamondTears );

                        if ( ps )
                        {
                            //Create Item with non-spec
                            ITEMSERVER->CreateItem( &sLootItem.sItem, ps, 0 );

                            const Point3D & s = saT5QuestCrymeARiver[rand() % 10];

                            //Get Position of Unit on Map
                            int iX = s.iX;
                            int iY = s.iY;
                            int iZ = s.iZ;

                            //Get Map Data from Unit
                            Map * psMap = MAPSDATA + MAPID_Oasis;

                            if ( psMap )
                            {
                                //Add Item to Map
                                Map::Loot * psItemD = psMap->AddItem( &sLootItem, iX, iY, iZ, TRUE );

                                //Added? Set Exclusive to User...
                                if ( psItemD )
                                {
                                    psItemD->lExclusiveOwnerID    = pcUser->GetID();
                                    psItemD->bExclusive            = TRUE;
                                    psItemD->dwEndTime            = TICKCOUNT + (10 * 60 * 1000);

                                    CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Oh the death of my boy" );
                                    CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Oh the death of my girls" );
                                    CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> Oh the death of my husband" );
                                    CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> One day you all shall pay" );
                                    CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> So now I cry until the world gets filled with water" );
                                    CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> And then you willl all drown in my sorrow" );

                                    pcUser->dwTimeT5QuestArenaSpawnUpdate = TICKCOUNT + (10 * 60 * 1000);
                                }
                            }
                        }
                    }
                }
            }


            //On Quest?
            if ( pcUser->bInT5QuestArena )
            {
				//Starting...
				if ( pcUser->iRoundT5QuestArena == 0 )
				{
					if ( pcUser->dwTimeT5QuestArenaUpdate < TICKCOUNT )
					{ 
						pcUser->iRoundT5QuestArena               = 1;
						pcUser->dwTimeT5QuestArenaUpdate		 = TICKCOUNT + QUESTT5_TIME_ROUND;
						pcUser->dwTimeT5QuestArenaSpawnUpdate    = TICKCOUNT + QUESTT5_TIME_SPAWN;

						SpawnUnitCrystalT5Quest( pcUser );

						//Spawn Units
						SpawnUnitsUserT5Quest( pcUser );

						//Process
						PacketQuestArenaTier5 s;
						s.iType			= PacketQuestArenaTier5::EType::TYPEID_Round;
						s.iRound		= pcUser->iRoundT5QuestArena;
						s.dwTimeLeft	= (QUESTT5_TIME_ROUND / 1000) * QUESTT5_MAXROUND;
						ProcessPacket( pcUser, &s );
					}
				}
				else
				{
					//Spawn Update?
					if ( pcUser->dwTimeT5QuestArenaSpawnUpdate < TICKCOUNT )
					{
						//Spawn Units
						SpawnUnitsUserT5Quest( pcUser );

						pcUser->dwTimeT5QuestArenaSpawnUpdate = TICKCOUNT + QUESTT5_TIME_SPAWN;
					}

					//TimeOut round?
					if ( pcUser->dwTimeT5QuestArenaUpdate < TICKCOUNT )
					{
						//Check Next Round
						if ( pcUser->iRoundT5QuestArena < QUESTT5_MAXROUND )
						{
							//Kill All Units of this User
							KillUnitsUserT5Quest( pcUser );

							//Next Round and Update Time
							pcUser->iRoundT5QuestArena++;

							//Spawn Next Monsters round
							SpawnUnitsUserT5Quest( pcUser );

							pcUser->dwTimeT5QuestArenaUpdate		= TICKCOUNT + QUESTT5_TIME_ROUND;
							pcUser->dwTimeT5QuestArenaSpawnUpdate	= TICKCOUNT + QUESTT5_TIME_SPAWN;

							//Process
							PacketQuestArenaTier5 s;
							s.iType			= PacketQuestArenaTier5::EType::TYPEID_Round;
							s.iRound		= pcUser->iRoundT5QuestArena;
							s.dwTimeLeft	= (QUESTT5_TIME_ROUND / 1000) * QUESTT5_MAXROUND;
							ProcessPacket( pcUser, &s );
						}
						else
						{
							//Finished quest
							CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, QUESTT5ARENA_X_LEAVE, QUESTT5ARENA_Z_LEAVE, MAPID_Atlantis );

							//Kill All Units of this User
							KillUnitsUserT5Quest( pcUser, TRUE );

							pcUser->sQuestT5ProgressValue[0]++;

							//Process
							PacketQuestArenaTier5 s;
							s.iType			= PacketQuestArenaTier5::EType::TYPEID_FinishQuest;
							s.iRound		= pcUser->iRoundT5QuestArena;
							s.dwTimeLeft	= 0;
							ProcessPacket( pcUser, &s );

							pcUser->bInT5QuestArena = FALSE;
						}
					}
				}
            }
        }
    }
}

void CQuestArenaHandler::KillUnitsCrystalT5Quest( UnitData * pcUnitData )
{
	Map * pcMap = MAPSDATA + MAPID_T5QuestArena;

	if ( pcMap == NULL )
		return;

	for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
	{
		UnitData * pc = pcMap->pcaUnitData[i];

		if ( pc && (pc != pcUnitData) )
		{
			//Alive?
			if ( pc->sCharacterData.sHP.sCur > 0 )
			{
				//Is User Target?
				if ( pc->lExclusiveTargetID == pcUnitData->lExclusiveTargetID )
				{
					//Kill
					pc->sCharacterData.sHP.sCur = 0;
					pc->Animate( ANIMATIONTYPE_Die );
				}
			}
		}
	}
}

void CQuestArenaHandler::KillUnitsUserT5Quest( User * pcUser, BOOL bDisconnected )
{
    Map * pcMap = MAPSDATA + MAPID_T5QuestArena;

    if ( pcMap == NULL )
        return;

    for ( int i = 0; i < MAX_ALIVEMONSTERS; i++ )
    {
        UnitData * pcUnitData = pcMap->pcaUnitData[i];

        if ( pcUnitData )
        {
            //Alive?
            if ( pcUnitData->sCharacterData.sHP.sCur > 0 )
            {
                //Is User Target?
                if ( pcUnitData->lExclusiveTargetID == pcUser->GetID() )
                {
					if ( bDisconnected == FALSE && pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
						continue;

                    //Kill
                    pcUnitData->sCharacterData.sHP.sCur = 0;
                    pcUnitData->Animate( ANIMATIONTYPE_Die );
                }
            }
        }
    }
}

void CQuestArenaHandler::SpawnUnitsUserT5Quest( User * pcUser )
{
    Map * pcMap = MAPSDATA + MAPID_T5QuestArena;

    if ( pcMap == NULL )
        return;

    for ( int i = 0; i < QUESTT5_MAXPOSITIONSPAWN; i++ )
    {
        Point3D sPosition                            = saT5QuestArenaSpawnPosition[i];

        sPosition.iX <<= 8;
        sPosition.iZ <<= 8;

        const MonsterDataT5QuestArena * psMonster    = saMonstersT5QuestArena + (pcUser->iRoundT5QuestArena-1);

        UnitData * pcUnitData = MAPSERVER->SpawnMonster( psMonster->pszName, MAPID_T5QuestArena, Point3D( sPosition.iX, sPosition.iY, sPosition.iZ ) );
        if ( pcUnitData )
        {
            pcUnitData->sCharacterData.iMinDamage    = psMonster->sDamage.sMin;
            pcUnitData->sCharacterData.iMaxDamage    = psMonster->sDamage.sMax;
            pcUnitData->sCharacterData.sHP.sMax        = psMonster->iHP;

            //Exclusive ID
            pcUnitData->lExclusiveTargetID            = pcUser->GetID();

            //New HP based on level...
            {
                int iLevel = pcUser->pcUserData->sCharacterData.iLevel;

                //x2 110~114
                if ( (iLevel >= 110) && (iLevel <= 114) )
                    pcUnitData->sCharacterData.sHP.sMax *= 2;

                //x3 115~119
                else if ( (iLevel >= 115) && (iLevel <= 119) )
                    pcUnitData->sCharacterData.sHP.sMax *= 3;

                //x4 120~124
                else if ( (iLevel >= 120) && (iLevel <= 124) )
                    pcUnitData->sCharacterData.sHP.sMax *= 4;

                //x5 125+
                else if ( iLevel >= 125 )
                    pcUnitData->sCharacterData.sHP.sMax *= 5;

                //Update Current HP
                pcUnitData->sCharacterData.sHP.sCur = pcUnitData->sCharacterData.sHP.sMax;
            }
        }
    }
}

void CQuestArenaHandler::SpawnUnitCrystalT5Quest( User * pcUser )
{
    Point3D sPosition;

	sPosition.iX = QUESTT5ARENA_X_;
	sPosition.iY = (8 << 8);
	sPosition.iZ = QUESTT5ARENA_Z_;

    UnitData * pcUnitData = MAPSERVER->SpawnMonster( "Crystal Tower", MAPID_T5QuestArena, Point3D( sPosition.iX, sPosition.iY, sPosition.iZ + 1 ), pcUser );
    if ( pcUnitData )
    {
        pcUnitData->sCharacterData.iMinDamage       = 0;
        pcUnitData->sCharacterData.iMaxDamage       = 0;
        pcUnitData->sCharacterData.iAbsorbRating    = 0;
        pcUnitData->sCharacterData.sHP.sCur			= 30000;
        pcUnitData->sCharacterData.sHP.sMax			= 30000;

        pcUnitData->iSpecialType					= 2;
        pcUnitData->iMovementSpeed					= 0;
        pcUnitData->sUnitInfo.iIntelligence			= 0;
        pcUnitData->sCharacterData.iMovementSpeed	= 0;
        pcUnitData->iAliveCounter					= 256;


        pcUnitData->dwLastUpdateTime                = (QUESTT5_TIME_ROUND * QUESTT5_MAXROUND) + TICKCOUNT + 3000;

        pcUnitData->lExclusiveTargetID                = pcUser->GetID();
    }
}

BOOL CQuestArenaHandler::IsExclusiveUnit( UnitData * pcUnitData, UserData * pcUserData )
{
    //Tier 3?
    if ( pcUnitData->lExclusiveTargetID != pcUserData->iID )
    {
        if ( pcUnitData->sCharacterData.iMonsterType != MONSTERTYPE_Friendly || pcUnitData->pcOwner != pcUserData )
            return TRUE;
    }

    return FALSE;
}

BOOL CQuestArenaHandler::IsCrystalTower( UnitData * pcUnitData )
{


    return FALSE;
}

BOOL CQuestArenaHandler::IsUpdatedUnit( UnitData * pcUnitData )
{
    Map * pcMap = MAPSDATA + MAPID_T5QuestArena;

    if ( pcMap == NULL )
        return FALSE;

    BOOL bRet = FALSE;

    if ( pcUnitData->lExclusiveTargetID )
    {
        //Set Target for Pet
        for ( int i = 0; i < pcMap->iNumUsingUnitData; i++ )
        {
            UnitData * ps = pcMap->pcaUnitData[i];

            //Alive and not the same Unit?
            if ( ps && ps != pcUnitData && ps->sCharacterData.sHP.sCur > 0 )
            {
                if ( ps->pcOwner && ps->pcOwner->iMapID == MAPID_T5QuestArena && ps->pcOwner->iID == pcUnitData->lExclusiveTargetID )
                {
                    //Update Target
                    pcUnitData->pcFocusTarget            = NULL;
                    pcUnitData->pcAttackTarget            = ps;
                    pcUnitData->dwHookedTargetEndTime    = 0;

                    bRet = TRUE;
                    break;
                }
            }
        }
    }

    return bRet;
}

BOOL CQuestArenaHandler::EnterArenaT5Quest( User * pcUser )
{
    if ( pcUser->bInT5QuestArena == FALSE )
    {
        pcUser->iRoundT5QuestArena					= 0;
        pcUser->dwTimeT5QuestArenaUpdate			= TICKCOUNT + QUESTT5_TIME_START;
        pcUser->dwTimeT5QuestArenaSpawnUpdate		= TICKCOUNT + QUESTT5_TIME_SPAWN;

        pcUser->bInT5QuestArena						= TRUE;

		CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, QUESTT5ARENA_X_, QUESTT5ARENA_Z_, MAPID_T5QuestArena );

		CHATSERVER->SendChatMessageBox( pcUser, "The battle will start in 20 seconds!" );
		CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> The battle will start in 20 seconds!" );

		//Process
		PacketQuestArenaTier5 s;
		s.iType			= PacketQuestArenaTier5::EType::TYPEID_Round;
		s.iRound		= pcUser->iRoundT5QuestArena;
		s.dwTimeLeft	= QUESTT5_TIME_START / 1000;
		ProcessPacket( pcUser, &s );
	}

    return FALSE;
}

void CQuestArenaHandler::HandlePacket( User * pcUser, PacketQuestArenaTier5 * psPacket )
{
    switch ( psPacket->iType )
    {
        case PacketQuestArenaTier5::EType::TYPEID_EnterArena:
			EnterArenaT5Quest( pcUser );
            break;

        default:
            break;
    }
}

void CQuestArenaHandler::HandlePacket( User * pcUser, PacketSkillDamageRangeEx * psPacket )
{
	if ( pcUser->GetMapID() != MAPID_T5QuestArena )
		return;

	if ( (pcUser->bInT5QuestArena == FALSE) || (pcUser->uQuestT5ID != QUESTID_T5QuestArena) )
		return;

	Map * pcMap = MAPSDATA + MAPID_T5QuestArena;

	if ( pcMap == NULL )
		return;

	if ( psPacket->iCount > 128 )
		return;

	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		for ( int j = 0; j < MAX_ALIVEMONSTERS; j++ )
		{
			UnitData * pcUnitData = pcMap->pcaUnitData[j];

			if ( pcUnitData )
			{
				//Alive?
				if ( pcUnitData->sCharacterData.sHP.sCur > 0 )
				{
					if ( psPacket->iaTargetID[i] == pcUnitData->iID )
					{
						//Is Not User Target?
						if ( pcUnitData->lExclusiveTargetID != pcUser->GetID() )
						{
							psPacket->iaTargetID[i] = -1;
						}
					}
				}
			}
		}
	}
}

void CQuestArenaHandler::ProcessPacket( User * pcUser, PacketQuestArenaTier5 * psPacket )
{
	psPacket->iLength = sizeof( PacketQuestArenaTier5 );
	psPacket->iHeader = PKTHDR_T5QuestArena;
	SENDPACKET( pcUser, psPacket );
}

void CQuestArenaHandler::Update()
{
    if ( GAME_SERVER )
    {
        UpdateQuestT5Users();
    }

	if ( LOGIN_SERVER )
		return;

	//Prince Spawn
	if ( ((GetServerTime()->wMinute % 20) == 0) && (GetServerTime()->wMinute != wLastMinute) )
	{
		UnitData * pcUnitData = MAPSERVER->SpawnMonster( "Prince", MAPID_MysteryDesert1, Point3D( -8604734, 273473, 862343 ) );

		if ( pcUnitData )
		{
			CHATSERVER->SendChatAllUsersInStage( MAPID_MysteryDesert1, CHATCOLOR_Global, "Boss> Prince Spawned!" );

			if ( pcUnitData->psaDamageUsersData == NULL )
			{
				pcUnitData->psaDamageUsersData = new AttackDamageData[ATTACK_DAMAGE_MAX];

				if ( pcUnitData->psaDamageUsersData )
					ZeroMemory( pcUnitData->psaDamageUsersData, sizeof( AttackDamageData ) * ATTACK_DAMAGE_MAX );
			}
		}
		wLastMinute = GetServerTime()->wMinute;
	}
}

BOOL CQuestArenaHandler::OnUnitKilled( UserData * pcUserData, UnitData * pcUnitData, BOOL bDamageUsers )
{
	User * pcUser = USERDATATOUSER( pcUserData );

	if ( pcUser->uQuestT5ID == QUESTID_KingHopyQuest )
	{
		if ( STRINGCOMPARE( pcUnitData->GetName(), "Undead King Hopy" ) )
		{
			if ( pcUser->sQuestT5ProgressValue[0] == 0 )
			{
				int iChance = rand() % 100;
				if ( iChance < 10 )
				{
					//Send Item to User
					ITEMSERVER->CreateItem( pcUserData, ITEMID_KingHopyHorn, 1 );

					pcUser->sQuestT5ProgressValue[0]++;
				}
			}
		}
	}
    else if ( pcUser->uQuestT5ID == QUESTID_AMatterOfStrenght )
    {
        if ( STRINGCOMPARE( pcUnitData->GetName(), "Bloody Knight" ) )
        {
           if ( pcUser->sQuestT5ProgressValue[0] == 0 )
            {
			   int iChance = rand() % 100;			  
			   if ( iChance < 10 )
			   {
				   //Send Item to User
				   ITEMSERVER->CreateItem( pcUserData, ITEMID_ValentoEye, 1 );

				   pcUser->sQuestT5ProgressValue[0]++;
			   }
            }
        }
        else if ( pcUnitData->psaDamageUsersData && bDamageUsers )
		{
			if ( STRINGCOMPARE( pcUnitData->GetName(), "Prince" ) )
			{
				if ( pcUnitData->psaDamageUsersData )
				{
					for ( int i = 0; i < PLAYERS_MAX; i++ )
					{
						UserData * ud = USERSDATA + i;
						pcUser = USERDATATOUSER( ud );

						if ( pcUser->IsConnected() && pcUser->IsInGame() && UNITSERVER->GetUserInDamageList( pcUnitData, pcUser ) )
						{
							int iX = (ud->sPosition.iX - pcUnitData->sPosition.iX) >> 8;
							int iZ = (ud->sPosition.iX - pcUnitData->sPosition.iX) >> 8;
							iX *= iX;
							iZ *= iZ;
							int iDistance = iX + iZ;

							if ( iDistance < DISTANCE_UNIT_FAR )
							{
								if ( pcUser->sQuestT5ProgressValue[1] == 0 )
								{
									//Send Item to User
									ITEMSERVER->CreateItem( ud, ITEMID_PrinceEye, 1 );

									pcUser->sQuestT5ProgressValue[1]++;
								}
							}
						}
					}
				}
			}
        }
    }
	else if ( pcUser->uQuestT5ID == QUESTID_T5QuestArena )
	{
		if ( pcUser->GetMapID() == MAPID_T5QuestArena )
		{
			if ( pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
			{
				pcUser->bInT5QuestArena = FALSE;

				CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, QUESTT5ARENA_X_LEAVE, QUESTT5ARENA_Z_LEAVE, MAPID_Atlantis );

				CHATSERVER->SendChatMessageBox( pcUser, "You Failed on Tier 5 Quest!" );
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> You Failed on Tier 5 Quest!" );


				KillUnitsUserT5Quest( pcUser );
			}
		}
	}

    return FALSE;
}

BOOL CQuestArenaHandler::OnUnitKilled( UnitData * pcUnitData )
{
	//Killed by an User?
	if ( pcUnitData->pcKillerData && pcUnitData->iFrameTimer > 128 && !pcUnitData->pcOwner )
		return FALSE;

	if ( pcUnitData->lExclusiveTargetID && pcUnitData->sCharacterData.iMonsterEffectID == MONSTEREFFECTID_CrystalTower )
	{
		User * pcUser = USERDATATOUSER( USERDATABYID( pcUnitData->lExclusiveTargetID ) );

		if ( pcUser && pcUser->IsInGame() && pcUser->bInT5QuestArena )
		{
			pcUser->bInT5QuestArena = FALSE;

			CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, QUESTT5ARENA_X_LEAVE, QUESTT5ARENA_Z_LEAVE, MAPID_Atlantis );

			CHATSERVER->SendChatMessageBox( pcUser, "You Failed on Tier 5 Quest!" );
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Global, "> You Failed on Tier 5 Quest!" );

			//Process
			PacketQuestArenaTier5 s1;
			s1.iType			= PacketQuestArenaTier5::EType::TYPEID_Lost;
			s1.iRound			= pcUser->iRoundT5QuestArena;
			s1.dwTimeLeft		= 0;
			ProcessPacket( pcUser, &s1 );

			KillUnitsUserT5Quest( pcUser );
		}
	}

	return TRUE;
}

BOOL CQuestArenaHandler::OnDisconnected( User * pcUser )
{
	if ( pcUser->bInT5QuestArena )
	{
		KillUnitsUserT5Quest( pcUser, TRUE );
	}

	return TRUE;
}

BOOL CQuestArenaHandler::OnReconnect( User * pcUser )
{
	if ( pcUser->bInT5QuestArena )
	{
		pcUser->bInT5QuestArena = FALSE;

		CHARACTERSERVER->SetPositionWarp( pcUser->pcUserData, QUESTT5ARENA_X_LEAVE, QUESTT5ARENA_Z_LEAVE, MAPID_Atlantis );
	}
	return TRUE;
}
