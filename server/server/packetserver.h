#pragma once
#include "userdataserver.h"

#define SENDPACKET PacketServer::Send
#define SENDPACKETSOCKET PacketServer::SendPacketSocket
#define SENDPACKETBLANK PacketServer::SendPacketBlank
#define SENDPACKETSTAGE PacketServer::SendPacketStage
#define SENDPACKETRANGE PacketServer::SendPacketRange

struct PacketSyncData
{
	int											iSize;
	int											iHeader;

	struct CharacterData							CharInfo;

	DWORD	dwObjectID;

	int	x, y, z;				//���� ��ǥ
	int ax, ay, az;			//���� ����
	int state;				//�Ӽ�
};

class PacketServer
{
public:
	PacketServer();
	virtual ~PacketServer();

	BOOL										AnalyzePacket( User * pcUser, PacketReceiving * p );

	static void									Send( User * pcUser, void * pPacket, BOOL bEncrypted = FALSE );
	static void									SendPacketSocket( SocketData * pcSocketData, void * pPacket, BOOL bEncrypted = FALSE );
	static void									SendPacketSocketConnect( SocketData * pcSocketData, void * pPacket, BOOL bEncrypted = FALSE );
	static void									SendPacketBlank( User * pcUser, int iPacket, BOOL bEncrypted = FALSE );
	static void									SendPacketStage( void * pPacket, int iStageID, BOOL bEncrypt = FALSE );
	static void									SendPacketRange( void * pPacket, Point3D * psPos, int iDistance, BOOL bEncrypted = FALSE );

private:

};