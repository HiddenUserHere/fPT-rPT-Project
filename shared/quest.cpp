#include "stdafx.h"
#include "Quest.h"
#include "strings.h"
#include "def.h"

#define QUEST_OBFUSCATOR_KEY	0x6A8A

void QuestData::HandleFinish()
{
	CALL_WITH_ARG1( pfnFinished, (DWORD)this );
}

UINT QuestData::GetQuestValidate()
{
	UINT iCode = QUEST_OBFUSCATOR_KEY ^ uValidateWheel;

	for ( int i = 0; i < _countof( iaMonsterCounter ); i++ )
	{
		if ( iaMonsterCounter[i] == 0 )
			continue;

		iCode ^= iaMonsterID[i];
		iCode ^= iaMonsterCounter[i];
		iCode ^= iaMonsterKilled[i];
	}

	for ( int i = 0; i < _countof( iaItemCounter ); i++ )
	{
		if ( iaItemCounter[i] == 0 )
			continue;

		iCode ^= iaItemID[i];
		iCode ^= iaItemCounter[i];
		iCode ^= iaItemsCollected[i];
	}

	return iCode;
}

QuestData::QuestData( PacketQuestStartData * psPacket )
{
	StringCbCopy( szName, _countof( szName ), psPacket->szName );
	StringCbCopy( szDescription, _countof( szDescription ), psPacket->szShortDescription );
	
	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		StringCbCopy( szaMonsterName[i], _countof( szaMonsterName[i] ), psPacket->szaMonsterName[i] );
	
	iType = psPacket->iType;
	iAreaType = psPacket->iAreaType;
	
	for ( int i = 0; i < psPacket->iMapCount; i++ )
		AddMapID( psPacket->iaMapID[i] );

	bTime = psPacket->bTime;
	dwTimeLeft = psPacket->dwTimeLeft;
	dwWaitingTime = psPacket->dwWaitingTime;
	dwTimeTotal = psPacket->dwTimeTotal;
	iXMin = psPacket->iXMin;
	iXMax = psPacket->iXMax;
	iZMin = psPacket->iZMin;
	iZMax = psPacket->iZMax;

	iParty = psPacket->iParty;
	bPvP = psPacket->bPvP;
	bMultiple = psPacket->bMultiple;

	CopyMemory( iaMonsterID, psPacket->iaMonsterID, _countof( iaMonsterID ) * sizeof( int ) );
	CopyMemory( iaMonsterCounter, psPacket->iaMonsterCounter, _countof( iaMonsterCounter ) * sizeof( int ) );
	CopyMemory( iaMonsterKilled, psPacket->iaMonsterKilled, _countof( iaMonsterKilled ) * sizeof( int ) );

	BOOL bF = TRUE;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetMonsterCounter( i ) == 0 )
			break;

		if ( GetMonsterKilledAmount( i ) < GetMonsterCounter( i ) )
		{
			bF = FALSE;
			break;
		}
	}

	bKillsFinished = bF;

	CopyMemory( iaItemID, psPacket->iaItemID, _countof( iaItemID ) * sizeof( int ) );
	CopyMemory( iaItemCounter, psPacket->iaItemCounter, _countof( iaItemCounter ) * sizeof( int ) );
	CopyMemory( iaItemsCollected, psPacket->iaItemsCollected, _countof( iaItemsCollected ) * sizeof( int ) );

	bF = TRUE;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetItemCounter( i ) == 0 )
			break;

		if ( GetItemsCollected( i ) < GetItemCounter( i ) )
		{
			bF = FALSE;
			break;
		}
	}

	uValidateWheel = (rand() % 9000) + 1000;

	ValidateQuest();

	bItemsFinished = bF;

	iID = psPacket->iID;

	iLevel = psPacket->iLevel;

	bUpdate = FALSE;
}

QuestData::~QuestData()
{

}

void QuestData::SetName( char * pszQuestName )
{
	StringCbCopy( szName, _countof( szName ), pszQuestName );
}

void QuestData::SetDescription( char * pszQuestDescription )
{
	StringCbCopy( szDescription, _countof( szDescription ), pszQuestDescription );
}



BOOL QuestData::Update()
{
	if ( IsValidQuest() == false )
		return FALSE;

	if ( bUpdate )
		return TRUE;

	if ( IsFinished() )
		return TRUE;

	return FALSE;
}

BOOL QuestData::HandleKill( int iTargetID, int iMapID, int iX, int iZ )
{
	BOOL bResult = FALSE;

	if ( bKillsFinished == FALSE )
	{
		if ( !IsInArea( iMapID, iX, iZ ) )
			return FALSE;

		int iKillID = -1;
		if ( GetMapIDs().size() == 1 )
		{
			for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
			{
				if ( GetMonsterCounter( i ) && (GetMonsterID( i ) == iTargetID || GetMonsterID( i ) == 0) )
				{
					if ( GetMonsterKilledAmount( i ) < GetMonsterCounter( i ) )
					{
						iKillID = i;
						break;
					}
				}
			}
		}
		else
		{
			for ( size_t i = 0; i < GetMapIDs().size(); i++ )
			{
				if ( iMapID == GetMapIDs()[i] )
				{
					if ( GetMonsterCounter( i ) && (GetMonsterID( i ) == iTargetID || GetMonsterID( i ) == 0) )
					{
						if ( GetMonsterKilledAmount( i ) < GetMonsterCounter( i ) )
							iKillID = i;
					}
					break;
				}
			}
		}

		if ( iKillID >= 0 )
		{
			AddMonsterKill( iKillID );

			bUpdate = TRUE;

			bResult = TRUE;
		}

		BOOL bF = TRUE;

		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetMonsterCounter( i ) == 0 )
				break;

			if ( GetMonsterKilledAmount( i ) < GetMonsterCounter( i ) )
			{
				bF = FALSE;
				break;
			}
		}

		bKillsFinished = bF;

		if ( IsFinished() )
			HandleFinish();
	}

	return bResult;
}

BOOL QuestData::HandleCollectedItem( int iItemID, int iMapID, int iX, int iZ )
{
	BOOL bResult = FALSE;

	if ( bItemsFinished == FALSE )
	{
		if ( !IsInArea( iMapID, iX, iZ ) )
			return FALSE;

		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetItemID( i ) == iItemID )
			{
				if ( GetItemsCollected( i ) < GetItemCounter( i ) )
				{
					AddItemCollected( i );

					bUpdate = TRUE;

					bResult = TRUE;
				}
			}
		}

		BOOL bF = TRUE;

		for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
		{
			if ( GetItemCounter( i ) == 0 )
				break;

			if ( GetItemsCollected( i ) < GetItemCounter( i ) )
			{
				bF = FALSE;
				break;
			}
		}

		bItemsFinished = bF;

		if ( IsFinished() )
			HandleFinish();
	}
	return bResult;
}

void QuestData::Update( float fTime )
{
	if ( bTime && dwTimeLeft > 0 )
		dwTimeLeft -= (int)fTime;
}

BOOL Quest::HandleKill( int iTargetID, int iMapID, int iX, int iZ, BOOL bParty, BOOL bRaid, BOOL bPvP )
{
	BOOL bResult = FALSE;

	//Handle Kills Quests
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		//Not in party? don't count
		if ( ps->IsInParty() )
		{
			if ( ((ps->GetPartyType() == 1) || (ps->GetPartyType() == 2)) && bRaid )
				continue;

			if ( bParty == FALSE )
			{
				if ( (ps->GetPartyType() != 2) && (ps->GetPartyType() != 4) )
					continue;
			}
		}
		//Not in raid? don't count
		if ( ps->IsInRaid() )
		{
			if ( bRaid == FALSE )
			{
				if ( (ps->GetPartyType() != 3) && (ps->GetPartyType() != 4) )
					continue;
			}
		}

		if ( (ps->GetPartyType() == 0) && (bParty || bRaid) )
			continue;
			

		int iTarget = iTargetID;
		if ( ps->IsPVP() && bPvP == FALSE )
			continue;

		if ( ps->IsPVP() )
			iTarget = 0;

		//Handle Kills, if handled, return TRUE
		if( ps->HandleKill( iTarget, iMapID, iX, iZ ) )
			bResult = TRUE;
	}

	return bResult;
}

BOOL Quest::HandleCollectedItem( int iItemID, int iMapID, int iX, int iZ, BOOL bParty, BOOL bRaid )
{
	BOOL bResult = FALSE;

	//Handle Kills Quests
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		if ( ps->IsInParty() )
			if ( bParty == FALSE )
				continue;

		if ( ps->IsInRaid() )
			if ( bRaid == FALSE )
				continue;

		//Handle Kills, if handled, return TRUE
		if ( ps->HandleCollectedItem( iItemID, iMapID, iX, iZ ) )
			bResult = TRUE;
	}

	return bResult;
}

Quest::Quest()
{
}

Quest::~Quest()
{
	vData.clear();
}

QuestData * Quest::GetQuest( int iID )
{
	QuestData * pcQuestData = NULL;

	//Get Quests on same ID
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		//Same ID? Push It to vector
		if ( ps->GetID() == iID )
		{
			pcQuestData = ps;
			break;
		}
	}

	//Return all data of Quest ID
	return pcQuestData;
}

BOOL Quest::IsActiveQuest( int iID )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = (*it);
		if ( ps->GetID() == iID )
			return TRUE;
	}
	return FALSE;
}

BOOL Quest::IsFinishedQuestKills( int iID )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = (*it);
		if ( ps->GetID() == iID && ps->IsKillsFinished() )
			return TRUE;
	}
	return FALSE;
}

BOOL Quest::IsFinishedQuestItems( int iID )
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = (*it);
		if ( ps->GetID() == iID && ps->IsItemsFinished() )
			return TRUE;
	}
	return FALSE;
}

BOOL Quest::AddQuest( QuestData * psQuestData )
{
	//Add QuestKill to vector Data
	GetData().push_back( psQuestData );

	return TRUE;
}

BOOL Quest::AddFinishedQuest( PacketHandleFinishedQuest * psPacket )
{

	for ( std::vector<QuestFinishedData*>::iterator it = vFinishedData.begin(); it != vFinishedData.end(); )
	{
		QuestFinishedData * ps = (*it);

		if ( ps && ps->iID == psPacket->iID )
		{
			delete ps;
			it = vFinishedData.erase( it );
		}
		else
			it++;
	}

	QuestFinishedData * ps = new QuestFinishedData;

	ps->iID = psPacket->iID;
	ps->iType = psPacket->iType;
	
	CopyMemory( &ps->sStartDate, &psPacket->sStartDate, sizeof( SYSTEMTIME ) );
	CopyMemory( &ps->sEndDate, &psPacket->sEndDate, sizeof( SYSTEMTIME ) );
	
	CopyMemory( ps->iaExtraReward, psPacket->iaExtraReward, sizeof( int ) * QUEST_MONSTERITEM_MAX );
	CopyMemory( ps->iaExtraRewardValue, psPacket->iaExtraRewardValue, sizeof( int ) * QUEST_MONSTERITEM_MAX );

	vFinishedData.push_back( ps );

	return TRUE;
}

BOOL Quest::IsDoneQuest( int iQuestID )
{
	for ( std::vector<QuestFinishedData*>::iterator it = vFinishedData.begin(); it != vFinishedData.end(); it++ )
	{
		QuestFinishedData * ps = (*it);

		if ( ps && ps->iID == iQuestID )
			return TRUE;
	}

	return FALSE;
}


BOOL Quest::DeleteQuest( int iID )
{
	BOOL bResult = FALSE;

	//Delete All Quests in vector with same ID
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); )
	{
		QuestData * ps = *it;

		if ( ps->GetID() == iID )
		{
			bResult = TRUE;
			DELET( ps );

			it = GetData().erase( it );
		}
		else 
			it++;
	}

	return bResult;
}

void Quest::DeleteAllQuests()
{
	for ( std::vector<QuestData*>::iterator it = GetData().begin(); it != GetData().end(); it++ )
	{
		QuestData * ps = *it;

		delete ps;
	}

	GetData().clear();
}

int Quest::GetRewardValue( int iRewardType )
{
	int iValue = 0;

	for ( std::vector<QuestFinishedData*>::iterator it = vFinishedData.begin(); it != vFinishedData.end(); it++ )
	{
		QuestFinishedData * ps = (*it);

		for ( int j = 0; j < QUEST_MONSTERITEM_MAX; j++ )
		{
			if ( ps->iaExtraReward[j] != 0 && ps->iaExtraReward[j] == iRewardType )
				iValue += ps->iaExtraRewardValue[j];
		}
	}

	return iValue;
}

BOOL QuestData::CanFinishQuestWaitTime()
{
	if ( dwWaitingTime > 0 )
	{
		if ( IsTimeQuest() )
		{
			DWORD dwTime = dwTimeTotal - GetTimeQuest();

			if ( dwTime <= dwWaitingTime )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL QuestData::IsInArea( int iMapID, int iX, int iZ )
{
	bool bMapNotFound = true;
	for ( auto & eMapID : vMapIDs )
	{
		if ( (eMapID == iMapID) || (eMapID == MAPID_Invalid) )
		{
			bMapNotFound = false;
			break;
		}
	}

	if ( bMapNotFound )
		return FALSE;

	if ( GetAreaType() == QUESTAREATYPE_Bound )
	{
		int iXMap = iX >> 8;
		int iZMap = iZ >> 8;

		if ( iXMap < GetXMin() || iXMap >GetXMax() || iZMap < GetZMin() || iZMap > GetZMax() )
			return FALSE;
	}

	if ( GetAreaType() == QUESTAREATYPE_Radius )
	{
		int iXMap = (iX >> 8) - GetXMin();
		int iZMap = (iZ >> 8) - GetZMin();

		int iDistance = abs( (iXMap*iXMap) * (iZMap*iZMap) );

		if ( iDistance >= GetRadius() )
			return FALSE;
	}

	return TRUE;
}

int QuestData::GetMonsterID( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaMonsterID[i];
}

int QuestData::GetMonsterCounter( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaMonsterCounter[i];
}

int QuestData::GetMonsterCount()
{
	int iCount = 0;
	
	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetMonsterCounter( i ) != 0 )
			iCount++;
	}

	return iCount;
}

int QuestData::GetItemCount()
{
	int iCount = 0;

	for ( int i = 0; i < QUEST_MONSTERITEM_MAX; i++ )
	{
		if ( GetItemCounter( i ) != 0 )
			iCount++;
	}

	return iCount;
}

int QuestData::GetMonsterKilledAmount( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaMonsterKilled[i];
}

char * QuestData::GetMonsterName( int i )
{
	static char * pszNull = "";
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return pszNull;

	return szaMonsterName[i];
}

char * QuestData::GetItemName( int i )
{
	static char * pszNull = "";
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return pszNull;

	return szaItemName[i];
}

int QuestData::GetItemID( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaItemID[i];
}

int QuestData::GetItemCounter( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaItemCounter[i];
}

int QuestData::GetItemsCollected( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return 0;

	return iaItemsCollected[i];
}

BOOL QuestData::AddMonsterKill( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return FALSE;

	if ( IsValidQuest() == false )
		return FALSE;

	if ( iaMonsterKilled[i] < iaMonsterCounter[i] )
		iaMonsterKilled[i]++;

	ValidateQuest();

	return TRUE;
}

BOOL QuestData::AddItemCollected( int i )
{
	if ( i < 0 || i >= QUEST_MONSTERITEM_MAX )
		return FALSE;

	if ( IsValidQuest() == false )
		return FALSE;

	if ( iaItemsCollected[i] < iaItemCounter[i] )
		iaItemsCollected[i]++;

	ValidateQuest();

	return TRUE;
}
