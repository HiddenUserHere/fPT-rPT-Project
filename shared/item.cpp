#include "stdafx.h"
#include "item.h"

BOOL ItemData::IsOneHandedWeapon()
{
	if( (iItemSlotFlag & ITEMSLOTFLAG_RightHand) || (iItemSlotFlag & ITEMSLOTFLAG_CostumeRight) )
	{
		if( (iItemSlotFlag & ITEMSLOTFLAG_LeftHand) || (iItemSlotFlag & ITEMSLOTFLAG_CostumeLeft) )
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

BOOL ItemData::IsTwoHandedWeapon()
{
	if ( ((iItemSlotFlag & ITEMSLOTFLAG_RightHand) && (iItemSlotFlag & ITEMSLOTFLAG_LeftHand)) ||
		 ((iItemSlotFlag & ITEMSLOTFLAG_CostumeRight) && (iItemSlotFlag & ITEMSLOTFLAG_CostumeLeft)) )
		return TRUE;

	return FALSE;
}