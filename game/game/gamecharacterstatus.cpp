#include "stdafx.h"
#include "gamecharacterstatus.h"

BOOL CGameCharacterStatus::UPDATE_BUFFSKILLS = 0;

int CGameCharacterStatus::SPEC_HPPOTION_EFFICIENCE = 0;
int CGameCharacterStatus::SPEC_MPPOTION_EFFICIENCE = 0;
int CGameCharacterStatus::SPEC_SPPOTION_EFFICIENCE = 0;

int EVADE_GLOBAL_ITEM = 0;

CGameCharacterStatus::CGameCharacterStatus()
{
}


CGameCharacterStatus::~CGameCharacterStatus()
{
}

void CGameCharacterStatus::OnMouseLDownClick( int iX, int iY )
{
	JMP( pfnOnMouseLClickDown );
}

void CGameCharacterStatus::UpdateCharacterStatus( CharacterDataLocal * psData )
{
#ifdef _NEW_DMG__

	//Base Damage
	PLAYERDATA->iMinDamage++;
	PLAYERDATA->iMaxDamage++;

	//Consts
	const DWORD				pdwInventory = 0x035EBB20;
	const DWORD				pdwViewDamagePercent = pdwInventory + 0x39C24;
	const DWORD				pdwViewDamageNumber = pdwInventory + 0x39C28;

	//Visual Damage
	if ( READDWORD( pdwViewDamagePercent ) != 0 )
	{
		//Percent Damage
		(*(int*)0x035EB600) += (PLAYERDATA->iMinDamage * READDWORD( pdwViewDamagePercent )) / 100;
		(*(int*)0x035EB604) += (PLAYERDATA->iMaxDamage * READDWORD( pdwViewDamagePercent )) / 100;
	}
	else if ( READDWORD( pdwViewDamageNumber ) != 0 )
	{
		//Static value on Damage
		(*(int*)0x035EB600) += READDWORD( pdwViewDamageNumber );
		(*(int*)0x035EB604) += READDWORD( pdwViewDamageNumber );
	}

	UNITDATA->sCharacterData.sHPBlocks.sCur = psData->iPotionSpace;

	float fLevel = (float)PLAYERDATA->iLevel;
	float fStrength = (float)PLAYERDATA->iStrength;
	float fSpirit = (float)PLAYERDATA->iSpirit;
	float fTalent = (float)PLAYERDATA->iTalent;
	float fAgility = (float)PLAYERDATA->iAgility;
	float fHealth = (float)PLAYERDATA->iHealth;


	//Attack Rating
	PLAYERDATA->iAttackRating = (psData->iAttackRatingSkill + (int)((fAgility * 3.0f) + (fLevel * 2.0f) + (fTalent * 1.2f))) + psData->iLevelAttackRating;

	int iLevel = 0;
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_ScoutHawk ) && (iLevel = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_ScoutHawk )) > 0 )
	{
		int * iSK = ((int*)0x00993BB0);

		PLAYERDATA->iAttackRating += (PLAYERDATA->iAttackRating * iSK[iLevel - 1]) / 100;
	}

	//Critical with 50% cap
	PLAYERDATA->iCritical = psData->iCriticalItems + psData->iCriticalSkills + (int)(fTalent / 50.0f);
	if ( PLAYERDATA->iCritical > 50 )
		PLAYERDATA->iCritical = 50;

	//Defense Formula
	if ( PLAYERDATA->iClass == CHARACTERCLASS_Atalanta )
	{
		//Atalanta Balance
		PLAYERDATA->iDefenseRating = ((int)((fTalent * 1.7f) + (fLevel * 2.0f) + (fAgility * 0.2f) + (fHealth * 3.0f)) + psData->iDefenseSkills) + psData->iDefenseAdd;
	}
	else
	{
		PLAYERDATA->iDefenseRating = ((int)((fTalent * 1.7f) + (fLevel * 2.0f) + (fAgility * 0.8f) + (fHealth * 3.0f)) + psData->iDefenseSkills) + psData->iDefenseAdd;
	}
	
	float fDefense = (float)PLAYERDATA->iDefenseRating;

	//Absorb formula with items that have formula div 2 ( 0.5 abs )
	psData->iCountAbsorbItems /= 2;
	PLAYERDATA->iAbsorbRating = (int)(((fDefense / 100.0f) + (fLevel / 10.0f) + psData->fAbsorbSkills + psData->fAbsorbAdd) + (fStrength / 40) + (fTalent / 40) + (fSpirit / 60) + (fAgility / 60) + (fHealth / 50)) + psData->iCountAbsorbItems + 1;

	//Attack Range for Rangers
	PLAYERDATA->iAttackRange = psData->iAttackRange + psData->iAttackRangeAdd;

	//Sight default is 180
	PLAYERDATA->iSight = psData->iViewSight + 180;

	//Block Rating with 50% cap
	PLAYERDATA->iBlockRating = (int)(round( psData->fBlockRating + psData->fBlockRatingAdd ));
	if ( PLAYERDATA->iBlockRating > 50 )
		PLAYERDATA->iBlockRating = 50;

	int iWeightAdd = 0;

	//Gravity Stone?
	if ( READDWORD( 0x035D0C50 ) > 0 )
	{
		iWeightAdd += READDWORD( 0x035D0C54 );
	}

	//Weight Formula based on status...
	PLAYERDATA->sWeight.sMax = (short)((fStrength * 2.0f) + (fHealth + 1.5f) + (fLevel * 3.0f) + 60 +
		PLAYERDATA->iSpirit + PLAYERDATA->iTalent + PLAYERDATA->iAgility + iWeightAdd);
	
	float fWeightCur = (float)PLAYERDATA->sWeight.sCur;
	float fWeightMax = (float)PLAYERDATA->sWeight.sMax;

	if ( fWeightCur < 0.0f )
		fWeightCur = 0.0f;

	if ( fWeightMax < 0.0f )
		fWeightMax = 0.0f;

	//Move Speed
	PLAYERDATA->iMovementSpeed = (int)( (fAgility / 100) + (fStrength / 240) + (fLevel / 100) - (fWeightCur / fWeightMax) + (psData->fMovementSpeed + psData->fMovementSpeedAdd) + 1);

	//HP Battle Royale
	float fHealthOld = fHealth;
	if ( BATTLEROYALEHANDLER->InBattle() || (MAP_ID == MAPID_OldRuinen2) || (MAP_ID == MAPID_RoyalDesert) )
	{
		if ( fHealth > 150.0f )
			fHealth = 150.0f;
	}

	//HP Formula based on Class
	switch ( PLAYERDATA->iClass )
	{
		case CHARACTERCLASS_Fighter:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.4f) + (fHealth * 1.9f) + (fStrength * 0.6f) + (fSpirit * 0.1f) + (fTalent * 0.2f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Mechanician:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.3f) + (fHealth * 1.7f) + (fStrength * 0.5f) + (fSpirit * 0.2f) + (fTalent * 0.4f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Archer:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.2f) + (fHealth * 1.5f) + (fStrength * 0.2f) + (fSpirit * 0.1f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Pikeman:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 1.7f) + (fStrength * 0.4f) + (fSpirit * 0.1f) + (fTalent * 0.1f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Atalanta:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.2f) + (fHealth * 1.6f) + (fStrength * 0.3f) + (fSpirit * 0.2f) + (fTalent * 0.1f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Knight:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.3f) + (fHealth * 1.6f) + (fStrength * 0.5f) + (fSpirit * 0.2f) + (fTalent * 0.5f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Magician:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 1.7f) + (fStrength * 0.1f) + (fSpirit * 0.2f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Priestess:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 1.6f) + (fStrength * 0.1f) + (fSpirit * 0.3f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Assassin:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 1.6f) + (fStrength * 0.4f) + (fSpirit * 0.1f) + (fTalent * 0.1f) + (fAgility * 0.2f));
			break;

		case CHARACTERCLASS_Shaman:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.6f) + (fHealth * 1.8f) + (fStrength * 0.1f) + (fSpirit * 0.3f) + (fTalent * 0.2f) + (fAgility * 0.1f));
			break;

		default:
			break;
	}

	//Update Old HP
	fHealth = fHealthOld;

	//All classes receive Add HP by spec and skills...
	PLAYERDATA->sHP.sMax += (short)((float)psData->iLevelHP + psData->fIncreaseHPAdd);

	//Items Premium of HP
	if ( READDWORD( 0x035D0C80 ) > 0 )
	{
		PLAYERDATA->sHP.sMax = (short)READDWORD( 0x035D0C84 );
	}

	//Quest 40
	if ( READDWORD( 0x0362B6A4 ) == 0 )
	{
		if ( ITEMHANDLER->GetItemAmountByCode( (EItemID)0, EItemCraftType::ITEMCRAFTTYPE_QuestWeapon ) == 0 )
		{
			if ( PLAYERDATA->iRank >= ECharacterRank::CHARACTERRANK_Rank3 && PLAYERDATA->iLevel >= 40 )
				PLAYERDATA->sHP.sMax += 15;
		}
	}

	//Quest 90
	if ( (READDWORD( 0x0362BA68 ) & 32) == 32 )
	{
		if ( PLAYERDATA->iLevel >= 90 )
			PLAYERDATA->sHP.sMax += 40;
	}

	//HP for Virtual Life
	(*(short*)0x04B0BA48) = PLAYERDATA->sHP.sMax;
	PLAYERDATA->sHP.sMax += (short)(*(int*)0x0367E0E4);

	//MP Formula based on type of characters ( Meele, Range, Magic )
	switch ( PLAYERDATA->iMPType )
	{	
		//Mage & Prs
		case 1:
			PLAYERDATA->sMP.sMax = (short)((fLevel * 1.5f) + (fSpirit * 3.8f));
			PLAYERDATA->sMP.sMax += (short)(psData->fIncreaseMPAdd);
			break;
		//Knight & Atalanta
		case 2:
			PLAYERDATA->sMP.sMax = (short)((fLevel * 0.9f) + (fSpirit * 2.7f));
			PLAYERDATA->sMP.sMax += (short)((float)psData->iLevelMP + psData->fIncreaseMPAdd);
			break;
		//Fs, MS...
		case 3:
			PLAYERDATA->sMP.sMax = (short)((fLevel * 0.6f) + (fSpirit * 2.2f));
			PLAYERDATA->sMP.sMax += (short)((float)psData->iLevelMP + psData->fIncreaseMPAdd);
			break;

		default:
			break;
	}

	//SP Formula is static...
	PLAYERDATA->sSP.sMax = (short)((fHealth * 1.4f) + (fStrength * 0.5f) + (fTalent * 0.5f) + (fLevel * 2.3f) + 80 + fSpirit);
	PLAYERDATA->sSP.sMax += (short)(psData->fIncreaseSPAdd);

	//Regen ( its not a formula )
	if ( int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill(SKILLID_SurvivalInstinct); iLevelSkill > 0 )
		psData->fRegenHP += faFighterT51AddHPRegen[iLevelSkill - 1];

	PLAYERDATA->fHPRegen = psData->fRegenHP + psData->fPercentHPAdd;
	PLAYERDATA->fMPRegen = psData->fRegenMP + psData->fPercentMPAdd;
	PLAYERDATA->fSPRegen = psData->fRegenSP + psData->fPercentSPAdd;

	//Attack Speed Formula
	// Bellatra Gold
	if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraGold ) )
	{
		if ( (MAP_ID != MAPID_Bellatra) && (MAP_ID != MAPID_BlessCastle) )
			psData->iAttackSpeedAdd++;
	}

	PLAYERDATA->iAttackSpeed = psData->iWeaponSpeed + psData->iAttackSpeedAdd + psData->iSkillWeaponSpeed;

#else

	//Base Damage
	PLAYERDATA->iMinDamage++;
	PLAYERDATA->iMaxDamage++;

	//Consts
	const DWORD				pdwInventory = 0x035EBB20;
	const DWORD				pdwViewDamagePercent = pdwInventory + 0x39C24;
	const DWORD				pdwViewDamageNumber = pdwInventory + 0x39C28;

	//Visual Damage
	if ( READDWORD( pdwViewDamagePercent ) != 0 )
	{
		//Percent Damage
		(*(int*)0x035EB600) += (PLAYERDATA->iMinDamage * READDWORD( pdwViewDamagePercent )) / 100;
		(*(int*)0x035EB604) += (PLAYERDATA->iMaxDamage * READDWORD( pdwViewDamagePercent )) / 100;
	}
	else if ( READDWORD( pdwViewDamageNumber ) != 0 )
	{
		//Static value on Damage
		(*(int*)0x035EB600) += READDWORD( pdwViewDamageNumber );
		(*(int*)0x035EB604) += READDWORD( pdwViewDamageNumber );
	}

	float fLevel = (float)PLAYERDATA->iLevel;
	float fStrength = (float)PLAYERDATA->iStrength;
	float fSpirit = (float)PLAYERDATA->iSpirit;
	float fTalent = (float)PLAYERDATA->iTalent;
	float fAgility = (float)PLAYERDATA->iAgility;
	float fHealth = (float)PLAYERDATA->iHealth;

	//Attack Rating
	PLAYERDATA->iAttackRating = (psData->iAttackRatingSkill + (int)((fAgility * 3.1f) + (fLevel * 1.9f) + (fTalent * 1.5f))) + psData->iLevelAttackRating;

	int iLevel = 0;
	if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_ScoutHawk ) && (iLevel = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_ScoutHawk )) > 0 )
	{
		int * iSK = ((int*)0x00993BB0);

		PLAYERDATA->iAttackRating += (PLAYERDATA->iAttackRating * iSK[iLevel - 1]) / 100;
	}


	//Critical with 50% cap
	PLAYERDATA->iCritical = psData->iCriticalItems + psData->iCriticalSkills;
	if ( PLAYERDATA->iCritical > 50 )
	PLAYERDATA->iCritical = 50;

	//Defense Formula
	PLAYERDATA->iDefenseRating = (int)(fAgility + (fTalent / 4.0f) + (fLevel * 1.4f) + psData->iDefenseSkills) + psData->iDefenseAdd;
	float fDefense = (float)PLAYERDATA->iDefenseRating;

	//Absorb formula with items that have formula div 2 ( 0.5 abs )
	psData->iCountAbsorbItems /= 2;
	PLAYERDATA->iAbsorbRating = (int)(((fDefense / 100.0f) + (fLevel / 10.0f) + psData->fAbsorbSkills + psData->fAbsorbAdd) + ((fStrength + fTalent + fHealth) / 40) + psData->iCountAbsorbItems + 1);

	//Attack Range for Rangers
	PLAYERDATA->iAttackRange = psData->iAttackRange + psData->iAttackRangeAdd;

	//Sight default is 180
	PLAYERDATA->iSight = psData->iViewSight + 180;

	//Block Rating with 50% cap
	PLAYERDATA->iBlockRating = (int)(psData->fBlockRating + psData->fBlockRatingAdd);
	if ( PLAYERDATA->iBlockRating > 50 )
	PLAYERDATA->iBlockRating = 50;

	int iWeightAdd = 0;

	//Gravity Stone?
	if ( READDWORD( 0x035D0C50 ) > 0 )
	{
		iWeightAdd += READDWORD( 0x035D0C54 );
	}

	//Weight Formula based on status...
	PLAYERDATA->sWeight.sMax = (short)((fStrength * 2.0f) + (fHealth + 1.5f) + (fLevel * 3.0f) + 60 +
		PLAYERDATA->iSpirit + PLAYERDATA->iTalent + PLAYERDATA->iAgility + iWeightAdd);

	float fWeightCur = (float)PLAYERDATA->sWeight.sCur;
	float fWeightMax = (float)PLAYERDATA->sWeight.sMax;

	if ( fWeightCur < 0.0f )
		fWeightCur = 0.0f;

	if ( fWeightMax < 0.0f )
		fWeightMax = 0.0f;

	//Move Speed
	PLAYERDATA->iMovementSpeed = (int)(((fTalent + fHealth + fLevel + 60.0f) / 150.0f) - (fWeightCur / fWeightMax) + (psData->fMovementSpeed + psData->fMovementSpeedAdd + 1.4f));

	//HP Battle Royale
	float fHealthOld = fHealth;
	if ( BATTLEROYALEHANDLER->InBattle() || (MAP_ID == MAPID_OldRuinen2) || (MAP_ID == MAPID_RoyalDesert) )
	{
		if ( fHealth > 150.0f )
			fHealth = 150.0f;
	}

	//HP Formula based on Class
	switch ( PLAYERDATA->iClass )
	{
		case CHARACTERCLASS_Fighter:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.4f) + (fStrength * 0.8f));
			break;

		case CHARACTERCLASS_Mechanician:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.4f) + (fStrength * 0.8f));
			break;

		case CHARACTERCLASS_Archer:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.8f) + (fHealth * 2.6f) + (fStrength * 0.4f));
			break;

		case CHARACTERCLASS_Pikeman:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.2f) + (fStrength * 0.6f));
			break;

		case CHARACTERCLASS_Atalanta:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.2f) + (fStrength * 0.6f));
			break;

		case CHARACTERCLASS_Knight:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.2f) + (fStrength * 0.6f));
			break;

		case CHARACTERCLASS_Magician:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.8f) + (fHealth * 1.8f) + (fSpirit * 0.6f));
			break;

		case CHARACTERCLASS_Priestess:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.8f) + (fHealth * 1.8f) + (fSpirit * 0.6f));
			break;

		case CHARACTERCLASS_Assassin:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 2.2f) + (fStrength * 0.6f));
			break;

		case CHARACTERCLASS_Shaman:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.8f) + (fHealth * 1.8f) + (fSpirit * 0.6f));
			break;

		default:
			break;
	}

	//Update Old HP
	fHealth = fHealthOld;

	//All classes receive Add HP by spec and skills...
	PLAYERDATA->sHP.sMax += (short)((float)psData->iLevelHP + psData->fIncreaseHPAdd);

	//Items Premium of HP
	if ( READDWORD( 0x035D0C80 ) > 0 )
	{
		PLAYERDATA->sHP.sMax = (short)READDWORD( 0x035D0C84 );
	}

	//Quest 40
	if ( READDWORD( 0x0362B6A4 ) == 0 )
	{
		if ( ITEMHANDLER->GetItemAmountByCode( (EItemID)0, EItemCraftType::ITEMCRAFTTYPE_QuestWeapon ) == 0 )
		{
			if ( PLAYERDATA->iRank >= ECharacterRank::CHARACTERRANK_Rank3 && PLAYERDATA->iLevel >= 40 )
				PLAYERDATA->sHP.sMax += 15;
		}
	}

	//Quest 90
	if ( (READDWORD( 0x0362BA68 ) & 32) == 32 )
	{
		if ( PLAYERDATA->iLevel >= 90 )
			PLAYERDATA->sHP.sMax += 40;
	}

	//HP for Virtual Life
	(*(short*)0x04B0BA48) = PLAYERDATA->sHP.sMax;
	PLAYERDATA->sHP.sMax += (short)(*(int*)0x0367E0E4);

	//MP Formula based on type of characters ( Meele, Range, Magic )
	switch ( PLAYERDATA->iMPType )
	{
		case 1:
			PLAYERDATA->sMP.sMax = (short)((fLevel * 1.5f) + (fSpirit * 3.8f));
			PLAYERDATA->sMP.sMax += (short)(psData->fIncreaseMPAdd);
			break;

		case 2:
			PLAYERDATA->sMP.sMax = (short)((fLevel * 0.9f) + (fSpirit * 2.7f));
			PLAYERDATA->sMP.sMax += (short)((float)psData->iLevelMP + psData->fIncreaseMPAdd);
			break;

		case 3:
			PLAYERDATA->sMP.sMax = (short)((fLevel * 0.6f) + (fSpirit * 2.2f));
			PLAYERDATA->sMP.sMax += (short)((float)psData->iLevelMP + psData->fIncreaseMPAdd);
			break;

		default:
			break;
	}

	//SP Formula is static...
	PLAYERDATA->sSP.sMax = (short)((fHealth * 1.4f) + (fStrength * 0.5f) + (fTalent * 0.5f) + (fLevel * 2.3f) + 80 + fSpirit);
	PLAYERDATA->sSP.sMax += (short)(psData->fIncreaseSPAdd);

	//Regen ( its not a formula )
	PLAYERDATA->fHPRegen = psData->fRegenHP + psData->fPercentHPAdd;
	PLAYERDATA->fMPRegen = psData->fRegenMP + psData->fPercentMPAdd;
	PLAYERDATA->fSPRegen = psData->fRegenSP + psData->fPercentSPAdd;


	//Attack Speed Formula
	//Bellatra Gold
	if ( SKILLMANAGERHANDLER->IsActiveBuff( TIMERSKILLID_BellatraGold ) )
	{
		if ( (MAP_ID != MAPID_Bellatra) && (MAP_ID != MAPID_BlessCastle) )
			psData->iAttackSpeedAdd++;
	}

	PLAYERDATA->iAttackSpeed = psData->iWeaponSpeed + psData->iAttackSpeedAdd + psData->iSkillWeaponSpeed;

#endif

	//Weapon Spec
	if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
	{
		bool bDamageColor = false;

		auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];

		switch ( UNITDATA->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Fighter:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Axe )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Mechanician:
				if ( (pcItemData->sItem.eSpecialization & UNITDATA->sCharacterData.iFlag) || (pcItemData->sItem.iItemUniqueID == 2) )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Archer:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Bow )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Pikeman:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Scythe )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Atalanta:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Javelin )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Knight:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Sword )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Magician:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Wand )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Priestess:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Wand )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Assassin:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Dagger )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
			case CHARACTERCLASS_Shaman:
				if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Phantom )
					bDamageColor = (UNITDATA->sCharacterData.iLevel / 6) ? true : false;
				break;
		}

		if ( bDamageColor )
			SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Gold );
	}
}

BOOL CGameCharacterStatus::UpdateCharacterStatusItem( BOOL bCheckHack )
{
	EVADE_GLOBAL_ITEM = 0;

	CGameCharacterStatus::SPEC_HPPOTION_EFFICIENCE = 0;
	CGameCharacterStatus::SPEC_MPPOTION_EFFICIENCE = 0;
	CGameCharacterStatus::SPEC_SPPOTION_EFFICIENCE = 0;

	//Time Skill Update
	{
		static DWORD dwTimeUpdate = 0;

		if ( (dwTimeUpdate + 1000) < TICKCOUNT )
		{
			CGameCharacterStatus::UPDATE_BUFFSKILLS = TRUE;

			dwTimeUpdate = TICKCOUNT;
		}
	}

	ElementalAttackData * psElementalAttack = (ElementalAttackData*)0x035E5BB8;
	ZeroMemory( psElementalAttack, sizeof( ElementalAttackData ) );

	ItemData cItemWrapper;

	ZeroMemory( &cItemWrapper, sizeof( ItemData ) );

	CharacterDataLocal sCharacterDataLocal;
	ZeroMemory( &sCharacterDataLocal, sizeof( CharacterDataLocal ) );

	sCharacterDataLocal.iPotionSpace = 2;

	IMinMax * psDamageFake = ((IMinMax *)0x035EB600);
	psDamageFake->iMin = psDamageFake->iMax = 0;

	//Get Inventory Items
	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		//Valid Item?
		auto pcItemData = INVENTORYITEMS + i;
		if ( pcItemData->bValid )
		{
			//Using Item?
			if ( pcItemData->iItemSlot && (pcItemData->sItem.bCanNotUse == FALSE) )
				AddStatusItem( sCharacterDataLocal, pcItemData );
		}
	}

	//Costume Weapon and Shield
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidWeaponItem() )
		{
			if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetWeaponItem() )
			{
				if ( pcItemData->bValid && (pcItemData->sItem.bCanNotUse == FALSE) )
					AddStatusItem( sCharacterDataLocal, pcItemData );
			}
		}

		if ( HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->IsValidShieldItem() )
		{
			if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetWindowCostume()->GetShieldItem() )
			{
				if ( pcItemData->bValid && (pcItemData->sItem.bCanNotUse == FALSE) )
					AddStatusItem( sCharacterDataLocal, pcItemData );
			}
		}

		if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetEarring1() )
		{
			if ( pcItemData->bValid && (pcItemData->sItem.bCanNotUse == FALSE) )
				AddStatusItem( sCharacterDataLocal, pcItemData );
		}
		if ( auto pcItemData = HUDHANDLER->GetInventoryWindow()->GetEarring2() )
		{
			if ( pcItemData->bValid && (pcItemData->sItem.bCanNotUse == FALSE) )
				AddStatusItem( sCharacterDataLocal, pcItemData );
		}
	}

	//Add Skill
	AddSkillStatus( sCharacterDataLocal );

	UNITGAME->SetCharacterFormula( INVENTORYITEMS, INVENTORYITEMSLOT, sCharacterDataLocal );

	UpdateCharacterStatus( &sCharacterDataLocal );

	AddSkillStatusPost( sCharacterDataLocal );

	CHARACTERGAME->OnCharacterUpdateData();

	if ( CHARACTERGAME->GetCurrentHP() > (int)UNITDATA->sCharacterData.sHP.sMax )
		CHARACTERGAME->SetCurrentHP( (int)UNITDATA->sCharacterData.sHP.sMax );

	ITEMHANDLER->CheckRequirements();

	UPDATE_CHARACTER_CHECKSUM;

	ITEMHANDLER->ResetPotionGate();

	if ( bCheckHack )
		CharacterStatusVerify( &UNITDATA->sCharacterData, ((CharacterData *)0x0362A1E8) );
	else
		CopyMemory( ((CharacterData *)0x0362A1E8), &UNITDATA->sCharacterData, sizeof( CharacterData ) );

	CGameCharacterStatus::UPDATE_BUFFSKILLS = FALSE;

	EVADE_GLOBAL_ITEM = sCharacterDataLocal.iEvade;

	return TRUE;
}

bool CGameCharacterStatus::AddStatusItem( CharacterDataLocal & sCharacterDataLocal, ItemData * pcItemData )
{
	//Potion?
	if ( pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
		return false;

	//Hasn't Requirements?
	if ( !HasRequirementItem( pcItemData ) )
		return false;

	//Two Hands Item and left side?
	if ( pcItemData->IsTwoHandedWeapon() && (pcItemData->sItem.iChk2 == 0) )
		return false;

	//Is Valid Item?
	if ( ITEMHANDLER->IsValidItem( &pcItemData->sItem ) == FALSE )
	{
		//TODO CHECKS
		pcItemData->bValid = FALSE;

		return false;
	}

	sCharacterDataLocal.iAttackRatingSkill += pcItemData->sItem.iAttackRating;
	if ( (pcItemData->sItem.sDamage.sMin > 0) || (pcItemData->sItem.sDamage.sMax > 0) )
	{
		sCharacterDataLocal.iMinDamage += pcItemData->sItem.sDamage.sMin;
		sCharacterDataLocal.iMaxDamage += pcItemData->sItem.sDamage.sMax;
	}

	sCharacterDataLocal.iCritical += pcItemData->sItem.iCritical;
	sCharacterDataLocal.iCriticalItems += pcItemData->sItem.iCritical;
	sCharacterDataLocal.iDefenseAdd += pcItemData->sItem.iDefenseRating;

	sCharacterDataLocal.fAbsorbAdd += ((pcItemData->sItem.fAbsorbRating * 10.000001f) / 10.0f) + 0.000001f;

	if ( pcItemData->sItem.fAbsorbRating )
		sCharacterDataLocal.iCountAbsorbItems++;

	sCharacterDataLocal.fBlockRating += pcItemData->sItem.fBlockRating;
	sCharacterDataLocal.iEvade += pcItemData->sItem.bEvade;
	sCharacterDataLocal.iWeaponSpeed += pcItemData->sItem.iAttackSpeed;
	sCharacterDataLocal.iAttackRange += pcItemData->sItem.iAttackRange;
	sCharacterDataLocal.fMagicMastery += pcItemData->sItem.fMagicMastery;
	sCharacterDataLocal.fMovementSpeed += pcItemData->sItem.fMovementSpeed;

	sCharacterDataLocal.fRegenHP += pcItemData->sItem.fHPRegen;
	sCharacterDataLocal.fRegenMP += pcItemData->sItem.fMPRegen;
	sCharacterDataLocal.fRegenSP += pcItemData->sItem.fSPRegen;

	sCharacterDataLocal.fIncreaseHPAdd += pcItemData->sItem.fAddHP;
	sCharacterDataLocal.fIncreaseMPAdd += pcItemData->sItem.fAddMP;
	sCharacterDataLocal.fIncreaseSPAdd += pcItemData->sItem.fAddSP;

	if ( pcItemData->sItem.iPotionStorage )
	{
		sCharacterDataLocal.iPotionSpace = pcItemData->sItem.iPotionStorage;
	}

	if ( (UNITDATA->sCharacterData.iFlag & pcItemData->sItem.eSpecialization) == UNITDATA->sCharacterData.iFlag )
	{
		sCharacterDataLocal.iLevelMP += pcItemData->sItem.sSpecData.iSpecAddMPDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAddMPDiv) : 0;
		sCharacterDataLocal.iLevelHP += pcItemData->sItem.sSpecData.iSpecAddHPDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAddHPDiv) : 0;
		sCharacterDataLocal.iLevelAttackRating += pcItemData->sItem.sSpecData.iSpecAttackRatingDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAttackRatingDiv) : 0;
		sCharacterDataLocal.iLevelDamage += pcItemData->sItem.sSpecData.iSpecAttackPowerDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAttackPowerDiv) : 0;
		sCharacterDataLocal.fPercentMPAdd += pcItemData->sItem.sSpecData.fSpecMPRegen ? (pcItemData->sItem.sSpecData.fSpecMPRegen / 2.0f) : 0;
		sCharacterDataLocal.fPercentHPAdd += pcItemData->sItem.sSpecData.fSpecHPRegen ? (pcItemData->sItem.sSpecData.fSpecHPRegen / 2.0f) : 0;
		sCharacterDataLocal.fPercentSPAdd += pcItemData->sItem.sSpecData.fSpecSPRegen ? (pcItemData->sItem.sSpecData.fSpecSPRegen / 2.0f) : 0;

		sCharacterDataLocal.iCriticalItems += pcItemData->sItem.sSpecData.iSpecCritical;
		sCharacterDataLocal.fAbsorbAdd += pcItemData->sItem.sSpecData.fSpecAbsorbRating;
		sCharacterDataLocal.fBlockRatingAdd += pcItemData->sItem.sSpecData.fSpecBlockRating;
		sCharacterDataLocal.iAttackRange += pcItemData->sItem.sSpecData.iSpecAttackRange;
		sCharacterDataLocal.fMagicMastery += pcItemData->sItem.sSpecData.fSpecMagicMastery;
		sCharacterDataLocal.iDefenseAdd += pcItemData->sItem.sSpecData.iSpecDefenseRating;
		sCharacterDataLocal.fMovementSpeedAdd += pcItemData->sItem.sSpecData.fSpecMovementSpeed;
		sCharacterDataLocal.iAttackSpeedAdd += pcItemData->sItem.sSpecData.iSpecAttackSpeed;

		CGameCharacterStatus::SPEC_HPPOTION_EFFICIENCE += pcItemData->sItem.sSpecData.sSpecHPPotionEfficience;
		CGameCharacterStatus::SPEC_MPPOTION_EFFICIENCE += pcItemData->sItem.sSpecData.sSpecMPPotionEfficience;
		CGameCharacterStatus::SPEC_SPPOTION_EFFICIENCE += pcItemData->sItem.sSpecData.sSpecSPPotionEfficience;

		if ( pcItemData->sItem.sSpecData.fSpecAbsorbRating )
			sCharacterDataLocal.iCountAbsorbItems++;

		//Resistances
		for ( int i = 0; i < _countof( pcItemData->sItem.sSpecData.saSpecElementalDef ); i++ )
		{
			sCharacterDataLocal.iaElementalSpecDef[i] += (int)pcItemData->sItem.sSpecData.saSpecElementalDef[i];

			if ( pcItemData->sItem.sSpecData.saSpecElementalAtkDiv[i] )
			{
				sCharacterDataLocal.iaElementalSpecAtk[i] += (int)pcItemData->sItem.sSpecData.saSpecElementalAtkDiv[i] +
					(UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.saSpecElementalAtkDiv[i]);
			}
		}
	}

	for ( int i = 0; i < _countof( pcItemData->sItem.saElementalDef ); i++ )
		sCharacterDataLocal.iaResistance[i] += (int)pcItemData->sItem.saElementalDef[i];

	return true;
}

bool bMetalArmorSkillActive = false;
bool CGameCharacterStatus::AddSkillStatus( CharacterDataLocal & sCharacterDataLocal )
{
	IMinMax * psDamageFake = ((IMinMax *)0x035EB600);

	bMetalArmorSkillActive = false;

	//Weapon Add
	if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
	{
		ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1;
		if ( pcItemData->bValid )
		{
			sCharacterDataLocal.fTempDamageMin = (float)pcItemData->sItem.sDamage.sMin;
			sCharacterDataLocal.fTempDamageMax = (float)pcItemData->sItem.sDamage.sMax;

			sCharacterDataLocal.iTempAttackRating = pcItemData->sItem.iAttackRating;
			
			sCharacterDataLocal.iTempWeaponSpeed = pcItemData->sItem.iAttackSpeed;

			sCharacterDataLocal.iTempBlockRating = (int)pcItemData->sItem.fBlockRating;

			if ( UNITDATA->sCharacterData.iFlag & pcItemData->sItem.eSpecialization )
				sCharacterDataLocal.iTempWeaponSpeed += pcItemData->sItem.sSpecData.iSpecAttackSpeed;

			sCharacterDataLocal.fTempCritical = (float)pcItemData->sItem.sSpecData.iSpecCritical;
		}
	}

	if ( INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex )
	{
		ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex - 1;
		if ( pcItemData->bValid )
		{
			sCharacterDataLocal.iTempDefense = pcItemData->sItem.iDefenseRating;

			if ( UNITDATA->sCharacterData.iFlag & pcItemData->sItem.eSpecialization )
			{
				sCharacterDataLocal.iTempDefense += pcItemData->sItem.sSpecData.iSpecDefenseRating;

				bMetalArmorSkillActive = true;
			}
		}
	}

	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
	{
		ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1;
		if ( pcItemData->bValid )
		{
			sCharacterDataLocal.iTempDefense2 = pcItemData->sItem.iDefenseRating;

			if ( UNITDATA->sCharacterData.iFlag & pcItemData->sItem.eSpecialization )
				sCharacterDataLocal.iTempDefense2 += pcItemData->sItem.sSpecData.iSpecDefenseRating;
		}
	}

	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_White );
	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_HP, CHARSTATUSCOLOR_White );
	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_MP, CHARSTATUSCOLOR_White );
	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_SP, CHARSTATUSCOLOR_White );
	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_White );
	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_White );

	int iaResistance[_countof( CharacterData::sElementalDef )];
	for ( int i = 0; i < _countof( CharacterData::sElementalDef ); i++ )
		iaResistance[i] = 0;

	for ( int i = 0; i < MAX_SKILLS_TIMER; i++ )
	{
		Skill * psSkill = TIMERSKILLHANDLER->GetPointerSkillTimer() + i;
		if ( psSkill->bActive )
		{
			switch ( psSkill->iID )
			{
				case SKILLID_ExtremeShield:
				{
					if ( ItemID( UNITDATA->sLeftHandTool.eItemID ).ToItemType() == ITEMTYPE_Shield )
					{
						if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
						{
							ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1;
							if ( pcItemData->bValid && pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield )
							{
								int * piaSkillTable = (int *)0x00992698;
								sCharacterDataLocal.fBlockRating += (pcItemData->sItem.fBlockRating * (float)piaSkillTable[psSkill->iLevel - 1]) / 100;
							}
						}
					}
				}
				break;

				case SKILLID_Automation:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
						if ( (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Bow) || (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Javelin) )
						{
							int iDamageBoost = psSkill->PlusState[0];

							psDamageFake->iMin += (UNITDATA->sCharacterData.iMinDamage * iaMechT23DamageBoost[psSkill->iLevel - 1]) / 100;
							psDamageFake->iMax += (UNITDATA->sCharacterData.iMaxDamage * iaMechT23DamageBoost[psSkill->iLevel - 1]) / 100;

							if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
								PROCESSSKILLBUFFEX( SKILLPLAYID_T23, psSkill->iLevel );
						}
						else if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
							PROCESSSKILLBUFFEX( SKILLPLAYID_T23, psSkill->iLevel );
					}
				}
				break;

				case SKILLID_EnchantWeapon:
					if ( UNITDATA->iEnchantWeaponType == 3 )
					{
						psDamageFake->iMin += iaMageT23FireDamage->iaValue[psSkill->iLevel - 1][0];
						psDamageFake->iMax += iaMageT23FireDamage->iaValue[psSkill->iLevel - 1][1];
					}
					else if ( UNITDATA->iEnchantWeaponType == 4 )
					{
						psDamageFake->iMin += iaMageT23IceDamage->iaValue[psSkill->iLevel - 1][0];
						psDamageFake->iMax += iaMageT23IceDamage->iaValue[psSkill->iLevel - 1][1];
					}
					else if ( UNITDATA->iEnchantWeaponType == 5 )
					{
						psDamageFake->iMin += iaMageT23LightDamage->iaValue[psSkill->iLevel - 1][0];
						psDamageFake->iMax += iaMageT23LightDamage->iaValue[psSkill->iLevel - 1][1];
					}
					break;

				case SKILLID_Windy:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
						if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Javelin )
						{
							sCharacterDataLocal.iAttackRatingSkill += (((int *)0x00994970)[psSkill->iLevel - 1] * sCharacterDataLocal.iTempAttackRating) / 100;

							sCharacterDataLocal.iAttackRange += 30;

							SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
						}
						else
						{
							if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
								PROCESSSKILLBUFFEX( SKILLPLAYID_T22, psSkill->iLevel );

							SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_White );
						}
					}
				}
				break;

				case SKILLID_MetalArmor:
				{
					if ( bMetalArmorSkillActive )
					{
						if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
							PROCESSSKILLBUFFEX( SKILLPLAYID_T22, psSkill->iLevel );

						sCharacterDataLocal.iDefenseSkills += (sCharacterDataLocal.iTempDefense * (float)iaMechT31DefenseRtg[psSkill->iLevel - 1]) / 100.0f;
					}
				}
				break;

				case SKILLID_SparkShield:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
						if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield )
							sCharacterDataLocal.iDefenseSkills += ((int *)0x00992BE8)[psSkill->iLevel - 1];
					}
				}
				break;

				case SKILLID_TriumphofJavelin:
					psDamageFake->iMax += ((int *)0x00994BC8)[psSkill->iLevel - 1] / psSkill->bActive;

					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Gold );
					break;

				case SKILLID_SpiritElemental:
					psDamageFake->iMin += (((int *)0x04B0DC04)[psSkill->iLevel - 1] * UNITDATA->sCharacterData.iMinDamage) / 100;
					psDamageFake->iMax += (((int *)0x04B0DC2C)[psSkill->iLevel - 1] * UNITDATA->sCharacterData.iMaxDamage) / 100;

					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Gold );
					break;

				case SKILLID_GoldenFalcon:
					sCharacterDataLocal.fRegenHP += ((float *)0x009940B0)[psSkill->iLevel - 1];
					break;

				case SKILLID_VirtualLife:
					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_HP, CHARSTATUSCOLOR_Pink );
					break;

				case SKILLID_GodlyShield:
					if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
						if ( pcItemData->bValid && (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield) )
						{
							if ( int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_DivineShield ); iLevelSkill > 0 )
								sCharacterDataLocal.fBlockRating += (float)((int *)0x009935E8)[iLevelSkill - 1];

							SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_Blue );
						}
					}
					break;

				case SKILLID_DivineShield:
					if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
						if ( pcItemData->bValid && (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield) )
							sCharacterDataLocal.fBlockRating += (float)((int *)0x009935E8)[psSkill->iLevel - 1];
					}
					break;

				case SKILLID_Berserker:
					psDamageFake->iMin += (((int *)0x009957A8)[psSkill->iLevel - 1] * UNITDATA->sCharacterData.iMinDamage) / 100;
					psDamageFake->iMax += (((int *)0x009957A8)[psSkill->iLevel - 1] * UNITDATA->sCharacterData.iMaxDamage) / 100;
					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Absorb, CHARSTATUSCOLOR_Red );
					break;

				case SKILLID_ForceofNature:
					psDamageFake->iMin += ((int *)0x00994470)[psSkill->iLevel - 1];
					psDamageFake->iMax += ((int *)0x00994470)[psSkill->iLevel - 1];
					sCharacterDataLocal.iAttackRatingSkill += ((int *)0x00994498)[psSkill->iLevel - 1];

					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Gold );
					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
					break;

				case SKILLID_HallofValhalla:
					//Has Hov Level?
					if ( int iLevelSkill = READDWORD( 0x0367E04C ) )
						psDamageFake->iMax += (((int *)0x00994BC8)[iLevelSkill - 1]) / psSkill->bActive;
					break;

				case SKILLID_Compulsion:
					sCharacterDataLocal.fAbsorbSkills += (float)psSkill->iLevel * 3;
					sCharacterDataLocal.fAbsorbSkills += (float)((((int *)0x00992D28)[psSkill->iLevel - 1] * UNITDATA->sCharacterData.iAbsorbRating) / 100);
					break;

				case SKILLID_FrostJavelin:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
						if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Javelin )
						{
							IMinMax * psaFrostJavelin = ((IMinMax *)0x00994DF8);

							psDamageFake->iMin += (float)psaFrostJavelin[psSkill->iLevel - 1].iMin;
							psDamageFake->iMax += (float)psaFrostJavelin[psSkill->iLevel - 1].iMax;
						}
						else
						{
							psSkill->iCheckTime = psSkill->iUseTime * 70;

							SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_White );

							if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
								SKILLMANAGERHANDLER->CancelSkillOnServer( SKILLPLAYID_T43 );
						}
					}
				}
				break;

				case SKILLID_Zenith:
				{
					int iElement = ((int *)0x00995CF8)[psSkill->iLevel - 1] / (psSkill->PartyFlag ? 2 : 1);

					iaResistance[0] += iElement;
					iaResistance[2] += iElement;
					iaResistance[3] += iElement;
					iaResistance[4] += iElement;
					iaResistance[5] += iElement;
				}
				break;

				case SKILLID_AdventMigal:
					psDamageFake->iMin += (float)((int *)0x00998BB0)[psSkill->iLevel - 1];
					psDamageFake->iMax += (float)((int *)0x00998BB0)[psSkill->iLevel - 1];

					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Gold );
					break;

				case SKILLID_Rainmaker:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex - 1];
						if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Orb )
						{
							psSkill->iCheckTime = psSkill->iUseTime * 70;

							SKILLMANAGERHANDLER->CancelSkillOnServer( SKILLID_Rainmaker );
						}
					}
				}
				break;

				case SKILLID_Vague:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
					{
						auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
						if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
						{
							SKILLMANAGERHANDLER->CancelSkillOnServer( SKILLID_Vague, (pcItemData->bValid &&
								(pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Scythe) && pcItemData->IsTwoHandedWeapon()) ? psSkill->iLevel : 0 );
						}
					}
					else if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
						SKILLMANAGERHANDLER->CancelSkillOnServer( SKILLID_Vague, 0 );
				}
				break;

				default:
					break;
			}
		}
	}

	auto HasItemRequiredSkill = []( Skill * psSkill, int iItemIndex ) -> bool
	{
		bool bRet = false;
		
		if ( INVENTORYITEMSLOT[iItemIndex].iItemIndex )
		{
			auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[iItemIndex].iItemIndex - 1];
			
			for ( int i = 0; i < _countof( SkillBase::eSkillItemAllowed ); i++ )
			{
				if ( psSkill->sSkillInfo.eSkillItemAllowed[i] == pcItemData->sItem.sItemID.ToItemType() )
				{
					bRet = true;

					break;
				}
			}
		}

		return bRet;
	};

	auto IsTwoHandedWeapon = []()-> BOOL
	{
		if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
		{
			auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
			if ( pcItemData->bValid && pcItemData->IsTwoHandedWeapon() )
				return TRUE;
		}

		return FALSE;
	};

	auto HasItemRequiredType = []( EItemType eItemType, int iItemIndex ) -> bool
	{
		bool bRet = false;

		if ( INVENTORYITEMSLOT[iItemIndex].iItemIndex )
		{
			auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[iItemIndex].iItemIndex - 1];
			if ( eItemType == pcItemData->sItem.sItemID.ToItemType() )
				bRet = true;
		}

		return bRet;
	};

	for ( int i = 1; i <= 20; i++ )
	{
		Skill * psSkill = SKILLSCHARACTER + i;
		if ( psSkill->bActive && psSkill->iLevel )
		{
			switch ( psSkill->iID )
			{
				case SKILLID_PoisonAttribute:
					iaResistance[5] = ((int *)0x00992850)[psSkill->iLevel - 1];
					break;

				case SKILLID_IceAttribute:
					iaResistance[3] = ((int *)0x00997580)[psSkill->iLevel - 1];
					break;

				case SKILLID_FireAttribute:
					iaResistance[2] = ((int *)0x00995258)[psSkill->iLevel - 1];
					break;

				case SKILLID_MeleeMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iMinDamageSkill = (((int *)0x00995230)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMin) / 100;
						sCharacterDataLocal.iMaxDamageSkill = (((int *)0x00995230)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMax) / 100;
					}
					break;

				case SKILLID_ShootingMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iMinDamageSkill = (((int *)0x00993C28)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMin) / 100;
						sCharacterDataLocal.iMaxDamageSkill = (((int *)0x00993C28)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMax) / 100;
					}
					break;

				case SKILLID_WeaponDefenseMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) && IsTwoHandedWeapon() )
					{
						sCharacterDataLocal.fBlockRating += (float)((float)((int *)0x009977D8)[psSkill->iLevel - 1] * sCharacterDataLocal.iTempBlockRating) / 100.0f;
					}
					break;

				case SKILLID_DionsEye:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iAttackRatingSkill += (((int *)0x009977D8)[psSkill->iLevel - 1] * sCharacterDataLocal.iTempAttackRating) / 100;
					}
					break;

				case SKILLID_ThrowingMaster:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iMinDamageSkill = (((int *)0x009948D0)[psSkill->iLevel - 1] * sCharacterDataLocal.iMinDamage) / 100;
						sCharacterDataLocal.iMaxDamageSkill = (((int *)0x009948D0)[psSkill->iLevel - 1] * sCharacterDataLocal.iMaxDamage) / 100;
					}
					break;

				case SKILLID_MechanicWeaponMastery:
				{
					auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex - 1];
					if ( (pcItemData->sItem.eSpecialization & UNITDATA->sCharacterData.iFlag) || (pcItemData->sItem.iItemUniqueID == 2) )
					{
						sCharacterDataLocal.iMinDamageSkill = (((int *)0x00992B98)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMin) / 100;
						sCharacterDataLocal.iMaxDamageSkill = (((int *)0x00992B98)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMax) / 100;
					}
				}
				break;

				case SKILLID_CriticalMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
						sCharacterDataLocal.iCriticalSkills += (((int *)0x00997A30)[psSkill->iLevel - 1] * sCharacterDataLocal.iCritical) / 100;
					break;

				case SKILLID_SwordMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iMinDamageSkill = (((int *)0x00993598)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMin) / 100;
						sCharacterDataLocal.iMaxDamageSkill = (((int *)0x00993598)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMax) / 100;
					}
					break;

				case SKILLID_DualWieldMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iMinDamageSkill = (((int *)0x00998020)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMin) / 100;
						sCharacterDataLocal.iMaxDamageSkill = (((int *)0x00998020)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.fTempDamageMax) / 100;
					}
					break;

				case SKILLID_AttackMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						sCharacterDataLocal.iAttackRatingSkill += (((int *)0x00998228)[psSkill->iLevel - 1] * (int)sCharacterDataLocal.iTempAttackRating) / 100;
						sCharacterDataLocal.fBlockRating += (float)((float)((int *)0x00998200)[psSkill->iLevel - 1] * sCharacterDataLocal.iTempBlockRating) / 100.0f;
					}
					break;

				case SKILLID_FatalMastery:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
						sCharacterDataLocal.iCriticalSkills += (((int *)0x00998458)[psSkill->iLevel - 1] * sCharacterDataLocal.iCritical) / 100;
					break;

				default:
					break;
			}
		}
	}

	for ( int i = 0; i < _countof( CharacterData::sElementalDef ); i++ )
		UNITDATA->sCharacterData.sElementalDef[i] = iaResistance[i] + sCharacterDataLocal.iaResistance[i];

	return true;
}

bool CGameCharacterStatus::AddSkillStatusPost( CharacterDataLocal & sCharacterDataLocal )
{
	IMinMax * psDamageFake = ((IMinMax *)0x035EB600);

	auto HasItemRequiredSkill = []( Skill * psSkill, int iItemIndex ) -> bool
	{
		bool bRet = false;

		if ( INVENTORYITEMSLOT[iItemIndex].iItemIndex )
		{
			auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[iItemIndex].iItemIndex - 1];

			for ( int i = 0; i < _countof( SkillBase::eSkillItemAllowed ); i++ )
			{
				if ( psSkill->sSkillInfo.eSkillItemAllowed[i] == pcItemData->sItem.sItemID.ToItemType() )
				{
					bRet = true;

					break;
				}
			}
		}

		return bRet;
	};

	for ( int i = 1; i <= 20; i++ )
	{
		auto psSkill = SKILLSCHARACTER + i;

		if ( psSkill->bActive && psSkill->iLevel )
		{
			switch ( psSkill->iID )
			{
				case SKILLID_PhysicalTraining:
				{
					float fValue = (float)((int *)0x00993318)[psSkill->iLevel - 1];
					float fTrain = (fValue * (float)UNITDATA->sCharacterData.sSP.sMax) / 100.0f;

					UNITDATA->sCharacterData.sSP.sMax += fTrain;
				}
				break;

				case SKILLID_FireJavelin:
				{
					ElementalAttackData * psElementalAttack = (ElementalAttackData *)0x035E5BB8;

					IMinMax * psFire = ((IMinMax *)0x00994AB0);

					psElementalAttack->sFire.iMin += psFire[psSkill->iLevel - 1].iMin;
					psElementalAttack->sFire.iMax += psFire[psSkill->iLevel - 1].iMax;
				}
				break;

				case SKILLID_Meditation:
					UNITDATA->sCharacterData.fMPRegen += ((float *)0x00996AE0)[psSkill->iLevel - 1];
					break;

				case SKILLID_MentalMastery:
				{
					float fValue = (float)iaMageT21MPCapacityBoost[psSkill->iLevel - 1];
					float fTrain = (fValue * (float)UNITDATA->sCharacterData.sMP.sMax) / 100.0f;

					UNITDATA->sCharacterData.sMP.sMax += (short)fTrain;
				}
				break;

				case SKILLID_BoostHealth:
					UNITDATA->sCharacterData.sHP.sMax += ((int *)0x00995938)[psSkill->iLevel - 1];
					break;

				case SKILLID_InnerPeace:
				{
					float fValue = (float)((int *)0x009989A8)[psSkill->iLevel - 1];
					float fTrain = (fValue * (float)UNITDATA->sCharacterData.sMP.sMax) / 100.0f;

					UNITDATA->sCharacterData.sMP.sMax += fTrain;
				}
				break;


				default:
					break;
			}
		}
	}

	SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Defense, CHARSTATUSCOLOR_White );

	for ( int i = 0; i < MAX_SKILLS_TIMER; i++ )
	{
		Skill * psSkill = TIMERSKILLHANDLER->GetPointerSkillTimer() + i;
		if ( psSkill->bActive )
		{
			switch ( psSkill->iID )
			{
				case SKILLID_DrasticSpirit:
				{
					float fValue = (float)((int *)0x00993520)[psSkill->iLevel - 1];
					UNITDATA->sCharacterData.iDefenseRating += (int)((float)UNITDATA->sCharacterData.iDefenseRating * fValue) / 100.0f;
					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Defense, CHARSTATUSCOLOR_Blue );
				}
				break;

				case SKILLID_SparkShield:
					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Defense, CHARSTATUSCOLOR_Blue );
					break;

				case SKILLID_Concentration:
					if ( HasItemRequiredSkill( psSkill, ITEMSLOT_RightHand - 1 ) )
					{
						int iValue = ((int *)0x00995578)[psSkill->iLevel - 1];
						UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * iValue) / 100;

						SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_AttackRating, CHARSTATUSCOLOR_Orange );
					}
					break;

				case SKILLID_Force:
				{
					if ( (psSkill->iLevel > 0) && (psSkill->iLevel <= _countof( ForceDamageTable )) )
					{
						if ( ForceDamageTable[psSkill->iLevel - 1] > 0 )
						{
							int iValue = (UNITDATA->sCharacterData.iMinDamage + UNITDATA->sCharacterData.iMaxDamage) / 2;

							psDamageFake->iMin += (ForceDamagePercentTable[psSkill->iLevel - 1] * iValue) / 100;
							psDamageFake->iMax += (ForceDamagePercentTable[psSkill->iLevel - 1] * iValue) / 100;

							psDamageFake->iMin += ForceDamageTable[psSkill->iLevel - 1];
							psDamageFake->iMax += ForceDamageTable[psSkill->iLevel - 1];
						}
					}
				}
				break;

				case SKILLID_PowerSiege:
					if ( MAP_ID != MAPID_Bellatra )
					{
						psDamageFake->iMin += (UNITDATA->sCharacterData.iMinDamage * 10) / 100;
						psDamageFake->iMax += (UNITDATA->sCharacterData.iMaxDamage * 10) / 100;
					}
					break;

				case SKILLID_RegenerationField:
					UNITDATA->sCharacterData.fMPRegen += ((float *)0x00996F68)[psSkill->iLevel - 1] / psSkill->bActive;
					UNITDATA->sCharacterData.fHPRegen += ((float *)0x00996F40)[psSkill->iLevel - 1] / psSkill->bActive;
					break;

				case SKILLID_Berserker:
					UNITDATA->sCharacterData.iAbsorbRating += (float)((((int *)0x009957D0)[psSkill->iLevel - 1] * UNITDATA->sCharacterData.iAbsorbRating) / 100);

					SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Damage, CHARSTATUSCOLOR_Gold );
					break;

				case SKILLID_Maximize:
				{
					if ( CGameCharacterStatus::UPDATE_BUFFSKILLS )
						PROCESSSKILLBUFFEX( SKILLPLAYID_T22, psSkill->iLevel );

					psDamageFake->iMin += (UNITDATA->sCharacterData.iMinDamage * (float)psSkill->PlusState[0]) / 100.0f;
					psDamageFake->iMax += (UNITDATA->sCharacterData.iMaxDamage * (float)psSkill->PlusState[0]) / 100.0f;

					UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * ((int *)0x04B0C034)[psSkill->iLevel - 1]) / 100;
				}
				break;

				case SKILLID_PhysicalAbsorb:
				{
					int * piaSkillTable = (int *)0x009927B0;
					UNITDATA->sCharacterData.iAbsorbRating += (UNITDATA->sCharacterData.iAbsorbRating * piaSkillTable[psSkill->iLevel - 1]) / 100;
				}
				break;

				case SKILLID_MetalArmor:
				{
					if ( bMetalArmorSkillActive )
					{
						SETSTATUSCOLOR( CHARSTATUSTYPECOLOR_Defense, CHARSTATUSCOLOR_Blue );
						if ( int iLevelSkill = SKILLMANAGERHANDLER->GetLevelSkill( SKILLID_PhysicalAbsorb ); iLevelSkill > 0 )
						{
							int * piaSkillTable = (int *)0x009927B0;
							UNITDATA->sCharacterData.iAbsorbRating += (UNITDATA->sCharacterData.iAbsorbRating * piaSkillTable[iLevelSkill - 1]) / 100;
						}
					}
				}
				break;

				case SKILLID_BellatraHonor:
				{
					if ( (MAP_ID != MAPID_Bellatra) && (MAP_ID != MAPID_BlessCastle) )
					{
						int iAddAttackRating = 1;
						switch ( (PacketHonor::EHonorType)psSkill->PlusState[0] )
						{
							case PacketHonor::EHonorType::HONORTYPE_BellatraBronze:
								iAddAttackRating = 3;
								break;
							case PacketHonor::EHonorType::HONORTYPE_BellatraSilver:
								iAddAttackRating = 5;
								break;
							case PacketHonor::EHonorType::HONORTYPE_BellatraGold:
								iAddAttackRating = 7;
								break;
						}

						UNITDATA->sCharacterData.iAttackRating += (UNITDATA->sCharacterData.iAttackRating * iAddAttackRating) / 100;
					}
				}
				break;

				case SKILLID_PvPHonor:
				{
					if ( (MAP_ID != MAPID_Bellatra) && (MAP_ID != MAPID_BlessCastle) )
					{
						int iAddAttackPower = 1;
						switch ( (PacketHonor::EHonorType)psSkill->PlusState[0] )
						{
							case PacketHonor::EHonorType::HONORTYPE_PvPBronze:
								iAddAttackPower = 3;
								break;
							case PacketHonor::EHonorType::HONORTYPE_PvPSilver:
								iAddAttackPower = 5;
								break;
							case PacketHonor::EHonorType::HONORTYPE_PvPGold:
								iAddAttackPower = 7;
								break;
						}

						psDamageFake->iMin += (UNITDATA->sCharacterData.iMinDamage * (float)iAddAttackPower) / 100.0f;
						psDamageFake->iMax += (UNITDATA->sCharacterData.iMaxDamage * (float)iAddAttackPower) / 100.0f;
					}
				}
				break;

				default:
					break;
			}
		}
	}

	WRITEDWORD( 0x035EB608, sCharacterDataLocal.iTempAbsorption );

	return false;
}

bool CGameCharacterStatus::HasRequirementItem( ItemData * pcItemData )
{
	bool bCanUseItem = true;

	if ( (pcItemData->sItem.iLevel > UNITDATA->sCharacterData.iLevel) ||
		(pcItemData->sItem.iStrength > UNITDATA->sCharacterData.iStrength) ||
		(pcItemData->sItem.iSpirit > UNITDATA->sCharacterData.iSpirit) ||
		(pcItemData->sItem.iTalent > UNITDATA->sCharacterData.iTalent) ||
		(pcItemData->sItem.iAgility > UNITDATA->sCharacterData.iAgility)||
		(pcItemData->sItem.iHealth > UNITDATA->sCharacterData.iHealth) )
		bCanUseItem = false;

	if ( bCanUseItem )
	{
		if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_ForceOrb )
		{
			int iForceID = (pcItemData->sItem.sItemID.ToInt() & 0x0000FFFF) >> 8;

			IMinMax * psForceLevel = &((IMinMax *)0x0082D7C0)[iForceID - 1];

			//Force? TODO LEVEL
			if ( (UNITDATA->sCharacterData.iLevel < psForceLevel->iMin) || (UNITDATA->sCharacterData.iLevel > psForceLevel->iMax) )
				bCanUseItem = false;
		}
	}

	return bCanUseItem;
}

bool CGameCharacterStatus::UpdateHasRequirementItem()
{
	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		//Valid Item?
		auto pcItemData = INVENTORYITEMS + i;
		if ( pcItemData->bValid )
		{
			pcItemData->sItem.bCanNotUse = FALSE;

			if ( HasRequirementItem( pcItemData ) == false )
			{
				pcItemData->sItem.bCanNotUse = TRUE;
				continue;
			}
		}
	}

	return false;
}

BOOL CGameCharacterStatus::CharacterStatusVerify( CharacterData * psCharacterData, CharacterData * psCharacterDataDest )
{
	return (BOOL)CALLT_WITH_ARG2( 0x004AF600, 0x035EBB20, (DWORD)psCharacterData, (DWORD)psCharacterDataDest );
}
