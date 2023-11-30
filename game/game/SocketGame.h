#pragma once

#include "CIocpGameHandler.h"

class SocketData;

struct PacketReceiving;
struct PacketSending;

#define SOCKETGAME					(SocketGame::GetInstance())

#define SOCKETL						SOCKETGAME->GetSocketL()
#define SOCKETG						SOCKETGAME->GetSocketG()

#define SENDPACKETL					SOCKETGAME->SendPacketLogin
#define SENDPACKETG					SOCKETGAME->SendPacketGame

#define SOCKETPACKET(s,p)			SocketGame::GetInstance()->SocketPacket(s,p);
#define SOCKETCLOSE(s)				SocketGame::GetInstance()->SocketClose(s);

#define MAX_CONNECTIONS				2

#define SOCKETSEND_DEBUGCOUNT		(*(DWORD*)0x03A97600)

#define SOCKET_NETSTANDCOUNT		(*(DWORD*)0x03A97794)

#define SOCKET_RECEIVEGAMESERVER	(*(BOOL*)0x009CA9C8)

class SocketGame
{
private:
	static SocketGame				* pcInstance;

	bool							  bActive;
	HWND							  hWnd;

	SocketData						* pcaSocketData;
	std::vector<CIocpGameHandler *>	vIocpClients;

	SocketData						**pcSocketDataL;
	SocketData						**pcSocketDataG;
	SocketData						**pcSocketDataG2;
	SocketData						**pcSocketDataG3;

	int								  iDisconnectCode;

	BOOL							  bReservePackets;
	BOOL							  bHandlingReservedPackets;
	std::vector<struct PacketReserved*>	  vReservedPackets;

	MinMax								sPingLogin;
	MinMax								sPingGame;

public:
	static void						  CreateInstance() { pcInstance = new SocketGame(); }
	static SocketGame				* GetInstance() { return pcInstance; }
	static void						  DeleteInstance() { delete pcInstance; }

	SocketGame();
	virtual							 ~SocketGame();

	void							  Load( HWND hWnd );
	void							  UnLoad();

	void							  Loop();
	void							  Tick( ETickType iTickType );
	void							  PHPing( PacketPing * psPacket, SocketData * pcSocketData );

	void							  PingSocket( SocketData * sd, DWORD dwTime );

	void							  PingConnections();

	char							* GetServerTypeName( SocketData * sd, int iOptionalCode = 0 );

	BOOL							  IsReservePackets();
	void							  SetReservePackets( BOOL b );
	BOOL							  IsHandlingReservedPackets();

	void							  HandlePacket( SocketData * sd, PacketReceiving * p );

	void							  SocketPacket( SocketData * sd, PacketReceiving * p );

	void								SetIPPort( const char * pszIP, int iPort );

	void							  SocketClose( SocketData * sd );

	SocketData						* GetFreeSocketData();
	SocketData						* GetSocketData( const char * pszIP, int iPort );
	SocketData						* GetSocketData( INT64 iID );

	CIocpGameHandler				* GetIocpHandler( SocketData * sd );
	void							  AddIocpHandler( SocketData * sd );
	void							  RemoveIocpHandler( SocketData * sd );

	void							  ConnectServerL( const char * pszIP, int iPort );
	void							  ConnectServerG( const char * pszIP, int iPort );
	void							  CloseServerL();
	void							  CloseServerG();
	void							  CloseConnections();

	int								  GetConnectionsAlive();
	HWND							  GetHWND() { return hWnd; }

	void							  Active( bool b ) { bActive = b; }
	bool							  Active() { return bActive; }

	bool							  ConnectSocket( SocketData * sd, const char * pszIP, int iPort );

	static SocketData				* Connect( const char * pszIP, int iPort );
	static DWORD WINAPI				  Receiver( SocketData * sd );
	static DWORD WINAPI				  Sender( SocketData * sd );

	int								  GetDisconnectCode() { return iDisconnectCode; }
	void							  SetDisconnectCode( int i );

	inline void						  SetSocketL( SocketData * p ) { *pcSocketDataL = p; }
	inline void						  SetSocketG( SocketData * p ) { *pcSocketDataG = p; *pcSocketDataG2 = p; *pcSocketDataG3 = p; }
	inline SocketData				* GetSocketL() { return *pcSocketDataL; }
	inline SocketData				* GetSocketG() { return *pcSocketDataG; }

	inline MinMax					& PingLogin() { return sPingLogin; }
	inline MinMax					& PingGame() { return sPingGame; }

	inline int						  GetAveragePingLogin() { return (sPingLogin.sMin + sPingLogin.sMax) >> 1; }
	inline int						  GetAveragePingGame() { return (sPingGame.sMin + sPingGame.sMax) >> 1; }

	static void						RequestOpenConnection( bool bGameRequest, int iSleep );

	BOOL							SendPacketLogin( Packet * psPacket, BOOL bEncrypted = FALSE );
	BOOL							SendPacketGame( Packet * psPacket, BOOL bEncrypted = FALSE );

	BOOL							SendPacket( SocketData * pcSocketData, Packet * psPacket, BOOL bEncrypted = FALSE );
};