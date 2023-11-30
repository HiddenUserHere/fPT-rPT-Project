#include "stdafx.h"
#include "CQuestMapHandler.h"


CQuestMapHandler::CQuestMapHandler()
{
}


CQuestMapHandler::~CQuestMapHandler()
{
}

void CQuestMapHandler::Init()
{
	//Charge Bar
	pChargeBar = std::make_shared<UI::ChargeBar>( Rectangle2D( 0, 0, 200, 200 ) );
	pChargeBar->SetImage( UI::ImageLoader::LoadActiveImage( "game\\images\\UI\\messagebox\\autoclosebar.png", "game\\images\\UI\\messagebox\\autoclosebar_.png" ) );
	pChargeBar->SetMinMax( IMinMax( 1, 100 ) );

	pChargeBar->Hide();
}

void CQuestMapHandler::Render()
{
	pChargeBar->Render();
}

void CQuestMapHandler::Update( float fTime )
{
	pChargeBar->Update( fTime );
}

BOOL CQuestMapHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( WINDOW_INVENTORY )
	{
		if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR )
		{
			ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();
			if ( pcItemData && pcItemData->bValid )
			{
				int iX = pcItemData->sPosition.iX + (pcItemData->iWidth >> 1) - (pChargeBar->GetWidthImage() >> 1);
				int iY = pcItemData->sPosition.iY + (pcItemData->iHeight >> 1) - (pChargeBar->GetHeightImage() >> 1);				
				iY += READDWORD( 0x036286EC ) == 2 ? (RESOLUTION_HEIGHT - 600) : 0;
				if ( iX <= 0 )
					iX = 4;

				if ( pcItemData->sItem.sItemID.ToItemID() == ITEMID_PotionAgingQuestBee )
				{
					if ( CQUESTGAME->GetQuestInstance()->IsActiveQuest( QUESTID_GreenLiquidQuestStep2 ) )
					{
						if ( (MAP_ID == MAPID_FrozenSanctuary) && IsNearFromFrozenBlessedTree( UNITDATA->sPosition ) )
						{
							pChargeBar->SetDuration( 2500.0f );
							pChargeBar->SetPosition( iX, iY );
							pChargeBar->SetEvent( UI::Event::Build( std::bind( &CQuestMapHandler::OnCompleteChargeBarQuest, this, QUESTID_GreenLiquidQuestStep2, std::placeholders::_1 ) ) );
							pChargeBar->SetHideOnComplete( TRUE );
							pChargeBar->Charge( TRUE );
							pChargeBar->Show();
						}
						else
							CTITLEBOX->SetText( "You are far from Blessed Tree!" );
					}
				}
			}
		}
		else if ( pcMouse->GetEvent() == EMouseEvent::ClickUpR )
		{
			if ( pChargeBar->IsCharging() )
			{
				pChargeBar->Charge( FALSE );
				pChargeBar->Hide();
				pChargeBar->Reset();
			}
		}
	}

	return FALSE;
}

void CQuestMapHandler::OnCompleteChargeBarQuest( int iQuestID, UIEventArgs eArgs )
{
	if ( iQuestID == QUESTID_GreenLiquidQuestStep2 )
	{
		ItemData * pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_PotionAgingQuestBee );
		if ( pcItemData )
		{
			PacketQuestItemSwap sPacket;
			sPacket.iQuestID	= QUESTID_GreenLiquidQuestStep2;
			sPacket.iItemID		= pcItemData->sItem.sItemID.ToInt();
			sPacket.iChk1		= pcItemData->sItem.iChk1;
			sPacket.iChk2		= pcItemData->sItem.iChk2;
			CQUESTGAME->ProcessPacket( &sPacket );
			
			ITEMHANDLER->DeleteItem( pcItemData );
		}
	}
}

BOOL CQuestMapHandler::IsNearFromFrozenBlessedTree( Point3D sPosition )
{
	const Point3D sTreePosition( 9718189, 269854, 11963293 );

	if ( sTreePosition.WithinPTDistance( &sPosition, MAX_DISTANCE_FROZENSANCTUARY_TREE ) )
		return TRUE;

	return FALSE;
}
