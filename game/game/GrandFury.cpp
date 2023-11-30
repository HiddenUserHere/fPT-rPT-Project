#include "stdafx.h"
#include "GrandFury.h"


CGrandFury::CGrandFury()
{
}


CGrandFury::~CGrandFury()
{
	DELET( pcGrandFuryWindow );
}

BOOL CGrandFury::InsideArena( int iX, int iZ )
{
	if ( iX >= _XMIN_FURYARENA && iX <= _XMAX_FURYARENA )
	{
		if ( iZ >= _ZMIN_FURYARENA && iZ <= _ZMAX_FURYARENA )
			return TRUE;
	}
	return FALSE;
}

void CGrandFury::Init()
{
	iCurrentHP	= 0;
	iMaxHP		= 0;
	pcGrandFuryWindow = new CGrandFuryWindow();
	pcGrandFuryWindow->Init();

	pcTimeBG = UI::ImageLoader::LoadImage( "game\\images\\events\\grandfury\\bartime.png" );
	pcHPBackground = UI::ImageLoader::LoadImage( "game\\images\\misc\\gfbar.png" );
	pcHPFiller = UI::ImageLoader::LoadImage( "game\\images\\misc\\gfbar_.png" );

	pTextTime = std::make_shared<UI::Text>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (208 >> 1), 42, 208, 20 ) );
	pTextTime->SetHorizontalAlign( ALIGN_Center );
	pTextTime->SetText( "Time Left: 00:00:00" );
	pTextTime->SetFont( "Arial", 14, 0, FALSE, FALSE, D3DCOLOR_ARGB( 255, 220, 135, 135 ) );

	pTextBossHP = std::make_shared<UI::Text>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (208 >> 1), 67, 208, 20 ) );
	pTextBossHP->SetHorizontalAlign( ALIGN_Center );
	pTextBossHP->SetText( "" );
	pTextBossHP->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
}

void CGrandFury::HandlePacket( PacketFuryArenaRequest * psPacket )
{
	switch ( psPacket->eState )
	{
		case FURYARENASTATEID_OpenNPCEntrance:
			iEventID		= psPacket->iEventID;
			dwTimeCountDown = psPacket->dwTimeLeft;
			GetWindow()->Open( 1 );
			break;

		case FURYARENASTATEID_OpenNPC:
			iEventID		= psPacket->iEventID;
			dwTimeCountDown = psPacket->dwTimeLeft;
			GetWindow()->Open( 0 );
			break;

		case FURYARENASTATEID_Status:
			if ( bStarted != psPacket->p1 )
				dwTimeArena = 0;

			bStarted = psPacket->p1;
			if ( !bStarted )
			{
				SOUNDHANDLER->ChangeMusic( BACKGROUNDMUSICID_Dungeon );
				iCurrentHP	= 0;
				iMaxHP		= 0;
			}
			else
			{
				SOUNDHANDLER->ChangeMusic( BACKGROUNDMUSICID_FuryArena );
			}
			break;

		case FURYARENASTATEID_EXP:
			PLAYMINISOUND( 25 );
			break;

		case FURYARENASTATEID_Gold:
			PLAYMINISOUND( 18 );
			break;

		default:
			break;
	}
}

void CGrandFury::HandlePacket( struct PacketFuryArenaItemData * psPacket )
{
	// Put Item Back to Inventory
	ItemData * psItemData = ITEMHANDLER->FindItemTableByCode( psPacket->sItemData.sBaseItemID.ToInt() );

	if ( psItemData )
	{
		ItemData sItemData;
		if ( ITEMHANDLER->LoadItemImage( &psPacket->sItemData.sItem, &sItemData ) )
		{
			sItemData.bValid = TRUE;
			ITEMHANDLER->BackItemToInventory( &sItemData );
			ITEMHANDLER->ResetInventoryItemChecksum();

			// iSound
			PLAYMINISOUND( psPacket->sItemData.iInvItemSound );

			// Validate Inventory
			CALLT( 0x0050E510, 0x036932FC );
		}
	}
}

void CGrandFury::HandlePacket( struct PacketFuryArenaBossData * psPacket )
{
	iCurrentHP	= psPacket->sHP.iCurrentBossHP;
	iMaxHP		= psPacket->sHP.iMaxBossHP;
}

void CGrandFury::Update()
{
	if ( dwTimeCountDown != 0 )
	{
		dwTimeCountDown -= 1;
	}
	if ( bStarted )
	{
		dwTimeArena += 1;
	}
}

void CGrandFury::Render()
{
	GetWindow()->Render();

	int iXCenter = (RESOLUTION_WIDTH >> 1);
	
	// Inside Arena?
	if ( InsideArena( UNITDATA->sPosition.iX, UNITDATA->sPosition.iZ ) )
	{
		DWORD dw = dwTimeCountDown;

		if ( bStarted )
			dw = dwTimeArena;

		int iHour	= (dw / 3600);
		int iMinute	= (dw / 60) % 60;
		int iSecond	= dw % 60;
		
		//Render Time Left
		UI::ImageRender::Render( pcTimeBG, (RESOLUTION_WIDTH >> 1) - (208 >> 1), 100, 208, 20, 0, 0, -1 );
		pTextTime->SetBox( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (208 >> 1), 103, 208, 20 ) );
		pTextTime->SetTextEx( "Time Left: %02d:%02d:%02d", iHour, iMinute, iSecond );

		//Render HP Bar Boss
		if( bStarted )
		{
			UI::ImageRender::Render( pcHPBackground, (RESOLUTION_WIDTH >> 1) - (452 >> 1), 111, 452, 47, 0, 0, -1 );

			//Compute new iWidth of Health Bar based on current HP of the Boss
			int iBossWidth = iMaxHP > 0 ? (iCurrentHP * 407) / iMaxHP : 0;
			UI::ImageRender::Render( pcHPFiller, (RESOLUTION_WIDTH >> 1) - (407 >> 1), 123, iBossWidth, 23, 0, 0, -1 );

			pTextBossHP->SetBox( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (208 >> 1), 128, 208, 20 ) );
			pTextBossHP->SetTextEx( "%d/%d", iCurrentHP, iMaxHP );
			pTextBossHP->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

			pTextTime->SetTextEx( "Time: %02d:%02d:%02d", iHour, iMinute, iSecond );
		}

		pTextTime->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	}
}

