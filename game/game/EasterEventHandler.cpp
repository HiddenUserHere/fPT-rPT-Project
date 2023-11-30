#include "stdafx.h"
#include "EasterEventHandler.h"

CEasterEventHandler::CEasterEventHandler()
{
}

CEasterEventHandler::~CEasterEventHandler()
{
}

void CEasterEventHandler::HandlePacket( PacketNPCEasterEgg * psPacket )
{
	//Open NPC
	TEXTWINDOW_OPEN( QUESTID_EasterNPC, "game\\npcs\\easter\\main.txt" );
}

void CEasterEventHandler::OnClickOKNPC()
{
	if ( (ITEMHANDLER->GetItemAmountByCode( ITEMID_RedEasterEgg ) >= 1) && 
		(ITEMHANDLER->GetItemAmountByCode( ITEMID_GreenEasterEgg ) >= 1) && (ITEMHANDLER->GetItemAmountByCode( ITEMID_PinkEasterEgg ) >= 1) )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Are you sure?" );
		MESSAGEBOX->SetDescription( "Do you like to exchange the eggs?" );
		MESSAGEBOX->SetEvent( std::bind( &CEasterEventHandler::OnOKBoxClick, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
}

void CEasterEventHandler::OnOKBoxClick( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( TRADEHANDLER->IsWindowTradeOpen() )
			TRADEHANDLER->CloseTradeWindow();

		PacketNPCEasterEgg s;
		s.iLength = sizeof( PacketNPCEasterEgg );
		s.iHeader = PKTHDR_NPCEasterEgg;
		s.iAction = PacketNPCEasterEgg::ACTION_GetReward;

		int iCount = 0;
		
		for ( int i = 0; i < EASTER_ITEMS_MAX; i++ )
		{
			ItemData * ps = NULL;

			if ( i == 0 )
				ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_RedEasterEgg );
			
			if ( i == 1 )
				ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GreenEasterEgg );
			
			if ( i == 2 )
				ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_PinkEasterEgg );

			if ( ps && ps->bValid )
			{
				s.iaItemID[i]		= ps->sItem.sItemID.ToItemID();
				s.dwaHead[i]		= ps->sItem.iChk1;
				s.dwaCheckSum[i]	= ps->sItem.iChk2;
				ITEMHANDLER->DeleteItem( ps );
				iCount++;
			}
		}
		
		if ( iCount != EASTER_ITEMS_MAX )
			return;

		SENDPACKETL( &s, TRUE );
	}
}
