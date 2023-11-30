#pragma once
#include "Screen.h"

class CWorldScreen : public CScreen
{
public:
						CWorldScreen();
					   ~CWorldScreen();

	BOOL				Init();
	void				Shutdown();

	void				Render3D();
	void				Render2D();

	EScreen				Frame();
	void				Update( float fTime );

	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );
	BOOL				OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL				OnMouseScroll( class CMouse * pcMouse );
	BOOL				OnKeyChar( class CKeyboard * pcKeyboard );

	void				OnResolutionChanged();

	static void			GetPlayerUnits();

    static void			LoadUnitsTest( int iCount );

	static void			RenderTestUnits();

	static void			UpdateTestUnits();

private:
	bool				bHUDInitialized = false;

	static BOOL			bLoadingTime;

	static DWORD WINAPI		ThreadLoader();
};

