#include "stdafx.h"
#include "CTeleportCoreHandler.h"


CTeleportCoreHandler::CTeleportCoreHandler()
{

}


CTeleportCoreHandler::~CTeleportCoreHandler()
{
	pWindow.reset();
}

void CTeleportCoreHandler::Init()
{
	BuildWindow();
}

BOOL CTeleportCoreHandler::OnMouseClick( CMouse * pcMouse )
{
	return pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CTeleportCoreHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CTeleportCoreHandler::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CTeleportCoreHandler::OnResolutionChanged()
{
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

	pWindow->SetPosition( iWindowX, iWindowY );
}

void CTeleportCoreHandler::Render()
{
	pWindow->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CTeleportCoreHandler::Update( float fTime )
{
	pWindow->Update( fTime );
}

void CTeleportCoreHandler::Open( ItemData * pcItemData )
{
	if ( pWindow->IsOpen() == FALSE )
	{
		this->iItemID = pcItemData->sItem.sItemID.ToItemID();
		this->iChk1 = pcItemData->sItem.iChk1;
		this->iChk2 = pcItemData->sItem.iChk2;

		for ( int i = 0; i < _countof( saTeleportMapInfo ); i++ )
		{
			if ( saTeleportMapInfo[i].bEnabled )
			{
				auto pWindowMap = pWindow->GetElement<UI::List>( LISTID_Map )->GetWindow( WINDOWID_Map + i );

				if ( UNITDATA->sCharacterData.iLevel >= MAPLEVEL( saTeleportMapInfo[i].iMapID ) )
				{
					pWindowMap->GetElement<UI::Text>( TEXTID_MapNameList + i )->SetColor( D3DCOLOR_XRGB( 255, 255, 255 ) );
					pWindowMap->GetElement<UI::Text>( TEXTID_MapLevelList + i )->SetColor( D3DCOLOR_XRGB( 235, 255, 255 ) );
				}
				else
				{
					pWindowMap->GetElement<UI::Text>( TEXTID_MapNameList + i )->SetColor( D3DCOLOR_XRGB( 235, 0, 0 ) );
					pWindowMap->GetElement<UI::Text>( TEXTID_MapLevelList + i )->SetColor( D3DCOLOR_XRGB( 235, 0, 0 ) );
				}
			}
		}

		pWindow->Show();
		MOUSEHANDLER->BlockMouse();
	}
}

void CTeleportCoreHandler::Close()
{
	pWindow->GetElement<UI::Text>( TEXTID_MapName )->SetText( "" );
	pWindow->GetElement<UI::Text>( TEXTID_MapType )->SetText( "" );
	pWindow->Hide();
	MOUSEHANDLER->UnblockMouse();

	pWindow->GetElement<UI::Button>( BUTTONID_Teleport )->Hide();

	pWindow->GetElement<UI::ImageBox>( IMAGEID_Minimap )->Hide();

	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapTown )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapEXP )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapHunt )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapPVP )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapSolo )->Hide();

	pWindow->GetElement<UI::Text>( TEXTID_MapLevelText )->Hide();
	pWindow->GetElement<UI::Text>( TEXTID_MapLevel )->Hide();

	iMapID = -1;
}

BOOL CTeleportCoreHandler::IsOpen()
{
	return pWindow->IsOpen();
}

void CTeleportCoreHandler::BuildWindow()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 364, 374 ) );

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 2) - (iWindowWidth >> 1) + 30;
	int iWindowY = (RESOLUTION_HEIGHT >> 2) - (iWindowHeight >> 1) + 78;

	pWindow->SetPosition( iWindowX, iWindowY );
	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window.png" ) );

	//Header
	UI::ImageBox_ptr pHeaderBG = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (181 >> 1), -13, 181, 32 ) );
	pHeaderBG->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header.png" ) );
	pWindow->AddElement( pHeaderBG );

	//Header Title
	UI::ImageBox_ptr pHeaderTitle = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1) - (61 >> 1), -4, 61, 15 ) );
	pHeaderTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_teleport.png" ) );
	pWindow->AddElement( pHeaderTitle );
	
	//Vertical Separator
	UI::ImageBox_ptr pVerticalSeparator = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindow->GetWidth() >> 1), 26, 1, 312 ) );
	pVerticalSeparator->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\separator_teleport_v.png" ) );
	pWindow->AddElement( pVerticalSeparator );

	//Minimap Image
	UI::ImageBox_ptr pMinimapImage = std::make_shared<UI::ImageBox>( Rectangle2D( 186, 44, 162, 162 ) );
	pMinimapImage->SetID( IMAGEID_Minimap );
	pMinimapImage->SetScale( 0.85f );
	pMinimapImage->Hide();
	pWindow->AddElement( pMinimapImage );

	//Map Name
	UI::Text_ptr pTextMapName = std::make_shared<UI::Text>( Rectangle2D( 186, 190, 158, 18 ) );
	pTextMapName->SetID( TEXTID_MapName );
	pTextMapName->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextMapName->SetHorizontalAlign( EAlign::ALIGN_Center );
	pTextMapName->SetVerticalAlign( EAlign::ALIGN_Middle );
	pTextMapName->SetMultiLine( TRUE );
	pTextMapName->SetWordWrap( TRUE );
	pTextMapName->SetText( "" );
	pWindow->AddElement( pTextMapName );

	//Map Level Text
	UI::Text_ptr pTextMapLevelText = std::make_shared<UI::Text>( Rectangle2D( 194, 220, 162, 18 ) );
	pTextMapLevelText->SetID( TEXTID_MapLevelText );
	pTextMapLevelText->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextMapLevelText->SetText( "" );
	pWindow->AddElement( pTextMapLevelText );

	//Map Level
	UI::Text_ptr pTextMapLevel = std::make_shared<UI::Text>( Rectangle2D( 268, 220, 162, 18 ) );
	pTextMapLevel->SetID( TEXTID_MapLevel );
	pTextMapLevel->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
	pTextMapLevel->SetText( "" );
	pWindow->AddElement( pTextMapLevel );

	//Map Type
	UI::Text_ptr pTextMapType = std::make_shared<UI::Text>( Rectangle2D( 194, 240, 162, 18 ) );
	pTextMapType->SetID( TEXTID_MapType );
	pTextMapType->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextMapType->SetText( "" );
	pWindow->AddElement( pTextMapType );

	//Map Type Town
	UI::ImageBox_ptr pTownTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 194, 260, 40, 13 ) );
	pTownTypeImage->SetID( GetMapTypeImageID( TELEPORTINFOTYPE_Town ) );
	pTownTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( TELEPORTINFOTYPE_Town ) ) );
	pTownTypeImage->Hide();
	pWindow->AddElement( pTownTypeImage );

	//Map Type EXP
	UI::ImageBox_ptr pEXPTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 194, 260, 40, 13 ) );
	pEXPTypeImage->SetID( GetMapTypeImageID( TELEPORTINFOTYPE_EXP ) );
	pEXPTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( TELEPORTINFOTYPE_EXP ) ) );
	pEXPTypeImage->Hide();
	pWindow->AddElement( pEXPTypeImage );

	//Map Type Hunt
	UI::ImageBox_ptr pHuntTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 194, 260, 40, 13 ) );
	pHuntTypeImage->SetID( GetMapTypeImageID( TELEPORTINFOTYPE_Hunt ) );
	pHuntTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( TELEPORTINFOTYPE_Hunt ) ) );
	pHuntTypeImage->Hide();
	pWindow->AddElement( pHuntTypeImage );

	//Map Type PVP
	UI::ImageBox_ptr pPVPTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 194, 260, 40, 13 ) );
	pPVPTypeImage->SetID( GetMapTypeImageID( TELEPORTINFOTYPE_PVP ) );
	pPVPTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( TELEPORTINFOTYPE_PVP ) ) );
	pPVPTypeImage->Hide();
	pWindow->AddElement( pPVPTypeImage );

	//Map Type Solo
	UI::ImageBox_ptr pSoloTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 194, 260, 40, 13 ) );
	pSoloTypeImage->SetID( GetMapTypeImageID( TELEPORTINFOTYPE_Solo ) );
	pSoloTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( TELEPORTINFOTYPE_Solo ) ) );
	pSoloTypeImage->Hide();
	pWindow->AddElement( pSoloTypeImage );

	//Map Type Mana
	UI::ImageBox_ptr pManaTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 194, 260, 40, 13 ) );
	pManaTypeImage->SetID( GetMapTypeImageID( TELEPORTINFOTYPE_Mana ) );
	pManaTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( TELEPORTINFOTYPE_Mana ) ) );
	pManaTypeImage->Hide();
	pWindow->AddElement( pManaTypeImage );

	//Teleport Button
	UI::Button_ptr pButtonTeleport = std::make_shared<UI::Button>( Rectangle2D( (pWindow->GetWidth() >> 1) + 6, 286, 153, 29 ) );
	pButtonTeleport->SetID( BUTTONID_Teleport );
	pButtonTeleport->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\events\\teleport\\btnteleport.png", "game\\images\\events\\teleport\\btnteleport_.png" ) );
	pButtonTeleport->SetEvent( UI::Event::Build( std::bind( &CTeleportCoreHandler::OnClickTeleport, this, std::placeholders::_1 ) ) );
	pButtonTeleport->Hide();
	pWindow->AddElement( pButtonTeleport );

	UI::List_ptr pListMap = std::make_shared<UI::List>( Rectangle2D( 16, 27, 166, 310 ) );
	pListMap->SetID( LISTID_Map );
	pListMap->MakeScrollBar( Rectangle2D( 158, 4, 6, 300 ), "game\\images\\UI\\scroll\\barbg_v.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pListMap->SetPaddingSeparator( 1 );

	//Set Map Info Data
	int iMapCount = 0;
	for ( int i = 0; i < _countof( saTeleportMapInfo ); i++ )
	{
		if ( saTeleportMapInfo[i].bEnabled )
		{
			UI::Window_ptr pWindowMapName = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 166, 32 ) );
			pWindowMapName->SetID( WINDOWID_Map + i );
			
			//Map Type Solo
			UI::ImageBox_ptr pMapTypeImage = std::make_shared<UI::ImageBox>( Rectangle2D( 2, (28 >> 1) - (13 >> 1), 40, 13 ) );
			pMapTypeImage->SetImage( UI::ImageLoader::LoadImage( GetMapTypeImageFile( saTeleportMapInfo[i].iMapTypeFirst ) ) );
			pWindowMapName->AddElement( pMapTypeImage );

			UI::Text_ptr pTextMapName = std::make_shared<UI::Text>( Rectangle2D( 42, 0, 90, 28 ) );
			pTextMapName->SetID( TEXTID_MapNameList + i );
			pTextMapName->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
			pTextMapName->SetHorizontalAlign( EAlign::ALIGN_Center );
			pTextMapName->SetVerticalAlign( EAlign::ALIGN_Middle );
			pTextMapName->SetMultiLine( TRUE );
			pTextMapName->SetWordWrap( TRUE );
			pTextMapName->SetTextEx( "%s", pszaMapsName[saTeleportMapInfo[i].iMapID] );
			pTextMapName->SetColorHover( D3DCOLOR_XRGB( 255, 165, 0 ) );
			pTextMapName->SetEvent( UI::Event::Build( std::bind( &CTeleportCoreHandler::OnClickMap, this, saTeleportMapInfo[i].iMapID, i, std::placeholders::_1 ) ) );
			pWindowMapName->AddElement( pTextMapName );

			UI::Text_ptr pTextMapLevel = std::make_shared<UI::Text>( Rectangle2D( 134, 0, 22, 28 ) );
			pTextMapLevel->SetID( TEXTID_MapLevelList + i );
			pTextMapLevel->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
			pTextMapLevel->SetHorizontalAlign( EAlign::ALIGN_Center );
			pTextMapLevel->SetVerticalAlign( EAlign::ALIGN_Middle );
			pTextMapLevel->SetTextEx( "%d", MAPLEVEL( saTeleportMapInfo[i].iMapID ) );
			pWindowMapName->AddElement( pTextMapLevel );

			UI::ImageBox_ptr pHorizontalSeparator = std::make_shared<UI::ImageBox>( Rectangle2D( 0, 31, 165, 1 ) );
			pHorizontalSeparator->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\separator_teleport_h.png" ) );
			pWindowMapName->AddElement( pHorizontalSeparator );

			pListMap->AddWindow( pWindowMapName );

			iMapCount++;
		}
	}

	pWindow->AddElement( pListMap );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 136, 342, 93, 23 ) );
	pButtonClose->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\btnclose.png" ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\window\\btnclose.png", "game\\images\\UI\\window\\btnclose_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CTeleportCoreHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );


	pWindow->Hide();
}

void CTeleportCoreHandler::OnClickMap( int iMapID, int iIndex, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->IsOpen() )
		return;

	pWindow->GetElement<UI::ImageBox>( IMAGEID_Minimap )->SetImage( UI::ImageLoader::LoadImage( MAPGAME->GetBaseMapGame()[iMapID].szMiniMapFilePath ) );
	pWindow->GetElement<UI::ImageBox>( IMAGEID_Minimap )->Show();
	this->iMapID = iMapID;

	pWindow->GetElement<UI::Text>( TEXTID_MapName )->SetTextEx( "%s", pszaMapsName[iMapID] );

	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapTown )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapEXP )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapHunt )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapPVP )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapSolo )->Hide();
	pWindow->GetElement<UI::ImageBox>( IMAGEID_MapMana )->Hide();

	//Compute Map Type Position
	pWindow->GetElement<UI::ImageBox>( GetMapTypeImageID( saTeleportMapInfo[iIndex].iMapTypeFirst ) )->SetPosition( 194, 260 );
	pWindow->GetElement<UI::ImageBox>( GetMapTypeImageID( saTeleportMapInfo[iIndex].iMapTypeFirst ) )->Show();

	pWindow->GetElement<UI::Button>( BUTTONID_Teleport )->Show();

	pWindow->GetElement<UI::Text>( TEXTID_MapType )->SetText( "Map Type:" );
	pWindow->GetElement<UI::Text>( TEXTID_MapLevelText )->SetText( "Map Level:" );
	pWindow->GetElement<UI::Text>( TEXTID_MapLevel )->SetTextEx( "%d", MAPLEVEL( iMapID ) );
}

void CTeleportCoreHandler::OnClickTeleport( UIEventArgs eArgs )
{
	if ( UNITDATA->sCharacterData.iLevel >= MAPLEVEL( iMapID ) )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( pszaMapsName[iMapID] );
		MESSAGEBOX->SetDescription( "Do you want to teleport?" );
		MESSAGEBOX->SetEvent( std::bind( &CTeleportCoreHandler::OnAcceptTeleport, this, iMapID, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
	else
		TITLEBOX( "You must be level %d or higher to teleport!", MAPLEVEL( iMapID ) );
}

void CTeleportCoreHandler::OnAcceptTeleport( int iMapID, UIEventArgs eArgs )
{
	if ( MESSAGEBOX->GetType() )
	{
		ItemData * pcItemData = ITEMHANDLER->GetItemInventory( iItemID, iChk1, iChk2 );
		if ( pcItemData )
		{
			ITEMHANDLER->DropSpecialItem( pcItemData );

			TELEPORTPLAYER( iMapID );

			RESTARTCOUNTER = 70 * 10;
		}

		Close();
	}
}

void CTeleportCoreHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	Close();
}

int CTeleportCoreHandler::GetMapTypeImageID( ETeleportMapInfoType iType )
{
	if ( iType == TELEPORTINFOTYPE_EXP )
		return IMAGEID_MapEXP;
	else if ( iType == TELEPORTINFOTYPE_Hunt )
		return IMAGEID_MapHunt;
	else if ( iType == TELEPORTINFOTYPE_PVP )
		return IMAGEID_MapPVP;
	else if ( iType == TELEPORTINFOTYPE_Solo )
		return IMAGEID_MapSolo;
	else if ( iType == TELEPORTINFOTYPE_Mana )
		return IMAGEID_MapMana;

	return IMAGEID_MapTown;
}

const char * CTeleportCoreHandler::GetMapTypeImageFile( ETeleportMapInfoType iType )
{
	if ( iType == TELEPORTINFOTYPE_EXP )
		return "game\\images\\events\\teleport\\map_exptype.png";
	else if ( iType == TELEPORTINFOTYPE_Hunt )
		return "game\\images\\events\\teleport\\map_hunttype.png";
	else if ( iType == TELEPORTINFOTYPE_PVP )
		return "game\\images\\events\\teleport\\map_pvptype.png";
	else if ( iType == TELEPORTINFOTYPE_Solo )
		return "game\\images\\events\\teleport\\map_solotype.png";
	else if ( iType == TELEPORTINFOTYPE_Mana )
		return "game\\images\\events\\teleport\\map_manatype.png";

	return "game\\images\\events\\teleport\\map_towntype.png";
}
