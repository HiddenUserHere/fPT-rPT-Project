#include "stdafx.h"
#include "actionfield.h"

CActionField::CActionField( int iID, const char szName[64], EActionFieldModeID iModeID )
{
	this->iID = iID;
	this->iModeID = iModeID;
	STRINGCOPY( this->szName, szName );
}

CActionField::~CActionField()
{
	Clear();
}

std::vector<ActionFieldReward*> CActionField::GetRewards( int iChestID )
{
	std::vector<ActionFieldReward*> v;
	for ( std::vector<ActionFieldReward*>::iterator it = vRewards.begin(); it != vRewards.end(); it++ )
	{
		ActionFieldReward * pc = (*it);
		if ( pc->iChestID == iChestID )
			v.push_back( pc );
	}

	return v;
}

ActionFieldRound * CActionField::GetRound( int iRoundLevel )
{
	ActionFieldRound * psRound = NULL;
	for ( std::vector<ActionFieldRound*>::iterator it = vRounds.begin(); it != vRounds.end(); it++ )
	{
		ActionFieldRound * ps = (*it);
		if ( ps->iLevel == iRoundLevel )
		{
			psRound = ps;
			break;
		}
	}

	return psRound;
}

void CActionField::Clear()
{
	for ( std::vector<ActionFieldChest*>::iterator it = vChests.begin(); it != vChests.end(); it++ )
	{
		ActionFieldChest * pc = (*it);
		DELET( pc );
	}
	vChests.clear();
	
	for ( std::vector<ActionFieldReward*>::iterator it = vRewards.begin(); it != vRewards.end(); it++ )
	{
		ActionFieldReward * pc = (*it);
		DELET( pc );
	}
	vRewards.clear();
	
	for ( std::vector<ActionFieldRound*>::iterator it = vRounds.begin(); it != vRounds.end(); it++ )
	{
		ActionFieldRound * pc = (*it);
		DELET( pc );
	}
	vRounds.clear();
}

void CActionField::SetSettings( ActionFieldSettings & sSettings )
{
	CopyMemory( &this->sSettings, &sSettings, sizeof( ActionFieldSettings ) );
}

void CActionField::AddChest( ActionFieldChest & sChest )
{
	ActionFieldChest * psChest = new ActionFieldChest;
	CopyMemory( psChest, &sChest, sizeof( ActionFieldChest ) );
	vChests.push_back( psChest );
}

void CActionField::AddReward( ActionFieldReward & sReward )
{
	ActionFieldReward * psReward = new ActionFieldReward;
	CopyMemory( psReward, &sReward, sizeof( ActionFieldReward ) );
	vRewards.push_back( psReward );
}

void CActionField::AddRound( ActionFieldRound & sRound )
{
	ActionFieldRound * psRound = new ActionFieldRound;
	
	psRound->iID = sRound.iID;
	psRound->iLevel = sRound.iLevel;
	
	psRound->iBeginTime = sRound.iBeginTime;
	psRound->iEndTime = sRound.iEndTime;
	
	psRound->iBossSpawnKills = sRound.iBossSpawnKills;
	psRound->iKillsNeeded = sRound.iKillsNeeded;
	
	psRound->iMonsterCount = sRound.iMonsterCount;
	
	CopyMemory( psRound->saMonsters, sRound.saMonsters, sizeof( ActionFieldMonster ) * MAX_ACTIONFIELD_MONSTERS );
	
	for ( auto sPosition : sRound.vSpawnMonsterPosition )
		psRound->vSpawnMonsterPosition.push_back( sPosition );

	for ( auto sPosition : sRound.vSpawnBossPosition )
		psRound->vSpawnBossPosition.push_back( sPosition );

	vRounds.push_back( psRound );
}

ActionFieldChest * CActionField::GetChestItem( const char * pszCode )
{
	for ( std::vector<ActionFieldChest*>::iterator it = vChests.begin(); it != vChests.end(); it++ )
	{
		ActionFieldChest * pc = (*it);
		if ( STRINGCOMPAREI( pc->szItemCode, pszCode ) )
			return pc;
	}

	return nullptr;
}
