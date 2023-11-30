#pragma once


struct SShopDataHandler
{
	BOOL										bValid;
	DWORD										iID;
	DWORD										dwTime;
	int											iPositionItemHandL;
	int											iPositionItemHandR;
	int											iPositionItemHandB;
	int											iX;
	int											iY;
	int											iZ;
	char										szShopText[256];
	CharacterData								sCharacterData;

	ItemData									cItemData[ITEM_PERSONALSHOP_MAX];
};

#define MAX_BOTS 300

class CBotShopServerHandler
{
private:
	struct SShopDataHandler					* psaShopData = NULL;

	int										iBotCount = 0;

public:
	CBotShopServerHandler();
	virtual ~CBotShopServerHandler();

	void									UpdateUnitStatus( UserData * pcUserData );

	BOOL									AddBot( User * pcUser, PacketPersonalBotShopData * psPacket );

	void									Init();
};

