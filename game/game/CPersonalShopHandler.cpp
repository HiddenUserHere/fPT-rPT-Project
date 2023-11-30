#include "stdafx.h"
#include "CPersonalShopHandler.h"


CPersonalShopHandler::CPersonalShopHandler()
{
	pcWindow = new CPersonalShopWindowHandler();
}


CPersonalShopHandler::~CPersonalShopHandler()
{
	ClearShop();

	DELET( pcWindow );

	vVisitedShop.clear();
	vPinnedShop.clear();
}

void CPersonalShopHandler::Init()
{
	pcWindow->Init();
}

void CPersonalShopHandler::HandlePacket( PacketPersonalShopItemList * psPacket )
{
	//First Packet? Clear Shop
	if ( psPacket->IsFirst() )
		ClearShop();

	if ( psPacket->iItemCount > ITEM_PERSONALSHOP_QUEUE )
		psPacket->iItemCount = ITEM_PERSONALSHOP_QUEUE;

	//Add items received
	for ( int i = 0; i < psPacket->iItemCount; i++ )
	{
		GetWindow()->AddItem( psPacket->caItemData + i, false );
		AddPrice( psPacket->saItemPrice + i );
	}

	//Last Packet? Open Shop
	if ( psPacket->IsOnly() )
	{
		iLastUserSellerID = psPacket->iSenderID;
		GetWindow()->Open( false );
	}
}

void CPersonalShopHandler::HandlePacket( PacketPersonalShopTradeItems * psPacket )
{
	if ( UNITDATA->iPersonalShop != 0 )
	{
		if ( psPacket->bBuy )
		{
			if ( auto[bRet, psPrice] = GetPrice( psPacket->sItemPrice.sItemID.ToInt(), psPacket->sItemPrice.iChk1, psPacket->sItemPrice.iChk2 ); bRet )
			{
				if ( psPrice->bCanBuy && (psPacket->sItemPrice.iPrice == psPrice->iPrice) )
				{
					ItemData * pcItemData = CGameInventory::GetInstance()->GetInventoryItemIndexEx( psPrice->sItemID.ToInt(), psPrice->iChk1, psPrice->iChk2 );
					if ( pcItemData )
					{
						CopyMemory( &psPacket->cItemData, pcItemData, sizeof( ItemData ) );
						CopyMemory( &psPacket->sItemPrice, psPrice, sizeof( PricePersonalShopData ) );

						psPacket->iSellerID = UNITDATA->iID;

						bool bSuccess = false;

						if ( (pcItemData->sItem.sItemID.ToItemBase() == ITEMBASE_Potion) )
						{
							if ( (psPacket->iItemCount > 0) && (pcItemData->sItem.iPotionCount > 0) && (pcItemData->sItem.iPotionCount >= psPacket->iItemCount) )
							{
								pcItemData->sItem.iPotionCount -= psPacket->iItemCount;
								psPacket->cItemData.sItem.iPotionCount = psPacket->iItemCount;

								psPrice->iCount -= psPacket->iItemCount;

								if ( ItemData * pcItemDataShop = GetWindow()->GetItem( psPrice->sItemID.ToInt(), psPrice->iChk1, psPrice->iChk2 ) )
									pcItemDataShop->sItem.iPotionCount -= psPacket->iItemCount;

								if ( pcItemData->sItem.iPotionCount == 0 )
								{
									GetWindow()->RemoveItem( psPacket->sItemPrice.sItemID.ToInt(), psPacket->sItemPrice.iChk1, psPacket->sItemPrice.iChk2 );
									RemovePrice( psPacket->sItemPrice.sItemID.ToInt(), psPacket->sItemPrice.iChk1, psPacket->sItemPrice.iChk2 );
									ITEMHANDLER->DeleteItem( pcItemData );

									if ( GetWindow()->GetItems().size() == 0 )
										GetWindow()->Close();
								}
								bSuccess = true;
							}
						}
						else
						{
							GetWindow()->RemoveItem( psPacket->sItemPrice.sItemID.ToInt(), psPacket->sItemPrice.iChk1, psPacket->sItemPrice.iChk2 );
							RemovePrice( psPacket->sItemPrice.sItemID.ToInt(), psPacket->sItemPrice.iChk1, psPacket->sItemPrice.iChk2 );
							ITEMHANDLER->DeleteItem( pcItemData );
							
							if ( GetWindow()->GetItems().size() == 0 )
								GetWindow()->Close();

							bSuccess = true;
						}

						if ( bSuccess )
						{
							ProcessPacket( psPacket, true );
							SAVE;
						}
					}
				}
			}
		}
	}
	else
	{
		if ( psPacket->bBuy == false )
		{
			ItemData sItemData;
			if ( ITEMHANDLER->LoadItemImage( &psPacket->cItemData.sItem, &sItemData ) )
			{
				ITEMHANDLER->BackItemToInventory( &sItemData, ITEMCHECKINVENTORY_Requirements );
				ITEMHANDLER->ResetInventoryItemChecksum();

				if ( pcLastBoughtItem )
				{
					GetWindow()->RemoveItem( pcLastBoughtItem->sItem.sItemID.ToInt(), pcLastBoughtItem->sItem.iChk1, pcLastBoughtItem->sItem.iChk2 );
					RemovePrice( pcLastBoughtItem->sItem.sItemID.ToInt(), pcLastBoughtItem->sItem.iChk1, pcLastBoughtItem->sItem.iChk2 );
					pcLastBoughtItem = nullptr;
				}

				if ( GetWindow()->GetItems().size() == 0 )
					GetWindow()->Close();

				SAVE;

				//ProcessPacket( &PacketPersonalShopRequestView( iLastUserSellerID, UNITDATA->iID ) );
			}
		}
	}
}

void CPersonalShopHandler::HandlePacket( PacketPersonalShopRequestView * psPacket )
{
	if ( UNITDATA->iPersonalShop != 0 )
	{
		std::vector<PacketPersonalShopItemList> vPackets;

		PacketPersonalShopItemList sPacket;
		sPacket.iItemCount = 0;

		for ( const auto & psPrice : vPriceData )
		{
			if ( auto pcItemData = CGameInventory::GetInstance()->GetInventoryItemIndexEx( psPrice->sItemID.ToInt(), psPrice->iChk1, psPrice->iChk2 ) )
			{
                if ( auto pcItemDataSlot = GetWindow()->GetItem( psPrice->sItemID.ToInt(), psPrice->iChk1, psPrice->iChk2 ) )
                {
                    auto pcItemDataCopy = sPacket.caItemData + sPacket.iItemCount;
                    CopyMemory( pcItemDataCopy, pcItemData, sizeof( ItemData ) );
                    CopyMemory( sPacket.saItemPrice + sPacket.iItemCount, psPrice, sizeof( PricePersonalShopData ) );

                    pcItemDataCopy->sPosition = pcItemDataSlot->sPosition;

                    sPacket.iItemCount++;

                    if ( sPacket.iItemCount == ITEM_PERSONALSHOP_QUEUE )
                    {
                        sPacket.sPacketNumber = (short)vPackets.size() + 1;
                        vPackets.push_back( sPacket );
                        sPacket.iItemCount = 0;
                    }
                }
			}
		}

		if ( (sPacket.iItemCount > 0) && (sPacket.iItemCount < ITEM_PERSONALSHOP_QUEUE) )
		{
			sPacket.sPacketNumber = (short)vPackets.size() + 1;
			vPackets.push_back( sPacket );
		}

		for ( auto & sPacket : vPackets )
		{
			sPacket.iRequesterID	= psPacket->iRequesterID;
			sPacket.iSenderID		= UNITDATA->iID;
			sPacket.sPacketMax		= (short)vPackets.size();
			ProcessPacket( &sPacket );
		}

		vPackets.clear();
	}
}

void CPersonalShopHandler::RequestPersonalShop( UnitData * pcUnitData )
{
	static DWORD dwTime = 0;

	if ( (dwTime + 2000) < TICKCOUNT )
	{
		ProcessPacket( &PacketPersonalShopRequestView( pcUnitData->iID, UNITDATA->iID ) );
		dwTime = TICKCOUNT;
	}
}

int CPersonalShopHandler::GetCanCarryGold()
{
	int iGold = 0;
	for ( const auto & ps : vPriceData )
	{
		if ( ps->bCanBuy )
			iGold += (ps->iPrice * ps->iCount);
	}

	return ITEMWINDOWHANDLER->GetGoldPersonalShopLimit( iGold );
}

int CPersonalShopHandler::GetGoldSelling()
{
	int iGold = 0;
	for ( const auto & ps : vPriceData )
	{
		if ( ps->bCanBuy )
			iGold += (ps->iPrice * ps->iCount);
	}

	return iGold;
}

bool CPersonalShopHandler::CanUseGoldShop( PricePersonalShopData * psPriceData )
{
	if ( psPriceData->bCredits )
	{
		if ( (ACCOUNTHANDLER->GetCoinAmount() - (psPriceData->iPrice * MESSAGEBOX->GetCountItem())) >= 0 )
			return true;
	}
	else
	{
		if ( CHARACTERGAME->CanUseGold( psPriceData->iPrice * MESSAGEBOX->GetCountItem() ) && (UNITDATA->sCharacterData.iGold - (psPriceData->iPrice * MESSAGEBOX->GetCountItem())) >= 0 )
			return true;
	}

	return false;
}

void CPersonalShopHandler::AddPrice( PricePersonalShopData * psPriceData )
{
	PricePersonalShopData * ps = new PricePersonalShopData();
	ps->sItemID.SetItemID( psPriceData->sItemID.ToItemID() );
	ps->iChk1	= psPriceData->iChk1;
	ps->iChk2	= psPriceData->iChk2;
	ps->iPrice	= psPriceData->iPrice;
	ps->bCredits = psPriceData->bCredits;
	ps->bCanBuy = psPriceData->bCanBuy;
	ps->iCount  = psPriceData->iCount;
	vPriceData.push_back( ps );
}

void CPersonalShopHandler::RemovePrice( int iItemID, int iChk1, int iChk2 )
{
	for ( std::vector<PricePersonalShopData*>::iterator it = vPriceData.begin(); it != vPriceData.end(); )
	{
		PricePersonalShopData * ps = (*it);
		if ( (ps->sItemID.ToInt() == iItemID) && (ps->iChk1 == iChk1) && (ps->iChk2 == iChk2) )
		{
			DELET( ps );
			it = vPriceData.erase( it );
		}
		else
			it++;
	}
}

std::tuple<bool, PricePersonalShopData *> CPersonalShopHandler::GetPrice( int iItemID, int iChk1, int iChk2 )
{
	bool bRet = false;
	PricePersonalShopData * psRet = nullptr;

	for ( const auto ps : vPriceData )
	{
		if ( ps->sItemID.ToInt() == iItemID )
		{
			if ( ps->iChk1 == iChk1 )
			{
				if ( ps->iChk2 == iChk2 )
				{
					psRet	= ps;
					bRet	= true;
					break;
				}
			}
		}
	}

	return std::make_tuple( bRet, psRet );
}

bool CPersonalShopHandler::GetVisitedShop( int iID )
{
	for ( const auto i : vVisitedShop )
		if ( i == iID )
			return true;

	return false;
}

void CPersonalShopHandler::AddVisitedShop( int iID )
{
	for ( const auto i : vVisitedShop )
		if ( i == iID )
			return;

	vVisitedShop.push_back( iID );

	if ( vVisitedShop.size() == MAX_PERSONALSHOP_VISITED )
	{
		auto p = vVisitedShop.begin();
		p = vVisitedShop.erase( p );
	}
}

void CPersonalShopHandler::RemoveVisitedShop( int iID )
{
	for ( std::vector<int>::iterator it = vVisitedShop.begin(); it != vVisitedShop.end(); )
	{
		int i = (*it);
		if ( i == iID )
		{
			it = vVisitedShop.erase( it );
		}
		else
			it++;
	}
}

bool CPersonalShopHandler::GetPinnedShop( int iID )
{
	for ( const auto i : vPinnedShop )
		if ( i == iID )
			return true;

	return false;
}

void CPersonalShopHandler::AddPinnedShop( int iID )
{
	for ( const auto i : vPinnedShop )
		if ( i == iID )
			return;

	vPinnedShop.push_back( iID );

	if ( vPinnedShop.size() == MAX_PERSONALSHOP_VISITED )
	{
		auto p = vPinnedShop.begin();
		p = vPinnedShop.erase( p );
	}
}

void CPersonalShopHandler::RemovePinnedShop( int iID )
{
	for ( std::vector<int>::iterator it = vPinnedShop.begin(); it != vPinnedShop.end(); )
	{
		int i = (*it);
		if ( i == iID )
		{
			it = vPinnedShop.erase( it );
		}
		else
			it++;
	}
}

void CPersonalShopHandler::ClearShop()
{
	for ( auto ps : vPriceData )
		DELET( ps );

	vPriceData.clear();

	GetWindow()->ClearItems();

	int * piaPersonalShopItemIndex = ((int*)0x03653C50);
	ZeroMemory( piaPersonalShopItemIndex, sizeof( int ) * 100 );
}

void CPersonalShopHandler::ProcessPacket( PacketChatBoxMessage * psPacket )
{
	psPacket->iLength = sizeof( PacketChatBoxMessage );
	psPacket->iHeader = PKTHDR_NewPersonalShop;
	SENDPACKETG( psPacket );
}

void CPersonalShopHandler::ProcessPacket( PacketPersonalShopItemList * psPacket )
{
	psPacket->iLength = sizeof( PacketPersonalShopItemList );
	psPacket->iHeader = PKTHDR_PersonalShopItemList;
	SENDPACKETG( psPacket );
}

void CPersonalShopHandler::ProcessPacket( PacketPersonalShopTradeItems * psPacket, bool bSell )
{
	psPacket->iLength = sizeof( PacketPersonalShopTradeItems );
	psPacket->iHeader = PKTHDR_PersonalShopTradeItems;
	if ( bSell )
		SENDPACKETL( psPacket );
	else
		SENDPACKETG( psPacket );
}

void CPersonalShopHandler::ProcessPacket( PacketPersonalShopRequestView * psPacket )
{
	psPacket->iLength = sizeof( PacketPersonalShopRequestView );
	psPacket->iHeader = PKTHDR_PersonalShopRequestView;
	SENDPACKETG( psPacket );
}
