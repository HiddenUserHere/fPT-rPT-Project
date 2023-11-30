#include "stdafx.h"
#include "CIceMeteoriteHandler.h"

CIceMeteoriteHandler::CIceMeteoriteHandler()
{
}

CIceMeteoriteHandler::~CIceMeteoriteHandler()
{
}

bool CIceMeteoriteHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( USESKILL( psSkill ) )
	{
		//Update Skill Data
		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		//Animate...
		UNITDATA->Animate( ANIMATIONTYPE_Skill );

		CHARACTERGAME->OnMotionChange( UNITDATA, 56, 0 );

		//New FPS based on Weapon Speed
		UNITDATA->iNewFPS = SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
		UNITDATA->bOverrideFPS = TRUE;

		UNITDATA->iEnchantWeaponType = 15;

		UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T52;
		UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

		return true;
	}

	return false;
}

BOOL CIceMeteoriteHandler::OnEvent( UnitData * pcUnitData )
{
	PacketIceMeteorite sPacket;
	sPacket.sBegin = pcUnitData->sPosition;
	sPacket.sBegin.iY += (585 << 8);
	sPacket.sEnd = pcUnitData->sPosition;
	sPacket.uMeteoriteCount = iaPrsT52MeteoriteCount[pcUnitData->sActiveSkill.cLevel - 1];
	sPacket.bSmall = FALSE;
	sPacket.iDelay = 0;
	
	for ( UINT i = 0; i < sPacket.uMeteoriteCount; i++ )
	{
		Point3D sEnd = sPacket.sEnd;
		int iDelay = sPacket.iDelay;

		if ( sPacket.uMeteoriteCount > 1 )
		{
			sEnd.iX -= RandomI( 0, 75 ) << 8;
			sEnd.iZ -= RandomI( 0, 75 ) << 8;

			sEnd.iX += RandomI( 0, 150 ) << 8;
			sEnd.iZ += RandomI( 0, 150 ) << 8;

			//iDelay = RandomI( 51, 250 );
		}

		EffectGame::IceMeteorite( &sPacket.sBegin, &sEnd, iDelay );
	}

	if ( pcUnitData == UNITDATA )
	{
		SKILLMANAGERHANDLER->SetUnitsRange( UNITDATA->sPosition, (iaPrsT52Area[pcUnitData->sActiveSkill.cLevel - 1] * iaPrsT52MeteoriteCount[pcUnitData->sActiveSkill.cLevel - 1]) >> 1 );
		SKILLMANAGERHANDLER->DamageAreaHandle( UNITDATA, UNITDATA->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
	}

	return TRUE;
}

BOOL CIceMeteoriteHandler::OnAnimationEnd( UnitData * pcUnitData )
{

	return FALSE;
}

