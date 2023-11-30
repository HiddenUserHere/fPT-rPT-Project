#pragma once
class CRankingServerHandler
{
public:
    CRankingServerHandler();
    virtual ~CRankingServerHandler();

    void                        Init();

    void                        Update( ETickType eTick );

    void                        HandlePacket( User * pcUser, PacketRankingLevelRequest * psPacket );

    void                        ProcessPacket( User * pcUser, PacketRankingLevel * psPacket );

    void                        SendUserRankingLevel( User * pcUser );

private:
    std::vector<Ranking::RankingLevelData>    vRankingLevelData;

    void                        SQLLoadRankingLevel( std::vector<Ranking::RankingLevelData> & vRanking );
};

