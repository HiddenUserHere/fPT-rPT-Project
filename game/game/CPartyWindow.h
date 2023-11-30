#pragma once

class CPartyWindow
{
enum EWindow1
{
	WINDOWID_Member1,
	WINDOWID_Member2,
	WINDOWID_Member3,
	WINDOWID_Member4,
	WINDOWID_Member5,
	WINDOWID_Member6,

	LISTID_PartyMembers				= 10,
	LISTID_PartyRaid				= 11,

	TEXTID_Name						= 18,
	TEXTID_CharacterLevel			= 19,

	IMAGEID_Background				= 20,
	IMAGEID_Icon					= 21,
	IMAGEID_LifeBar					= 22,
	IMAGEID_ManaBar					= 23,
	IMAGEID_Class					= 24,
	IMAGEID_IconLeader				= 25,
	IMAGEID_PartyType				= 26,

	DROPDOWNMENUID_Options			= 30, //up to 70

	TOOLTIPID_PartyType,
};

enum ENotificationWindow
{
	TEXTID_CharacterName			= 10,
};
public:
	//Constructor
									CPartyWindow();
								   ~CPartyWindow();

	void							Init();
	void							Render();

	void							Update( float fTime );

	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseClick( class CMouse * pcMouse );

	void							ShowPartyNotification( char * pszCharacterName, int iType );
	void							ShowRaidNotification( char * pszCharacterName );

	void							UpdateParty( CPartyData * pcParty );
	void							UpdatePartyData( CPartyData * pcParty );

	void							LeaveParty();
	void							ToggleWindow();

	UI::Window_ptr					GetWindowParty(){ return pWindowParty; }
	UI::Window_ptr					GetWindowRaid(){ return pWindowRaid; }
protected:
	UI::Window_ptr					pWindowParty;
	UI::Window_ptr					pWindowRaid;
private:
	int								iPartyRequestType = 0;

	UI::Image						* pImagePartyMode[3];

private:
	void							BuildWindowParty();
	void							BuildWindowRaid();

	void							ShowMessageBox( int iType, int iIndex = -1 );

	void							OnSelectMenuParty( int iIndex = -1 );

	void							OnAcceptPartyButtonClick();
	void							OnAcceptRaidButtonClick();
	void							OnDelegatePartyButtonClick( int iIndex );
	void							OnKickPartyButtonClick( int iIndex );
	void							OnLeavePartyButtonClick();
	void							OnDisbandPartyButtonClick();
	void							OnChangePartyModeButtonClick();
};