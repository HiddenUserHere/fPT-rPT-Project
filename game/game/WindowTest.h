#pragma once
class CWindowTest
{
public:
	CWindowTest();
	virtual ~CWindowTest();

	void							Init();

	BOOL							OnMouseClick( class CMouse * pcMouse );
	void							OnAllStatsClick( UIEventArgs e );
	void							OnAllSkillsClick( UIEventArgs e );
	void							OnResetButtonClick( UIEventArgs e );
	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	void							Render();

	void							Update( float fTime );

	void							Open();
	void							Close( UIEventArgs e );
	void							OnDOMReady( UIEventArgs e );

	void							Clear();

	BOOL							IsOpen() { return pWindow->IsOpen(); }

	enum
	{
		CHECKBOXID_Strenght,
		CHECKBOXID_Spirit,
		CHECKBOXID_Talent,
		CHECKBOXID_Agility,
		CHECKBOXID_Health,
		WEBVIEW_Test,
	};

private:
	UI::Window_ptr pWindow = NULL;
};

