#include "stdafx.h"
#include "CAchievementWindow.h"


CAchievementWindow::CAchievementWindow()
{
}


CAchievementWindow::~CAchievementWindow()
{
}

void CAchievementWindow::Init()
{
	BuildWindow();
	BuildWindowHover();
	BuildWindowList();

	UpdateWindowPosition();
}

void CAchievementWindow::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pWindowHover->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pWindowList->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CAchievementWindow::Update( float fTime )
{
	UpdateWindowPosition();

	if ( fAchievementTransitionTime > 0.0f )
	{
		fAchievementTransitionTime -= fTime;

		int iAlphaCurrent	= ((int)fAchievementTransitionTime * 255) / MS_TIME_TRANSITION_ACHIEVEMENT;
		int iAlphaNext		= 255 - (((int)fAchievementTransitionTime * 255) / MS_TIME_TRANSITION_ACHIEVEMENT);

		SetAlphaAchievementWindowID( uCurrentAchievementID, iAlphaCurrent );
		SetAlphaAchievementWindowID( uNextAchievementID, iAlphaNext );

		if ( fAchievementTransitionTime < 0.0f )
		{
			SetAlphaAchievementWindowID( uCurrentAchievementID, 0 );
			SetAlphaAchievementWindowID( uNextAchievementID, 255 );

			uCurrentAchievementID = uNextAchievementID;

			fAchievementTransitionTime = 0.0f;
		}
	}
}

BOOL CAchievementWindow::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( pWindowHover->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( pWindowList->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( pWindowList->IsOpen() )
	{
		if ( pWindowList->GetRect().Inside( pcMouse->GetPosition() ) == FALSE )
		{
			pWindowList->Hide();
			pWindowHover->Hide();
		}
	}

	return FALSE;
}

BOOL CAchievementWindow::OnMouseScroll( CMouse * pcMouse )
{
	if ( pWindow->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( pWindowHover->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( pWindowList->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	return FALSE;
}

void CAchievementWindow::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindowHover->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindowList->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

void CAchievementWindow::SetAchievementViewHover( CAchievement * pcAchievemenent )
{
	auto pImageLogo = pWindowHover->GetElement<UI::ImageBox>( IMAGEID_LogoAchievement );
	pImageLogo->SetImage( UI::ImageLoader::LoadImage( pcAchievemenent->GetImagePath() ) );

	auto pTextName = pWindowHover->GetElement<UI::Text>( TEXTID_NameAchievement );
	pTextName->SetText( pcAchievemenent->GetName() );

	auto pTextDescription = pWindowHover->GetElement<UI::Text>( TEXTID_DescriptionAchievement );
	pTextDescription->SetText( pcAchievemenent->GetDescription() );

}

void CAchievementWindow::AddAchievement( CAchievement * pcAchievement, Unit * pcUnit )
{
	std::string strName			= pcAchievement->GetName();
	std::string strDescription	= pcAchievement->GetDescription();

	if ( pcUnit )
	{
		if ( (pcAchievement->GetType() == ACHIEVEMENTTYPE_BattleRoyaleLastStandingT) || (pcAchievement->GetType() == ACHIEVEMENTTYPE_BattleRoyaleLastStandingM) )
		{
			switch ( pcUnit->pcUnitData->sCharacterData.iClass )
			{
				case CHARACTERCLASS_Fighter:
					strName			= "Fire Fist";
					strDescription	= "Be the last one standing on Battle Royale as a Fighter";
					break;
				case CHARACTERCLASS_Mechanician:
					strName			= "Master Tactician";
					strDescription	= "Be the last one standing on Battle Royale as a Mechanician";
					break;
				case CHARACTERCLASS_Archer:
					strName			= "Master Tactician";
					strDescription	= "Be the last one standing on Battle Royale as an Archer";
					break;
				case CHARACTERCLASS_Pikeman:
					strName			= "The Terror";
					strDescription	= "Be the last one standing on Battle Royale as a Pikeman";
					break;
				case CHARACTERCLASS_Knight:
					strName			= "Cavalier";
					strDescription	= "Be the last one standing on Battle Royale as a Knight";
					break;
				case CHARACTERCLASS_Atalanta:
					strName			= "Head Hunter";
					strDescription	= "Be the last one standing on Battle Royale as an Atalanta";
					break;
				case CHARACTERCLASS_Magician:
					strName			= "The Rogue Mage";
					strDescription	= "Be the last one standing on Battle Royale as a Magician";
					break;
				case CHARACTERCLASS_Priestess:
					strName			= "Enchantress";
					strDescription	= "Be the last one standing on Battle Royale as a Priestess";
					break;
				case CHARACTERCLASS_Assassin:
					strName			= "Fists of Shadow";
					strDescription	= "Be the last one standing on Battle Royale as an Assassin";
					break;
				case CHARACTERCLASS_Shaman:
					strName			= "Spirit Warden";
					strDescription	= "Be the last one standing on Battle Royale as a Shaman";
					break;

				default:
					break;
			}
		}
	}

	auto pList = pWindowList->GetElement<UI::List>( LISTID_ListAchievement );

	UI::Window_ptr pWindowAchievement = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 244, 220 ) );

	auto pImageAchievement = UI::ImageLoader::LoadImage( pcAchievement->GetImagePath() );

	UI::ImageBox_ptr pImage = std::make_shared<UI::ImageBox>( Rectangle2D( (pWindowAchievement->GetWidth() >> 1) - (pImageAchievement->GetWidth() >> 1), 0, pImageAchievement->GetWidth(), pImageAchievement->GetHeight() ) );
	pImage->SetID( IMAGEID_LogoAchievement );
	pImage->SetImage( pImageAchievement );
	pWindowAchievement->AddElement( pImage );

	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 0, pImageAchievement->GetHeight() + 10, 244, 36 ) );
	pTextName->SetID( TEXTID_NameAchievement );
	pTextName->SetFont( "Frutiger LT 55 Roman", 24, 0, FALSE, TRUE, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	pTextName->SetText( strName );
	pTextName->SetHorizontalAlign( ALIGN_Center );
	pWindowAchievement->AddElement( pTextName );

	UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 0, pTextName->GetY() + pTextName->GetHeight() + 10, 244, 42 ) );
	pTextDescription->SetID( TEXTID_DescriptionAchievement );
	pTextDescription->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
	pTextDescription->SetText( strDescription );
	pTextDescription->SetHorizontalAlign( ALIGN_Center );
	pTextDescription->SetMultiLine( TRUE );
	pTextDescription->SetWordWrap( TRUE );
	pWindowAchievement->AddElement( pTextDescription );

	pList->AddWindow( pWindowAchievement );

	if ( pList->GetCountItemsList() > 1 )
		pWindowAchievement->Hide();
}

void CAchievementWindow::OnResolutionChanged()
{
	//Hud Button
	{
		int iX = RESOLUTION_WIDTH - 36;
		int iY = RESOLUTION_HEIGHT - 300;

		pWindow->SetPosition( iX, iY );
	}

	UpdateWindowPosition();
}

void CAchievementWindow::ClearAchievementsList()
{
	auto pList = pWindowList->GetElement<UI::List>( LISTID_ListAchievement );
	pList->Clear();

	uCurrentAchievementID		= 0;
	uNextAchievementID			= 0;
	fAchievementTransitionTime	= 0.0f;
}

void CAchievementWindow::SetWindowType( EAchievementWindow iType )
{
	iAchievementWindow = iType;

	UpdateWindowPosition();
}

void CAchievementWindow::UpdateWindowPosition()
{
	//List
	if( pWindowList )
	{
		int iX = RESOLUTION_WIDTH - pWindowList->GetWidth() - 38;
		int iY = pWindow->GetY() - (pWindowList->GetHeight() >> 1);

		if ( iAchievementWindow == ACHIEVEMENTWINDOW_Target )
		{
			iX = HUDHANDLER->GetTargetWindow()->GetX();
			iY = HUDHANDLER->GetTargetWindow()->GetY() + HUDHANDLER->GetTargetWindow()->GetHeight() + 20;
		}

		pWindowList->SetPosition( iX, iY );
	}

	//Hover
	if ( pWindowHover )
	{
		int iX = pWindowList->GetX();
		int iY = pWindowList->GetY() - pWindowList->GetHeight() - 20;

		if ( iAchievementWindow == ACHIEVEMENTWINDOW_Target )
		{
			iX = HUDHANDLER->GetTargetWindow()->GetX();
			iY = pWindowList->GetY() + pWindowList->GetHeight() + 20;
		}

		pWindowHover->SetPosition( iX, iY );
	}
}

void CAchievementWindow::UpdateWindowList( Unit * pcUnit, EAchievementWindow iType )
{
	if ( pWindowList->IsOpen() == FALSE )
	{
		SetWindowType( iType );
		ACHIEVEMENTHANDLER->AddWindowAchievementList( pcUnit );
		pWindowList->Show();
	}
	else
	{
		pWindowList->Hide();
		pWindowHover->Hide();
	}
}

void CAchievementWindow::BuildWindow()
{
	int iX = RESOLUTION_WIDTH - 36;
	int iY = RESOLUTION_HEIGHT - 300;

	pWindow = std::make_shared<UI::Window>( Rectangle2D( iX, iY, 32, 32 ) );
	UI::Button_ptr pButton = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 32, 32 ) );
	pButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\achievements\\achievement_btn.png", "game\\images\\achievements\\achievement_btn.png" ) );
	pButton->SetEvent( UI::Event::Build( std::bind( &CAchievementWindow::OnButtonAchievementListOpenClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButton );

	UI::Tooltip_ptr pTooltip = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltip->Init();
	pTooltip->SetLinkObject( pButton );
	pTooltip->SetAddAxis( -84, 6 );
	pTooltip->SetFont( "Arial", 16, -1 );
	pTooltip->SetPosition( TOOLTIPPOSITION_Left );
	pTooltip->SetFollowMouse( FALSE );
	pTooltip->SetText( "Achievements" );
	pWindow->AddElement( pTooltip );
}

void CAchievementWindow::BuildWindowHover()
{
	pWindowHover = std::make_shared<UI::Window>( Rectangle2D( 10, 80, 320, 180 ) );
	
	UI::RectangleBox_ptr pRectangleBox = std::make_shared<UI::RectangleBox>( Rectangle2D( 0, 0, 320, 180 ) );
	pRectangleBox->SetID( 10 );
	pRectangleBox->SetColor( DX::Color( 235, 45, 39, 27 ), DX::Color( 245, 9, 23, 22 ) );
	pWindowHover->AddElement( pRectangleBox );

	UI::ImageBox_ptr pBackground = std::make_shared<UI::ImageBox>( Rectangle2D( 60, 0, 260, 160 ) );
	pBackground->SetImage( UI::ImageLoader::LoadImage( "game\\images\\achievements\\background_smoke.png" ) );
	pWindowHover->AddElement( pBackground );

	UI::ImageBox_ptr pImageLogo = std::make_shared<UI::ImageBox>( Rectangle2D( 20, 20, 64, 64 ) );
	pImageLogo->SetID( IMAGEID_LogoAchievement );
	pWindowHover->AddElement( pImageLogo );

	UI::Text_ptr pTextName = std::make_shared<UI::Text>( Rectangle2D( 90, 40, 200, 36 ) );
	pTextName->SetID( TEXTID_NameAchievement );
	pTextName->SetFont( "Frutiger LT 55 Roman", 24, 0, FALSE, TRUE, -1 );
	pTextName->SetText( "Legacy Member" );
	pTextName->SetDropdownShadow( TRUE );
	pTextName->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowHover->AddElement( pTextName );

	UI::Text_ptr pTextDescription = std::make_shared<UI::Text>( Rectangle2D( 20, 90, 280, 36 ) );
	pTextDescription->SetID( TEXTID_DescriptionAchievement );
	pTextDescription->SetFont( "Frutiger LT 55 Roman", 14, 0, FALSE, FALSE, -1 );
	pTextDescription->SetText( "Legacy Member description achievement. BLA BLA BLA BLA BLA" );
	pTextDescription->SetMultiLine( TRUE );
	pTextDescription->SetWordWrap( TRUE );
	pWindowHover->AddElement( pTextDescription );

	pWindowHover->Hide();
}

void CAchievementWindow::BuildWindowList()
{
	pWindowList = std::make_shared<UI::Window>( Rectangle2D( 10, 10, 320, 240 ) );

	UI::RectangleBox_ptr pRectangleBox = std::make_shared<UI::RectangleBox>( Rectangle2D( 0, 0, 320, 240 ) );
	pRectangleBox->SetID( 10 );
	pRectangleBox->SetColor( DX::Color( 235, 45, 39, 27 ), DX::Color( 245, 9, 23, 22 ) );
	pWindowList->AddElement( pRectangleBox );

	UI::List_ptr pList = std::make_shared<UI::List>( Rectangle2D( 38, 0, 244, 220 ) );
	pList->SetID( LISTID_ListAchievement );
	pList->SetNoUpdatePosition( TRUE );

	//Left
	UI::Button_ptr pLeftButton = std::make_shared<UI::Button>( Rectangle2D( 6, (pWindowList->GetHeight() >> 1) - 26, 32, 32 ) );
	pLeftButton->SetID( BUTTON_Left );
	pLeftButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\achievements\\leftarrow.png", "game\\images\\achievements\\leftarrow_.png" ) );
	pLeftButton->SetEvent( UI::Event::Build( std::bind( &CAchievementWindow::OnButtonAchievementLeftClick, this, std::placeholders::_1 ) ) );
	pWindowList->AddElement( pLeftButton );

	//Right
	UI::Button_ptr pRightButton = std::make_shared<UI::Button>( Rectangle2D( pWindowList->GetWidth() - 38, (pWindowList->GetHeight() >> 1) - 26, 32, 32 ) );
	pRightButton->SetID( BUTTON_Right );
	pRightButton->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\achievements\\rightarrow.png", "game\\images\\achievements\\rightarrow_.png" ) );
	pRightButton->SetEvent( UI::Event::Build( std::bind( &CAchievementWindow::OnButtonAchievementRightClick, this, std::placeholders::_1 ) ) );
	pWindowList->AddElement( pRightButton );

	pWindowList->AddElement( pList );
	pWindowList->Hide();
}

void CAchievementWindow::OnButtonAchievementListOpenClick( UIEventArgs eArgs )
{
	UpdateWindowList( UNIT, ACHIEVEMENTWINDOW_User );
}

void CAchievementWindow::OnButtonAchievementListClick( CAchievement * pcAchievement, UIEventArgs eArgs )
{
	SetAchievementViewHover( pcAchievement );
	pWindowHover->Show();
}

void CAchievementWindow::OnButtonAchievementLeftClick( UIEventArgs eArgs )
{
	auto pList = pWindowList->GetElement<UI::List>( LISTID_ListAchievement );
	if ( pList->GetCountItemsList() > 1 )
	{
		uNextAchievementID = (uCurrentAchievementID - 1) % pList->GetCountItemsList();
		fAchievementTransitionTime = (float)MS_TIME_TRANSITION_ACHIEVEMENT;
	}
}

void CAchievementWindow::OnButtonAchievementRightClick( UIEventArgs eArgs )
{
	auto pList = pWindowList->GetElement<UI::List>( LISTID_ListAchievement );
	if ( pList->GetCountItemsList() > 1 )
	{
		uNextAchievementID = (uCurrentAchievementID + 1) % pList->GetCountItemsList();
		fAchievementTransitionTime = (float)MS_TIME_TRANSITION_ACHIEVEMENT;
	}
}

void CAchievementWindow::SetAlphaAchievementWindowID( UINT uWindowID, int iAlpha )
{
	auto pList = pWindowList->GetElement<UI::List>( LISTID_ListAchievement );
	if ( (pList->GetCountItemsList() > 0) && (uWindowID < (UINT)pList->GetCountItemsList()) )
	{
		auto pWindowAchievement = pList->GetWindowList()[uWindowID];
		pWindowAchievement->SetPosition( 0, 0 );

		auto pImage = pWindowAchievement->GetElement<UI::ImageBox>( IMAGEID_LogoAchievement );
		pImage->SetColor( D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );

		auto pCurrentTextName = pWindowAchievement->GetElement<UI::Text>( TEXTID_NameAchievement );
		pCurrentTextName->SetColor( D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );

		auto pCurrentTextDescription = pWindowAchievement->GetElement<UI::Text>( TEXTID_DescriptionAchievement );
		pCurrentTextDescription->SetColor( D3DCOLOR_ARGB( iAlpha, 255, 255, 255 ) );

		if ( iAlpha > 0 )
			pWindowAchievement->Show();
		else
			pWindowAchievement->Hide();
	}
}
