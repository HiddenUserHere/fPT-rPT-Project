#include "stdafx.h"
#include "Coin.h"

int Base::Coin::GetLevelItemEachAge( Item * psItem )
{
	int iLevel = 0;

	if ( psItem->sAgeLevel > 0 )
	{
		iLevel = 2;

		//New Sets
		switch ( psItem->sItemID.ToItemID() )
		{
			case ITEMID_AbyssAxe:
			case ITEMID_AbyssClaw:
			case ITEMID_AbyssHammer:
			case ITEMID_AbyssStaff:
			case ITEMID_AbyssScythe:
			case ITEMID_AbyssBow:
			case ITEMID_AbyssSword:
			case ITEMID_AbyssJavelin:
			case ITEMID_AbyssPhantom:
			case ITEMID_AbyssDagger:
			case ITEMID_AbyssShield:
			case ITEMID_AbyssOrb:
			case ITEMID_AbyssArmor:
			case ITEMID_AbyssRobe:
				iLevel = 3;
				break;

			default:
				break;
		}
	}

	return iLevel;
}

int Base::Coin::GetPerfectizePrice( Item * psItem )
{
	int iCoins = 0;

	int iLevelItem = psItem->iLevel;
	if ( psItem->eCraftType == ITEMCRAFTTYPE_Aging && psItem->sAgeLevel > 0 )
	{
		int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );
		
		if ( iLevelAge > 0 )
			iLevelItem -= (psItem->sAgeLevel / iLevelAge);
	}

	//Weapon?
	if ( psItem->sItemID.ToItemBase() == ITEMBASE_Weapon )
	{
		if ( iLevelItem <= 122 )
			iCoins = 250;
		else if ( iLevelItem == 126 )
			iCoins = 600;
		else if ( iLevelItem == 130 )
			iCoins = 800;
		else if ( iLevelItem == 134 )
			iCoins = 1000;
		else if ( iLevelItem == 138 )
			iCoins = 1500;
	}
	else if ( psItem->sItemID.ToItemBase() == ITEMBASE_Defense || psItem->sItemID.ToItemBase() == ITEMBASE_Accessory )
	{
		if ( psItem->sItemID.ToItemType() == ITEMTYPE_Armor ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Robe ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Shield ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Orb )
		{
			if ( iLevelItem <= 122 )
				iCoins = 250;
			else if ( iLevelItem == 126 )
				iCoins = 600;
			else if ( iLevelItem == 130 )
				iCoins = 800;
			else if ( iLevelItem == 134 )
				iCoins = 1000;
			else if ( iLevelItem == 138 )
				iCoins = 1500;
		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Boots ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Gauntlets ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Bracelets )
		{
			if ( psItem->sItemID.ToItemID() == ITEMID_DraxosBoots )
				iCoins = 2500;
			else if ( psItem->sItemID.ToItemID() == ITEMID_MokovaBoots )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_GhostGauntlets )
				iCoins = 1250;
			else if ( (psItem->sItemID.ToItemID() == ITEMID_GhostBracelets) || (psItem->sItemID.ToItemID() == ITEMID_TropicalBracelets) )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_GhostBoots )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_ArgonianGauntlets )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_GhostCastleBracelets )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_GreedyGauntlets )
				iCoins = 1500;
			else if ( iLevelItem <= 118 )
				iCoins = 200;
			else if ( iLevelItem == 122 )
				iCoins = 250;
			else if ( iLevelItem == 126 )
				iCoins = 500;
			else if ( iLevelItem == 130 )
				iCoins = 800;
			else if ( iLevelItem == 134 )
				iCoins = 1000;
			else if ( iLevelItem == 138 )
				iCoins = 1200;
		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Ring ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Ring2 ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Sheltom ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Amulet )
		{
			if (psItem->sItemID.ToItemID() == ITEMID_ThroneSheltom)
				iCoins = 2000;
			else if ( psItem->sItemID.ToItemID() == ITEMID_FurySheltom )
				iCoins = 1500;
			else if ( psItem->sItemID.ToItemID() == ITEMID_DarkGuardianSheltom )
				iCoins = 1000;
			else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Sheltom )
			{
				if ( iLevelItem <= 60 )
					iCoins = 15;
				else if ( iLevelItem == 70 )
					iCoins = 25;
				else if ( iLevelItem == 80 )
					iCoins = 38;
				else if ( iLevelItem == 90 )
					iCoins = 50;
				else if ( iLevelItem == 100 )
					iCoins = 75;
				else if ( iLevelItem == 110 )
					iCoins = 125;
			}
			else if ( psItem->sItemID.ToItemID() == ITEMID_ValentoRing )
				iCoins = 1250;
			else if ( psItem->sItemID.ToItemID() == ITEMID_DevilShyRing )
				iCoins = 1500;
			else if (psItem->sItemID.ToItemID() == ITEMID_ThroneRing )
				iCoins = 2000;
			else if ( psItem->sItemID.ToItemID() == ITEMID_KelvezuAmulet )
				iCoins = 1500;
			else if ( psItem->sItemID.ToItemID() == ITEMID_TullaAmulet )
				iCoins = 1500;
			else if ( psItem->sItemID.ToItemID() == ITEMID_ThroneAmulet )
				iCoins = 2000;
			else if ( iLevelItem <= 100 )
				iCoins = 125;
			else if ( iLevelItem == 110 )
				iCoins = 200;
			else if ( iLevelItem == 114 )
				iCoins = 300;
			else if ( iLevelItem == 118 )
				iCoins = 400;
			else if ( iLevelItem == 122 )
				iCoins = 500;
			else if ( iLevelItem == 126 )
				iCoins = 700;
		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Earring )
		{
			if ( iLevelItem <= 100 )
				iCoins = 700;
			else if ( iLevelItem == 110 )
				iCoins = 1000;
		}
		else if ( psItem->sItemID.ToItemID() == ITEMID_SpecStoneFS || psItem->sItemID.ToItemID() == ITEMID_SpecStoneMS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneAS || psItem->sItemID.ToItemID() == ITEMID_SpecStonePS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneATA || psItem->sItemID.ToItemID() == ITEMID_SpecStoneKS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneMG || psItem->sItemID.ToItemID() == ITEMID_SpecStonePRS ||
			psItem->sItemID.ToItemID() == ITEMID_SpecStoneASS || psItem->sItemID.ToItemID() == ITEMID_SpecStoneSHA )
		{
			iCoins = 400;
		}
	}

	return iCoins;
}

int Base::Coin::GetSwapperPrice( Item * psItem, int iItemType )
{
	int iCoins = 0;

	if ( (psItem->sItemID.ToItemID() == ITEMID_GhostGauntlets) || (psItem->sItemID.ToItemID() == ITEMID_GhostBracelets) ||
		(psItem->sItemID.ToItemID() == ITEMID_GhostBoots) || (psItem->sItemID.ToItemID() == ITEMID_TropicalBracelets) )
		return 0;

	//Lambda
	auto GetSwapCategoryPrice = []( int iLevel )->int
	{
		int iAdd = 0;
		
		if ( iLevel <= 122 )
			iAdd = 400;
		else if ( iLevel == 126 )
			iAdd = 600;
		else if ( iLevel == 130 )
			iAdd = 900;
		else if ( iLevel == 134 )
			iAdd = 1500;

		return iAdd;
	};

	int iLevelItem = GetRealItemLevel( psItem );

	//Weapon?
	if ( psItem->sItemID.ToItemBase() == ITEMBASE_Weapon )
	{
		if ( iLevelItem <= 122 )
			iCoins = 250;
		else if ( iLevelItem == 126 )
			iCoins = 350;
		else if ( iLevelItem == 130 )
			iCoins = 700;
		else if ( iLevelItem == 134 )
			iCoins = 1000;

		if ( iCoins > 0 && psItem->sItemID.ToItemBase() != (iItemType & 0xFF000000) )
		{
			iCoins = GetSwapCategoryPrice( iLevelItem );
		}
	}
	else if ( psItem->sItemID.ToItemBase() == ITEMBASE_Defense || psItem->sItemID.ToItemBase() == ITEMBASE_Accessory )
	{
		if ( psItem->sItemID.ToItemType() == ITEMTYPE_Armor ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Robe ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Shield ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Orb )
		{
			if ( iLevelItem <= 122 )
				iCoins = 250;
			else if ( iLevelItem == 126 )
				iCoins = 350;
			else if ( iLevelItem == 130 )
				iCoins = 700;
			else if ( iLevelItem == 134 )
				iCoins = 1000;

			if ( iCoins > 0 && (psItem->sItemID.ToItemBase() != (iItemType & 0xFF000000) || iItemType == ITEMTYPE_Boots || iItemType == ITEMTYPE_Gauntlets) )
			{
				if ( ((iItemType != ITEMTYPE_Armor) && (iItemType != ITEMTYPE_Robe) && (iItemType != ITEMTYPE_Shield) && (iItemType != ITEMTYPE_Orb)) )
					iCoins = GetSwapCategoryPrice( iLevelItem );
			}

		}
		else if ( psItem->sItemID.ToItemType() == ITEMTYPE_Boots ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Gauntlets ||
			psItem->sItemID.ToItemType() == ITEMTYPE_Bracelets )
		{
			if ( iLevelItem <= 122 )
				iCoins = 250;
			else if ( iLevelItem == 126 )
				iCoins = 350;
			else if ( iLevelItem == 130 )
				iCoins = 700;
			else if ( iLevelItem == 134 )
				iCoins = 1000;

			if ( iCoins > 0 && (psItem->sItemID.ToItemBase() != (iItemType & 0xFF000000) || 
				iItemType == ITEMTYPE_Armor ||
				iItemType == ITEMTYPE_Shield ||
				iItemType == ITEMTYPE_Orb ||
				iItemType == ITEMTYPE_Robe) )
			{
				iCoins = GetSwapCategoryPrice( iLevelItem );
			}
		}
	}

	return iCoins;
}

int Base::Coin::GetAgingRecoveryPrice( Item * psItem )
{
	int iPrice = 800;

	int iLevel = Base::Coin::GetRealItemLevel( psItem );

	switch ( iLevel )
	{
		case 138:
			iPrice = 800;
			break;
		case 134:
			iPrice = 600;
			break;
		case 130:
			iPrice = 400;
			break;

		//Below 122
		default:
			iPrice = 200;
			break;
	}

	return iPrice;
}

int Base::Coin::GetRealItemLevel( Item * psItem )
{
	int iLevelItem = psItem->iLevel;
	if ( psItem->eCraftType == ITEMCRAFTTYPE_Aging && psItem->sAgeLevel > 0 )
	{
		int iLevelAge = Base::Coin::GetLevelItemEachAge( psItem );

		if ( iLevelAge > 0 )
			iLevelItem -= (psItem->sAgeLevel / iLevelAge);
	}

	return iLevelItem;
}

int Base::Coin::GetLevelUPPrice( int iLevelOld, int iNewLevel )
{
	static int iaLevelPrice[MAX_LEVEL_COINSHOP] =
	{
		100,		//1
		100,		//2
		100,		//3
		100,		//4
		100,		//5
		100,		//6
		100,		//7
		100,		//8
		100,		//9
		100,		//10
		100,		//11
		100,		//12
		100,		//13
		100,		//14
		100,		//15
		100,		//16
		100,		//17
		100,		//18
		100,		//19
		100,		//20
		100,		//21
		100,		//22
		100,		//23
		100,		//24
		100,		//25
		100,		//26
		100,		//27
		100,		//28
		100,		//29
		100,		//30
		100,		//31
		100,		//32
		100,		//33
		100,		//34
		100,		//35
		100,		//36
		100,		//37
		100,		//38
		100,		//39
		100,		//40
		100,		//41
		100,		//42
		100,		//43
		100,		//44
		100,		//45
		100,		//46
		100,		//47
		100,		//48
		100,		//49
		100,		//50
		100,		//51
		100,		//52
		100,		//53
		100,		//54
		100,		//55
		100,		//56
		100,		//57
		100,		//58
		100,		//59
		100,		//60
		100,		//61
		100,		//62
		100,		//63
		100,		//64
		100,		//65
		100,		//66
		100,		//67
		100,		//68
		100,		//69
		100,		//70
		100,		//71
		100,		//72
		100,		//73
		100,		//74
		100,		//75
		100,		//76
		100,		//77
		100,		//78
		100,		//79
		100,		//80
		100,		//81
		100,		//82
		100,		//83
		100,		//84
		100,		//85
		100,		//86
		100,		//87
		100,		//88
		100,		//89
		100,		//90
		100,		//91
		100,		//92
		100,		//93
		100,		//94
		100,		//95
		100,		//96
		100,		//97
		100,		//98
		100,		//99
		100,		//100
		100,		//101
		100,		//102
		100,		//103
		100,		//104
		100,		//105
		100,		//106
		100,		//107
		100,		//108
		100,		//109
		100,		//110
		100,		//111
		100,		//112
		100,		//113
		100,		//114
		100,		//115
		100,		//116
		100,		//117
		100,		//118
		100,		//119
		150,		//120
		150,		//121
		150,		//122
		150,		//123
		150,		//124
		150,		//125
		150,		//126
		150,		//127
		150,		//128
		150,		//129
		250,		//130
		250,		//131
		250,		//132
		250,		//133
		250,		//134
		250,		//135
		250,		//136
		250,		//137
		250,		//138
		250,		//139
		250,		//140
	};

	if ( ((iLevelOld > 0) && (iLevelOld < MAX_LEVEL_COINSHOP)) && ((iNewLevel > iLevelOld) && (iNewLevel <= MAX_LEVEL_COINSHOP)) )
		return (iaLevelPrice[iNewLevel - 1] * iNewLevel) - (((iaLevelPrice[iLevelOld - 1] * iLevelOld) * 35) / 100);

	return 0;
}

bool Base::Coin::CanLevelUP( int iLevel )
{
	return ((iLevel >= 110) && (iLevel < MAX_LEVEL_COINSHOP));
}
