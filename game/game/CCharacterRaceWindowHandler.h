#pragma once
class CCharacterRaceWindowHandler
{
public:
	CCharacterRaceWindowHandler();
	virtual ~CCharacterRaceWindowHandler();

	void							Init();

	void							Render();

	BOOL							OnMouseClick( CMouse * pcMouse );

	void							OnMouseMove( CMouse * pcMouse );

	void							OnResolutionChanged();

private:

	enum
	{
		WINDOWID_Main,

		BUTTONID_Morion,
		BUTTONID_Tempskron,
	};

	UI::Window_ptr					pWindow;

	void							OnTempskronButtonClick( UIEventArgs eArgs );
	void							OnMorionButtonClick( UIEventArgs eArgs );
};

