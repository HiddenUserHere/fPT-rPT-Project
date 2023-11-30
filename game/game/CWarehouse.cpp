#include "stdafx.h"
#include "CWarehouse.h"

int iSizeLastPacketWarehouse = 0;


CWarehouse::CWarehouse()
{
}


CWarehouse::~CWarehouse()
{
	DELET( pcWarehouseWindow );
}

void CWarehouse::Init()
{
	pcWarehouseWindow = new CWarehouseWindow();
	pcWarehouseWindow->Init();
}

void CWarehouse::SetPage( int iPage )
{
	// Check for quests
	int iOldPage = GetPage();

	if ( iOldPage < iPage )
	{
		if ( CQUESTGAME->GetQuestInstance()->GetRewardValue( EQuestExtraRewardType::QUESTREWARDTYPE_WarehousePage ) < (iPage-1) )
		{
			TITLEBOX( "You need to complete the Quest Warehouse #%d", iPage - 1 );
			return;
		}
	}

	*(int*)0x3693304 = iPage-1;
	if ( *(int*)0x3693304 > 4 )
		*(int*)0x3693304 = 4;
	else if ( *(int*)0x3693304 < 0 )
		*(int*)0x3693304 = 0;
}

ItemData * CWarehouse::GetItemData()
{
	return (ItemData*)0x036A1380;
}

void CWarehouse::HandlePacket( PacketWarehouse * psPacket )
{
	int iPage		= psPacket->dwCheckSum;
	int iPageCount	= (int)psPacket->wPage;
	PacketWarehouse * pPacketWarehouse = (PacketWarehouse *)0x397A738;
//	if ( iPage == 0 )
//		ZeroMemory( (void*)0x397A738, 319444 );


//	CopyMemory( pPacketWarehouse + (iPage * 7936), &psPacket->Data[8], psPacket->WareHouseMoney );
}

int CWarehouse::OnRenderTime( struct Skill * psSkill )
{
	int iLine = 0;

	QuestStatusData * psQuestData = QUESTGAMEHANDLER->GetData();

	char szBuffer[128] = { 0 };

	return iLine;
}

void CWarehouse::RestoreItems()
{
	CALLT( 0x0050CC90, 0x036932FC );
}

ItemData * CWarehouse::GetMouseItemHover()
{
	if ( IsWarehouseWindowOpen() == FALSE )
		return NULL;

	ItemData * pcItemData = NULL;

	// Items in warehouse from current page
	ItemData * psaDataWarehouse = WAREHOUSEHANDLER->GetItemData() + ((WAREHOUSEHANDLER->GetPage() - 1) * 81);

	// Warehouse rect box
	RECT rWarehouseBox{ 21, 86, 21 + 200, 86 + 200 };

	// Rows
	for ( int i = 0; i < 9; i++ )
	{
		// Columns
		for ( int j = 0; j < 9; j++ )
		{
			// Get Item from mouse inside
			for ( int k = 0; k < 81; k++ )
			{
				ItemData * psWarehouseItem = psaDataWarehouse + k;

				if ( !psWarehouseItem->bValid )
					continue;

				Rectangle2D rRectangle;
				rRectangle.iX = psWarehouseItem->sPosition.iX;
				rRectangle.iY = psWarehouseItem->sPosition.iY;
				rRectangle.iWidth = psWarehouseItem->iWidth - 1;
				rRectangle.iHeight = psWarehouseItem->iHeight - 1;

				if ( rRectangle.Inside( MOUSEHANDLER->GetPosition() ) )
				{
					pcItemData = psWarehouseItem;
					break;
				}
			}

			if ( pcItemData )
				break;
		}

		if ( pcItemData )
			break;
	}


	return pcItemData;
}

BOOL CWarehouse::OnMouseClick( class CMouse * pcMouse )
{
	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR )
	{
		
	}
	return TRUE;
}

void CWarehouse::OpenNPCQuest()
{
	GetWindow()->OpenWindowQuest2Page();
}
