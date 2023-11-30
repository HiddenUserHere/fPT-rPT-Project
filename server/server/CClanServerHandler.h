#pragma once
class CClanServerHandler
{
public:
	CClanServerHandler();
	virtual ~CClanServerHandler();


	void							HandlePacket( User * pcUser, PacketGetClanInfo * psPacket );

private:
	void							ProcessPacket( User * pcUser, PacketClanInfoData * psPacket );

	BOOL							SQLGetClanInfoData( ClanInfoHeader & sHeader );
};

