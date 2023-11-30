#pragma once
class CCharacterStatusWindowHandler
{
public:
	CCharacterStatusWindowHandler();
	~CCharacterStatusWindowHandler();

	void						Init();

	void						RenderText();

	DX::Font_ptr				pFont = nullptr;

private:
	void						DrawTextCharacter( const std::string & strText, Rectangle2D & rRect, const DX::Color & cColor );
};

