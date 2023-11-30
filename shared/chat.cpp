#include "stdafx.h"
#include "chat.h"

unsigned int Chat::ChatCommand( char * pszCommand, char * pszChat )
{
	int iLen = lstrlenA( pszCommand );

	for( int i = 0; i < iLen; i++ )
	{		
		if( pszChat[i] != pszCommand[i] )
		{
			if( (pszChat[i] >= 0x41) && (pszChat[i] <= 0x5A) )
			{
				if( (pszChat[i] + 0x20) == pszCommand[i] )
					continue;
			}

			if( (pszCommand[i] >= 0x41) && (pszCommand[i] <= 0x5A) )
			{
				if( (pszCommand[i] + 0x20) == pszChat[i])
					continue;
			}

			return 0;
		}
	}

	unsigned char cLastCharacter = pszChat[iLen];
	if( (cLastCharacter != 0x00) && (cLastCharacter != 0x09) && (cLastCharacter != 0x20) )
		return 0;

	return iLen;
}

void Chat::ChatParamQ( char * pszCommand, int iParamNum, char * pszOut )
{
	int iLen = lstrlenA( pszCommand ), iMode = 0, iParam = 0, j = 0;
	char cChar;

	for( int i = 0; i <= iLen; i++ )
	{
		cChar = pszCommand[i];

		if( cChar == 0 )
		{
			if( iParam != iParamNum )
				j = 0;

			break;
		}
		else if( cChar == 0x20 )
		{
			if( iMode == 0 )
				iMode = 1;
			else if( iMode == 2 )
			{
				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 3 )
			{
				iMode = 1;

				if( iParam == iParamNum )
					break;
				else
					j = 0;
			}
		}
		else if( cChar == 0x22 )
		{
			if( iMode == 1 )
			{
				iParam++;
				iMode = 2;

				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 2 )
			{
				iMode = 0;

				if( iParam == iParamNum )
				{
					pszOut[j] = cChar;
					j++;

					break;
				}
				else
					j = 0;
			}
		}
		else
		{
			if( iMode == 1 )
			{
				iParam++;
				iMode = 3;

				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 2 )
			{
				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 3 )
			{
				pszOut[j] = cChar;
				j++;
			}
		}
	}

	pszOut[j] = 0;
}

void Chat::ChatParam( char * pszCommand, int iParamNum, char * pszOut )
{
	int iLen = lstrlenA( pszCommand ), iMode = 0, iParam = 0, j = 0;
	char cChar;

	for( int i = 0; i <= iLen; i++ )
	{
		cChar = pszCommand[i];

		if( cChar == 0 )
		{
			if( iParam != iParamNum )
				j = 0;

			break;
		}
		else if( cChar == 0x20 )
		{
			if( iMode == 0 )
				iMode = 1;
			else if( iMode == 2 )
			{
				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 3 )
			{
				iMode = 1;

				if( iParam == iParamNum )
					break;
				else
					j = 0;
			}
		}
		else if( cChar == 0x22 )
		{
			if( iMode == 1 )
			{
				iParam++;
				iMode = 2;
			}
			else if( iMode == 2 )
			{
				iMode = 0;

				if( iParam == iParamNum )
					break;
				else
					j = 0;
			}
		}
		else
		{
			if( iMode == 1 )
			{
				iParam++;
				iMode = 3;

				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 2 )
			{
				pszOut[j] = cChar;
				j++;
			}
			else if( iMode == 3 )
			{
				pszOut[j] = cChar;
				j++;
			}
		}
	}

	pszOut[j] = 0;
}

void Chat::TrimSpace( char * pszIn, char * pszOut )
{
	int iLen = lstrlenA( pszIn );
	pszOut[0] = 0;

	for( int i = 0; i < iLen; i++ )
	{
		if( (pszIn[i] == 0) || (pszIn[i] == ' ') )
			break;

		pszOut[i] = pszIn[i];
	}

	pszOut[lstrlenA( pszOut )] = 0;
}

int Chat::TrimSpaceBegin( char * pszIn, char * pszOut )
{
	int iLen = lstrlenA( pszIn );
	pszOut[0] = 0;

	bool bStartedEdging = false;
	int j = 0, r = -1;

	for( int i = 0; i < iLen; i++ )
	{
		if( pszIn[i] == 0 )
			break;
		else if( (pszIn[i] == ' ') && (!bStartedEdging) )
			continue;
		else if( (!bStartedEdging) )
			r = i;

		bStartedEdging = true;
		pszOut[j] = pszIn[i];
		j++;
	}

	pszOut[j] = 0;

	return r;
}