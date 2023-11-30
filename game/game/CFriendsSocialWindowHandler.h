#pragma once


class CFriendsSocialWindowHandler
{
private:
	friend class CSocialWindowHandler;

	enum EWindowFriendsID : int
	{
		WINDOWID_Recents,
		WINDOWID_Friends,
		WINDOWID_Ignored,
	};


public:
	CFriendsSocialWindowHandler();
	~CFriendsSocialWindowHandler();

	void					Init();

	void					Update( float fTime );

	UI::Window_ptr			GetWindow() { return pWindow; }

	void					Open( EWindowFriendsID iWindowID );

	void					Close();

	void					AddRecent( const std::string strName );
	void					AddFriend( const std::string strName );
	void					AddIgnored( const std::string strName );

	bool					IsBlocked( const std::string strName );

	std::vector<std::string> GetFriendNames();
	std::vector<std::string> GetBlockedNames();

protected:
	enum : int
	{
		WINDOW_Main,

		LIST_Recent,
		LIST_Friends,
		LIST_Ignored,

		GROUP_Name,

		GROUP_TabBottom,

		BUTTON_AddFriend,
		BUTTON_Recent,
		BUTTON_Blocked,
		BUTTON_Favorite,

		TEXT_Name,

		WINDOW_NameList = 5000,
	};

	void					OnButtonBlockClick( UI::Text_ptr pText, UIEventArgs eArgs );
	void					OnButtonRemoveClick( UI::Text_ptr pText, UIEventArgs eArgs );

	void					OnButtonAddFriendClick( UIEventArgs eArgs );
	void					OnButtonRecentClick( UIEventArgs eArgs );
	void					OnButtonBlockedClick( UIEventArgs eArgs );
	void					OnButtonFavoriteClick( UIEventArgs eArgs );

	void					OnNameClick( UI::Text_ptr pText, int iListID, UIEventArgs eArgs );
	void					OnNameButtonDoubleClick( UI::Text_ptr pText, int iListID, UIEventArgs eArgs );

	void					Add( int iListID, const std::string strName );
	void					Remove( const std::string strName, bool bRemoveRecents, bool bRemoveFriends, bool bRemoveIgnored );

	UI::Window_ptr			pWindow;

	UINT					uNameWheel = 0;

	std::string				strLastName;

};

