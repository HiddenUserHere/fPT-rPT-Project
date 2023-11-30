#include "stdafx.h"
#include "CCharacterStatusHandler.h"


CCharacterStatusHandler::CCharacterStatusHandler()
{
	for ( int i = 0; i < MAX_CHARACTERSTATUS_SAVE_POINTS; i++ )
	{
		saSavePoints[i].iStatus = CHARACTERSTATUS_None;
		saSavePoints[i].iAddPoints = 0;
		saSavePoints[i].dwTime = 0;
	}

	iSavePointsWheel = 0;
}


CCharacterStatusHandler::~CCharacterStatusHandler()
{
}

void CCharacterStatusHandler::UpdateBaseStatus( ECharacterStatus iStatus, int iAddPoints )
{
	CharacterData & sCharacterData = UNITDATA->sCharacterData;

	switch ( iStatus )
	{
		case CHARACTERSTATUS_Strength:
			sCharacterData.iStrength += iAddPoints;

			AddSavePoints( iStatus, iAddPoints );
			break;

		case CHARACTERSTATUS_Spirit:
			sCharacterData.iSpirit += iAddPoints;

			AddSavePoints( iStatus, iAddPoints );
			break;

		case CHARACTERSTATUS_Talent:
			sCharacterData.iTalent += iAddPoints;

			AddSavePoints( iStatus, iAddPoints );
			break;

		case CHARACTERSTATUS_Agility:
			sCharacterData.iStrength += iAddPoints;

			AddSavePoints( iStatus, iAddPoints );
			break;

		case CHARACTERSTATUS_Health:
			sCharacterData.iStrength += iAddPoints;

			AddSavePoints( iStatus, iAddPoints );
			break;

		case CHARACTERSTATUS_StatusPoints:
			CharacterStatusSavePoints * psOldStatus;
			psOldStatus = GetLastSavePoints();
			switch ( psOldStatus->iStatus )
			{
				case CHARACTERSTATUS_Strength:
					sCharacterData.iStrength -= psOldStatus->iAddPoints;
					break;

				case CHARACTERSTATUS_Spirit:
					sCharacterData.iSpirit -= psOldStatus->iAddPoints;
					break;

				case CHARACTERSTATUS_Talent:
					sCharacterData.iTalent -= psOldStatus->iAddPoints;
					break;

				case CHARACTERSTATUS_Agility:
					sCharacterData.iAgility -= psOldStatus->iAddPoints;
					break;

				case CHARACTERSTATUS_Health:
					sCharacterData.iHealth -= psOldStatus->iAddPoints;
					break;

				default:
					break;
			}

			psOldStatus->iStatus = CHARACTERSTATUS_None;

			sCharacterData.iStatPoints += psOldStatus->iAddPoints;
			break;



		default:
			break;
	}
}

void CCharacterStatusHandler::AddSavePoints( ECharacterStatus iStatus, int iAddPoints )
{
	CharacterStatusSavePoints * ps = saSavePoints + (iSavePointsWheel % MAX_CHARACTERSTATUS_SAVE_POINTS);
	ps->iStatus		= iStatus;
	ps->iAddPoints	= iAddPoints;
	ps->dwTime		= TICKCOUNT;

	iSavePointsWheel++;
}

CCharacterStatusHandler::CharacterStatusSavePoints * CCharacterStatusHandler::GetLastSavePoints()
{
	for ( int i = 0; i < MAX_CHARACTERSTATUS_SAVE_POINTS; i++ )
	{
		CharacterStatusSavePoints * ps = saSavePoints + ((iSavePointsWheel - i) % MAX_CHARACTERSTATUS_SAVE_POINTS);
		if ( ps->iStatus != CHARACTERSTATUS_None )
		{
			if ( (ps->dwTime + 60000) > TICKCOUNT )
				return ps;
		}
	}

	return nullptr;
}
