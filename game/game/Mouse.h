#pragma once

enum EMouseEvent
{
	None,
	ClickDownL,
	ClickUpL,
	ClickDownR,
	ClickUpR,
	DoubleClickL,
	DoubleClickR,
	ScrollUp,
	ScrollDown,
};

struct CursorData
{
	int					iID;

	HCURSOR				hCursor;

	CursorData( int i, HCURSOR h ) { iID = i; hCursor = h; };

	~CursorData() {};
};

class CMouse
{
private:
	Point2D									cPosition;
	BOOL									bDisable = FALSE;
	BOOL									bNoMove = FALSE;
	BOOL									bEndNoMove = FALSE;
	EMouseEvent								eEventMouse = (EMouseEvent)-1;
	short									sDistance = 0;

	IMPFNC									pfnAdjustMouseToResolution = 0x00409CC0;

	std::vector<CursorData*>				vCursor;
	int										iLastCursorID = 0;
	int										iBlockedRefCount = 0;


public:
	CMouse();
	virtual ~CMouse();
	EMouseEvent								GetEvent( DWORD dwKey );
	EMouseEvent								GetEvent() { return eEventMouse; };
	Point2D									* GetPosition(){return &cPosition;};
	short									GetDistance() { return sDistance; }
	BOOL									IsDisabled() { return bDisable; };
	BOOL									OnMouseClickHandler( DWORD dwKey );
	void									OnMouseMoveHandler( int iX, int iY );
	BOOL									OnMouseScrollHandler( short sDistance );

	void									IsNoMove( BOOL b );
	BOOL									IsNoMove() { return bNoMove; }
	BOOL									IsEndNoMove() { return bEndNoMove; }
	void									IsEndNoMove( BOOL b ) { bEndNoMove = b; }

	void									BlockMouse();
	void									UnblockMouse();

	void									FixedToRealPosition();

	static void								AdjustMouseToResolution( Point2D * psPosition, BOOL bAdjust );

	void									SetCursorGame( int iID );

};

