#include "stdafx.h"
#include "CMountServerHandler.h"

CMountServerHandler::CMountServerHandler()
{
}

CMountServerHandler::~CMountServerHandler()
{
	Clear();
}

void CMountServerHandler::Clear()
{
	for ( auto psMount : vMountsList )
		DELET( psMount );

	vMountsList.clear();
}

void CMountServerHandler::Init()
{
	SQLLoadMounts();
}

void CMountServerHandler::OnLoadUser( User * pcUser )
{
	PacketMountUserList sPacket;
	sPacket.iCount = 0;
	sPacket.sPacketCount.sMax = sPacket.sPacketCount.sMin = 0;

	std::vector<MountUserListData> vMounts;
	std::vector<PacketMountUserList> vPackets;

	if ( SQLGetUserMounts( pcUser, vMounts ) )
	{
		for ( auto & sData : vMounts )
		{
			sPacket.saMounts[sPacket.iCount].iID			= sData.iID;
			STRINGCOPY( sPacket.saMounts[sPacket.iCount].szName, sData.szName );
			STRINGCOPY( sPacket.saMounts[sPacket.iCount].szImageFile, sData.szImageFile );
			sPacket.saMounts[sPacket.iCount].eMountType		= sData.eMountType;
			sPacket.saMounts[sPacket.iCount].eRareType		= sData.eRareType;

			for ( int i = 0; i < sData.iItemCount; i++ )
				CopyMemory( &sPacket.saMounts[sPacket.iCount].saItemList[i], &sData.saItemList[i], sizeof( Item ) );

			sPacket.saMounts[sPacket.iCount].iItemCount = sData.iItemCount;

			sPacket.iCount++;

			if ( sPacket.iCount == MAX_MOUNTS_PER_DATA )
			{
				sPacket.sPacketCount.sMin++;
				vPackets.push_back( sPacket );

				sPacket.iCount = 0;
			}
		}

		sPacket.sPacketCount.sMin++;
		vPackets.push_back( sPacket );

		for ( auto & sSendPacket : vPackets )
		{
			sSendPacket.sPacketCount.sMax = (short)vPackets.size();

			ProcessPacket( pcUser, &sSendPacket );
		}
	}

	vPackets.clear();
	vMounts.clear();
}

void CMountServerHandler::OnUnloadUser( User * pcUser )
{
	SQLAddUserMounts( pcUser, pcUser->vMountList );
}

void CMountServerHandler::OnUpdate( User * pcUser )
{
}

void CMountServerHandler::HandlePacket( User * pcUser, PacketMountUserList * psPacket )
{
	if ( psPacket->IsFirst() )
		pcUser->vMountList.clear();

	if ( (psPacket->iCount > 0) && (psPacket->iCount <= MAX_MOUNTS_PER_DATA) )
	{
		for ( int i = 0; i < psPacket->iCount; i++ )
			pcUser->vMountList.push_back( psPacket->saMounts[i] );
	}

	if ( psPacket->IsOnly() )
		SQLAddUserMounts( pcUser, pcUser->vMountList );
}

bool CMountServerHandler::HandleItem( User * pcUser, EItemID eItemID, int iChk1, int iChk2 )
{
	bool bRet = false;

	EMountType eMountType = GetMountTypeByItemID( eItemID );

	if ( eMountType != EMountType::MOUNTTYPE_None )
	{
		//if ( ITEMSERVER->UseItem( pcUser, eItemID, iChk1, iChk2 ) >= 0 )
		{
			if ( auto psMount = GetMount( eMountType ) )
			{
				PacketNewMount sPacket;
				sPacket.iID = pcUser->GetID() + (int)eMountType;
				sPacket.eMountType = eMountType;
				sPacket.eRareType = psMount->eRareType;
				sPacket.iItemCount = 0;
				sPacket.bIsSelected = FALSE;

				STRINGCOPY( sPacket.szName, psMount->strName.c_str() );
				STRINGCOPY( sPacket.szImageFile, psMount->strImageFile.c_str() );

				ProcessPacket( pcUser, &sPacket );

				bRet = true;
			}
		}
	}

	return bRet;
}

void CMountServerHandler::SQLLoadMounts()
{
	Clear();

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID, Name, ImageFile, MountType, MountRareType FROM MountList" ) )
		{
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					MountMonster * psData = new MountMonster;

					char szName[32] = {0};
					char szImageFile[64] = {0};

					pcDB->GetData( 1, PARAMTYPE_Integer, &psData->iID );
					pcDB->GetData( 2, PARAMTYPE_String, szName, _countof( szName ) );
					pcDB->GetData( 3, PARAMTYPE_String, szImageFile, _countof( szImageFile ) );
					pcDB->GetData( 4, PARAMTYPE_Integer, &psData->eMountType );
					pcDB->GetData( 5, PARAMTYPE_Integer, &psData->eRareType );

					psData->strName = szName;
					psData->strImageFile = szImageFile;

					vMountsList.push_back( psData );
				}
			}
		}
		pcDB->Close();
	}
}

bool CMountServerHandler::SQLGetUserMounts( User * pcUser, std::vector<MountUserListData> & vMounts )
{
    bool bRet = false;
	
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID, MountName, MountType, IsSelected FROM CharacterMount WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					MountUserListData sData = {0};

					pcDB->GetData( 1, PARAMTYPE_Integer, &sData.iID );
					pcDB->GetData( 2, PARAMTYPE_String, sData.szName, _countof( sData.szName ) );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sData.eMountType );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sData.bIsSelected );

					if ( auto psMount = GetMount( sData.eMountType ) )
					{
						sData.eRareType = psMount->eRareType;
						STRINGCOPY( sData.szName, psMount->strName.c_str() );
						STRINGCOPY( sData.szImageFile, psMount->strImageFile.c_str() );
					}

					sData.iItemCount = 0;
					vMounts.push_back( sData );

					bRet = true;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MountID, MountItem FROM CharacterMountItems WHERE CharacterID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			if ( pcDB->Execute() )
			{
				while ( pcDB->Fetch() )
				{
					int iMountID = -1;
					Item sItem;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iMountID );
					pcDB->GetData( 2, PARAMTYPE_Binary, &sItem, sizeof( Item ) );

					for ( auto & sMount : vMounts )
					{
						if ( sMount.iID == iMountID )
						{
							if ( (sMount.iItemCount < _countof( sMount.saItemList )) && (sItem.sItemID.ToItemID() != ITEMID_None) )
								CopyMemory( &sMount.saItemList[sMount.iItemCount++], &sItem, sizeof( Item ) );

							break;
						}
					}
				}
			}
		}

		pcDB->Close();
	}

    return bRet;
}

bool CMountServerHandler::SQLAddUserMounts( User * pcUser, std::vector<MountUserListData> & vMounts )
{
	bool bRet = false;

	for ( auto & sData : vMounts )
	{
		if ( SQLHasUserMount( pcUser->iCharacterID, sData.eMountType ) )
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "UPDATE CharacterMount SET MountName=?, IsSelected=? WHERE (CharacterID=?) AND (MountType=?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, sData.szName, STRLEN( sData.szName ) );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &sData.bIsSelected );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &pcUser->iCharacterID );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &sData.eMountType );
					pcDB->Execute();
				}


				if ( (sData.iItemCount > 0) && (sData.iItemCount <= _countof( sData.saItemList ) ) )
				{
					for ( int i = 0; i < sData.iItemCount; i++ )
					{
						pcDB->Clear();
						if ( pcDB->Prepare( "UPDATE CharacterMountItems SET MountItem=? WHERE (CharacterID=?) AND (MountType=?)" ) )
						{
							pcDB->BindParameterInput( 1, PARAMTYPE_Binary, &sData.saItemList[i], sizeof( Item ) );
							pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &pcUser->iCharacterID );
							pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &sData.eMountType );
							pcDB->Execute();
						}
					}
				}

				pcDB->Close();
			}
		}
		else
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO CharacterMount([CharacterID], [MountName], [MountType], [IsSelected], [Date]) VALUES(?,?,?,0,GETDATE())" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
					pcDB->BindParameterInput( 2, PARAMTYPE_String, sData.szName, STRLEN( sData.szName ) );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &sData.eMountType );
					pcDB->Execute();
				}

				int iLastID = -1;
				pcDB->Clear();
				if ( pcDB->Prepare( "SELECT TOP 1 ID FROM CharacterMount ORDER BY ID DESC" ) )
				{
					if ( pcDB->Execute() && pcDB->Fetch() )
						pcDB->GetData( 1, PARAMTYPE_Integer, &iLastID );
				}

				for ( int i = 0; i < _countof( sData.saItemList ); i++ )
				{
					ZeroMemory( &sData.saItemList[i], sizeof( Item ) );

					pcDB->Clear();
					if ( pcDB->Prepare( "INSERT INTO CharacterMountItems([CharacterID], [MountID], [MountType], [MountItem], [Date]) VALUES(?,?,?,?,GETDATE())" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
						pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &iLastID );
						pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &sData.eMountType );
						pcDB->BindParameterInput( 4, PARAMTYPE_Binary, &sData.saItemList[i], sizeof( Item ) );
						pcDB->Execute();
					}
				}

				pcDB->Close();
			}
		}
	}

	return bRet;
}

bool CMountServerHandler::SQLDeleteUserMount( User * pcUser, EMountType eMountType )
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "DELETE FROM CharacterMount WHERE (CharacterID=?) AND (MountType=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &pcUser->iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &eMountType );
			pcDB->Execute();
		}
		pcDB->Close();
	}

	return true;
}

bool CMountServerHandler::SQLHasUserMount( int iCharacterID, EMountType eMountType )
{
	int iID = -1;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_UserDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM CharacterMount WHERE (CharacterID=?) AND (MountType=?)" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iCharacterID );
			pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &eMountType );

			if ( pcDB->Execute() )
				if ( pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iID );
		}

		pcDB->Close();
	}


	return (iID >= 0);
}

MountMonster * CMountServerHandler::GetMount( EMountType eMountType )
{
	for ( auto ps : vMountsList )
		if ( ps->eMountType == eMountType )
			return ps;

	return nullptr;
}

void CMountServerHandler::ProcessPacket( User * pcUser, PacketMountUserList * psPacket )
{
	psPacket->iLength = sizeof( Packet ) + sizeof( MinMax ) + sizeof( int ) + (sizeof( MountUserListData ) * psPacket->iCount);
    psPacket->iHeader = PKTHDR_MountsList;
    SENDPACKET( pcUser, psPacket, TRUE );
}

void CMountServerHandler::ProcessPacket( User * pcUser, PacketMountDelete * psPacket )
{
	psPacket->iLength = sizeof( PacketMountDelete );
	psPacket->iHeader = PKTHDR_MountDelete;
	SENDPACKET( pcUser, psPacket, TRUE );
}

void CMountServerHandler::ProcessPacket( User * pcUser, PacketNewMount * psPacket )
{
	psPacket->iLength = sizeof( PacketNewMount );
	psPacket->iHeader = PKTHDR_MountNew;
	SENDPACKET( pcUser, psPacket, TRUE );
}
