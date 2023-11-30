#pragma once

#include "party.h"

#define MAX_PARTY			360

class CPartyHandler
{
public:
							CPartyHandler();
	virtual				   ~CPartyHandler();

	void					ProcessPacket( User * pcUser, PacketRequestParty * psPacket );
	void					ProcessPacket( User * pcUser, PacketUpdateParty * psPacket );
	void					ProcessPacket( User * pcUser, PacketUpdatePartyData * psPacket );
	void					ProcessPacket( User * pcUser, PacketRequestRaid * psPacket );

	void					HandlePacket( User * pcUser, PacketRequestParty * psPacket );
	void					HandlePacket( User * pcUser, PacketJoinParty * psPacket );
	void					HandlePacket( User * pcUser, PacketActionParty * psPacket );
	void					HandlePacket( User * pcUser, PacketRequestRaid * psPacket );
	void					HandlePacket( User * pcUser, PacketJoinRaid * psPacket );

	void					HandlePacket( User * pcUser, PacketPartyCore * psPacket );

	void					SendChatParty( User * pcUser, std::string strMessage, EChatColor eChatColor, bool bSendRaid = false );
	void					SendChatRaid( User * pcUser, std::string strMessage, EChatColor eChatColor );
	void					LeaveParty( User * pcUser, bool bKick = false );
	void					UpdatePartyData( User * pcUser, PartyInfo * psParty );

	void					SendActivePartyInfo( User * pcUser, int iMapID = -1 );

	std::vector<User*>		GetPartyMembers( PartyInfo * psParty, bool bRaid = true );
private:

	PartyInfo				* psaParty;

	BOOL					DoParty( User * pcUser, User * pcOtherUser );
	BOOL					JoinParty( User * pcUser, PartyInfo * psParty );
	BOOL					DeleteParty( PartyInfo * psParty );
	void					DelegateParty( User * pcUser );
	void					ChangePartyMode( PartyInfo * psParty );

	BOOL					DoRaid( User * pcUser, User * pcOtherUser );
	BOOL					JoinRaid( User * pcUser, PartyInfo * psParty );

	void					ArrangePartyMembers( PartyInfo * psParty );
	void					ArrangeRaidParty( PartyInfo * psParty );
	void					UpdateParty( PartyInfo * psParty, bool bUpdateToRaid = false );

	PartyInfo				* GetFreeParty();
};