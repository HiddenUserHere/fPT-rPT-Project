#pragma once
class CEasterEventHandler
{
public:
	CEasterEventHandler();
	virtual ~CEasterEventHandler();


	void									HandlePacket( struct PacketNPCEasterEgg * psPacket );

	void									OnClickOKNPC();

protected:

	void									OnOKBoxClick(UIEventArgs e);
};

