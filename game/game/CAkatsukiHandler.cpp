#include "stdafx.h"
#include "CAkatsukiHandler.h"

CAkatsukiHandler::CAkatsukiHandler()
{
}

CAkatsukiHandler::~CAkatsukiHandler()
{
}

void CAkatsukiHandler::Init()
{
}

void CAkatsukiHandler::Update( float fTime )
{
   
}

void CAkatsukiHandler::VerifyUnit( UnitData * pcUnitData )
{
    if ( dwTimeUpdate < TICKCOUNT )
    {
        if ( !bHasUserCostume )
        {
            if ( std::string( pcUnitData->sCharacterData.Player.szBodyModel ).find( "AkatsukiCostume" ) != std::string::npos )
                bHasUserCostume = true;
        }
    }
}

void CAkatsukiHandler::UpdateVerify( bool bReset )
{
    if ( Game::GetGameMode() == GAMEMODE_InGame )
    {
        if ( UNITDATA->iLoadedMapIndex >= 0 )
        {
            if ( iBackgroundMusicID == BACKGROUNDMUSICID_Login )
                iBackgroundMusicID = FIELDGAME[UNITDATA->iLoadedMapIndex]->iBackgroundMusicID;

            if ( dwTimeUpdate < TICKCOUNT )
            {
                if ( bReset == false )
                {
                    if ( bHasUserCostume )
                    {
                        if ( iBackgroundMusicID != BACKGROUNDMUSICID_AkatsukiTheme )
                        {
                            SOUNDHANDLER->ChangeMusic( BACKGROUNDMUSICID_AkatsukiTheme );

                            iBackgroundMusicID = BACKGROUNDMUSICID_AkatsukiTheme;
                        }
                    }
                    else
                    {
                        if ( iBackgroundMusicID == BACKGROUNDMUSICID_AkatsukiTheme )
                        {
                            iBackgroundMusicID = FIELDGAME[UNITDATA->iLoadedMapIndex]->iBackgroundMusicID;

                            SOUNDHANDLER->ChangeMusic( FIELDGAME[UNITDATA->iLoadedMapIndex]->iBackgroundMusicID );
                        }
                    }

                    dwTimeUpdate = TICKCOUNT + 4000;
                }
                else
                    bHasUserCostume = false;
            }
        }
    }
}
