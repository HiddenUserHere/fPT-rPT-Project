#include "stdafx.h"
#include "CRankingServerHandler.h"

CRankingServerHandler::CRankingServerHandler()
{
}

CRankingServerHandler::~CRankingServerHandler()
{
    vRankingLevelData.clear();
}

void CRankingServerHandler::Init()
{
    SQLLoadRankingLevel( vRankingLevelData );
}

void CRankingServerHandler::Update( ETickType eTick )
{
    if ( LOGIN_SERVER )
        return;

    if ( eTick == TICKTYPE_1Minute )
        SQLLoadRankingLevel( vRankingLevelData );
}

void CRankingServerHandler::HandlePacket( User * pcUser, PacketRankingLevelRequest * psPacket )
{
    SendUserRankingLevel( pcUser );
}

void CRankingServerHandler::ProcessPacket( User * pcUser, PacketRankingLevel * psPacket )
{
    psPacket->iLength = sizeof( PacketRankingLevel );
    psPacket->iHeader = PKTHDR_RankingLevel;
    SENDPACKET( pcUser, psPacket );
}

void CRankingServerHandler::SendUserRankingLevel( User * pcUser )
{
    std::vector< PacketRankingLevel> vPackets;

    PacketRankingLevel sPacket;
    sPacket.iCount = 0;
    sPacket.sPacketCount.sMin = 1;

    for ( auto & sRanking : vRankingLevelData )
    {
        CopyMemory( sPacket.saRankingData + sPacket.iCount++, &sRanking, sizeof( Ranking::RankingLevelData ) );

        if ( sPacket.iCount == MAX_RANKINGDATA )
        {
            vPackets.push_back( sPacket );
            sPacket.sPacketCount.sMin++;

            sPacket.iCount = 0;
        }
    }

    if ( sPacket.iCount > 0 )
        vPackets.push_back( sPacket );

    for ( auto & sPacketRankingSend : vPackets )
    {
        sPacketRankingSend.sPacketCount.sMax = (short)vPackets.size();

        ProcessPacket( pcUser, &sPacketRankingSend );
    }

    vPackets.clear();
}

void CRankingServerHandler::SQLLoadRankingLevel( std::vector<Ranking::RankingLevelData> & vRanking )
{
    vRanking.clear();

    SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
    if ( pcDB->Open() )
    {
        if ( pcDB->Prepare( QUERY_RANKING_LEVEL ) )
        {
            if ( pcDB->Execute() )
            {
                while ( pcDB->Fetch() )
                {
                    Ranking::RankingLevelData sRankingData = {0};

                    pcDB->GetData( 1, PARAMTYPE_Integer, &sRankingData.iNumber );
                    pcDB->GetData( 2, PARAMTYPE_Integer, &sRankingData.iNumberClass );
                    pcDB->GetData( 3, PARAMTYPE_String, sRankingData.szCharacterName, _countof( sRankingData.szCharacterName ) );
                    pcDB->GetData( 4, PARAMTYPE_Integer, &sRankingData.eCharacterClass );
                    pcDB->GetData( 5, PARAMTYPE_Integer, &sRankingData.iClanID );
                    pcDB->GetData( 6, PARAMTYPE_String, sRankingData.szClanName, _countof( sRankingData.szClanName ) );
                    pcDB->GetData( 7, PARAMTYPE_Integer, &sRankingData.iCharacterLevel );
                    pcDB->GetData( 8, PARAMTYPE_Float, &sRankingData.fCharacterPercentEXP );

                    vRanking.push_back( sRankingData );
                }
            }
        }
        pcDB->Close();
    }
}
