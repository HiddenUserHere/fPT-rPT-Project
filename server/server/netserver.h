#pragma once


struct NetConnection
{
	class SocketData							* pcSocketData;
	int											iServerType;
	char										szIP[32];
	int											iPort;
};


class NetServer
{

protected:
	BOOL											bInit			= FALSE;
	WORD											wTimeMinute		= 0;
	BOOL											bReceivedBack	= FALSE;
private:

	WSADATA										wsaData;
	SOCKET										SocketData = INVALID_SOCKET;

	std::vector<NetConnection*>					vNetConnections;

	void										OnReceiveServer( UserData * pcUserData, void * pPacket );
	void										OnReceiveClient( UserData * pcUserData, void * pPacket );

	

public:



	NetServer();
	virtual ~NetServer();

	NetConnection								* GetLoginServerConnection();

	void										SendPacket( class SocketData * pcSocket, void * packet );
	void										SendPacketGameServers( void * packet );

	BOOL										Disconnected( class SocketData * pcSocketData );
	void										Reconnect( class SocketData * pcSocketData );
	void										Connect( NetConnection * nc );
	void										AddConnection( char * pszIP, int iPort, int iServerType );
	NetConnection *								AddNetConnection( class SocketData * pcSocketData );

	void										ConnectAll();
	void										DisconnectAll();

	void										SendBotAdd( void * pPacket );
	void										SendExp( UserData * pcUserData, INT64 iExp );
	void										GiveEXP( UserData * pcUserData, INT64 iExp );
	void										SendExpLogin( UserData * pcUserData, INT64 iExp );
	void										SendForceOrb( UserData * pcUserData, BOOL bCity );
	void										SendEXPPotion( UserData * pcUserData, int iPercent );
	void										SendEXPEventLogin( int iPercent );
	void										SendClan( UserData * pcUserData );
	void										SendSyncChar( UserData *pcUserData );
	void										SendGameMaster( UserData * pcUserData );
	void										OnGetGameMaster( UserData * pcUserData );
	void										DisconnectUser( UserData * pcUserData );
	void										OnUnloadUser( UserData * pcUserData );
	void										GetExp( UserData * pcUserData );
	void										SendVersion( int iVersion );
	void										SendTradeCoin( int iID, int iReceiverID );
	void										SendQuestData( User * pcUser );
	void										SendQuestUpdateData( User * pcUser );
    void										SendHardwareID( User * pcUser );
    void										SendUpdateMessageEvent();

	void										SendPvPBuff( std::vector<int> vID );
	void										ReceivePvPBuff( PacketNetPvPBuff * psPacket );

	void										SendPvPResetLogin();
	void										SendPvPUpdateGameServer( PacketNetUpdatePvPBellatraBuff * psPacket );

	void										SendBellatraRequest( User * pcUser, int iGold );
	void										SendBellatraGiveGold( User * pcUser, int iGold );

	void										HandlePvPBufferLogin( bool bNotifyReset, bool bAddBuff );
	void										ReceiveHandlePvPBuffer( PacketNetPvPLoginHandle * psPacket );

	void										SendActionFieldEvent( BOOL b );

	void										SendBattleRoyaleStatus( User * pcUser );
	void										SendBattleRoyaleEnter( User * pcUser );
	void										SendActionFieldRequest( PacketNetActionFieldRequest * psPacket, BOOL bSendToLogin );

	void										SendEXPEventQuestFree(PacketNetQuestEXPFree * psPacket);

	NetConnection								* GetNetConnectionBySocketData( class SocketData * sd );

	void										OnReceivePacket( UserData * pcUserData, void * pPacket );

	void										Init();


	void										Tick();
};

