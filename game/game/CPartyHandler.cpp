#include "stdafx.h"
#include "CPartyHandler.h"

CPartyHandler::CPartyHandler()
{
	pcPartyCore = new CPartyCoreHandler();

	pcPartyWindow = new CPartyWindow();
	ZeroMemory( &sPartyData, sizeof(CPartyData) );
}

CPartyHandler::~CPartyHandler()
{
	DELET( pcPartyWindow );

	DELET( pcPartyCore );
}

void CPartyHandler::HandlePacket( PacketUpdateParty * psPacket )
{
	if( psPacket )
	{
		UpdateSafeMembers( FALSE );

		//Setting Party Data
		ZeroMemory( &sPartyData, sizeof(CPartyData) );
		sPartyData.iMembersCount = (int)psPacket->cMembersCount;
		sPartyData.eMode = (EPartyMode)psPacket->cPartyMode;

		//Deleted Party
		if( sPartyData.iMembersCount < 1 )
		{
			pcPartyWindow->LeaveParty();
			return;
		}

		//Add Party Members to Party Data
		for( char i = 0; i < psPacket->cMembersCount; i++ )
			CopyMemory( &sPartyData.saMembers[i], &psPacket->saMembers[i], sizeof(PartyMember) );

		//Party it's in a Raid
		if( psPacket->cRaidCount > 0 )
		{
			sPartyData.iRaidCount = (int)psPacket->cRaidCount;

			for( size_t i = 0; i < MAX_RAID-1; i++ )
			{
				sPartyData.saRaid[i].cRaidID = psPacket->saRaid[i].cRaidID;

				for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
				{
					if( psPacket->saRaid[i].saMembers[j].sData.iID )
						CopyMemory( &sPartyData.saRaid[i].saMembers[j], &psPacket->saRaid[i].saMembers[j], sizeof(PartyRaidMember) );
				}
			}
		}

		//Update Party Window
		if( sPartyData.iMembersCount > 1 )
			pcPartyWindow->UpdateParty( &sPartyData );

		UpdateSafeMembers( TRUE );
	}
}

void CPartyHandler::HandlePacket( PacketRequestParty * psPacket )
{
	if( psPacket )
	{
		if( psPacket->eState == PARTYSTATE_Success || psPacket->eState == PARTYSTATE_RequestInvite )
			pcPartyWindow->ShowPartyNotification( psPacket->szCharacterName, psPacket->eState == PARTYSTATE_Success ? 1 : 2 );

		CopyMemory( &sPacketRequestParty, psPacket, sizeof(PacketRequestParty) );
	}
}

void CPartyHandler::HandlePacket( PacketUpdatePartyData * psPacket )
{
	if( psPacket )
	{
		UpdateSafeMembers( FALSE );

		//Find a Party member on PartyData of Client side to update the data
		if( psPacket->cMembersCount > 0 )
			for( char i = 0; i < psPacket->cMembersCount; i++ )
				for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
					if( sPartyData.saMembers[j].sData.iID == psPacket->saMembers[i].iID )
						CopyMemory( &sPartyData.saMembers[j].sData, &psPacket->saMembers[i], sizeof( PartyMemberData ) );

		//Find a Raid Party Member on PartyData of Client side to update the data
		for( size_t i = 0; i < MAX_RAID - 1; i++ )
			for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
				if( psPacket->saRaidMember[i][j].iID != 0 )
					for( size_t k = 0; k < MAX_RAID - 1; k++ )
						for( size_t l = 0; l < MAX_PARTY_MEMBERS; l++ )
							if( sPartyData.saRaid[k].saMembers[l].sData.iID == psPacket->saRaidMember[i][j].iID )
								CopyMemory( &sPartyData.saRaid[k].saMembers[l].sData, &psPacket->saRaidMember[i][j], sizeof(PartyRaidMemberData) );

		UpdateSafeMembers( TRUE );
	}
}

void CPartyHandler::HandlePacket( PacketRequestRaid * psPacket )
{
	if( psPacket )
	{
		if( psPacket->eState == PARTYSTATE_Success )
			pcPartyWindow->ShowRaidNotification( psPacket->szCharacterName );

		CopyMemory( &sPacketRequestRaid, psPacket, sizeof(PacketRequestRaid) );
	}
}

void CPartyHandler::ProcessPacket( PacketJoinParty * psPacket )
{
	psPacket->iLength = sizeof( PacketJoinParty );
	psPacket->iHeader = PKTHDR_JoinParty;
	SENDPACKETG( psPacket );

	ZeroMemory( &sPacketRequestParty, sizeof(PacketRequestParty) );
}

void CPartyHandler::ProcessPacket( PacketJoinRaid * psPacket )
{
	psPacket->iLength = sizeof( PacketJoinRaid );
	psPacket->iHeader = PKTHDR_JoinRaid;
	SENDPACKETG( psPacket );

	ZeroMemory( &sPacketRequestRaid, sizeof(PacketRequestParty) );
}

void CPartyHandler::ProcessPacket( PacketRequestParty * psPacket )
{
	psPacket->iLength = sizeof( PacketRequestParty );
	psPacket->iHeader = PKTHDR_RequestParty;
	SENDPACKETG( psPacket );

	ZeroMemory( &sPacketRequestParty, sizeof(PacketRequestParty) );
}

void CPartyHandler::ProcessPacket( PacketRequestRaid * psPacket )
{
	psPacket->iLength = sizeof( PacketRequestRaid );
	psPacket->iHeader = PKTHDR_RequestRaid;
	SENDPACKETG( psPacket );

	ZeroMemory( &sPacketRequestRaid, sizeof( PacketRequestRaid ) );
}

void CPartyHandler::ProcessPacket( PacketActionParty * psPacket )
{
	psPacket->iLength = sizeof( PacketActionParty );
	psPacket->iHeader = PKTHDR_ActionParty;
	SENDPACKETG( psPacket );
}

void CPartyHandler::Init()
{
	INIT( pcPartyWindow );

	INIT( pcPartyCore );
}

void CPartyHandler::Render()
{
	if( pcPartyWindow )
		pcPartyWindow->Render();

	pcPartyCore->Render();
}

void CPartyHandler::Update( float fTime )
{
	UNIT->bPartyLeader = false;

	for( int i = 0; i < sPartyData.iMembersCount; i++ )
	{
		UnitData * p = UNITDATABYID( sPartyData.saMembers[i].sData.iID );

		if( p && p->bActive )
		{
			sPartyData.saMembers[i].sData.sHP = p->iID == UNITDATA->iID ? CurMax(CHARACTERGAME->GetCurrentHP(),p->sCharacterData.sHP.sMax) : p->sCharacterData.sHP;
			sPartyData.saMembers[i].sData.sLevel = p->sCharacterData.iLevel;
			sPartyData.saMembers[i].sData.sMP = p->sCharacterData.sMP;
			sPartyData.saMembers[i].sData.sPosition = p->sPosition;

			//Leader?
			if ( p->iID == UNITDATA->iID && i == 0 )
				UNIT->bPartyLeader = true;
		}
	}

	for( int i = 0; i < sPartyData.iRaidCount; i++ )
	{
		for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
		{
			UnitData * p = UNITDATABYID( sPartyData.saRaid[i].saMembers[j].sData.iID );
			if( p && p->bActive )
			{
				sPartyData.saRaid[i].saMembers[j].sData.sHP = p->sCharacterData.sHP;
				sPartyData.saRaid[i].saMembers[j].sData.sPosition = p->sPosition;
				continue;
			}
		}
	}

	UpdateSafeMembers( TRUE );

	if( pcPartyWindow )
		pcPartyWindow->Update( fTime );

	pcPartyCore->Update( fTime );
}

void CPartyHandler::OnMouseMove( CMouse * pcMouse )
{
	if( pcPartyWindow )
		pcPartyWindow->OnMouseMove( pcMouse );

	pcPartyCore->OnMouseMove( pcMouse );
}

BOOL CPartyHandler::OnMouseClick( CMouse * pcMouse )
{
	BOOL bRet = FALSE;

	bRet = pcPartyCore->OnMouseClick( pcMouse );

	if( pcPartyWindow && bRet == FALSE )
		bRet = pcPartyWindow->OnMouseClick( pcMouse );

	return bRet;
}

void CPartyHandler::AcceptPartyRequest()
{
	sPacketRequestParty.eState = PARTYSTATE_Success;
	ProcessPacket( &sPacketRequestParty );
}

void CPartyHandler::JoinPartyRequest()
{
	PacketJoinParty p;
	p.iID = sPacketRequestParty.iID;
	ProcessPacket( &p );
}

void CPartyHandler::JoinRaidRequest()
{
	PacketJoinRaid p;
	p.iID = sPacketRequestRaid.iID;
	ProcessPacket( &p );
}

BOOL CPartyHandler::IsPartyMember( int iID )
{
	for( int i = 0; i < sPartyData.iMembersCount; i++ )
		if( sPartyData.saMembers[i].sData.iID == iID )
			return TRUE;

	return FALSE;
}

BOOL CPartyHandler::IsRaidMember( int iID )
{
	for( int i = 0; i < sPartyData.iRaidCount; i++ )
		for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
			if( sPartyData.saRaid[i].saMembers[j].sData.iID == iID )
				return TRUE;

	return FALSE;
}

std::vector<UnitData*> CPartyHandler::GetPartyMembers( bool bRaid )
{
	std::vector<UnitData*> v;

	if( sPartyData.iMembersCount > 0 )
	{
		for( int i = 0; i < MAX_PARTY_MEMBERS; i++ )
			if( sPartyData.saMembers[i].sData.iID )
				v.push_back( UNITGAME->GetUnitDataByID(sPartyData.saMembers[i].sData.iID) );

		if( bRaid )
		{
			for( int i = 0; i < MAX_RAID-1; i++ )
				for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
					if( sPartyData.saRaid[i].saMembers[j].sData.iID != 0 && sPartyData.saRaid[i].saMembers[j].szCharacterName[0] != 0 )
						v.push_back( UNITGAME->GetUnitDataByID( sPartyData.saRaid[i].saMembers[j].sData.iID ) );
		}
	}

	return v;
}

std::vector<UnitData*> CPartyHandler::GetPartyMembersArea( int iArea )
{
	std::vector<UnitData*> vRet;
	auto v = GetPartyMembers();

	for ( auto &p : v )
	{
		if ( p != UNITDATA )
		{
			if ( p && p->bActive )
			{
				int iX = (UNITDATA->sPosition.iX - p->sPosition.iX) >> 8;
				int iY = (UNITDATA->sPosition.iY - p->sPosition.iY) >> 8;
				int iZ = (UNITDATA->sPosition.iZ - p->sPosition.iZ) >> 8;
				int iDistance = iX * iX + iY * iY + iZ * iZ;

				if ( iDistance < (iArea * iArea) )
				{
					vRet.push_back( p );
				}
			}
		}
	}

	return vRet;
}

void CPartyHandler::UpdateSafeMembers( BOOL bSafe )
{
	std::vector<UnitData*> vMembers = GetPartyMembers();

	for ( auto & v : vMembers )
	{
		if ( v && v != UNITDATA )
		{
			if ( bSafe )
			{
				if ( PARTYHANDLER->IsPartyMember( v->iID ) )
					v->dwExclusiveNum = 1;
				else if ( PARTYHANDLER->IsRaidMember( v->iID ) )
					v->dwExclusiveNum = 2;
			}
			else
				v->dwExclusiveNum = 0;
		}
	}
}
