#pragma once
class CSocketSystemHandler
{
public:
	CSocketSystemHandler();
	virtual ~CSocketSystemHandler();


	void									HandlePacket( User * pcUser, PacketSocketSystem * psPacket );

	void									OnMixingItem( User * pcUser, struct PacketMixServer * psPacket );

private:
	void									OnSocketDrill( User * pcUser, PacketSocketSystem * psPacket );
	void									OnSocketStone( User * pcUser, PacketSocketSystem * psPacket );


};

