#include "stdafx.h"
#include "SQLConnection.h"

std::vector<SQLConnection*> vSQLConnection;

BOOL show_errorSQL( unsigned int handletype, const SQLHANDLE& handle )
{
	static SQLCHAR sqlstateSqlError[ 1024 ];
	static SQLCHAR messageSqlError[ 1024 ];
	static char szBufSQLError[ 2048 ];
	ZeroMemory( szBufSQLError, 2048 );
	ZeroMemory( sqlstateSqlError, 1024 );
	ZeroMemory( messageSqlError, 1024 );
	if ( SQL_SUCCESS == SQLGetDiagRecA( handletype, handle, 1, sqlstateSqlError, NULL, messageSqlError, 1024, NULL ) )
	{
		STRINGFORMAT( szBufSQLError, "Message: %s\nSQLSTATE: %s\n", messageSqlError, sqlstateSqlError );
		WRITEERR( szBufSQLError );
		return TRUE;
	}
	return FALSE;
}

DatabaseStructureData saDatabaseData[]
{
	{ DATABASEID_GameDB,	"GameDB" },
	{ DATABASEID_UserDB,	"UserDB" },
	{ DATABASEID_ServerDB,	"ServerDB" },
	{ DATABASEID_LogDB,		"LogDB" },
	{ DATABASEID_SkillDB,	"SkillDB" },
	{ DATABASEID_EventDB,	"EventDB" },
	{ DATABASEID_ClanDB,	"ClanDB" },
	{ DATABASEID_SkillDBNew,	"SkillDBNew" },

};


SQLConnection::SQLConnection()
{
	eID = DATABASEID_None;

	for (int i = 0; i < MAX_PARAMS; i++)
		baBlock[i] = false;

	for (int i = 0; i < MAX_PARAMS; i++)
		iaBlock[i] = 0;

	//Create mutex
	pcMutex = new CMutex( "SQLConnection" );
}


SQLConnection::~SQLConnection()
{
	DELET( pcMutex );
}

DatabaseStructureData * SQLConnection::GetDatabaseData( EDatabaseID eDatabaseID )
{
	if ( GSERVER->GetServerType() != SERVERTYPE_Multi )
	{
		for ( auto & v : saDatabaseData )
		{
			if ( v.eDatabaseID == eDatabaseID )
				return &v;
		}
	}
	else
	{
		for ( auto & v : saDatabaseData )
		{
			if ( v.eDatabaseID == eDatabaseID )
				return &v;
		}
	}

	return NULL;
}

SQLINTEGER * SQLConnection::BlockNext()
{
	for ( int i = 0; i < MAX_PARAMS; i++ )
	{
		if ( baBlock[i] == false )
		{
			baBlock[i] = true;
			return &iaBlock[i];
		}
	}

	return NULL;
}

void SQLConnection::BlockFree()
{
	for ( int i = 0; i < MAX_PARAMS; i++ )
		baBlock[i] = false;
}

void SQLConnection::Init( EDatabaseID eDatabaseID )
{
	//Copy database name to class
	strcpy_s( szDatabaseName, GetDatabaseData( eDatabaseID )->pszDatabaseName );
	eID = eDatabaseID;
	
	//Try alloc Handle SQL ODBC
	if ( SQL_SUCCESS != SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv ) )
		show_errorSQL( SQL_HANDLE_ENV, hStatement );

	//Set Version ODBC
	if ( SQL_SUCCESS != SQLSetEnvAttr( hEnv, SQL_ATTR_ODBC_VERSION, ( SQLPOINTER )SQL_OV_ODBC3, 0 ) )
		return;

	//Try alloc handle connection
	if ( SQL_SUCCESS != SQLAllocHandle( SQL_HANDLE_DBC, hEnv, &hConnection ) )
		show_errorSQL( SQL_HANDLE_DBC, hStatement );

	//Set connection timeout
	SQLSetConnectAttrA( hConnection, SQL_LOGIN_TIMEOUT, ( SQLPOINTER )5, SQL_IS_INTEGER );

	char szStringConnectionSQL[ 512 ];
	SQLCHAR strconn[512] = { 0 };
	ZeroMemory( szStringConnectionSQL, 512 );

	//Set string connection
	
	if ( FileExist( "testserver.ini" ) )
	{
		STRINGFORMAT( szStringConnectionSQL, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{ODBC Driver 17 for SQL Server}", "DESKTOP-44P2GP6", szDatabaseName, "sa", "pt123@" );
		//Host
		//STRINGFORMAT( szStringConnectionSQL, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{ODBC Driver 17 for SQL Server}", "NS560531", szDatabaseName, "sa", "UeQ8uGz66q" );
	}
	else
	{
		//Host Canada
		STRINGFORMAT( szStringConnectionSQL, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{ODBC Driver 17 for SQL Server}", "NS560531", szDatabaseName, "sa", "pt123@" );
	}

	//PC
	//STRINGFORMAT( szStringConnectionSQL, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{ODBC Driver 17 for SQL Server}", "DESKTOP-44P2GP6", szDatabaseName, "sa", "pt123@" );
	
	//CacktoPC
//	STRINGFORMAT( szStringConnectionSQL, 512, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{SQL Server Native Client 10.0}", "CACKTO\\SQLEXPRESS", szDatabaseName, "sa", "pt123@" );

	//Igor
//	STRINGFORMAT( szStringConnectionSQL, 512, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{SQL Server Native Client 10.0}", "DESKTOP-U8VJKKJ\\SQLEXPRESS", szDatabaseName, "sa", "123456" );
	
// Host

	//STRINGFORMAT( szStringConnectionSQL, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{SQL Server Native Client 11.0}", "LGV-EEB9A375-24\\SQLEXPRESS", szDatabaseName, "sa", "pt765@jvfl" );

	memcpy( strconn, szStringConnectionSQL, 512 );

	SQLCHAR retconstring[ 1024 ];
	
	//Connect to Database
	SQLDriverConnectA( hConnection, NULL, strconn, lstrlenA((char*)strconn), retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT );

	//Create Statement handler
	if ( SQL_SUCCESS != SQLAllocHandle( SQL_HANDLE_STMT, hConnection, &hStatement ) )
		show_errorSQL( SQL_HANDLE_DBC, hStatement );

	SQLSetStmtAttr( hStatement, SQL_ATTR_CURSOR_SCROLLABLE, (SQLPOINTER)SQL_SCROLLABLE, SQL_IS_INTEGER );

	BlockFree();

	vSQLConnection.push_back( this );
}

BOOL SQLConnection::SQLTimeStampToSystemTime( SYSTEMTIME * psSystemTime, SQL_TIMESTAMP_STRUCT * psSQLTimeStamp )
{
	if ( !psSystemTime || !psSQLTimeStamp )
		return FALSE;

	psSystemTime->wYear			= psSQLTimeStamp->year;
	psSystemTime->wMonth		= psSQLTimeStamp->month;
	psSystemTime->wDayOfWeek	= 0;
	psSystemTime->wDay			= psSQLTimeStamp->day;
	psSystemTime->wHour			= psSQLTimeStamp->hour;
	psSystemTime->wMinute		= psSQLTimeStamp->minute;
	psSystemTime->wSecond		= psSQLTimeStamp->second;
	psSystemTime->wMilliseconds = 0;

	return TRUE;
}

void SQLConnection::SQLSystemTimeToTimeStamp( SYSTEMTIME * in, SQL_TIMESTAMP_STRUCT * out )
{
	out->year = in->wYear;
	out->month = in->wMonth;
	out->day = in->wDay;
	out->hour = in->wHour;
	out->minute = in->wMinute;
	out->second = in->wSecond;
	out->fraction = 0;
}

BOOL SQLConnection::Open()
{
	if ( bOpen == FALSE )
		pLastAddress = _ReturnAddress();

	pcMutex->Lock( 1000 + szDatabaseName[0] );

	bOpen = TRUE;
	
	pLastAfterAddress = _ReturnAddress();

	return TRUE;
}

BOOL SQLConnection::Prepare( const char * pszQuery )
{
	if ( !bOpen )
	{
		WRITEERR( "[SQLConnection::Prepare()] : %s [%s]", pszQuery, bOpen ? "TRUE" : "FALSE" );
		return FALSE;
	}

	szQuery[0] = 0;
	
	//Copy Query
	strcpy_s( szQuery, pszQuery );

	//Check SQLI
	for ( int i = 0; i < _countof( szQuery ); i++ )
	{
		//End string? break it
		if ( szQuery[i] == 0 )
			break;

		//Have quote mark? SQLI, return error
		if ( szQuery[i] == 0x27 )
		{
			//Query Not secure
			WRITEERR("Query not secure!");
			return FALSE;
		}
	}

	//Prepare Query error? return
	if ( SQLPrepareA( hStatement, (SQLCHAR*)szQuery, SQL_NTS ) == SQL_ERROR )
	{
		WRITEERR( "SQLPrepareA() - %s", szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}

	bPrepared = TRUE;

	//Success
	return TRUE;
}

BOOL SQLConnection::Execute()
{
	if ( !bOpen || !bPrepared )
	{
		WRITEERR( "[SQLConnection::Execute()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	SQLRETURN iRetCode = SQL_SUCCESS;

	//Try Execute query
	if ( (iRetCode = SQLExecute( hStatement )) == SQL_ERROR )
	{
		WRITEERR( "error %s - %s", szDatabaseName, szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}

	return TRUE;
}

BOOL SQLConnection::Execute( const char * pszQuery )
{
	if ( !bOpen )
	{
		WRITEERR( "[SQLConnection::Execute()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	SQLRETURN iRetCode = SQL_SUCCESS;

	//Try Execute query
	if ( (iRetCode = SQLExecDirectA( hStatement, (SQLCHAR*)pszQuery, SQL_NTS )) == SQL_ERROR )
	{
		WRITEERR( "error %s - %s", szDatabaseName, szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}

	return TRUE;
}

BOOL SQLConnection::BindParameterInput( int iPosition, EDatabaseDataType eDataType, void * pParameter, int iSize )
{
	if ( !bOpen || !bPrepared )
	{
		WRITEERR( "[SQLConnection::BindParameterInput()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	int iType		= 0;
	int iSQLType	= 0;
	int iLen		= 0;

	SQLINTEGER * piInputOutput = BlockNext();
	SQL_TIMESTAMP_STRUCT sTimeStamp;
	int iExtra = 0;
	int icbMax = 0;

	void * pNewParameter = pParameter;

	//Set Data Type
	switch ( eDataType )
	{
		case PARAMTYPE_Integer:
			iType = SQL_C_LONG;
			iSQLType = SQL_INTEGER;
			*piInputOutput = 0;
			break;
		case PARAMTYPE_String:
			iType = SQL_C_CHAR;
			iSQLType = SQL_VARCHAR;
			iLen = lstrlenA( (char*)pParameter );
			*piInputOutput = SQL_NTS;
			break;
		case PARAMTYPE_Float:
			iType = SQL_C_FLOAT;
			iSQLType = SQL_REAL;
			*piInputOutput = 0;
			break;
		case PARAMTYPE_Int64:
			iType = SQL_C_SBIGINT;
			iSQLType = SQL_BIGINT;
			*piInputOutput = 0;
			break;
		case PARAMTYPE_Time:
			iType = SQL_C_TIMESTAMP;
			iSQLType = SQL_DATETIME;
			*piInputOutput = sizeof( SQL_TIMESTAMP_STRUCT );
			SQLSystemTimeToTimeStamp( (SYSTEMTIME*)pParameter, &sTimeStamp );
			iLen = 19;
			iExtra = 0;
			icbMax = sizeof( SQL_TIMESTAMP_STRUCT );
			pNewParameter = &sTimeStamp;
			break;
		case PARAMTYPE_Binary:
			iType = SQL_C_BINARY;
			iSQLType = SQL_BINARY;
			iLen = iSize;
			*piInputOutput = iSize;
			break;
		default:
			break;
	}
	
	//Bind parameters
	if ( SQLBindParameter( hStatement, iPosition, SQL_PARAM_INPUT, iType, iSQLType, iLen, iExtra, pNewParameter, icbMax, piInputOutput ) == SQL_ERROR )
	{
		WRITEERR( "SQLBindParameter() %s - %s", szDatabaseName, szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}
	
	return TRUE;
}

BOOL SQLConnection::GetData( int iPosition, EDatabaseDataType eDataType, void * pParameter, int iSize )
{
	if ( !bOpen || !bPrepared )
	{
		WRITEERR( "[SQLConnection::GetData()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	int iType				= 0;
	SQLINTEGER iDataSize	= 0;
	void * pParam			= pParameter;
	int iSizeParam			= iSize;
	SQL_TIMESTAMP_STRUCT	sTimeStamp;

	//Set Data Type
	switch ( eDataType )
	{
		case PARAMTYPE_Integer:
			iType = SQL_C_LONG;
			break;
		case PARAMTYPE_Short:
			iType = SQL_C_SHORT;
			break;

		case PARAMTYPE_String:
			iType = SQL_C_CHAR;
			break;
		case PARAMTYPE_Float:
			iType = SQL_C_FLOAT;
			break;
		case PARAMTYPE_Int64:
			iType = SQL_C_SBIGINT;
			break;
		case PARAMTYPE_Time:
			iType		= SQL_C_TYPE_TIMESTAMP;
			pParam		= &sTimeStamp;
			iSizeParam	= 23;
			break;
		case PARAMTYPE_Binary:
			iType = SQL_C_BINARY;
			break;
		default:
			break;
	}

	//Get Data
	if ( SQLGetData( hStatement, iPosition, iType, pParam, iSizeParam, &iDataSize ) == SQL_ERROR )
	{
		WRITEERR( "error %d - %s - %s", iPosition, szDatabaseName, szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}

	if ( eDataType == PARAMTYPE_Time )
		SQLTimeStampToSystemTime( (SYSTEMTIME*)pParameter, &sTimeStamp );
	
	return TRUE;
}

BOOL SQLConnection::GetColumnName( int iPosition, void * pParameter, int iSize )
{
	if ( !bOpen || !bPrepared )
	{
		WRITEERR( "[SQLConnection::GetColumnName()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	SQLSMALLINT cbSize = 0;
	SQLSMALLINT cbDataType = 0;
	SQLUINTEGER cbColumn = 0;
	SQLSMALLINT cbScale = 0;
	SQLSMALLINT uNullType = 0;

	if ( SQLDescribeColA( hStatement, iPosition, (SQLCHAR*)pParameter, iSize, &cbSize, &cbDataType, &cbColumn, &cbScale, &uNullType ) == SQL_ERROR )
	{
		WRITEERR( "error %d - %s - %s", iPosition, szDatabaseName, szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}

	WRITEERR( "error %d - %d - %d - %d - %d", iPosition, cbSize, cbDataType, cbColumn, cbScale );


	return TRUE;
}

BOOL SQLConnection::GetColumnCount( int & iCount )
{
	if ( !bOpen || !bPrepared )
	{
		WRITEERR( "[SQLConnection::GetColumnCount()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	iCount = 0;

	SQLSMALLINT cbCount = 0;

	if( SQLNumResultCols(hStatement, &cbCount ) == SQL_ERROR )
	{
		WRITEERR( "error %s - %s", szDatabaseName, szQuery );
		show_errorSQL( SQL_HANDLE_STMT, hStatement );
		return FALSE;
	}

	iCount = (int)cbCount;

	return TRUE;
}

EDatabaseDataType SQLConnection::GetColumnType( const char * pszTableName, const char * pszColumnName )
{
	EDatabaseDataType i = PARAMTYPE_Null;

	char szResult[64] = { 0 };

	if ( Prepare( "SELECT DATA_TYPE FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = ? AND COLUMN_NAME = ?" ) )
	{
		BindParameterInput( 1, PARAMTYPE_String, (char*)pszTableName, STRLEN( pszTableName ) );
		BindParameterInput( 2, PARAMTYPE_String, (char*)pszColumnName, STRLEN( pszColumnName ) );

		if ( Execute() && Fetch() )
		{
			GetData( 1, PARAMTYPE_String, szResult, _countof( szResult ) );

			if ( STRINGCOMPAREI( szResult, "varchar" ) )
				i = PARAMTYPE_String;
			else if ( STRINGCOMPAREI( szResult, "int" ) )
				i = PARAMTYPE_Integer;
			else if ( STRINGCOMPAREI( szResult, "real" ) )
				i = PARAMTYPE_Float;
			else if ( STRINGCOMPAREI( szResult, "bigint" ) )
				i = PARAMTYPE_Int64;
		}
	}

	return i;
}

BOOL SQLConnection::Fetch()
{
	if ( !bOpen || !bPrepared )
	{
		WRITEERR( "[SQLConnection::Fetch()] : %s [%s,%s]", szQuery, bOpen ? "TRUE" : "FALSE", bPrepared ? "TRUE" : "FALSE" );
		return FALSE;
	}

	//Fetch ( Next Row )
	if ( !SQL_SUCCEEDED( SQLFetch( hStatement ) ) )
		return FALSE;

	return TRUE;
}

BOOL SQLConnection::Close()
{
	if ( !bOpen )
		return FALSE;

	Clear();

	bOpen		= FALSE; 

	pcMutex->Unlock();

	return TRUE;
}

BOOL SQLConnection::Clear()
{
	SQLCancel( hStatement );
	SQLFreeStmt( hStatement, SQL_CLOSE );
	SQLFreeStmt( hStatement, SQL_UNBIND );
	SQLFreeStmt( hStatement, SQL_RESET_PARAMS );

	BlockFree();

	//Clear Query
	//ZeroMemory( szQuery, _countof( szQuery ) );

	bPrepared	= FALSE;

	return TRUE;
}

SQLConnection * SQLConnection::GetConnection( EDatabaseID eDatabaseID )
{
	//Find connection pointer
	for ( const auto v : vSQLConnection )
	{
		//Same DatabaseID? return pointer of class
		if ( v->eID == eDatabaseID )
			return v;
	}
	return NULL;
}