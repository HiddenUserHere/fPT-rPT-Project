#include "stdafx.h"
#include "ChristmasHandler.h"


CChristmasHandler::CChristmasHandler()
{
}


CChristmasHandler::~CChristmasHandler()
{
}


BOOL CChristmasHandler::IsValidRune( class ItemData * pcItem )
{
	BOOL bResult = FALSE;

	switch ( pcItem->sItem.sItemID.ToItemID() )
	{
		case ITEMID_Cotton:
		case ITEMID_RedButton:
		case ITEMID_GreenCloth:
			bResult = TRUE;
			break;
	}

	return bResult;
}

int CChristmasHandler::IsPremiumXMas( int iItemID )
{
	BOOL bResult = FALSE;
	switch ( iItemID )
	{
		case ITEMID_Gingerbread:
			bResult = TRUE;
			break;
		default:
			break;
	}

	return bResult;
}

void CChristmasHandler::OpenRudolphWindow()
{
	TEXTWINDOW_OPEN( QUESTID_RudolphNPC, "game\\npcs\\xmas\\rudolph_main.txt" );
}

void CChristmasHandler::OnClickOKNPC()
{
	if ( (ITEMHANDLER->GetItemAmountByCode( ITEMID_GiftBox1 ) > 0) || (ITEMHANDLER->GetItemAmountByCode( ITEMID_GiftBox2 ) > 0) )
	{
		MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
		MESSAGEBOX->SetTitle( "Are you sure?" );
		MESSAGEBOX->SetDescription( "Do you like to exchange the Gift Box?" );
		MESSAGEBOX->SetEvent( std::bind( &CChristmasHandler::OnClickBox, this, std::placeholders::_1 ) );
		MESSAGEBOX->Show();
	}
}

void CChristmasHandler::OnClickBox( UIEventArgs e )
{
	if ( MESSAGEBOX->GetType() )
	{
		if ( TRADEHANDLER->IsWindowTradeOpen() )
		{
			TRADEHANDLER->CloseTradeWindow();
		}

		PacketNPCRudolphXmas s;
		s.iLength = sizeof( PacketNPCRudolphXmas );
		s.iHeader = PKTHDR_NPCRudolphXmas;

		ItemData * ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GiftBox1 );

		if ( ps == NULL )
			ps = ITEMHANDLER->GetItemInventoryByCode( ITEMID_GiftBox2 );

		if ( ps != NULL )
		{
			s.iItemID		= ps->sItem.sItemID.ToItemID();
			s.dwHead		= ps->sItem.iChk1;
			s.dwCheckSum	= ps->sItem.iChk2;

			ITEMHANDLER->DeleteItem( ps );

			SENDPACKETL( &s );
		}
	}
}

void CChristmasHandler::LoadParticles()
{
	iParticleSnowLoad = PARTICLE->Load( "game\\scripts\\particles\\XmasSnow.part" );
}

void CChristmasHandler::Render()
{
	if ( Game::GetGameMode() == GAMEMODE_InGame && CHARACTERGAME->IsStageVillage() && XMAS_EVENT_THINGS )
	{
		
		if ( dwTimeSnow < TICKCOUNT )
		{
			PARTICLE->Stop( iParticleSnow );
			iParticleSnow = PARTICLE->Start( iParticleSnowLoad, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY + 20000, UNITDATA->sPosition.iZ );
			dwTimeSnow = TICKCOUNT + (13000*120);
		}
		else
			PARTICLE->SetPosition( iParticleSnow, D3DVECTOR{ (float)(UNITDATA->sPosition.iX + RADIAN3D_X), (float)(UNITDATA->sPosition.iY + 20000), (float)(UNITDATA->sPosition.iZ + RADIAN3D_Z) } );
	}
}


