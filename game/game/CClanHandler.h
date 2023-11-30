#pragma once

#define MAX_CLANINFO			500
#define MAX_TIME_CLANUPDATE		10000

class CClanHandler
{
public:
	CClanHandler();
	~CClanHandler();

	void					Init();

	void					Update( float fTime );

	int						GetNewClanInfoIndex( int iClanID );

	ClanInfo				* GetClanInfo() { return (ClanInfo*)0x03AA4608; }

private:

	void					UpdateGetClanInfo();

	void					ProcessPacket( PacketGetClanInfo * psPacket );

};

