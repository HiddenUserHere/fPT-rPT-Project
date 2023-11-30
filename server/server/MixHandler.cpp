#include "stdafx.h"
#include "MixHandler.h"


CMixHandler::CMixHandler()
{
}


CMixHandler::~CMixHandler()
{
}

void CMixHandler::SetMixStatus( Item * psItem, int iIndex )
{
	MixServerData * ps = ((MixServerData*)0x006BB6B0) + iIndex;

	bool bAddStatus = true;

	if ( psItem->bCostumeItem )
		bAddStatus = false;

	auto pcItemData = ITEMSERVER->FindItemPointerTable( psItem->sItemID.ToInt() );
	if ( pcItemData )
		if ( pcItemData->iItemSlotFlag == ITEMSLOTFLAG_Costume )
			bAddStatus = false;

	for ( int i = 0; i < 8; i++ )
	{
		int iType = ps->iaTypeAttributte[i];

		if ( bAddStatus )
		{
			switch ( iType )
			{
				case MIXATTRTYPE_Fire:
					psItem->saElementalDef[2] += (int)ps->faAttributte[i];
					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Ice:
					psItem->saElementalDef[3] += (int)ps->faAttributte[i];
					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Lightning:
					psItem->saElementalDef[4] += (int)ps->faAttributte[i];
					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Poison:
					psItem->saElementalDef[5] += (int)ps->faAttributte[i];
					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Organic:
					psItem->saElementalDef[0] += (int)ps->faAttributte[i];
					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Critical:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->iCritical += (int)ps->faAttributte[i];
					else
						psItem->iCritical += (psItem->iCritical * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_AttackRating:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->iAttackRating += (int)ps->faAttributte[i];
					else
						psItem->iAttackRating += (psItem->iAttackRating * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_DamageMin:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->sDamage.sMin += (int)ps->faAttributte[i];
					else
						psItem->sDamage.sMin += (psItem->sDamage.sMin * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_DamageMax:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->sDamage.sMax += (int)ps->faAttributte[i];
					else
						psItem->sDamage.sMax += (psItem->sDamage.sMax * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_AttackSpeed:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->iAttackSpeed += (int)ps->faAttributte[i];
					else
						psItem->iAttackSpeed += (psItem->iAttackSpeed * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Absorption:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fAbsorbRating += ps->faAttributte[i];
					else
						psItem->fAbsorbRating += (psItem->fAbsorbRating * ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_Defense:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->iDefenseRating += (int)ps->faAttributte[i];
					else
						psItem->iDefenseRating += (psItem->iDefenseRating * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_BlockRating:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fBlockRating += ps->faAttributte[i];
					else
						psItem->fBlockRating += (psItem->fBlockRating * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_WalkSpeed:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fMovementSpeed += ps->faAttributte[i];
					else
						psItem->fMovementSpeed += (psItem->fMovementSpeed * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_HP:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fAddHP += ps->faAttributte[i];
					else
						psItem->fAddHP += (psItem->fAddHP * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_MP:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fAddMP += ps->faAttributte[i];
					else
						psItem->fAddMP += (psItem->fAddMP * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_SP:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fAddSP += ps->faAttributte[i];
					else
						psItem->fAddSP += (psItem->fAddSP * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_HPRegen:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fHPRegen += ps->faAttributte[i];
					else
						psItem->fHPRegen += (psItem->fHPRegen * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_MPRegen:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fMPRegen += ps->faAttributte[i];
					else
						psItem->fMPRegen += (psItem->fMPRegen * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_SPRegen:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->fSPRegen += ps->faAttributte[i];
					else
						psItem->fSPRegen += (psItem->fSPRegen * ps->faAttributte[i]) / 100.0f;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				case MIXATTRTYPE_PotionCount:
					if ( ps->iaAttributtePercent[i] == 1 )
						psItem->iPotionStorage += (int)ps->faAttributte[i];
					else
						psItem->iPotionStorage += (psItem->iPotionStorage * (int)ps->faAttributte[i]) / 100;

					psItem->eMixEffect |= (EMixEffect)iType;
					break;

				default:
					break;
			}
		}
		else
			psItem->eMixEffect |= (EMixEffect)iType;
	}

	psItem->eMixTypeName = (EMixTypeName)ps->iMixNameID;

	ITEMSERVER->ValidateItem( psItem );
}

void CMixHandler::SetClearMixStatus( Item * psItem, int iIndex )
{
	MixServerData * ps = ((MixServerData*)0x006BB6B0) + iIndex;

	for ( int i = 0; i < 8; i++ )
	{
		int iType = ps->iaTypeAttributte[i];

		switch ( iType )
		{
			case MIXATTRTYPE_Fire:
				psItem->saElementalDef[2] -= (int)ps->faAttributte[i];
				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Ice:
				psItem->saElementalDef[3] -= (int)ps->faAttributte[i];
				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Lightning:
				psItem->saElementalDef[4] -= (int)ps->faAttributte[i];
				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Poison:
				psItem->saElementalDef[5] -= (int)ps->faAttributte[i];
				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Organic:
				psItem->saElementalDef[0] -= (int)ps->faAttributte[i];
				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Critical:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->iCritical -= (int)ps->faAttributte[i];
				else
					psItem->iCritical -= (psItem->iCritical * 100) / ((int)ps->faAttributte[i] + 100);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_AttackRating:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->iAttackRating -= (int)ps->faAttributte[i];
				else
					psItem->iAttackRating -= (psItem->iAttackRating * 100) / ((int)ps->faAttributte[i] + 100);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_DamageMin:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->sDamage.sMin -= (int)ps->faAttributte[i];
				else
					psItem->sDamage.sMin -= (psItem->sDamage.sMin * 100) / ((int)ps->faAttributte[i] + 100);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_DamageMax:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->sDamage.sMax -= (int)ps->faAttributte[i];
				else
					psItem->sDamage.sMax += (psItem->sDamage.sMax * 100) / ((int)ps->faAttributte[i] + 100);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_AttackSpeed:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->iAttackSpeed -= (int)ps->faAttributte[i];
				else
					psItem->iAttackSpeed -= (psItem->iAttackSpeed * (int)ps->faAttributte[i]);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Absorption:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fAbsorbRating -= ps->faAttributte[i];
				else
					psItem->fAbsorbRating -= (psItem->fAbsorbRating * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_Defense:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->iDefenseRating -= (int)ps->faAttributte[i];
				else
					psItem->iDefenseRating -= (psItem->iDefenseRating * 100) / ((int)ps->faAttributte[i] + 100);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_BlockRating:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fBlockRating -= ps->faAttributte[i];
				else
					psItem->fBlockRating -= (psItem->fBlockRating * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_WalkSpeed:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fMovementSpeed -= ps->faAttributte[i];
				else
					psItem->fMovementSpeed -= (psItem->fMovementSpeed * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_HP:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fAddHP -= ps->faAttributte[i];
				else
					psItem->fAddHP -= (psItem->fAddHP * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_MP:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fAddMP -= ps->faAttributte[i];
				else
					psItem->fAddMP -= (psItem->fAddMP * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_SP:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fAddSP -= ps->faAttributte[i];
				else
					psItem->fAddSP -= (psItem->fAddSP * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_HPRegen:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fHPRegen -= ps->faAttributte[i];
				else
					psItem->fHPRegen -= (psItem->fHPRegen * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_MPRegen:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fMPRegen -= ps->faAttributte[i];
				else
					psItem->fMPRegen -= (psItem->fMPRegen * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_SPRegen:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->fSPRegen -= ps->faAttributte[i];
				else
					psItem->fSPRegen -= (psItem->fSPRegen * 100.0f) / (ps->faAttributte[i] + 100.0f);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			case MIXATTRTYPE_PotionCount:
				if ( ps->iaAttributtePercent[i] == 1 )
					psItem->iPotionStorage -= (int)ps->faAttributte[i];
				else
					psItem->iPotionStorage -= (psItem->iPotionStorage * (int)ps->faAttributte[i]);

				psItem->eMixEffect |= (EMixEffect)iType;
				break;

			default:
				break;
		}
	}

	ITEMSERVER->ValidateItem( psItem );
}

int CMixHandler::GetTypeMixByItemID( int iItemID )
{
	int iType = 0;

	if ( (iItemID & 0xFF000000) == ITEMBASE_Weapon )
	{
		iType = 1;

		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Phantom || (iItemID & 0xFFFF0000) == ITEMTYPE_Dagger )
			iType = 10;
	}

	if ( (iItemID & 0xFF000000) == ITEMBASE_Defense )
	{
		iType = 2;

		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Shield )
			iType = 3;
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Gauntlets )
			iType = 6;
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Boots )
			iType = 7;
	}

	if ( (iItemID & 0xFF000000) == ITEMBASE_Accessory )
	{
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Orb )
			iType = 4;
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Bracelets )
			iType = 5;
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Amulet )
			iType = 8;
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Ring )
			iType = 9;
		if ( (iItemID & 0xFFFF0000) == ITEMTYPE_Ring2 )
			iType = 9;
	}


	return iType;
}

int CMixHandler::GetIndexMixByNameID( int iMixNameID, int iItemID )
{
	int iType = GetTypeMixByItemID( iItemID );
	if ( iType != 0 )
	{
		for ( int i = 0; i < 300; i++ )
		{
			MixServerData * ps = ((MixServerData*)0x006BB6B0) + i;
			if ( ps->iType == iType && ps->iMixNameID == iMixNameID )
				return i;
		}
	}

	return 0;
}

void CMixHandler::SetItemMixByMixName( Item * psItem, int iMixNameID )
{
	int iType = GetTypeMixByItemID( psItem->sItemID.ToInt() );

	if ( iType != 0 )
	{
		for ( int i = 0; i < 300; i++ )
		{
			MixServerData * ps = ((MixServerData*)0x006BB6B0) + i;

			if ( ps->iType == iType && ps->iMixNameID == iMixNameID )
			{
				SetMixStatus( psItem, i );
				break;
			}
		}
	}
}

void CMixHandler::SetMixStatusNPC( Item * psItem, int iIndex )
{
	for ( int i = 0; i < 14; i++ )
	{
		int * piaSheltoms = ((int *)0x08B7096C);

		if ( piaSheltoms[i] )
			psItem->sMixID = i + 1;
	}

	SetMixStatus( psItem, iIndex );
}
