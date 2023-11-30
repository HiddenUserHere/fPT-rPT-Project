#include "stdafx.h"
#include "CImpulsionHandler.h"


CImpulsionHandler::CImpulsionHandler()
{
}


CImpulsionHandler::~CImpulsionHandler()
{
}

BOOL CImpulsionHandler::OnEvent( UnitData * pcUnitData )
{
	int * piRange		= ((int*)0x00992CB0);
	int * piTargetCount = ((int*)0x00992CD8);

	Point3D s;
	//Hitting Position
	if ( pcUnitData->GetAnimationAttackPosition( &s.iX, &s.iY, &s.iZ ) )
	{
		//Set Effect
		CImpulsionHandler::SetImpulsionHitEffect( &s );

		//Set Units to Area Damage
		SkillManager::SetUnitsRangeCount( pcUnitData->sPosition, piRange[pcUnitData->sActiveSkill.cLevel - 1], TRUE, piTargetCount[pcUnitData->sActiveSkill.cLevel - 1] );

		if ( UNITDATA == pcUnitData && UNITS_AREA_COUNT > 0 )
		{
			//Send Damage
			SKILLMANAGERHANDLER->DamageAreaHandle( pcUnitData, pcUnitData->sPosition, 0, 0, 0, 0, pcUnitData->sActiveSkill.iValue );
		}

		//Skill Impulsion Effect to Targets...
		CALL_WITH_ARG2( 0x005D5410, (DWORD)UNITS_AREA_TARGET, UNITS_AREA_COUNT );

		if ( UNITS_AREA_COUNT )
			SKILLMANAGERHANDLER->SkillPlaySound( 0x4010, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

		SKILLMANAGERHANDLER->SkillPlaySound( 0x3241, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );
	}
	return TRUE;
}

NAKED void CImpulsionHandler::SetImpulsionHitEffect( Point3D * psPosition )
{
	JMP( 0x005B02E0 );
}
