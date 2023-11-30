
#include "SQLAPI.h"

bool bCreatedConnectionSQL = false;

CRITICAL_SECTION								CriticalSectionSqlData;
BOOL											bCreatedCSectionSQL = FALSE;

struct stSQLDataConn
{
	SQLHANDLE sqlenvhandle;
	SQLHANDLE sqlconnectionhandle;
	SQLHANDLE sqlstatementhandle;
	char szDBName[ 64 ];
};

std::vector<stSQLDataConn> vtSQLData;


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
		wsprintfA( szBufSQLError, "Message: %s\nSQLSTATE: %s\n", messageSqlError, sqlstateSqlError );
		WRITEDBG( szBufSQLError );
		return TRUE;
	}
	return FALSE;
}

int SQLAPI::findvtSQLData( const char *DB )
{
	int sizevt = vtSQLData.size();
	for ( int i = 0; i < sizevt; i++ )
	{
		if ( COMPAREISTRING( DB, vtSQLData[ i ].szDBName ) )
			return i;
	}
	return -1;
}



void SQLAPI::Init( const char * DB )
{
	stSQLDataConn sqlST;
	vtSQLData.push_back( sqlST );

	int iPos = vtSQLData.size() - 1;
	this->iPosDataSQL = iPos;

	CReader cReader( "server.ini" );

	if ( SQL_SUCCESS != SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &vtSQLData[ iPos ].sqlenvhandle ) )
		show_errorSQL( SQL_HANDLE_ENV, vtSQLData[ iPos ].sqlstatementhandle );

	if ( SQL_SUCCESS != SQLSetEnvAttr( vtSQLData[ iPos ].sqlenvhandle, SQL_ATTR_ODBC_VERSION, ( SQLPOINTER )SQL_OV_ODBC3, 0 ) )
		return;

	SQLSetStmtAttr( vtSQLData[ iPos ].sqlenvhandle, SQL_ATTR_CURSOR_TYPE, ( SQLPOINTER )SQL_CURSOR_DYNAMIC, 0 );

	if ( SQL_SUCCESS != SQLAllocHandle( SQL_HANDLE_DBC, vtSQLData[ iPos ].sqlenvhandle, &vtSQLData[ iPos ].sqlconnectionhandle ) )
		show_errorSQL( SQL_HANDLE_DBC, vtSQLData[ iPos ].sqlstatementhandle );

	SQLSetConnectAttrA( vtSQLData[ iPos ].sqlconnectionhandle, SQL_LOGIN_TIMEOUT, ( SQLPOINTER )5, 0 );

	char szStringConnectionSQL[ 512 ];
	SQLCHAR strconn[ 512 ];
	ZeroMemory( szStringConnectionSQL, 512 );

	std::string pszDriver = cReader.ReadString( "Database", "Driver" );
	std::string pszUser = cReader.ReadString( "Database", "User" );
	std::string pszPass = cReader.ReadString( "Database", "Password" );
	std::string pszServer = cReader.ReadString( "Database", "Host" );
	wsprintfA( szStringConnectionSQL, "Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", pszDriver.c_str(), pszServer.c_str(), DB, pszUser.c_str(), pszPass.c_str() );

	memcpy( strconn, szStringConnectionSQL, 512 );


	SQLCHAR retconstring[ 1024 ];
	SQLDriverConnectA( vtSQLData[ iPos ].sqlconnectionhandle, NULL, strconn, SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT );

	if ( SQL_SUCCESS != SQLAllocHandle( SQL_HANDLE_STMT, vtSQLData[ iPos ].sqlconnectionhandle, &vtSQLData[ iPos ].sqlstatementhandle ) )
		show_errorSQL( SQL_HANDLE_DBC, vtSQLData[ iPos ].sqlstatementhandle );

	lstrcpyA( vtSQLData[ iPos ].szDBName, DB );

}

int SQLAPI::NextRow()
{
	if ( SQL_SUCCESS != SQLFetch( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle ) )
	{
		show_errorSQL( SQL_HANDLE_STMT, &vtSQLData[ this->iPosDataSQL ].sqlstatementhandle );
	}
	this->iRowPosition++;
	return 1;
}

int SQLAPI::GetData( char * pBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( sizedatasql == SQL_NULL_DATA )
	{
		lstrcpyA( pBuf, "NULL" );
		return 1;
	}


	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( pBuf, ( char* )buf );
	else
		lstrcpyA( pBuf, "NULL" );
	return 1;
}



int SQLAPI::GetData( int * piBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( sizedatasql == SQL_NULL_DATA )
	{
		*piBuf = 0;
		return 1;
	}

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*piBuf = atoi( szBuf );

	return 1;
}

int SQLAPI::GetData( unsigned int * piBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*piBuf = ( unsigned int )atoi( szBuf );

	return 1;
}

int SQLAPI::GetData( unsigned long * piBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*piBuf = ( unsigned long )atoi( szBuf );

	return 1;
}

int SQLAPI::GetData( INT64 * piBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*piBuf = (INT64)_atoi64( szBuf );

	return 1;
}

int SQLAPI::GetData( float * pfBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*pfBuf = ( float )atof( szBuf );

	return 1;
}

int SQLAPI::GetData( short * psBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*psBuf = ( short )atoi( szBuf );

	return 1;
}

int SQLAPI::GetData( unsigned short * psBuf, int iColumn )
{
	SQLCHAR buf[ 256 ];
	SQLSMALLINT columns;
	SQLINTEGER sizedatasql;
	int iField = 0;

	char szBuf[ 256 ];

	SQLNumResultCols( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, &columns );
	SQLGetData( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle, iColumn + 1, SQL_C_CHAR, buf, 256, &sizedatasql );

	if ( lstrlenA( ( char* )buf ) > 0 )
		lstrcpyA( szBuf, ( char* )buf );
	else
		lstrcpyA( szBuf, "NULL" );

	*psBuf = ( unsigned short )atoi( szBuf );

	return 1;
}


BOOL SQLAPI::CheckSqlInject( const char * value, int SizeString )
{
	// Check if have ' on string
	int iSize = lstrlenA( value );

	if ( iSize > SizeString )
		return true;

	if ( iSize <= 0 )
		return true;
	for ( int i = 0; i < iSize; i++ )
	{
		if ( value[ i ] == 0x27 )
			return true;
	}
	return false;
}


void SQLAPI::Close()
{
	SQLCancel( vtSQLData[ this->iPosDataSQL ].sqlstatementhandle );
}


void SQLAPI::DisconnectAll()
{
	for ( UINT i = 0; i < vtSQLData.size(); i++ )
	{
		SQLFreeHandle( SQL_HANDLE_STMT, vtSQLData[ i ].sqlstatementhandle );
		SQLDisconnect( vtSQLData[ i ].sqlconnectionhandle );
		SQLFreeHandle( SQL_HANDLE_DBC, vtSQLData[ i ].sqlconnectionhandle );
		SQLFreeHandle( SQL_HANDLE_ENV, vtSQLData[ i ].sqlenvhandle );
	}
	vtSQLData.clear();
	std::vector<stSQLDataConn>( vtSQLData ).swap( vtSQLData );
}


void SQLAPI::Select( const char * DB, const char * Command, ... )
{

	if ( !bCreatedCSectionSQL )
	{
		InitializeCriticalSection( &CriticalSectionSqlData );
		bCreatedCSectionSQL = TRUE;
	}

	EnterCriticalSection( &CriticalSectionSqlData );

	va_list ap;
	static char szSQLData[ 4096 ];

	ZeroMemory( szSQLData, 4096 );
	va_start( ap, Command );
	_vsnprintf_s( szSQLData, 4096, 4095, Command, ap );
	va_end( ap );

	int iPos = findvtSQLData( DB );



	static SQLCHAR sqlcCommand[ 4096 ] = { 0 };
	ZeroMemory( sqlcCommand, 4096 );
	memcpy_s( sqlcCommand, 4096, szSQLData, lstrlenA( szSQLData ) );
	if ( SQL_SUCCESS != SQLExecDirectA( vtSQLData[ iPos ].sqlstatementhandle, sqlcCommand, SQL_NTS ) )
	{
		if ( show_errorSQL( SQL_HANDLE_STMT, vtSQLData[ iPos ].sqlstatementhandle ) )
			WRITEDBG( "QUERY: %s", szSQLData );
	}

	SQLFetch( vtSQLData[ iPos ].sqlstatementhandle );

	this->iRowPosition = 0;
	this->iPosDataSQL = iPos;

	LeaveCriticalSection( &CriticalSectionSqlData );
};


