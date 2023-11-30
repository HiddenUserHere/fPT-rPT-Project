#pragma once
class CPersonalShopHandler
{
public:
	CPersonalShopHandler();
	virtual ~CPersonalShopHandler();


	void								HandlePacket( User * pcUser, PacketChatBoxMessage * psPacket );
	void								HandlePacket( User * pcUser, PacketPersonalShopItemList * psPacket );
	void								HandlePacket( User * pcUser, PacketPersonalShopTradeItems * psPacket );
	void								HandlePacket( User * pcUser, PacketPersonalShopRequestView * psPacket );

private:
	void								ProcessPacket( User * pcUser, PacketPersonalShopItemList * psPacket );
	void								ProcessPacket( User * pcUser, PacketPersonalShopTradeItems * psPacket );
	void								ProcessPacket( User * pcUser, PacketPersonalShopRequestView * psPacket );
};

