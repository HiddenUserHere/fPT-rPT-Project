#include "stdafx.h"
#include "CPartyHandler.h"

CPartyHandler::CPartyHandler()
{
	psaParty = new PartyInfo[MAX_PARTY];
	ZeroMemory( psaParty, sizeof( PartyInfo ) * MAX_PARTY );
}

CPartyHandler::~CPartyHandler()
{
	DELETA( psaParty );
}

void CPartyHandler::ProcessPacket( User * pcUser, PacketRequestParty * psPacket )
{
	psPacket->iLength = sizeof(PacketRequestParty);
	psPacket->iHeader = PKTHDR_RequestParty;
	SENDPACKET( pcUser, psPacket );
}

void CPartyHandler::ProcessPacket( User * pcUser, PacketUpdateParty * psPacket )
{
	psPacket->iLength = sizeof(PacketUpdateParty);
	psPacket->iHeader = PKTHDR_UpdateParty;
	SENDPACKET( pcUser, psPacket );
}

void CPartyHandler::ProcessPacket( User * pcUser, PacketUpdatePartyData * psPacket )
{
	psPacket->iLength = sizeof(PacketUpdatePartyData);
	psPacket->iHeader = PKTHDR_UpdatePartyData;
	SENDPACKET( pcUser, psPacket );
}

void CPartyHandler::ProcessPacket( User * pcUser, PacketRequestRaid * psPacket )
{
	psPacket->iLength = sizeof(PacketRequestRaid);
	psPacket->iHeader = PKTHDR_RequestRaid;
	SENDPACKET( pcUser, psPacket );
}

void CPartyHandler::HandlePacket( User * pcUser, PacketRequestParty * psPacket )
{
	if( !pcUser || !psPacket )
		return;

	if( psPacket->iTargetID && psPacket->eState != PARTYSTATE_Failed )
	{
		User * pcTargetUser = USERDATATOUSER( USERDATABYID( psPacket->iTargetID ) );
		
		//User found?
		if( pcTargetUser )
		{
			//Request Party to myself
			if( pcUser == pcTargetUser )
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You can't sent a Party Request to yourself" );
				return;
			}

			//In Battle Royale
			if ( pcUser->bInBattleRoyale )
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You can't sent a Party Request while you are at Battle Royale" );
				return;
			}

			//In Battle Royale
			if ( pcTargetUser->bInBattleRoyale )
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> You can't sent a Party Request while friend is at Battle Royale" );
				return;
			}

			//Party disabled on User
			if( pcTargetUser->pcUserData->bDisabledParty )
			{
				CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s has disabled party.", pcTargetUser->pcUserData->sCharacterData.szName );
				return;
			}

			//User already on Party?
			if( pcUser->bParty )
			{
				if( pcUser->psParty )
				{
					if( pcTargetUser->bParty && pcTargetUser->psParty )
					{
						psPacket->eState = PARTYSTATE_AlreadyParty;
						ProcessPacket( pcUser, psPacket );

						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s is already in a Party!", pcTargetUser->pcUserData->sCharacterData.szName );
						return;
					}

					//User it's a Party Leader?
					if( pcUser->psParty->pcLeader == pcUser )
					{
						psPacket->iID = pcUser->GetID();
						psPacket->eState = PARTYSTATE_Success;
						STRINGCOPY( psPacket->szCharacterName, pcUser->pcUserData->sCharacterData.szName );
						ProcessPacket( pcTargetUser, psPacket );

						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Sent Party Request to %s!", pcTargetUser->pcUserData->sCharacterData.szName );
					}
					else
					{
						psPacket->iTargetID = pcTargetUser->GetID();
						psPacket->eState = PARTYSTATE_RequestInvite;
						STRINGCOPY( psPacket->szCharacterName, pcTargetUser->pcUserData->sCharacterData.szName );
						ProcessPacket( pcUser->psParty->pcLeader, psPacket );

						SendChatParty( pcUser, FormatString( "> %s recommended %s to join the Party!", pcUser->pcUserData->sCharacterData.szName, pcTargetUser->pcUserData->sCharacterData.szName ), CHATCOLOR_Error );
					}
				}
			}
			else if( pcTargetUser->bParty )
			{
				if( pcTargetUser->psParty )
				{
					//Target it's a Leader from Party?
					if( pcTargetUser->psParty->pcLeader == pcTargetUser )
					{
						if( pcTargetUser->psParty->iMembersCount == MAX_PARTY_MEMBERS )
						{
							psPacket->eState = PARTYSTATE_FullParty;
							ProcessPacket( pcUser, psPacket );

							CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Party is currently Full!" );
							return;
						}
						else if( pcTargetUser->psParty == pcUser->psParty )
						{
							psPacket->eState = PARTYSTATE_AlreadyParty;
							ProcessPacket( pcUser, psPacket );

							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s is already in a Party!", pcTargetUser->pcUserData->sCharacterData.szName );
						}
						else
						{
							psPacket->iID = pcUser->GetID();
							psPacket->eState = PARTYSTATE_Success;
							STRINGCOPY( psPacket->szCharacterName, pcUser->pcUserData->sCharacterData.szName );
							ProcessPacket( pcTargetUser, psPacket );

							CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Sent Party Request to %s!", pcTargetUser->pcUserData->sCharacterData.szName );
						}
					}
					else
					{
						psPacket->eState = PARTYSTATE_AlreadyParty;
						ProcessPacket( pcUser, psPacket );

						CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> %s is already in a Party!", pcTargetUser->pcUserData->sCharacterData.szName );
						return;
					}
				}
			}
			else
			{
				psPacket->iID = pcUser->GetID();
				psPacket->eState = PARTYSTATE_Success;
				STRINGCOPY( psPacket->szCharacterName, pcUser->pcUserData->sCharacterData.szName );
				ProcessPacket( pcTargetUser, psPacket );

				CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Sent Party Request to %s!", pcTargetUser->pcUserData->sCharacterData.szName );
			}
		}
	}
}

void CPartyHandler::HandlePacket( User * pcUser, PacketJoinParty * psPacket )
{
	if( !pcUser || !psPacket )
		return;

	User * pcOtherUser = USERDATATOUSER( USERDATABYID(psPacket->iID) );

	if ( pcOtherUser == nullptr )
		return;

	if( !pcUser->bParty && !pcOtherUser->bParty )
		DoParty( pcUser, pcOtherUser );
	else if( !pcUser->bParty && pcOtherUser->psParty )
		JoinParty( pcUser, pcOtherUser->psParty );
	else if( pcUser->psParty && !pcOtherUser->psParty )
		JoinParty( pcOtherUser, pcUser->psParty );

	ACTIONFIELDHANDLER->OnEnterParty( pcUser );
}

void CPartyHandler::HandlePacket( User * pcUser, PacketActionParty * psPacket )
{
	if( !pcUser || !psPacket )
		return;

	if( !pcUser->bParty || !pcUser->psParty )
		return;

	switch( psPacket->eAction )
	{
		case PARTYACTION_Leave:
			this->LeaveParty( pcUser );
			break;
		case PARTYACTION_Kick:
			if( pcUser->psParty->pcLeader == pcUser )
			{
				User * pcOtherUser = USERDATATOUSER( USERDATABYID(psPacket->iUserID) );
				if( pcOtherUser )
				{
					//Same party?
					if ( (pcOtherUser->bParty && pcOtherUser->psParty) && pcUser->psParty == pcOtherUser->psParty )
						this->LeaveParty( pcOtherUser, true );
				}
			}
			break;
		case PARTYACTION_Disband:
			if( pcUser->psParty->pcLeader == pcUser )
				this->DeleteParty( pcUser->psParty );
			break;
		case PARTYACTION_Delegate:
			if( pcUser->psParty->pcLeader == pcUser )
			{
				User * pcOtherUser = USERDATATOUSER( USERDATABYID(psPacket->iUserID) );
				if( pcOtherUser )
				{
					//Same party?
					if( (pcOtherUser->bParty && pcOtherUser->psParty) && pcUser->psParty == pcOtherUser->psParty )
						this->DelegateParty( pcOtherUser );
				}
			}
			break;
		case PARTYACTION_ChangeMode:
			if( pcUser->psParty->pcLeader == pcUser )
				this->ChangePartyMode( pcUser->psParty );
			break;
	}
}

void CPartyHandler::HandlePacket( User * pcUser, PacketRequestRaid * psPacket )
{
	if( !pcUser || !psPacket )
		return;

	User * pcTargetUser = USERDATATOUSER( USERDATABYID( psPacket->iTargetID ) );
	if( pcTargetUser )
	{
		//Both in a party?
		if( (pcUser->bParty && pcUser->psParty) && (pcTargetUser->bParty && pcTargetUser->psParty) )
		{
			//Are both leaders of Party?
			if( pcUser->psParty->pcLeader == pcUser && pcTargetUser->psParty->pcLeader == pcTargetUser )
			{
				//Raid isn't full?
				if( (pcUser->psParty->iRaidCount < MAX_RAID - 1) && (pcTargetUser->psParty->iRaidCount < MAX_RAID - 1) )
				{
					psPacket->iID = pcUser->GetID();
					psPacket->eState = RAIDSTATE_Success;
					STRINGCOPY( psPacket->szCharacterName, pcUser->pcUserData->sCharacterData.szName );
					ProcessPacket( pcTargetUser, psPacket );

					CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Sent Raid request to %s!", pcTargetUser->pcUserData->sCharacterData.szName );
				}
				else
				{
					CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Raid is currently Full!" );
					return;
				}
			}
			else
			{
				CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Raid must be formed between party leaders!" );
				return;
			}
		}
	}
}

void CPartyHandler::HandlePacket( User * pcUser, PacketJoinRaid * psPacket )
{
	if( !pcUser || !psPacket || !pcUser->psParty )
		return;

	User * pcOtherUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) );
	if( pcOtherUser && pcOtherUser->psParty )
	{
		if( !pcUser->psParty->pcRaidLeader && !pcOtherUser->psParty->pcRaidLeader )
			DoRaid( pcOtherUser, pcUser );
		else if( pcOtherUser->psParty->pcRaidLeader == pcOtherUser->psParty && !pcUser->psParty->pcRaidLeader )
			JoinRaid( pcUser, pcOtherUser->psParty );
	}
}

void CPartyHandler::HandlePacket( User * pcUser, PacketPartyCore * psPacket )
{
	if ( User * pcOtherUser = USERDATATOUSER( USERDATABYID( psPacket->iID ) ) )
	{
		if ( pcUser->pcUserData->dwExclusiveNum || pcOtherUser->pcUserData->dwExclusiveNum ||
			MAPSERVER->IsBellatraMapArea( pcOtherUser->pcUserData->sPosition.iX, pcOtherUser->pcUserData->sPosition.iY, pcOtherUser->pcUserData->sPosition.iZ ) ||
			pcOtherUser->bInBattleRoyale ||
			FURYARENAHANDLER->InsideArena( &pcOtherUser->pcUserData->sPosition ) || // Inside Fury Arena?
			pcOtherUser->pcUserData->iMapID == MAPID_QuestArena || pcOtherUser->pcUserData->iMapID == MAPID_T5QuestArena || pcOtherUser->pcUserData->iMapID == MAPID_BlessCastle ||
			pcOtherUser->pcUserData->iMapID == MAPID_OldRuinen2 || pcOtherUser->pcUserData->iMapID == MAPID_RoyalDesert )
		{
			CHATSERVER->SendChatMessageBox( pcUser, "You cannot teleport to player" );
			return;
		}

		Map * pcMap = MAPSDATA + pcOtherUser->pcUserData->iMapID;
		if ( pcMap )
		{
			if ( pcMap->pcBaseMap )
			{
				if ( pcUser->pcUserData->sCharacterData.iLevel < pcMap->pcBaseMap->iLevel )
				{
					CHATSERVER->SendChatMessageBox( pcUser, "Level does not match" );
					return;
				}


				PacketUserWarpEffect sPacketEffect;
				ZeroMemory( &sPacketEffect, sizeof( PacketUserWarpEffect ) );
				sPacketEffect.iID = 0x100;
				sPacketEffect.iX = pcUser->pcUserData->sPosition.iX;
				sPacketEffect.iY = pcUser->pcUserData->sPosition.iY;
				sPacketEffect.iZ = pcUser->pcUserData->sPosition.iZ;
				CALL_WITH_ARG2( 0x00552130, (DWORD)pcUser->pcUserData, (DWORD)& sPacketEffect );

				// Success, Send coordinates to teleport to player
				PacketSendUseUnionCore sPacket;
				sPacket.iLength = sizeof( PacketSendUseUnionCore );
				sPacket.iHeader = PKTHDR_UseUnionCore;
				sPacket.iMapID = pcMap->pcBaseMap->GetID();
				sPacket.iX = pcOtherUser->pcUserData->sPosition.iX + (rand() % 256);
				sPacket.iY = pcOtherUser->pcUserData->sPosition.iY;
				sPacket.iZ = pcOtherUser->pcUserData->sPosition.iZ + (rand() % 256);
				sPacket.dwItemID = psPacket->sItem.sItemID.ToItemID();
				sPacket.dwItemHead = psPacket->sItem.iChk1;
				sPacket.dwItemChecksum = psPacket->sItem.iChk2;
				sPacket.dwUnk = 0;
				SENDPACKET( pcUser, &sPacket, TRUE );

				//pcUser->pcUserData->iMapID = (EMapID)pcMap->pcBaseMap->GetID();

				SENDPACKET( pcUser, psPacket );
			}
		}
	}
}

void CPartyHandler::SendChatParty( User * pcUser, std::string strMessage, EChatColor eChatColor, bool bSendRaid )
{
	if( pcUser && pcUser->psParty && pcUser->bParty )
	{
		//Send message to Party Members
		for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
		{
			User * p = i == 0 ? pcUser->psParty->pcLeader : pcUser->psParty->pcaMembers[i - 1];

			if( p )
				CHATSERVER->SendChat( p, eChatColor, strMessage.c_str() );
		}

		//Send message to others Party (Raid)
		if( pcUser->psParty->iRaidCount > 0 && bSendRaid )
		{
			for( size_t i = 0; i < MAX_RAID; i++ )
			{
				PartyInfo * psParty = i == 0 ? pcUser->psParty->pcRaidLeader : pcUser->psParty->pcaRaid[i-1];
				if( psParty && pcUser->psParty != psParty )
				{
					for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
					{
						User * p = j == 0 ? psParty->pcLeader : psParty->pcaMembers[j-1];

						if( p )
							CHATSERVER->SendChat( p, eChatColor, strMessage.c_str() );
					}
				}
			}
		}
	}
}

void CPartyHandler::SendChatRaid( User * pcUser, std::string strMessage, EChatColor eChatColor )
{
	if( pcUser && pcUser->psParty && pcUser->bParty )
	{
		//Send message to others Party (Raid)
		if( pcUser->psParty->iRaidCount > 0 )
		{
			for( size_t i = 0; i < MAX_RAID; i++ )
			{
				PartyInfo * psParty = i == 0 ? pcUser->psParty->pcRaidLeader : pcUser->psParty->pcaRaid[i-1];
				if( psParty )
				{
					for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
					{
						User * p = j == 0 ? psParty->pcLeader : psParty->pcaMembers[j-1];

						if( p )
							CHATSERVER->SendChat( p, eChatColor, strMessage.c_str() );
					}
				}
			}
		}
	}
}

void CPartyHandler::LeaveParty( User * pcUser, bool bKick )
{
	if( pcUser )
	{
		if( pcUser->bParty )
		{
			PartyInfo * psParty = pcUser->psParty;

			//Valid Party?
			if( psParty )
			{
				//Have more than one member?
				if( psParty->iMembersCount > 2 )
				{
					//It's a party Leader?
					if( psParty->pcLeader == pcUser )
					{
						psParty->pcLeader = psParty->pcaMembers[0];
						psParty->pcaMembers[0] = NULL;

						ArrangePartyMembers( psParty );
						psParty->iMembersCount--;

						UpdateParty( psParty );

						//Delete Party from User
						pcUser->bParty = FALSE;
						pcUser->psParty = NULL;
						ProcessPacket( pcUser, &PacketUpdateParty() );

						SendChatParty( psParty->pcLeader, FormatString("> %s has left the party.", pcUser->pcUserData->sCharacterData.szName), CHATCOLOR_Error );
						SendChatParty( psParty->pcLeader, FormatString("> %s has been delegated as the New party leader.", psParty->pcLeader->pcUserData->sCharacterData.szName), CHATCOLOR_Error );
					}
					else
					{
						//Update Party Settings
						for( size_t i = 0; i < MAX_PARTY_MEMBERS - 1; i++ )
						{
							if( psParty->pcaMembers[i] == pcUser )
							{
								psParty->pcaMembers[i] = NULL;

								ArrangePartyMembers( psParty );
								psParty->iMembersCount--;

								UpdateParty( psParty );

								SendChatParty( psParty->pcLeader, FormatString( bKick ? "> %s has been expelled from the party." : "> %s has left the party.", pcUser->pcUserData->sCharacterData.szName ), CHATCOLOR_Error );
							}
						}

						//Delete Party from User
						pcUser->bParty = FALSE;
						pcUser->psParty = NULL;
						ProcessPacket( pcUser, &PacketUpdateParty() );
					}
				}
				else
					DeleteParty( psParty );

				ACTIONFIELDHANDLER->OnLeaveParty( pcUser );
				DROPAREAHANDLER->OnLeaveParty( pcUser );
			}
		}
	}
}

void CPartyHandler::UpdatePartyData( User * pcUser, PartyInfo * psParty )
{
	if( psParty && pcUser )
	{
		PacketUpdatePartyData sPacket;
		ZeroMemory( &sPacket, sizeof(PacketUpdatePartyData) );

		//Update Party Data
		for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
		{
			User * p = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];
			if( p && p != pcUser )
			{
				bool bUpdate = false;

				//Map it's different or User it's longer? So update it
				if( p->GetMapID() != pcUser->GetMapID() )
					bUpdate = true;
				else if( !p->pcUserData->sPosition.WithinPTDistance( &pcUser->pcUserData->sPosition, DISTANCE_UNIT_PARTY_MAX ) )
					bUpdate = true;

				//Update Member Data
				if( bUpdate )
				{
					sPacket.saMembers[sPacket.cMembersCount].iID = p->GetID();
					sPacket.saMembers[sPacket.cMembersCount].sLevel = p->pcUserData->sCharacterData.iLevel;
					sPacket.saMembers[sPacket.cMembersCount].sHP.sCur = p->pcUserData->sCharacterData.sHP.sCur;
					sPacket.saMembers[sPacket.cMembersCount].sHP.sMax = p->pcUserData->sCharacterData.sHP.sMax;
					sPacket.saMembers[sPacket.cMembersCount].sMP.sCur = p->pcUserData->sCharacterData.sMP.sCur;
					sPacket.saMembers[sPacket.cMembersCount].sMP.sMax = p->pcUserData->sCharacterData.sMP.sMax;
					sPacket.saMembers[sPacket.cMembersCount].sPosition.iX = p->pcUserData->sPosition.iX;
					sPacket.saMembers[sPacket.cMembersCount].sPosition.iY = p->pcUserData->sPosition.iY;
					sPacket.saMembers[sPacket.cMembersCount].sPosition.iZ = p->pcUserData->sPosition.iZ;

					sPacket.cMembersCount++;
				}
			}
		}

		//Update Raid Party Data
		if( psParty->iRaidCount > 0 )
		{
			int iIndex = 0;
			for( size_t i = 0; i < MAX_RAID; i++ )
			{
				PartyInfo * pRaidParty = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i - 1];
				if( pRaidParty && (pRaidParty != psParty) )
				{
					int iRaidMemberCount = 0;
					for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
					{
						User * p = j == 0 ? pRaidParty->pcLeader : pRaidParty->pcaMembers[j - 1];
						if( p && p != pcUser )
						{
							bool bUpdate = false;

							//Map it's different or User it's longer? So update it
							if( p->GetMapID() != pcUser->GetMapID() )
								bUpdate = true;
							else if( !p->pcUserData->sPosition.WithinPTDistance( &pcUser->pcUserData->sPosition, DISTANCE_UNIT_PARTY_MAX ) )
								bUpdate = true;

							//Update Member Data
							if( bUpdate )
							{
								sPacket.saRaidMember[iIndex][iRaidMemberCount].iID = p->GetID();
								sPacket.saRaidMember[iIndex][iRaidMemberCount].sHP = p->pcUserData->sCharacterData.sHP;
								sPacket.saRaidMember[iIndex][iRaidMemberCount].sPosition = p->pcUserData->sPosition;
								iRaidMemberCount++;
								sPacket.cRaidMembersCount++;
							}
						}
					}

					iIndex++;
				}
			}
		}
		
		if( sPacket.cMembersCount > 0 || sPacket.cRaidMembersCount > 0 )
			ProcessPacket( pcUser, &sPacket );
	}
}

void CPartyHandler::SendActivePartyInfo( User * pcUser, int iMapID )
{
	int iPartyCount = 0;

	for ( int i = 0; i < MAX_PARTY; i++ )
	{
		PartyInfo * ps = psaParty + i;

		if ( ps && ps->iMembersCount > 0 )
		{
			User * p = ps->pcLeader;
			if ( p )
			{
				if ( ((iMapID != -1) && (p->GetMapID() == iMapID)) || (iMapID == -1) )
				{
					CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> [%d] Leader[%s] MembersCount[%d] MapID[%d]", i, CHARACTERSERVER->GetCharacterName( p->pcUserData ), ps->iMembersCount, p->GetMapID() );
					iPartyCount++;
				}
			}
		}
	}

	CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Error, "> Total Active Party: %d", iPartyCount );
}

std::vector<User*> CPartyHandler::GetPartyMembers( PartyInfo * psParty, bool bRaid )
{
	std::vector<User*> v;
	if( psParty )
	{
		//Get Party Members
		if( psParty->iMembersCount > 1 )
		{
			for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
			{
				User * u = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i-1];
				if( u && u->pcUserData && u->pcUserData->pcSocketData )
					v.push_back( u );
			}
		}

		//Get Raid Members
		if( bRaid && psParty->iRaidCount > 0 )
		{
			for( size_t i = 0; i < MAX_RAID; i++ )
			{
				PartyInfo * p = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i-1];
				if( p && p != psParty )
				{
					for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
					{
						User * u = j == 0 ? p->pcLeader : p->pcaMembers[j-1];
						if( u && u->pcUserData && u->pcUserData->pcSocketData )
							v.push_back( u );
					}
				}
			}
		}
	}

	return v;
}

BOOL CPartyHandler::DoParty( User * pcUser, User * pcOtherUser )
{
	if( !pcOtherUser )
		return FALSE;

	PartyInfo * psParty = GetFreeParty();
	ZeroMemory( psParty, sizeof(PartyInfo) );

	psParty->iID = TICKCOUNT;
	psParty->iMembersCount = 1;
	psParty->pcLeader = pcOtherUser;

	if( pcUser )
	{
		psParty->pcaMembers[psParty->iMembersCount - 1] = pcUser;
		psParty->iMembersCount++;

		//Setting Other User
		pcUser->bParty = TRUE;
		pcUser->psParty = psParty;
	}

	//Setting User
	pcOtherUser->bParty = TRUE;
	pcOtherUser->psParty = psParty;

	//Update Party
	UpdateParty( psParty );

	return TRUE;
}

BOOL CPartyHandler::JoinParty( User * pcUser, PartyInfo * psParty )
{
	if( pcUser )
	{
		if( psParty->iMembersCount >= MAX_PARTY_MEMBERS )
		{
			CHATSERVER->SendChat( pcUser, CHATCOLOR_Error, "> Party is currently Full!" );
			return FALSE;
		}

		psParty->pcaMembers[psParty->iMembersCount - 1] = pcUser;
		psParty->iMembersCount++;

		//Setting Other User
		pcUser->bParty = TRUE;
		pcUser->psParty = psParty;

		//Update Party
		UpdateParty( psParty );

		SendChatParty( pcUser, FormatString("> %s has joined the Party!", pcUser->pcUserData->sCharacterData.szName), CHATCOLOR_Error );
	}

	return TRUE;
}

BOOL CPartyHandler::DeleteParty( PartyInfo * psParty )
{
	if( psParty )
	{
		//Delete party from Raid
		if( psParty->iRaidCount > 0 )
		{
			//A loop to make a verification on each party inside Raid
			for( size_t i = 0; i < MAX_RAID; i++ )
			{
				PartyInfo * p = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i-1];
				if( p != psParty )
				{
					//Party is leaving its a raid leader?
					if( p->pcRaidLeader == psParty )
					{
						p->pcRaidLeader = NULL;

						if( p->pcaRaid[0] != p )
						{
							p->pcRaidLeader = p->pcaRaid[0];
							p->pcaRaid[0] = NULL;
							ArrangeRaidParty( p );
						}

						p->iRaidCount--;
					}
					else
					{
						//Check Raid array to delete the party from this
						for( size_t j = 0; j < MAX_RAID-1; j++ )
						{
							PartyInfo * pPartyToDelete = p->pcaRaid[j];
							if( pPartyToDelete == psParty )
							{
								p->pcaRaid[j] = NULL;
								p->iRaidCount--;
								ArrangeRaidParty( p );
								break;
							}
						}
					}

					//Don't have Raid Leader anymore? So delete Raid
					if( !p->pcRaidLeader || p->iRaidCount < 1 )
					{
						p->pcRaidLeader = NULL;
						p->iRaidCount = 0;
						ZeroMemory( p->pcaRaid, 4 * (MAX_RAID-1) );
						SendChatParty( p->pcLeader, "> Raid has been disbanded!", CHATCOLOR_Error, false );
					}
					else
						SendChatParty( p->pcLeader, "> Party has left the Raid.", CHATCOLOR_Error, true );
				}
			}
		}

		psParty->iMembersCount = 0;

		UpdateParty( psParty );

		//Delete party from User
		for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
		{
			User * p = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i-1];
			if( p )
			{
				p->bParty  = NULL;
				p->psParty = NULL;
			}
		}

		ZeroMemory( psParty, sizeof( PartyInfo ) );

		return TRUE;
	}

	return FALSE;
}

void CPartyHandler::DelegateParty( User * pcUser )
{
	if( pcUser->bParty && pcUser->psParty )
	{
		User * pLeader = pcUser->psParty->pcLeader;
		pcUser->psParty->pcLeader = pcUser;

		for( size_t i = 0; i < MAX_PARTY_MEMBERS - 1; i++ )
		{
			User * p = pcUser->psParty->pcaMembers[i];
			if( p )
			{
				if( p->pcUserData->iID == pcUser->pcUserData->iID )
				{
					pcUser->psParty->pcaMembers[i] = pLeader;
					SendChatParty( pcUser, FormatString( "> %s delegated %s as the new Party Leader!", pLeader->pcUserData->sCharacterData.szName, pcUser->pcUserData->sCharacterData.szName ), CHATCOLOR_Error );
					break;
				}
			}
		}

		UpdateParty( pcUser->psParty );
	}
}

void CPartyHandler::ChangePartyMode( PartyInfo * psParty )
{
	if( psParty )
	{
		psParty->iMode = (psParty->iMode + 1) % 3;

		char * pszPartyMode = GetPartyMode( (EPartyMode)psParty->iMode );

		for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
		{
			User * p = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];
			if( p )
				CHATSERVER->SendChatEx( p, CHATCOLOR_Error, "> Party changed to %s!", pszPartyMode );
		}

		UpdateParty( psParty );
	}
}

BOOL CPartyHandler::DoRaid( User * pcUser, User * pcOtherUser )
{
	if( !pcUser || !pcOtherUser )
		return FALSE;

	PartyInfo * psParty1 = pcUser->psParty;
	PartyInfo * psParty2 = pcOtherUser->psParty;

	if( !psParty1 || !psParty2 )
		return FALSE;

	//Update Party 1 Info
	psParty1->iRaidCount = 1;
	psParty1->pcRaidLeader = psParty1;
	psParty1->pcaRaid[0] = psParty2;

	//Update Party 2 Info
	psParty2->iRaidCount = 1;
	psParty2->pcRaidLeader = psParty1;
	psParty2->pcaRaid[0] = psParty2;

	//Update Party Leader from Raid
	UpdateParty( psParty1 );

	return TRUE;
}

BOOL CPartyHandler::JoinRaid( User * pcUser, PartyInfo * psParty )
{
	if( !pcUser || !psParty )
		return FALSE;

	PartyInfo * psPartyLeader = pcUser->psParty;

	if ( !psParty || !psPartyLeader )
		return FALSE;

	if ( psPartyLeader->iRaidCount < (MAX_RAID - 1) )
	{
		for ( int i = 0; i < psPartyLeader->iRaidCount; i++ )
		{
			psPartyLeader->pcaRaid[i]->pcaRaid[psPartyLeader->iRaidCount] = psParty;
			psPartyLeader->pcaRaid[i]->iRaidCount++;

			psParty->pcaRaid[i] = psPartyLeader->pcaRaid[i];
		}

		psPartyLeader->iRaidCount++;
		psParty->iRaidCount = psPartyLeader->iRaidCount;
		psParty->pcRaidLeader = psPartyLeader;
	}
	return TRUE;
}

void CPartyHandler::ArrangePartyMembers( PartyInfo * psParty )
{
	if( psParty )
	{
		User * pcaNewVetor[MAX_PARTY_MEMBERS-1] = { 0 };
		int iIndex = 0;

		for( size_t i = 0; i < MAX_PARTY_MEMBERS-1; i++ )
		{
			User * p = psParty->pcaMembers[i];
			if( p )
			{
				pcaNewVetor[iIndex] = p;
				iIndex++;
			}
		}

		CopyMemory( psParty->pcaMembers, pcaNewVetor, 4 * (MAX_PARTY_MEMBERS-1) );
	}
}

void CPartyHandler::ArrangeRaidParty( PartyInfo * psParty )
{
	if( psParty )
	{
		PartyInfo * pcaNewVetor[MAX_RAID-1] = { 0 };
		int iIndex = 0;

		for( size_t i = 0; i < MAX_RAID-1; i++ )
		{
			PartyInfo * p = psParty->pcaRaid[i];
			if( p )
			{
				pcaNewVetor[iIndex] = p;
				iIndex++;
			}
		}

		CopyMemory( psParty->pcaRaid, pcaNewVetor, 4 * (MAX_RAID-1) );
	}
}

void CPartyHandler::UpdateParty( PartyInfo * psParty, bool bUpdateToRaid )
{
	if( psParty )
	{
		//Prepare Leader Party info to send on packet
		if( psParty->pcLeader )
		{
			PacketUpdateParty sPacket;
			ZeroMemory( &sPacket, sizeof(PacketUpdateParty) );

			sPacket.cMembersCount = (char)psParty->iMembersCount;
			sPacket.cPartyMode = (char)psParty->iMode;

			//Prepare Party Info
			for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
			{
				User * pcMember = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i-1];

				if( pcMember )
				{
					CopyMemory( sPacket.saMembers[i].szCharacterName, pcMember->pcUserData->sCharacterData.szName, 32 );
					sPacket.saMembers[i].sData.iID = pcMember->GetID();
					sPacket.saMembers[i].cCharacterClass = (char)pcMember->pcUserData->sCharacterData.iClass;
				}
			}

			//Prepare Raid Party Info
			if( psParty->iRaidCount > 0 )
			{
				sPacket.cRaidCount = (char)psParty->iRaidCount;

				int iIndex = 0;
				for( size_t i = 0; i < MAX_RAID; i++ )
				{
					PartyInfo * pRaidParty = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i-1];
					if( pRaidParty && (pRaidParty != psParty) )
					{
						sPacket.saRaid[iIndex].cRaidID = (char)(i + 1);

						for( size_t j = 0; j < MAX_PARTY_MEMBERS; j++ )
						{
							User * u = j == 0 ? pRaidParty->pcLeader : pRaidParty->pcaMembers[j - 1];
							if( u )
							{				
								STRINGCOPY( sPacket.saRaid[iIndex].saMembers[j].szCharacterName, u->pcUserData->sCharacterData.szName );
								sPacket.saRaid[iIndex].saMembers[j].sData.iID = u->GetID();
							}
						}

						if( !bUpdateToRaid )
							UpdateParty( pRaidParty, true );

						iIndex++;
					}
				}
			}

			//Process Packets to Update Party Client-Side
			for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
			{
				User * u = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];

				if( u )
				{
					//Party it's over?
					if( psParty->iMembersCount == 0 )
						CHATSERVER->SendChat( u, CHATCOLOR_Error, "> Party has been disbanded." );

					ProcessPacket( u, &sPacket );
				}
			}
		}
	}
}

PartyInfo * CPartyHandler::GetFreeParty()
{
	PartyInfo * psParty = psaParty + 0;

	for ( int i = 0; i < MAX_PARTY; i++ )
	{
		PartyInfo * ps = psaParty + i;
		if ( ps->iMembersCount <= 0 )
		{
			psParty = ps;
			break;
		}
	}

	return psParty;
}
