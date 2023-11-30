#include "stdafx.h"
#include "PerfectItemViewHandler.h"


CPerfectItemViewHandler::CPerfectItemViewHandler()
{
	pcItemData = new ItemData;
	ZeroMemory( pcItemData, sizeof( ItemData ) );
}


CPerfectItemViewHandler::~CPerfectItemViewHandler()
{
	DELET( pcItemData );
}

void CPerfectItemViewHandler::PHItemData( PacketItemPerfectView * psPacket )
{
	// Copy Item Data
	CopyMemory( pcItemData, &psPacket->cItemData, sizeof( ItemData ) );

	// Set sItemID
	bReceivedItem = TRUE;
}

BOOL CPerfectItemViewHandler::OnKeyPress( class CKeyboard * pcKeyBoard )
{
	// Is CTRL? 
	if ( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyDown && tolower(pcKeyBoard->GetKey()) == CHAR('p') )
	{
		// Get Item Mouse Hover
		ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = WAREHOUSEHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHover();
		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHoverOther();

		if ( pcItemData == NULL )
		{
			if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
			{
				pcItemData = PERSONALSHOPHANDLER->GetWindow()->GetLastRenderedItem();
			}
		}

		if ( pcItemData )
		{
			int iItemType = pcItemData->sItem.sItemID.ToItemType();
			int iItemBase = pcItemData->sItem.sItemID.ToItemBase();

			if ( (iItemType != ITEMTYPE_BlessCastle) &&
				(iItemType != ITEMTYPE_Bellatra) &&
				(iItemType != ITEMTYPE_Respec) &&
				(iItemType != ITEMTYPE_Smelting) &&
				(iItemType != ITEMTYPE_Crafting) &&
				(iItemType != ITEMTYPE_Event) &&
				(iItemType != ITEMTYPE_EventPR5) &&
				(iItemType != ITEMTYPE_ForceOrb) &&
				(iItemBase != ITEMBASE_Core) &&
				(iItemBase != ITEMBASE_Potion) &&
				(iItemBase != ITEMBASE_Premium) &&
				(iItemBase != ITEMBASE_Quest1) &&
				(iItemBase != ITEMBASE_Quest2) &&
				(iItemBase != ITEMBASE_Special) &&
				(iItemBase != ITEMBASE_Recipe) )
			{
				bool bSameItem = true;
				if ( (this->pcItemData->sItem.sItemID.ToItemID() != pcItemData->sItem.sItemID.ToItemID()) || (this->pcItemData->sItem.iChk1 != pcItemData->sItem.iChk1) ||
					(this->pcItemData->sItem.iChk2 != pcItemData->sItem.iChk2) )
					bSameItem = false;

				if ( bSameItem == false )
				{
					//Send Data
					ProcessPacket( pcItemData->sBaseItemID.ToInt(), (int)pcItemData->sItem.sAgeLevel, pcItemData->sItem.eMixTypeName );
				}

				//Set Visible
				bVisible = TRUE;
			}
		}
	}
	// Key Up and shown? Set Hide
	else if ( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyUp && bVisible )
	{
		bVisible = FALSE;
		ITEMINFOBOX->SetViewingPerfectItem( FALSE );
	}

	return FALSE;
}

void CPerfectItemViewHandler::Render()
{
	// Is Show?
	if ( bVisible )
	{
		// Get Mouse Item
		ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = WAREHOUSEHANDLER->GetMouseItemHover();

		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHover();
		if ( pcItemData == NULL )
			pcItemData = TRADEHANDLER->GetMouseItemHoverOther();

		if ( pcItemData == NULL )
		{
			if ( PERSONALSHOPHANDLER->GetWindow()->IsOpen() )
			{
				pcItemData = PERSONALSHOPHANDLER->GetWindow()->GetLastRenderedItem();
			}
		}


		if ( pcItemData && pcItemData->sBaseItemID.ToItemID() )
		{
			// Set iItemSlot
			this->pcItemData->sPosition.iX = pcItemData->sPosition.iX;
			this->pcItemData->sPosition.iY = pcItemData->sPosition.iY;
			this->pcItemData->sOldPosition.iX = pcItemData->sOldPosition.iX;
			this->pcItemData->sOldPosition.iY = pcItemData->sOldPosition.iY;
			this->pcItemData->sPlacePosition.iX = pcItemData->sPlacePosition.iX;
			this->pcItemData->sPlacePosition.iY = pcItemData->sPlacePosition.iY;

			ITEMINFOBOX->SetViewingPerfectItem( TRUE );

			//Show Info
			if ( bReceivedItem )
				ITEMINFOBOX->PrepareShowItem( this->pcItemData, FALSE, FALSE, FALSE );
		}
	}
}

void CPerfectItemViewHandler::ProcessPacket( int iItemID, int iAgeLevel, int iMixID )
{
	PacketGetItemPerfectView sPacket;
	sPacket.iLength = sizeof( PacketGetItemPerfectView );
	sPacket.iHeader = PKTHDR_PerfectItemData;
	sPacket.iItemID = iItemID;
	sPacket.iAgeLevel = iAgeLevel;
	sPacket.iMixID = iMixID;
	SENDPACKETL( &sPacket );

	bReceivedItem = FALSE;
}
