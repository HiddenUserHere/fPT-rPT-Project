#pragma once

#define SOCKBUFF_SIZE			8192
#define TOTALPACKET_SEND		10

class Socket;

struct PacketReceiving
{
	int													iLength;
	int													iOpcode;
	char												baPacket[SOCKBUFF_SIZE + 256];
};

struct PacketSending
{
	int													iLength;
	int													iOpcode;
	BYTE												baPacket[SOCKBUFF_SIZE - 8];
};

class SocketData
{
public:

	SocketData();

	SocketData									* pcThis;
	BOOL										bInUse;
	SOCKET										sSocket;

	class UserServer							* u  = NULL;
	class UserdataServer						* ud = NULL;

	char										szIP[16];
	DWORD										dwIP;
	int											iPort;

	BOOL										bConnected = FALSE;

	BOOL										bKeepAlive;

	HANDLE										hRecvThread;
	HANDLE										hSendThread;

	HANDLE										hSendThreadSignal;
	HANDLE										hReceiveThreadSignal;

	DWORD										iRecvLastError;
	DWORD										iSendLastError;

	// Data
	PacketReceiving								* psPacketRecv = NULL;
	char										szSendBuff[(SOCKBUFF_SIZE + 256) * TOTALPACKET_SEND];
	int											iPacketsSend;


	DWORD										dwTimeLastPacketReceived;

	int											iLastReceiveLength;
	int											iNextPacketReceiverPointer;
	int											iNextPacketSendPointer;

	BOOL										bReceivedPacket;

	void										Open( SOCKET s, sockaddr_in * addr );

	BOOL										Connect( const char * pszIP, int iPort );

	void										Close();

	void										EncryptPacket( BYTE bKey, void * pPacket, void * pEncPacket );
	void										DecryptPacket( BYTE bKey, void * pPacket, void * pDecPacket );

	PacketReceiving								* ReceivePacket();

	PacketSending								* NextPacketSending();

	void										Send( Packet * psPacket, BOOL bEncrypt );
	BOOL										Send( PacketSending * psPacket );

	void										FreePacketReceiving( PacketReceiving * psPacket );
	void										FreePacketSending( PacketSending * psPacket );

private:
	int											GetFreePacketSend();

};

