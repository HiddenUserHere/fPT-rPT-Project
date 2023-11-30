#pragma once

enum EGameLevel;

#define MAX_SERVERINFO		5
#define MAX_GAMEMASTERINFO	20

#define WAREHOUSE_ITEM_MAX 405

enum EServerType
{
	SERVERTYPE_Unknown = 0,
	SERVERTYPE_Login = 1,
	SERVERTYPE_Game = 2,
	SERVERTYPE_Multi = 3,
};

enum EUsersOnline
{
	USERSONLINE_Insert = 0,
	USERSONLINE_Delete = 1,
	USERSONLINE_DeleteAll = 2
};

struct ServerInfo
{
	EServerType		iType;
	char			szName[32];
	in_addr			lIP;
	char			szIP[32];
	in_addr			lIP2;
	char			szIP2[32];
	in_addr			lIP3;
	char			szIP3[32];
	in_addr			lNetIP;
	char			szNetIP[32];
	int				iPort;
	int				iMaxUsers;
};

class ServerCore
{
private:
	static const	DWORD		pfnHash = 0x00415F80;
public:
	static DWORD *				pdwTickCount;
	static DWORD *				pdwUnitsOnline;
	static DWORD *				pdwUsersOnline;

	static inline DWORD			GetTickCounts()
	{
		return (*pdwTickCount);
	}
	static inline void			SetTickCounts( DWORD dw )
	{
		(*pdwTickCount) = dw;
	}
	static inline DWORD			GetUnitsOnline()
	{
		return (*pdwUnitsOnline);
	}
	static inline void			SetUnitsOnline( DWORD dw )
	{
		(*pdwUnitsOnline) = dw;
	}
	static inline DWORD			GetUsersOnline()
	{
		return (*pdwUsersOnline);
	}
	static inline void			SetUsersOnline( DWORD dw )
	{
		(*pdwUsersOnline) = dw;
	}
	static void					LoadDirty();
	static void					UnLoadDirty();
	static long					Hash( char * s );

	static void					UpdateItemLevel( Item * psItem );

	static void					UpdateCharacterItemsLevelData( const char * pszFile );
	static void					UpdateCharacterItemsLevelWarehouse( const char * pszFile );
	static void					UpdateCharacterItemsLevel( const char * pszBaseFolder, const char * pszName );


	static void					UpdateCharacterItemsLevelAll();
};