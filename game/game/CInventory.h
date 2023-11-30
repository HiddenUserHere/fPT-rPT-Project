#pragma once

class CInventory
{
public:
	//Constructor
							CInventory();
						   ~CInventory();

	void					Init();
	void					Render();
	void					Update( float fTime );

	void					OnMouseMove( class CMouse * pcMouse );
	BOOL					OnMouseClick( class CMouse * pcMouse );
	BOOL					OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL					OnKeyChar( class CKeyboard * pcKeyboard );
protected:
	UI::Window_ptr			pWindow1;
private:
	void					BuildWindow1();
};
