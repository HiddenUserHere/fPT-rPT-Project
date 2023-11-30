#include "stdafx.h"
#include "Mouse.h"

BOOL bMouseCreatedHandler = FALSE;


CMouse::CMouse()
{
	cPosition.iX = 0;
	cPosition.iY = 0;
	bMouseCreatedHandler = TRUE;

	vCursor.push_back( new CursorData( EMOUSEICON_Default, LoadCursorFromFileA( "game\\images\\mouse\\default.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_Sell, LoadCursorFromFileA( "game\\images\\mouse\\sell.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_Buy, LoadCursorFromFileA( "game\\images\\mouse\\buy.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_Repair, LoadCursorFromFileA( "game\\images\\mouse\\repair.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_Attack, LoadCursorFromFileA( "game\\images\\mouse\\attack.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_GetItem1, LoadCursorFromFileA( "game\\images\\mouse\\dropsee.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_GetItem2, LoadCursorFromFileA( "game\\images\\mouse\\droppick.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_Talk, LoadCursorFromFileA( "game\\images\\mouse\\talk.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_GlamorousBrush1, LoadCursorFromFileA( "game\\images\\mouse\\repair.cur" ) ) );
	vCursor.push_back( new CursorData( EMOUSEICON_GlamorousBrush2, LoadCursorFromFileA( "game\\images\\mouse\\repair.cur" ) ) );
}


CMouse::~CMouse()
{
}

EMouseEvent	CMouse::GetEvent( DWORD dwKey )
{
	switch ( dwKey )
	{
		case WM_LBUTTONDOWN:
			return EMouseEvent::ClickDownL;
		case WM_LBUTTONUP:
			return EMouseEvent::ClickUpL;
		case WM_RBUTTONDOWN:
			return EMouseEvent::ClickDownR;
		case WM_RBUTTONUP:
			return EMouseEvent::ClickUpR;
		case WM_LBUTTONDBLCLK:
			return EMouseEvent::DoubleClickL;
		case WM_RBUTTONDBLCLK:
			return EMouseEvent::DoubleClickR;
	}
	return (EMouseEvent)-1;
}

void CMouse::OnMouseMoveHandler( int iX, int iY )
{
	cPosition.iX = iX;
	cPosition.iY = iY;

	if ( GAME )
		GAME->OnMouseMove( this );
}


BOOL CMouse::OnMouseClickHandler( DWORD dwKey )
{
	if ( GameCore::GetInstance()->IsInit() )
	{
		if ( IsEndNoMove() )
		{
			MOUSEHANDLER->IsEndNoMove( FALSE );
			MOUSEHANDLER->IsNoMove( FALSE );
		}

		eEventMouse = GetEvent( dwKey );

		if ( GAME->OnMouseClick( this ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CMouse::OnMouseScrollHandler( short sDistance )
{
	if ( GameCore::GetInstance()->IsInit() )
	{
		this->sDistance = sDistance / WHEEL_DELTA;

		if ( sDistance < 0 )
			eEventMouse = ScrollUp;
		else
			eEventMouse = ScrollDown;

		if ( GAME->OnMouseScroll( this ) )
			return TRUE;
	}
	return FALSE;
}

void CMouse::IsNoMove( BOOL b )
{
	bNoMove = b;

	if ( bNoMove == TRUE )
		bEndNoMove = FALSE;

	Game::SetMouseBlocked( b );
}

void CMouse::BlockMouse()
{
	iBlockedRefCount++;

	IsNoMove( TRUE );

	UNITDATA->bWalking = FALSE;
}

void CMouse::UnblockMouse()
{
	if ( iBlockedRefCount > 0 )
	{
		iBlockedRefCount--;
		if ( iBlockedRefCount == 0 )
			IsEndNoMove( TRUE );
	}
}

void CMouse::FixedToRealPosition()
{
	WRITEDWORD( 0x00A16194, READDWORD( 0x00A17518 ) );
	WRITEDWORD( 0x00A16198, READDWORD( 0x00A1751C ) );
}

NAKED void CMouse::AdjustMouseToResolution( Point2D * psPosition, BOOL bAdjust )
{
	JMP( pfnAdjustMouseToResolution );
}

void CMouse::SetCursorGame( int iID )
{
	if ( iID == EMOUSEICON_RepairAll )
		iID = EMOUSEICON_Repair;

	for ( std::vector<CursorData*>::iterator it = vCursor.begin(); it != vCursor.end(); it++ )
	{
		CursorData * pc = (*it);

		if ( pc->iID == iID )
			SetCursor( pc->hCursor );
	}

	iLastCursorID = iID;
}
