#include "stdafx.h"
#include "GameProtect.h"

#define CRC_REGEN_CODE		-1181.5f

RegenPointerData CGameProtect::saRegenData[5];
int CGameProtect::iRegenWheel;

float CGameProtect::fDecreaseMPData;
float CGameProtect::fDecreaseSPData;

CGameProtect::CGameProtect()
{
}


CGameProtect::~CGameProtect()
{
}

void CGameProtect::Update()
{
	//Old Regen Positiion
	int iOld = CGameProtect::iRegenWheel;

	//New Regen iItemSlot
	int iNew = iOld + 1;

	//Set New Regen Data
	CGameProtect::saRegenData[iNew % 5] = CGameProtect::saRegenData[iOld % 5];

	//Zero Old Regen Data
	CGameProtect::saRegenData[iOld % 5] = RegenPointerData{ 0.0f, 0.0f, 0.0f };

	//Next iItemSlot
	CGameProtect::iRegenWheel = iNew;
}

void CGameProtect::OnRegen()
{
	UnitData * pcCur		= UNITDATA;

	RegenPointerData * ps	= &CGameProtect::saRegenData[CGameProtect::iRegenWheel % 5];

	//Zero Regen Data
	ps->fHP = ps->fSP = ps->fMP = 0.0f;

	//Character Status needed to Regen
	float fLevel	= (float)pcCur->sCharacterData.iLevel;
	float fStrength = (float)pcCur->sCharacterData.iStrength;
	float fHealth	= (float)pcCur->sCharacterData.iHealth;
	float fSpirit	= (float)pcCur->sCharacterData.iSpirit;

#ifdef _NEW_DMG__

	if ( pcCur->psModelAnimation )
	{
		//Character Standing Mode?
		if ( pcCur->psModelAnimation->iType == ANIMATIONTYPE_Idle )
		{
			//Increase Stamina
			switch ( pcCur->sCharacterData.bSPType )
			{
				case 1:
					ps->fSP = (float)(3.8f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;

				case 2:
					ps->fSP = (float)(3.3f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;

				case 3:
					ps->fSP = (float)(2.9f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;

				default:
					ps->fSP = (float)(3.8f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;
			}
		}

		//Character Walking Mode?
		if ( pcCur->psModelAnimation->iType == ANIMATIONTYPE_Walking )
		{
			//Increase Stamina
			switch ( pcCur->sCharacterData.bSPType )
			{
				case 1:
					ps->fSP = (float)(3.8f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;

				case 2:
					ps->fSP = (float)(3.3f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;

				case 3:
					ps->fSP = (float)(2.9f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;

				default:
					ps->fSP = (float)(3.8f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;
			}
		}

		//Character is Alive?
		if ( pcCur->psModelAnimation->iType != ANIMATIONTYPE_Die )
		{
			//Set HP Regen
			ps->fHP = (((fLevel + (fStrength / 2.0f) + fHealth) / 180.0f + pcCur->sCharacterData.fHPRegen) / 1.5f);

			//Set MP Regen
			ps->fMP = (((fLevel + (fSpirit * 1.2f) + (fHealth / 2.0f)) / 115.0f) + pcCur->sCharacterData.fMPRegen);

			(*(float*)0x04B07C00) = ps->fHP;
			(*(float*)0x04B07C04) = ps->fMP - CGameProtect::GetDecreaseMP();
			(*(float*)0x04B07C08) = ps->fSP - CGameProtect::GetDecreaseSP();
		}

		ps->fHP += CRC_REGEN_CODE;
		ps->fMP += CRC_REGEN_CODE;
		ps->fSP += CRC_REGEN_CODE;
	}
#else

	if ( pcCur->psModelAnimation )
	{
		//Character Standing Mode?
		if ( pcCur->psModelAnimation->iType == ANIMATIONTYPE_Idle )
		{
			//Increase Stamina
			switch ( pcCur->sCharacterData.bSPType )
			{
				case 1:
					ps->fSP = (float)(3.8f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;

				case 2:
					ps->fSP = (float)(3.3f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;

				case 3:
					ps->fSP = (float)(2.9f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;

				default:
					ps->fSP = (float)(3.8f + fLevel / 7 + pcCur->sCharacterData.fSPRegen);
					break;
			}
		}

		//Character Walking Mode?
		if ( pcCur->psModelAnimation->iType == ANIMATIONTYPE_Walking )
		{
			//Increase Stamina
			switch ( pcCur->sCharacterData.bSPType )
			{
				case 1:
					ps->fSP = (float)(3.8f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;

				case 2:
					ps->fSP = (float)(3.3f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;

				case 3:
					ps->fSP = (float)(2.9f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;

				default:
					ps->fSP = (float)(3.8f + (fLevel / 7.0f) + pcCur->sCharacterData.fSPRegen);
					break;
			}
		}

		//Character is Alive?
		if ( pcCur->psModelAnimation->iType != ANIMATIONTYPE_Die )
		{
			//Set HP Regen
			ps->fHP = (((fLevel + (fStrength / 2.0f) + fHealth) / 180.0f + pcCur->sCharacterData.fHPRegen) / 1.5f);

			//Set MP Regen
			ps->fMP = (((fLevel + (fSpirit * 1.2f) + (fHealth / 2.0f)) / 115.0f) + pcCur->sCharacterData.fMPRegen);

			(*(float *)0x04B07C00) = ps->fHP;
			(*(float *)0x04B07C04) = ps->fMP - CGameProtect::GetDecreaseMP();
			(*(float *)0x04B07C08) = ps->fSP - CGameProtect::GetDecreaseSP();
		}
		
		ps->fHP += CRC_REGEN_CODE;
		ps->fMP += CRC_REGEN_CODE;
		ps->fSP += CRC_REGEN_CODE;
	}
#endif
}

void CGameProtect::OnSetRegen()
{
	static float fLDecreaseSP	= 0.0;
	static float fLIncreaseSP	= 0.0;

	static float fLIncreaseHP	= 0.0;
	
	static float fLDecreaseMP	= 0.0;
	static float fLIncreaseMP	= 0.0;
	
	float fDecreaseSPOld		= (*(float*)ppDecreaseSP);
	float fDecreaseSP			= CGameProtect::GetDecreaseSP();
	float fDecreaseMP			= CGameProtect::GetDecreaseMP();

	RegenPointerData sRegenData;
	sRegenData = CGameProtect::saRegenData[CGameProtect::iRegenWheel % 5];

	RegenPointerData * ps		= &sRegenData;
	RegenPointerData * psOld	= &CGameProtect::saRegenData[CGameProtect::iRegenWheel % 5];

	ps->fHP -= CRC_REGEN_CODE;
	ps->fMP -= CRC_REGEN_CODE;
	ps->fSP -= CRC_REGEN_CODE;

	//Check Character Validate
	CALL( pfnValidateCaracterCheck );

	//Decrease SP
	if ( fDecreaseSPOld )
	{
		fLDecreaseSP += (fDecreaseSPOld / (float)(60 / 4));

		if ( fLDecreaseSP >= 1.0f )
		{
			CHARACTERGAME->SetCurrentSP( CHARACTERGAME->GetCurrentSP() - (short)fLDecreaseSP );
			fLDecreaseSP -= 1.0f;
		}

		//Set SP
		(*(float*)ppDecreaseSP) = fDecreaseSPOld;
	}

	//Increase SP
	if ( ps->fSP )
	{
		fLIncreaseSP += (ps->fSP / (float)(60 / 4));

		if ( fLIncreaseSP >= 1.0f )
		{
			CHARACTERGAME->SetCurrentSP( CHARACTERGAME->GetCurrentSP() + (short)fLIncreaseSP );

			//Loop to increasing...
			while ( true )
			{
				if ( fLIncreaseSP > 1.0f )
					fLIncreaseSP -= 1.0f;
				else
					break;
			}
		}
	}

	//Increase HP
	if ( ps->fHP )
	{
		fLIncreaseHP += (ps->fHP / (float)(60 / 4));

		if ( fLIncreaseHP >= 1.0f )
		{
			CHARACTERGAME->SetCurrentHP( CHARACTERGAME->GetCurrentHP() + (short)fLIncreaseHP );

			//Loop to increasing...
			while ( true )
			{
				if ( fLIncreaseHP > 1.0f )
					fLIncreaseHP -= 1.0f;
				else
					break;
			}
		}
	}

	//Increase MP
	if ( ps->fMP )
	{
		fLIncreaseMP += (ps->fMP / (float)(60 / 4));

		if ( fLIncreaseMP >= 1.0f )
		{
			CHARACTERGAME->SetCurrentMP( CHARACTERGAME->GetCurrentMP() + (short)fLIncreaseMP );

			//Loop to increasing...
			while ( true )
			{
				if ( fLIncreaseMP > 1.0f )
					fLIncreaseMP -= 1.0f;
				else
					break;
			}
		}
	}

	//Decrease MP
	if ( fDecreaseMP )
	{
		fLDecreaseMP += (fDecreaseMP / (float)(70 / 4));

		if ( fLDecreaseMP >= 1.0f )
		{
			CHARACTERGAME->SetCurrentMP( CHARACTERGAME->GetCurrentMP() - (short)fLDecreaseMP );

			//Loop to decreasing...
			while ( true )
			{
				if ( fLDecreaseMP > 1.0f )
					fLDecreaseMP -= 1.0f;
				else
					break;
			}

			//Set MP
			CGameProtect::SetDecreaseMP( 0.0f );
		}
	}

	//Decrease SP
	if ( fDecreaseSP )
	{
		fLDecreaseSP += (fDecreaseSP / (float)(60 / 4));

		if ( fLDecreaseSP >= 1.0f )
		{
			CHARACTERGAME->SetCurrentSP( CHARACTERGAME->GetCurrentSP() - (short)fLDecreaseSP );

			//Loop to decreasing...
			while ( true )
			{
				if ( fLDecreaseSP > 1.0f )
					fLDecreaseSP -= 1.0f;
				else
					break;
			}

			//Set SP
			CGameProtect::SetDecreaseSP( 0.0f );
		}
	}


	//Validate
	CHECK_CHARACTER_CHECKSUM;

	Update();
}

void CGameProtect::SetDecreaseMP( float fMP )
{
	CGameProtect::fDecreaseMPData = fMP;
}

float CGameProtect::GetDecreaseMP()
{
	return CGameProtect::fDecreaseMPData;
}

void CGameProtect::SetDecreaseSP( float fSP )
{
	CGameProtect::fDecreaseSPData = fSP;
}

float CGameProtect::GetDecreaseSP()
{
	return CGameProtect::fDecreaseSPData;
}

BOOL CGameProtect::ValidateStatusPoints( CharacterData & sCharacterData, UINT iQuestFlag )
{
	BOOL bRet = TRUE;

	int iBaseStatusCount = abs( sCharacterData.iStrength ) +
		abs( sCharacterData.iSpirit ) +
		abs( sCharacterData.iTalent ) +
		abs( sCharacterData.iAgility ) +
		abs( sCharacterData.iHealth ) +
		abs( sCharacterData.iStatPoints );

	int iAddStatusPoints = ((sCharacterData.iLevel - 1) * 5) + 99;
	iAddStatusPoints += GetStatusPointQuest( sCharacterData.iLevel, iQuestFlag );

	sCharacterData.iStatPoints += (iAddStatusPoints - iBaseStatusCount);

	if ( sCharacterData.iStatPoints < 0 )
	{
		if ( sCharacterData.iStatPoints <= (-10) )
		{
			sCharacterData.iStrength = 1;
			sCharacterData.iSpirit = 8;
			sCharacterData.iTalent = 1;
			sCharacterData.iAgility = 1;
			sCharacterData.iHealth = 30;

			bRet = FALSE;
		}

		sCharacterData.iStatPoints = 0;
	}

	return bRet;
}

BOOL CGameProtect::AddStatusLevelUP( int iLevelCount, UINT iQuestFlag )
{
	int iAddPoints = 0;

	//Kingdom Test
	if ( iQuestFlag & 64 )
		iAddPoints += iLevelCount * 2;

	//Bitter Ordeal
	if ( iQuestFlag & 128 )
		iAddPoints += iLevelCount * 3;

	//Custom Quests
	if ( iQuestFlag & QUESTSAVEFLAG_TheLoyaltyTest )
		iAddPoints += iLevelCount * 3;

	UNITDATA->sCharacterData.iStatPoints += iAddPoints;

	return TRUE;
}

NAKED BOOL CGameProtect::ValidateCharacterCheck()
{
	JMP( pfnValidateCaracterCheck );
}

int CGameProtect::GetStatusPointQuest( int iLevel, UINT iQuestFlag )
{
	int iPoints = 0;

	//For Her
	if ( (iQuestFlag & 1) && (iLevel >= 30) )
		iPoints += 5;

	//Help Michelle
	if ( (iQuestFlag & 4) && (iLevel >= 70) )
		iPoints += 5;

	//Closed Fury
	if ( (iQuestFlag & 8) && (iLevel >= 80) )
		iPoints += 5;

	//Kingdom Test
	if ( (iQuestFlag & 64) && (iLevel >= 80) )
		iPoints += (iLevel - 79) * 2;

	//Bitter Ordeal
	if ( (iQuestFlag & 128) && (iLevel >= 90) )
		iPoints += (iLevel - 89) * 3;

	//Custom Quests
	if ( (iQuestFlag & QUESTSAVEFLAG_TheLoyaltyTest) && (iLevel >= 120) )
		iPoints += (iLevel - 119) * 3;

	return iPoints;
}
