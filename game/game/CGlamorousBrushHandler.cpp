#include "stdafx.h"
#include "CGlamorousBrushHandler.h"

CGlamorousBrushHandler::CGlamorousBrushHandler()
{
}

CGlamorousBrushHandler::~CGlamorousBrushHandler()
{
}

void CGlamorousBrushHandler::Init()
{
    pcImageAnimation = UI::ImageLoader::LoadImage( "game\\images\\misc\\brush.png" );
}

bool CGlamorousBrushHandler::IsGlamorousMouse()
{
    return ((MOUSE_ICON == EMouseIcon::EMOUSEICON_GlamorousBrush1) || (MOUSE_ICON == EMouseIcon::EMOUSEICON_GlamorousBrush2));
}

void CGlamorousBrushHandler::OnItemBrush( ItemData * pcItemData )
{
    switch ( pcItemData->sItem.sItemID.ToItemID() )
    {
        //Glamorous Attire Glaze
        case ITEMID_GlamorousAttireGlaze:
            MOUSE_ICON = EMouseIcon::EMOUSEICON_GlamorousBrush1;
            eGlamorousItemID = pcItemData->sItem.sItemID.ToItemID();
            iGlamorousChk1 = pcItemData->sItem.iChk1;
            iGlamorousChk2 = pcItemData->sItem.iChk2;
            break;
        case ITEMID_GlamorousArmamentVarnish:
            MOUSE_ICON = EMouseIcon::EMOUSEICON_GlamorousBrush2;
            eGlamorousItemID = pcItemData->sItem.sItemID.ToItemID();
            iGlamorousChk1 = pcItemData->sItem.iChk1;
            iGlamorousChk2 = pcItemData->sItem.iChk2;
            break;
    }
}

BOOL CGlamorousBrushHandler::OnMouseClick( CMouse * pcMouse )
{
    if ( pcMouse->GetEvent() == EMouseEvent::ClickDownL )
    {
        if ( IsGlamorousMouse() )
        {
            if ( auto pcItemData = ITEMHANDLER->GetMouseItemHover() )
            {
                if ( CGameInventory::GetInstance()->GetInventoryItemIndex( pcItemData ) >= 0 )
                {
                    if ( pcItemData->bValid )
                    {
                        if ( IsValidItem( pcItemData, true ) )
                        {
                            MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
                            MESSAGEBOX->SetTitle( "Glamorize Item" );
                            MESSAGEBOX->SetDescription( "Do you want to turn the item into a skin?" );
                            MESSAGEBOX->SetEvent( std::bind( &CGlamorousBrushHandler::OnAcceptItem, this, pcItemData ) );
                            MESSAGEBOX->SetAutoClose( 10 );
                            MESSAGEBOX->Show();

                            MOUSE_ICON = EMOUSEICON_Default;

                            return TRUE;
                        }
                        else
                            TITLEBOX( AY_OBFUSCATE( "This Item can't be turned into a Skin!" ) );
                    }
                }

                return TRUE;
            }
            else
            {
                MOUSE_ICON = EMOUSEICON_Default;
            }
        }
    }
    return FALSE;
}

void CGlamorousBrushHandler::Update( float fTime )
{
    if ( fTimeWait > 0.0f )
    {
        fTimeWait -= fTime;
        if ( fTimeWait <= 0.0f )
        {
            fTimeWait = 0.0f;

            MOUSEHANDLER->UnblockMouse();
        }
    }

    if ( fTimeAnimation > 0.0f )
    {
        fTimeAnimation -= fTime;

        if ( fTimeAnimation <= 0.0f )
            fTimeAnimation = 0.0f;
    }
}

void CGlamorousBrushHandler::HandlePacket( PacketGlamorousBrush * psPacket )
{
    if ( psPacket->bResult )
    {
        Point2D sPosition;
        Point2D sPlacePosition;

        if ( auto pcItemData = CGameInventory::GetInstance()->GetInventoryItemIndexEx( eItemID, iChk1, iChk2 ) )
        {
            sPosition = pcItemData->sPosition;
            sPlacePosition = pcItemData->sPlacePosition;

            ITEMHANDLER->DeleteItem( pcItemData );
        }
        
        ITEMHANDLER->DeleteItem( CGameInventory::GetInstance()->GetInventoryItemIndexEx( eGlamorousItemID, iGlamorousChk1, iGlamorousChk2 ) );

        ItemData sItemData;
        if ( ITEMHANDLER->LoadItemImage( &psPacket->cItemData.sItem, &sItemData ) )
        {
            ITEMHANDLER->BackItemToInventory( &sItemData, ITEMCHECKINVENTORY_Requirements );
            ITEMHANDLER->ResetInventoryItemChecksum();
            TITLEBOX( "Glamorous Item Sucessful!" );

            fTimeAnimation = 1000.0f;

            if ( auto pcItemData = CGameInventory::GetInstance()->GetInventoryItemIndexEx( sItemData.sItem.sItemID.ToItemID(), sItemData.sItem.iChk1, sItemData.sItem.iChk2 ) )
            {
                pcItemData->sPosition = sPosition;
                pcItemData->sPlacePosition = sPlacePosition;

                sRectangleItem.iX = pcItemData->sPosition.iX;
                sRectangleItem.iY = pcItemData->sPosition.iY;
                sRectangleItem.iWidth = pcItemData->iWidth;
                sRectangleItem.iHeight = pcItemData->iHeight;
            }

            SAVE;
        }
    }
}

void CGlamorousBrushHandler::Render()
{
    if ( fTimeAnimation > 0.0f )
    {
        if ( pcImageAnimation )
        {
            for ( int i = 0; i < 16; i++ )
            {
                int iX = RandomI( sRectangleItem.iX - 22, sRectangleItem.iX + sRectangleItem.iWidth - 22 );
                int iY = RandomI( sRectangleItem.iY - 22, sRectangleItem.iY + sRectangleItem.iHeight - 22 );

                DWORD dwColor = D3DCOLOR_ARGB( (int)X3D::easeInOutCircf( 0.0f, 255.0f, fTimeAnimation / 1000.0f ), 0, 190, 255 );
                if ( eGlamorousItemID == ITEMID_GlamorousArmamentVarnish )
                    dwColor = D3DCOLOR_ARGB( (int)X3D::easeInOutCircf( 0.0f, 255.0f, fTimeAnimation / 1000.0f ), 255, 10, 175 );

                UI::ImageRender::Render( pcImageAnimation, iX, iY + (DX::Graphic::GetHeightBackBuffer() + 56) - WINDOW_YINVENTORYMOVE - 400, 44, 44, 0, 0, dwColor, 1.0f );
            }
        }
    }
}

void CGlamorousBrushHandler::OnAcceptItem( ItemData * pcItemData )
{
    if ( MESSAGEBOX->GetType() )
    {
        fTimeWait = 3000.0f;

        MOUSEHANDLER->BlockMouse();

        PacketGlamorousBrush sPacket;
        sPacket.bResult = FALSE;
        sPacket.eGlamorousItemID = eGlamorousItemID;
        sPacket.iChk1 = iGlamorousChk1;
        sPacket.iChk2 = iGlamorousChk2;
        CopyMemory( &sPacket.cItemData, pcItemData, sizeof( ItemData ) );

        eItemID = pcItemData->sItem.sItemID.ToItemID();
        iChk1 = pcItemData->sItem.iChk1;
        iChk2 = pcItemData->sItem.iChk2;

        ProcessPacket( &sPacket );
    }
}

void CGlamorousBrushHandler::ProcessPacket( PacketGlamorousBrush * psPacket )
{
    psPacket->iLength = sizeof( PacketGlamorousBrush );
    psPacket->iHeader = PKTHDR_GlamorousBrush;
    SENDPACKETL( psPacket );
}

bool CGlamorousBrushHandler::IsValidItem( ItemData * pcItemData, bool bMessage )
{
    bool bRet = false;

    auto IsEquippedItem = []( EItemID eItemID, int iChk1, int iChk2 )-> bool
    {
        for ( int i = 0; i < 14; i++ )
        {
            if ( INVENTORYITEMSLOT[i].iItemIndex )
            {
                auto pcItemData = &INVENTORYITEMS[INVENTORYITEMSLOT[i].iItemIndex - 1];
                if ( pcItemData->bValid )
                {
                    if ( (pcItemData->sItem.sItemID.ToItemID() == eItemID) && (pcItemData->sItem.iChk1 == iChk1) && (pcItemData->sItem.iChk2 == iChk2) )
                        return true;
                }
            }
        }

        return false;
    };

    if ( (pcItemData->sItem.bCostumeItem == 0) && ((pcItemData->iItemSlotFlag != ITEMSLOTFLAG_Costume) && (pcItemData->iItemSlotFlag != ITEMSLOTFLAG_CostumeLeft) &&
        (pcItemData->iItemSlotFlag != ITEMSLOTFLAG_CostumeLeftRight) && (pcItemData->iItemSlotFlag != ITEMSLOTFLAG_CostumeRight)) )
    {
        if ( !IsEquippedItem( pcItemData->sItem.sItemID.ToItemID(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) )
        {
            switch ( eGlamorousItemID )
            {
                case ITEMID_GlamorousAttireGlaze:
                    if ( (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Armor) || (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Robe) )
                        bRet = true;
                    else if ( bMessage )
                        TITLEBOX( AY_OBFUSCATE( "Incorrect Item type! This Brush can turn into Skin only Armors and Robes!" ) );
                    break;
                case ITEMID_GlamorousArmamentVarnish:
                    if ( (pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Weapon) || (pcItemData->sItem.sItemID.ToItemType() == ITEMTYPE_Shield) )
                        bRet = true;
                    else if ( bMessage )
                        TITLEBOX( AY_OBFUSCATE( "Incorrect Item type! This Brush can turn into Skin only Weapons and Shields!" ) );
                    break;
            }
        }
        else if ( bMessage )
            TITLEBOX( AY_OBFUSCATE( "You can't turn equipped items into a Skin!" ) );
    }
    else if ( bMessage )
        TITLEBOX( AY_OBFUSCATE( "This Item is already a Skin!" ) );

    return bRet;
}
