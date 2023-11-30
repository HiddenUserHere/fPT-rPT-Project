#include "stdafx.h"
#include "room.h"

CRoomTeam::CRoomTeam( int iID )
{
	this->iID = iID;
}

CRoomTeam::~CRoomTeam()
{
}


void CRoomTeam::Clear()
{
	for ( std::vector<UnitRoom*>::iterator it = vUnits.begin(); it != vUnits.end(); it++ )
	{
		UnitRoom * pc = (*it);
		DELET( pc );
	}
	vUnits.clear();
}

UnitRoom * CRoomTeam::GetUnit( int iID )
{
	for ( std::vector<UnitRoom*>::iterator it = vUnits.begin(); it != vUnits.end(); it++ )
	{
		UnitRoom * pc = (*it);
		if ( pc->iID == iID )
			return pc;
	}
	return nullptr;
}

UnitRoom * CRoomTeam::Add( int iID, BOOL bCanViewOther )
{
	UnitRoom * pc = new UnitRoom;
	pc->iID = iID;
	pc->bCanViewOther = bCanViewOther;
	pc->bInvalid = false;
	vUnits.push_back( pc );

	return pc;
}

void CRoomTeam::Remove( int iID )
{
	for ( std::vector<UnitRoom*>::iterator it = vUnits.begin(); it != vUnits.end(); )
	{
		UnitRoom * pc = (*it);
		if ( pc->iID == iID )
		{
			DELET( pc );
			it = vUnits.erase( it );
		}
		else
			it++;
	}
}

void CRoomTeam::Invalidate( int iID )
{
	for ( auto ps : vUnits )
	{
		if ( ps->iID == iID )
		{
			ps->bInvalid = true;
			break;
		}
	}
}


CRoomAllies::CRoomAllies( int iID )
{
	this->iID = iID;
}

CRoomAllies::~CRoomAllies()
{
	Clear();
}

void CRoomAllies::Clear()
{
	for ( std::vector<CRoomTeam*>::iterator it = vTeams.begin(); it != vTeams.end(); it++ )
	{
		CRoomTeam * pc = (*it);
		DELET( pc );
	}
	vTeams.clear();
}

CRoomTeam * CRoomAllies::Get( int iID )
{
	for ( std::vector<CRoomTeam*>::iterator it = vTeams.begin(); it != vTeams.end(); it++ )
	{
		CRoomTeam * pc = (*it);
		if ( pc->GetID() == iID )
			return pc;
	}
	return nullptr;
}

CRoomTeam * CRoomAllies::Add( int iID )
{
	CRoomTeam * pcTeam = new CRoomTeam( iID );
	vTeams.push_back( pcTeam );
	return pcTeam;
}

UnitRoom * CRoomAllies::GetUnit( int iID )
{
	for ( std::vector<CRoomTeam*>::iterator it = vTeams.begin(); it != vTeams.end(); it++ )
	{
		CRoomTeam * pcTeam	= (*it);
		UnitRoom * pcUnit	= pcTeam->GetUnit( iID );
		if ( pcUnit )
			return pcUnit;
	}
	return nullptr;
}

CRoomInstance::CRoomInstance( UINT lID, EMapID iMapID, BOOL bCanView, const char * pszName )
{
	this->lID		= lID;
	this->iMapID	= iMapID;
	this->bVisible	= bCanView;
	STRINGCOPY( this->szName, szName );
}

CRoomInstance::~CRoomInstance()
{
	Clear();
}

void CRoomInstance::Clear()
{
	for ( std::vector<CRoomAllies*>::iterator it = vAllies.begin(); it != vAllies.end(); it++ )
	{
		CRoomAllies * pc = (*it);
		DELET( pc );
	}
	vAllies.clear();
}

CRoomAllies * CRoomInstance::Get( UINT lID )
{
	for ( std::vector<CRoomAllies*>::iterator it = vAllies.begin(); it != vAllies.end(); it++ )
	{
		CRoomAllies * pc = (*it);
		if ( pc->GetID() == lID )
			return pc;
	}
	return nullptr;
}

CRoomAllies * CRoomInstance::Add( UINT lID )
{
	CRoomAllies * pc = new CRoomAllies( lID );
	vAllies.push_back( pc );
	return pc;
}

BOOL CRoomInstance::IsInRoom( int iID )
{
	auto InRoom = []( std::vector<CRoomTeam*> & vTeams, int iUnitID ) -> BOOL
	{
		for ( std::vector<CRoomTeam*>::iterator it = vTeams.begin(); it != vTeams.end(); it++ )
		{
			CRoomTeam * pc = (*it);
			if ( pc->GetUnit( iUnitID ) )
				return TRUE;
		}

		return FALSE;
	};

	for ( std::vector<CRoomAllies*>::iterator it = vAllies.begin(); it != vAllies.end(); it++ )
	{
		CRoomAllies * pc = (*it);
		if ( InRoom( pc->GetTeams(), iID ) )
			return TRUE;
	}
	return FALSE;
}

UnitRoom * CRoomInstance::GetUnit( int iID )
{
	for ( std::vector<CRoomAllies*>::iterator it = vAllies.begin(); it != vAllies.end(); it++ )
	{
		CRoomAllies * pcAlly = (*it);
		UnitRoom * pcUnit = pcAlly->GetUnit( iID );
		if ( pcUnit )
			return pcUnit;
	}
	return nullptr;
}
