#include <Windows.h>

#define PREMIUM_ITEM_TOTAL 50

#define MAX_LEVEL_BEGINNERSBUFF		130

enum EItemTimerType
{
	ITEMTIMERTYPE_None,
	ITEMTIMERTYPE_ExpUp,
	ITEMTIMERTYPE_ThirdEye,
	ITEMTIMERTYPE_Phoenix,
	ITEMTIMERTYPE_PhoenixFire,
	ITEMTIMERTYPE_PhoenixIce,
	ITEMTIMERTYPE_PhoenixLightning,
	ITEMTIMERTYPE_PhoenixHeal,
	ITEMTIMERTYPE_MPDown,
	ITEMTIMERTYPE_SPDown,
	ITEMTIMERTYPE_HPUp,
	ITEMTIMERTYPE_MPUp,
	ITEMTIMERTYPE_BigHead,
	ITEMTIMERTYPE_WeightStone,
	ITEMTIMERTYPE_MatureStone,
	ITEMTIMERTYPE_SkillStone,
	ITEMTIMERTYPE_CartolaHat,
	ITEMTIMERTYPE_WitchHat,
	ITEMTIMERTYPE_SheepHat,
	ITEMTIMERTYPE_GiraffeHat,
	ITEMTIMERTYPE_SoccerHat,
	ITEMTIMERTYPE_XMasGreenHat,
	ITEMTIMERTYPE_XMasRedHat,
	ITEMTIMERTYPE_BigHeadHappiness,
	ITEMTIMERTYPE_BigHeadLove,
	ITEMTIMERTYPE_BigHeadSadness,
	ITEMTIMERTYPE_BigHeadShyness,
	ITEMTIMERTYPE_BigHeadAngry,
	ITEMTIMERTYPE_BigHeadSurprised,
	ITEMTIMERTYPE_BigHeadSensual,
	ITEMTIMERTYPE_BCBuff,
	ITEMTIMERTYPE_PumpkinHat,
	ITEMTIMERTYPE_CaravanHopy,
	ITEMTIMERTYPE_CaravanRudolph,
	ITEMTIMERTYPE_SkillCooldownReduceT1,
	ITEMTIMERTYPE_SkillCooldownReduceT2,
	ITEMTIMERTYPE_SkillCooldownReduceT3,
	ITEMTIMERTYPE_SkillCooldownReduceT4,
	ITEMTIMERTYPE_SkillCooldownReduceT5,
	ITEMTIMERTYPE_HPBooster,
	ITEMTIMERTYPE_MPBooster,
	ITEMTIMERTYPE_STMBooster,
	ITEMTIMERTYPE_CaravanArma,
	ITEMTIMERTYPE_CaravanRabie,
	ITEMTIMERTYPE_CaravanTobie,
	ITEMTIMERTYPE_CaravanCuepy,
	ITEMTIMERTYPE_CaravanHungky,
	ITEMTIMERTYPE_CaravanBuma,
	ITEMTIMERTYPE_HalloweenHat,
	ITEMTIMERTYPE_TuttiFruttiHat,
	ITEMTIMERTYPE_EasterHat,
	ITEMTIMERTYPE_ChemistHat,
	ITEMTIMERTYPE_PatronumHat,
	ITEMTIMERTYPE_CaravanSirKnight,
	ITEMTIMERTYPE_CaravanPingu,
	ITEMTIMERTYPE_CaravanYeti,
	ITEMTIMERTYPE_CaravanGoldenHopy,
	ITEMTIMERTYPE_GoldenHopyHat,
	ITEMTIMERTYPE_MountPinguMysterious,
	ITEMTIMERTYPE_ObscureHat,
};

enum
{
	ITEMTIMERTIME_1H							= (60*60),
	ITEMTIMERTIME_3H							= (60*60*3),
	ITEMTIMERTIME_6H							= (60*60*6),
	ITEMTIMERTIME_12H							= (60*60*12),
	ITEMTIMERTIME_24H							= (60*60*24),
	ITEMTIMERTIME_48H							= (60*60*48),
	ITEMTIMERTIME_72H							= (60*60*72),
	ITEMTIMERTIME_168H							= (60*60*168),

};


struct PremiumItem
{
	enum EItemTimerType							eType;
	DWORD										dwCode;
	DWORD										dwTimeLeft;
	BOOL										bRepeatUse;
	BOOL										bOnlyServerUse;
	DWORD										dwLevel;
	char *										pszName;
	char *										pszDescription;
	char *										pszImage;
	char *										pszImageBlack;
};



struct ItemPremium
{
	EItemTimerType								eType;
	DWORD										dwCode;
	DWORD										dwTimeLeft;
	DWORD										dwTimeTotal;
};
struct PacketItemPremiumData
{
	int											iSize;
	int											iHeader;

	ItemPremium									saItemList[PREMIUM_ITEM_TOTAL];
};


struct PacketActivePremium
{
	int											iLength;
	int											iHeader;
	ItemPremium									sPremium;
};


static BOOL ItemTimerCanStack( int iType )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_BCBuff:
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			bRet = TRUE;
			break;
	}

	return bRet;
}

static BOOL ItemTimerIsSame( int iType, int iTypeOther )
{
	BOOL bRet = FALSE;

	switch ( iType )
	{
		case ITEMTIMERTYPE_PhoenixFire:
		case ITEMTIMERTYPE_PhoenixIce:
		case ITEMTIMERTYPE_PhoenixLightning:
		case ITEMTIMERTYPE_PhoenixHeal:
			switch ( iTypeOther )
			{
				case ITEMTIMERTYPE_PhoenixFire:
				case ITEMTIMERTYPE_PhoenixIce:
				case ITEMTIMERTYPE_PhoenixLightning:
				case ITEMTIMERTYPE_PhoenixHeal:
					bRet = TRUE;
					break;
			}
			break;
	}

	return bRet;
}


