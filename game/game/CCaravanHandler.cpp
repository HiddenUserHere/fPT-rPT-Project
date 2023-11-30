#include "stdafx.h"
#include "CCaravanHandler.h"


CCaravanHandler::CCaravanHandler()
{
	pcWindow = new CCaravanWindowHandler();
	pcCaravanBase = nullptr;
}


CCaravanHandler::~CCaravanHandler()
{
	DELET( pcWindow );
	if ( pcCaravanBase )
		DELET( pcCaravanBase->psCaravanData );
	DELET( pcCaravanBase );
}

void CCaravanHandler::Init()
{
	pcWindow->Init();
}

void CCaravanHandler::OpenNPC()
{
	ProcessPacket( &PacketCaravanOpen() );
}

void CCaravanHandler::HandlePacket( PacketCaravanOpen * psPacket )
{
	ProcessPacket( psPacket );
}

void CCaravanHandler::HandlePacket( PacketCaravan * psPacket )
{
	if ( psPacket->IsFirst() )
		pcWindow->ClearItems();

	if ( pcCaravanBase == nullptr )
	{
		pcCaravanBase = new CaravanBase();
		pcCaravanBase->uBufferCount = 0;
	}

	Receive( psPacket );

	if ( psPacket->IsOnly() )
	{
		pcCaravanBase->psCaravanData = new CaravanData;
		ZeroMemory( pcCaravanBase->psCaravanData, sizeof( CaravanData ) );

		int iLength = ITEMHANDLER->DecodeItemsData( pcCaravanBase->baBuffer, pcCaravanBase->psCaravanData->caItemData, sizeof( ItemData ) * MAX_CARAVAN_ITEMS );
		if ( iLength > 0 )
		{
			pcCaravanBase->psCaravanData->iItemCount = psPacket->iItemCount;
			for ( int i = 0; i < pcCaravanBase->psCaravanData->iItemCount; i++ )
				pcWindow->AddItem( pcCaravanBase->psCaravanData->caItemData + i );
		}

		pcWindow->Open();

		DELET( pcCaravanBase->psCaravanData );
		DELET( pcCaravanBase );
	}
}

void CCaravanHandler::HandlePacket( PacketCaravanUpdate * psPacket )
{
	pcWindow->UpdateCaravanData( psPacket->szName, psPacket->bFollow );
}

void CCaravanHandler::SaveCaravan( std::vector<ItemData*> vItems, const std::string strName, bool bFollow )
{
	CaravanData * psCaravanData = new CaravanData;
	ZeroMemory( psCaravanData, sizeof( CaravanData ) );

	for ( const auto pc : vItems )
		CopyMemory( &psCaravanData->caItemData[psCaravanData->iItemCount++], pc, sizeof( ItemData ) );

	BYTE * baData = new BYTE[sizeof( ItemData ) * _countof( psCaravanData->caItemData )];
	ZeroMemory( baData, sizeof( ItemData ) * _countof( psCaravanData->caItemData ) );

	int iLength = ITEMHANDLER->EncodeItemsData( psCaravanData->caItemData, baData, sizeof( ItemData ) * _countof( psCaravanData->caItemData ) );
	if ( iLength >= 0 )
	{
		PacketCaravan sPacket;
		std::vector<PacketCaravan> vPackets;

		sPacket.sPacketCount.sMin = 1;

		sPacket.iItemCount = psCaravanData->iItemCount;

		UINT uBufferPosition = 0;
		if ( iLength > 0 )
		{
			while ( uBufferPosition < (UINT)iLength )
			{
				sPacket.uBufferCount = (int)(uBufferPosition + MAX_CARAVAN_BUFFERSIZE) <= iLength ? MAX_CARAVAN_BUFFERSIZE : (iLength - uBufferPosition);
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

	DELET( psCaravanData );

	//Update
	PacketCaravanUpdate s;
	s.bFollow = bFollow;
	STRINGCOPY( s.szName, strName.c_str() );
	ProcessPacket( &s );

	SAVE;
}

ItemTimer * CCaravanHandler::GetCaravanItemTimer()
{
	ItemTimer * pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanArma );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanRabie );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanTobie );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanCuepy );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanHungky );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanBuma );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanHopy );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanRudolph );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanPingu );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanYeti );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanSirKnight );
	if ( pcItemTimer == nullptr )
		pcItemTimer = ITEMTIMERHANDLER->GetItemTimer( ITEMTIMERTYPE_CaravanGoldenHopy );

	return pcItemTimer;
}

void CCaravanHandler::UpdateCaravanParticle( Unit * pcUnit )
{
	auto IsGoldenHopyEffect = []( const char szBodyModel[64] ) -> bool
	{
		int iLength = STRLEN( szBodyModel );
		if ( (szBodyModel[25] == 'G') && (szBodyModel[26] == '_') && (szBodyModel[27] == 'H') && (szBodyModel[28] == 'o') && (szBodyModel[29] == 'p') && (szBodyModel[30] == 't') )
			return true;

		if ( (szBodyModel[12] == 'c') && (((szBodyModel[14] == '0') && (szBodyModel[15] == '4') && (szBodyModel[16] == '5')) || ((szBodyModel[15] == '0') && (szBodyModel[16] == '4') && (szBodyModel[17] == '5'))) )
			return true;

		return false;
	};

	if ( IsGoldenHopyEffect( pcUnit->pcUnitData->sCharacterData.Player.szBodyModel ) )
	{
		if ( pcUnit->bSpecialEffect == false )
		{
			if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\G_Hopt.efk" ) )
			{
				pcScript->SetID( PARTICLEID_CARAVAN_GOLDEN_HOPY );

				auto pcOwner = new Engine::ObjectOwnerUnit( pcUnit->pcUnitData );
				pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
				pcScript->SetOwner( pcOwner );

				EFFEKSEERHANDLER->AddScript( pcScript );
			}

			pcUnit->bSpecialEffect = true;
		}
	}
	else
	{
		if ( pcUnit->bSpecialEffect )
		{
			EFFEKSEERHANDLER->Kill( PARTICLEID_CARAVAN_GOLDEN_HOPY, pcUnit->GetID() );

			pcUnit->bSpecialEffect = false;
		}
	}
}

void CCaravanHandler::Receive( PacketCaravan * psPacket )
{
	if ( psPacket->uBufferCount <= MAX_CARAVAN_BUFFERSIZE )
	{
		if ( (pcCaravanBase->uBufferCount + psPacket->uBufferCount) <= MAX_CARAVAN_DATASIZE )
		{
			//First?
			if ( psPacket->IsFirst() )
			{
				ZeroMemory( pcCaravanBase->baBuffer, MAX_CARAVAN_DATASIZE );
				pcCaravanBase->uBufferCount = 0;
			}

			CopyMemory( pcCaravanBase->baBuffer + pcCaravanBase->uBufferCount, psPacket->baBuffer, psPacket->uBufferCount );
			pcCaravanBase->uBufferCount += psPacket->uBufferCount;
		}
	}
}

void CCaravanHandler::ProcessPacket( PacketCaravan * psPacket )
{
	psPacket->iLength = sizeof( Packet ) + psPacket->uBufferCount + sizeof( psPacket->uBufferCount ) + sizeof( psPacket->sPacketCount ) + sizeof( psPacket->iItemCount );
	psPacket->iHeader = PKTHDR_NPCCaravan;
	SENDPACKETL( psPacket );
}

void CCaravanHandler::ProcessPacket( PacketCaravanOpen * psPacket )
{
	psPacket->iLength = sizeof( PacketCaravanOpen );
	psPacket->iHeader = PKTHDR_NPCCaravanOpen;
	
	SENDPACKETL( psPacket );
}

void CCaravanHandler::ProcessPacket( PacketCaravanUpdate * psPacket )
{
	psPacket->iLength = sizeof( PacketCaravanUpdate );
	psPacket->iHeader = PKTHDR_NPCCaravanUpdate;
	SENDPACKETG( psPacket );
}
