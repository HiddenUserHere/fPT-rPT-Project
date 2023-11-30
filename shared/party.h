#pragma once

class User;

#define MAX_PARTY_MEMBERS		6
#define MAX_RAID				2

enum ERaidState
{
	RAIDSTATE_Failed			= -1,

	RAIDSTATE_None,
	RAIDSTATE_Success,
	RAIDSTATE_FullRaid,
};

enum EPartyState
{
	PARTYSTATE_Failed			= -1,
	
	PARTYSTATE_None,
	PARTYSTATE_Success,
	PARTYSTATE_AlreadyParty,
	PARTYSTATE_FullParty,
	PARTYSTATE_RequestInvite,
};

enum EPartyAction
{
	PARTYACTION_None,

	PARTYACTION_Leave,
	PARTYACTION_Kick,
	PARTYACTION_Delegate,
	PARTYACTION_Disband,
	PARTYACTION_ChangeMode,
};

enum EPartyMode
{
	PARTYMODE_Normal,
	PARTYMODE_EXP,
	PARTYMODE_Hunt,
};

struct PartyInfo
{
	int							iID;
	int							iMembersCount;
	int							iRaidCount;
	int							iMode;
	User					  * pcLeader;
	PartyInfo				  * pcRaidLeader;
	User					  * pcaMembers[MAX_PARTY_MEMBERS-1];
	PartyInfo				  * pcaRaid[MAX_RAID-1];
};


struct PartyMemberData
{
	int							iID;

	short						sLevel;
	CurMax						sHP;
	CurMax						sMP;
	Point3D						sPosition;
};

struct PartyRaidMemberData
{
	int							iID;

	CurMax						sHP;
	Point3D						sPosition;
};

struct PartyMember
{
	char						szCharacterName[32];
	char						cCharacterClass;

	int							iClanID;

	PartyMemberData				sData;
};

struct PartyRaidMember
{
	char						szCharacterName[32];

	PartyRaidMemberData			sData;
};

struct PartyRaid
{
	char						cRaidID;

	PartyRaidMember				saMembers[MAX_PARTY_MEMBERS];
};

struct PacketUpdateParty : Packet
{
	char						cMembersCount;
	char						cPartyMode;
	PartyMember					saMembers[MAX_PARTY_MEMBERS];

	char						cRaidCount;
	PartyRaid					saRaid[MAX_RAID-1];

								PacketUpdateParty(){ cMembersCount = 0; cRaidCount = 0; }
};

struct PacketUpdatePartyData : Packet
{
	char						cMembersCount;
	char						cRaidMembersCount;

	PartyMemberData				saMembers[MAX_PARTY_MEMBERS];
	PartyRaidMemberData			saRaidMember[MAX_RAID-1][MAX_PARTY_MEMBERS];
};

struct PacketRequestParty : Packet
{
								PacketRequestParty() = default;
								PacketRequestParty( int _iTargetID ){ iID = 0; iTargetID = _iTargetID; eState = PARTYSTATE_None; }

	int							iID;
	int							iTargetID;
	EPartyState					eState;
	char						szCharacterName[32];
};

struct PacketRequestRaid : Packet
{
								PacketRequestRaid() = default;
								PacketRequestRaid( int _iTargetID ){ iID = 0; iTargetID = _iTargetID; eState = RAIDSTATE_None; }

	int							iID;
	int							iTargetID;
	ERaidState					eState;
	char						szCharacterName[32];
};

struct PacketJoinRaid : Packet
{
	int							iID;
};

struct PacketJoinParty : Packet
{
	int							iID;
};

struct PacketActionParty : Packet
{
	EPartyAction				eAction;
	int							iUserID;

								PacketActionParty() = default;
								PacketActionParty( EPartyAction _eAction, int _iUserID = -1 ){ eAction = _eAction; iUserID = _iUserID; }
};

struct PacketPartyCore : Packet
{
	int							iID;

	Item						sItem;
};

struct PacketSkillPartyData : Packet
{
	int										iSkillID;
	int										iLevel;
	int										iParam1;
	int										iParam2;
	int										iParam3;
	int										iClass;
	int										iCount;
	int										iaID[(MAX_PARTY_MEMBERS * MAX_RAID) + 1];
};


class CPartyData
{
public:
	int							iMembersCount;
	EPartyMode					eMode;
	PartyMember					saMembers[MAX_PARTY_MEMBERS];

	int							iRaidCount;
	PartyRaid					saRaid[MAX_RAID-1];
};

char * GetPartyMode( EPartyMode eMode );