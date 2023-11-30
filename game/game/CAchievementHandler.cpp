#include "stdafx.h"
#include "CAchievementHandler.h"


CAchievementHandler::CAchievementHandler()
{
	pcWindow = new CAchievementWindow();

	ZeroMemory( saPacketCache, sizeof( PacketAchievementUser ) * _countof( saPacketCache ) );
}


CAchievementHandler::~CAchievementHandler()
{
	DELET( pcWindow );
}

void CAchievementHandler::Init()
{
	pcWindow->Init();
}

void CAchievementHandler::Render()
{
	pcWindow->Render();
}

void CAchievementHandler::Update( float fTime )
{
}

void CAchievementHandler::HandlePacket( PacketAchievement * psPacket )
{
	//New Achievement
	CAchievement * pcAchievement = new CAchievement();
	pcAchievement->SetAchievement( psPacket->iID, psPacket->iType, psPacket->szName, psPacket->szImagePath, psPacket->szDescription );

	//Handle Events
	for ( int i = 0; i < psPacket->iEventCount; i++ )
	{
		//Add new Event to the Achievement
		CAchievementEvent * pcAchievementEvent = new CAchievementEvent();
		pcAchievementEvent->SetID( psPacket->saEvent[i].iID );
		pcAchievementEvent->SetType( psPacket->saEvent[i].iType );
		pcAchievementEvent->SetIsLevelAchievement( psPacket->saEvent[i].bIsLevelAchievement );
		pcAchievementEvent->SetValue( 0, psPacket->saEvent[i].szaValue[0] );
		pcAchievementEvent->SetValue( 1, psPacket->saEvent[i].szaValue[1] );
		pcAchievementEvent->SetValue( 2, psPacket->saEvent[i].szaValue[2] );
		pcAchievementEvent->SetIsUnique( psPacket->saEvent[i].bIsUnique );

		pcAchievement->AddEvent( pcAchievementEvent );
	}

	//Add Achievement to the list
	vAchievement.push_back( pcAchievement );
}

void CAchievementHandler::HandlePacket( PacketAchievementUser * psPacket, BOOL bCanCache )
{
	CAchievementUser * pcAchievementUser = new CAchievementUser();
	pcAchievementUser->SetID( psPacket->iAchievementID );
	pcAchievementUser->SetLevel( psPacket->iLevel );
	pcAchievementUser->SetValue( 0, psPacket->szaValue[0] );
	pcAchievementUser->SetValue( 1, psPacket->szaValue[1] );
	pcAchievementUser->SetValue( 2, psPacket->szaValue[2] );
	pcAchievementUser->SetDate( psPacket->sDate );

	if ( (psPacket->iID == 0) || (psPacket->iID == UNITDATA->iID) )
	{
		AddUnitAchievement( UNIT, pcAchievementUser );
	}
	else
	{
		Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iID ) );
		if ( pcUnit )
			AddUnitAchievement( pcUnit, pcAchievementUser );
		else
		{
			if ( bCanCache )
				CopyMemory( saPacketCache + (uCachePacketWheel++ % MAX_ACHIEVEMENT_PACKET_CACHE), psPacket, sizeof( PacketAchievementUser ) );

			DELET( pcAchievementUser );
		}
	}
}

CAchievement * CAchievementHandler::GetAchievement( int iID )
{
	CAchievement * pcAchievement = NULL;

	for ( std::vector<CAchievement*>::iterator it = vAchievement.begin(); it != vAchievement.end(); it++ )
	{
		CAchievement * pc = (*it);
		if ( pc->GetID() == iID )
		{
			pcAchievement = pc;
			break;
		}
	}

	return pcAchievement;
}

int CAchievementHandler::GetAchievementID( EAchievementType iType )
{
	int iRet = 0;
	CAchievement * pcAchievement = GetAchievement( iType );
	if ( pcAchievement )
		iRet = pcAchievement->GetID();

	return iRet;
}

CAchievement * CAchievementHandler::GetAchievement( EAchievementType iType )
{
	CAchievement * pcAchievement = NULL;

	if ( vAchievement.size() > 0 )
	{
		for ( std::vector<CAchievement*>::iterator it = vAchievement.begin(); it != vAchievement.end(); it++ )
		{
			CAchievement * pc = (*it);

			//Get Events of that Achievement
			for ( std::vector<CAchievementEvent*>::iterator itE = pc->GetEvents().begin(); itE != pc->GetEvents().end(); itE++ )
			{
				CAchievementEvent * pcAchievementEvent = (*itE);

				if ( pcAchievementEvent->GetType() == iType )
				{
					pcAchievement = pc;
					break;
				}
			}
		}
	}

	return pcAchievement;
}

BOOL CAchievementHandler::HaveAchievement( Unit * pcUnit, EAchievementType iType )
{
	BOOL bRet = FALSE;

	int iID = GetAchievementID( iType );

	for ( const auto & v : pcUnit->vAchievement )
	{
		if ( v->GetID() == iID )
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

void CAchievementHandler::AddWindowAchievementList( Unit * pcUnit )
{
	GetWindow()->ClearAchievementsList();

	for ( const auto & v : pcUnit->vAchievement )
	{
		CAchievement * pcAchievement = GetAchievement( v->GetID() );
		if ( pcAchievement )
			GetWindow()->AddAchievement( pcAchievement, pcUnit );
	}
}

void CAchievementHandler::ProcessCache( Unit * pcUnit )
{
	if ( pcUnit )
	{
		for ( int i = 0; i < MAX_ACHIEVEMENT_PACKET_CACHE; i++ )
		{
			if ( saPacketCache[i].iHeader && (saPacketCache[i].iID == pcUnit->GetID()) )
			{
				CAchievementUser * pcAchievementUser = new CAchievementUser();
				pcAchievementUser->SetID( saPacketCache[i].iAchievementID );
				pcAchievementUser->SetLevel( saPacketCache[i].iLevel );
				pcAchievementUser->SetValue( 0, saPacketCache[i].szaValue[0] );
				pcAchievementUser->SetValue( 1, saPacketCache[i].szaValue[1] );
				pcAchievementUser->SetValue( 2, saPacketCache[i].szaValue[2] );
				pcAchievementUser->SetDate( saPacketCache[i].sDate );
				AddUnitAchievement( pcUnit, pcAchievementUser );

				saPacketCache[i].iHeader = 0;
			}
		}
	}
}

BOOL CAchievementHandler::AddUnitAchievement( Unit * pcUnit, CAchievementUser * pcAchievementUser )
{
	BOOL bRet = TRUE;
	for ( std::vector<CAchievementUser*>::iterator it = pcUnit->vAchievement.begin(); it != pcUnit->vAchievement.end(); )
	{
		CAchievementUser * pc = (*it);
		if ( pc->GetID() == pcAchievementUser->GetID() )
		{
			DELET( pc );
			it = pcUnit->vAchievement.erase( it );
		}
		else
			it++;
	}

	pcUnit->vAchievement.push_back( pcAchievementUser );

	return bRet;
}
