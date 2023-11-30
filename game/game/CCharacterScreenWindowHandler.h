#pragma once

class CCharacterScreenWindowHandler
{
public:
	CCharacterScreenWindowHandler();
	virtual ~CCharacterScreenWindowHandler();

	void							Init();

	void							Render();

	BOOL							OnMouseClick( CMouse * pcMouse );

	void							OnMouseMove( CMouse * pcMouse );

	void							OnResolutionChanged();

	void							UpdateCharacterInfo( const CharacterData & sCharacterData );

private:

	enum
	{
		WINDOWID_Main,
		WINDOWID_Bottom,
		WINDOWID_CharacterInfo,
		BUTTONID_NewCharacter,
		BUTTONID_Ok,
		BUTTONID_Back,
		BUTTONID_Delete,

		RECTBOXID_Bottom,

		TEXTID_CharacterInfoLeft = 300,
		TEXTID_CharacterInfoRight = 400,
	};

	struct CharacterInfoText
	{
		std::string strText;

		DWORD		dwColor;

		BOOL		bBold;
	};


	UI::Window_ptr					pWindow = nullptr;

	std::vector<CharacterInfoText>	vTextLeft;
	std::vector<CharacterInfoText>	vTextRight;

	void							ClearText();
	void							AddString( const std::string strText, BOOL bBold, DX::Color cColor, bool bLeft );

	void							BuildCharacterText();

	void							OnButtonNewCharacterClick( UIEventArgs eArgs );

	void							OnButtonOkClick( UIEventArgs eArgs );
	void							OnButtonBackClick( UIEventArgs eArgs );

	void							UpdateWindowPosition();

	class CCharacterScreen			* GetCharacterScreen();
};

