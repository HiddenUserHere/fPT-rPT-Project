#pragma once
class CFollowUnitHandler
{
public:
	CFollowUnitHandler();
	virtual ~CFollowUnitHandler();

	bool							CanEnterOnMap( EMapID iMapID );
	bool							CanWalkOnMap( EMapID iMapID );

	bool							SpawnUnit( User * pcUser, const char * pszUnitName );

	bool							IsFollowUnitType( Unit * pcUnit );

	void							UpdateMap( Unit * pcUnit, EMapID iMapID, Point3D sPosition );

	void							OpenCaravan( User * pcUser );

	void							SendOpenCaravan( User * pcUser, Unit * pcUnit );

	void							HandlePacket( User * pcUser, PacketCaravanOpen * psPacket );
	void							HandlePacket( User * pcUser, PacketCaravan * psPacket );
	void							HandlePacket( User * pcUser, PacketCaravanUpdate * psPacket );

	void							OnDisconnected( User * pcUser );
	void							OnItemTimerEnd( User * pcUser );

	void							ProcessPacket( User * pcUser, PacketCaravanOpen * psPacket );

private:

	void							Receive( User * pcUser, PacketCaravan * psPacket );

	void							SaveCaravan( User * pcUser );

	void							SendCaravan( User * pcUser, CaravanData * psCaravanData );

	bool							UpdateCaravanItems( User * pcUser );

	void							ProcessPacket( User * pcUser, PacketCaravan * psPacket );
	void							ProcessPacket( User * pcUser, PacketCaravanUpdate * psPacket );

	std::string						SQLGetCaravanName( User * pcUser );

	bool							SQLHaveCaravan( User * pcUser );

	void							SQLNewCaravan( User * pcUser );
	void							SQLUpdateCaravanName( User * pcUser );
	bool							SQLGetCaravanItems( CaravanData * psCaravanData, int iCaravanID );
	void							SQLDeleteCaravanItems( User * pcUser );
};

