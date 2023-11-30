#include "stdafx.h"
#include "gamecharacterstatus.h"


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
	int iESP14 = psData->iAttackRatingSkill;
	psData->iAttackRatingSkill = READDWORD( 0x04B0DA60 );


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
		if ( MAP_ID != MAPID_Bellatra )
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
	PLAYERDATA->iDefenseRating = (int)(fAgility + (fTalent / 4.0f) + (fLevel * 1.5f) + psData->iDefenseSkills) + psData->iDefenseAdd;
	float fDefense = (float)PLAYERDATA->iDefenseRating;

	//Absorb formula with items that have formula div 2 ( 0.5 abs )
	psData->iCountAbsorbItems /= 2;
	PLAYERDATA->iAbsorbRating = (int)(((fDefense / 100.0f) + (fLevel / 10.0f) + psData->fAbsorbSkills + psData->fAbsorbAdd) + ((fStrength + fTalent) / 40) + psData->iCountAbsorbItems + 1);

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
	PLAYERDATA->iMovementSpeed = (int)(((fTalent + fHealth + fLevel + 60.0f) / 150.0f) - (fWeightCur / fWeightMax) + (psData->fMovementSpeed + psData->fMovementSpeedAdd) + 1);

	//HP Formula based on Class
	switch ( PLAYERDATA->iClass )
	{
		case CHARACTERCLASS_Fighter:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 2.4f) + (fStrength * 0.5f));
			break;

		case CHARACTERCLASS_Mechanician:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 1.8f) + (fStrength * 0.5f));
			break;

		case CHARACTERCLASS_Archer:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.8f) + (fHealth * 1.1f) + (fStrength * 0.4f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Pikeman:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 1.6f) + (fStrength * 0.4f));
			break;

		case CHARACTERCLASS_Atalanta:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.8f) + (fHealth * 1.1f) + (fStrength * 0.4f) + (fAgility * 0.1f));
			break;

		case CHARACTERCLASS_Knight:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.0f) + (fHealth * 1.7f) + (fStrength * 0.4f));
			break;

		case CHARACTERCLASS_Magician:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 1.1f) + (fSpirit * 0.2f));
			break;

		case CHARACTERCLASS_Priestess:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 1.6f) + (fSpirit * 0.3f));
			break;

		case CHARACTERCLASS_Assassin:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 2.1f) + (fHealth * 1.6f) + (fStrength * 0.5f));
			break;

		case CHARACTERCLASS_Shaman:
			PLAYERDATA->sHP.sMax = (short)((fLevel * 1.5f) + (fHealth * 1.1f) + (fSpirit * 0.2f));
			break;

		default:
			break;
	}

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
	PLAYERDATA->iAttackSpeed = psData->iWeaponSpeed + psData->iAttackSpeedAdd + psData->iSkillWeaponSpeed;

#endif

	psData->iAttackRatingSkill = iESP14;
}

void CGameCharacterStatus::UpdateCharacterStatusItem( BOOL bCheckHack )
{
	ElementalAttackData * psElementalAttack = (ElementalAttackData*)0x035E5BB8;
	ZeroMemory( psElementalAttack, sizeof( ElementalAttackData ) );

	ItemData cItemWrapper;

	ZeroMemory( &cItemWrapper, sizeof( ItemData ) );

	CharacterDataLocal sCharacterDataLocal;

	//Get Inventory Items
	for ( int i = 0; i < INVENTORY_DATACOUNT; i++ )
	{
		//Valid Item?
		auto pcItemData = INVENTORYITEMS + i;
		if ( pcItemData->bValid )
		{
			//Using Item?
			if ( pcItemData->iItemSlot )
				AddStatusItem( sCharacterDataLocal, pcItemData );
		}
	}
}

bool CGameCharacterStatus::AddStatusItem( CharacterDataLocal & sCharacterDataLocal, ItemData * pcItemData )
{
	//Potion?
	if ( pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
		return false;

	//Can't use this Item?
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
	sCharacterDataLocal.iMinDamage += pcItemData->sItem.sDamage.sMin;
	sCharacterDataLocal.iMaxDamage += pcItemData->sItem.sDamage.sMax;
	sCharacterDataLocal.iCriticalItems += pcItemData->sItem.iCritical;
	sCharacterDataLocal.iDefenseAdd += pcItemData->sItem.iDefenseRating;

	sCharacterDataLocal.fAbsorbAdd += ((pcItemData->sItem.fAbsorbRating * 10.000001f) / 10.0f) + 0.000001f;

	if ( pcItemData->sItem.fAbsorbRating )
		sCharacterDataLocal.iCountAbsorbItems++;

	sCharacterDataLocal.fBlockRating += pcItemData->sItem.fBlockRating;
	sCharacterDataLocal.iWeaponSpeed += pcItemData->sItem.iAttackSpeed;
	sCharacterDataLocal.iAttackRange += pcItemData->sItem.iAttackRange;
	sCharacterDataLocal.fMagicMastery += pcItemData->sItem.fMagicMastery;
	sCharacterDataLocal.iLevelMP += pcItemData->sItem.sSpecData.iSpecAddMPDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAddMPDiv) : 0;
	sCharacterDataLocal.iLevelHP += pcItemData->sItem.sSpecData.iSpecAddHPDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAddHPDiv) : 0;
	sCharacterDataLocal.iLevelAttackRating += pcItemData->sItem.sSpecData.iSpecAttackRatingDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAttackRatingDiv) : 0;
	sCharacterDataLocal.iLevelDamage += pcItemData->sItem.sSpecData.iSpecAttackPowerDiv ? (UNITDATA->sCharacterData.iLevel / pcItemData->sItem.sSpecData.iSpecAttackPowerDiv) : 0;
	sCharacterDataLocal.fPercentMPAdd += pcItemData->sItem.sSpecData.fSpecMPRegen ? (pcItemData->sItem.sSpecData.fSpecMPRegen / 2.0f) : 0;
	sCharacterDataLocal.fPercentHPAdd += pcItemData->sItem.sSpecData.fSpecHPRegen ? (pcItemData->sItem.sSpecData.fSpecHPRegen / 2.0f) : 0;
	sCharacterDataLocal.fPercentSPAdd += pcItemData->sItem.sSpecData.fSpecSPRegen ? (pcItemData->sItem.sSpecData.fSpecSPRegen / 2.0f) : 0;


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

	for ( int i = 0; i < _countof( pcItemData->sItem.saElementalDef ); i++ )
		sCharacterDataLocal.iaResistance[i] += (int)pcItemData->sItem.saElementalDef[i];

	return true;
}

bool CGameCharacterStatus::AddSkillStatus( CharacterDataLocal & sCharacterDataLocal )
{
	bool bMetalArmor = false;

	//Weapon Add
	if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
	{
		ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex;
		if ( pcItemData->bValid )
		{
			sCharacterDataLocal.fTempDamageMin = (float)pcItemData->sItem.sDamage.sMin;
			sCharacterDataLocal.fTempDamageMax = (float)pcItemData->sItem.sDamage.sMax;

			sCharacterDataLocal.iTempAttackRating = pcItemData->sItem.iAttackRating;
			
			sCharacterDataLocal.iTempWeaponSpeed = pcItemData->sItem.iAttackSpeed;

			if ( UNITDATA->sCharacterData.iFlag & pcItemData->sItem.eSpecialization )
				sCharacterDataLocal.iTempWeaponSpeed += pcItemData->sItem.sSpecData.iSpecAttackSpeed;

			sCharacterDataLocal.fTempCritical = (float)pcItemData->sItem.sSpecData.iSpecCritical;
		}
	}

	if ( INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex )
	{
		ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_Armor - 1].iItemIndex;
		if ( pcItemData->bValid )
		{
			sCharacterDataLocal.iTempDefense = pcItemData->sItem.iDefenseRating;

			if ( UNITDATA->sCharacterData.iFlag & pcItemData->sItem.eSpecialization )
			{
				sCharacterDataLocal.iTempDefense += pcItemData->sItem.sSpecData.iSpecDefenseRating;

				bMetalArmor = true;
			}
		}
	}

	if ( INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex )
	{
		ItemData * pcItemData = INVENTORYITEMS + INVENTORYITEMSLOT[ITEMSLOT_LeftHand - 1].iItemIndex;
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
						int * piaSkillTable = (int *)0x00992698;
						sCharacterDataLocal.iTempBlockRating += (UNITDATA->sCharacterData.iBlockRating * piaSkillTable[psSkill->iLevel - 1]) / 100;
					}
				}
				break;

				case SKILLID_PhysicalAbsorb:
				{
					if ( ItemID( UNITDATA->sLeftHandTool.eItemID ).ToItemType() == ITEMTYPE_Shield )
					{
						int * piaSkillTable = (int *)0x009927B0;
						sCharacterDataLocal.iTempAbsorption += (UNITDATA->sCharacterData.iAbsorbRating * piaSkillTable[psSkill->iLevel - 1]) / 100;
					}
				}
				break;

				case SKILLID_Automation:
				{
					if ( INVENTORYITEMSLOT[ITEMSLOT_RightHand - 1].iItemIndex )
					{
						if ( ItemID( UNITDATA->sLeftHandTool.eItemID ).ToItemType() == ITEMTYPE_Shield )
						{
							int * piaSkillTable = (int *)0x009927B0;
							sCharacterDataLocal.iTempAbsorption += (UNITDATA->sCharacterData.iAbsorbRating * piaSkillTable[psSkill->iLevel - 1]) / 100;
						}
					}
				}
				break;

				default:
					break;
			}
		}
	}

	return true;
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
		//Force? TODO LEVEL
		if ( pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_ForceOrb )
		{
			//Don't has Level?
			for ( int i = 0; i < 15; i++ )
			{
				int iLevelMin = 1, iLevelMax = 10;

				if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_Bellum )
				{
					if ( (UNITDATA->sCharacterData.iLevel < iLevelMin) || (UNITDATA->sCharacterData.iLevel > iLevelMax) )
						bCanUseItem = false;

					break;
				}
			}
		}
	}

	return bCanUseItem;
}
