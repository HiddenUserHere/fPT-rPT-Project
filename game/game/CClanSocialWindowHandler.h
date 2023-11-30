#pragma once

class CClanSocialWindowHandler
{
private:
	enum EClanMemberType
	{
		CLANMEMBERTYPE_Member,
		CLANMEMBERTYPE_Leader,
		CLANMEMBERTYPE_ViceLeader,
	};

public:
	CClanSocialWindowHandler();
	virtual ~CClanSocialWindowHandler();

	void					Init();

	void					Update();

	UI::Window_ptr			GetWindow() { return pWindow; }

	void					Open() { pWindow->Show(); }
	void					Close() { pWindow->Hide(); }

	void					AddMember( const std::string strName, EClanMemberType eMemberType );
	void					RemoveMember( const std::string strName );

	void					UpdateMemberStatus( const std::string strName, bool bOnline );

	bool					MemberExists( const std::string strName );

private:

	UI::Window_ptr			pWindow = nullptr;

	UINT					uClanTimerWheel = 0;

	void					OnButtonNameClick( UI::Text_ptr pText, UIEventArgs eArgs );

	enum
	{
		WINDOW_Main,

		LIST_Members,

		GROUP_Member,

		TEXT_MemberName = 10000,

	};
};

