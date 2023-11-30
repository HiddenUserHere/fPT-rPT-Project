#pragma once
class CBellatraReward
{
private:
	UI::Window_ptr					pWindow = NULL;

	enum
	{
		WINDOW_Main,
		LIST_Main,

		TEXTWINDOW_Main,
	};

	void							OnButtonOKClick( UIEventArgs e );
	void							OnButtonCloseClick( UIEventArgs e );

public:
	CBellatraReward();
	virtual ~CBellatraReward();

	void							BuildWindow();

	void							Init();

	void							OnResolutionChanged();

	void							Render();

	void							Open();

	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseClick( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );
};

