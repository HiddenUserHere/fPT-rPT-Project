#include "stdafx.h"
#include "AgeHandler.h"


CAgeHandler::CAgeHandler()
{
}


CAgeHandler::~CAgeHandler()
{
}

void CAgeHandler::SQLItemBroke( User * pcUser, Item * psItem )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "INSERT INTO AgingRecovery([AccountName],[CharacterID],[Date],[ItemName],[ItemData],[AgeNumber],[Code1],[Code2],[DateRecovered]) VALUES (?,?,GETDATE(),?,?,?,?,?,NULL)" ) )
		{
			int iAge = (int)psItem->sAgeLevel;
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, psItem->szItemName, STRLEN( psItem->szItemName ) );
			pcDB->BindParameterInput( 4, PARAMTYPE_Binary, psItem, sizeof( Item ) );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iAge );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &psItem->iChk1 );
			pcDB->BindParameterInput( 7, PARAMTYPE_Integer, &psItem->iChk2 );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

void CAgeHandler::SQLItemBrokeRecovered( User * pcUser, int iID )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "UPDATE AgingRecovery SET DateRecovered=GETDATE() WHERE (AccountName=?) AND (ID=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iID );

			pcDB->Execute();
		}

		pcDB->Close();
	}
}

BOOL CAgeHandler::GetItemDataBroke( User * pcUser, Item * psItem, int iID )
{
	BOOL bRet = FALSE;

	char szCode[128] = { 0 };

	if ( pcUser && pcUser->pcUserData && psItem )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT [Code1],[Code2],[ItemData] FROM AgingRecovery WHERE (AccountName=?) AND (ID=?) AND (DateRecovered IS NULL)" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
				pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iID );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					ZeroMemory( psItem, sizeof( Item ) );

					int iCode1 = 0, iCode2 = 0;

					pcDB->GetData( 1, PARAMTYPE_Integer, &iCode1 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iCode2 );
					pcDB->GetData( 3, PARAMTYPE_Binary, psItem, sizeof( Item ) );

					STRINGFORMAT( szCode, "%d@%d", iCode1, iCode2 );
					bRet = TRUE;
				}
			}

			pcDB->Close();
		}
	}
	return bRet;
}

std::vector<AgingRecoveryDataHandle> CAgeHandler::SQLGetBrokeItems( char * pszAccountName )
{
	std::vector<AgingRecoveryDataHandle> vData;
	
	AgingRecoveryDataHandle sData;
	
	ZeroMemory( &sData, sizeof( AgingRecoveryDataHandle ) );

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );

	Item sItem;

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT [ID],[ItemName],[AgeNumber],[ItemData],[Date] FROM AgingRecovery WHERE (AccountName=?) AND (DateRecovered IS NULL) ORDER BY ID DESC" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pszAccountName, STRLEN( pszAccountName ) );

			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					ZeroMemory( &sData, sizeof( AgingRecoveryDataHandle ) );

					pcDB->GetData( 1, PARAMTYPE_Integer, &sData.iID );
					pcDB->GetData( 2, PARAMTYPE_String, sData.szItemName, _countof( sData.szItemName ) );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sData.iAgeNumber );
					pcDB->GetData( 4, PARAMTYPE_Binary, &sItem, sizeof( Item ) );
					pcDB->GetData( 5, PARAMTYPE_Time, &sData.sDate );

					sData.iPrice = Base::Coin::GetAgingRecoveryPrice( &sItem );

					vData.push_back( sData );
				}
			}
		}

		pcDB->Close();
	}

	return vData;
}

void CAgeHandler::DownDamage( Item * psItem, int iDown, int iLevel )
{
	if ( iLevel < 10 )
	{
		psItem->sDamage.sMin -= (short)iDown;
		psItem->sDamage.sMax -= (short)iDown;
	}
	else
	{
		int iDec = 2;
		if ( iLevel >= 21 )
			iDec = 3;

		switch ( iDown )
		{
			case 1:
				psItem->sDamage.sMin -= iDec;
				psItem->sDamage.sMax -= iDec;
				break;
			case 2:
				if ( iLevel == 10 )
				{
					psItem->sDamage.sMin -= 2 + 1;
					psItem->sDamage.sMax -= 2 + 1;
				}
				else
				{
					psItem->sDamage.sMin -= iDec * 2;
					psItem->sDamage.sMax -= iDec * 2;
				}
				break;
			case 3:
				if ( iLevel == 10 )
				{
					psItem->sDamage.sMin -= 2 + 1 + 1;
					psItem->sDamage.sMax -= 2 + 1 + 1;
				}
				else if ( iLevel == 11 )
				{
					psItem->sDamage.sMin -= (2 * 2) + 1;
					psItem->sDamage.sMax -= (2 * 2) + 1;
				}
				else
				{
					psItem->sDamage.sMin -= iDec * 3;
					psItem->sDamage.sMax -= iDec * 3;
				}
				break;

			default:
				break;
		}
	}
}

void CAgeHandler::DownDefense( Item * psItem, int iDown, int iPercent )
{
	for ( int i = 0; i < iDown; i++ )
	{
		float f = (100.0f * (float)psItem->iDefenseRating) / (100.0f + (float)iPercent);
		psItem->iDefenseRating = (int)round( f );
	}
}

void CAgeHandler::DownAttackRating( Item * psItem, int iDown, int iNum )
{
	psItem->iAttackRating -= iNum * iDown;
}

void CAgeHandler::DownAbsorbRating( Item * psItem, int iDown, int iLevel )
{
	auto GetAgeAbsorb = []( Item * psItem, int iLevel ) -> float
	{
		float fPercent = 0.4f;
		switch ( psItem->sItemID.ToItemType() )
		{
			case ITEMTYPE_Shield:
				fPercent = 0.4f;
				if ( iLevel >= 21 )
					fPercent = 1.2f;
				else if ( iLevel >= 9 )
					fPercent = 0.8f;
				break;

			case ITEMTYPE_Orb:
			case ITEMTYPE_Robe:
			case ITEMTYPE_Armor:
				fPercent = 0.5f;
				if ( iLevel >= 21 )
					fPercent = 1.5f;
				else if ( iLevel >= 9 )
					fPercent = 1.0f;
				break;
		}

		return fPercent;
	};

	for ( int i = 0; i < iDown; i++ )
		psItem->fAbsorbRating -= GetAgeAbsorb( psItem, iLevel - i ) * (float)iDown;
}

void CAgeHandler::DownCritical( Item * psItem, int iDown )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
		psItem->iCritical -= 1;

	if ( iDown > 2 )
	{
		if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
			psItem->iCritical -= 1;
	}
}

void CAgeHandler::DownBlockRating( Item * psItem, int iDown )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
		psItem->fBlockRating -= 1.0f;

	if ( iDown > 2 )
	{
		if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
			psItem->fBlockRating -= 1.0f;
	}
}

void CAgeHandler::UpDamage( Item * psItem )
{
	int iAdd = 1;
	if ( psItem->sAgeLevel >= 20 )
		iAdd = 3;
	else if ( psItem->sAgeLevel >= 9 )
		iAdd = 2;

	psItem->sDamage.sMin += iAdd;
	psItem->sDamage.sMax += iAdd;
}

void CAgeHandler::UpDefense( Item * psItem, int iPercent )
{
	float f = (float)psItem->iDefenseRating / (100.0f / (float)iPercent);
	psItem->iDefenseRating += (int)round( f );
}

void CAgeHandler::UpAttackRating( Item * psItem, int iNum )
{
	psItem->iAttackRating += iNum;
}

void CAgeHandler::UpAbsorbRating( Item * psItem )
{
	auto GetAgeAbsorb = []( Item * psItem, int iLevel ) -> float
	{
		float fPercent = 0.4f;
		switch ( psItem->sItemID.ToItemType() )
		{
			case ITEMTYPE_Shield:
				fPercent = 0.4f;
				if ( iLevel >= 21 )
					fPercent = 1.2f;
				else if ( iLevel >= 9 )
					fPercent = 0.8f;
				break;

			case ITEMTYPE_Orb:
			case ITEMTYPE_Robe:
			case ITEMTYPE_Armor:
				fPercent = 0.5f;
				if ( iLevel >= 21 )
					fPercent = 1.5f;
				else if ( iLevel >= 9 )
					fPercent = 1.0f;
				break;
		}

		return fPercent;
	};

	psItem->fAbsorbRating += GetAgeAbsorb( psItem, psItem->sAgeLevel + 1 );
}

void CAgeHandler::UpCritical( Item * psItem )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
		psItem->iCritical++;
}

void CAgeHandler::UpBlockRating( Item * psItem )
{
	if ( psItem->sAgeLevel && (psItem->sAgeLevel % 2) == 1 )
		psItem->fBlockRating += 1.0f;
}

BOOL CAgeHandler::OnDownAge( Item * psItem, int iCount )
{
	int iAgeLevel = (int)psItem->sAgeLevel;

	int iDown = iCount + 1;

	int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );

	int iDownResult = 1;

	if ( iLevelAge == 4 )
		iDownResult = 0;

	psItem->sAgeLevel -= (short)iDown;

	bool bDecStatus = true;

	if ( psItem->bCostumeItem )
		bDecStatus = false;

	auto pcItemData = ITEMSERVER->FindItemPointerTable( psItem->sItemID.ToInt() );
	if ( pcItemData )
		if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Costume )
			bDecStatus = false;

	if ( bDecStatus )
	{
		switch ( psItem->sItemID.ToItemType() )
		{
			case ITEMTYPE_Axe:
				DownDamage( psItem, iDown, iAgeLevel );
				DownAttackRating( psItem, iDown, 10 );
				break;

			case ITEMTYPE_Hammer:
				DownDamage( psItem, iDown, iAgeLevel );
				DownAttackRating( psItem, iDown, 8 );
				DownCritical( psItem, iDown );
				break;

			case ITEMTYPE_Scythe:
			case ITEMTYPE_Dagger:
			case ITEMTYPE_Sword:
			case ITEMTYPE_Claw:
				DownDamage( psItem, iDown, iAgeLevel );
				DownAttackRating( psItem, iDown, 5 );
				DownCritical( psItem, iDown );
				break;

			case ITEMTYPE_Phantom:
			case ITEMTYPE_Wand:
				DownDamage( psItem, iDown, iAgeLevel );
				DownAttackRating( psItem, iDown, 8 );
				DownCritical( psItem, iDown );
				psItem->fAddMP -= 10.0f * (float)iDown;
				break;

			case ITEMTYPE_Bow:
			case ITEMTYPE_Javelin:
				DownDamage( psItem, iDown, iAgeLevel );
				DownCritical( psItem, iDown );
				break;

			case ITEMTYPE_Shield:
				DownDefense( psItem, iDown, 5 );
				DownBlockRating( psItem, iDown );
				DownAbsorbRating( psItem, iDown, iAgeLevel );
				break;

			case ITEMTYPE_Orb:
				DownDefense( psItem, iDown, 6 );
				DownAbsorbRating( psItem, iDown, iAgeLevel );
				break;

			case ITEMTYPE_Robe:
			case ITEMTYPE_Armor:
				DownDefense( psItem, iDown, 5 );
				DownAbsorbRating( psItem, iDown, iAgeLevel );
				break;

			default:
				break;
		}
	}

	psItem->bMaturing = FALSE;
	psItem->sMatureBar.sCur = 0;
	psItem->sMatureBar.sMax = 0;

	if ( iLevelAge > 0 )
	{
		if ( iLevelAge == 4 )
		{
			psItem->iLevel -= (iAgeLevel / 4) - (psItem->sAgeLevel / 4);
		}
		if ( iLevelAge == 3 )
		{
			psItem->iLevel -= (iAgeLevel / 3) - (psItem->sAgeLevel / 3);
		}
		else
		{
			if ( (psItem->sAgeLevel % iLevelAge) == iDownResult )
				psItem->iLevel -= 1;

			if ( iDown > 2 )
			{
				if ( (psItem->sAgeLevel % iLevelAge) == iDownResult )
					psItem->iLevel -= 1;
			}
		}
	}

	return TRUE;
}

BOOL CAgeHandler::OnDownAgeHandler( Item * psItem, int iCount )
{
	if ( iCount > 0 )
	{
		for ( int i = 0; i < iCount; i++ )
			OnDownAge( psItem, 0 );
	}
	
	OnDownAge( psItem, 0 );

	ITEMSERVER->ValidateItem( psItem );

	return TRUE;
}

BOOL CAgeHandler::OnUpAge( Item * psItem )
{
	bool bAddStatus = true;

	auto pcItemData = ITEMSERVER->FindItemPointerTable( psItem->sItemID.ToInt() );

	if ( psItem->bCostumeItem )
		bAddStatus = false;

	if ( pcItemData )
		if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Costume )
			bAddStatus = false;

	if ( bAddStatus )
	{
		switch ( psItem->sItemID.ToItemType() )
		{
			case ITEMTYPE_Axe:
				UpDamage( psItem );
				UpAttackRating( psItem, 10 );
				break;

			case ITEMTYPE_Hammer:
				UpDamage( psItem );
				UpAttackRating( psItem, 8 );
				UpCritical( psItem );
				break;

			case ITEMTYPE_Scythe:
			case ITEMTYPE_Dagger:
			case ITEMTYPE_Sword:
			case ITEMTYPE_Claw:
				UpDamage( psItem );
				UpAttackRating( psItem, 5 );
				UpCritical( psItem );
				break;

			case ITEMTYPE_Phantom:
			case ITEMTYPE_Wand:
				UpDamage( psItem );
				UpAttackRating( psItem, 8 );
				UpCritical( psItem );
				psItem->fAddMP += 10;
				break;

			case ITEMTYPE_Bow:
			case ITEMTYPE_Javelin:
				UpDamage( psItem );
				UpCritical( psItem );
				break;

			case ITEMTYPE_Shield:
				UpDefense( psItem, 5 );
				UpBlockRating( psItem );
				UpAbsorbRating( psItem );
				break;

			case ITEMTYPE_Orb:
				UpDefense( psItem, 6 );
				UpAbsorbRating( psItem );
				break;

			case ITEMTYPE_Robe:
			case ITEMTYPE_Armor:
				UpDefense( psItem, 5 );
				UpAbsorbRating( psItem );
				break;

			default:
				break;
		}
	}

	psItem->bMaturing = FALSE;
	psItem->sMatureBar.sCur = 0;
	psItem->sMatureBar.sMax = 0;

	psItem->sAgeLevel++;


	int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );
	if ( iLevelAge > 0 )
	{
		if ( (psItem->sAgeLevel % iLevelAge) == 0 )
			psItem->iLevel++;
	}

	return TRUE;
}

BOOL CAgeHandler::OnUpAgeHandler( Item * psItem, int iCount )
{
	for ( int i = 0; i < iCount; i++ )
		OnUpAge( psItem );

	ITEMSERVER->ValidateItem( psItem );

	return TRUE;
}

void CAgeHandler::OnBrokeItemHandler( User * pcUser, Item * psItem )
{
	if ( pcUser && psItem )
	{
		ITEMSERVER->SaveItemData( psItem );

		SQLItemBroke( pcUser, psItem );
	}
}

int CAgeHandler::AgingChanceHandler( User * pcUser )
{
	auto IncreaseAgeFunction = [this]( User * pcUser, int iAgingLevel, int & iResult, bool bIncreaseTwo ) -> void
	{
		//Increase by one
		iResult = 3;

		//Up aging (going)
		iAgingLevel++;

		//Increase by two? Up aging (going)
		if ( bIncreaseTwo )
		{
			int iRandIncrease = RandomI( 1, 100 );
			if ( (iRandIncrease < 5) && (iAgingLevel < 19) )
			{
				iResult = 4;

				iAgingLevel++;
			}
		}

		//Going to 18+? Send announcement to users
		if ( iAgingLevel >= 18 )
			CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s aged %s to +%d successfully! Congratz!", pcUser->pcUserData->sCharacterData.szName, GetItem()->szItemName, iAgingLevel );

		//Update item integrity
		ITEMSERVER->UpdateIntegrity( GetItem(), -1 );
	};


	//Save item before aging proccess...
	ITEMSERVER->SaveItemData( GetItem() );

	//-1 = Broken, 0 = Just Fail, 1 = decrease by one, 2 = decrease by two, 3 = increase by one, 4 = increase by two
	int iResult = 0;

	UINT iAgingLevel = GetItem()->sAgeLevel;

	bool bCostumeItem = GetItem()->bCostumeItem == 1;

	auto pcItemData = ITEMSERVER->FindItemPointerTable( GetItem()->sItemID.ToInt() );
	if ( pcItemData )
		if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Costume )
			bCostumeItem = true;

	if ( iAgingLevel < MAX_AGING )
	{
		bool bCanBroke = true;
		bool bIncreaseAgingMax = false;

		static int iaAgingChanceServer[MAX_AGING] =
		{
			0,0,0,0,0,0,0,0,4,8,12,16,18,20,24,33,37,41,45,49,58,63,68,72
		};

		//Rand and fail chance
		int iRandChance = RandomI( 0, 99 );
		int iFailChance = iaAgingChance[iAgingLevel];

		//Reset Aging Amount
		SetAgingIncreaseAmount( 0 );

		switch ( GetStoneItemID().ToItemID() )
		{
			case ITEMID_CopperOre:
				bCanBroke = false;
				break;
			case ITEMID_AgingStone:
				iFailChance -= 10;
				break;
			case ITEMID_RipeStone:
				iFailChance -= 20;
				break;
			case ITEMID_FluenceStone:
				iFailChance -= 30;
				break;
			case ITEMID_ArcaneStone:
				iFailChance -= 40;
				break;
			case ITEMID_MaxAge:
				bIncreaseAgingMax = true;
				break;

			default:
				break;
		}

		//Event
		if ( EVENT_AGINGREDUCEFAIL > 0 )
			iFailChance -= EVENT_AGINGREDUCEFAIL;

		//Aging max stone?
		if ( bIncreaseAgingMax )
		{
			iResult = 5;

			//Increase to 20
			SetAgingIncreaseAmount( 20 );
		}
		else if ( pcUser->bUnbrokeAge ) //Unbroke Age power? Increase by one
		{
			if ( pcUser->bUnbrokeAge == 2 )
				iResult = 4;
			else if ( pcUser->bUnbrokeAge == 15 )
			{
				iResult = 5;

				//Increase to 15
				SetAgingIncreaseAmount( 15 );
			}
			else
				iResult = 3;
		}
		else if ( bCostumeItem )
			IncreaseAgeFunction( pcUser, iAgingLevel, iResult, true );
		else
		{
			//Fail?
			if ( (iFailChance > 0) && (iRandChance < iFailChance) )
			{
				//Chance to decrease?
				int iRandDecreaseRate = RandomI( 1, 100 );
				if ( iRandDecreaseRate < 50 )
				{
					//Decrease by one
					if ( iRandDecreaseRate < 25 )
						iResult = 1;
					else //By two
						iResult = 2;

					//Update Item Integrity
					if ( iAgingLevel <= 9 )
						ITEMSERVER->UpdateIntegrity( GetItem(), -3 );
					else
						ITEMSERVER->UpdateIntegrity( GetItem(), -5 );
				}
				else if ( iRandDecreaseRate < 75 ) //Broke
				{
					//Can Broke?
					if ( bCanBroke )
					{
						//Increase (going)
						iAgingLevel++;

						//Send announcement to users and add item to recovery shop
						CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s broke %s going to +%d! Better luck next time!", pcUser->pcUserData->sCharacterData.szName, GetItem()->szItemName, iAgingLevel );
						AGEHANDLER->OnBrokeItemHandler( pcUser, GetItem() );

						iResult = -1;
					}
				}
			}
			else
				IncreaseAgeFunction( pcUser, iAgingLevel, iResult, true );
		}

		if ( iResult == 0 )
			CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "Arcane Moriph> %s fail %s going to +%d! Better luck next time!", pcUser->pcUserData->sCharacterData.szName, GetItem()->szItemName, iAgingLevel );

	}

	//Save after process
	ITEMSERVER->SaveItemData( GetItem() );

	return iResult;
}

bool CAgeHandler::CanAge( ItemID sItemID )
{
	bool bRet = false;

	switch ( sItemID.ToItemType() )
	{
		case ITEMTYPE_Axe:
		case ITEMTYPE_Hammer:
		case ITEMTYPE_Scythe:
		case ITEMTYPE_Dagger:
		case ITEMTYPE_Sword:
		case ITEMTYPE_Claw:
		case ITEMTYPE_Phantom:
		case ITEMTYPE_Wand:
		case ITEMTYPE_Bow:
		case ITEMTYPE_Javelin:
		case ITEMTYPE_Shield:
		case ITEMTYPE_Orb:
		case ITEMTYPE_Robe:
		case ITEMTYPE_Armor:
			bRet = true;
			break;

		default:
			break;
	}

	return bRet;
}
