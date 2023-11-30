#pragma once

#include <objbase.h>
#include <vector>
#include <comdef.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

#define MAX_PARAMS		10000
#define MAX_QUERY_SIZE	2048

enum EDatabaseID
{
	DATABASEID_None,
	DATABASEID_GameDB,
	DATABASEID_UserDB,
	DATABASEID_ServerDB,
	DATABASEID_LogDB,
	DATABASEID_SkillDB,
	DATABASEID_EventDB,
	DATABASEID_ClanDB,
	DATABASEID_SkillDBNew,
};

enum EDatabaseDataType
{
	PARAMTYPE_Integer,
	PARAMTYPE_String,
	PARAMTYPE_Float,
	PARAMTYPE_Int64,
	PARAMTYPE_Short,
	PARAMTYPE_Time,
	PARAMTYPE_Binary,
	PARAMTYPE_Null = 5000,
};

struct DatabaseStructureData
{
	EDatabaseID									eDatabaseID;
	char										* pszDatabaseName;
};


class SQLConnection
{
private:
	DatabaseStructureData						* GetDatabaseData( EDatabaseID eDatabaseID );
	SQLHANDLE									hEnv = INVALID_HANDLE_VALUE;
	SQLHANDLE									hConnection = INVALID_HANDLE_VALUE;
	SQLHANDLE									hStatement = INVALID_HANDLE_VALUE;
	EDatabaseID									eID;
	char										szDatabaseName[64] = { 0 };

	char										szQuery[MAX_QUERY_SIZE] = { 0 };

	CMutex										* pcMutex = nullptr;

	BOOL										bOpen		= FALSE;
	BOOL										bPrepared	= FALSE;

	void										* pLastAddress = NULL;
	void										* pLastAfterAddress = NULL;

	bool										baBlock[MAX_PARAMS];
	SQLINTEGER									iaBlock[MAX_PARAMS];
	SQLINTEGER									* BlockNext();
	void										BlockFree();

public:

	void										Init( EDatabaseID eDatabaseID );

	BOOL										SQLTimeStampToSystemTime( SYSTEMTIME * psSystemTime, SQL_TIMESTAMP_STRUCT * psSQLTimeStamp );
	void										SQLSystemTimeToTimeStamp( SYSTEMTIME * in, SQL_TIMESTAMP_STRUCT * out );

	BOOL										Open();
	BOOL										Prepare( const char * pszQuery );
	BOOL										Execute();
	BOOL										Execute( const char * pszQuery );

	BOOL										BindParameterInput( int iPosition, EDatabaseDataType eDataType, void * pParameter, int iSize = 0 );
	BOOL										GetData( int iPosition, EDatabaseDataType eDataType, void * pParameter, int iSize = 0 );
	BOOL										GetColumnName( int iPosition, void * pParameter, int iSize );
	BOOL										GetColumnCount( int & iCount );
	EDatabaseDataType							GetColumnType( const char * pszTableName, const char * pszColumnName );
	BOOL										Fetch();
	BOOL										Close();
	BOOL										Clear();

	static SQLConnection						* GetConnection( EDatabaseID eDatabaseID );

	SQLConnection();
	virtual ~SQLConnection();
};

