#pragma once

#include "Screen.h"
#include "DXVideoRenderer.h"
#include "CBrowserWindowHandler.h"
#include "CLoginScreenWindowHandler.h"

#define MAX_LOGINSERVER_LIST                    10
#define MAX_LOGINSERVER_CHARACTER_LIST          10

enum ELoginState
{
	LOGINSTATE_None,
	LOGINSTATE_Opening,
	LOGINSTATE_Connecting,
	LOGINSTATE_ServerSelect,
	LOGINSTATE_Finish,
};


struct LoginScreenMovementData
{
	Point3D				sBeginPosition;
	Point3D				sEndPosition;

	Point3D				sLookAt;

	float				fTime;
};


class CLoginScreen : public CScreen
{
private:
    struct LoginServerList
    {
        char                szName[32];

        char                szIP1[20];
        char                szIP2[20];
        char                szIP3[20];

        int                 iPort1;
        int                 iPort2;
        int                 iPort3;
    };

    struct LoginServerCharacterInfo
    {
        char                szName[100];

        char                szBodyModel[256];
        char                szHeadModel[256];
        
        char                szClanName[256];
        bool                bClan;

        void                * pcTextureClan;

        int                 iLevel;
        int                 iRace;
        int                 iClass;

        int                 iMapID;

        int                 iX;
        int                 iZ;

        DWORD               dwUnk[2];
    };

    static LoginServerList * psLoginServerList;

    static LoginServerCharacterInfo * psLoginServerCharacterInfo;

public:
	static int			GAMESERVER_SERVERID;

						CLoginScreen();
						~CLoginScreen();


	BOOL				Init();
	void				Shutdown();

	void				Render3D();
	void				Render2D();

	EScreen				Frame();

	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );

    BOOL				OnMouseScroll( class CMouse * pcMouse );

    BOOL				OnKeyPress( class CKeyboard * pcKeyboard );
    BOOL				OnKeyChar( class CKeyboard * pcKeyboard );

    void			    OnResolutionChanged();

	void				SetCamera( Point3D sPosition, Point3D sLookAt );

	void				UpdateCameraMovement();

	void				SetLoginBoxPosition( int iX, int iY ) { sLoginBoxPosition.iX = iX; sLoginBoxPosition.iY = iY; }
	void				SetLoginBoxSize( int iWidth, int iHeight ) { iLoginBoxWidth = iWidth; iLoginBoxHeight = iHeight; }

	ELoginState			GetCurrentState() { return iCurrentStateLogin; }

	BOOL				IsLoginVideo() { return bLoginVideo; }

    void                DisconnectAllServers();

    void                ConnectLoginServer( const std::string strAccountName, const std::string strPassword );

    void                ConnectGameServer( int iIndex );

    void                HandlePacket( SocketData * pcSocketData, PacketAccountLoginCode * psPacket );
    void                HandlePacket( PacketCharacterSelectUser * psPacket );
    void                HandlePacket( PacketServerList * psPacket );
    void                HandlePacket( SocketData * pcSocketData, PacketVersion * psPacket );

    void                CreateThreadLogin();
    bool                CreateThreadGame( int iIndex );


private:

	enum
	{
		TEXTID_TextNotify = 10,

		CHECKBOXID_ImageOnly,
		CHECKBOXID_VideoLow,
		CHECKBOXID_VideoHigh,

		CHECKBOXID_OfficialServer,
		CHECKBOXID_TestServer,
	};

    CLoginScreenWindowHandler * GetWindow() { return pcWindow; }

	void				OnUpdateResolution();

	void				OnCheckScreen( int iID, UIEventArgs e );
	void				OnCheckServer( int iID, UIEventArgs e );

	static void			SetClanData( const char * pszIP1, int iPort1, const char * pszIP2, int iPort2, int iTicket );

    static DWORD WINAPI        ThreadLogin();
    static DWORD WINAPI        ThreadGame( int * piIndex );

	BOOL				NewScreen() { return /*(CALLT( 0x0059D860, GetThis() ) == 2)*/ (iCurrentStateLogin == LOGINSTATE_Finish); }

	char				cPadding001[0x0080];

	ELoginState			iCurrentStateLogin;			//0x080

	ELoginState			iNextStateLogin;			//0x084

	int					iMode;						//0x088

	char				cPadding002[0x004];

	BOOL				bFailed;					//0x090

	char				cPadding003[0x78C];			//0x094

	Point2D				sLoginBoxPosition;			//0x820


	Point2D				sLoginBoxAccountPosition;	//0x828

	int					iUnk;						//0x830

	int					iLoginBoxWidth;				//0x834
	int					iLoginBoxHeight;			//0x838

	char				size[0x088];				//0x83C

	UI::Window_ptr		pWindowLoginScreenSelector = NULL;
	UI::Window_ptr		pWindowServerSelector = NULL;

	int					iOldCheckBox = 0;
	int					iOldCheckBoxServer = 0;

	BOOL				bShowNotify = FALSE;

	Stage				* pcStage = NULL;

	Point3D				sPositionCamera;
	Point3D				sPositionPoint;
	Point3D				sAngleCamera;

	BOOL				bLoginVideo = FALSE;

	int					iCameraMovementID = 0;
	float				fTimeCameraMovement = 0.0f;

	float				fTransparency = 0.0f;

#ifdef CEF_UI_BROWSER
	CBrowserWindowHandler * pcBrowser;
#endif

	UnitData			* pcaUnitData[10];

    CLoginScreenWindowHandler * pcWindow = nullptr;

}; //Size = 0x88C

