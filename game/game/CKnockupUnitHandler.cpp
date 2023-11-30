#include "stdafx.h"
#include "CKnockupUnitHandler.h"


CKnockupUnit::CKnockupUnit( Unit * pcUnit, float fTimeUp, float fTimeDown, float fTimeInAir, float fMaxY )
{
	this->pcUnit = pcUnit;
	this->iID = pcUnit->GetID();
	this->fTimeUp = fTimeUp;
	this->fTimeDown = fTimeDown;
	this->fTimeInAir = fTimeInAir;
	this->fMaxY = fMaxY;
	this->fTime = 0.0f;
	this->iBeginY = 0;
	this->iCurrentY = 0;
}

CKnockupUnit::~CKnockupUnit()
{
	vAnimationTypeLostConditions.clear();
}


void CKnockupUnit::AddAnimationLostCondition( EAnimationType eAnimationType )
{
	vAnimationTypeLostConditions.push_back( eAnimationType );
}

void CKnockupUnit::Update( float fTime )
{
	this->fTime += fTime;

	if ( GetTime() >= GetDuration() )
		return;

	if ( iBeginY == 0 )
		iBeginY = pcUnit->pcUnitData->sPosition.iY;

	float fChangeYUP = (fMaxY / (fTimeUp == 0.0f ? 1.0f : fTimeUp));
	float fChangeYDown = (fMaxY / (fTimeDown == 0.0f ? 1.0f : fTimeDown));

	if ( pcUnit->pcUnitData->GetAnimation() != ANIMATIONTYPE_Running )
		pcUnit->pcUnitData->Animate( ANIMATIONTYPE_Running );

	//Up?
	if ( GetTime() <= fTimeUp )
	{
		float fY = X3D::easeOutQuadf( 0.0f, (fTimeUp * 0.75f), GetTime() );

		pcUnit->pcUnitData->sPosition.iY = iBeginY + (int)roundf( fY * fChangeYUP );

		iCurrentY = pcUnit->pcUnitData->sPosition.iY;
	}
	else
	{
		//Down? instead of this, will be freezed...
		if ( GetTime() >= (fTimeUp + GetTimeInAir()) )
		{
			float fCurrentTime = GetTime() - (fTimeUp + GetTimeInAir());
			float fY = X3D::easeInQuadf( 0.0f, (fTimeDown * 0.75f), fCurrentTime );

			pcUnit->pcUnitData->sPosition.iY = iCurrentY - (int)roundf( fY * fChangeYDown );
		}
	}
}

BOOL CKnockupUnit::Lost( DWORD dwTickCount )
{
	if ( GetTime() >= GetDuration() )
		return TRUE;

	if ( pcUnit == NULL )
		return TRUE;

	if ( pcUnit->GetID() != iID )
		return TRUE;

	UnitData * pcUnitData = pcUnit->pcUnitData;

	if ( pcUnitData->bActive == FALSE )
		return TRUE;

	if ( pcUnitData != UNITDATA && (pcUnitData->dwLastActiveTime + 10000) < dwTickCount )
		return TRUE;

	if ( pcUnitData->psModelAnimation == NULL )
		return TRUE;

	for ( std::vector<EAnimationType>::iterator it = vAnimationTypeLostConditions.begin(); it != vAnimationTypeLostConditions.end(); it++ )
	{
		EAnimationType eAnimationType = *it;

		if ( pcUnitData->psModelAnimation->iType == eAnimationType )
			return TRUE;
	}

	return FALSE;
}


CKnockupUnitHandler::CKnockupUnitHandler()
{
}


CKnockupUnitHandler::~CKnockupUnitHandler()
{
	for ( std::vector<CKnockupUnit*>::iterator it = vKnockupUnits.begin(); it != vKnockupUnits.end(); it++ )
	{
		CKnockupUnit * pcKnockupUnit = (*it);
		DELET( pcKnockupUnit );
	}
	vKnockupUnits.clear();
}

void CKnockupUnitHandler::Init()
{
}

void CKnockupUnitHandler::Update( float fTime )
{
	for ( std::vector<CKnockupUnit*>::iterator it = vKnockupUnits.begin(); it != vKnockupUnits.end(); )
	{
		CKnockupUnit * pcKnockupUnit = (*it);
		if ( pcKnockupUnit->Lost( TICKCOUNT ) == FALSE )
		{
			pcKnockupUnit->Update( fTime );
			it++;
		}
		else
		{
			DELET( pcKnockupUnit );
			it = vKnockupUnits.erase( it );
		}
	}
}

void CKnockupUnitHandler::AddUnit( CKnockupUnit * pcKnockupUnit )
{
	vKnockupUnits.push_back( pcKnockupUnit );
}

void CKnockupUnitHandler::RemoveUnit( Unit * pcUnit )
{
	for ( std::vector<CKnockupUnit*>::iterator it = vKnockupUnits.begin(); it != vKnockupUnits.end(); )
	{
		CKnockupUnit * pcKnockupUnit = (*it);
		if ( pcKnockupUnit->GetUnit() == pcUnit )
		{
			DELET( pcKnockupUnit );
			it = vKnockupUnits.erase( it );
		}
		else
			it++;
	}
}

void CKnockupUnitHandler::RemoveUnit( int iID )
{
	for ( std::vector<CKnockupUnit*>::iterator it = vKnockupUnits.begin(); it != vKnockupUnits.end(); )
	{
		CKnockupUnit * pcKnockupUnit = (*it);
		if ( pcKnockupUnit->GetID() == iID )
		{
			DELET( pcKnockupUnit );
			it = vKnockupUnits.erase( it );
		}
		else
			it++;
	}
}

BOOL CKnockupUnitHandler::CanChangeUnitY( Unit * pcUnit )
{
	BOOL bRet = TRUE;

	for ( std::vector<CKnockupUnit*>::iterator it = vKnockupUnits.begin(); it != vKnockupUnits.end(); it++ )
	{
		CKnockupUnit * pcKnockupUnit = (*it);

		if ( (pcKnockupUnit->GetUnit() == pcUnit) && (pcKnockupUnit->GetID() == pcUnit->GetID()) )
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}
