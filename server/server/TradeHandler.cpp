#include "stdafx.h"
#include "TradeHandler.h"


CTradeHandler::CTradeHandler()
{
}


CTradeHandler::~CTradeHandler()
{
}

void CTradeHandler::HandlePacket( User * pcUser, PacketTradeData * psPacket )
{
	TradeDataItem sData;
	ZeroMemory( &sData, sizeof( TradeDataItem ) );

	UserData * pcUserData	= pcUser->pcUserData;

	char * pszCharacterName = CHARACTERSERVER->GetCharacterName( pcUserData );

	//Decode Trade Data
	ITEMSERVER->DecodeItemsData( psPacket->baData, &sData, 0x7FFFF );

	//Log All Items on Trade
	for ( int i = 0; i < 20; i++ )
	{
		ItemData * pcItemData		= sData.cItemData + i;
		
		DropItemData * psItemTrade	= pcUser->saTradeItems + i;

		//Valid Item?
		if ( pcItemData->bValid )
		{
			//Not same old item?
			if ( (psItemTrade->iItemID != pcItemData->sItem.sItemID.ToItemID() ||
				psItemTrade->iChk1    != pcItemData->sItem.iChk1 ||
				psItemTrade->iChk2    != pcItemData->sItem.iChk2) )
			{
				//Log It
				LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put On Trade ( [%s][0x%08X] %d x %d )",
					pcUserData->szAccountName, pszCharacterName, pcItemData->sItem.szItemName, pcItemData->sItem.sItemID.ToItemID(),
					pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

				//Add To User
				psItemTrade->iItemID = pcItemData->sItem.sItemID.ToItemID();
				psItemTrade->iChk1	= pcItemData->sItem.iChk1;
				psItemTrade->iChk2	= pcItemData->sItem.iChk2;
			}
		}
		else if ( pcItemData->sBaseItemID.ToItemID() == 0 && psItemTrade->iItemID != 0 ) // Removed Item?
		{
			
			//Get Item Name
			char szItemName[32] = { 0 };
			ITEMSERVER->FindItemName( psItemTrade->iItemID, szItemName );

			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemsRemove, "[%s](%s) Removed from Trade ( [%s][0x%08X] %d x %d )",
				pcUserData->szAccountName, pszCharacterName, szItemName, psItemTrade->iItemID,
				pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 );

			//Add To User
			psItemTrade->iItemID = 0;
			psItemTrade->iChk1	= 0;
			psItemTrade->iChk2	= 0;
		}
	}

	int iGold = sData.iGold - 193;

	//Gold
	if ( pcUser->iTradeGold > 0 )
	{
		//Added More Gold?
		if ( iGold > pcUser->iTradeGold )
		{
			int iDifference = iGold - pcUser->iTradeGold;

			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put on Trade ( [Gold][%d][%d] )",
				pcUserData->szAccountName, pszCharacterName, iDifference, iGold );

			pcUser->iTradeGold = iGold;
		}
		else if ( iGold < pcUser->iTradeGold )
		{
			int iDifference = pcUser->iTradeGold - iGold;

			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemsRemove, "[%s](%s) Removed from Trade ( [Gold][%d][%d] )",
				pcUserData->szAccountName, pszCharacterName, iDifference, iGold );

			pcUser->iTradeGold = iDifference;
		}
	}
	else
	{
		//Added Gold?
		if ( iGold > 0 )
		{
			//Log It
			LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItems, "[%s](%s) Put on Trade ( [Gold][%d][%d] )",
				pcUserData->szAccountName, pszCharacterName, iGold, iGold );

			pcUser->iTradeGold = iGold;
		}
	}
}

void CTradeHandler::HandlePacket( User * pcUser, PacketTradeRequest * psPacket )
{
	User * pcPartner = USERDATATOUSER( USERDATABYID( psPacket->iUnitID ) );

	if ( pcPartner && pcPartner->pcUserData->pcSocketData )
	{
		if ( psPacket->iType == 2 )
		{
			PacketCoinAmount s;

			pcUser->iTradeCoin = 0;
			pcPartner->iTradeCoin = 0;

			s.iCoinAmount = COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName );
			COINSHOPHANDLER->ProcessPacket( pcUser, &s );

			s.iCoinAmount = COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcPartner->pcUserData->szAccountName );
			COINSHOPHANDLER->ProcessPacket( pcPartner, &s );

			PacketCoinsTraded s1;

			s1.iLength = sizeof( PacketCoinsTraded );
			s1.iHeader = PKTHDR_CoinTraded;

			s1.iCoinsTraded = COINSHOPHANDLER->GetCoinTradeAmount( pcUser->pcUserData->szAccountName );
			SENDPACKET( pcUser, &s1 );

			s1.iCoinsTraded = COINSHOPHANDLER->GetCoinTradeAmount( pcPartner->pcUserData->szAccountName );
			SENDPACKET( pcPartner, &s1 );

			//Gold
			pcUser->iTradeGold		= 0;
			pcPartner->iTradeGold	= 0;
		}
		else if ( psPacket->iType == 3 )
		{
			pcUser->iTradeCoin = 0;
			pcPartner->iTradeCoin = 0;
		}
	}
}

void CTradeHandler::HandlePacket( User * pcUser, PacketTradeSuccess * psPacket )
{
	User * pcPartner = USERDATATOUSER( USERDATABYID( psPacket->iReceiverID ) );

	if ( pcPartner && pcPartner->pcUserData->pcSocketData )
	{
		//Net Server...
		NETSERVER->SendTradeCoin( pcUser->pcUserData->iID, pcPartner->pcUserData->iID );
	}
}

void CTradeHandler::HandleTradeCoin( User * pcUser, User * pcPartner )
{
	if ( pcPartner && pcPartner->pcUserData->pcSocketData && pcUser && pcUser->pcUserData->pcSocketData )
	{
		//Trade Coins...
		if ( pcUser->iTradeCoin && COINSHOPHANDLER->HasCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName, pcUser->iTradeCoin ) )
		{
			//Add Coin to Partner...
			COINSHOPHANDLER->AddCoin( COINSHOPHANDLER->GetCoinShop(), pcPartner->pcUserData->szAccountName, pcUser->iTradeCoin );

			//Discount from user...
			COINSHOPHANDLER->UseCoin( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName, pcUser->iTradeCoin );
			COINSHOPHANDLER->AddTradeCoin( pcUser->pcUserData->szAccountName, pcUser->iTradeCoin );

			CHATSERVER->SendChatEx( pcPartner, EChatColor::CHATCOLOR_Trade, "> You received %s credits from %s!", FormatNumber( pcUser->iTradeCoin ), pcUser->pcUserData->sCharacterData.szName );
			CHATSERVER->SendChatEx( pcPartner, EChatColor::CHATCOLOR_Trade, "> You now have %s credits!", FormatNumber( COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcPartner->pcUserData->szAccountName ) ) );

			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Trade, "> You sent %s credits to %s!", FormatNumber( pcUser->iTradeCoin ), pcPartner->pcUserData->sCharacterData.szName );
			CHATSERVER->SendChatEx( pcUser, EChatColor::CHATCOLOR_Trade, "> You now have %s credits!", FormatNumber( COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName ) ) );

			//Coin Log
			COINSHOPHANDLER->SQLCoinLog( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName, (char*)FormatString( "AccountName[%s] Character[%s] CoinAmount[%d] Sent Coin[%d] To AccountName[%s] Character[%s] CoinAmount[%d]",
				pcUser->pcUserData->szAccountName, pcUser->pcUserData->sCharacterData.szName, COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName ), pcUser->iTradeCoin,
				pcPartner->pcUserData->szAccountName, pcPartner->pcUserData->sCharacterData.szName, COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcPartner->pcUserData->szAccountName ) ) );

			//Notify new value

			pcUser->iTradeCoin = 0;

			COINSHOPHANDLER->SendUserCoinAmount( pcUser );

			COINSHOPHANDLER->SendUserCoinAmount( pcPartner );
		}

        if ( pcUser->iTradeGold > 0 )
        {
            //Log It
            LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemSuccess, "[%s](%s) Traded item to [%s](%s)( [Gold][%d] )",
                                    pcUser->pcUserData->szAccountName, pcUser->pcUserData->sCharacterData.szName, pcPartner->pcUserData->szAccountName, 
                                    pcPartner->pcUserData->sCharacterData.szName, pcUser->iTradeGold );

            LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_TradeItemSuccessReceive, "[%s](%s) Received Trade item from [%s](%s)( [Gold][%d] )",
                                    pcPartner->pcUserData->szAccountName, pcPartner->pcUserData->sCharacterData.szName, pcUser->pcUserData->szAccountName,
                                    pcUser->pcUserData->sCharacterData.szName, pcUser->iTradeGold );
        }
	}
}

void CTradeHandler::Tick()
{
	SYSTEMTIME * ps = GetServerTime();
	
	if ( wDay != ps->wDay )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "UPDATE UserInfo SET CoinsTraded=0" ) )
			{
				pcDB->Execute();
			}
			pcDB->Close();
		}

		wDay = ps->wDay;
	}
}

BOOL CTradeHandler::IsInTrade( User * pcUser, int iItemID, DWORD dwHead, DWORD dwCheckSum )
{

	return FALSE;
}
