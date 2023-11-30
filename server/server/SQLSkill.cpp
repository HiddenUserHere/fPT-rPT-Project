#include "stdafx.h"
#include "SQLSkill.h"

typedef int( __cdecl *t_rangeNumberPT ) ( int min, int max );
t_rangeNumberPT rangeNumberPT = ( t_rangeNumberPT )0x00439B50;

CSQLSkill::CSQLSkill()
{
}

int CSQLSkill::GetSkillValueInt( const char * SQLCommand, ... )
{
	va_list ap;
	char sqlC[ 1024 ];

	ZeroMemory( sqlC, 1024 );
	va_start( ap, SQLCommand );
	_vsnprintf_s( sqlC, 1024, 1023, SQLCommand, ap );
	va_end( ap );

	int iValue = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( sqlC ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iValue, 0 );
		}
		pcDB->Close();
	}
	return iValue;
}

float CSQLSkill::GetSkillValueFloat( const char * SQLCommand, ... )
{
	va_list ap;
	char sqlC[ 8092 ];

	ZeroMemory( sqlC, 1024 );
	va_start( ap, SQLCommand );
	_vsnprintf_s( sqlC, 1024, 1023, SQLCommand, ap );
	va_end( ap );

	float fValue = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( sqlC ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Float, &fValue, 0 );
		}
		pcDB->Close();
	}
	return fValue;
}


int CSQLSkill::GetMinMaxInt( int iMin, int iMax, int iPercent )
{
	if ( iPercent > 0 )
		return ( rangeNumberPT( iMin, iMax ) * iPercent ) / 100;
	return rangeNumberPT( iMin, iMax );
}


CSQLSkill::~CSQLSkill()
{
}
