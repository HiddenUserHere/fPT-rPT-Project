#include "stdafx.h"
#include "CChristmasTreeHandler.h"


CChristmasTreeHandler::CChristmasTreeHandler()
{
	pcWindow = new CChristmasTreeWindowHandler();
	pcWindowGift = new CChristmasGiftWindowHandler();
	pcXmasTreeBase = nullptr;
}


CChristmasTreeHandler::~CChristmasTreeHandler()
{
	DELET( pcWindow );
	DELET( pcWindowGift );
	if ( pcXmasTreeBase )
		DELET( pcXmasTreeBase->psXmasTreeData );
	DELET( pcXmasTreeBase );
}

void CChristmasTreeHandler::Init()
{
	pcWindow->Init();
	pcWindowGift->Init();
}

void CChristmasTreeHandler::Update( float fTime )
{
	pcWindow->Update( fTime );
	pcWindowGift->Update( fTime );
}

void CChristmasTreeHandler::OpenXmasTreeNPC( const char * pszCharacterName, int iCharacterID )
{
	this->iCharacterID = iCharacterID;
	pcWindow->Open( pszCharacterName );
}

void CChristmasTreeHandler::OpenXmasTreeGiftNPC()
{
	PacketXmasTreeItemNPC sPacket;
	sPacket.bItem = FALSE;
	ProcessPacket( &sPacket );
}

void CChristmasTreeHandler::HandlePacket( PacketXmasTreeOpen * psPacket )
{
	OpenXmasTreeNPC( psPacket->szCharacterNameOwner, psPacket->iUnitID );
}

void CChristmasTreeHandler::HandlePacket( PacketXmasTreeItemNPC * psPacket )
{
	pcWindowGift->Open( psPacket->iItemCount );
}

void CChristmasTreeHandler::HandlePacket( PacketXmasTreeResponse * psPacket )
{
	if ( psPacket->iResult == 1 )
	{
		ChatGame::AddChatBoxText( "Xmas Tree> Gift(s) sent successful!", CHATCOLOR_Global );
		TITLEBOX( "Gift(s) sent successful!" );
		GetWindow()->Close();
		GetWindow()->ResetTimeWait();
	}
	else if ( psPacket->iResult == 2 )
	{
		ChatGame::AddChatBoxText( "Nataly> The gift(s) has been sent to Item Distributor!", CHATCOLOR_Global );
		TITLEBOX( "The gift(s) has been sent to Item Distributor!" );
		GetWindowGift()->Close();
		GetWindowGift()->ResetTimeWait();
	}
	else
	{
		ChatGame::AddChatBoxText( "> Unknown error occured!" );
		GetWindow()->Close();
		GetWindow()->ResetTimeWait();
		GetWindowGift()->Close();
		GetWindowGift()->ResetTimeWait();
	}
}

void CChristmasTreeHandler::SendItems( std::vector<ItemData*> vItems )
{
	XmasTreeData * psXmasTreeData = new XmasTreeData;
	ZeroMemory( psXmasTreeData, sizeof( XmasTreeData ) );

	for ( const auto pc : vItems )
		CopyMemory( &psXmasTreeData->caItemData[psXmasTreeData->iItemCount++], pc, sizeof( ItemData ) );

	BYTE * baData = new BYTE[sizeof( ItemData ) * _countof( psXmasTreeData->caItemData )];
	ZeroMemory( baData, sizeof( ItemData ) * _countof( psXmasTreeData->caItemData ) );

	int iLength = ITEMHANDLER->EncodeItemsData( psXmasTreeData->caItemData, baData, sizeof( ItemData ) * _countof( psXmasTreeData->caItemData ) );
	if ( iLength >= 0 )
	{
		PacketXmasTree sPacket;
		std::vector<PacketXmasTree> vPackets;

		sPacket.sPacketCount.sMin = 1;

		sPacket.iItemCount = psXmasTreeData->iItemCount;

		sPacket.iCharacterID = iCharacterID;

		UINT uBufferPosition = 0;
		if ( iLength > 0 )
		{
			while ( uBufferPosition < (UINT)iLength )
			{
				sPacket.uBufferCount = (int)(uBufferPosition + MAX_XMASTREE_BUFFERSIZE) <= iLength ? MAX_XMASTREE_BUFFERSIZE : (iLength - uBufferPosition);
				CopyMemory( sPacket.baBuffer, baData + uBufferPosition, sPacket.uBufferCount );

				vPackets.push_back( sPacket );

				sPacket.sPacketCount.sMin++;

				uBufferPosition += sPacket.uBufferCount;
			}
		}
		else
			vPackets.push_back( sPacket );

		for ( auto & s : vPackets )
		{
			s.sPacketCount.sMax = (short)vPackets.size();
			ProcessPacket( &s );
		}

		vPackets.clear();
	}

	DELETA( baData );

	DELET( psXmasTreeData );

	SAVE;
}

void CChristmasTreeHandler::ProcessPacket( PacketXmasTreeItemUse * psPacket )
{
	psPacket->iLength = sizeof( PacketXmasTreeItemUse );
	psPacket->iHeader = PKTHDR_XmasTreeItemUse;
	SENDPACKETG( psPacket );
}

bool CChristmasTreeHandler::IsNearNPC( Point3D sPosition )
{
	bool bRet = false;

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		UnitData * pcUnitData = UNITGAME->pcaUnitData + i;
		Unit * pcUnit = UNITDATATOUNIT( pcUnitData );
		if ( pcUnitData->bActive && (pcUnitData->sCharacterData.iType == CHARACTERTYPE_NPC) && (pcUnit && (pcUnit->bFollowUnitType == false)) )
		{
			if ( sPosition.WithinPTDistance( &pcUnitData->sPosition, 100 ) )
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

void CChristmasTreeHandler::ProcessPacket( PacketXmasTree * psPacket )
{
	psPacket->iLength = sizeof( PacketXmasTree );
	psPacket->iHeader = PKTHDR_XmasTree;
	SENDPACKETL( psPacket );
}

void CChristmasTreeHandler::ProcessPacket( PacketXmasTreeItemNPC * psPacket )
{
	psPacket->iLength = sizeof( PacketXmasTreeItemNPC );
	psPacket->iHeader = PKTHDR_XmasTreeNPCGift;
	SENDPACKETL( psPacket );
}
