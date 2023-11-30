#include "stdafx.h"
#include "CompareItemHandler.h"

CCompareItemHandler::CCompareItemHandler()
{
	pcTempItemData = new ItemData;
	ZeroMemory( pcTempItemData, sizeof( ItemData ) );
}

CCompareItemHandler::~CCompareItemHandler()
{
	DELET( pcTempItemData );
}

void CCompareItemHandler::Init()
{
	pItemInfoBox = std::make_shared<UI::ItemInfoBox>();
	pItemInfoBox->Init();
}

BOOL CCompareItemHandler::OnKeyPress( class CKeyboard * pcKeyBoard )
{
	//Is CTRL? 
	if ( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyDown && pcKeyBoard->GetKey() == VK_SHIFT )
	{
		if( ITEMHANDLER->GetMouseItemHover() )
		{
			// Get Item Mouse Hover
			ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();

			for( int i = 1; i < 14; i++ )
			{
				if( INVENTORYITEMSLOT[ i ].iItemIndex >= 0 )
				{
					ItemData * psItemData = &INVENTORYITEMS[ INVENTORYITEMSLOT[ i - 1 ].iItemIndex - 1 ];
					if( ( psItemData->sItem.iChk1 == pcItemData->sItem.iChk1 ) && ( psItemData->sItem.iChk2 == pcItemData->sItem.iChk2 ) )
						return FALSE;
				}
			}

			//Already checked? So dont check again
			if( bAlreadyChecked )
				return FALSE;

			if( pcItemData )
			{
				//Find Equipped item with same type
				ItemData * pcItemDataEquipped = ITEMHANDLER->GetItemEquippedByType( pcItemData->sBaseItemID.ToItemType() );
				if( pcItemDataEquipped )
				{
					CopyMemory( pcTempItemData, pcItemDataEquipped, sizeof( ItemData ) );

					int iXExtra = ( pcItemData->sPosition.iX + ( pcItemData->iWidth / 2 ) ) - 90;
					if( iXExtra < 0 )
						iXExtra = abs( iXExtra );
					else
						iXExtra = 0;

					//Set iItemSlot
					this->pcTempItemData->sPosition.iX = pcItemData->sPosition.iX + 230 + iXExtra;
					this->pcTempItemData->sPosition.iY = pcItemData->sPosition.iY;
					this->pcTempItemData->sOldPosition.iX = pcItemData->sOldPosition.iX;
					this->pcTempItemData->sOldPosition.iY = pcItemData->sOldPosition.iY;
					this->pcTempItemData->sPlacePosition.iX = pcItemData->sPlacePosition.iX;
					this->pcTempItemData->sPlacePosition.iY = pcItemData->sPlacePosition.iY;
					
					ITEMINFOBOX->SetCompareItem( pcTempItemData );
					ITEMINFOBOX->PrepareShowItem( pcItemData, FALSE, FALSE, FALSE );

					pItemInfoBox->SetCompareItem( pcItemData, TRUE );
					pItemInfoBox->PrepareShowItem( pcTempItemData, FALSE, FALSE, FALSE );

					bVisible = TRUE;
					bAlreadyChecked = TRUE;
				}
			}
		}
	}
	//Key Up and shown? Set Hide
	else if( pcKeyBoard->GetEvent() == EKeyboardEvent::KeyUp && bVisible )
	{
		ZeroMemory( pcTempItemData, sizeof( ItemData ) );
		bAlreadyChecked = FALSE;
		bVisible = FALSE;

		//Not compare Item
		ITEMINFOBOX->ClearCompareItem();
		pItemInfoBox->ClearCompareItem();
	}
	return FALSE;
}

void CCompareItemHandler::Render()
{
	if( !pcTempItemData )
		return;

	// Is Show?
	if ( bVisible )
	{
		// Get Mouse Item
		ItemData * pcItemData = ITEMHANDLER->GetMouseItemHover();
		if ( pcItemData && pcItemData->sBaseItemID.ToItemType() == pcTempItemData->sBaseItemID.ToItemType() )
		{
			// Show Info
			pItemInfoBox->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
	}
}