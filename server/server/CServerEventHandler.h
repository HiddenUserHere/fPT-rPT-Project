#pragma once

class CServerEvent
{
private:

	friend class CServerEventHandler;

	virtual BOOL					OnUnitKilled( Unit * pcUnit ) { return FALSE; }
	virtual BOOL					OnUnitKilled( User * pcUser, Unit * pcUnit ) { return FALSE; }

	virtual BOOL					OnUserKilled( User * pcUser ) { return FALSE; }
	virtual BOOL					OnUserKilled( User * pcKiller, User * pcVictim ) { return FALSE; }
	virtual BOOL					OnUserKilled( Unit * pcUnit, User * pcUser ) { return FALSE; }

	virtual BOOL					OnDisconnected( User * pcUser ) { return FALSE; }

	virtual BOOL					OnUserMapChanged( User * pcUser, EMapID eMapID ) { return FALSE; }

	virtual BOOL					OnUserGetMapItem( User * pcUser, EItemID eItemID ) { return FALSE; }

	virtual BOOL					CannotSave( User * pcUser ) { return FALSE; }

	virtual BOOL					OnNPCClick( User * pcUser, Unit * pcUnit ) { return FALSE; };

	virtual void					Update( float fTime ) {};
	virtual void					Update( ETickType eTick ) {};

	virtual BOOL					HandlePacket( User * pcUser, Packet * psPacket ) { return FALSE; }

};

class CServerEventHandler
{
private:
	std::vector<CServerEvent *>		vMapEvents;

public:
	CServerEventHandler();
	virtual ~CServerEventHandler();

	void							AddHandler( CServerEvent * pcServerEvent ) { vMapEvents.push_back( pcServerEvent ); }

	BOOL							OnUnitKilled( Unit * pcUnit );
	BOOL							OnUnitKilled( User * pcUser, Unit * pcUnit );

	BOOL							OnUserKilled( User * pcUser );
	BOOL							OnUserKilled( User * pcKiller, User * pcVictim );
	BOOL							OnUserKilled( Unit * pcUnit, User * pcUser );

	BOOL							OnUserMapChanged( User * pcUser, EMapID eMapID );

	BOOL							OnUserGetMapItem( User * pcUser, EItemID eItemID );

	BOOL							CannotSave( User * pcUser );

	BOOL							OnNPCClick( User * pcUser, Unit * pcUnit );

	BOOL							OnDisconnected( User * pcUser );

	void							Update( float fTime );
	void							Update( ETickType eTick );

	BOOL							HandlePacket( User * pcUser, Packet * psPacket );
};