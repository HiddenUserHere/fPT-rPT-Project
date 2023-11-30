#include "stdafx.h"
#include "CBrowserWindowHandler.h"

#ifdef CEF_UI_BROWSER

CBrowserWindowHandler::CBrowserWindowHandler( int iWidth, int iHeight )
{
	pcBrowserClient = new CBrowserClient( new CBrowserRendererHandler( iWidth, iHeight ) );
}


CBrowserWindowHandler::~CBrowserWindowHandler()
{
	DELET( pcBrowserClient );
}

void CBrowserWindowHandler::Init()
{
	sWindowInfo.SetAsWindowless( GAMEWINDOW->GetWindowHandle() );
	pcBrowser = CefBrowserHost::CreateBrowserSync( sWindowInfo, pcBrowserClient.get(), "https://www.google.com/search?newwindow=1&q=o", sSettings, nullptr );

	pcBrowserClient->GetWindowRenderer()->Init();
}

void CBrowserWindowHandler::Shutdown()
{
	pcBrowser = nullptr;

	pcBrowserClient->GetWindowRenderer()->Shutdown();
}

void CBrowserWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	CefMouseEvent mouse_event;
	mouse_event.x = pcMouse->GetPosition()->iX;
	mouse_event.y = pcMouse->GetPosition()->iY;
	mouse_event.modifiers = 0;

	pcBrowser->GetHost()->SendMouseMoveEvent( mouse_event, false );
}

BOOL CBrowserWindowHandler::OnMouseClick( CMouse * pcMouse )
{
	CefMouseEvent mouse_event;
	mouse_event.x = pcMouse->GetPosition()->iX;
	mouse_event.y = pcMouse->GetPosition()->iY;

	CefBrowserHost::MouseButtonType iEvent = ((pcMouse->GetEvent() == EMouseEvent::ClickDownL) || (pcMouse->GetEvent() == EMouseEvent::ClickUpL)) ? MBT_LEFT : MBT_RIGHT;

	bool bButtonUP = ((pcMouse->GetEvent() == EMouseEvent::ClickUpL) || (pcMouse->GetEvent() == EMouseEvent::ClickUpR)) ? true : false;

	pcBrowser->GetHost()->SendMouseClickEvent( mouse_event, iEvent, bButtonUP, 1 );

	return FALSE;
}

#endif