#include "stdafx.h"
#include "CRoomInstanceHandler.h"


CRoomInstanceHandler::CRoomInstanceHandler()
{
	uRoomWheelID = 1;
}


CRoomInstanceHandler::~CRoomInstanceHandler()
{
	for ( std::vector<CRoomInstance*>::iterator it = vRooms.begin(); it != vRooms.end(); it++ )
	{
		CRoomInstance * pc = (*it);
		DELET( pc );
	}
	vRooms.clear();
}

void CRoomInstanceHandler::Init()
{
}

void CRoomInstanceHandler::Shutdown()
{
}

void CRoomInstanceHandler::Update()
{
}

void CRoomInstanceHandler::Test( User * pcUser )
{
	CRoomInstance * pc = Add( MAPID_Invalid );
	pc->Add( 1 )->Add( 1 )->Add( pcUser->GetID() );
}


CRoomInstance * CRoomInstanceHandler::Add( EMapID iMapID, BOOL bCanViewOtherRoom, const char * pszName )
{
	CRoomInstance * pc = new CRoomInstance( uRoomWheelID++, iMapID, bCanViewOtherRoom, pszName );
	vRooms.push_back( pc );
	return pc;
}

void CRoomInstanceHandler::RemoveUnit( UINT lID )
{
	CRoomTeam * pcTeam = GetTeamUnitID( lID );
	if ( pcTeam )
		pcTeam->Remove( lID );
}

void CRoomInstanceHandler::Delete( UINT lID )
{
	for ( std::vector<CRoomInstance*>::iterator it = vRooms.begin(); it != vRooms.end(); )
	{
		CRoomInstance * pc = (*it);
		if ( pc->GetID() == lID )
		{
			DELET( pc );
			it = vRooms.erase( it );
		}
		else
			it++;
	}
}

void CRoomInstanceHandler::Delete( CRoomInstance * pcRoom )
{
	Delete( pcRoom->GetID() );
}

std::vector<CRoomInstance*> CRoomInstanceHandler::GetRooms( EMapID iMapID )
{
	std::vector<CRoomInstance*> v;
	for ( std::vector<CRoomInstance*>::iterator it = vRooms.begin(); it != vRooms.end(); it++ )
	{
		CRoomInstance * pc = (*it);
		if ( (iMapID == MAPID_Invalid) || (pc->GetMapID() == iMapID) )
			v.push_back( pc );
	}
	return v;
}

CRoomInstance * CRoomInstanceHandler::GetRoomUnitID( int iID )
{
	for ( std::vector<CRoomInstance*>::iterator it = vRooms.begin(); it != vRooms.end(); it++ )
	{
		CRoomInstance * pc = (*it);
		if ( pc->IsInRoom( iID ) )
			return pc;
	}

	return nullptr;
}

CRoomTeam * CRoomInstanceHandler::GetTeamUnitID( int iID )
{
	CRoomInstance * pcRoom = GetRoomUnitID( iID );
	if ( pcRoom )
	{
		for ( auto pcAlly : pcRoom->GetAllies() )
		{
			for ( auto pcTeam : pcAlly->GetTeams() )
			{
				for ( auto psUnit : pcTeam->GetUnits() )
				{
					if ( psUnit->iID == iID )
						return pcTeam;
				}
			}
		}
	}

	return nullptr;
}

CRoomInstance * CRoomInstanceHandler::GetRoom( int iID )
{
	for ( std::vector<CRoomInstance*>::iterator it = vRooms.begin(); it != vRooms.end(); it++ )
	{
		CRoomInstance * pc = (*it);
		if ( pc->GetID() == iID )
			return pc;
	}

	return nullptr;
}

BOOL CRoomInstanceHandler::SameRoom( int iUnitOneID, int iUnitTwoID )
{
	CRoomInstance * pcRoomOne = GetRoomUnitID( iUnitOneID );
	CRoomInstance * pcRoomTwo = GetRoomUnitID( iUnitTwoID );

	return ((pcRoomOne != NULL) && (pcRoomTwo != NULL) && (pcRoomOne == pcRoomTwo)) ? TRUE : FALSE;
}

BOOL CRoomInstanceHandler::CanSee( int iUnitOneID, int iUnitTwoID )
{
	//If both are in a room
	CRoomInstance * pcRoomOne = GetRoomUnitID( iUnitOneID );
	CRoomInstance * pcRoomTwo = GetRoomUnitID( iUnitTwoID );

	//Both in a room?
	if ( pcRoomOne && pcRoomTwo )
	{
		UnitRoom * pcUnitRoomOne = pcRoomOne->GetUnit( iUnitOneID );
		UnitRoom * pcUnitRoomTwo = pcRoomTwo->GetUnit( iUnitTwoID );

		//If some of room units can't see each other
		if ( (pcUnitRoomOne->bCanViewOther == FALSE) || (pcUnitRoomTwo->bCanViewOther == FALSE) )
		{
			//CHATSERVER->SendChatAll( CHATCOLOR_Global, "> Can't see 1" );
			return FALSE;
		}
	}

	if ( pcRoomOne != pcRoomTwo )
	{		
		//Room one view
		if ( pcRoomOne )
			if ( pcRoomOne->CanView() == FALSE )
			{
				//CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "> Can't see 2 [%d][%d]", pcRoomOne ? 1 : 0, pcRoomTwo ? 1 : 0 );
				return FALSE;
			}
		//Room two view
		if ( pcRoomTwo )
			if ( pcRoomTwo->CanView() == FALSE )
			{
				//CHATSERVER->SendChatAllEx( CHATCOLOR_Global, "> Can't see 3 [%d][%d]", pcRoomOne ? 1 : 0, pcRoomTwo ? 1 : 0 );
				return FALSE;
			}
	}

	return TRUE;
}


std::vector<User*> CRoomInstanceHandler::GetUsersRoom( UINT lID )
{
	std::vector<User*> v;
	
	CRoomInstance * pcRoom = GetRoom( lID );
	if ( pcRoom )
	{
		for ( auto pcAlly : pcRoom->GetAllies() )
		{
			for ( auto pcTeam : pcAlly->GetTeams() )
			{
				for ( auto psUnit : pcTeam->GetUnits() )
				{
					User * pcUser = USERDATATOUSER( USERDATABYID( psUnit->iID ) );
					if ( pcUser )
						v.push_back( pcUser );
				}
			}
		}
	}

	return v;
}
