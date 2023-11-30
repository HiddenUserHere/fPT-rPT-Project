#pragma once

#include <Windows.h>
#include <memory>
#include <objbase.h>
#include <vector>
#include <comdef.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include "..\..\shared\Functions.h"
#include "..\..\shared\Reader.h"



class SQLAPI
{
private:
	UINT									_sizesqlarray;
	int										iPosDataSQL;
	int										iRowPosition;
	int										findvtSQLData( const char *DB );

public:
	void									Init( const char * DB );
	int										GetData( char * pBuf, int iColumn );
	int										GetData( int * piBuf, int iColumn );
	int										GetData( unsigned int * piBuf, int iColumn );
	int										GetData( unsigned long * piBuf, int iColumn );
	int										GetData( float * pfBuf, int iColumn );
	int										GetData( short * psBuf, int iColumn );
	int										GetData( unsigned short * psBuf, int iColumn );
	int										GetData( INT64 * pIBuf, int iColumn );
	int										GetRowPosition() { return iRowPosition; };
	int										NextRow();
	void									Close();
	BOOL									CheckSqlInject( const char *value, int SizeString );
	void									Select( const char * DB, const char * Command, ... );
	void									DisconnectAll();
};

typedef std::shared_ptr<SQLAPI> SQLAPI_ptr;
