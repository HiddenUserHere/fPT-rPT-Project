#pragma once

enum EKeyboardEvent
{
	NoneKey,
	KeyDown,
	KeyUp,
	KeyChar,
};

class CKeyboard
{
private:
	EKeyboardEvent								eEvent;
	wchar_t										cKeyChar = 0;
	int											iKeyPress = 0;
	void										SetEvent( EKeyboardEvent eKeyEvent );
	BOOL										baKeyDown[ 256 ];

    LPARAM                                      lParam;
public:
	CKeyboard();
	virtual ~CKeyboard();

	EKeyboardEvent								GetEvent() { return eEvent; };
	char										GetChar() { return (char)cKeyChar; };
	wchar_t										GetWChar() { return cKeyChar; };
	int											GetKey() { return iKeyPress; };
	BOOL										IsKeyDown( int iKey );

	BOOL										OnKeyPress( int iKey, BOOL bKeyDown );
	BOOL										OnKeyChar( wchar_t cKey );
	BOOL										OnKeyChar( char cKey ) { return OnKeyChar( (wchar_t)cKey ); };

    LPARAM                                      GetLParam() { return lParam; }
    void                                        SetLParam( LPARAM _lParam ) { lParam = _lParam; }
};

