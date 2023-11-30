#pragma once
class CTestEditor
{
private:
	UI::Window_ptr					pWindow;

public:
	CTestEditor();
	virtual ~CTestEditor();

	void							BuildWindowAnimator();

	void							BuildWindow();

	void							Init();

	void							Render();


	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseClick( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	BOOL							OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL							OnKeyChar( class CKeyboard * pcKeyboard );

};

