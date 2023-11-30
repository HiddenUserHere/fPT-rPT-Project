#pragma once
class CPartyCoreHandler
{
public:
	CPartyCoreHandler();
	~CPartyCoreHandler();

	void						Init();

	BOOL						IsOpen();

	void						Open( ItemData * pcItemData );
	void						Close();

	void						Clear();

	void						Render();

	void						Update( float fTime );

	void						OnMouseMove( CMouse * pcMouse );

	BOOL						OnMouseClick( CMouse * pcMouse );

	void						OnResolutionChanged();

private:
	UI::Window_ptr				pWindow = nullptr;

	Item						sItem;

	enum
	{
		WINDOW_Main,
		DROPDOWNMENU_Main,
	};

	void						OnChooseMemberParty();

	void						ProcessPacket( PacketPartyCore * psPacket );
};

