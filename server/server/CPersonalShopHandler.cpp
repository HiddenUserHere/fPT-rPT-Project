#include "stdafx.h"
#include "CPersonalShopHandler.h"


CPersonalShopHandler::CPersonalShopHandler()
{
}


CPersonalShopHandler::~CPersonalShopHandler()
{
}

void CPersonalShopHandler::HandlePacket( User * pcUser, PacketChatBoxMessage * psPacket )
{
	STRINGCOPY( pcUser->pcUserData->szPersonalShopMessage, psPacket->szChatBoxMessage );
	pcUser->pcUserData->iPersonalShop = psPacket->iChatColor;
}

void CPersonalShopHandler::HandlePacket( User * pcUser, PacketPersonalShopItemList * psPacket )
{
	//Same User?
	if ( psPacket->iSenderID == pcUser->GetID() )
	{
		User * pcUserViewer = USERDATATOUSER( USERDATABYID( psPacket->iRequesterID ) );
		if ( pcUserViewer )
		{
			//Check if trying to crash
			if ( psPacket->iItemCount > ITEM_PERSONALSHOP_QUEUE )
				psPacket->iItemCount = ITEM_PERSONALSHOP_QUEUE;

			//Obfuscate Item Checksum
			int iObfuscationKey = (pcUser->pcUserData->iPersonalShop + 2);
			for ( int i = 0; i < psPacket->iItemCount; i++ )
			{
				ItemData * pcItemData	 = psPacket->caItemData + i;
				pcItemData->sItem.iChk1 ^= iObfuscationKey;
				pcItemData->sItem.iChk2 ^= iObfuscationKey;

				PricePersonalShopData * psPrice  = psPacket->saItemPrice + i;
				psPrice->iChk1					^= iObfuscationKey;
				psPrice->iChk2					^= iObfuscationKey;
			}

			//Send
			ProcessPacket( pcUserViewer, psPacket );
		}
	}
}

void CPersonalShopHandler::HandlePacket( User * pcUser, PacketPersonalShopTradeItems * psPacket )
{
	//Seller User?
	if ( psPacket->iSellerID == pcUser->GetID() )
	{
		//Get Buyer
		User * pcBuyerUser = USERDATATOUSER( USERDATABYID( psPacket->iBuyerID ) );
		if ( pcBuyerUser )
		{
			bool bCanTrade = true;
			if ( bCanTrade )
			{
				//Delete Item
				if ( ITEMSERVER->UseItem( pcUser->pcUserData, psPacket->cItemData.sItem.sItemID.ToInt(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 ) >= 0 )
				{
					if ( (psPacket->sItemPrice.iPrice > 0) && (psPacket->iItemCount > 0) )
					{
						if ( psPacket->sItemPrice.bCredits == false )
						{
							int iSum = psPacket->sItemPrice.iPrice * psPacket->iItemCount;
							if ( iSum > 0 )
							{
								//Gold
								if ( CHARACTERSERVER->GiveGOLD( pcBuyerUser, -(psPacket->sItemPrice.iPrice * psPacket->iItemCount), WHEREID_PersonalShop ) )
								{
									CHARACTERSERVER->GiveGOLD( pcUser, psPacket->sItemPrice.iPrice * psPacket->iItemCount, WHEREID_PersonalShop );

									//Add Item to Buyer
									ITEMSERVER->AddItemInventory( pcBuyerUser->pcUserData, psPacket->cItemData.sItem.sItemID.ToInt(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 );

									psPacket->bBuy = false;

									pcBuyerUser->pcUserData->dwPersonalShopBuyTimeOut = TICKCOUNT + 2000;

									//Send
									ProcessPacket( pcBuyerUser, psPacket );

									LOGSERVER->OnLogItemEx( pcBuyerUser, ITEMLOGID_PersonalShopBought, "AccountName[%s] Character[%s] bought item on Personal Shop %s[%d][%sgp] from %s!", pcBuyerUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ),
															psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

									LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_PersonalShopSell, "AccountName[%s] Character[%s] sold item on Personal Shop %s[%d][%sgp] to %s!", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
															psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ) );

									//Notify Buyer
									CHATSERVER->SendChatEx( pcBuyerUser, CHATCOLOR_Notice, "> You bought %s[%d][%sgp] from %s!", psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

									//Notify Seller
									CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Notice, "> You sold %s[%d][%sgp] to %s!", psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ) );
								}
								else
								{
									PacketLogCheat sPacket;
									sPacket.iCheatID = CHEATLOGID_PersonalShopGoldReceiveError;
									sPacket.LParam = iSum;
									STRINGCOPY( sPacket.szBuffer1, pcBuyerUser->pcUserData->szAccountName );
									STRINGCOPY( sPacket.szBuffer2, CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ) );
									LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
								}
							}
						}
						else
						{
							int iSum = psPacket->sItemPrice.iPrice * psPacket->iItemCount;
							if ( iSum > 0 )
							{
								//Credits
								if ( COINSHOPHANDLER->HasCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcBuyerUser->pcUserData->szAccountName, (psPacket->sItemPrice.iPrice * psPacket->iItemCount) ) )
								{
									//Discount Coins from Buyer
									COINSHOPHANDLER->UseCoin( COINSHOPHANDLER->GetCoinShop(), pcBuyerUser->pcUserData->szAccountName, (psPacket->sItemPrice.iPrice * psPacket->iItemCount) );

									//Add Coins to Seller
									COINSHOPHANDLER->AddCoin( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName, (psPacket->sItemPrice.iPrice * psPacket->iItemCount) );


									//Add Item to Buyer
									ITEMSERVER->AddItemInventory( pcBuyerUser->pcUserData, psPacket->cItemData.sItem.sItemID.ToInt(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 );

									psPacket->bBuy = false;

									pcBuyerUser->pcUserData->dwPersonalShopBuyTimeOut = TICKCOUNT + 2000;

									//Send
									ProcessPacket( pcBuyerUser, psPacket );

									LOGSERVER->OnLogItemEx( pcBuyerUser, ITEMLOGID_PersonalShopBought, "AccountName[%s] Character[%s] bought item on Personal Shop %s[%d][%s Coins] from %s!", pcBuyerUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ),
															psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

									LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_PersonalShopSell, "AccountName[%s] Character[%s] sold item on Personal Shop %s[%d][%s Coins] to %s!", pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
															psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ) );

									//Notify Buyer
									CHATSERVER->SendChatEx( pcBuyerUser, CHATCOLOR_Notice, "> You bought %s[%d][%s Coins] from %s!", psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );

									//Notify Seller
									CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Notice, "> You sold %s[%d][%s Coins] to %s!", psPacket->cItemData.sItem.szItemName, psPacket->iItemCount,
															FormatNumber( (INT64)(psPacket->sItemPrice.iPrice * psPacket->iItemCount) ), CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ) );


									//Notify Buyer
									CHATSERVER->SendChatEx( pcBuyerUser, CHATCOLOR_Notice, "> You have now %s coins!", FormatNumber( (INT64)COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcBuyerUser->pcUserData->szAccountName  ) ) );

									//Notify Seller
									CHATSERVER->SendChatEx( pcUser, CHATCOLOR_Notice, "> You have now %s coins!", FormatNumber( (INT64)COINSHOPHANDLER->GetCoinAmount( COINSHOPHANDLER->GetCoinShop(), pcUser->pcUserData->szAccountName ) ) );

									//Send Coin Amount
									COINSHOPHANDLER->SendUserCoinAmount( pcUser );
									COINSHOPHANDLER->SendUserCoinAmount( pcBuyerUser );
								}
								else
								{
									PacketLogCheat sPacket;
									sPacket.iCheatID = CHEATLOGID_PersonalShopGoldReceiveError;
									sPacket.LParam = iSum;
									STRINGCOPY( sPacket.szBuffer1, pcBuyerUser->pcUserData->szAccountName );
									STRINGCOPY( sPacket.szBuffer2, CHARACTERSERVER->GetCharacterName( pcBuyerUser->pcUserData ) );
									LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
								}
							}
						}
					}
				}
			}
		}
	} 
	//Buyer User?
	else if ( psPacket->iBuyerID == pcUser->GetID() )
	{
		if ( !pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyBuyPersonalShop ) )
		{
            bool bCanTrade = true;
            if ( pcUser->pcUserData->dwPersonalShopBuyTimeOut > TICKCOUNT )
            {
                int iSeconds = (pcUser->pcUserData->dwPersonalShopBuyTimeOut - TICKCOUNT) / 1000;
                if ( iSeconds == 0 )
                    iSeconds = 1;

                CHATSERVER->SendChatMessageBox( pcUser, "You must wait %d second%s before buy another item!", iSeconds, iSeconds > 1 ? "s" : "" );

                bCanTrade = false;
            }

            if ( bCanTrade )
            {
                User * pcSellerUser = USERDATATOUSER( USERDATABYID( psPacket->iSellerID ) );
                if ( pcSellerUser )
                {
                    int iObfuscationKey = (pcSellerUser->pcUserData->iPersonalShop + 2);

                    //Undo the Obfuscation on the Item
                    psPacket->cItemData.sItem.iChk1 ^= iObfuscationKey;
                    psPacket->cItemData.sItem.iChk2 ^= iObfuscationKey;

                    psPacket->sItemPrice.iChk1 ^= iObfuscationKey;
                    psPacket->sItemPrice.iChk2 ^= iObfuscationKey;

                    //Send
                    ProcessPacket( pcSellerUser, psPacket );
                }
            }
		}
		else
			CHATSERVER->SendChatMessageBox( pcUser, "You can't buy items on Personal Shop, because you're logged in with the Share PW!" );
	}
}

void CPersonalShopHandler::HandlePacket( User * pcUser, PacketPersonalShopRequestView * psPacket )
{
	//Wants to see Item Shop List from another User?
	if ( psPacket->iRequesterID == pcUser->GetID() )
	{
		//Send to User
		User * pcUserSender = USERDATATOUSER( USERDATABYID( psPacket->iSenderID ) );
		if ( pcUserSender )
		{
			ProcessPacket( pcUserSender, psPacket );

			//Send Coin Amount
			COINSHOPHANDLER->SendUserCoinAmount( pcUser );
			COINSHOPHANDLER->SendUserCoinAmount( pcUserSender );

			CHATSERVER->SendChatEx( pcUserSender, CHATCOLOR_Notice, "> %s visited your Personal Shop!", CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ) );
		}
	}
}

void CPersonalShopHandler::ProcessPacket( User * pcUser, PacketPersonalShopItemList * psPacket )
{
	psPacket->iLength = sizeof( PacketPersonalShopItemList );
	psPacket->iHeader = PKTHDR_PersonalShopItemList;
	SENDPACKET( pcUser, psPacket );
}

void CPersonalShopHandler::ProcessPacket( User * pcUser, PacketPersonalShopTradeItems * psPacket )
{
	psPacket->iLength = sizeof( PacketPersonalShopTradeItems );
	psPacket->iHeader = PKTHDR_PersonalShopTradeItems;
	SENDPACKET( pcUser, psPacket );
}

void CPersonalShopHandler::ProcessPacket( User * pcUser, PacketPersonalShopRequestView * psPacket )
{
	psPacket->iLength = sizeof( PacketPersonalShopRequestView );
	psPacket->iHeader = PKTHDR_PersonalShopRequestView;
	SENDPACKET( pcUser, psPacket );
}
