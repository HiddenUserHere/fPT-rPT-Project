#include "stdafx.h"
#include "GuideGame.h"


BOOL CGuideGame::guide_handle( int iLevel, ECharacterClass iClass )
{
	CGuideGame * p = this;
	switch ( iLevel )
	{
		case 10:
			p->open_guide( "Level_10", "Message", "Icon" );
			break;
		case 20:
			p->open_guide( "Level_20", "Message", "Icon" );
			break;
		case 30:
			p->open_guide( "Level_30", "Message", "Icon" );
			break;
		case 40:
			p->open_guide( "Level_40", "Message", "Icon" );
			break;
		case 41:
			p->open_guide( "Level_41", "Message", "Icon" );
			break;
		case 50:
			p->open_guide( "Level_50", "Message", "Icon" );
			break;
		case 55:
			p->open_guide( "Level_55", "Message", "Icon" );
			break;
		case 61:
			p->open_guide( "Level_61", "Message", "Icon" );
			break;
		case 60:
			p->open_guide( "Level_60", "Message", "Icon" );
			break;
		case 65:
			p->open_guide( "Level_65", "Message", "Icon" );
			break;
		case 70:
			p->open_guide( "Level_70", "Message", "Icon" );
			break;
		case 71:
			p->open_guide( "Level_71", "Message", "Icon" );
			break;
		case 75:
			p->open_guide( "Level_75", "Message", "Icon" );
			break;
		case 80:
			p->open_guide( "Level_80", "Message", "Icon" );
			break;
		case 81:
			p->open_guide( "Level_81", "Message", "Icon" );
			break;
		case 85:
			p->open_guide( "Level_85", "Message", "Icon" );
			break;
		case 90:
			p->open_guide( "Level_90", "Message", "Icon" );
			break;
		case 100:
			p->open_guide( "Level_100", "Message", "Icon" );
			break;
		case 101:
			p->open_guide( "Level_101", "Message", "Icon" );
			break;
		case 105:
			p->open_guide( "Level_105", "Message", "Icon" );
			break;
		case 110:
			p->open_guide( "Level_110", "Message", "Icon" );
			break;
		case 111:
			p->open_guide( "Level_111", "Message", "Icon" );
			break;
		case 115:
			p->open_guide( "Level_115", "Message", "Icon" );
			break;
		case 120:
			p->open_guide( "Level_120", "Message", "Icon" );
			break;
		case 125:
			p->open_guide( "Level_125", "Message", "Icon" );
			break;
		case 126:
			p->open_guide( "Level_126", "Message", "Icon" );
			break;
		case 130:
			p->open_guide( "Level_130", "Message", "Icon" );
			break;
	}
	return TRUE;
}

void CGuideGame::open_guide( char * head, char * key, char * guide )
{
	INI::CReader cReader( "game\\guides\\list.ini" );

	std::string str = cReader.ReadString( head, key );

	BOOL bNextLine = FALSE;

	for ( size_t i = 1; i < str.size(); i++ )
	{
		if ( (i % 50) == 0 )
			bNextLine = TRUE;

		if ( (str[i] == ' ') && bNextLine )
		{
			str.insert( i, "\r\n" );
			bNextLine = FALSE;
		}
	}

	ZeroMemory( this->szBuffer, _countof( this->szBuffer ) );

	CALLT_WITH_ARG1( 0x00640420, (DWORD)this, (DWORD)str.c_str() );
}
