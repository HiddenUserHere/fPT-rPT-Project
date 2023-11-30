#pragma once

#define MAX_ACHIEVEMENT_COLUMN	7


class CAchievementWindow
{
public:

	enum EAchievementWindow
	{
		ACHIEVEMENTWINDOW_User,
		ACHIEVEMENTWINDOW_Target,
	};

	enum
	{
		LISTID_ListAchievement,
		WINDOWID_ListWindowButtons,

		BUTTON_Left,
		BUTTON_Right,


		IMAGEID_LogoAchievement,
		TEXTID_NameAchievement,
		TEXTID_DescriptionAchievement,
	};

	CAchievementWindow();
	~CAchievementWindow();

	void					Init();

	void					Render();

	void					Update( float fTime );

	BOOL					OnMouseClick( CMouse * pcMouse );
	BOOL					OnMouseScroll( CMouse * pcMouse );
	void					OnMouseMove( CMouse * pcMouse );

	void					SetAchievementViewHover( CAchievement * pcAchievemenent );

	void					AddAchievement( CAchievement * pcAchievement, Unit * pcUnit = nullptr );

	void					OnResolutionChanged();

	void					ClearAchievementsList();

	void					SetWindowType( EAchievementWindow iType );

	void					UpdateWindowPosition();

	void					UpdateWindowList( Unit * pcUnit, EAchievementWindow iType );

private:
	const int				MS_TIME_TRANSITION_ACHIEVEMENT = 500;

	void					BuildWindow();
	void					BuildWindowHover();
	void					BuildWindowList();

	void					OnButtonAchievementListOpenClick( UIEventArgs eArgs );
	void					OnButtonAchievementListClick( CAchievement * pcAchievement, UIEventArgs eArgs );

	void					OnButtonAchievementLeftClick( UIEventArgs eArgs );
	void					OnButtonAchievementRightClick( UIEventArgs eArgs );

	void					SetAlphaAchievementWindowID( UINT uWindowID, int iAlpha );

	UI::Window_ptr			pWindow;
	UI::Window_ptr			pWindowList;
	UI::Window_ptr			pWindowHover;

	EAchievementWindow		iAchievementWindow = ACHIEVEMENTWINDOW_Target;

	UINT					uCurrentAchievementID	= 0;
	UINT					uNextAchievementID		= 0;
	float					fAchievementTransitionTime = 0.0f;

};

