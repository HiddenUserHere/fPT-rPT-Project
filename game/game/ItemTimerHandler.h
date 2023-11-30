#pragma once

#define DEFAULT_ITEMS_PER_COLUMN	5

#define MAX_ITEMTIMER		20

#define ITEMTIMER_WIDTH		38
#define ITEMTIMER_HEIGHT	38

#define POS_ITMTMRBASEX				10
#define POS_ITMTMRBASEY				165
#define TMR_SPACEX					40
#define TMR_SPACEY					40

#define NUM_ITEMTIMERTYPE			59

#define ITEM_TIMER(x)				(ITEMTIMERHANDLER->GetItemTimer(x))

class ItemTimer
{
public:
	EItemTimerType		iType;
	EItemID				iItemID;
	char				szName[32];
	char				szDescription[64];
	DWORD				dwTimeLeft;
	DWORD				dwTimeTotal;
	UI::Image			* pcImage;
	UI::Image			* pcImageBackground;

	std::vector<int>	vFreezeMaps;

	BOOL				bView;
	BOOL				bFreeze;
	BOOL				bAlive;
	int					iFade;
	Rectangle2D			sPosition;
	Rectangle2D			sMoveTo;
	int					iColumn;

	static UI::Image	* pcImageBox;

public:
	ItemTimer( EItemTimerType iType, EItemID iItemID, char * pszName, char * pszDescription, DWORD dwTimeLeft, DWORD dwTimeTotal, UI::Image * pcImage, UI::Image * pcImageBackground );
	virtual ~ItemTimer();

	void				RenderTimer();
	void				RenderBox();
	void				Loop();
	void				Tick();
	void				OnMouseMove( Point2D * psPosition );

	BOOL				IsAlive() { return bAlive; }
	void				Die() { bAlive = FALSE; }

	void				FreezeMap( int iMapID ) { vFreezeMaps.push_back( iMapID ); }
	std::vector<int>	GetFreezeMaps() { return vFreezeMaps; }
	void				Freeze( int iMapID );

	BOOL				Moving();

	BOOL				IsViewing() { return bView; }
};


class CItemTimerHandler
{
private:
	ItemTimer			* pcaItemTimer[MAX_ITEMTIMER];
	ItemTimer			* pcaItemTimerPrefab[NUM_ITEMTIMERTYPE];

	int					GetFreeSlot();

	BOOL				CanStack( int iType );

	int					GetIndex( ItemTimer * pcTimer );

	BOOL				IsHead( int iType );

	void				ProcessCancel( ItemTimer * pcTimer );

	void				OnClickCancelOK( ItemTimer * pcTimer, UIEventArgs eArgs );
	void				OnClickBeginnerRequest( UIEventArgs eArgs );
	void				OnClickChemistBuffRequest( UIEventArgs eArgs );

public:
	CItemTimerHandler();
	virtual ~CItemTimerHandler();

	void				Init();

	void				Render();
	void				Loop();
	void				Tick();
	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );

	ItemTimer			* GetPrefab( EItemTimerType iType );

	ItemTimer			* GetItemTimer( int iType );

	ItemTimer			* GetHead();

	void				EventTimerStart( ItemTimer * pcTimer, bool bFirstTime );
	void				EventTimerStop( ItemTimer * pcTimer );

	void				RemoveTimer( int iIndex );

	void				OnResolutionChanged();

	BOOL				AddTimer( EItemTimerType iType, EItemID iItemID, DWORD dwTimeTotal, DWORD dwTimeLeft, bool bFirstTime = false );

	//Handlers
	void				HandlePacket( SocketData * pcSocketData, PacketNewItemTimer * psPacket );
	void				HandlePacket( PacketItemPremiumData * psPacket );
	void				HandlePacket( PacketOldHeadUse * psPacket );

	void				HandlePacket( PacketRequestBeginnerItemsTimer * psPacket );
	void				HandlePacket( PacketRequestChemistItemTimer * psPacket );

	//Packet Process
	void				ProcessPacket( PacketOldHeadUse * psPacket );
	void				ProcessPacket( PacketNewItemTimer * psPacket );
	void				ProcessPacket( PacketCancelItemTimer * psPacket );

	void				UpdateOldHead();

};

