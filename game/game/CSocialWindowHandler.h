#pragma once

#include "CClanSocialWindowHandler.h"
#include "CFriendsSocialWindowHandler.h"

class CSocialWindowHandler
{
public:
	CSocialWindowHandler();
	virtual ~CSocialWindowHandler();

	void						Init();

	void						Update( float fTime );

	void						Render();

	void						OnMouseMove( CMouse * pcMouse );
	BOOL						OnMouseClick( CMouse * pcMouse );

	BOOL						OnMouseScroll( CMouse * pcMouse );

	void						OnResolutionChanged();

	BOOL						IsOpen() { return pWindow->IsOpen(); };
	void						Open();
	void						Close();

	CFriendsSocialWindowHandler * GetFriends() { return pcFriendsWindow; }
	CClanSocialWindowHandler	* GetClan() { return pcClanWindow; }

	void						LoadData();
	void						SaveData();

	void						UpdateObjectSettings();

private:

	enum
	{
		WINDOW_Main,

		BUTTON_FriendsTab,
		BUTTON_ClanTab,


	};

	bool						bLoaded = false;

	UI::Window_ptr				pWindow;

	void						BuildWindow();

	void						BuildFriendsWindow();

	void						OnButtonFriendsClick( UIEventArgs eArgs );
	void						OnButtonClanClick( UIEventArgs eArgs );
	void						OnButtonCloseClick( UIEventArgs eArgs );

	CFriendsSocialWindowHandler	* pcFriendsWindow	= nullptr;
	CClanSocialWindowHandler	* pcClanWindow		= nullptr;
};

