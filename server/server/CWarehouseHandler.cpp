#include "stdafx.h"
#include "CWarehouseHandler.h"


CWarehouseHandler::CWarehouseHandler()
{
}


CWarehouseHandler::~CWarehouseHandler()
{
}

CWarehouseBase * CWarehouseHandler::Open( const char * pszFile, BOOL bReadFile )
{
	CWarehouseBase * pcWarehouse = new CWarehouseBase;
	ZeroMemory( pcWarehouse, sizeof( CWarehouseBase ) );
	pcWarehouse->strFile	= pszFile;
	pcWarehouse->bEncrypted = TRUE;

	if ( bReadFile )
	{
		FILE * pFile = NULL;
		fopen_s( &pFile, pszFile, "rb" );
		if ( pFile )
		{
			pcWarehouse->bHaveFile = TRUE;

			PacketWarehouse sPacket;

			fseek( pFile, 0, SEEK_END );
			UINT uFileSize = ftell( pFile );

			UINT uBufferPosition = 0;
			while ( uBufferPosition < uFileSize )
			{
				//Read Packet Size
				fseek( pFile, uBufferPosition, SEEK_SET );
				fread( &sPacket, 4, 1, pFile );

				//Get Packet Buffer Block
				fseek( pFile, uBufferPosition, SEEK_SET );
				fread( &sPacket, sPacket.iLength, 1, pFile );

				//Add Packet
				Receive( pcWarehouse, &sPacket );

				//Last?
				if ( pcWarehouse->sPacketCount.iMin == pcWarehouse->sPacketCount.iMax )
					break;

				//Next Block
				uBufferPosition += sPacket.iLength;
			}

			fclose( pFile );
		}
	}

	return pcWarehouse;
}

void CWarehouseHandler::Receive( CWarehouseBase * pcWarehouse, PacketWarehouse * psPacket )
{
	if ( pcWarehouse )
	{
		//First?
		if ( pcWarehouse->sPacketCount.iMin == 0 )
		{
			//Set Page
			pcWarehouse->iPageCount			= psPacket->iMaxPage;
			
			//Update Packet Count
			pcWarehouse->sPacketCount.iMax	= psPacket->iMaxPage;
			if ( pcWarehouse->sPacketCount.iMax == 0 )
				pcWarehouse->sPacketCount.iMax = 1;

			//Set Opened User Pages
			PacketWarehouse * ps		= (PacketWarehouse*)&psPacket->baData[8];
			pcWarehouse->iMaxPageUser	= ps->iMaxPage;
		}

		//More Packets to Receive?
		if ( pcWarehouse->sPacketCount.iMin < pcWarehouse->sPacketCount.iMax )
		{
			//Next Packet
			pcWarehouse->sPacketCount.iMin++;

			//Error?
			if ( psPacket->uBufferCount > 0x1F00 )
			{
				if ( pcWarehouse->bHaveFile )
					pcWarehouse->bHaveFile = FALSE;

				pcWarehouse->sPacketCount.iMin = pcWarehouse->sPacketCount.iMax;
				return;
			}

			//Add Data Packet into Buffer
			CopyMemory( pcWarehouse->baData + pcWarehouse->uBufferPosition, &psPacket->baData[8], psPacket->uBufferCount );
			pcWarehouse->uBufferPosition += psPacket->uBufferCount;
		}
	}
}

BOOL CWarehouseHandler::Send( User * pcUser, CWarehouseBase * pcWarehouse )
{
	if ( pcWarehouse )
	{
		//Have File to send?
		if ( HaveFile( pcWarehouse ) )
		{
			//Encrypt
			if ( pcWarehouse->bEncrypted == FALSE )
				Encrypt( pcWarehouse );

			//Get Packets on Encrypted Buffer
			std::vector<PacketWarehouse*> vPackets = GetEncryptedBuffer( pcWarehouse );

			for ( std::vector<PacketWarehouse*>::iterator it = vPackets.begin(); it != vPackets.end(); it++ )
			{
				PacketWarehouse * psPacket = (*it);

				//Send
				SENDPACKET( pcUser, psPacket );

				DELET( psPacket );
			}

			//Clear Packets
			vPackets.clear();
		}
		else
		{
			//Send Empty Warehouse, just for open
			PacketWarehouse sPacket;
			ZeroMemory( &sPacket, sizeof( PacketWarehouse ) );
			sPacket.iLength		= sizeof( PacketWarehouse ) - (_countof( sPacket.baData ) * sizeof( BYTE ));
			sPacket.iHeader		= PKTHDR_SendWarehouse;
			sPacket.iMaxPage	= 1;
			SENDPACKET( pcUser, &sPacket );
		}
	}

	return TRUE;
}

BOOL CWarehouseHandler::Encrypt( CWarehouseBase * pcWarehouse )
{
	if ( pcWarehouse )
	{
		if ( pcWarehouse->psWarehouse && (pcWarehouse->bEncrypted == FALSE) )
		{
			//Clear
			ZeroMemory( pcWarehouse->baData, 331748 );

			//Encrypt
			PacketWarehouse * psPacket = (PacketWarehouse*)pcWarehouse->baData;
			int iEncryptSize = ITEMSERVER->EncodeItemsData( pcWarehouse->psWarehouse, psPacket->baData, sizeof( WarehouseData ), (sizeof( WarehouseData ) - 0x10) );
			iEncryptSize += 0x120;

			//Get Page Count
			pcWarehouse->iPageCount = (iEncryptSize / 0x1F00);
			if ( ((iEncryptSize % 0x1F00) > 0) && (iEncryptSize > 0x1F00) )
				pcWarehouse->iPageCount++;

			//Update Packet Count
			pcWarehouse->sPacketCount.iMax = pcWarehouse->iPageCount;		
			if ( pcWarehouse->sPacketCount.iMax == 0 )
				pcWarehouse->sPacketCount.iMax = 1;
			pcWarehouse->sPacketCount.iMin = pcWarehouse->sPacketCount.iMax;

			//Packet Header
			psPacket->iLength			= iEncryptSize;
			psPacket->iHeader			= PKTHDR_Warehouse;
			psPacket->dwCheckSum		= GetChecksum( pcWarehouse->psWarehouse, sizeof( WarehouseData ) );
			psPacket->iMaxPage			= pcWarehouse->iMaxPageUser;
			psPacket->iGold				= (pcWarehouse->iGold + 2023) ^ psPacket->iHeader ^ psPacket->dwCheckSum;
			psPacket->iDataLength		= iEncryptSize - 0x120;

			//Clear Memory
			DELET( pcWarehouse->psWarehouse );

			pcWarehouse->bEncrypted = TRUE;

			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWarehouseHandler::Decrypt( CWarehouseBase * pcWarehouse )
{
	if ( pcWarehouse )
	{
		if ( pcWarehouse->bEncrypted )
		{
			PacketWarehouse * psPacket = (PacketWarehouse*)pcWarehouse->baData;

			//Clear Memory
			DELET( pcWarehouse->psWarehouse );

			//New Warehouse
			pcWarehouse->psWarehouse = new WarehouseData;
			ZeroMemory( pcWarehouse->psWarehouse, sizeof( WarehouseData ) );

			//Gold
			pcWarehouse->iGold = (int)(((psPacket->iGold ^ psPacket->dwCheckSum) ^ psPacket->iHeader) - 2023);
			if ( pcWarehouse->iGold == (-2023) )
				pcWarehouse->iGold = 0;

			//Decrypt Data
			ITEMSERVER->DecodeItemsData( psPacket->baData, pcWarehouse->psWarehouse, sizeof( WarehouseData ) );

			pcWarehouse->bEncrypted = FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWarehouseHandler::Save( CWarehouseBase * pcWarehouse )
{
	if ( pcWarehouse )
	{
		//Open File
		FILE * pFile = NULL;
		fopen_s( &pFile, pcWarehouse->strFile.c_str(), "wb" );
		if ( pFile )
		{
			//Get Packets on Encrypted Buffer
			std::vector<PacketWarehouse*> vPackets = GetEncryptedBuffer( pcWarehouse );

			//Buffer Position for file
			UINT uBufferPosition = 0;

			for ( std::vector<PacketWarehouse*>::iterator it = vPackets.begin(); it != vPackets.end(); it++ )
			{
				PacketWarehouse * psPacket = (*it);

				//Write Packet into file
				fseek( pFile, uBufferPosition, SEEK_SET );
				fwrite( psPacket, psPacket->iLength, 1, pFile );

				//Next Block to write
				uBufferPosition += psPacket->iLength;

				DELET( psPacket );
			}

			//Clear Packets
			vPackets.clear();

			fclose( pFile );

			//Success
			return TRUE;
		}
	}
	return FALSE;
}

void CWarehouseHandler::Close( CWarehouseBase * pcWarehouse )
{
	//Clear Memory
	DELET( pcWarehouse->psWarehouse );
	DELET( pcWarehouse );
}

void CWarehouseHandler::HandlePacket( User * pcUser, PacketWarehouseOpen * psPacket )
{
	char szFile[MAX_PATH] = { 0 };

	//Opened?
	if ( pcUser->pcUserData->bWarehouseOpen )
		return;

	//Check if can Open
	if ( pcUser->IsAccountShareDenied( ACCOUNTSHARE_DenyWarehouse ) )
	{
		CHATSERVER->SendChatMessageBox( pcUser, "You can't open Warehouse, because you're logged in with the Share PW!" );
		return;
	}

	//Open File
	STRINGFORMAT( szFile, "Data\\Warehouse\\%s.dat", pcUser->pcUserData->szAccountName );
	CWarehouseBase * pcWarehouse = Open( szFile );

	//Decrypt
	Decrypt( pcWarehouse );

	//Set Gold
	pcUser->iWarehouseGold = pcWarehouse->iGold;

	//Log Gold
	SQLGoldLog( pcUser, pcWarehouse->iGold, TRUE );

	//Add to Server Inventory
	AddWarehouseInventoryItems( pcUser, pcWarehouse );

	//Send to User
	Send( pcUser, pcWarehouse );

	//Opened!
	pcUser->pcUserData->bWarehouseOpen = TRUE;

	//Close and Clear Warehouse Data
	Close( pcWarehouse );
}

void CWarehouseHandler::HandlePacket( User * pcUser, PacketWarehouse * psPacket )
{
	if ( pcUser->pcUserData->bWarehouseOpen )
	{
		CWarehouseBase * pcWarehouse = pcUser->pcUserData->pcWarehouse;

		//Open File
		if ( pcWarehouse == NULL )
		{
			char szFile[MAX_PATH] = { 0 };
			STRINGFORMAT( szFile, "Data\\Warehouse\\%s.dat", pcUser->pcUserData->szAccountName );
			pcWarehouse = Open( szFile, FALSE );

			pcUser->pcUserData->pcWarehouse = pcWarehouse;
		}

		//Receive Data
		Receive( pcWarehouse, psPacket );

		//Last Packet?
		if ( pcWarehouse->sPacketCount.iMin == pcWarehouse->sPacketCount.iMax )
		{
			//Decrypt
			Decrypt( pcWarehouse );

			//Add Gold to User
			USERSERVER->AddServerUserGold( pcUser->pcUserData, pcUser->iWarehouseGold, WHEREID_OpenWarehouse );
			
			//Decrease
			pcUser->iWarehouseGold -= pcWarehouse->iGold;
			
			//Subtract for check
			USERSERVER->SubServerUserGold( pcUser->pcUserData, pcWarehouse->iGold, WHEREID_OpenWarehouse );		
			
			SQLGoldLog( pcUser, pcWarehouse->iGold, FALSE );

			//Everything good?
			if ( CheckWarehouseInventoryItems( pcUser, pcWarehouse ) )
			{
				//Make Backup
				MakeBackup( pcUser );

				//Encrypt
				Encrypt( pcWarehouse );

				//Save Warehouse
				Save( pcWarehouse );

				//Closed
				pcUser->pcUserData->bWarehouseOpen = FALSE;

				//Send Success
				SENDPACKETBLANK( pcUser, PKTHDR_WarehouseSaveSuccess, TRUE );
			}

			//Close and Clear Warehouse Data
			Close( pcWarehouse );

			pcUser->pcUserData->pcWarehouse = NULL;
		}
	}
}

DWORD CWarehouseHandler::GetChecksum( WarehouseData * psData, UINT uSize )
{
	DWORD dwRet = 0;
	for ( UINT i = 0; i < uSize; i++ )
		dwRet += ((BYTE*)psData)[i] ^ 0x6A;

	return dwRet;
}

std::vector<PacketWarehouse*> CWarehouseHandler::GetEncryptedBuffer( CWarehouseBase * pcWarehouse )
{
	std::vector<PacketWarehouse*> v;

	if ( pcWarehouse->bEncrypted )
	{
		PacketWarehouse * psPacketBase = (PacketWarehouse*)pcWarehouse->baData;

		//Total Size of Buffer
		UINT uSize = psPacketBase->iLength;

		//Buffer Position
		UINT uBufferPosition = 0;

		pcWarehouse->sPacketCount.iMax = (uSize / 0x1F00);
		if ( (uSize % 0x1F00) > 0 && (uSize > 0x1F00) )
			pcWarehouse->sPacketCount.iMax++;

		int iCurrentPage = 0;

		//Create Packets
		do
		{
			//Write Encrypted Buffer into Packet Block
			PacketWarehouse * psPacket = new PacketWarehouse;
			ZeroMemory( psPacket, sizeof( PacketWarehouse ) );
	
			//Size of Block
			UINT uDataSize = uSize - uBufferPosition;
			if ( uDataSize > 0x1F00 )
				uDataSize = 0x1F00;
		
			//Packet Header
			psPacket->iLength		= uDataSize + 0x100;
			psPacket->iHeader		= PKTHDR_Warehouse;
			psPacket->iCurrentPage	= iCurrentPage++;
			psPacket->iMaxPage		= pcWarehouse->iPageCount;
			psPacket->uBufferCount	= uDataSize;

			//Copy into Packet
			CopyMemory( &psPacket->baData[8], pcWarehouse->baData + uBufferPosition, uDataSize );

			//Add to Packets vector
			v.push_back( psPacket );

			//Next Buffer
			uBufferPosition += uDataSize;
		}
		while ( uBufferPosition < uSize );
	}

	return v;
}

void CWarehouseHandler::SQLGoldLog( User * pcUser, int iGold, BOOL bOpenWarehouse )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		char * pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,4,?,?,?,0,GETDATE())";
		if ( bOpenWarehouse )
			pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,3,?,?,?,0,GETDATE())";

		if ( pcDB->Prepare( pszQuery ) )
		{
			int iCode1 = ITEMID_Gold;
			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, (char*)"Gold", 4 );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iCode1 );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iGold );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CWarehouseHandler::SQLItemLog( User * pcUser, int iItemID, int iChk1, int iChk2, BOOL bGotItem )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_LogDB );
	if ( pcDB->Open() )
	{
		char * pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,1,?,?,?,?,GETDATE())";
		if ( bGotItem )
			char * pszQuery = "INSERT INTO WarehouseLog VALUES(?,?,2,?,?,?,?,GETDATE())";

		if ( pcDB->Prepare( pszQuery ) )
		{
			char * pszItemName = "Unknown";
			ItemData * psItemWH = ITEMSERVER->FindItemPointerTable( iItemID );
			if ( psItemWH )
				pszItemName = psItemWH->szBaseName;

			pcDB->BindParameterInput( 1, PARAMTYPE_String, pcUser->pcUserData->szAccountName, 32 );
			pcDB->BindParameterInput( 2, PARAMTYPE_String, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ), 32 );
			pcDB->BindParameterInput( 3, PARAMTYPE_String, pszItemName, lstrlenA( pszItemName ) );
			pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iItemID );
			pcDB->BindParameterInput( 5, PARAMTYPE_Integer, &iChk1 );
			pcDB->BindParameterInput( 6, PARAMTYPE_Integer, &iChk2 );
			pcDB->Execute();
		}
		pcDB->Close();
	}
}

void CWarehouseHandler::AddWarehouseInventoryItems( User * pcUser, CWarehouseBase * pcWarehouse )
{
	//Clear Warehouse Inventory
	ZeroMemory( pcUser->sWarehouseItems, sizeof( DropItemData ) * 420 );

	int iCount = 0;
	//Add items to inventory
	for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
	{
		ItemData * pc = pcWarehouse->psWarehouse->saItem + i;
		if ( pc->bValid )
		{
			pcUser->sWarehouseItems[iCount].iItemID = pc->sItem.sItemID.ToItemID();
			pcUser->sWarehouseItems[iCount].iChk1	= pc->sItem.iChk1;
			pcUser->sWarehouseItems[iCount].iChk2	= pc->sItem.iChk2;
			iCount++;
			
			ITEMSERVER->SaveItemData( &pc->sItem );

			//Potion
			if ( pc->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
				ITEMSERVER->AddPotion( pcUser->pcUserData, pc->sItem.sItemID.ToItemID(), pc->sItem.iPotionCount );
		}
	}
}

BOOL CWarehouseHandler::CheckWarehouseInventoryItems( User * pcUser, CWarehouseBase * pcWarehouse )
{
	BOOL bContinue = TRUE;

	//Check Put Items
	for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
	{
		ItemData * pc = pcWarehouse->psWarehouse->saItem + i;
		if ( pc->bValid )
		{
			BOOL bFound = FALSE;

			//Find Item on Warehouse
			for ( int j = 0; j < WAREHOUSE_ITEM_MAX; j++ )
			{
				DropItemData * psItemWarehouse = pcUser->sWarehouseItems + j;

				//Found Item on Warehouse?
				if ( psItemWarehouse->iItemID == pc->sItem.sItemID.ToItemID() &&
					psItemWarehouse->iChk1 == pc->sItem.iChk1 &&
					psItemWarehouse->iChk2 == pc->sItem.iChk2 )
				{
					//Set Found
					bFound = TRUE;
					break;
				}
			}

			//Not Found?
			if ( !bFound )
			{
				//Log Put
				SQLItemLog( pcUser, pc->sItem.sItemID.ToInt(), pc->sItem.iChk1, pc->sItem.iChk2, FALSE );

				//Item Not Found?
				if ( ITEMSERVER->UseItem( pcUser->pcUserData, pc->sItem.sItemID.ToInt(), pc->sItem.iChk1, pc->sItem.iChk2 ) < 0 )
				{
					//Hack Clone Warehouse Log
					PacketLogCheat sPacketCheat;
					sPacketCheat.iCheatID = CHEATLOGID_WarehouseNewClone;
					sPacketCheat.LParam = pc->sItem.sItemID.ToItemID();
					sPacketCheat.SParam = pc->sItem.iChk1;
					sPacketCheat.EParam = pc->sItem.iChk2;
					ITEMSERVER->SaveItemData( &pc->sItem );
					LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacketCheat );
					bContinue = FALSE;
					break;
				}
				else
				{
					//Add to Warehouse Inventory
					DropItemData sItem;
					sItem.iItemID = pc->sItem.sItemID.ToItemID();
					sItem.iChk1 = pc->sItem.iChk1;
					sItem.iChk2 = pc->sItem.iChk2;
					AddItemWarehouse( pcUser, pc->sItem.sItemID.ToItemID(), pc->sItem.iChk1, pc->sItem.iChk2 );
					ITEMSERVER->SaveItemData( &pc->sItem );

					//Potion
					if ( pc->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
					{
						ITEMSERVER->AddPotion( pcUser->pcUserData, pc->sItem.sItemID.ToItemID(), (-pc->sItem.iPotionCount) );
					}
				}
			}
		}
	}

	//Check Got Items
	if ( bContinue )
	{
		for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
		{
			DropItemData * psItemWarehouse = pcUser->sWarehouseItems + i;

			if ( psItemWarehouse->iItemID )
			{
				BOOL bFound = FALSE;

				int iIndex = -1;

				//Find Item on Warehouse
				for ( int j = 0; j < WAREHOUSE_ITEM_MAX; j++ )
				{
					ItemData * pc = pcWarehouse->psWarehouse->saItem + j;
					if ( pc->bValid )
					{
						//Found item on Warehouse?
						if ( psItemWarehouse->iItemID == pc->sItem.sItemID.ToItemID() &&
							psItemWarehouse->iChk1 == pc->sItem.iChk1 &&
							psItemWarehouse->iChk2 == pc->sItem.iChk2 )
						{
							//Set Found
							iIndex = j;
							bFound = TRUE;
							break;
						}
					}
				}

				//Not Found? Log it
				if ( bFound == FALSE )
				{
					//Log Item
					SQLItemLog( pcUser, psItemWarehouse->iItemID, psItemWarehouse->iChk1, psItemWarehouse->iChk2, TRUE );

					//Add to User Inventory
					ITEMSERVER->AddItemInventory( pcUser->pcUserData, psItemWarehouse->iItemID, psItemWarehouse->iChk1, psItemWarehouse->iChk2 );

					DelItemWarehouse( pcUser, psItemWarehouse->iItemID, psItemWarehouse->iChk1, psItemWarehouse->iChk2 );
				}
				else
				{
					ItemData * pc = pcWarehouse->psWarehouse->saItem + iIndex;

					//Potion?
					if ( pc->sItem.sItemID.ToItemBase() == ITEMBASE_Potion )
					{
						ITEMSERVER->AddPotion( pcUser->pcUserData, pc->sItem.sItemID.ToItemID(), pc->sItem.iPotionCount );
					}
				}
			}
		}
	}

	return bContinue;
}

void CWarehouseHandler::MakeBackup( User * pcUser )
{
	char szFile[MAX_PATH] = { 0 };
	STRINGFORMAT( szFile, "Data\\Warehouse\\%s.dat", pcUser->pcUserData->szAccountName );

	char szNewFile[MAX_PATH] = { 0 };
	STRINGFORMAT( szNewFile, "Data\\WarehouseBackup\\%s.dat", pcUser->pcUserData->szAccountName );

	CopyFileA( szFile, szNewFile, FALSE );
}

BOOL CWarehouseHandler::AddItemWarehouse( User * pcUser, int iItemID, int iChk1, int iChk2 )
{
	BOOL bRet = FALSE;

	for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
	{
		if ( pcUser->sWarehouseItems[i].iItemID == 0 )
		{
			pcUser->sWarehouseItems[i].iItemID = iItemID;
			pcUser->sWarehouseItems[i].iChk1 = iChk1;
			pcUser->sWarehouseItems[i].iChk2 = iChk2;
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL CWarehouseHandler::DelItemWarehouse( User * pcUser, int iItemID, int iChk1, int iChk2 )
{
	BOOL bRet = FALSE;

	for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
	{
		if ( pcUser->sWarehouseItems[i].iItemID )
		{
			if ( (pcUser->sWarehouseItems[i].iItemID == iItemID) &&
				 (pcUser->sWarehouseItems[i].iChk1 == iChk1) &&
				 (pcUser->sWarehouseItems[i].iChk2 == iChk2) )
			{
				pcUser->sWarehouseItems[i].iItemID = 0;
				pcUser->sWarehouseItems[i].iChk1 = 0;
				pcUser->sWarehouseItems[i].iChk2 = 0;
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

int CWarehouseHandler::GetItemWarehouseIndex( User * pcUser, int iItemID, int iChk1, int iChk2 )
{
	int iIndex = -1;

	for ( int i = 0; i < WAREHOUSE_ITEM_MAX; i++ )
	{
		if ( pcUser->sWarehouseItems[i].iItemID )
		{
			if ( (pcUser->sWarehouseItems[i].iItemID == iItemID) &&
				(pcUser->sWarehouseItems[i].iChk1 == iChk1) &&
				(pcUser->sWarehouseItems[i].iChk2 == iChk2) )
			{
				iIndex = i;
				break;
			}
		}
	}

	return iIndex;
}

void CWarehouseHandler::ClearWarehouseInventory( User * pcUser )
{
}
