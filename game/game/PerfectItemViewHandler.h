#pragma once

class ItemData;

class CPerfectItemViewHandler
{
protected:
	class ItemData										* pcItemData = NULL;
	BOOL												bVisible = FALSE;
	BOOL												bReceivedItem = FALSE;
public:
	CPerfectItemViewHandler();
	~CPerfectItemViewHandler();

	BOOL												IsSet() { return bVisible && bReceivedItem; };

	void												PHItemData( struct PacketItemPerfectView * psPacket );

	BOOL												OnKeyPress( class CKeyboard * pcKeyBoard );

	void												Render();

private:
	void												ProcessPacket( int iItemID, int iAgeLevel, int iMixID );

};

