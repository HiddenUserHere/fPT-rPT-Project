#include "stdafx.h"
#include "achievement.h"


void CAchievement::SetAchievement( int iID, int iType, const std::string & strName, const std::string & strImage, const std::string & strDescription )
{
	this->iID	= iID;
	this->iType = iType;
	STRINGCOPY( szName, strName.c_str() );
	STRINGCOPY( szImagePath, strImage.c_str() );
	STRINGCOPY( szDescription, strDescription.c_str() );
}

void CAchievement::Shutdown()
{
	for ( std::vector<CAchievementEvent*>::iterator it = vAchievementEvent.begin(); it != vAchievementEvent.end(); it++ )
	{
		CAchievementEvent * pc = (*it);
		DELET( pc );
	}

	vAchievementEvent.clear();
}

void CAchievementEvent::SetValue( int iIndex, const char * pszValue )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		STRINGCOPY( szaValue[iIndex], pszValue );
}

void CAchievementEvent::SetValue( int iIndex, int iValue )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		STRINGFORMAT( szaValue[iIndex], "%d", iValue );
}

void CAchievementEvent::SetValue( int iIndex, float fValue )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		STRINGFORMAT( szaValue[iIndex], "%f", fValue );
}

char * CAchievementEvent::GetValueString( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		return szaValue[iIndex];

	return "";
}

int CAchievementEvent::GetValueInt( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		return atoi( szaValue[iIndex] );

	return 0;
}

float CAchievementEvent::GetValueFloat( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		return (float)atof( szaValue[iIndex] );

	return 0.0f;
}

void CAchievementUser::SetValue( int iIndex, const char * pszValue )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		STRINGCOPY( szaValue[iIndex], pszValue );
}

void CAchievementUser::SetValue( int iIndex, int iValue )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		STRINGFORMAT( szaValue[iIndex], "%d", iValue );
}

void CAchievementUser::SetValue( int iIndex, float fValue )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		STRINGFORMAT( szaValue[iIndex], "%f", fValue );
}

void CAchievementUser::SetDate( SYSTEMTIME & s )
{
	CopyMemory( &sDate, &s, sizeof( SYSTEMTIME ) );
}

char * CAchievementUser::GetValueString( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		return szaValue[iIndex];

	return "";
}

int CAchievementUser::GetValueInt( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		return atoi( szaValue[iIndex] );

	return 0;
}

float CAchievementUser::GetValueFloat( int iIndex )
{
	if ( iIndex >= 0 && iIndex < MAX_ACHIEVEMENT_EVENT_VALUES )
		return (float)atof( szaValue[iIndex] );

	return 0.0f;
}
