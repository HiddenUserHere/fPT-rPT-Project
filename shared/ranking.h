#pragma once

#define MAX_RANKINGDATA     75

namespace Ranking
{

struct RankingLevelData
{
    int                 iNumber;

    int                 iNumberClass;

    char                szCharacterName[32];

    ECharacterClass     eCharacterClass;

    int                 iClanID;
    char                szClanName[32];

    int                 iCharacterLevel;
    float               fCharacterPercentEXP;
};

};

struct PacketRankingLevelRequest : Packet
{
    ECharacterClass             eCharacterClass;
};


struct PacketRankingLevel : Packet
{
    MinMax				sPacketCount;

    int                 iCount;
    Ranking::RankingLevelData    saRankingData[MAX_RANKINGDATA];

    bool				IsFirst() { return sPacketCount.sMin == 1 ? true : false; }
    bool				IsOnly() { return sPacketCount.sMin == sPacketCount.sMax ? true : false; }
};

static const char * QUERY_RANKING_LEVEL = R"PROG(SELECT * FROM (
    SELECT 
        RANK() OVER (ORDER BY ci.Experience DESC, 
                              ci.DateLevelUP ASC) AS RankAll,
        RANK() OVER (PARTITION BY ci.JobCode 
                     ORDER BY ci.Experience DESC, 
                              ci.DateLevelUP ASC) AS RankClass,
        ci.Name AS CharacterName,
        ci.JobCode AS ClassID,
        cl.IconID AS ClanIconID,
        cl.ClanName,
        ci.Level AS CharacterLevel,
        CAST(ROUND(((ci.Experience - et.ExpTotal) / et.ExpRequired) * 100, 4, 1) AS real) AS CharacterExp
    FROM
        UserDB.dbo.CharacterInfo ci
    LEFT JOIN
        WebDB.dbo.CharacterClass cc ON ci.JobCode = cc.ID
    LEFT JOIN
        WebDB.dbo.ExperienceTable et ON ci.Level = et.ID
    LEFT JOIN
        UserDB.dbo.UserInfo ui ON ci.AccountName = ui.AccountName
    LEFT JOIN
        ClanDB.dbo.ClanList cl ON ci.ClanID = cl.ID
    WHERE    
        ci.JobCode > 0 AND        -- exclude bugged chars
        ci.Experience > 0 AND     -- exclude inactive chars
        ui.Flag = 98 AND          -- exclude deactivated accounts
        ui.BanStatus = 0 AND      -- exclude permanent banned
        ui.UnbanDate IS NULL AND  -- exclude temporarily banned
        ui.GameMasterType = 0     -- exclude accounts with gm active
) Ranking WHERE Ranking.RankClass <= 50
          ORDER BY Ranking.RankAll ASC;)PROG";
