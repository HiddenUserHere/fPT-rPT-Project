#include "stdafx.h"
#include "SocketSystemHandler.h"


CSocketSystemHandler::CSocketSystemHandler()
{
}


CSocketSystemHandler::~CSocketSystemHandler()
{
}

void CSocketSystemHandler::HandlePacket( User * pcUser, PacketSocketSystem * psPacket )
{
	if ( psPacket->iLength != sizeof( PacketSocketSystem ) )
		return;

	if ( psPacket->iSocketWorkType == 0x0E )
		OnSocketDrill( pcUser, psPacket );
	else
		OnSocketStone( pcUser, psPacket );
}

void CSocketSystemHandler::OnMixingItem( User * pcUser, PacketMixServer * psPacket )
{
	ItemData * pcItemData = &psPacket->sItemData;

	int iCountItems = 0;
	int iType = 0;

	switch ( pcItemData->sItem.sItemID.ToItemID() )    
	{
		case ITEMID_PurpleDiamond:
			iCountItems = 12;
			iType = 1;
			break;

		case ITEMID_SilverDiamond:
			iCountItems = 8;
			iType = 2;
			break;

		case ITEMID_GoldDiamond:
			iCountItems = 4;
			iType = 3;
			break;

		case ITEMID_RedMarquiseGem:
		case ITEMID_GreenMarquiseGem:
		case ITEMID_BlueMarquiseGem:
		case ITEMID_YellowMarquiseGem:
		case ITEMID_PurpleMarquiseGem:
			iCountItems = 4;
			iType = 4;
			break;

		default:
			break;
	}

	int iItemID			= psPacket->dwCode[0];
	int iReceiveItemID	= 0;

	//Not Combination Type?
	if ( iType != 4 )
	{
		//Count Same Items
		for ( int i = 1; i < iCountItems; i++ )
		{
			if ( psPacket->dwCode[i] != iItemID )
			{
				//Error
				psPacket->iResult = FALSE;
				SENDPACKET( pcUser, psPacket, TRUE );
				return;
			}
		}
	}

	bool bFound[12]			= { false, false, false, false, false, false, false, false, false, false, false, false };
	int iFoundCombinations	= 0;
	int iCombinationID		= -1;

	//Elaborated Gems Create
	if ( iType == 4 )
	{
		//Array for Combinations
		static const int iaaCombinationItems[7][12]=
		{
			{ ITEMID_Oredo, ITEMID_Oredo, ITEMID_Oredo, ITEMID_Oredo, 0,0,0,0,0,0,0,0 },
			{ ITEMID_Bellum, ITEMID_Bellum, ITEMID_Bellum, ITEMID_Bellum, 0,0,0,0,0,0,0,0 },
			{ ITEMID_Enigma, ITEMID_Enigma, ITEMID_Enigma, ITEMID_Enigma, ITEMID_Enigma, ITEMID_Enigma, 0,0,0,0,0,0 },
			{ ITEMID_Inferna, ITEMID_Inferna, ITEMID_Inferna, ITEMID_Inferna, ITEMID_Inferna, ITEMID_Inferna, ITEMID_Inferna, ITEMID_Inferna, 0,0,0,0 },
			{ ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage, ITEMID_Mirage,0,0 },
			{ ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto, ITEMID_Celesto },
			{ ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine, ITEMID_Devine },
		};

		//Loop for all Conbinations
		for ( int i = 0; i < 7; i++ )
		{
			//Loop for all inside Sheltoms Combinations
			for ( int j = 0; j < 12; j++ )
			{
				//Loop for all Sheltoms on Mix
				for ( int k = 0; k < 12; k++ )
				{
					//Same Sheltom on a Not Found Combination?
					if ( !bFound[k] && iaaCombinationItems[i][j] == psPacket->dwCode[k] )
					{
						//Count it and Set Flag Found...
						iFoundCombinations++;
						bFound[k] = true;
					}
				}
			}
			//Found All Sheltoms?
			if ( iFoundCombinations == 12 )
			{
				//Set Combination ID
				iCombinationID = i;
				break;
			}
		}
	}


	//Purple Diamond
	if ( iType == 1 )
	{
		if ( iItemID == ITEMID_RedGemPiece )
			iReceiveItemID = ITEMID_RedTriangularGem;
		
		if ( iItemID == ITEMID_GreenGemPiece )
			iReceiveItemID = ITEMID_GreenTriangularGem;
		
		if ( iItemID == ITEMID_BlueGemPiece )
			iReceiveItemID = ITEMID_BlueTriangularGem;
		
		if ( iItemID == ITEMID_YellowGemPiece )
			iReceiveItemID = ITEMID_YellowTriangularGem;
		
		if ( iItemID == ITEMID_PurpleGemPiece )
			iReceiveItemID = ITEMID_PurpleTriangularGem;
	}
	
	//Silver Diamond
	if ( iType == 2 )
	{
		if ( iItemID == ITEMID_RedTriangularGem )
			iReceiveItemID = ITEMID_RedSquareGem;

		if ( iItemID == ITEMID_GreenTriangularGem )
			iReceiveItemID = ITEMID_GreenSquareGem;

		if ( iItemID == ITEMID_BlueTriangularGem )
			iReceiveItemID = ITEMID_BlueSquareGem;

		if ( iItemID == ITEMID_YellowTriangularGem )
			iReceiveItemID = ITEMID_YellowSquareGem;

		if ( iItemID == ITEMID_PurpleTriangularGem )
			iReceiveItemID = ITEMID_PurpleSquareGem;
	}

	//Gold Diamond
	if ( iType == 3 )
	{
		if ( iItemID == ITEMID_RedSquareGem )
			iReceiveItemID = ITEMID_RedMarquiseGem;

		if ( iItemID == ITEMID_GreenSquareGem )
			iReceiveItemID = ITEMID_GreenMarquiseGem;

		if ( iItemID == ITEMID_BlueSquareGem )
			iReceiveItemID = ITEMID_BlueMarquiseGem;

		if ( iItemID == ITEMID_YellowSquareGem )
			iReceiveItemID = ITEMID_YellowMarquiseGem;

		if ( iItemID == ITEMID_PurpleSquareGem )
			iReceiveItemID = ITEMID_PurpleMarquiseGem;
	}

	EGemSpecial eGemSpecial = EGemSpecial::GEMSPECIAL_None;

	//Sheltom Combination to Elaborated Gems
	if ( iType == 4 )
	{
		if ( iCombinationID == (-1) )
		{
			//Error

			return;
		}

		iItemID = pcItemData->sItem.sItemID.ToItemID();

		if ( iCombinationID == 0 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNMaxAtkPowAdd2;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNMaxAtkPowAdd2;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_AbsorbAdd5;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNMaxAtkPowAdd2;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_BlockAdd1Percent;
			}
		}
		if ( iCombinationID == 1 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNReqDec5PercentStrenght;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNReqDec5PercentSpirit;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNReqDec5PercentTalent;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNReqDec5PercentAgility;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_HPAdd25;
			}
		}
		if ( iCombinationID == 2 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNMaxAtkPowAdd1;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNMaxAtkPowAdd1;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_AtkRtgAdd20;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNMaxAtkPowAdd1;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_AbsorbAdd8;
			}
		}
		if ( iCombinationID == 3 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_HPAdd10;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_MPAdd100;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_DefenseAdd30;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_AtkRtgAdd30;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_AllStatsDec2Percent;
			}
		}
		if ( iCombinationID == 4 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WeightAdd100;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_MPRegen2;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_SPAdd200;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_DefenseAdd40;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_HPRegen2;
			}
		}
		if ( iCombinationID == 5 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_HPRegen1;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_SPAdd200;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_WPNRangeAdd10;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_RunSpeedAdd1;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_DefenseAdd50;
			}
		}
		if ( iCombinationID == 6 )
		{
			if ( iItemID == ITEMID_RedMarquiseGem )
			{
				iReceiveItemID	= ITEMID_RedElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_FireResistance10;
			}
			if ( iItemID == ITEMID_BlueMarquiseGem )
			{
				iReceiveItemID	= ITEMID_BlueElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_IceResistance10;
			}
			if ( iItemID == ITEMID_YellowMarquiseGem )
			{
				iReceiveItemID	= ITEMID_YellowElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_OrganicResistanceAdd10;
			}
			if ( iItemID == ITEMID_GreenMarquiseGem )
			{
				iReceiveItemID	= ITEMID_GreenElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_LightningResistanceAdd10;
			}
			if ( iItemID == ITEMID_PurpleMarquiseGem )
			{
				iReceiveItemID	= ITEMID_PurpleElaboratedGem;
				eGemSpecial		= EGemSpecial::GEMSPECIAL_PoisonResistanceAdd10;
			}
		}
	}

	//Send Item
	psPacket->iResult = 2;
	DefinitionItem * psItemDefinition = ITEMSERVER->FindItemDefByCode( iReceiveItemID );
	if ( psItemDefinition )
	{
		ItemData * pcItemDataS = ITEMSERVER->FindItemPointerTable( iReceiveItemID );
		if ( pcItemDataS )
		{
			Item sItem;
			ITEMSERVER->CreateItem( &sItem, psItemDefinition, 0 );

			CopyMemory( &psPacket->sItemData, pcItemDataS, sizeof( ItemData ) );

			if ( eGemSpecial != EGemSpecial::GEMSPECIAL_None )
				sItem.eMixTypeName = (EMixTypeName)eGemSpecial;

			CopyMemory( &psPacket->sItemData.sItem, &sItem, sizeof( Item ) );

			ITEMSERVER->ValidateItem( &psPacket->sItemData );

			if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->sItemData.sBaseItemID.ToItemID(), psPacket->sItemData.sItem.iChk1, psPacket->sItemData.sItem.iChk2 ) < 0 )
			{
				psPacket->iResult = FALSE;
			}
			psPacket->iLength = sizeof( ItemData ) + 32;
			SENDPACKET( pcUser, psPacket, TRUE );

			CHATSERVER->SendChatMessageBox( pcUser, "Received %s", psPacket->sItemData.sItem.szItemName );
		}
	}

}


void CSocketSystemHandler::OnSocketDrill( User * pcUser, PacketSocketSystem * psPacket )
{
	ItemData * psItemData = &psPacket->sItemData;

	int iSocketSlot = psItemData->sItem.sSocketData[0].eSocketType == 0 ? 0 : 1;

	int iStoneCount = 0;

	int iItemID = psPacket->sRune[0].sItem.sItemID.ToItemID();

	switch ( iItemID )
	{
		case ITEMID_DiamondDrill:
			iStoneCount = 1;
			break;

		case ITEMID_TitaniumDrill:
			iStoneCount = 3;
			break;

		case ITEMID_IronDrill:
			iStoneCount = 5;
			break;

		default:
			break;
	}

	//Check if all runes is the same
	for ( int i = 1; i < iStoneCount; i++ )
	{
		ItemData * ps = psPacket->sRune + i;

		if ( ps->sItem.sItemID.ToItemID() != iItemID )
		{
			//Error
		}
	}

	//Check All valid runes
	for ( int i = 0; i < iStoneCount; i++ )
	{
		ItemData * ps = psPacket->sRune + i;

		if ( ITEMSERVER->UseItem( pcUser->pcUserData, ps->sBaseItemID.ToItemID(), ps->sItem.iChk1, ps->sItem.iChk2 ) < 0 )
		{
			//Error
		}
	}

	//Check Item validate
	if ( ITEMSERVER->UseItem( pcUser->pcUserData, psItemData->sBaseItemID.ToItemID(), psItemData->sItem.iChk1, psItemData->sItem.iChk2 ) < 0 )
	{
		//Error
	}

	ESocketType eType = SOCKETTYPE_None;
	BOOL bSuccess = FALSE;

	//Socket Type
	switch ( iItemID )
	{
		case ITEMID_IronDrill:
			if ( RandomI( 0, 100 ) < 10 )
				bSuccess = TRUE;
			break;
		case ITEMID_TitaniumDrill:
			if ( RandomI( 0, 100 ) < 20 )
				bSuccess = TRUE;
			break;
		case ITEMID_DiamondDrill:
			if ( RandomI( 0, 100 ) < 50 )
				bSuccess = TRUE;
			break;
		default:
			break;
	}

	if ( bSuccess )
	{
		// 5% Prismatic, 19% all
		if ( RandomI( 0, 100 ) < 5 )
			eType = ESocketType::SOCKETTYPE_Prismatic;
		else
			eType = (ESocketType)RandomI( ESocketType::SOCKETTYPE_Emerald, ESocketType::SOCKETTYPE_Count-2 );
	}
	else //Fail
	{
		if ( iSocketSlot == 1 ) //Lose All Sockets
		{
			ZeroMemory( psItemData->sItem.sSocketData, sizeof( SocketItem ) * 2 );
		}
		else
		{
			ZeroMemory( &psItemData->sItem.sSocketData[0], sizeof( SocketItem ) );
		}
	}

	if ( ITEMSERVER->UseItem( pcUser->pcUserData, psItemData->sBaseItemID.ToItemID(), psItemData->sItem.iChk1, psItemData->sItem.iChk2 ) < 0 )
	{

	}

	psItemData->sItem.sSocketData[iSocketSlot].eSocketType = eType;
	ITEMSERVER->ValidateItem( &psItemData->sItem );

	if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, psItemData->sBaseItemID.ToItemID(), psItemData->sItem.iChk1, psItemData->sItem.iChk2 ) < 0 )
	{

	}



	ZeroMemory( psPacket->sRune, sizeof( ItemData ) * _countof( psPacket->sRune ) );

	psPacket->iHeader = PKTHDR_SocketItemDataToClient;
	SENDPACKET( pcUser, psPacket, TRUE );

	if ( bSuccess )
		CHATSERVER->SendChatMessageBox( pcUser, "Socket Creation Success" );
	else
		CHATSERVER->SendChatMessageBox( pcUser, "Socket Creation Failed" );

}

void CSocketSystemHandler::OnSocketStone( User * pcUser, PacketSocketSystem * psPacket )
{
	ItemData * pcItemData = &psPacket->sItemData;
	int iSocket = (pcItemData->sItem.sSocketData[0].eSocketType != 0 && pcItemData->sItem.sSocketData[0].eGemItem != 0) ? 1 : 0;


	ItemData * pcRuneData = &psPacket->sRune[0];

	EGemType eGemType = EGemType::GEMTYPE_None;
	EGemItem eGemItem = EGemItem::GEMITEM_None;

	switch ( pcRuneData->sBaseItemID.ToItemID() )
	{
		case ITEMID_RedTriangularGem:
		case ITEMID_GreenTriangularGem:
		case ITEMID_BlueTriangularGem:
		case ITEMID_YellowTriangularGem:
		case ITEMID_PurpleTriangularGem:
			eGemType = EGemType::GEMTYPE_Triangular;
			break;

		case ITEMID_RedSquareGem:
		case ITEMID_GreenSquareGem:
		case ITEMID_BlueSquareGem:
		case ITEMID_YellowSquareGem:
		case ITEMID_PurpleSquareGem:
			eGemType = EGemType::GEMTYPE_Square;
			break;

		case ITEMID_RedMarquiseGem:
		case ITEMID_GreenMarquiseGem:
		case ITEMID_BlueMarquiseGem:
		case ITEMID_YellowMarquiseGem:
		case ITEMID_PurpleMarquiseGem:
			eGemType = EGemType::GEMTYPE_Marquise;
			break;

		default:
			break;
	}

	switch ( pcRuneData->sBaseItemID.ToItemID() )
	{
		case ITEMID_RedTriangularGem:
		case ITEMID_RedSquareGem:
		case ITEMID_RedMarquiseGem:
			eGemItem = EGemItem::GEMITEM_Ruby;
			break;

		case ITEMID_BlueTriangularGem:
		case ITEMID_BlueSquareGem:
		case ITEMID_BlueMarquiseGem:
			eGemItem = EGemItem::GEMITEM_Aquamarine;
			break;

		case ITEMID_YellowTriangularGem:
		case ITEMID_YellowSquareGem:
		case ITEMID_YellowMarquiseGem:
			eGemItem = EGemItem::GEMITEM_Topaz;
			break;

		case ITEMID_GreenTriangularGem:
		case ITEMID_GreenSquareGem:
		case ITEMID_GreenMarquiseGem:
			eGemItem = EGemItem::GEMITEM_Emerald;
			break;

		case ITEMID_PurpleTriangularGem:
		case ITEMID_PurpleSquareGem:
		case ITEMID_PurpleMarquiseGem:
			eGemItem = EGemItem::GEMITEM_Amethyst;
			break;

		default:
			break;
	}

	BOOL bSuccess = FALSE;
	if ( eGemItem != 0 && eGemType != 0 )
	{
		pcItemData->sItem.sSocketData[iSocket].eGemItem = eGemItem;
		pcItemData->sItem.sSocketData[iSocket].eGemType = eGemType;
		if ( ITEMSERVER->UseItem( pcUser->pcUserData, pcItemData->sBaseItemID.ToItemID(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) < 0 )
		{

		}

		ITEMSERVER->ValidateItem( &pcItemData->sItem );

		if ( ITEMSERVER->AddItemInventory( pcUser->pcUserData, pcItemData->sBaseItemID.ToItemID(), pcItemData->sItem.iChk1, pcItemData->sItem.iChk2 ) < 0 )
		{

		}
		bSuccess = TRUE;
	}


	psPacket->iHeader = PKTHDR_SocketItemDataToClient;
	SENDPACKET( pcUser, psPacket, TRUE );

	if ( bSuccess )
		CHATSERVER->SendChatMessageBox( pcUser, "Socket Stone Success" );
	else
		CHATSERVER->SendChatMessageBox( pcUser, "Socket Stone Failed" );
}
