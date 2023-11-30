#include "stdafx.h"
#include "CServerEventHandler.h"


CServerEventHandler::CServerEventHandler()
{
}


CServerEventHandler::~CServerEventHandler()
{
	vMapEvents.clear();
}

BOOL CServerEventHandler::OnUnitKilled( Unit * pcUnit )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUnitKilled( pcUnit ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnUnitKilled( User * pcUser, Unit * pcUnit )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUnitKilled( pcUser, pcUnit ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnUserKilled( User * pcUser )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUserKilled( pcUser ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnUserKilled( User * pcKiller, User * pcVictim )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUserKilled( pcKiller, pcVictim ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnUserKilled( Unit * pcUnit, User * pcUser )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUserKilled( pcUnit, pcUser ) )
			return TRUE;

	return FALSE;

}

BOOL CServerEventHandler::OnUserMapChanged( User * pcUser, EMapID eMapID )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUserMapChanged( pcUser, eMapID ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnUserGetMapItem( User * pcUser, EItemID eItemID )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnUserGetMapItem( pcUser, eItemID ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::CannotSave( User * pcUser )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->CannotSave( pcUser ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnNPCClick( User * pcUser, Unit * pcUnit )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnNPCClick( pcUser, pcUnit ) )
			return TRUE;

	return FALSE;
}

BOOL CServerEventHandler::OnDisconnected( User * pcUser )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->OnDisconnected( pcUser ) )
			return TRUE;

	return FALSE;
}

void CServerEventHandler::Update( float fTime )
{
	for ( auto pcMapEvent : vMapEvents )
		pcMapEvent->Update( fTime );
}

void CServerEventHandler::Update( ETickType eTick )
{
	for ( auto pcMapEvent : vMapEvents )
		pcMapEvent->Update( eTick );
}

BOOL CServerEventHandler::HandlePacket( User * pcUser, Packet * psPacket )
{
	for ( auto pcMapEvent : vMapEvents )
		if ( pcMapEvent->HandlePacket( pcUser, psPacket ) )
			return TRUE;

	return FALSE;
}