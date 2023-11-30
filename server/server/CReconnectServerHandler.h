#pragma once

#define MAX_RECONNECT_USERS			64


class CReconnectServerHandler
{
public:
	CReconnectServerHandler();
	virtual ~CReconnectServerHandler();

	void							Init();

	BOOL							AddUser( User * pcUser );

	void							HandlePacket( User * pcUser, PacketReconnectUser * psPacket );

private:
	User							* pcaUser;
	UserData						* pcaUserData;
	CharacterFile					* psaCharacterFile;

	User							* GetUser( User * pcUser );
	User							* GetUser( int iID );

	BOOL							FreeUser( User * pcUser );

	int								GetIndex( User * pcUser );

	User							* GetFreeUser();

	CharacterFile					* GetNewCharacterFile( int iIndex );
};

