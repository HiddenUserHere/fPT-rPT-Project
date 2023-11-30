#include "stdafx.h"
#include "CTimePointsHandler.h"

CTimePointsHandler::CTimePointsHandler()
{
}

CTimePointsHandler::~CTimePointsHandler()
{
}

void CTimePointsHandler::OnLoadUser( User * pcUser )
{
    pcUser->uInGameSecondsTime = 0;
}

void CTimePointsHandler::OnUnloadUser( User * pcUser )
{
    int iMinutes    = pcUser->uInGameSecondsTime / 60;
    int iPoints     = iMinutes * TIMECOINS_POINTS_MINUTES;

    if ( SQLHasUser( pcUser ) )
    {
        SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
        if ( pcDB->Open() )
        {
            if ( pcDB->Prepare( "UPDATE UserTimeCoin SET Coins=Coins+?, [Date]=GETDATE() WHERE AccountID=?" ) )
            {
                pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iPoints );
                pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iAccountID );
                pcDB->Execute();
            }
            pcDB->Close();
        }

        pcDB = SQLCONNECTION( DATABASEID_ServerDB );
        if ( pcDB->Open() )
        {
            if ( pcDB->Prepare( "UPDATE UserTimeCoinRanking SET Points=Points+?, TotalMinutes=TotalMinutes+?, [Date]=GETDATE() WHERE CharacterID=?" ) )
            {
                pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iPoints );
                pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iMinutes );
                pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
                pcDB->Execute();
            }
            pcDB->Close();
        }
    }
    else
    {
        SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
        if ( pcDB->Open() )
        {
            if ( pcDB->Prepare( "INSERT INTO UserTimeCoin([AccountID],[AccountName],[Coins],[Date]) VALUES(?,?,?,GETDATE())" ) )
            {
                pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iAccountID );
                pcDB->BindParameterInput( 2, PARAMTYPE_String, pcUser->pcUserData->szAccountName, STRLEN( pcUser->pcUserData->szAccountName ) );
                pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &iPoints );
                pcDB->Execute();
            }
            pcDB->Close();
        }

        pcDB = SQLCONNECTION( DATABASEID_ServerDB );
        if ( pcDB->Open() )
        {
            if ( pcDB->Prepare( "INSERT INTO UserTimeCoinRanking([CharacterID],[Points],[TotalMinutes],[Date]) VALUES(?,?,?,GETDATE())" ) )
            {
                pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iPoints );
                pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iMinutes );
                pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iAccountID );
                pcDB->Execute();
            }
            pcDB->Close();
        }
    }
}

void CTimePointsHandler::Update( ETickType eTick )
{
    if ( LOGIN_SERVER )
        return;

    if ( eTick == TICKTYPE_1Second )
    {
        for ( UINT i = 0; i < USERSERVER->uUsersInGame; i++ )
        {
            User * pcUser = USERSERVER->pcaUserInGame[i];
            pcUser->uInGameSecondsTime++;

            if ( (pcUser->uInGameSecondsTime % 60) == 0 )
            {
                OnUnloadUser( pcUser );
                pcUser->uInGameSecondsTime = 0;
            }
        }
    }
}

bool CTimePointsHandler::SQLHasUser( User * pcUser )
{
    int iRet = -1;

    SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
    if ( pcDB->Open() )
    {
        if ( pcDB->Prepare( "SELECT ID FROM UserTimeCoin WHERE AccountID=?" ) )
        {
            pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iAccountID );
            if ( pcDB->Execute() )
                if ( pcDB->Fetch() )
                    pcDB->GetData( 1, PARAMTYPE_Integer, &iRet );

        }
        pcDB->Close();
    }

    return iRet > 0;
}
