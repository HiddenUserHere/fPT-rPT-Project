#pragma once
class CLoginScreenWindowHandler
{
private:

    enum ELoginAction
    {
        LOGINACTION_None,
        LOGINACTION_BlockLevel1,
        LOGINACTION_BlockLevel2,
    };

    enum
    {
        WINDOW_Main,
        WINDOW_Login,
        WINDOW_SubServer,
        LIST_SubServer,

        INPUT_AccountName,
        INPUT_Password,
        IMAGE_AccountName,
        IMAGE_Password,

        IMAGE_LoginBox,

        IMAGE_Candle,
        IMAGE_AnimationXmas,

        BUTTON_Login,
        BUTTON_Exit,
        BUTTON_Settings,

        WINDOW_LoginMessage,
        RECTANGLEBOX_Message,
        TEXT_Message,

        WINDOW_SubServerFirst = 300,
    };

    ELoginAction                    eLoginAction = LOGINACTION_None;

public:
    CLoginScreenWindowHandler();
    virtual ~CLoginScreenWindowHandler();

    void                            Init();

    void                            Render();

    void                            Update( float fTime );

    void							OnResolutionChanged();

    void							OnMouseMove( class CMouse * pcMouse );
    BOOL							OnMouseClick( class CMouse * pcMouse );
    BOOL							OnMouseScroll( class CMouse * pcMouse );

    BOOL							OnKeyPress( class CKeyboard * pcKeyboard );
    BOOL							OnKeyChar( class CKeyboard * pcKeyboard );

    void                            AddLoginMessage( const std::string & strMessage, float fTime, bool bSound = false );

    void                            ClearServerList();

    void                            AddSubServer( const std::string strName, bool bPvP, int iIndex );

    void                            ShowServerList();

    void                            BlockWindowLoginMouse();
    void                            UnblockWindowLoginMouse();

    void                            BlockWindowSubServerMouse();
    void                            UnblockWindowSubServerMouse();

private:
    UI::Window_ptr                  pWindow1 = nullptr;

    float                           fAnimationUpdateTime = 0.0f;
    float                           fAnimationXmasUpdateTime = 0.0f;
    UINT                            uAnimationWheel = 0;
    UINT                            uAnimationXmasWheel = 0;
    UI::Image                       * pcaImageCandle[4];
    UI::Image                       * pcaImageAnimationXmas[4];

    std::string                     strLoginMessage;
    float                           fLoginMessageTime = 0.0f;

    void                            BuildWindow1();
    void                            BuildWindowMessage();
    void                            BuildWindowSubServer();

    UI::Window_ptr                  GetLoginWindow();

    UI::Window_ptr                  GetLoginMessageWindow();

    UI::Window_ptr                  GetSubServerWindow();

    void                            SwapInputs();
    void                            UnselectInputs();

    std::string                     GetAccountName();
    std::string                     GetPassword();

    bool                            PreviousBlock();

    void                            ShowLoginBox( bool bShow );

    void                            OnButtonLoginClick( UIEventArgs eArgs );
    void                            OnButtonExitClick( UIEventArgs eArgs );

    void                            OnButtonSelectServerClick( int iIndex, UIEventArgs eArgs );

    void                            ConnectLogin();
    void                            ConnectSubServer( int iIndex );
};

