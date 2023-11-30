#include "stdafx.h"
#include "CoinHandler.h"


CCoinHandler::CCoinHandler()
{
	pcCoinShopWindow = new CCoinShopWindow();
}


CCoinHandler::~CCoinHandler()
{
	DELET( pcCoinShopWindow );
}

void CCoinHandler::Init()
{
	pcCoinShopWindow->Init();
}

void CCoinHandler::Render()
{
	pcCoinShopWindow->Render();
}

void CCoinHandler::OpenCoinShopNPC( int iID )
{
	if ( !pcCoinShopWindow->IsOpen() )
	{
		// Get Coin Amount
		PacketCoinAmount sPacketCoinAmount;
		ZeroMemory( &sPacketCoinAmount, sizeof( PacketCoinAmount ) );
		ProcessPacket( &sPacketCoinAmount );

		// Open Coin Shop
		PacketCoinShopNPC sPacketNPC;
		ZeroMemory( &sPacketNPC, sizeof( PacketCoinShopNPC ) );
		sPacketNPC.iID = iID;
		ProcessPacket( &sPacketNPC );
	}
}

void CCoinHandler::ProcessPacket( struct PacketCoinShopNPC * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinShopNPC );
	psPacket->iHeader = PKTHDR_CoinShopNPC;
	SENDPACKETL( psPacket );
}

void CCoinHandler::ProcessPacket( struct PacketCoinAmount * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinAmount );
	psPacket->iHeader = PKTHDR_CoinShopAmount;
	SENDPACKETL( psPacket );
}

void CCoinHandler::ProcessPacket( struct PacketCoinOpenTab * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinOpenTab );
	psPacket->iHeader = PKTHDR_CoinShopOpenTab;
	SENDPACKETL( psPacket );
}

void CCoinHandler::ProcessPacket( struct PacketCoinOpenItemData * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinOpenItemData );
	psPacket->iHeader = PKTHDR_CoinShopOpenItemData;
	SENDPACKETL( psPacket );
}

void CCoinHandler::ProcessPacket( struct PacketCoinBuy * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinBuy );
	psPacket->iHeader = PKTHDR_CoinShopBuy;
	SENDPACKETL( psPacket );
}

void CCoinHandler::ProcessPacket( PacketCoinServiceBuy * psPacket )
{
	psPacket->iLength = sizeof( PacketCoinServiceBuy );
	psPacket->iHeader = PKTHDR_CoinServiceBuy;
	SENDPACKETL( psPacket, TRUE );
}

void CCoinHandler::HandlePacket( PacketOpenCoinShopNPC * psPacket )
{
	OpenCoinShopNPC( psPacket->iID );
}

void CCoinHandler::HandlePacket( struct PacketCoinShopNPC * psPacket )
{
	// First Packet?
	if ( psPacket->IsFirst() )
	{
		// Recreate coin shop
		DELET( pcCoinShop );
		pcCoinShop = new CCoinShop( psPacket->iID );
		pcCoinShop->SetName( psPacket->szName );
		pcCoinShop->SetMessage( psPacket->szMessage );
		pcCoinShop->SetDiscount( psPacket->iDiscount );
		pcCoinShop->SetIsTimeShop( psPacket->bIsTimeShop );

		bIsTimeShop = pcCoinShop->IsTimeShop();
	}

	// Add Tabs
	for ( int i = 0; i < psPacket->iTabs; i++ )
	{
		PacketCoinShopNPC::Tab * psTab	= psPacket->aTabs + i;
		CCoinShopTab * pcTab			= new CCoinShopTab( psTab->iID, psTab->szName, psTab->iParentTabID, psTab->iBulk, psTab->iMaxBulk );
		pcTab->SetDiscount( psTab->iDiscount );

		// Child Tab?
		if ( pcTab->GetParentTabID() != 0 )
		{
			// Add to parent
			CCoinShopTab * pcParentTab = pcCoinShop->GetTabByID( pcTab->GetParentTabID() );
		
			if ( pcParentTab )
				pcParentTab->AddChildTab( pcTab );
		}
		else
		{
			// Add Tab to CoinShop
			pcCoinShop->AddTab( pcTab );
		}
	}

	if ( psPacket->IsOnly() )
	{
		// Build Window
		pcCoinShopWindow->Load( pcCoinShop );

		// Open Window
		pcCoinShopWindow->Open();
	}
}

void CCoinHandler::HandlePacket( struct PacketCoinAmount * psPacket )
{
	ACCOUNTHANDLER->SetCoinAmount( psPacket->iCoinAmount );
	ACCOUNTHANDLER->SetTimePointsAmount( psPacket->iTimePoints );
	COINHANDLER->GetWindow()->SetCoinAmount( IsTimeShop() ? ACCOUNTHANDLER->GetTimePointsAmount() : ACCOUNTHANDLER->GetCoinAmount(), !IsTimeShop() );
}

void CCoinHandler::HandlePacket( struct PacketCoinTab * psPacket )
{
	//Only show Right Window if there are Items
	if( psPacket->iItems > 0 )
	{

		//Get the Coin Shop Tab
		CCoinShopTab * pcTab = pcCoinShop->GetTabByID( psPacket->iID );

		if( !pcTab )
		{
			ChatGame::AddChatBoxText( "Tab error!" );
			return;
		}

		//Is this the first of multiple PacketCoinTabs?
		if( psPacket->IsFirst() )
		{
			//Clear Items from Tab
			pcTab->ClearItems();
		}

		//Add Items to the Coin Shop Tab
		for( int i = 0; i < psPacket->iItems; i++ )
		{
			PacketCoinTab::Item * pItem = psPacket->aItems + i;

			CCoinShopItem * pcItem = new CCoinShopItem( pItem->iID, pcTab->GetID(), pItem->szName, pItem->szDescription, pItem->szCode, pItem->szImage, pItem->iValue, pItem->iDiscount, pItem->iBulk, pItem->iMaxBulk );

			pcTab->AddItem( pcItem );
		}

		//Build the Coin Right Window
		pcCoinShopWindow->Load( pcTab );

	}
	else
	{
		//Only show Right NPC Box if this is the only PacketCoinTab
		if( psPacket->IsOnly() )
		{ 

		}
	}
}

void CCoinHandler::HandlePacket( struct PacketCoinItemData * psPacket )
{
	pcCoinShopWindow->bItemData = FALSE;
	if ( psPacket->iType == ITEMCOIN_Data )
	{
		pcCoinShopWindow->ClearItemsList();
		CCoinShopTab * pcTab = pcCoinShop->GetTabByID( psPacket->iTabID );
		if ( pcTab )
		{
			CCoinShopItem * pcItem = pcTab->GetItemByID( psPacket->iID );
			if ( pcItem )
			{
				pcCoinShopWindow->bItemData = TRUE;
				CopyMemory( pcCoinShopWindow->GetItemDefinition(), &psPacket->sItemDefinition, sizeof( DefinitionItem ) );

				pcCoinShopWindow->SetItemStatus( pcCoinShopWindow->GetItemDefinition(), pcItem->GetName(), pcItem->GetDescription() );
				pcCoinShopWindow->BuildItemNormal( pcItem, psPacket->bSpec, psPacket->bQuantity );
			}
		}
	}
}

void CCoinHandler::HandlePacket( struct PacketCoinBuy * psPacket )
{
	switch ( psPacket->iReturnCode )
	{
		case 0:
			ChatGame::AddChatBoxText( "> Not enough credits!", CHATCOLOR_Global );
			TITLEBOX( "Not enough credits!" );
			break;

		case 1:
			ChatGame::AddChatBoxText( "> Item bought! Please visit Item Distiributor to get it!", CHATCOLOR_Global );
			TITLEBOX( "Item bought! Please visit Item Distiributor to get it!" );
			PLAYMINISOUND( 25 );
			pcCoinShopWindow->ClearQuantity();
			break;

		case 2:
			ChatGame::AddChatBoxText( "> Item bought!", CHATCOLOR_Global );
			TITLEBOX( "Item bought!" );
			PLAYMINISOUND( 25 );
			pcCoinShopWindow->ClearQuantity();
			break;

		case -1:
			ChatGame::AddChatBoxText( "> Tab Error" );
			break;

		case -2:
			ChatGame::AddChatBoxText( "> Item Error" );
			break;

		case -3:
			ChatGame::AddChatBoxText( "> You already have this premium item!" );
			break;

		default:
			break;
	}
	if ( pcCoinShopWindow->IsBuying() )
		pcCoinShopWindow->SetBuying( FALSE );
}

void CCoinHandler::HandlePacket( PacketCoinOpenService * psPacket )
{
	switch ( psPacket->iID )
	{
		case ITEMCOIN_LevelUP:
			pcCoinShopWindow->ClearItemsList();
			pcCoinShopWindow->BuildLevelUP();
			break;
		case ITEMCOIN_ItemPerfect:
			pcCoinShopWindow->ClearItemsList();
			pcCoinShopWindow->BuildPerfectize( TRUE );
			break;
		case ITEMCOIN_ItemSwapper:
			pcCoinShopWindow->ClearItemsList();
			pcCoinShopWindow->BuildSwapper( TRUE );
			break;
		case ITEMCOIN_ChangeNick:
			pcCoinShopWindow->ClearItemsList();
			pcCoinShopWindow->BuildChangeNick( psPacket->iPrice );
			break;
		case ITEMCOIN_ChangeClass:
			pcCoinShopWindow->ClearItemsList();
			pcCoinShopWindow->BuildChangeClass( psPacket->iPrice );
			break;
		default:
			break;
	}
}

void CCoinHandler::HandlePacket( PacketCoinServiceBuy * psPacket )
{
	if ( psPacket->iType == ITEMCOIN_LevelUP || psPacket->iType == ITEMCOIN_ItemPerfect || psPacket->iType == ITEMCOIN_ItemSwapper ||
		psPacket->iType == ITEMCOIN_ChangeNick || psPacket->iType == ITEMCOIN_AgingRecovery || psPacket->iType == ITEMCOIN_ChangeClass )
		pcCoinShopWindow->HandleItemService( psPacket, &psPacket->ItemPerfect.cItemData );
}

void CCoinHandler::HandlePacket( PacketOpenAgingRecoveryService * psPacket )
{
	if ( psPacket->IsFirst() )
		pcCoinShopWindow->ClearItemsList();

	pcCoinShopWindow->BuildAgingRecovery( psPacket->saData, psPacket->iCount, psPacket->iPrice );
}
