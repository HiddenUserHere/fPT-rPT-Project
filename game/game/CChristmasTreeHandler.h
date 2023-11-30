#pragma once

#include "CChristmasTreeWindowHandler.h"
#include "CChristmasGiftWindowHandler.h"

class CChristmasTreeHandler
{
private:
	CChristmasTreeWindowHandler		* pcWindow;
	CChristmasGiftWindowHandler		* pcWindowGift;
	XmasTreeBase					* pcXmasTreeBase;

	int								iCharacterID = 0;

public:
	CChristmasTreeHandler();
	virtual ~CChristmasTreeHandler();


	CChristmasTreeWindowHandler		* GetWindow() { return pcWindow; }
	CChristmasGiftWindowHandler		* GetWindowGift() { return pcWindowGift; }

	void							Init();

	void							Update( float fTime );

	void							OpenXmasTreeNPC( const char * pszCharacterName, int iUnitID );
	void							OpenXmasTreeGiftNPC();

	void							HandlePacket( PacketXmasTreeOpen * psPacket );
	void							HandlePacket( PacketXmasTreeItemNPC * psPacket );
	void							HandlePacket( PacketXmasTreeResponse * psPacket );

	void							SendItems( std::vector<ItemData*> vItems );

	void							ProcessPacket( PacketXmasTreeItemUse * psPacket );

	bool							IsNearNPC( Point3D sPosition );

private:
	void							ProcessPacket( PacketXmasTree * psPacket );
	void							ProcessPacket( PacketXmasTreeItemNPC * psPacket );

	friend class CChristmasGiftWindowHandler;
};

