#pragma once

enum EScreen
{
	SCREEN_None			= 0,
	SCREEN_Login		= 1,
	SCREEN_Character	= 2,
	SCREEN_World		= 3,
	SCREEN_Test			= 4,
};



class CScreen
{
public:
	CScreen();
	virtual ~CScreen();

	virtual BOOL				Init()		= 0;
	virtual void				Shutdown()	= 0;

	virtual void				Render3D()	= 0;
	virtual void				Render2D()	= 0;

	virtual EScreen				Frame()		= 0;

	virtual void				Update( float fTime ){}

	virtual void				OnMouseMove( class CMouse * pcMouse )		= 0;
	virtual BOOL				OnMouseClick( class CMouse * pcMouse )		= 0;
	virtual BOOL				OnKeyPress( class CKeyboard * pcKeyboard )  {return FALSE;}
	virtual BOOL				OnMouseScroll( class CMouse * pcMouse ){ return FALSE; }
	virtual BOOL				OnKeyChar( class CKeyboard * pcKeyboard ){ return FALSE; }

	virtual void				OnResolutionChanged(){}

	DWORD						GetThis() { return ((DWORD)this) + 4; }
};

