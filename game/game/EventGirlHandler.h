#pragma once

#define EVENTGIRL_ALLSTATS_PRICE		1000000
#define EVENTGIRL_ALLSKILLS_PRICE		1000000
#define EVENTGIRL_RESETSTATS_PRICE		200000

class CEventGirlHandler
{
public:
									CEventGirlHandler();
	virtual						   ~CEventGirlHandler();

	BOOL							OnMouseClick( class CMouse * pcMouse );
	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	void							OnResolutionChanged();

	void							Init();

	void							Render();

	void							Open( struct PacketEventGirl * psPacket );

	void							Close();

	BOOL							IsOpen() { return pWindow->IsOpen(); }

private:

	void							OnAllStatsClick( UIEventArgs e );
	void							OnAllSkillsClick( UIEventArgs e );
	void							OnResetButtonClick( UIEventArgs e );
	void							OnCloseButtonClick( UIEventArgs e );

	void							OnAcceptAllSkills( UIEventArgs e );
	void							OnAcceptAllStats( UIEventArgs e );
	void							OnAcceptResetStats( UIEventArgs e );

	void							ResetCheckBox();

	BOOL							IsFreeGold();

	enum
	{
		CHECKBOXID_Strength,
		CHECKBOXID_Spirit,
		CHECKBOXID_Talent,
		CHECKBOXID_Agility,
		CHECKBOXID_Health,

		TEXTID_StatusValue
	};

	UI::Window_ptr pWindow = NULL;

	int							iResetPrice = 0;

	BOOL						bFreeGold = FALSE;

};

