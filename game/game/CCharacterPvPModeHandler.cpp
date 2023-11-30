#include "stdafx.h"
#include "CCharacterPvPModeHandler.h"

CCharacterPvPModeHandler::CCharacterPvPModeHandler()
{
}

CCharacterPvPModeHandler::~CCharacterPvPModeHandler()
{
}

void CCharacterPvPModeHandler::Init()
{
	pcImage = UI::ImageLoader::LoadImage( "game\\images\\hud\\pkmodetop.png" );
}

BOOL CCharacterPvPModeHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( toupper( pcKeyboard->GetChar() ) == 'K' )
		{
			if ( UNIT->bPvPMode )
				Kill();
			else
			{
				if ( CHARACTERPVPMODEHANDLER->IsPKMap( MAP_ID ) )
					AddTimer();
				else
				{
					TITLEBOX( AY_OBFUSCATE( "You can't turn into Pk Mode on this map!" ) );
					CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, AY_OBFUSCATE( "> You can't turn into Pk Mode on this map!" ) );
				}
			}
		}
	}

	return FALSE;
}

void CCharacterPvPModeHandler::Kill( bool bForce )
{
	if ( UNIT->bPvPMode && ((TICKCOUNT > (dwTimeUpdate + MAX_PKMODE_DISABLE_TIME)) || bForce) )
	{
		UNIT->bPvPMode = false;

		TIMERSKILLHANDLER->KillTimer( SKILLID_PKMode );

		TITLEBOX( AY_OBFUSCATE( "You turned into Neutral Mode!" ) );

		CHATGAMEHANDLER->AddChatBoxText( AY_OBFUSCATE( "> You turned into Neutral Mode!" ) );

		dwTimeUpdate = TICKCOUNT;
	}
	else
	{
		int iTime = CHARACTERPVPMODEHANDLER->GetTimeLeft();

		TITLEBOX( AY_OBFUSCATE( "You will be able to turn into Neutral Mode in %02d:%02d!" ), iTime / 60, iTime % 60 );
		CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, AY_OBFUSCATE( "> You will be able to turn into Neutral Mode in %02d:%02d!" ), iTime / 60, iTime % 60 );
	}
}

int CCharacterPvPModeHandler::OnRenderTime( Skill * psSkill )
{
	int iLineCount = 0;

	if ( psSkill->iID == SKILLID_PKMode )
	{
		SKILLMANAGERHANDLER->AddSkillInfo( 1, "Time left to disable:\r" );

		int iTime = (dwTimeUpdate - TICKCOUNT) / 1000;

		if ( iTime > 0 )
			SKILLMANAGERHANDLER->AddSkillInfo( 2, "%d:%d\r", iTime / 60, iTime % 60 );
		else
			SKILLMANAGERHANDLER->AddSkillInfo( 2, "You can disable now!\r" );

		iLineCount++;
	}

	return iLineCount;
}

int CCharacterPvPModeHandler::GetTimeLeft( bool bEnable )
{
	if ( (dwTimeUpdate + (bEnable ? MAX_PKMODE_ENABLE_TIME : MAX_PKMODE_DISABLE_TIME)) < TICKCOUNT )
		return 0;

	return (int)(((dwTimeUpdate + (bEnable ? MAX_PKMODE_ENABLE_TIME : MAX_PKMODE_DISABLE_TIME)) - TICKCOUNT) / 1000);
}

BOOL CCharacterPvPModeHandler::IsPKMap( int iMapID )
{
	BOOL bResult = TRUE;
	switch ( iMapID )
	{
		case MAPID_BlessCastle:
		case MAPID_QuestArena:
		case MAPID_GhostCastle:
		case MAPID_OldRuinen2:
		case MAPID_RoyalDesert:
		case MAPID_ForestDungeon:
		case MAPID_RicartenTown:
		case MAPID_PhillaiTown:
		case MAPID_BattleTown:
		case MAPID_Swamp:
		case MAPID_DistortedForest:
		case MAPID_T5QuestArena:
		case MAPID_Atlantis:
		case MAPID_Bellatra:
		case MAPID_DragonsDungeon:
			bResult = FALSE;
			break;
		default:
			break;
	}
	return bResult;
}

void CCharacterPvPModeHandler::OnUpdateMap( int iMapID )
{
	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( UNIT->bPvPMode )
		{
			if ( CHARACTERPVPMODEHANDLER->IsPKMap( iMapID ) == FALSE )
				Kill( true );
		}
	}
}

void CCharacterPvPModeHandler::RenderHeaderTopPK( Unit * pcUnit, int iLastHeight )
{
	if ( ISSHOW_INTERFACE == FALSE )
		return;

	if ( pcUnit )
	{
		if ( pcUnit->bPvPMode )
		{
			int iY = pcUnit->pcUnitData->rRenderRectangle.top - pcImage->GetHeight() - iLastHeight;

			UI::ImageRender::Render( pcImage, pcUnit->pcUnitData->sRenderPoint.iX - 10, iY, pcImage->GetWidth(), pcImage->GetHeight(), -1 );
		}
	}
}

void CCharacterPvPModeHandler::AddTimer()
{
    if ( (UNIT->bPvPMode == false) && (TICKCOUNT > (dwTimeUpdate + MAX_PKMODE_ENABLE_TIME)) )
    {
		TIMERSKILLHANDLER->KillTimer( SKILLID_PKMode );

		if ( !iTimerID )
			iTimerID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\PKMode.tga" );

		if ( !TIMERSKILLHANDLER->FindSkillTimer( SKILLID_PKMode ) )
		{
			Skill sSkill;
			ZeroMemory( &sSkill, sizeof( sSkill ) );
			sSkill.iID = SKILLID_PKMode;
			sSkill.iUseTime = 70 * 70;
			sSkill.iLevel = 1;
			sSkill.MatIcon = iTimerID;
			STRINGCOPY( sSkill.sSkillInfo.szName, "Pk Mode" );
			TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
		}

        dwTimeUpdate = TICKCOUNT;

		UNIT->bPvPMode = true;

		TITLEBOX( AY_OBFUSCATE( "You turned into Pk Mode!" ) );
		CHATGAMEHANDLER->AddChatBoxText( AY_OBFUSCATE( "> You turned into Pk Mode!" ) );
    }
	else if ( UNIT->bPvPMode == false )
	{
		int iTime = CHARACTERPVPMODEHANDLER->GetTimeLeft( true );

		TITLEBOX( AY_OBFUSCATE( "You will be able to turn into Pk Mode in %02d:%02d!" ), iTime / 60, iTime % 60 );
		CHATGAMEHANDLER->AddChatBoxTextEx( CHATCOLOR_Error, AY_OBFUSCATE( "> You will be able to turn into Pk Mode in %02d:%02d!" ), iTime / 60, iTime % 60 );
	}
}
