#pragma once
class CDamageHandler
{
public:
	CDamageHandler();
	virtual ~CDamageHandler();


	void												HandlePacket( struct PacketDamageQuick * psPacket );
};

