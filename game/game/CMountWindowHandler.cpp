#include "stdafx.h"
#include "CMountWindowHandler.h"

CMountWindowHandler::CMountWindowHandler()
{
    pWindow1 = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 282, 556 ) );
}

CMountWindowHandler::~CMountWindowHandler()
{
    pWindow1 = nullptr;

    for ( auto ps : vMountItemsData )
        DELET( ps );

    vMountItemsData.clear();
}

void CMountWindowHandler::ClearMounts()
{
    auto pList = pWindow1->GetElement<UI::List>( LIST_Mounts );
    pList->Clear();

    for ( auto ps : vMountItemsData )
        DELET( ps );

    vMountItemsData.clear();
}

void CMountWindowHandler::Init()
{
    BuildWindow1();

    OnResolutionChanged();
}

void CMountWindowHandler::Render()
{
    pWindow1->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CMountWindowHandler::Update( float fTime )
{
    pWindow1->Update( fTime );
}

void CMountWindowHandler::OnMouseMove( CMouse * pcMouse )
{
    pWindow1->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CMountWindowHandler::OnMouseClick( CMouse * pcMouse )
{
    return pWindow1->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CMountWindowHandler::OnMouseScroll( CMouse * pcMouse )
{
    return pWindow1->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CMountWindowHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
    return pWindow1->OnKeyPress( pcKeyboard );
}

BOOL CMountWindowHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
    return pWindow1->OnKeyChar( pcKeyboard );
}

void CMountWindowHandler::OnResolutionChanged()
{
    int iWindowX = RESOLUTION_WIDTH - pWindow1->GetWidth() - 32;
    int iWindowY = (RESOLUTION_HEIGHT >> 1) - (pWindow1->GetHeight() >> 1);
    pWindow1->SetPosition( iWindowX, iWindowY );
}

void CMountWindowHandler::Open()
{
    if ( IsOpen() == FALSE )
        pWindow1->Show();
}

void CMountWindowHandler::Close()
{
    if ( IsOpen() )
        pWindow1->Hide();
}

void CMountWindowHandler::AddMount( int iID, const std::string strMountName, const std::string strMountImageFile, EMountType eMountType, EMountRareType eRareType, Item * psaItems, int iCount, BOOL bIsSelected )
{
    AddEquipmentWindow( iID, strMountName, eMountType, eRareType, psaItems, iCount );

    AddMountWindow( iID, strMountName, strMountImageFile, eMountType, eRareType, bIsSelected );
}


void CMountWindowHandler::BuildWindow1()
{
    pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\window.png" ) );
    pWindow1->SetMoveBox( Rectangle2D( 0, 0, 282, 80 ) );
    pWindow1->SetCanMove( TRUE );

    //Button Close
    UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 240, 30, 24, 24 ) );
    pButtonClose->SetID( BUTTON_Close );
    pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\mount\\btnclose.png", "game\\images\\mount\\btnclose_.png" ) );
    pButtonClose->SetHoldImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\btnclose_click.png" ) );
    pButtonClose->SetMouseEvent( EMouseEvent::ClickUpL );
    pButtonClose->SetEvent( UI::Event::Build( std::bind( &CMountWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
    pWindow1->AddElement( pButtonClose );

    //Background Images
    UI::ImageBox_ptr pImageItemSlot1 = std::make_shared<UI::ImageBox>( Rectangle2D( 50, 471, 52, 52 ) );
    pImageItemSlot1->SetID( IMAGEBOX_Item1 );
    pImageItemSlot1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\bgitem.png" ) );
    pImageItemSlot1->Hide();
    pWindow1->AddElement( pImageItemSlot1 );

    UI::ImageBox_ptr pImageItemSlot2 = std::make_shared<UI::ImageBox>( Rectangle2D( 114, 471, 52, 52 ) );
    pImageItemSlot2->SetID( IMAGEBOX_Item2 );
    pImageItemSlot2->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\bgitem.png" ) );
    pImageItemSlot2->Hide();
    pWindow1->AddElement( pImageItemSlot2 );

    UI::ImageBox_ptr pImageItemSlot3 = std::make_shared<UI::ImageBox>( Rectangle2D( 178, 471, 52, 52 ) );
    pImageItemSlot3->SetID( IMAGEBOX_Item3 );
    pImageItemSlot3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\bgitem.png" ) );
    pImageItemSlot3->Hide();
    pWindow1->AddElement( pImageItemSlot3 );

    //Mount List
    UI::List_ptr pListMounts = std::make_shared<UI::List>( Rectangle2D( 24, 128, 230, 272 ) );
    pListMounts->SetID( LIST_Mounts );
    pListMounts->MakeScrollBar( Rectangle2D( 226, 2, 6, 268 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
    pWindow1->AddElement( pListMounts );

    UI::ItemBox_ptr pItemBox1 = std::make_shared<UI::ItemBox>( 52, 473, 2, 2 );
    pItemBox1->SetID( ITEMBOX_Item1 );
    pItemBox1->SetMaxItems( 1 );
    pItemBox1->SetEventOnGetItem( UI::Event::Build( std::bind( &CMountWindowHandler::OnGetItemSlot, this, IMAGEBOX_Item1, ITEMBOX_Item1, std::placeholders::_1 ) ) );
    pItemBox1->SetEventOnPutItem( UI::Event::Build( std::bind( &CMountWindowHandler::OnPutItemSlot, this, IMAGEBOX_Item1, ITEMBOX_Item1, std::placeholders::_1 ) ) );
    pItemBox1->DisableMouse();
    pWindow1->AddElement( pItemBox1 );

    UI::ItemBox_ptr pItemBox2 = std::make_shared<UI::ItemBox>( 116, 473, 2, 2 );
    pItemBox2->SetID( ITEMBOX_Item2 );
    pItemBox2->SetMaxItems( 1 );
    pItemBox2->SetEventOnGetItem( UI::Event::Build( std::bind( &CMountWindowHandler::OnGetItemSlot, this, IMAGEBOX_Item2, ITEMBOX_Item2, std::placeholders::_1 ) ) );
    pItemBox2->SetEventOnPutItem( UI::Event::Build( std::bind( &CMountWindowHandler::OnPutItemSlot, this, IMAGEBOX_Item2, ITEMBOX_Item2, std::placeholders::_1 ) ) );
    pItemBox2->DisableMouse();
    pWindow1->AddElement( pItemBox2 );

    UI::ItemBox_ptr pItemBox3 = std::make_shared<UI::ItemBox>( 180, 473, 2, 2 );
    pItemBox3->SetID( ITEMBOX_Item3 );
    pItemBox3->SetMaxItems( 1 );
    pItemBox3->SetEventOnGetItem( UI::Event::Build( std::bind( &CMountWindowHandler::OnGetItemSlot, this, IMAGEBOX_Item3, ITEMBOX_Item3, std::placeholders::_1 ) ) );
    pItemBox3->SetEventOnPutItem( UI::Event::Build( std::bind( &CMountWindowHandler::OnPutItemSlot, this, IMAGEBOX_Item3, ITEMBOX_Item3, std::placeholders::_1 ) ) );
    pItemBox3->DisableMouse();
    pWindow1->AddElement( pItemBox3 );

    pWindow1->Hide();
}

void CMountWindowHandler::AddMountWindow( int iID, const std::string strMountName, const std::string strMountImageFile, EMountType eMountType, EMountRareType eRareType, BOOL bIsSelected )
{
    auto pList = pWindow1->GetElement<UI::List>( LIST_Mounts );

    UI::Window_ptr pWindowMountList = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 222, 57 ) );
    pWindowMountList->SetID( WINDOW_Mounts + iID );
    pWindowMountList->SetEvent( UI::Event::Build( std::bind( &CMountWindowHandler::OnButtonSelectMountWindow, this, pWindowMountList->GetID(), eMountType, std::placeholders::_1 ) ) );

    UI::ImageBox_ptr pImageSlotDefault = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 222, 57 ) );
    pImageSlotDefault->SetID( IMAGEBOX_MountBox );
    pImageSlotDefault->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\bg_mount.png" ) );
    pWindowMountList->AddElement( pImageSlotDefault );

    UI::ImageBox_ptr pImageSlotSelected = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 0, 222, 57 ) );
    pImageSlotSelected->SetID( IMAGEBOX_MountBoxSelected );
    pImageSlotSelected->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\bg_mount_.png" ) );
    pImageSlotSelected->Hide();
    pWindowMountList->AddElement( pImageSlotSelected );

    UI::ImageBox_ptr pImageMount = std::make_shared<UI::ImageBox>( Rectangle2D( 6, 6, 44, 44 ) );
    pImageMount->SetID( IMAGEBOX_MountImage );
    pImageMount->SetImage( UI::ImageLoader::LoadImage( strMountImageFile.c_str() ) );
    pWindowMountList->AddElement( pImageMount );

    UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 54, 10, 160, 20 ) );
    pTextName->SetID( TEXT_MountName );
    pTextName->SetFont( DX::DEFAULT_FONT.c_str(), DX::DEFAULT_FONT_HEIGHT, DX::DEFAULT_FONT_WIDTH, FALSE, TRUE, DX::Color( 255, 40, 40, 40 ).Get() );
    pTextName->SetText( strMountName );
    pWindowMountList->AddElement( pTextName );

    UI::Text_ptr pTextRareType = std::make_shared<UI::Text>( Rectangle2D( 54, 30, 160, 20 ) );
    pTextRareType->SetFont( DX::DEFAULT_FONT.c_str(), DX::DEFAULT_FONT_HEIGHT, DX::DEFAULT_FONT_WIDTH, FALSE, TRUE, GetMountRareTypeToColor( eRareType ) );
    pTextRareType->SetTextEx( "%s", GetMountRareTypeToString( eRareType ) );
    pWindowMountList->AddElement( pTextRareType );

    UI::Button_ptr pButtonMount = std::make_shared<UI::Button>( Rectangle2D( 182, 13, 31, 31 ) );
    pButtonMount->SetImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\btn_mount.png" ) );
    pButtonMount->SetHoldImage( UI::ImageLoader::LoadImage( "game\\images\\mount\\btn_mount_.png" ) );
    pButtonMount->SetMouseEvent( EMouseEvent::ClickUpL );
    pButtonMount->SetEvent( UI::Event::Build( std::bind( &CMountWindowHandler::OnButtonMountClick, this, pWindowMountList->GetID(), eMountType, std::placeholders::_1 ) ) );
    pWindowMountList->AddElement( pButtonMount );

    if ( bIsSelected )
        OnButtonSelectMountWindow( pWindowMountList->GetID(), eMountType, UIEventArgs{} );

    pList->AddWindow( pWindowMountList );
}

void CMountWindowHandler::AddEquipmentWindow( int iID, const std::string strMountName, EMountType eMountType, EMountRareType eRareType, Item * psaItems, int iCount )
{
    MountItemGame * psMountItems = new MountItemGame;
    psMountItems->iMountID = iID;
    psMountItems->strName = strMountName;
    psMountItems->eMountType = eMountType;
    psMountItems->eRareType = eRareType;

    for ( int i = 0; i < iCount; i++ )
        psMountItems->vItems.push_back( psaItems[i] );

    vMountItemsData.push_back( psMountItems );
}

void CMountWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
    Close();
}

void CMountWindowHandler::OnButtonMountClick( int iID, EMountType eMountType, UIEventArgs eArgs )
{
    static UINT uTimeMountButton = 0;
    if ( (uTimeMountButton + 2000) < TICKCOUNT )
    {
        OnButtonSelectMountWindow( iID, eMountType, eArgs );

        MOUNTHANDLER->Mount();

        uTimeMountButton = TICKCOUNT;
    }
    else
    {
        int iSeconds = ((uTimeMountButton + 2000) - TICKCOUNT) / 1000;
        TITLEBOX( "You must wait %d second%s", iSeconds, iSeconds > 1 ? "s" : "" );
    }
}

void CMountWindowHandler::OnButtonSelectMountWindow( int iID, EMountType eMountType, UIEventArgs eArgs )
{
    if ( iCurrentMountWindowID != -1 )
    {
        if ( auto pWindowOld = pWindow1->GetElement<UI::List>( LIST_Mounts )->GetWindow( iCurrentMountWindowID ) )
        {
            pWindowOld->GetElement<UI::ImageBox>( IMAGEBOX_MountBoxSelected )->Hide();
            pWindowOld->GetElement<UI::Text>( TEXT_MountName )->SetColor( DX::Color( 255, 40, 40, 40 ).Get() );
        }

        for ( auto psMountItems : vMountItemsData )
        {
            if ( psMountItems->iMountID == (iCurrentMountWindowID - WINDOW_Mounts) )
            {
                psMountItems->bIsSelected = FALSE;
                break;
            }
        }
    }

    if ( auto pWindowSelected = pWindow1->GetElement<UI::List>( LIST_Mounts )->GetWindow( iID ) )
    {
        pWindowSelected->GetElement<UI::ImageBox>( IMAGEBOX_MountBoxSelected )->Show();
        pWindowSelected->GetElement<UI::Text>( TEXT_MountName )->SetColor( DX::Color( 255, 255, 255, 255 ).Get() );
    }

    iCurrentMountWindowID = iID;

    for ( auto psMountItems : vMountItemsData )
    {
        if ( psMountItems->iMountID == (iCurrentMountWindowID - WINDOW_Mounts) )
        {
            for ( int i = 0; i < psMountItems->vItems.size(); i++ )
            {
                UI::ItemBox_ptr pItemBox1 = pWindow1->GetElement<UI::ItemBox>( ITEMBOX_Item1 + i );
                pItemBox1->ClearItems();

                ItemData cItemData;
                if ( ITEMHANDLER->LoadItemImage( &psMountItems->vItems[i], &cItemData ) )
                {
                    pItemBox1->ResetLastRender();

                    pItemBox1->SetNoCheckSlot( TRUE );

                    pItemBox1->AddItem( &cItemData );

                    pItemBox1->SetNoCheckSlot( FALSE );
                }
            }

            psMountItems->bIsSelected = TRUE;

            eCurrentMountType = eMountType;

            break;
        }
    }
}

void CMountWindowHandler::OnGetItemSlot( int iImageID, int iItemBoxID, UIEventArgs eArgs )
{
    pWindow1->GetElement<UI::ImageBox>( iImageID )->Show();
}

void CMountWindowHandler::OnPutItemSlot( int iImageID, int iItemBoxID, UIEventArgs eArgs )
{
    pWindow1->GetElement<UI::ImageBox>( iImageID )->Hide();

    for ( auto psMountItems : vMountItemsData )
    {
        if ( psMountItems->iMountID == (iCurrentMountWindowID - WINDOW_Mounts) )
        {
            UI::ItemBox_ptr pItemBox1 = pWindow1->GetElement<UI::ItemBox>( iItemBoxID );
            
            //Add Item
            psMountItems->vItems.push_back( pItemBox1->GetItems()[0]->sItem );

            //Reorder
            ReorderItems( psMountItems );
            break;
        }
    }
}

void CMountWindowHandler::ReorderItems( MountItemGame * psMountItemsData )
{
    for ( auto it = psMountItemsData->vItems.begin(); it != psMountItemsData->vItems.end(); )
    {
        auto & sItem = (*it);
        if ( sItem.sItemID.ToItemID() == ITEMID_None )
            it = psMountItemsData->vItems.erase( it );
        else
            it++;
    }
}
