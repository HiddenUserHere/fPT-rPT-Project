#include "stdafx.h"
#include "RessurectionHandler.h"


BOOL CRessurectionHandler::OnUseSkill( UnitData * pcUnitData )
{
#define _DIST_RESSURECTION (300*300)

	static int iaTargets[10]
	{
		3,3,3,4,4,4,5,5,5,6
	};

	int iCount = 0;

	int iMax = iaTargets[UNITDATA->sActiveSkill.cLevel - 1];

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pc = UNITGAME->pcaUnitData + i;

		if ( pc->bActive && pc->sCharacterData.iType == ECharacterType::CHARACTERTYPE_Player )
		{

			if ( CHARACTERGAME->GetStageID() == MAPID_BlessCastle )
			{
				if ( UNITDATA->iClanID != 0 && UNITDATA->iClanID != pc->iClanID )
					continue;
			}

			if ( pc->IsDead() )
			{
				int iX = (pcUnitData->sPosition.iX - pc->sPosition.iX) >> 8;
				int iZ = (pcUnitData->sPosition.iZ - pc->sPosition.iZ) >> 8;


				int iDistance = (iX * iX) + (iZ * iZ);

				if ( iDistance < _DIST_RESSURECTION )
				{
					CALL_WITH_ARG4( 0x00623790, 0x3B, UNITDATA->sActiveSkill.cLevel, pc->iID, 0 );

					iCount++;

					if ( iCount == iMax )
						break;
				}
			}
		}
	}

	return TRUE;
}

CRessurectionHandler::CRessurectionHandler()
{
}


CRessurectionHandler::~CRessurectionHandler()
{
}
