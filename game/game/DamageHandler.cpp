#include "stdafx.h"
#include "DamageHandler.h"


CDamageHandler::CDamageHandler()
{
}


CDamageHandler::~CDamageHandler()
{
}

void CDamageHandler::HandlePacket( PacketDamageQuick * psPacket )
{
	if ( psPacket->iDamage <= 0 )
		return;

	int iDecrease;

	if ( psPacket->bPercentile == FALSE )
	{
		//Decrease HP
		iDecrease = psPacket->iDamage;
	}
	else
	{
		if ( psPacket->bBaseCurrentHP )
		{
			int iCurHP = CHARACTERGAME->GetCurrentHP();

			//Based on Current HP
			iDecrease = (iCurHP * psPacket->iDamage) / 100;
		}
		else
		{
			int iMaxHP = UNITDATA->sCharacterData.sHP.sMax;

			//Based on Max HP
			iDecrease = (iMaxHP * psPacket->iDamage) / 100;
		}
	}

	int iNewHP = CHARACTERGAME->GetCurrentHP() - iDecrease;

	if ( iNewHP > 0 )
	{
		CHARACTERGAME->SetCurrentHP( iNewHP );
		CHECK_CHARACTER_CHECKSUM;
	}
	else
	{
		//Die
		if ( psPacket->bDie )
			CHARACTERGAME->Die();
	}
}
