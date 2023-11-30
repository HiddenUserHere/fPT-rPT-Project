#pragma once
#include "GrandFuryWindow.h"

struct StructFuryArenaBoss;

class CGrandFury
{
private:
	DWORD												dwTimeCountDown = 0;

	DWORD												dwTimeArena = 0;

	BOOL												bStarted = FALSE;

	int													iEventID = 0;

	CGrandFuryWindow									* pcGrandFuryWindow = NULL;

	UI::Image											* pcTimeBG;
	UI::Image											* pcHPBackground;
	UI::Image											* pcHPFiller;

	UI::Text_ptr										pTextTime, pTextBossHP;

	int													iCurrentHP;
	int													iMaxHP;

	

public:
	CGrandFury();
	virtual ~CGrandFury();

	void												Init();

	BOOL												InsideArena( int iX, int iZ );

	CGrandFuryWindow									* GetWindow() { return pcGrandFuryWindow; }

	int													GetCurrentHP() { return iCurrentHP; }
	int													GetMaxHP() { return iMaxHP; }

	DWORD												GetTimeLeft() { return dwTimeCountDown; }

	BOOL												IsStarted() { return bStarted; }

	void												HandlePacket( struct PacketFuryArenaRequest * psPacket );
	void												HandlePacket( struct PacketFuryArenaItemData * psPacket );
	void												HandlePacket( struct PacketFuryArenaBossData * psPacket );

	int													GetEventID() { return iEventID; }


	void												Update();
	void												Render();
};

