#pragma once

#include "CServer.h"

#include "servercore.h"

#include "common.h"
#include "mapserver.h"
#include "userserver.h"
#include "packetserver.h"
#include "accountserver.h"
#include "characterserver.h"
#include "sqlconnection.h"
#include "netserver.h"
#include "pvpserver.h"
#include "socketserver.h"
#include "servercommand.h"
#include "itemserver.h"
#include "questserver.h"
#include "eventserver.h"
#include "bellatraserver.h"
#include "blesscastleserver.h"
#include "characterserver.h"
#include "unitserver.h"
#include "chatserver.h"
#include "cheatserver.h"
#include "logserver.h"
#include "SQLSkill.h"
#include "unitinfo.h"

#include "CConfigFileReader.h"

extern BOOL g_bLoginServer;
extern BOOL g_bGameServer;

#define GSERVER					Server::GetInstance()

#define SERVER_TYPE				(GSERVER->GetServerType())
#define LOGIN_SERVER			g_bLoginServer
#define GAME_SERVER				g_bGameServer

#define SERVER_PORT				(GSERVER->GetServerInfo()->iPort)

#define TICKCOUNT				Server::TickCount()

#define SERVER_MUTEX			Server::GetInstance()->GetMutex()

#define SOCKETSERVER					(GSERVER->GetSocketServer())
#define	PACKETSERVER					(GSERVER->GetPacketServer())
#define	ACCOUNTSERVER					(GSERVER->GetAccountServer())
#define	CHARACTERSERVER					(GSERVER->GetCharacterServer())
#define CHATSERVER						(GSERVER->GetChatServer())
#define SERVERCOMMAND					(GSERVER->GetServerCommand())
#define UNITSERVER						(GSERVER->GetUnitServer())
#define USERSERVER						(GSERVER->GetUserServer())
#define SQLSKILL						(GSERVER->GetSQLSkill())
#define NETSERVER						(GSERVER->GetNetServer())
#define ITEMSERVER						(GSERVER->GetItemServer())
#define LOGSERVER						(GSERVER->GetLogServer())
#define CHEATSERVER						(GSERVER->GetCheatServer())
#define MAPSERVER						(GSERVER->GetMapServer())
#define QUESTSERVER						(GSERVER->GetQuestServer())
#define UNITINFODATA					(GSERVER->GetUnitInfoServer())
#define BELLATRASERVER					(GSERVER->GetBellatraServer())
#define PVPSERVER						(GSERVER->GetPvPServer())
#define BLESSCASTLESERVER				(GSERVER->GetBlessCastleServer())

class Server : public ServerCore
{
private:
	static Server				* pcInstance;

	EServerType					  iServerType;
	SYSTEMTIME					  sLocalTime;

	HINSTANCE					  hInstance;
	HWND						  hWnd;

	CMutex						* pcMutex;

	int							  iServers;
	ServerInfo					  saServerInfo[MAX_SERVERINFO];
	ServerInfo					* psServerInfo;

	bool						  bMaintenanceCountdown;
	int							  iMaintenanceCountdown;
	DWORD						  dwLastAnnounce;

	int							  iGameVersion;
	int							  iServerVersion;

	PacketServerList			  sPacketServerList;

	//Servers
	SocketServer					* pcSocketServer;
	PacketServer					* pcPacketServer;
	AccountServer					* pcAccountServer;
	SQLConnection					* pcSQLServerConn;
	CharacterServer					* pcCharacterServer;
	ChatServer						* pcChatServer;
	ServerCommand					* pcServerCommand;
	UnitServer						* pcUnitServer;
	UserServer						* pcUserServer;
	CSQLSkill						* pcSQLSkill;
	NetServer						* pcNetServer;
	ItemServer						* pcItemServer;
	LogServer						* pcLogServer;
	CheatServer						* pcCheatServer;
	MapServer						* pcMapServer;
	QuestServer						* pcQuestServer;
	UnitInfoServer					* pcUnitInfoServer;
	BellatraServer					* pcBellatraServer;
	BlessCastleServer				* pcBlessCastleServer;
	PvPServer						* pcPvPServer;

public:
	static void					  CreateInstance() { pcInstance = new Server(); }
	static Server				* GetInstance() { return pcInstance; }
	static void					  DeleteInstance() { delete pcInstance; }

	static void					CreateSQLConnection( EDatabaseID eID );

	Server();
	virtual						 ~Server();

	void						  Init(HWND hWnd);
	void						  UnInit();
	void						  Load();
	void						  UnLoad();

	void						  Begin();
	void						  End();

	void						  Tick(ETickType iTickType);
	void						  Loop();

	void						  Time(double fTime);

	void						  LoadServerInfo(CConfigFileReader * pcConfigFileReader, ServerInfo * si, EServerType eServerType, string strSection);

	inline EServerType			  GetServerType() { return iServerType; }
	inline SYSTEMTIME			* GetServerTime() { return &sLocalTime; }

	CMutex						* GetMutex() { return pcMutex; }

	ServerInfo					* GetServerInfo() { return psServerInfo; }

	char						* GetServerName(int iIndex);
	int							  GetServerIndex(char * pszIP, int iPort);

	EGameLevel					  GetGameLevel(char * pszIP);

	int							  GetGameVersion() { return iGameVersion; }
	void						  SetGameVersion(int i) { iGameVersion = i; }
	int							  GetServerVersion() { return iServerVersion; }
	void						  SetServerVersion(int i) { iServerVersion = i; }

	void						  StartMaintenanceCountdown(int iSeconds);
	void						  StopMaintenanceCountdown();
	void						  AnnounceMaintenanceCountdown();

	BOOL						  SQLGetMaintenace();

	ID							  GetNextID();

	void						  PHServerList(SocketData * pcSocketData, int iTicket, EAccountShare eAccountShare);
	void						  PHBossTime(User * pcUser);
	void						  PHLocalTime(User * pcUser);
	void						  PHSiegeTime(User * pcUser);

	static DWORD				  TickCount() { return GetTickCounts(); }
	static void					  TickCount(DWORD dw) { SetTickCounts(dw); }

	//SQL
	static void					  SQLUserOnline(char * pszAccountName, char * pszCharacterName, char * pszIP, int iTicket, EUsersOnline iAction, int iCharacterClass, int iCharacterLevel);
	static void					  SQLUpdateUserOnline(char * pszAccountName, char * pszGameServerName);
	static int					  SQLGetStatus();
	static void					  SQLSetStatus(int iStatus);

	SocketServer					* GetSocketServer() { return pcSocketServer; }
	PacketServer					* GetPacketServer() { return pcPacketServer; }
	AccountServer					* GetAccountServer() { return pcAccountServer; }
	SQLConnection					* GetSQLServerConnection() { return pcSQLServerConn; }
	CharacterServer					* GetCharacterServer() { return pcCharacterServer; }
	ChatServer						* GetChatServer() { return pcChatServer; }
	ServerCommand					* GetServerCommand() { return pcServerCommand; }
	UnitServer						* GetUnitServer() { return pcUnitServer; }
	UserServer						* GetUserServer() { return pcUserServer; }
	CSQLSkill						* GetSQLSkill() { return pcSQLSkill; }
	NetServer						* GetNetServer() { return pcNetServer; }
	ItemServer						* GetItemServer() { return pcItemServer; }
	LogServer						* GetLogServer() { return pcLogServer; }
	CheatServer						* GetCheatServer() { return pcCheatServer; }
	MapServer						* GetMapServer() { return pcMapServer; }
	QuestServer						* GetQuestServer() { return pcQuestServer; }
	UnitInfoServer					* GetUnitInfoServer() { return pcUnitInfoServer; }
	BellatraServer					* GetBellatraServer() { return pcBellatraServer; }
	BlessCastleServer				* GetBlessCastleServer() { return pcBlessCastleServer; }
	PvPServer						* GetPvPServer() { return pcPvPServer; }
};
