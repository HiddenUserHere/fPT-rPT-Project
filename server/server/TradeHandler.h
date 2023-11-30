#pragma once
class CTradeHandler
{
public:
	CTradeHandler();
	virtual ~CTradeHandler();

	struct TradeDataItem
	{
		ItemData					cItemData[20];
		
		int							iGold;
		int							iWeight;	//Maybe not used... need to be check

		int							iFlag;      //???

		int							iGoldCheck; //??? Maybe not used too...
	};

	void							HandlePacket( User * pcUser, PacketTradeData * psPacket );
	void							HandlePacket( User * pcUser, PacketTradeRequest * psPacket );
	void							HandlePacket( User * pcUser, PacketTradeSuccess * psPacket );

	void							HandleTradeCoin( User * pcUser, User * pcPartner );

	void							Tick();

private:

	BOOL							IsInTrade( User * pcUser, int iItemID, DWORD dwHead, DWORD dwCheckSum );



	WORD							wDay = 0;
	WORD							wMonth = 0;
};

