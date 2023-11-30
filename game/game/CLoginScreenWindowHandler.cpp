#include "stdafx.h"
#include "CLoginScreenWindowHandler.h"

CLoginScreenWindowHandler::CLoginScreenWindowHandler()
{
    pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 800, 600 ) );

    pcaImageCandle[0] = nullptr;
    pcaImageCandle[1] = nullptr;
    pcaImageCandle[2] = nullptr;
    pcaImageCandle[3] = nullptr;

    pcaImageAnimationXmas[0] = nullptr;
    pcaImageAnimationXmas[1] = nullptr;
    pcaImageAnimationXmas[2] = nullptr;
    pcaImageAnimationXmas[3] = nullptr;
}

CLoginScreenWindowHandler::~CLoginScreenWindowHandler()
{
    pWindow1 = nullptr;
}

void CLoginScreenWindowHandler::Init()
{
    BuildWindow1();
    BuildWindowSubServer();
    BuildWindowMessage();

    pWindow1->SetWindowScale( 1.25f );

    OnResolutionChanged();
}

void CLoginScreenWindowHandler::Render()
{
    pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CLoginScreenWindowHandler::Update( float fTime )
{
    pWindow1->Update( fTime );

    if ( GetLoginMessageWindow()->IsOpen() )
    {
        if ( fLoginMessageTime > 0.0f )
        {
            fLoginMessageTime -= fTime;
            if ( fLoginMessageTime <= 0.0f )
            {
                GetLoginMessageWindow()->Hide();

                fLoginMessageTime = 0.0f;
            }
        }
    }

    fAnimationUpdateTime += fTime;
    if ( fAnimationUpdateTime >= (1000.0f / (60.0f / 8.0f)) )
    {
        GetLoginWindow()->GetElement<UI::ImageBox>( IMAGE_Candle )->SetImage( pcaImageCandle[uAnimationWheel++ % 4] );

        fAnimationUpdateTime = 0.0f;
    }

    if ( XMAS_EVENT_THINGS )
    {
        fAnimationXmasUpdateTime += fTime;
        if ( fAnimationXmasUpdateTime >= (1000.0f / (60.0f / 32.0f)) )
        {
            GetLoginWindow()->GetElement<UI::ImageBox>( IMAGE_AnimationXmas )->SetImage( pcaImageAnimationXmas[uAnimationXmasWheel++ % 4] );

            fAnimationXmasUpdateTime = 0.0f;
        }
    }

    auto pImageAccount = GetLoginWindow()->GetElement<UI::ImageBox>( IMAGE_AccountName );
    auto pImagePassword = GetLoginWindow()->GetElement<UI::ImageBox>( IMAGE_Password );

    auto pInputAccount = GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName );
    auto pInputPassword = GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password );

    pImageAccount->Hide();
    pImagePassword->Hide();

    if ( pInputAccount->IsOpen() && pInputAccount->IsSelected() )
        pImageAccount->Show();
    else if ( pInputPassword->IsOpen() && pInputPassword->IsSelected() )
        pImagePassword->Show();
}

void CLoginScreenWindowHandler::OnResolutionChanged()
{
    pWindow1->SetWindowScale( Element::GetResolutionScale() );

    pWindow1->SetBox( Rectangle2D( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT ) );

    //Update Login Position
    auto pWindowLogin = pWindow1->GetElement<UI::Window>( WINDOW_Login );
    {
        int iX = (RESOLUTION_WIDTH >> 1) - (pWindowLogin->GetWidth() >> 1);
        int iY = (RESOLUTION_HEIGHT >> 1) - (pWindowLogin->GetHeight() >> 1);
        
        pWindowLogin->SetPosition( iX, iY );
    }

    //Update Exit Position
    {
        auto pButtonExit = pWindow1->GetElement<UI::Button>( BUTTON_Exit );

        int iX = pWindowLogin->GetX() + (int)((float)504 * Element::GetResolutionScale());
        int iY = pWindowLogin->GetY() + (int)((float)498 * Element::GetResolutionScale());

        pButtonExit->SetPosition( iX, iY );
    }

    //Update Settings Position
    {
        auto pButtonSettings = pWindow1->GetElement<UI::Button>( BUTTON_Settings );

        pButtonSettings->SetPosition( (RESOLUTION_WIDTH >> 1) - (pButtonSettings->GetWidth() >> 1) + (int)(12.0f * Element::GetResolutionScale()), 0 );
    }

    //Update Message Window Position
    auto pWindowMessage = GetLoginMessageWindow();
    pWindowMessage->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowMessage->GetWidth() >> 1) + 7, (RESOLUTION_HEIGHT >> 1) - (pWindowMessage->GetHeight() >> 1) );

    //Update Server Select
    {
        auto pWindowSubServer = GetSubServerWindow();

        int iX = (RESOLUTION_WIDTH >> 1) - (pWindowLogin->GetWidth() >> 1);
        int iY = (RESOLUTION_HEIGHT >> 1) - (pWindowLogin->GetHeight() >> 1);

        pWindowSubServer->SetPosition( (int)((float)(iX) + 286.0f * Element::GetResolutionScale()), (int)((float)(iY + 327) * Element::GetResolutionScale()) );

        auto pList = pWindowSubServer->GetElement<UI::List>( LIST_SubServer );
        pList->SetHeight( pList->GetListHeight() );
        pList->SetPosition( pList->GetX(), ((pWindowSubServer->GetHeight() + (int)(40.0f * Element::GetResolutionScale())) >> 1) - (pList->GetHeight() >> 1) );
    }
}

void CLoginScreenWindowHandler::OnMouseMove( CMouse * pcMouse )
{
    pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CLoginScreenWindowHandler::OnMouseClick( CMouse * pcMouse )
{
    return pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CLoginScreenWindowHandler::OnMouseScroll( CMouse * pcMouse )
{
    return pWindow1->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CLoginScreenWindowHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
    if ( pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown )
    {
        if ( GetLoginWindow()->IsMouseClickEnabled() )
        {
            if ( pcKeyboard->GetKey() == VK_RETURN )
            {
                ConnectLogin();

                UnselectInputs();

                return TRUE;
            }
            else if ( pcKeyboard->GetKey() == VK_TAB )
            {
                SwapInputs();

                return TRUE;
            }

            if ( pcKeyboard->IsKeyDown( VK_CONTROL ) && (pcKeyboard->GetKey() >= VK_F1 && pcKeyboard->GetKey() <= VK_F10) )
            {
                char * pszMacroText = SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[(pcKeyboard->GetKey() & 0x0F)];

                if ( pszMacroText[0] != NULL )
                {
                    if ( GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->IsSelected() )
                        GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->AddInput( pszMacroText );
                    else
                        GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password )->AddInput( pszMacroText );
                    return TRUE;
                }
            }
        }
        else if ( GetSubServerWindow()->IsOpen() )
        {
            if ( pcKeyboard->GetKey() == VK_ESCAPE )
            {
                PreviousBlock();

                return TRUE;
            }
        }
    }

    return pWindow1->OnKeyPress( pcKeyboard );
}

BOOL CLoginScreenWindowHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
    if ( GetSubServerWindow()->IsOpen() )
        return TRUE;

    return pWindow1->OnKeyChar( pcKeyboard );
}

void CLoginScreenWindowHandler::AddLoginMessage( const std::string & strMessage, float fTime, bool bSound )
{
    fLoginMessageTime = fTime;

    auto pWindowMessage = GetLoginMessageWindow();

    auto pText = pWindowMessage->GetElement<UI::Text>( TEXT_Message );
    auto pRectangleBox = pWindowMessage->GetElement<UI::RectangleBox>( RECTANGLEBOX_Message );

    pText->SetText( strMessage );

    pWindowMessage->SetWidth( GETWIDTHTEXT( strMessage.c_str() ) + 60 );
    
    pRectangleBox->SetWidth( pWindowMessage->GetWidth() );
    
    pText->SetWidth( pRectangleBox->GetWidth() - 60 );

    pWindowMessage->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowMessage->GetWidth() >> 1) + 7, (RESOLUTION_HEIGHT >> 1) - (pWindowMessage->GetHeight() >> 1) );

    if ( fLoginMessageTime > 0.0f )
        pWindowMessage->Show();
    else
        pWindowMessage->Hide();

    if ( bSound )
    {
        CSound::PlaySoundID( 12 );

        SOUNDHANDLER->UpdateVolume();
    }
}

void CLoginScreenWindowHandler::ClearServerList()
{
    auto pList = GetSubServerWindow()->GetElement<UI::List>( LIST_SubServer );
    pList->Clear();
}

void CLoginScreenWindowHandler::AddSubServer( const std::string strName, bool bPvP, int iIndex )
{
    auto pList = GetSubServerWindow()->GetElement<UI::List>( LIST_SubServer );

    UI::Window_ptr pWindowSubServer = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 197, 50 ) );
    pWindowSubServer->SetID( WINDOW_SubServerFirst + iIndex );
    pList->AddWindow( pWindowSubServer );

    UI::Text_ptr pTextSubServer = std::make_shared<UI::Text>( Rectangle2D( 25, 15, 148, 45 ) );
    pTextSubServer->SetFont( "Constantia", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
    pTextSubServer->SetText( strName );
    pTextSubServer->SetHorizontalAlign( ALIGN_Center );
    pTextSubServer->SetColorHover( D3DCOLOR_XRGB( 226, 208, 80 ) );
    pWindowSubServer->AddElement( pTextSubServer );

    UI::Button_ptr pButtonServerSelect = std::make_shared<UI::Button>( Rectangle2D( 25, 2, 148, 45 ) );
    pButtonServerSelect->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\login\\serverselect.png", "game\\images\\login\\serverselect_hover.png" ) );
    pButtonServerSelect->SetEvent( UI::Event::Build( std::bind( &CLoginScreenWindowHandler::OnButtonSelectServerClick, this, iIndex, std::placeholders::_1 ) ) );
    pWindowSubServer->AddElement( pButtonServerSelect );

    if ( bPvP )
    {
        int iTextWidth = (197 >> 1) + (pTextSubServer->GetTextWidth() >> 1) + 8;

        UI::ImageBox_ptr pImageBoxPvPFlag = std::make_shared<UI::ImageBox>( Rectangle2D( iTextWidth, 15, 25, 13 ) );
        pImageBoxPvPFlag->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\pvpflag.png" ) );
        pWindowSubServer->AddElement( pImageBoxPvPFlag );
    }

    pList->SetHeight( pList->GetListHeight() );
    pList->SetPosition( pList->GetX(), ((GetSubServerWindow()->GetHeight() + (int)(40.0f * Element::GetResolutionScale())) >> 1) - (pList->GetHeight() >> 1) );
}

void CLoginScreenWindowHandler::ShowServerList()
{
    ShowLoginBox( false );

    GetSubServerWindow()->Show();

    AddLoginMessage( "", 0.0f );
}

void CLoginScreenWindowHandler::BlockWindowLoginMouse()
{
    GetLoginWindow()->SetMouseClickEnable( FALSE );
}

void CLoginScreenWindowHandler::UnblockWindowLoginMouse()
{
    GetLoginWindow()->SetMouseClickEnable( TRUE );
}

void CLoginScreenWindowHandler::BlockWindowSubServerMouse()
{
    GetSubServerWindow()->SetMouseClickEnable( FALSE );
}

void CLoginScreenWindowHandler::UnblockWindowSubServerMouse()
{
    GetSubServerWindow()->SetMouseClickEnable( TRUE );
}

void CLoginScreenWindowHandler::BuildWindow1()
{
    UI::Window_ptr pWindowLogin = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 757, 645 ) );
    pWindowLogin->SetID( WINDOW_Login );
    if ( XMAS_EVENT_THINGS )
        pWindowLogin->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\xmas\\window_xmas.png" ) );
    else
        pWindowLogin->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\window.png" ) );
    
    UI::Group_ptr pGroupInput = std::make_shared<UI::Group>();
    pWindowLogin->AddElement( pGroupInput );

    //Image Login Box
    UI::ImageBox_ptr pImageSelectServer = std::make_shared<UI::ImageBox>( Rectangle2D( 286, 347, 197, 117 ) );
    pImageSelectServer->SetID( IMAGE_LoginBox );
    pImageSelectServer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\loginbox.png" ) );
    pWindowLogin->AddElement( pImageSelectServer );

    //Image Candle
    UI::ImageBox_ptr pImageCandle = std::make_shared<UI::ImageBox>( Rectangle2D( 195, 408, 381, 59 ) );
    pImageCandle->SetID( IMAGE_Candle );
    pImageCandle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\candle1.png" ) );
    pWindowLogin->AddElement( pImageCandle );

    for ( int i = 0; i < _countof( pcaImageCandle ); i++ )
        pcaImageCandle[i] = UI::ImageLoader::LoadImage( "game\\images\\login\\candle%d.png", i + 1 );

    //Xmas
    if ( XMAS_EVENT_THINGS )
    {
        //Image Xmas Animation
        UI::ImageBox_ptr pImageXmasAnimation = std::make_shared<UI::ImageBox>( Rectangle2D( 98, 128, 625, 159 ) );
        pImageXmasAnimation->SetID( IMAGE_AnimationXmas );
        pImageXmasAnimation->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\xmas\\xmaslight1.png" ) );
        pWindowLogin->AddElement( pImageXmasAnimation );

        for ( int i = 0; i < _countof( pcaImageAnimationXmas ); i++ )
            pcaImageAnimationXmas[i] = UI::ImageLoader::LoadImage( "game\\images\\login\\xmas\\xmaslight%d.png", i + 1 );
    }

    //Image Logo
    UI::ImageBox_ptr pImageRegnumLogo = std::make_shared<UI::ImageBox>( Rectangle2D( 261, 189, 240, 140 ) );
    if( XMAS_EVENT_THINGS )
        pImageRegnumLogo->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\xmas\\logo_xmas.png" ) );
    else
        pImageRegnumLogo->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\logo.png" ) );
    pWindowLogin->AddElement( pImageRegnumLogo );

    //Image Account
    UI::ImageBox_ptr pImageAccount = std::make_shared<UI::ImageBox>( Rectangle2D( 310, 359, 148, 45 ) );
    pImageAccount->SetID( IMAGE_AccountName );
    pImageAccount->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\serverselect_hover.png" ) );
    pImageAccount->Hide();
    pWindowLogin->AddElement( pImageAccount );

    //Image Account
    UI::Noesis_ptr pNN = std::make_shared<UI::UINoesis>( Rectangle2D( 0, 0, 100, 100 ) );
    pNN->SetID( 5155115 );
    pNN->LoadFromFile( "MainWindow.xaml" );
   // pWindowLogin->AddElement( pNN );

    //Image Password
    UI::ImageBox_ptr pImagePassword = std::make_shared<UI::ImageBox>( Rectangle2D( 310, 418, 148, 45 ) );
    pImagePassword->SetID( IMAGE_Password );
    pImagePassword->SetImage( UI::ImageLoader::LoadImage( "game\\images\\login\\serverselect_hover.png" ) );
    pImagePassword->Hide();
    pWindowLogin->AddElement( pImagePassword );

    //Input Account
    UI::InputField_ptr pInputAccount = std::make_shared<UI::InputField>( Rectangle2D( 319, 363, 133, 30 ) );
    pInputAccount->SetID( INPUT_AccountName );
    pInputAccount->SetMaxLength( 16 );
    pInputAccount->SetGroup( pGroupInput );
    pInputAccount->SetFont( "Constantia", 18, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
    pInputAccount->SetText( SETTINGSHANDLER->GetModel()->GetSettings().szLastAccount );
    pInputAccount->SetPlaceHolder( "Account", DX::Color( 64, 255, 255, 255 ).Get() );
    pInputAccount->SetTextAlign( ALIGN_Center, ALIGN_Center );
    pInputAccount->SetSelect( TRUE );
    pInputAccount->SetSelected( SETTINGSHANDLER->GetModel()->GetSettings().szLastAccount[0] ? FALSE : TRUE );
    pInputAccount->SetCanSelectText( FALSE );
    pWindowLogin->AddElement( pInputAccount );

    //Input Account
    UI::InputField_ptr pInputPassword = std::make_shared<UI::InputField>( Rectangle2D( 319, 423, 133, 30 ) );
    pInputPassword->SetID( INPUT_Password );
    pInputPassword->SetMaxLength( 16 );
    pInputPassword->SetGroup( pGroupInput );
    pInputPassword->SetFont( "Constantia", 18, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
    pInputPassword->SetText( "" );
    pInputPassword->SetPlaceHolder( "Password", DX::Color( 64, 255, 255, 255 ).Get() );
    pInputPassword->SetTextAlign( ALIGN_Center, ALIGN_Center );
    pInputPassword->SetEnableMaskPassword( TRUE );
    pInputPassword->SetSelect( TRUE );
    pInputPassword->SetSelected( SETTINGSHANDLER->GetModel()->GetSettings().szLastAccount[0] ? TRUE : FALSE );
    pInputPassword->SetCanSelectText( FALSE );
    pWindowLogin->AddElement( pInputPassword );

    //Button Login
    UI::Button_ptr pButtonLogin = std::make_shared<UI::Button>( Rectangle2D( 318, 453, 133, 88 ) );
    pButtonLogin->SetID( BUTTON_Login );
    pButtonLogin->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\login\\loginbtn.png", "game\\images\\login\\loginbtn_.png" ) );
    pButtonLogin->SetHoldImage( UI::ImageLoader::LoadImage( "game\\images\\login\\loginbtn_pressed.png" ) );
    pButtonLogin->SetDisabledImage( UI::ImageLoader::LoadImage( "game\\images\\login\\loginbtn_pressed.png" ) );
    pButtonLogin->SetEvent( UI::Event::Build( std::bind( &CLoginScreenWindowHandler::OnButtonLoginClick, this, std::placeholders::_1 ) ) );
    pButtonLogin->SetMouseEvent( EMouseEvent::ClickUpL );
    pWindowLogin->AddElement( pButtonLogin );

    pWindow1->AddElement( pWindowLogin );

    //Button Exit
    UI::Button_ptr pButtonExit = std::make_shared<UI::Button>( Rectangle2D( 504, 498, 71, 55 ) );
    pButtonExit->SetID( BUTTON_Exit );
    pButtonExit->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\login\\exitbtn.png", "game\\images\\login\\exitbtn_.png" ) );
    pButtonExit->SetHoldImage( UI::ImageLoader::LoadImage( "game\\images\\login\\exitbtn_pressed.png" ) );
    pButtonExit->SetEvent( UI::Event::Build( std::bind( &CLoginScreenWindowHandler::OnButtonExitClick, this, std::placeholders::_1 ) ) );
    pButtonExit->SetMouseEvent( EMouseEvent::ClickUpL );
    pWindow1->AddElement( pButtonExit );

    //Button Settings
    UI::Button_ptr pButtonSettings = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 187, 57 ) );
    pButtonSettings->SetID( BUTTON_Settings );
    pButtonSettings->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\login\\settingsbtn.png", "game\\images\\login\\settingsbtn_.png" ) );
    pButtonSettings->SetHoldImage( UI::ImageLoader::LoadImage( "game\\images\\login\\settingsbtn_pressed.png" ) );
    pButtonSettings->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnLoginButtonClick, SETTINGSHANDLER->GetWindow(), std::placeholders::_1 ) ) );
    pButtonSettings->SetMouseEvent( EMouseEvent::ClickUpL );
    pWindow1->AddElement( pButtonSettings );
}

void CLoginScreenWindowHandler::BuildWindowMessage()
{
    //Window Message
    UI::Window_ptr pWindowMessage = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 200, 80 ) );
    pWindowMessage->SetID( WINDOW_LoginMessage );
    pWindowMessage->SetUseSelfScale( true );

    //Rectangle Box
    UI::RectangleBox_ptr pRectangleBox = std::make_shared<UI::RectangleBox>( Rectangle2D( 0, 0, 200, 50 ) );
    pRectangleBox->SetID( RECTANGLEBOX_Message );
    pRectangleBox->SetColor( DX::Color( 235, 45, 39, 27 ), DX::Color( 245, 9, 23, 22 ) );
    pWindowMessage->AddElement( pRectangleBox );

    //Text Message
    UI::Text_ptr pTextMessage = std::make_shared<UI::Text>( Rectangle2D( 30, 5, 140, 20 ) );
    pTextMessage->SetID( TEXT_Message );
    pTextMessage->SetFont( "Arial", 16, 0, FALSE, FALSE, D3DCOLOR_XRGB( 255, 255, 255 ) );
    pTextMessage->SetText( "" );
    pWindowMessage->AddElement( pTextMessage );

    pWindowMessage->Hide();
    pWindow1->AddElement( pWindowMessage );
}

void CLoginScreenWindowHandler::BuildWindowSubServer()
{
    UI::Window_ptr pWindowSubServer = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 197, 117 ) );
    pWindowSubServer->SetID( WINDOW_SubServer );

    //List Subservers
    UI::List_ptr pListSubServers = std::make_shared<UI::List>( Rectangle2D( 0, 0, 197, 117 ) );
    pListSubServers->SetID( LIST_SubServer );
    pWindowSubServer->AddElement( pListSubServers );

    pWindowSubServer->Hide();
    pWindow1->AddElement( pWindowSubServer );
}

UI::Window_ptr CLoginScreenWindowHandler::GetLoginWindow()
{
    return pWindow1->GetElement<UI::Window>( WINDOW_Login );
}

UI::Window_ptr CLoginScreenWindowHandler::GetLoginMessageWindow()
{
    return pWindow1->GetElement<UI::Window>( WINDOW_LoginMessage );
}

UI::Window_ptr CLoginScreenWindowHandler::GetSubServerWindow()
{
    return pWindow1->GetElement<UI::Window>( WINDOW_SubServer );
}

void CLoginScreenWindowHandler::SwapInputs()
{
    if ( GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->IsSelected() )
    {
        GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->SetSelected( FALSE );

        GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password )->SetSelected( TRUE );
    }
    else
    {
        GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password )->SetSelected( FALSE );

        GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->SetSelected( TRUE );
    }
}

void CLoginScreenWindowHandler::UnselectInputs()
{
    GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password )->SetSelected( FALSE );

    GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->SetSelected( FALSE );
}

std::string CLoginScreenWindowHandler::GetAccountName()
{
    return GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName )->GetText();
}

std::string CLoginScreenWindowHandler::GetPassword()
{
    return GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password )->GetText();
}

bool CLoginScreenWindowHandler::PreviousBlock()
{
    if ( fLoginMessageTime > 0.0f )
        return false;

    if ( GetLoginWindow()->IsMouseClickEnabled() == FALSE )
    {
        UnblockWindowLoginMouse();

        ClearServerList();

        GetSubServerWindow()->Hide();

        ShowLoginBox( true );

        CSound::PlaySoundID( 3 );

        SOUNDHANDLER->UpdateVolume();

        return false;
    }

    return true;
}

void CLoginScreenWindowHandler::ShowLoginBox( bool bShow )
{
    auto pLoginBox = GetLoginWindow()->GetElement<UI::ImageBox>( IMAGE_LoginBox );
    auto pInputAccount = GetLoginWindow()->GetElement<UI::InputField>( INPUT_AccountName );
    auto pInputPassword = GetLoginWindow()->GetElement<UI::InputField>( INPUT_Password );
    auto pButtonLogin = GetLoginWindow()->GetElement<UI::Button>( BUTTON_Login );

    if ( bShow )
    {
        pButtonLogin->Enable();
        pInputAccount->Show();
        pInputPassword->Show();
        pLoginBox->Show();
    }
    else
    {
        pButtonLogin->Disable();
        pInputAccount->Hide();
        pInputPassword->Hide();
        pLoginBox->Hide();
    }
}

void CLoginScreenWindowHandler::OnButtonLoginClick( UIEventArgs eArgs )
{
    ConnectLogin();
}

void CLoginScreenWindowHandler::OnButtonExitClick( UIEventArgs eArgs )
{
    if ( PreviousBlock() )
    {
        GameCore::QuitGame( TRUE );

        ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->DisconnectAllServers();
    }
}

void CLoginScreenWindowHandler::OnButtonSelectServerClick( int iIndex, UIEventArgs eArgs )
{
    ConnectSubServer( iIndex );
}

void CLoginScreenWindowHandler::ConnectLogin()
{
    if ( GetAccountName().length() == 0 )
    {
        AddLoginMessage( "Please fill the Account", 3000.0f, true );
        return;
    }

    if ( GetPassword().length() == 0 )
    {
        AddLoginMessage( "Please fill the Password", 3000.0f, true );
        return;
    }

    STRINGCOPYLEN( (char *)0x039033E8, 32, trim(std::string(GetAccountName())).c_str() );
    STRINGCOPYLEN( (char *)0x039032E8, 32, trim(std::string(GetPassword())).c_str() );

    AddLoginMessage( "Connecting", 60000.0f, true );

    BlockWindowLoginMouse();

    ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->CreateThreadLogin();
}

void CLoginScreenWindowHandler::ConnectSubServer( int iIndex )
{
    AddLoginMessage( "Connecting", 60000.0f, true );

    BlockWindowSubServerMouse();

    ((CLoginScreen *)GAMESCREEN->GetActiveScreen())->CreateThreadGame( iIndex );
}
