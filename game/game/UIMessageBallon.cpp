#include "stdafx.h"
#include "UIMessageBallon.h"

namespace UI
{
MessageBalloon::MessageBalloon( Rectangle2D rRectangle ) : Element(rRectangle)
{
}

MessageBalloon::~MessageBalloon()
{
	vCache.clear();
}

void MessageBalloon::Init()
{
	//Normal Skin
	saMessagesBalloon[SKINID_Normal][0].pImageLeftTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTopLeft.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTopMiddle.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTopRight.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageLeft.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageCenter		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageCenter.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageRight.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageLeftBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageBottomLeft.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageBottomMiddle.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageRightBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageBottomRight.png" );
	saMessagesBalloon[SKINID_Normal][0].pImageTailBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\MessageTail.png" );

	//Blue Skin
	saMessagesBalloon[SKINID_Blue][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTopLeft.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTopMiddle.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTopRight.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopLeft.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopCenter.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopRight.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopBottomLeft.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopBottomMiddle.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopBottomRight.png" );
	saMessagesBalloon[SKINID_Blue][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\PersonalShopTail.png" );

	//Green Skin
	saMessagesBalloon[SKINID_Green][0].pImageLeftTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopTopLeft.png" );
	saMessagesBalloon[SKINID_Green][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopTopMiddle.png" );
	saMessagesBalloon[SKINID_Green][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopTopRight.png" );
	saMessagesBalloon[SKINID_Green][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopLeft.png" );
	saMessagesBalloon[SKINID_Green][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopCenter.png" );
	saMessagesBalloon[SKINID_Green][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopRight.png" );
	saMessagesBalloon[SKINID_Green][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopBottomLeft.png" );
	saMessagesBalloon[SKINID_Green][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopBottomMiddle.png" );
	saMessagesBalloon[SKINID_Green][0].pImageRightBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopBottomRight.png" );
	saMessagesBalloon[SKINID_Green][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\green\\PersonalShopTail.png" );

	//Pink Skin
	saMessagesBalloon[SKINID_Pink][0].pImageLeftTop			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopTopLeft.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageMiddleTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopTopMiddle.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageRightTop		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopTopRight.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageLeft			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopLeft.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageCenter			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopCenter.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageRight			= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopRight.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageLeftBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopBottomLeft.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageMiddleBottom	= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopBottomMiddle.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageRightBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopBottomRight.png" );
	saMessagesBalloon[SKINID_Pink][0].pImageTailBottom		= ImageLoader::LoadImage( "game\\images\\UI\\balloon\\pink\\PersonalShopTail.png" );

	pcaBellatraCrown[0]										= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_01.tga" );
	pcaBellatraCrown[1]										= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_02.tga" );
	pcaBellatraCrown[2]										= ImageLoader::LoadImage( "StartImage\\MessageBox\\BellaBox\\Bella_03.tga" );

	pTextMessage = std::make_shared<Text>( Rectangle2D( rRect ), FALSE, -1 );
	pTextMessage->SetMaxTextWidth( 200 );
	pTextMessage->SetNoClip( TRUE );
	pTextMessage->SetMultiLine( TRUE );
	pTextMessage->SetFont( FONTNAME_DEFAULT, 16, 6, FALSE, FALSE, -1 );

	pTextClanName = std::make_shared<Text>( Rectangle2D( rRect ), FALSE, D3DCOLOR_XRGB( 150, 255, 250 ) );
	pTextClanName->SetMaxTextWidth( 200 );
	pTextClanName->SetNoClip( TRUE );
	pTextClanName->SetMultiLine( TRUE );
	pTextClanName->SetFont( FONTNAME_DEFAULT, 16, 6, FALSE, FALSE, -1 );

	pTextPetName = std::make_shared<Text>( Rectangle2D( rRect ), FALSE, -1 );
	pTextPetName->SetMaxTextWidth( 200 );
	pTextPetName->SetNoClip( TRUE );
	pTextPetName->SetMultiLine( TRUE );
	pTextPetName->SetFont( FONTNAME_DEFAULT, 16, 6, FALSE, FALSE, -1 );

	iBellatraCrown = -1;

	bHover = FALSE;
	bIsMessage = FALSE;
	bIsCharacterMessage = FALSE;
	bShowClanIcon = FALSE;
	bClanName = FALSE;
	bPetName = FALSE;
	psTextureClanIcon = NULL;
}

void MessageBalloon::SetMessage( std::string strMessage )
{
	if( strMessage.find( ":" ) != std::string::npos )
	{
		if( bShowClanIcon )
			strMessage.insert( 0, "      " );

		bIsMessage = TRUE;
		pTextMessage->SetHighlightText( TRUE );
		pTextMessage->SetHighlightTextColor( D3DCOLOR_XRGB( 240, 180, 100 ) );
	}

	//Get Message
	if ( auto & it = vCache.find( std::hash<std::string>{}(strMessage) ); it != vCache.end() )
	{
		pTextMessage->SetText( it->second.strText, false );

		if ( pTextMessage->GetIsHighlightText() )
		{
			pTextMessage->SetHighlightTextWidthBuild( it->second.iHighlightWidth );

			pTextMessage->SetHighlightTextBuild( strMessage.substr( 0, strMessage.find( ":" ) + 1 ) );
		}

		it->second.dwTimeOut = TICKCOUNT + 15000;
	}
	else
	{
		pTextMessage->SetText( strMessage );

		vCache.insert( std::pair<std::size_t, StringBalloonHash>( std::hash<std::string>{}(strMessage), StringBalloonHash( pTextMessage->GetText(), pTextMessage->GetHighlightTextWidthBuild(), TICKCOUNT ) ) );
	}
}

void MessageBalloon::SetClanName( std::string strText )
{
	strText.insert( 0, "          " );
	pTextClanName->SetText( strText );
	pTextMessage->GetText() = trim( pTextMessage->GetText() );
	bClanName = TRUE;
}

void MessageBalloon::SetPetName( std::string strText, DWORD dwColor )
{
	pTextPetName->SetText( strText );
	pTextPetName->SetColor( dwColor );
	bPetName = TRUE;
}

void MessageBalloon::AddExtraText( int iID, EPositionText ePosition, std::string strText, DWORD dwColor )
{
	ExtraText * psExtraText = new ExtraText;

	Text_ptr pText = std::make_shared<UI::Text>( strText, Rectangle2D( 0, 0, 0, 0 ), FALSE, dwColor );
	pText->SetID( iID );
	pText->SetFont( FONTNAME_DEFAULT, 16, 6, FALSE, FALSE, dwColor );
	psExtraText->ePosition = ePosition;
	psExtraText->pText = pText;
	vExtraText.push_back( psExtraText );
}

void MessageBalloon::ResetMessage()
{
	ResetLastHeight();

	pTextMessage->SetHighlightText( FALSE );
	bHover = FALSE;
	bIsMessage = FALSE;
	bIsCharacterMessage = FALSE;
	bShowClanIcon = FALSE;
	bClanName = FALSE;
	bPetName = FALSE;
	psTextureClanIcon = NULL;
	iBellatraCrown = -1;
	pTextClanName->SetText( "" );
	pTextMessage->SetText( "" );
	pTextPetName->SetText( "" );

	for ( std::vector<ExtraText*>::iterator it = vExtraText.begin(); it != vExtraText.end(); it++ )
	{
		ExtraText * ps = (*it);

		CLEARPTR( ps->pText );
		ps->pText = nullptr;
		DELET( ps );
	}

	vExtraText.clear();
}

void MessageBalloon::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if ( ISSHOW_INTERFACE == FALSE )
		return;

	int iRenderX		= GetX() + iX;
	int iRenderY		= GetY() + iY;
	int iRenderWidth	= low( GetWidth(), iWidth	- GetX() );
	int iRenderHeight	= low( GetHeight(), iHeight - GetY() );

	int iYBase = 0;
	int iXBase = 0;

	if( iRenderX < 0 || iRenderX > RESOLUTION_WIDTH )
		return;

	if( iRenderY < 0 || iRenderY > RESOLUTION_HEIGHT )
		return;

	int iMessageBalloonBoxWidth		= pTextMessage->GetTextWidth();
	int iMessageBalloonBoxHeight	= pTextMessage->GetHeightText();
	int iClanWidthAdd = (bShowClanIcon && !bIsMessage) ? 34 : 0;

	for ( std::vector<ExtraText*>::iterator it = vExtraText.begin(); it != vExtraText.end(); it++ )
	{
		ExtraText * ps = (*it);

		if ( iMessageBalloonBoxWidth < ps->pText->GetTextWidth() )
			iMessageBalloonBoxWidth = ps->pText->GetTextWidth();

		iMessageBalloonBoxHeight += ps->pText->GetHeightFont() + 2;
	}

	iMessageBalloonBoxWidth += iClanWidthAdd;

	//New Width and Height
	if ( bClanName )
	{
		if ( iMessageBalloonBoxWidth < pTextClanName->GetTextWidth() )
			iMessageBalloonBoxWidth = pTextClanName->GetTextWidth();

		iMessageBalloonBoxHeight += pTextClanName->GetHeightFont() + 2;
	}
	if ( bPetName )
	{
		if ( iMessageBalloonBoxWidth < pTextPetName->GetTextWidth() )
			iMessageBalloonBoxWidth = pTextPetName->GetTextWidth();

		iMessageBalloonBoxHeight += pTextPetName->GetHeightFont() + 2;
	}

	if( iMessageBalloonBoxWidth < 20 )
	{
		iMessageBalloonBoxWidth = 20;
		iXBase += 10 - (pTextMessage->GetTextWidth() >> 1);
	}

	//Render Box
	DrawMessageBalloonBox( iRenderX - 6 - (iMessageBalloonBoxWidth >> 1), iRenderY - 4 - iMessageBalloonBoxHeight - 18, iMessageBalloonBoxWidth - 4, iMessageBalloonBoxHeight );

	int iLineCount = 0;

	if ( bClanName )
	{
		pTextClanName->Render( iX - (iMessageBalloonBoxWidth >> 1) + 2, iY - iMessageBalloonBoxHeight - 18, iWidth, iHeight, iSourceX, iSourceY );
		
		if ( iMessageBalloonBoxWidth > pTextMessage->GetTextWidth() )
			iXBase = (iMessageBalloonBoxWidth >> 1) - (pTextMessage->GetTextWidth() >> 1);

		iYBase += pTextClanName->GetHeightFont();

		iLineCount++;
	}

	//Top Render
	for ( std::vector<ExtraText*>::iterator it = vExtraText.begin(); it != vExtraText.end(); it++ )
	{
		ExtraText * ps = (*it);
		if ( ps->ePosition == POSITIONTEXT_Top )
		{
			int iAddX = iClanWidthAdd - 2;
			if ( iLineCount >= 2 )
				iAddX = ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1));

			if ( bShowClanIcon )
				ps->pText->Render( iX - (iMessageBalloonBoxWidth >> 1) + iAddX, iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );
			else
				ps->pText->Render( iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );

			iYBase += ps->pText->GetHeightFont();

			iLineCount++;
		}
	}


	if ( bPetName )
	{
		if ( iMessageBalloonBoxWidth > pTextPetName->GetTextWidth() )
			pTextPetName->Render( iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (pTextPetName->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 2, iWidth, iHeight, iSourceX, iSourceY );
		else	
			pTextPetName->Render( iX - (iMessageBalloonBoxWidth >> 1), iY - iMessageBalloonBoxHeight - 2, iWidth, iHeight, iSourceX, iSourceY );

		if ( iMessageBalloonBoxWidth > pTextMessage->GetTextWidth() )
			iXBase = (iMessageBalloonBoxWidth >> 1) - (pTextMessage->GetTextWidth() >> 1);

		iLineCount++;
	}

	//Clan Icon
	if ( bShowClanIcon )
	{
		int iClanWidth = psTextureClanIcon->sTexture.iWidth;
		int iClanHeight = psTextureClanIcon->sTexture.iHeight;

		UI::ImageRender::Render( psTextureClanIcon->sTexture.pcD3DTexture, iX - (iMessageBalloonBoxWidth >> 1), iY - iMessageBalloonBoxHeight - 17, iClanWidth, iClanHeight, 0, 0, -1, bIsMessage ? 0.5f : 1.0f );
		iXBase += bIsMessage ? 0 : ( iClanWidth >> 1 );
	}

	//Middle Render
	for ( std::vector<ExtraText*>::iterator it = vExtraText.begin(); it != vExtraText.end(); it++ )
	{
		ExtraText * ps = (*it);
		if ( ps->ePosition == POSITIONTEXT_Middle )
		{
			int iAddX = iClanWidthAdd - 2;
			if ( iLineCount >= 2 )
				iAddX = ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1));

			if ( bShowClanIcon )
				ps->pText->Render( iX - (iMessageBalloonBoxWidth >> 1) + iAddX, iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );
			else
				ps->pText->Render( iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );

			iYBase += ps->pText->GetHeightFont();

			iLineCount++;
		}
	}

	if ( bShowClanIcon )
	{
		int iAddX = ((iMessageBalloonBoxWidth >> 1) - (pTextMessage->GetTextWidth() >> 1));
		if ( iLineCount < 2 )
			iAddX = iXBase;

		pTextMessage->Render( iX - (iMessageBalloonBoxWidth >> 1) + iAddX, iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );
	}
	else
		pTextMessage->Render( iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (pTextMessage->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );

	iLineCount++;
	
	iYBase += pTextMessage->GetHeightFont();

	//Bottom Render
	for ( std::vector<ExtraText*>::iterator it = vExtraText.begin(); it != vExtraText.end(); it++ )
	{
		ExtraText * ps = (*it);
		if ( ps->ePosition == POSITIONTEXT_Bottom )
		{
			int iAddX = iClanWidthAdd - 2;
			if ( iLineCount >= 2 )
				iAddX = ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1));

			if ( bShowClanIcon )
				ps->pText->Render( iX - (iMessageBalloonBoxWidth >> 1) + iAddX, iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );
			else
				ps->pText->Render( iX - (iMessageBalloonBoxWidth >> 1) + ((iMessageBalloonBoxWidth >> 1) - (ps->pText->GetTextWidth() >> 1)), iY - iMessageBalloonBoxHeight - 18 + iYBase, iWidth, iHeight, iSourceX, iSourceY );

			iYBase += ps->pText->GetHeightFont();

			iLineCount++;
		}
	}

	iLastHeight = iMessageBalloonBoxHeight + 10;

	//Bellatra Icon
	if ( iBellatraCrown > 0 )
	{
		UI::Image * pcImage = pcaBellatraCrown[iBellatraCrown - 1];

		int iIconX = iRenderX - (iMessageBalloonBoxWidth >> 1);
		int iIconY = iRenderY - iMessageBalloonBoxHeight - 18;

		iLastHeight += 20;

		UI::ImageRender::Render( pcImage, iIconX + (iMessageBalloonBoxWidth >> 1) - (pcImage->GetWidth() >> 1), iIconY - pcImage->GetHeight() - 2, pcImage->GetWidth(), pcImage->GetHeight(), -1 );
	}

	Element::Render( iX, iY, iWidth, iHeight, iSourceX, iSourceY );
}

void MessageBalloon::SetColor( DWORD dwColor )
{
	pTextMessage->SetColor( dwColor );
}

void MessageBalloon::IsCharacterMessage( BOOL b )
{
	bIsCharacterMessage = b;
}

void MessageBalloon::SetClanIconTexture( EXESurfaceOld * ps )
{
	psTextureClanIcon = ps;
	
	bShowClanIcon = TRUE;
}

void MessageBalloon::DrawMessageBalloonBox( int iX, int iY, int iWidth, int iHeight )
{
	//Check if have Hover Images
	if( !saMessagesBalloon[eSkinID][1].pImageLeftTop )
		bHover = 0;

	//Drawing Top
	if( saMessagesBalloon[eSkinID][bHover].pImageLeftTop )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageLeftTop, iX, iY, 16, 8, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageMiddleTop )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageMiddleTop, iX + 16, iY, iWidth - 16, 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageRightTop )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageRightTop, iX + iWidth, iY, 16, 8, 0, 0, -1 );

	//Drawing Middle
	if( saMessagesBalloon[eSkinID][bHover].pImageLeft )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageLeft, iX, iY + 8, 16, iHeight - 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageCenter )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageCenter, iX + 16, iY + 8, iWidth - 16, iHeight - 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageRight )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageRight, iX + iWidth, iY + 8, 16, iHeight - 8, 0, 0, -1 );

	//Drawing Bottom
	if( saMessagesBalloon[eSkinID][bHover].pImageLeftBottom )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageLeftBottom, iX, iY + iHeight, 16, 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageMiddleBottom )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageMiddleBottom, iX + 16, iY + iHeight, iWidth - 16, 8, 0, 0, -1 );

	if( saMessagesBalloon[eSkinID][bHover].pImageRightBottom )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageRightBottom, iX + iWidth, iY + iHeight, 16, 8, 0, 0, -1 );

	//Drawing Tail Bottom
	if( saMessagesBalloon[eSkinID][bHover].pImageTailBottom && bIsMessage )
		UI::ImageRender::Render( saMessagesBalloon[eSkinID][bHover].pImageTailBottom, iX + (iWidth >> 1), iY + iHeight + 5, 16, 16, 0, 5, -1 );
}
void MessageBalloon::Update( float fTime )
{
	for ( auto & it = vCache.begin(); it != vCache.end(); )
	{
		if ( it->second.dwTimeOut < TICKCOUNT )
			it = vCache.erase( it );
		else
			it++;
	}
}
};
