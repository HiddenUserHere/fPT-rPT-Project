#pragma once

#include "CServerSocketHandler.h"
#include "CIocpGameHandler.h"

#define NUM_RESERVED_SLOTS								10

#define MAX_DISCONNECT_CONNECTIONS						64
#define MAX_DENYCONNECTIONS								256
#define MAX_BANCONNECTIONS								512
#define MAX_RECONNECTCONNECTIONS						128

#define MAX_DISCONNECT_TICK								4
#define MAX_DISCONNECT_TIME								400

#define MAX_PACKETFLOW 2000
#define TIM_PACKETFLOW 5000
#define MAX_PACKETBYTESFLOW			SocketServer::uBytePacketFlow

#define SOCKETACCEPT(s,a)			SocketServer::GetInstance()->SocketAccept(s,a);
#define SOCKETPACKET(s,p)			SocketServer::GetInstance()->SocketPacket(s,p);
#define SOCKETCLOSE(s)				SocketServer::GetInstance()->SocketClose(s);

struct DisconnectedUser
{
	UINT						lIP;
	int							iDisconnectTimes;
	DWORD						dwTime;
};

struct SocketServerAccept
{
	INT64		 iID;
	sockaddr_in  sAddr;
};

class SocketServer
{
protected:

	const BOOL											IOCP_SERVER_TYPE = TRUE;

	HWND												hWnd;
	WSADATA												sWsaData;
	SOCKET												sListenSocket = INVALID_SOCKET;
	int													iMaxConnections;
	BOOL												bActive = FALSE;
	BOOL												bSetFull = FALSE;
	SocketData											* pcaSocketData;
	CMutex												* pcMutex = NULL;

	HANDLE							  hEventSocket;
	SOCKET							  saSocket[MAX_DENYCONNECTIONS];
	DWORD								dwaIPListBan[MAX_BANCONNECTIONS];
	User												* pcaReconnectedUsers[MAX_RECONNECTCONNECTIONS];
	DisconnectedUser									saDisconnectUser[MAX_DISCONNECT_CONNECTIONS];
	int								  iNextSocket;

	BYTE												bPacketObfuscatorByte = 0;

	CServerSocketHandler								* pcSocketServerHandler = nullptr;

	std::vector<CIocpGameHandler *>						vIocpClients;

public:

	static UINT											uBytePacketFlow;

	CServerSocketHandler								* GetServerSocketHandler() { return pcSocketServerHandler; }

	HANDLE												hListener;
	static SocketServer									* pcInstance;
	static SocketServer									* GetInstance() { return SocketServer::pcInstance; };
	CMutex												* GetMutex() { return pcMutex; };
	static void											CreateInstance() { SocketServer::pcInstance = new SocketServer(); };
	HWND												GetHWND() { return hWnd; };
	BOOL												IsActive() { return bActive; };
	BOOL												IsFull() { return bSetFull; };
	void												SetFull( BOOL b ) { bSetFull = b; };

	SocketData											* GetFreeSocketData();
	int													GetNumFreeSlots();

	SocketData											* GetSocketData( const char * pszIP, int iPort );
	SocketData											* GetSocketData( SOCKET s );
	SocketData											* GetSocketDataByID( INT64 iID );

	CIocpGameHandler									* GetIocpHandler( SocketData * sd );
	void												AddIocpHandler( SocketData * sd );
	void												RemoveIocpHandler( SocketData * sd );

	int													GetConnectionCount( UINT lIP );
	void												DisconnectIP( UINT lIP );

	SocketData											* Connect( const char * pszIP, int iPort );
	void												SocketAccept( SOCKET s, SocketServerAccept * psAccept );
	void												SocketAcceptUDP( SOCKET s, sockaddr_in * addr, UINT uSocketID, UINT uPacketID );
	void												SocketPacket( SocketData * sd, PacketReceiving * p );
	void												SocketClose( SocketData * sd );

	SocketServer();
	virtual ~SocketServer();

	static DWORD WINAPI									Sender( SocketData * sd );
	static DWORD WINAPI									Receiver( SocketData * sd );
	static DWORD WINAPI									Listener( SOCKET * ls );
	static DWORD WINAPI									DeniedSocketsHandler();

	void												HandleDeniedSockets();
	void												HandleBanFirewall();
	void												HandleReconnectedUsers();
	void												HandleConnectionCount();

	static BOOL											AllowPacket( SocketData * sd );

	BOOL												AddDeniedSocket( SOCKET s );
	BOOL												AddBanIP( DWORD dwIP );
	BOOL												IsBannedIP( UINT lIP );
	BOOL												AddDisconnectedUser( UINT lIP );
	BOOL												AddReconnectedUser( User * pcUser );

	void												Load();

	BOOL												Init(HWND hwnd);

	void												Listen( int iPort );

	void												OnReadHandler( SOCKET s, DWORD dwParam );

	HANDLE												GetHandleEventSocket() { return hEventSocket; }

	BYTE												GetPacketObfuscatorByte() { return bPacketObfuscatorByte; }
	void												SetPacketObfuscatorByte( BYTE b ) { bPacketObfuscatorByte = b; }

	SocketData											* GetClientSocket( UINT uSocketID, UINT uPacketID );
};

#define SOCKETMUTEX										(SocketServer::GetInstance()->GetMutex())
