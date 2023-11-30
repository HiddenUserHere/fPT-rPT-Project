#include "stdafx.h"
#include "unitinfo.h"

// Global Vars
UINT								uCurrentStageUnitStand = 0;
UINT								uCurrentStageUnitSpawn = 0;
UINT								uCurrentStageUnitEnemy = 0;

ItemData								* pAllocItemTable = NULL;
UINT								iTableItemCompressPosition = 0;


void * allocIT = NULL;
UINT ItemV = 0;

void refIT();
void endIT();

UnitInfoServer::UnitInfoServer()
{
	pDefinitionItemExTable = new DefinitionItemEx[ITEM_SERVER_MAX];
	ZeroMemory( pDefinitionItemExTable, sizeof( DefinitionItemEx ) * ITEM_SERVER_MAX );
}

UnitInfoServer::~UnitInfoServer()
{
	DELETA( pDefinitionItemExTable );
}

unsigned short read_ushort( char * pBuf )
{
	return (unsigned short)*(unsigned short*)((char*)pBuf);
}

int UnitInfoServer::GetEnemyEffect( char * pszEffect )
{
	for ( auto f : saCharacterSoundEffect )
	{
		if ( STRINGCOMPAREI( f.szCodeName, pszEffect ) )
			return f.iMonsterEffectID;
	}
	return 0;
}

void UnitInfoServer::CompressItemDataInTable( ItemData * pItem )
{
	pTableNewItem[ iTableItemCompressPosition ].sItemID = pItem->sBaseItemID;
	STRINGCOPY( pTableNewItem[ iTableItemCompressPosition ].szBaseName, pItem->szBaseName );
	STRINGCOPY( pTableNewItem[ iTableItemCompressPosition ].szInventoryName, pItem->szInventoryName );
	pTableNewItem[ iTableItemCompressPosition ].iWidth = pItem->iWidth;
	pTableNewItem[ iTableItemCompressPosition ].iHeight = pItem->iHeight;
	STRINGCOPY( pTableNewItem[ iTableItemCompressPosition ].szDropFolder, pItem->szCategory );
	pTableNewItem[ iTableItemCompressPosition ].iClass = pItem->iItemSlotFlag;
	STRINGCOPY( pTableNewItem[ iTableItemCompressPosition ].szDropName, pItem->szModelName );
	pTableNewItem[ iTableItemCompressPosition ].iModelPosition = pItem->iVisibleItemType;
	pTableNewItem[ iTableItemCompressPosition ].iSound = pItem->iInvItemSound;
	pTableNewItem[ iTableItemCompressPosition ].iWeaponClass = pItem->iItemWpnDmg;

}

BOOL UnitInfoServer::EnemyDataItem( UnitInfo * pMonsterInfo, const char * pszItems, int iChance, int iGoldMin, int iGoldMax )
{

	if ( pMonsterInfo->iDropCount == 200 )
		return FALSE;

	//Gold or Air
	{
		if ( STRINGCOMPAREI( pszItems, "Gold" ) )
		{
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].iItemID = ITEMID_Gold;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].iPercent    = iChance;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].sGoldMin    = iGoldMin;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].sGoldMax	  = iGoldMax;
			pMonsterInfo->iDropPercentNum += iChance;
			pMonsterInfo->iDropCount++;
			return TRUE;
		}
		else if ( STRINGCOMPAREI( pszItems, "Air" ) )
		{
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].iItemID = ITEMID_None;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].iPercent	  = iChance;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].sGoldMin    = iGoldMin;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].sGoldMax    = iGoldMax;
			pMonsterInfo->iDropPercentNum += iChance;
			pMonsterInfo->iDropCount++;
			return TRUE;
		}
	}


	std::vector<std::string> vItems = split( pszItems, ' ' );

	UINT uVecSize = vItems.size() + pMonsterInfo->iDropCount;
	if ( uVecSize > 200 )
		uVecSize = 200;

	int iPosition = 0;

	UINT uTotalItem = uVecSize - pMonsterInfo->iDropCount;
	if ( uTotalItem == 0 )
		uTotalItem = 1;

	for ( UINT u = pMonsterInfo->iDropCount; u < uVecSize; u++ )
	{

		ItemData * pItem = FindItemPointerTable( vItems[ iPosition++ ].c_str() );
		if ( pItem )
		{
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].iItemID = pItem->sBaseItemID.ToItemID();
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].iPercent	  = iChance / uTotalItem;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].sGoldMin    = iGoldMin;
			pMonsterInfo->saDropData[ pMonsterInfo->iDropCount ].sGoldMax    = iGoldMax;
			pMonsterInfo->iDropCount++;
			if ( pMonsterInfo->iDropCount == 200 )
			{
				pMonsterInfo->iDropPercentNum += iChance;
				return FALSE;
			}
		}
	}

	vItems.clear();

	pMonsterInfo->iDropPercentNum += iChance;

	return TRUE;
}

void UnitInfoServer::ReadDropItem()
{
	int iEnemy = 0;
	UINT uCount = ( *( UINT* )0x07AC57E8 );

	UnitInfo * pMonsterInfo = *( UnitInfo** )0x07AB22A4;

	for ( UINT u = 0; u < uCount; u++ )
	{
		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT COUNT(ID) FROM DropList WHERE MonsterName=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_String, pMonsterInfo[u].szName );
				if ( pcDB->Execute() && pcDB->Fetch() )
					pcDB->GetData( 1, PARAMTYPE_Integer, &iEnemy, 0 );
			}
			pcDB->Close();
		}

		//Is Enemy in SQL?
		if ( !IsNull( iEnemy ) )
		{
			//Get Data DropList
			int iDropID = 0;
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT * FROM DropList WHERE MonsterName=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, pMonsterInfo[ u ].szName );
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iDropID, 0 );
						pcDB->GetData( 3, PARAMTYPE_Integer, &pMonsterInfo[ u ].bPublicDrop, 0 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &pMonsterInfo[ u ].iNumDrops, 0 );
					}
				}
				pcDB->Close();
			}

			//Get Data DropItem
			int iCountDrop = 0;
			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT COUNT(ID) FROM DropItem WHERE DropID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iDropID );
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iCountDrop, 0 );
					}
				}
				pcDB->Close();
			}

			//Is Drops in SQL?
			if ( !IsNull( iCountDrop ) )
			{
				std::vector<std::string> vItems;

				if ( pcDB->Open() )
				{
					if ( pcDB->Prepare( "SELECT * FROM DropItem WHERE DropID=? ORDER BY Chance ASC" ) )
					{
						pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iDropID );
						if ( pcDB->Execute() && pcDB->Fetch() )
						{
							for ( int i = 0; i < iCountDrop; i++ )
							{
								char szItems[512] = { 0 };
								int iChance = 0;
								short iGoldMin = 0;
								short iGoldMax = 0;

								pcDB->GetData( 3, PARAMTYPE_String, szItems, 512 );
								pcDB->GetData( 4, PARAMTYPE_Integer, &iChance, 0 );
								pcDB->GetData( 5, PARAMTYPE_Short, &iGoldMin, 0 );
								pcDB->GetData( 6, PARAMTYPE_Short, &iGoldMax, 0 );
								EnemyDataItem( &pMonsterInfo[u], szItems, iChance, iGoldMin, iGoldMax );

								// Is Max items?
								if ( pMonsterInfo[u].iDropCount == 200 )
									break;

								pcDB->Fetch();
							}
						}
					}
					pcDB->Close();
				}
			}
		}
	}
}

UINT UnitInfoServer::GetTotalUnitEnemy()
{
	return ( *( UINT* )0x07AC57E8 );
}

CharacterData * UnitInfoServer::GetEnemyCharInfoPtr()
{
	return ( *( CharacterData** )0x07AB3050 );
}

UnitInfo * UnitInfoServer::GetEnemyDataInfoPtr()
{
	return ( *( UnitInfo** )0x07AB22A4 );
}

DefinitionItemEx * UnitInfoServer::GetItemDefinitionExByItemID( EItemID eItemID )
{
	for ( int i = 0; i < ITEM_SERVER_MAX; i++ )
		if ( pDefinitionItemExTable[i].eItemID == eItemID )
			return &pDefinitionItemExTable[i];

	return nullptr;
}

ItemData * UnitInfoServer::FindItemPointerTable( const char * szCode )
{
	if ( !pAllocItemTable )
		return NULL;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( STRINGCOMPAREI( pAllocItemTable[ u ].szInventoryName, szCode ) )
			return &pAllocItemTable[ u ];
	}

	return NULL;
}

ItemData * UnitInfoServer::FindItemPointerTable( UINT uCode )
{
	if ( !pAllocItemTable )
		return NULL;

	for ( UINT u = 0; u < iTableItemCompressPosition; u++ )
	{
		if ( pAllocItemTable[ u ].sBaseItemID.ToItemID() == uCode )
			return &pAllocItemTable[ u ];
	}

	return NULL;
}

CharacterData * UnitInfoServer::GetCharacterDataByMonsterName( const char * pszName )
{
	CharacterData * pEnemyCharInfo = GetEnemyCharInfoPtr();
	UINT uEnemyCount = GetTotalUnitEnemy();

	for ( UINT u = 0; u < uEnemyCount; u++ )
	{
		if ( STRINGCOMPAREI( pEnemyCharInfo[ u ].szName, pszName ) )
			return &pEnemyCharInfo[ u ];
	}

	return NULL;
}

CharacterData * UnitInfoServer::GetCharacterDataByMonsterID( int iID )
{
	UnitInfo * psaUnitInfo = *(UnitInfo**)0x07AB22A4;
	UINT uEnemyCount = GetTotalUnitEnemy();

	for ( UINT u = 0; u < uEnemyCount; u++ )
	{
		if ( psaUnitInfo[u].iRank == iID )
		{
			CharacterData * pEnemyCharInfo = GetEnemyCharInfoPtr();
			return &pEnemyCharInfo[u];
		}
	}

	return NULL;
}

int UnitInfoServer::GetModelID( const char * pszModelFile )
{
	int iID = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
				
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ID FROM ModelList WHERE Model=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszModelFile );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				if ( !pcDB->GetData( 1, PARAMTYPE_Integer, &iID, 0 ) )
					WRITEERR( "%s- %d", pszModelFile, iID );
			}
		}
		pcDB->Close();
	}
	return iID;
}

int UnitInfoServer::GetModelDeathID( const char * pszModelFile )
{
	int iID = 0;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DeathModelID FROM ModelList WHERE Model=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_String, (void*)pszModelFile );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iID, 0 );
			}
		}
		pcDB->Close();
	}

	return iID;
}

UINT UnitInfoServer::ReadModelSize( const char * pszModelFile )
{
	int iID = 0;
	// Is in SQL Data?
	if ( iID = GetModelID( pszModelFile ) )
	{
		int iWidth = 0;
		int iHeight = 0;
		UINT uModelPat = 0;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT Width, Height FROM ModelList WHERE ID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iWidth, 0 );
					pcDB->GetData( 2, PARAMTYPE_Integer, &iHeight, 0 );
				}
			}
			pcDB->Close();
		}

		typedef UINT( __cdecl *t_newPT )( UINT Size );
		t_newPT newPT = ( t_newPT )0x005C6F40;
		uModelPat = newPT( 1252 );
		if ( uModelPat )
		{
			ZeroMemory( (void*)uModelPat, 1252 );
			*( int* )( uModelPat + 0x294 ) = iWidth;
			*( int* )( uModelPat + 0x298 ) = iHeight;
			*( int* )( uModelPat + 0x284 ) = newPT( 1252 ); // TmPattern new smPat3D ( death model work )
			return uModelPat;
		}
	}
	return NULL;
}

BOOL UnitInfoServer::ReadUnitAnimation( char * pszModelFile, ModelDataOld * pModelData )
{
	static ModelDataOld sModelData;
	ZeroMemory( &sModelData, sizeof( ModelDataOld ) );

	sModelData.iTalkNumModelAnimation = 10;
	sModelData.iNumModelAnimation = 10;

	int iID = 0;

	// Is in SQL Data?
	if ( iID = GetModelID( pszModelFile ) )
	{
		int iCountAnimation = 0;
		int iType			= 0;
		int iBeginFrame		= 0;
		int iEndFrame		= 0;

		// Model File ( Pattern )
		STRINGCOPY( sModelData.szModelPath, pszModelFile );

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT COUNT(*) FROM ModelAnimationList WHERE ModelID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &iCountAnimation, 0 );
				}
			}
			pcDB->Close();
		}

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM ModelAnimationList WHERE ModelID=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					for ( int i = 10; i < (iCountAnimation+10); i++ )
					{
						pcDB->GetData( 3, PARAMTYPE_Integer, &iType, 0 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &iBeginFrame, 0 );
						pcDB->GetData( 5, PARAMTYPE_Integer, &iEndFrame, 0 );
						sModelData.saModelAnimation[ i ].iType = (EAnimationType)iType;
						sModelData.saModelAnimation[ i ].iBeginFrame	= iBeginFrame;
						sModelData.saModelAnimation[ i ].iEndFrame		= iEndFrame;

						// Control Frames
						if ( sModelData.saModelAnimation[ i ].iType == EAnimationType::ANIMATIONTYPE_Running )
							sModelData.saModelAnimation[ i ].bLoop = TRUE;
			
		//				if ( sModelData.saModelAnimation[ i ].iType == ANIMATIONTYPE_Die )
		//					sModelData.saModelAnimation[ i ].iEndFrame -= 8;
			
						else if ( sModelData.saModelAnimation[ i ].iType == ANIMATIONTYPE_Idle )
							sModelData.saModelAnimation[ i ].bLoop = TRUE;
			
						else if ( sModelData.saModelAnimation[ i ].iType == ANIMATIONTYPE_Walking )
							sModelData.saModelAnimation[ i ].bLoop = TRUE;
			
						pcDB->Fetch();
					}
				}
			}
			pcDB->Close();
		}
	
		sModelData.iNumModelAnimation += ( iCountAnimation );

		// Death Model
		int iDeathModelID = 0;
		if ( iDeathModelID = GetModelDeathID( pszModelFile ) )
		{
			int iType = 0;
			int iBeginFrame = 0;
			int iEndFrame = 0;

			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

			// Model Death File ( Pattern 2 ) 
			char szModelDeathFile[ 64 ] = { 0 };

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT Model FROM ModelList WHERE ID=?" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iDeathModelID );
					if ( pcDB->Execute() && pcDB->Fetch() )
						pcDB->GetData( 1, PARAMTYPE_String, szModelDeathFile, 64 );
				}
				pcDB->Close();
			}

			STRINGCOPY( sModelData.szDeathModelDataFilePath, szModelDeathFile );
		}

		memcpy( pModelData, &sModelData, sizeof( ModelDataOld ) );

		return TRUE;
	}
	return FALSE;
}

int UnitInfoServer::ReadInx( char * pszFilePath )
{
//	if ( pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'x' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'X' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'i' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'I' )
//		return 0;

	//keep a filepath with INI
	char szTurd[ 256 ] = { 0 };
	char szTurd2[ 256 ] = { 0 };
	STRINGCOPY( szTurd, pszFilePath );
	STRINGCOPY( szTurd2, pszFilePath );
	szTurd[ lstrlenA( szTurd ) - 1 ] = 'I';
	szTurd2[ lstrlenA( szTurd2 ) - 1 ] = 'X';

	if ( GetFileSize( szTurd2 ) <= 67084 )
		return ReadInxOld( szTurd2 );
	
	// Lower case
	for ( int i = 0; i < lstrlenA( szTurd ); i++ )
		szTurd[ i ] = tolower( szTurd[ i ] );

	int iID = 0;
	
	if ( ( iID = GetModelID( szTurd ) ) != 0 )
		return iID;


	FILE * file;
	if ( fopen_s( &file, szTurd2, "rb" ) == 0 )
	{
		static ModelData tmp;
		fread( &tmp, sizeof( tmp ), 1, file );
		fclose( file );

		int iDeathModelID = 0;

		//Has Death Model?
		if ( tmp.szDeathModelDataFilePath[ 0 ] != 0 )
			iDeathModelID = ReadInx( tmp.szDeathModelDataFilePath );

		//Obtain Width & Height
		DWORD dwWidth = 0, dwHeight = 0;

		int iLen = lstrlenA( tmp.szModelPath );
		tmp.szModelPath[ iLen - 3 ] = 's';
		tmp.szModelPath[ iLen - 2 ] = 'm';
		tmp.szModelPath[ iLen - 1 ] = 'd';

		if ( fopen_s( &file, tmp.szModelPath, "rb" ) == 0 )
		{
			int iFileLength;
			fseek( file, 0, SEEK_END );
			iFileLength = ftell( file );
			rewind( file );

			if ( iFileLength > 0 )
			{
				char * buffer = new char[ iFileLength ];
				fread( buffer, 1, iFileLength, file );

				for ( int i = 0; i < ( iFileLength - 2 ); i++ )
				{
					char d = buffer[ i ];
					char c = buffer[ i + 1 ];
					char b = buffer[ i + 2 ];

					if ( d == 'D' && c == 'C' && b == 'B' )
					{
						i += 44;

						DWORD * dwPtr = ( DWORD* )&buffer[ i ];

						dwWidth = *dwPtr;

						dwPtr += 2;

						dwHeight = *dwPtr;
						break;
					}
				}

				delete[] buffer;
			}

			fclose( file );
		}

		//Obtain Frame Info
		struct FrameInfo
		{
			int iStartFrame;
			int iEndFrame;
			int iPosition;
			int iCount;
		};

		struct FileHeader
		{
			char szHeader[ 24 ];
			int iObjectCount;
			int iMaterialCount;
			int iMaterialPosition;
			int iFirstObjectPosition;
			int iFrameCounter;
			FrameInfo saFrameInfo[ 32 ];
		};

		//Got overlaying Model Animation file?
		if ( tmp.szModelDataAnimationFile[0] )
		{
			tmp.szModelDataAnimationFile[ lstrlenA( tmp.szModelDataAnimationFile ) ] = 'x';

			if ( fopen_s( &file, tmp.szModelDataAnimationFile, "rb" ) == 0 )
			{
				fread( &tmp, sizeof( tmp ), 1, file );
				fclose( file );
			}
		}

		static FileHeader sFileHeader;
		ZeroMemory( &sFileHeader, sizeof( FileHeader ) );

		iLen = lstrlenA( tmp.szBoneModelFilePath );
		tmp.szBoneModelFilePath[ iLen - 3 ] = 's';
		tmp.szBoneModelFilePath[ iLen - 2 ] = 'm';
		tmp.szBoneModelFilePath[ iLen - 1 ] = 'b';

		if ( fopen_s( &file, tmp.szBoneModelFilePath, "rb" ) == 0 )
		{
			fread( &sFileHeader, sizeof( FileHeader ), 1, file );
			fclose( file );
		}

		// Insert model data in SQL
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ModelList([Model],[Width],[Height],[DeathModelID]) VALUES(?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szTurd );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &dwWidth );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &dwHeight );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iDeathModelID );

					pcDB->Execute();
				}
				pcDB->Close();
			}

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT TOP 1 ID FROM ModelList ORDER BY ID DESC" ) )
				{
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iID, 0 );
					}
				}
				pcDB->Close();
			}

			for ( int i = 10; i < tmp.iNumModelAnimation; i++ )
			{
				ModelAnimation * p = tmp.saModelAnimation + i;

				if ( p->iType != NULL )
				{
					//Decode Frames
					char caBeginFrame[2], caEndFrame[2];
					caBeginFrame[0] = *(char*)((DWORD)p + 4);
					caBeginFrame[1] = *(char*)((DWORD)p + 6);
					p->iBeginFrame = read_ushort( caBeginFrame );

					caEndFrame[0] = *(char*)((DWORD)p + 16);
					caEndFrame[1] = *(char*)((DWORD)p + 18);
					p->iEndFrame = read_ushort( caEndFrame );
					if ( ( p->iSubFileFrameNumber - 1 ) < sFileHeader.iFrameCounter )
					{
						int iFrameAppend = sFileHeader.saFrameInfo[ p->iSubFileFrameNumber - 1 ].iStartFrame;
						iFrameAppend /= 160;

						p->iBeginFrame += iFrameAppend;
						p->iEndFrame += iFrameAppend;
					}


					if ( pcDB->Open() )
					{
						if ( pcDB->Prepare( "INSERT INTO ModelAnimationList([ModelID],[Type],[FrameBegin],[FrameEnd]) VALUES(?,?,?,?)" ) )
						{
							pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
							pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &p->iType );
							pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &p->iBeginFrame );
							pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &p->iEndFrame );

							pcDB->Execute();
						}
						pcDB->Close();
					}
				}
			}
		}
	}
	return iID;
}

int UnitInfoServer::ReadInxOld( char * pszFilePath )
{
//	if ( pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'x' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'X' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'i' && pszFilePath[ lstrlenA( pszFilePath ) - 1 ] != 'I' )
//		return 0;

	//keep a filepath with INI
	char szTurd[ 256 ] = { 0 };
	char szTurd2[ 256 ] = { 0 };
	STRINGCOPY( szTurd, pszFilePath );
	STRINGCOPY( szTurd2, pszFilePath );
	szTurd[ lstrlenA( szTurd ) - 1 ] = 'I';
	szTurd2[ lstrlenA( szTurd2 ) - 1 ] = 'X';
	
	// Lower case
	for ( int i = 0; i < lstrlenA( szTurd ); i++ )
		szTurd[ i ] = tolower( szTurd[ i ] );

	int iID = 0;
	
	if ( ( iID = GetModelID( szTurd ) ) != 0 )
		return iID;


	FILE * file;
	if ( fopen_s( &file, szTurd2, "rb" ) == 0 )
	{
		static ModelDataOld tmp;
		fread( &tmp, sizeof( tmp ), 1, file );
		fclose( file );

		int iDeathModelID = 0;

		//Has Death Model?
		if ( tmp.szDeathModelDataFilePath[ 0 ] != 0 )
			iDeathModelID = ReadInx( tmp.szDeathModelDataFilePath );

		//Obtain Width & Height
		DWORD dwWidth = 0, dwHeight = 0;

		int iLen = lstrlenA( tmp.szModelPath );
		tmp.szModelPath[ iLen - 3 ] = 's';
		tmp.szModelPath[ iLen - 2 ] = 'm';
		tmp.szModelPath[ iLen - 1 ] = 'd';

		if ( fopen_s( &file, tmp.szModelPath, "rb" ) == 0 )
		{
			int iFileLength;
			fseek( file, 0, SEEK_END );
			iFileLength = ftell( file );
			rewind( file );

			if ( iFileLength > 0 )
			{
				char * buffer = new char[ iFileLength ];
				fread( buffer, 1, iFileLength, file );

				for ( int i = 0; i < ( iFileLength - 2 ); i++ )
				{
					char d = buffer[ i ];
					char c = buffer[ i + 1 ];
					char b = buffer[ i + 2 ];

					if ( d == 'D' && c == 'C' && b == 'B' )
					{
						i += 44;

						DWORD * dwPtr = ( DWORD* )&buffer[ i ];

						dwWidth = *dwPtr;

						dwPtr += 2;

						dwHeight = *dwPtr;
						break;
					}
				}

				delete[] buffer;
			}

			fclose( file );
		}

		//Obtain Frame Info
		struct FrameInfo
		{
			int iStartFrame;
			int iEndFrame;
			int iPosition;
			int iCount;
		};

		struct FileHeader
		{
			char szHeader[ 24 ];
			int iObjectCount;
			int iMaterialCount;
			int iMaterialPosition;
			int iFirstObjectPosition;
			int iFrameCounter;
			FrameInfo saFrameInfo[ 32 ];
		};

		//Got overlaying Model Animation file?
		if ( tmp.szModelDataAnimationFile[ 0 ] )
		{
			tmp.szModelDataAnimationFile[ lstrlenA( tmp.szModelDataAnimationFile ) ] = 'x';

			if ( fopen_s( &file, tmp.szModelDataAnimationFile, "rb" ) == 0 )
			{
				fread( &tmp, sizeof( tmp ), 1, file );
				fclose( file );
			}
		}

		static FileHeader sFileHeader;
		ZeroMemory( &sFileHeader, sizeof( FileHeader ) );

		iLen = lstrlenA( tmp.szBoneModelFilePath );
		tmp.szBoneModelFilePath[ iLen - 3 ] = 's';
		tmp.szBoneModelFilePath[ iLen - 2 ] = 'm';
		tmp.szBoneModelFilePath[ iLen - 1 ] = 'b';

		if ( fopen_s( &file, tmp.szBoneModelFilePath, "rb" ) == 0 )
		{
			fread( &sFileHeader, sizeof( FileHeader ), 1, file );
			fclose( file );
		}

		// Insert model data in SQL
		{
			SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "INSERT INTO ModelList([Model],[Width],[Height],[DeathModelID]) VALUES(?,?,?,?)" ) )
				{
					pcDB->BindParameterInput( 1, PARAMTYPE_String, szTurd );
					pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &dwWidth );
					pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &dwHeight );
					pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &iDeathModelID );
					pcDB->Execute();
				}
				pcDB->Close();
			}

			if ( pcDB->Open() )
			{
				if ( pcDB->Prepare( "SELECT TOP 1 ID FROM ModelList ORDER BY ID DESC" ) )
				{
					if ( pcDB->Execute() && pcDB->Fetch() )
					{
						pcDB->GetData( 1, PARAMTYPE_Integer, &iID, 0 );
					}
				}
				pcDB->Close();
			}

			for ( int i = 10; i < tmp.iNumModelAnimation; i++ )
			{
				ModelAnimationOld * p = tmp.saModelAnimation + i;

				if ( p->iType != NULL )
				{
					//Decode Frames
					char caBeginFrame[2], caEndFrame[2];
					caBeginFrame[0] = *(char*)((DWORD)p + 4);
					caBeginFrame[1] = *(char*)((DWORD)p + 6);
					p->iBeginFrame = read_ushort( caBeginFrame );

					caEndFrame[0] = *(char*)((DWORD)p + 16);
					caEndFrame[1] = *(char*)((DWORD)p + 18);
					p->iEndFrame = read_ushort( caEndFrame );
					if ( ( p->iSubFileFrameNumber - 1 ) < sFileHeader.iFrameCounter )
					{
						int iFrameAppend = sFileHeader.saFrameInfo[ p->iSubFileFrameNumber - 1 ].iStartFrame;
						iFrameAppend /= 160;

						p->iBeginFrame += iFrameAppend;
						p->iEndFrame += iFrameAppend;
					}


					if ( pcDB->Open() )
					{
						if ( pcDB->Prepare( "INSERT INTO ModelAnimationList([ModelID],[Type],[FrameBegin],[FrameEnd]) VALUES(?,?,?,?)" ) )
						{
							pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iID );
							pcDB->BindParameterInput( 2, PARAMTYPE_Integer, &p->iType );
							pcDB->BindParameterInput( 3, PARAMTYPE_Integer, &p->iBeginFrame );
							pcDB->BindParameterInput( 4, PARAMTYPE_Integer, &p->iEndFrame );

							pcDB->Execute();
						}
						pcDB->Close();
					}
				}
			}
		}
	}
	return iID;
}

void UnitInfoServer::CreateUnitEnemyData()
{
	const int iFieldCount = 50; // Field Count in SQL
	int iCount = 0;

	// Read Items
	{
		if ( !pAllocItemTable )
			CreateItemMemoryTable();
	}
	// Clear pcUnitData Memory Data
	ZeroMemory( ( void* )*( CharacterData** )0x07AB3050, sizeof( CharacterData ) * MONSTER_SERVER_MAX );
	ZeroMemory( ( void* )*( UnitInfo** )0x07AB22A4, sizeof( UnitInfo ) * MONSTER_SERVER_MAX );

	// Get Total Monsters
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(*) FROM MonsterList" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCount, 0 );
			}
		}
		pcDB->Close();
	}

	CharacterData * pMonsterCharInfo = *( CharacterData** )0x07AB3050;
	UnitInfo * pMonsterInfo  = *( UnitInfo** )0x07AB22A4;

	// Select Data Monster
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM MonsterList" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				for ( int i = 0; i < iCount; i++ )
				{
					pMonsterInfo[i].iMovementRange = 64 * 256;

					// ID
					int iEnemyID = 0;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iEnemyID, 0 );

					// Name
					pcDB->GetData( 2, PARAMTYPE_String, pMonsterCharInfo[i].szName, 32 );
					STRINGCOPY( pMonsterInfo[i].szName, pMonsterCharInfo[i].szName );

					if ( lstrcmpiA( pMonsterCharInfo[i].szName, "Zombie" ) == 0 )
						*(CharacterData**)0x07AC9D5C = &pMonsterCharInfo[i];

					// Model
					pcDB->GetData( 3, PARAMTYPE_String, pMonsterCharInfo[i].Monster.szBodyModel, 64 );

					// State
					pMonsterCharInfo[i].iType = CHARACTERTYPE_Monster;

					// Level
					pcDB->GetData( 4, PARAMTYPE_Integer, &pMonsterCharInfo[i].iLevel, 0 );

					// Glow
					pcDB->GetData( 5, PARAMTYPE_Short, &pMonsterCharInfo[i].sGlow, 0 );

					// Size
					float fSize = 0.0f;
					pcDB->GetData( 6, PARAMTYPE_Float, &fSize, 0 );
					pMonsterCharInfo[i].sSize = (short)(fSize * 256.0f);

					// Camera Y
					pcDB->GetData( 7, PARAMTYPE_Short, &pMonsterCharInfo[i].sViewBoxZoom.sMin, 0 );
					// Camera Z
					pcDB->GetData( 8, PARAMTYPE_Short, &pMonsterCharInfo[i].sViewBoxZoom.sMax, 0 );

					// pcUnitData ID
					pcDB->GetData( 9, PARAMTYPE_Integer, &pMonsterInfo[i].iRank, 0 );

					// Spawn Time
					pcDB->GetData( 10, PARAMTYPE_Integer, &pMonsterInfo[i].iActiveHour, 0 );

					// Spawn pcUnitData Min
					pcDB->GetData( 11, PARAMTYPE_Integer, &pMonsterInfo[i].iGroupLimitMin, 0 );

					// Spawn pcUnitData Max
					pcDB->GetData( 12, PARAMTYPE_Integer, &pMonsterInfo[i].iGroupLimitMax, 0 );

					// IQ
					pcDB->GetData( 13, PARAMTYPE_Integer, &pMonsterInfo[i].iIntelligence, 0 );

					// Nature
					char szNatureName[32] = { 0 };
					pcDB->GetData( 14, PARAMTYPE_String, szNatureName, 32 );
					{
					pMonsterInfo[i].iNature = MONSTERNATURE_Neutral;

					if ( STRINGCOMPAREI( szNatureName, "good" ) )
						pMonsterInfo[i].iNature = MONSTERNATURE_Good;
					else if ( STRINGCOMPAREI( szNatureName, "evil" ) )
						pMonsterInfo[i].iNature = MONSTERNATURE_Evil;
					}

					// Undead (?)
					pMonsterInfo[i].iUndead = 1;

					// View Sight
					pcDB->GetData( 15, PARAMTYPE_Integer, &pMonsterInfo[i].iSight, 0 );
					pMonsterCharInfo[i].iSight = pMonsterInfo[i].iSight;
					pMonsterCharInfo[i].iSight *= pMonsterCharInfo[i].iSight;

					// HP
					pcDB->GetData( 16, PARAMTYPE_Short, &pMonsterCharInfo[i].sHP.sCur, 0 );
					pMonsterCharInfo[i].sHP.sMax = pMonsterCharInfo[i].sHP.sCur;

					// EXP
					INT64 iExp = 0;
					pcDB->GetData( 17, PARAMTYPE_Int64, &iExp, 0 );
					if ( RATE_EXP > 0 )
						iExp = iExp * RATE_EXP;

					if ( EVENT_PVPMODE )
						iExp += (iExp * RATE_EXPPVPMODE) / 100;


					pMonsterInfo[i].iExp = (int)iExp;
					typedef void( __cdecl *t_SetExpUnitFunc )(CharacterData * pMonster, INT64 iExp);
					t_SetExpUnitFunc SetExpUnitFunc = (t_SetExpUnitFunc)0x08B815CC;
					SetExpUnitFunc( &pMonsterCharInfo[i], iExp );

					// Special Skill Curse
					pcDB->GetData( 18, PARAMTYPE_Integer, &pMonsterInfo[i].iSkillType, 0 );

					// Special Skill Distance
					pcDB->GetData( 19, PARAMTYPE_Integer, &pMonsterInfo[i].iSkillPierceRange, 0 );

					// Special Skill Rating
					pcDB->GetData( 20, PARAMTYPE_Integer, &pMonsterInfo[i].iSkillChance, 0 );

					// Special Skill Range
					pcDB->GetData( 21, PARAMTYPE_Integer, &pMonsterInfo[i].iSkillArea, 0 );

					// Special Skill Damage Min
					pcDB->GetData( 22, PARAMTYPE_Short, &pMonsterInfo[i].sSkillPower.sMin, 0 );

					// Special Skill Damage Max
					pcDB->GetData( 23, PARAMTYPE_Short, &pMonsterInfo[i].sSkillPower.sMax, 0 );

					// Damage Min
					pcDB->GetData( 24, PARAMTYPE_Integer, &pMonsterCharInfo[i].iMinDamage, 0 );

					// Damage Max
					pcDB->GetData( 25, PARAMTYPE_Integer, &pMonsterCharInfo[i].iMaxDamage, 0 );

					// Absorption
					pcDB->GetData( 26, PARAMTYPE_Integer, &pMonsterCharInfo[i].iAbsorbRating, 0 );

					// Stun Chance ( 0% ~ 100% )
					pcDB->GetData( 27, PARAMTYPE_Integer, &pMonsterInfo[i].iFlinch, 0 );

					// Block
					pcDB->GetData( 28, PARAMTYPE_Integer, &pMonsterCharInfo[i].iBlockRating, 0 );

					// Defense
					pcDB->GetData( 29, PARAMTYPE_Integer, &pMonsterCharInfo[i].iDefenseRating, 0 );

					// Attack Speed
					pcDB->GetData( 30, PARAMTYPE_Integer, &pMonsterCharInfo[i].iAttackSpeed, 0 );
					pMonsterCharInfo[i].iAttackSpeed *= 256;

					// Attack Rating
					pcDB->GetData( 31, PARAMTYPE_Integer, &pMonsterCharInfo[i].iAttackRating, 0 );

					// Shooting Range
					pcDB->GetData( 32, PARAMTYPE_Integer, &pMonsterCharInfo[i].iAttackRange, 0 );
					pMonsterCharInfo[i].iAttackRange *= 256;

					// Attack Percent
					pcDB->GetData( 33, PARAMTYPE_Integer, &pMonsterInfo[i].iLureDistance, 0 );

					// Size Shadow
					pcDB->GetData( 34, PARAMTYPE_Integer, &pMonsterCharInfo[i].iShadowSize, 0 );

					// Organic
					pcDB->GetData( 35, PARAMTYPE_Short, &pMonsterCharInfo[i].sElementalDef[ELEMENTID_Organic], 0 );

					// Lightning
					pcDB->GetData( 36, PARAMTYPE_Short, &pMonsterCharInfo[i].sElementalDef[ELEMENTID_Lighting], 0 );

					// Ice
					pcDB->GetData( 37, PARAMTYPE_Short, &pMonsterCharInfo[i].sElementalDef[ELEMENTID_Ice], 0 );

					// Fire
					pcDB->GetData( 38, PARAMTYPE_Short, &pMonsterCharInfo[i].sElementalDef[ELEMENTID_Fire], 0 );

					// Poison
					pcDB->GetData( 39, PARAMTYPE_Short, &pMonsterCharInfo[i].sElementalDef[ELEMENTID_Poison], 0 );

					// Magic
					pcDB->GetData( 40, PARAMTYPE_Short, &pMonsterCharInfo[i].sElementalDef[ELEMENTID_Wind], 0 );

					// Property Monster
					char szPropertyName[32] = { 0 };
					pcDB->GetData( 41, PARAMTYPE_String, szPropertyName, 32 );
					{
						pMonsterCharInfo[i].iMonsterType = MONSTERTYPE_Normal;

						if ( STRINGCOMPAREI( szPropertyName, "undead" ) )
							pMonsterCharInfo[i].iMonsterType = MONSTERTYPE_Undead;
						else if ( STRINGCOMPAREI( szPropertyName, "mutant" ) )
							pMonsterCharInfo[i].iMonsterType = MONSTERTYPE_Mutant;
						else if ( STRINGCOMPAREI( szPropertyName, "demon" ) )
							pMonsterCharInfo[i].iMonsterType = MONSTERTYPE_Demon;
						else if ( STRINGCOMPAREI( szPropertyName, "machine" ) )
							pMonsterCharInfo[i].iMonsterType = MONSTERTYPE_Mechanic;
					}

					// Move Speed
					pcDB->GetData( 42, PARAMTYPE_Integer, &pMonsterCharInfo[i].iMovementSpeed, 0 );
					pMonsterCharInfo[i].iMovementSpeed = (int)(((float)pMonsterCharInfo[i].iMovementSpeed - 9) * 16) + 256;

					// Percent Potion
					pcDB->GetData( 43, PARAMTYPE_Integer, &pMonsterInfo[i].iPerPotions, 0 );

					// Potion Count
					pcDB->GetData( 44, PARAMTYPE_Integer, &pMonsterInfo[i].iMaxPotions, 0 );

					// Sound Effect
					char szEffectName[64] = { 0 };
					pcDB->GetData( 45, PARAMTYPE_String, szEffectName, 64 );
					pMonsterCharInfo[i].iMonsterEffectID = (EMonsterEffectID)GetEnemyEffect( szEffectName );

					// Quest Item
					char szQuestItem[10] = { 0 };
					pcDB->GetData( 46, PARAMTYPE_String, szQuestItem, 10 );
					ItemData * pItem = FindItemPointerTable( szQuestItem );
					if ( pItem )
						pMonsterInfo[i].iQuestItemID = (EItemID)pItem->sBaseItemID.ToItemID();

					// Quest ID
					pcDB->GetData( 47, PARAMTYPE_Integer, &pMonsterInfo[i].eSpecialType, 0 );

					// Quest Map
					pcDB->GetData( 48, PARAMTYPE_Integer, &pMonsterInfo[i].eMapID, 0 );

					pMonsterCharInfo[i].psUnitInfo = (UnitInfo*)&pMonsterInfo[i];


					pcDB->Fetch();
				}
			}
		}
		pcDB->Close();
	}
	( *( UINT* )0x07AC57E8 ) = iCount;

	ReadDropItem();

	// Old Code

	int LoopVars = 0;
	int LoopMob1 = 0;
	int LoopMob2 = 0;
	int CountMob = *( DWORD* )0x07AC57E8;

	int Mob1P = *( DWORD* )0x07AB3050;
	int Mob2P = *( DWORD* )0x07AB22A4;

	while ( LoopVars < CountMob )
	{
		if ( *( BYTE* )( Mob1P + LoopMob1 + 0x20 ) > 0 )
		{
			char szModelFile[ 64 ] = { 0 };
			STRINGCOPY( szModelFile, ( char* )( Mob1P + LoopMob1 + 0x20 ) );
			ReadInx( szModelFile );
			typedef void( __cdecl *t_ClassMob ) ( DWORD v );
			t_ClassMob LoadOftenMeshPattern = ( t_ClassMob )0x00442860;
			LoadOftenMeshPattern( Mob1P + LoopMob1 + 0x20 );
		}

		int LoopSide = 0x08B76514;

		while ( LoopSide < 0x08B76730 )
		{
			if ( lstrcmpA( ( char* )( Mob1P + LoopMob1 ), ( char* )*( UINT* )( LoopSide - 4 ) ) == 0 )
				*( DWORD* )LoopSide = ( Mob1P + LoopMob1 );
			LoopSide += 12;
		}

		LoopSide = 0x08B76610;

		while ( LoopSide < 0x08B7670C )
		{
			if ( lstrcmpA( ( char* )( Mob1P + LoopMob1 ), ( char* )*( UINT* )( LoopSide - 4 ) ) == 0 )
				*( DWORD* )LoopSide = ( Mob1P + LoopMob1 );
			LoopSide += 12;
		}

		LoopSide = 0x006E2BB4;

		while ( LoopSide < 0x006E2BD8 )
		{
			if ( lstrcmpA( ( char* )( Mob1P + LoopMob1 ), ( char* )*( UINT* )( LoopSide - 4 ) ) == 0 )
				*( DWORD* )LoopSide = ( Mob1P + LoopMob1 );
			LoopSide += 12;
		}

		// Hestian
		if ( lstrcmpA( (char*)(Mob1P + LoopMob1), "Hestian" ) == 0 )
		{
			DWORD dwOld = 0;
			VirtualProtect( (void*)0x005DF8F2, 8, PAGE_EXECUTE_READWRITE, &dwOld );
			*(DWORD*)0x005DF8F4 = (Mob1P + LoopMob1);
		}

		if ( *( WORD* )( Mob1P + LoopMob1 + 0x0180 ) == 0x0C8 )
		{
			if ( *( DWORD* )0x07AC9D04 < 0x10 )
			{
				*( DWORD* )( ( *( DWORD* )0x07AC9D04 * 4 ) + 0x07AC56A8 ) = ( Mob1P + LoopMob1 );
				*( DWORD* )0x07AC9D04 += 1;
			}
		}

		if ( *( BYTE* )( Mob2P + LoopMob2 ) != 0 )
		{
			if ( *( BYTE* )( Mob1P + LoopMob1 ) == 0 )
			{
				STRINGCOPYLEN( ( char* )( Mob1P + LoopMob1 ), 32, ( char* )( Mob2P + LoopMob2 ) );
				*( BYTE* )( Mob2P + LoopMob2 ) = 0;
			}
		}

		LoopMob1 += 0x01D0;
		LoopMob2 += 0x0C60;
		LoopVars += 1;
	}

}

void UnitInfoServer::ReadUnitSpawnData( Map * pcMap )
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	// Get Pointers
	Map::SpawnFlag * pSpawnInfo = pcMap->saSpawnFlag;

	int iCount = 0;

	// Get SQL count
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(ID) FROM MapSpawnPoint WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitSpawn );
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCount, 0 );
		}
		pcDB->Close();
	}

	// Max Spawn is MAX_SPAWN_PER_MAP
	if ( iCount > MAX_SPAWNFLAGS )
		iCount = MAX_SPAWNFLAGS;
	
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM MapSpawnPoint WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitSpawn );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				for ( int i = 0; i < iCount; i++ )
				{
					int iSpawnID = 0;
					char szName[32] = { 0 };
					pSpawnInfo[i].bActive = TRUE;
					pcDB->GetData( 1, PARAMTYPE_Integer, &iSpawnID, 0 );
					pcDB->GetData( 3, PARAMTYPE_Integer, &pSpawnInfo[i].iX, 0 );
					pcDB->GetData( 4, PARAMTYPE_Integer, &pSpawnInfo[i].iZ, 0 );
					pcDB->GetData( 5, PARAMTYPE_String, szName, 32 );
					pcDB->Fetch();

					MAPSERVER->AddMapSpawnData( uCurrentStageUnitSpawn, szName, iSpawnID, pSpawnInfo[i].iX, pSpawnInfo[i].iZ );
				}
			}
		}
		pcDB->Close();
	}
	pcMap->iState = 1;
	uCurrentStageUnitSpawn++;
}

void UnitInfoServer::ReadUnitEnemyStage( Map::SpawnSetting * pMonsterStageList )
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	pMonsterStageList->iSpawnIntervalMin = 127;
	pMonsterStageList->iSpawnMonsters = 3;

	int iCount = 0;

	const int iEnemyCountStage = 12; // Total Enemy in Stage
	const int iBossCountStage = 3;   // Total Boss in Stage

	// Get Count Data in SQL
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(Stage) FROM MapMonster WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitEnemy );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCount, 0 );
			}
		}
		pcDB->Close();
	}

	// Is Data?
	if ( iCount != 0 )
	{
		// Get Enemies in Stage
		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM MapMonster WHERE Stage=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitEnemy );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{

					// Stage Enemy Settings
					{
						pcDB->GetData( 3, PARAMTYPE_Integer, &pMonsterStageList->iSpawnMaxMonsters, 0 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &pMonsterStageList->iSpawnIntervalMax, 0 );
						pcDB->GetData( 5, PARAMTYPE_Integer, &pMonsterStageList->iSpawnMonsters, 0 );
						pMonsterStageList->iSpawnIntervalMax *= 1000;
						pMonsterStageList->iSpawnIntervalMin = 1 << 5;
					}

					// Get Enemy data
					for ( int i = 0; i < iEnemyCountStage; i++ )
					{
						char szEnemyName[32] = { 0 };
						pcDB->GetData( 6 + (i * 2), PARAMTYPE_String, szEnemyName, 32 );

						// Is enemy?
						if ( !IsNull( szEnemyName ) )
						{
							CharacterData * pEnemyInfo = GetCharacterDataByMonsterName( szEnemyName );

							// Is Enemy in Data?
							if ( pEnemyInfo )
							{
								STRINGCOPY( pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].szMonsterName, szEnemyName );
								pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].cd = pEnemyInfo;
								pcDB->GetData( 6 + (i * 2) + 1, PARAMTYPE_Integer, &pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].iSpawnRate, 0 );
								pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].iSpawnRateCum = pMonsterStageList->iTotalMonsterSpawnRate;
								pMonsterStageList->iTotalMonsterSpawnRate += pMonsterStageList->saMonster[pMonsterStageList->iTotalMonsters].iSpawnRate;
								pMonsterStageList->iTotalMonsters++;
							}
						}
					}

					// Get Boss data
					std::vector<std::string> vTimeBoss;
					for ( int i = 0; i < iBossCountStage; i++ )
					{
						char szBossName[32] = { 0 };
						pcDB->GetData( 30 + (i * 4), PARAMTYPE_String, szBossName, 32 );

						// Is Boss?
						if ( !IsNull( szBossName ) )
						{
							CharacterData * pBossInfo = GetCharacterDataByMonsterName( szBossName );

							// Is Boss in Data?
							if ( pBossInfo )
							{
								char szTimeBoss[128] = { 0 };
								pcDB->GetData( 30 + (i * 4) + 1, PARAMTYPE_String, szTimeBoss, 128 );
								vTimeBoss = split( szTimeBoss, ' ' );

								pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].cdBoss = pBossInfo;

								for ( UINT u = 0; u < vTimeBoss.size(); u++ )
								{
									pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].caBossHour[u] = (char)atoi( vTimeBoss[u].c_str() );
									pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].iBossHours++;
								}

								// Minion Boss
								char szMinionBoss[32]	= { 0 };
								int iMinionCount		= 0;
								pcDB->GetData( 30 + (i * 4) + 2, PARAMTYPE_String, szMinionBoss, 32 );
								pcDB->GetData( 30 + (i * 4) + 3, PARAMTYPE_Integer, &iMinionCount, 32 );

								// Is Minion Boss?
								if ( !IsNull( szMinionBoss ) )
								{
									CharacterData * pMinionBossInfo = GetCharacterDataByMonsterName( szMinionBoss );

									// Is Minion Boss in Data?
									if ( pMinionBossInfo )
									{
										pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].cdMinion	= pMinionBossInfo;
										pMonsterStageList->saBoss[pMonsterStageList->iTotalBosses].iMinions	= iMinionCount;
									}
								}
								pMonsterStageList->iTotalBosses++;
							}
						}
					}
				}
			}
			pcDB->Close();
		}
	}

	uCurrentStageUnitEnemy++;
}

void UnitInfoServer::CreateItemMemoryTable()
{

	if ( pAllocItemTable )
		return;

	DefinitionItem DefItem;

	ZeroMemory( ( void* )*( DefinitionItem** )0x07A9EB10, ITEM_SERVER_MAX * sizeof( DefinitionItem ) );

	// Create memory for Item Table
	{
		pAllocItemTable = ( ItemData* )VirtualAlloc( NULL, 0x314 * ( 1500 + 4 ), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );
		pTableNewItem = ( CompressedItem* )VirtualAlloc( NULL, 0x80 * ( 1500 + 4 ), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	}

	DefinitionItem * pDefItem = *( DefinitionItem** )0x07A9EB10;
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM ItemList ORDER BY ID ASC" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				for ( int i = 0; i < 1500; i++ )
				{
					// Code
					pcDB->GetData( 2, PARAMTYPE_Integer, &pDefItem[i].sItem.sItemID, 0 );

					// NAME
					pcDB->GetData( 3, PARAMTYPE_String, pDefItem[i].sItem.szItemName, 32 );


					// Item Table vars
					int iCode = 0;
					char szLastCategory[10] = { 0 };
					char szDropItem[64] = { 0 };
					int iWidth = 0;
					int iHeight = 0;
					char szItemPath[64] = { 0 };
					int iClass = 0;
					int iModelPosition = 0;
					int iSoundIndex = 0;
					int iWeaponClass = 0;
					{
						// Code
						iCode = pDefItem[i].sItem.sItemID.ToItemID();

						// Last Category
						pcDB->GetData( 5, PARAMTYPE_String, szLastCategory, 10 );

						// Drop Item Image
						pcDB->GetData( 6, PARAMTYPE_String, szDropItem, 64 );

						// Width x Height
						pcDB->GetData( 7, PARAMTYPE_Integer, &iWidth, 0 );
						pcDB->GetData( 8, PARAMTYPE_Integer, &iHeight, 0 );

						// Path File Item
						pcDB->GetData( 9, PARAMTYPE_String, szItemPath, 64 );

						// Class Item
						pcDB->GetData( 10, PARAMTYPE_Integer, &iClass, 0 );

						// Model Position
						pcDB->GetData( 11, PARAMTYPE_Integer, &iModelPosition, 0 );

						// Sound
						pcDB->GetData( 12, PARAMTYPE_Integer, &iSoundIndex, 0 );

						// Weapon Class
						pcDB->GetData( 13, PARAMTYPE_Integer, &iWeaponClass, 0 );
					}

					pcDB->GetData( 14, PARAMTYPE_Integer, &pDefItem[i].sItem.iItemUniqueID, 0 );

					// Item Table
					{
						// Is in table?
						if ( !pDefItem[i].sItem.iItemUniqueID )
						{
							// Code
							pAllocItemTable[iTableItemCompressPosition].sBaseItemID = pDefItem[i].sItem.sItemID;

							// Name
							STRINGCOPY( pAllocItemTable[iTableItemCompressPosition].szBaseName, pDefItem[i].sItem.szItemName );

							// Last Category
							STRINGCOPY( pAllocItemTable[iTableItemCompressPosition].szInventoryName, szLastCategory );

							// Drop Item Image
							STRINGCOPY( pAllocItemTable[iTableItemCompressPosition].szModelName, szDropItem );

							// Width x Height
							pAllocItemTable[iTableItemCompressPosition].iWidth = iWidth;
							pAllocItemTable[iTableItemCompressPosition].iHeight = iHeight;

							// Path File Item
							STRINGCOPY( pAllocItemTable[iTableItemCompressPosition].szCategory, szItemPath );

							// Class Item
							pAllocItemTable[iTableItemCompressPosition].iItemSlotFlag = (EItemSlotFlag)iClass;

							// Model Position
							pAllocItemTable[iTableItemCompressPosition].iVisibleItemType = (EItemSlotFlag)iModelPosition;

							// Sound
							pAllocItemTable[iTableItemCompressPosition].iInvItemSound = (EItemInvSound)iSoundIndex;

							// Weapon Class
							pAllocItemTable[iTableItemCompressPosition].iItemWpnDmg = iWeaponClass;

							ItemV += 0x314;

							CompressItemDataInTable( &pAllocItemTable[iTableItemCompressPosition] );
							iTableItemCompressPosition++;
						}
					}

					// Quest
					{
						// R
						pcDB->GetData( 15, PARAMTYPE_Short, &pDefItem[i].sItem.sEffectColor[0], 0 );

						// G
						pcDB->GetData( 16, PARAMTYPE_Short, &pDefItem[i].sItem.sEffectColor[1], 0 );

						// B
						pcDB->GetData( 17, PARAMTYPE_Short, &pDefItem[i].sItem.sEffectColor[2], 0 );

						// A
						pcDB->GetData( 18, PARAMTYPE_Short, &pDefItem[i].sItem.sEffectColor[3], 0 );

						// Flashing Time
						pcDB->GetData( 19, PARAMTYPE_Short, &pDefItem[i].sItem.sEffectBlink[0], 0 );


					}

					// Req. Level
					pcDB->GetData( 20, PARAMTYPE_Integer, &pDefItem[i].sItem.iLevel, 0 );

					// Req. Strength
					pcDB->GetData( 21, PARAMTYPE_Integer, &pDefItem[i].sItem.iStrength, 0 );

					// Req. Spirit
					pcDB->GetData( 22, PARAMTYPE_Integer, &pDefItem[i].sItem.iSpirit, 0 );

					// Req. Talent
					pcDB->GetData( 23, PARAMTYPE_Integer, &pDefItem[i].sItem.iTalent, 0 );

					// Req. Agility
					pcDB->GetData( 24, PARAMTYPE_Integer, &pDefItem[i].sItem.iAgility, 0 );

					// Req. Health
					pcDB->GetData( 25, PARAMTYPE_Integer, &pDefItem[i].sItem.iHealth, 0 );

					// Integrity Min
					pcDB->GetData( 26, PARAMTYPE_Short, &pDefItem[i].IntegrityMin, 0 );

					// Integrity Max
					pcDB->GetData( 27, PARAMTYPE_Short, &pDefItem[i].IntegrityMax, 0 );

					// Weight
					pcDB->GetData( 28, PARAMTYPE_Integer, &pDefItem[i].sItem.iWeight, 0 );

					// Price
					pcDB->GetData( 29, PARAMTYPE_Integer, &pDefItem[i].sItem.iSalePrice, 0 );

					// Organic
					pcDB->GetData( 30, PARAMTYPE_Short, &pDefItem[i].OrganicMin, 0 );
					pcDB->GetData( 31, PARAMTYPE_Short, &pDefItem[i].OrganicMax, 0 );

					// Fire
					pcDB->GetData( 32, PARAMTYPE_Short, &pDefItem[i].FireMin, 0 );
					pcDB->GetData( 33, PARAMTYPE_Short, &pDefItem[i].FireMax, 0 );

					// Frost
					pcDB->GetData( 34, PARAMTYPE_Short, &pDefItem[i].FrostMin, 0 );
					pcDB->GetData( 35, PARAMTYPE_Short, &pDefItem[i].FrostMax, 0 );

					// Lightning
					pcDB->GetData( 36, PARAMTYPE_Short, &pDefItem[i].LightningMin, 0 );
					pcDB->GetData( 37, PARAMTYPE_Short, &pDefItem[i].LightningMax, 0 );

					// Poison
					pcDB->GetData( 38, PARAMTYPE_Short, &pDefItem[i].PoisonMin, 0 );
					pcDB->GetData( 39, PARAMTYPE_Short, &pDefItem[i].PoisonMax, 0 );

					// Attack Power 1
					pcDB->GetData( 40, PARAMTYPE_Short, &pDefItem[i].AttackPower1Min, 0 );
					pcDB->GetData( 41, PARAMTYPE_Short, &pDefItem[i].AttackPower1Max, 0 );

					// Attack Power 2
					pcDB->GetData( 42, PARAMTYPE_Short, &pDefItem[i].AttackPower2Min, 0 );
					pcDB->GetData( 43, PARAMTYPE_Short, &pDefItem[i].AttackPower2Max, 0 );

					// Shooting Range
					pcDB->GetData( 44, PARAMTYPE_Integer, &pDefItem[i].sItem.iAttackRange, 0 );

					// Attack Speed
					pcDB->GetData( 45, PARAMTYPE_Integer, &pDefItem[i].sItem.iAttackSpeed, 0 );

					// Attack Rating
					pcDB->GetData( 46, PARAMTYPE_Short, &pDefItem[i].AttackRatingMin, 0 );
					pcDB->GetData( 47, PARAMTYPE_Short, &pDefItem[i].AttackRatingMax, 0 );

					// Critical
					pcDB->GetData( 48, PARAMTYPE_Integer, &pDefItem[i].sItem.iCritical, 0 );

					// Block
					pcDB->GetData( 49, PARAMTYPE_Float, &pDefItem[i].BlockRatingMin, 0 );
					pcDB->GetData( 50, PARAMTYPE_Float, &pDefItem[i].BlockRatingMax, 0 );

					// Absorb
					pcDB->GetData( 51, PARAMTYPE_Float, &pDefItem[i].AbsorbMin, 0 );
					pcDB->GetData( 52, PARAMTYPE_Float, &pDefItem[i].AbsorbMax, 0 );

					// Defense
					pcDB->GetData( 53, PARAMTYPE_Short, &pDefItem[i].DefenseMin, 0 );
					pcDB->GetData( 54, PARAMTYPE_Short, &pDefItem[i].DefenseMax, 0 );

					// Potions
					pcDB->GetData( 55, PARAMTYPE_Integer, &pDefItem[i].sItem.iPotionStorage, 0 );

					// Potions
					pcDB->GetData( 56, PARAMTYPE_Integer, &pDefItem[i].sItem.iPotionCount, 0 );

					// Regeneration HP, MP, STM
					pcDB->GetData( 57, PARAMTYPE_Float, &pDefItem[i].HPRegenMin, 0 );
					pcDB->GetData( 58, PARAMTYPE_Float, &pDefItem[i].HPRegenMax, 0 );
					pcDB->GetData( 59, PARAMTYPE_Float, &pDefItem[i].MPRegenMin, 0 );
					pcDB->GetData( 60, PARAMTYPE_Float, &pDefItem[i].MPRegenMax, 0 );
					pcDB->GetData( 61, PARAMTYPE_Float, &pDefItem[i].STMRegenMin, 0 );
					pcDB->GetData( 62, PARAMTYPE_Float, &pDefItem[i].STMRegenMax, 0 );

					// Add HP
					pcDB->GetData( 63, PARAMTYPE_Integer, &pDefItem[i].AddHPMin, 0 );
					pcDB->GetData( 64, PARAMTYPE_Integer, &pDefItem[i].AddHPMax, 0 );

					// Add MP
					pcDB->GetData( 65, PARAMTYPE_Integer, &pDefItem[i].AddMPMin, 0 );
					pcDB->GetData( 66, PARAMTYPE_Integer, &pDefItem[i].AddMPMax, 0 );

					// Add STM
					pcDB->GetData( 67, PARAMTYPE_Integer, &pDefItem[i].AddSTMMin, 0 );
					pcDB->GetData( 68, PARAMTYPE_Integer, &pDefItem[i].AddSTMMax, 0 );

					// Recovery HP
					pcDB->GetData( 69, PARAMTYPE_Short, &pDefItem[i].sItem.sHPRecovery.sMin, 0 );
					pcDB->GetData( 70, PARAMTYPE_Short, &pDefItem[i].sItem.sHPRecovery.sMax, 0 );

					// Recovery MP
					pcDB->GetData( 71, PARAMTYPE_Short, &pDefItem[i].sItem.sMPRecovery.sMin, 0 );
					pcDB->GetData( 72, PARAMTYPE_Short, &pDefItem[i].sItem.sMPRecovery.sMax, 0 );

					// Recovery STM
					pcDB->GetData( 73, PARAMTYPE_Short, &pDefItem[i].sItem.sSPRecovery.sMin, 0 );
					pcDB->GetData( 74, PARAMTYPE_Short, &pDefItem[i].sItem.sSPRecovery.sMax, 0 );

					// Run Speed
					pcDB->GetData( 75, PARAMTYPE_Float, &pDefItem[i].RunSpeedMin, 0 );
					pcDB->GetData( 76, PARAMTYPE_Float, &pDefItem[i].RunSpeedMax, 0 );

					// Specs
					pcDB->GetData( 77, PARAMTYPE_Integer, &pDefItem[i].sItem.eSpecialization, 0 );
					pDefItem[i].sItem.eSpecialization = CharacterClassToClassFlag((ECharacterClass)pDefItem[i].sItem.eSpecialization);

					for ( int j = 0; j < 12; j++ )
					{
						pcDB->GetData( 78 + j, PARAMTYPE_Integer, &pDefItem[i].JobBitCodeRandom[pDefItem[i].JobBitCodeRandomCount], 0 );
						if ( pDefItem[i].JobBitCodeRandom[pDefItem[i].JobBitCodeRandomCount] )
						{
							pDefItem[i].JobBitCodeRandom[pDefItem[i].JobBitCodeRandomCount] = CharacterClassToClassFlag((ECharacterClass)(j + 1));
							pDefItem[i].JobBitCodeRandomCount++;
						}
					}

					// Spec Data
					{
						// Run Speed
						pcDB->GetData( 90, PARAMTYPE_Float, &pDefItem[i].AddSpecRunSpeedMin, 0 );
						pcDB->GetData( 91, PARAMTYPE_Float, &pDefItem[i].AddSpecRunSpeedMax, 0 );

						// Absorb
						pcDB->GetData( 92, PARAMTYPE_Float, &pDefItem[i].AddSpecAbsorbMin, 0 );
						pcDB->GetData( 93, PARAMTYPE_Float, &pDefItem[i].AddSpecAbsorbMax, 0 );

						// Defense
						pcDB->GetData( 94, PARAMTYPE_Short, &pDefItem[i].AddSpecDefenseMin, 0 );
						pcDB->GetData( 95, PARAMTYPE_Short, &pDefItem[i].AddSpecDefenseMax, 0 );

						// Attack Speed
						pcDB->GetData( 96, PARAMTYPE_Integer, &pDefItem[i].sItem.sSpecData.iSpecAttackSpeed, 0 );

						// Critical
						pcDB->GetData( 97, PARAMTYPE_Integer, &pDefItem[i].sItem.sSpecData.iSpecCritical, 0 );

						// Attack Power
						pcDB->GetData( 98, PARAMTYPE_Short, &pDefItem[i].sItem.sSpecData.iSpecAttackPowerDivMin, 0 );
						pcDB->GetData( 99, PARAMTYPE_Short, &pDefItem[i].sItem.sSpecData.iSpecAttackPowerDiv, 0 );

						// Attack Rating
						pcDB->GetData( 100, PARAMTYPE_Integer, &pDefItem[i].SpecAttackRatingLevelMin, 0 );
						pcDB->GetData( 101, PARAMTYPE_Integer, &pDefItem[i].SpecAttackRatingLevelMax, 0 );

						// HP Regen
						pcDB->GetData( 102, PARAMTYPE_Float, &pDefItem[i].sItem.sSpecData.fSpecHPRegen, 0 );

						// MP Regen
						pcDB->GetData( 103, PARAMTYPE_Float, &pDefItem[i].AddSpecMPRegenMin, 0 );
						pcDB->GetData( 104, PARAMTYPE_Float, &pDefItem[i].AddSpecMPRegenMax, 0 );

						// STM Regen
						pcDB->GetData( 105, PARAMTYPE_Float, &pDefItem[i].sItem.sSpecData.fSpecSPRegen, 0 );

						// Block
						pcDB->GetData( 106, PARAMTYPE_Float, &pDefItem[i].sItem.sSpecData.fSpecBlockRating, 0 );

						// Shooting Range
						pcDB->GetData( 107, PARAMTYPE_Integer, &pDefItem[i].sItem.sSpecData.iSpecAttackRange, 0 );

						//Penetration
						pcDB->GetData( 108, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecPenetration.sMin, 0 );
						pcDB->GetData( 109, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecPenetration.sMax, 0 );

						//HP Potion Efficience
						pcDB->GetData( 110, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecHPPotionEfficience.sMin, 0 );
						pcDB->GetData( 111, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecHPPotionEfficience.sMax, 0 );

						//MP Potion Efficience
						pcDB->GetData( 112, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecMPPotionEfficience.sMin, 0 );
						pcDB->GetData( 113, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecMPPotionEfficience.sMax, 0 );

						//SP Potion Efficience
						pcDB->GetData( 114, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecSPPotionEfficience.sMin, 0 );
						pcDB->GetData( 115, PARAMTYPE_Short, &pDefinitionItemExTable[i].sSpecSPPotionEfficience.sMax, 0 );

					}

					pDefinitionItemExTable[i].eItemID = pDefItem[i].sItem.sItemID.ToItemID();

					// Cannot Drop
					pcDB->GetData( 116, PARAMTYPE_Integer, &pDefItem[i].sItem.bEnableEffect, 0 );


					typedef void( __cdecl *t_SetLowerItem ) (DefinitionItem * pItem);
					t_SetLowerItem SetLowerItem = (t_SetLowerItem)0x00439C70;

					memcpy( &DefItem, &pDefItem[i], sizeof( DefinitionItem ) );

					SetLowerItem( &DefItem );
					DefItem.sItem.iBackupKey = 0;
					DefItem.sItem.iBackupChk = 0;
					DefItem.sItem.tTime = 0;

					typedef int( __cdecl *t_EncodeCompress ) (BYTE *lpSrcData, BYTE *lpDestData, int size);
					t_EncodeCompress EncodeCompress = (t_EncodeCompress)0x00445480;

					pDefItem[i].DefCompressDataLen = EncodeCompress( (BYTE *)&DefItem.sItem, (BYTE *)pDefItem[i].DefCompressData, sizeof( Item ) );

					(*(UINT*)0x07AAC890) += 1;

					if ( !pcDB->Fetch() )
						break;
				}
			}
		}
		pcDB->Close();
	}

	allocIT = ( void* )pAllocItemTable;
	endIT();
	refIT();

	// Create Item table compressed
	{
		FILE * fp;
		INI::CReader cReader( "server.ini" );

		char szFilePath[ 256 ];
		STRINGFORMAT( szFilePath, "%s\\items.dat", cReader.ReadString( "Database", "NetFolder" ).c_str() );

		fopen_s( &fp, szFilePath, "wb" );
		if ( fp != NULL )
		{
			fseek( fp, 0, SEEK_SET );
			fwrite( ( void* )pTableNewItem, 1, 0x80 * ( ( *( UINT* )0x07AAC890 ) + 3 ), fp );
			fclose( fp );
		}
	}

}

void UnitInfoServer::ReadUnitNpcStand( Map * pcMap )
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	int iTotalUnits = 0;
	
	// Get Count NPCs in Stage

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(*) FROM MapNPC WHERE Stage=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitStand );
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iTotalUnits, 0 );
		}
		pcDB->Close();
	}
	
	if ( iTotalUnits != 0 )
	{
		// Pointer to packet PlayInfo[200] in StageData
		PacketUnitInfo * psPacket = pcMap->saNPC;


		// Get Data Array

		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM MapNPC WHERE Stage=?" ) )
			{
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &uCurrentStageUnitStand );

				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					for ( int i = 0; i < iTotalUnits; i++ )
					{
						BOOL bEnabled = FALSE;

						// SQL data
						pcDB->GetData( 3, PARAMTYPE_Integer, &psPacket[i].sPosition.iX, 0 );
						pcDB->GetData( 4, PARAMTYPE_Integer, &psPacket[i].sPosition.iY, 0 );
						pcDB->GetData( 5, PARAMTYPE_Integer, &psPacket[i].sPosition.iZ, 0 );
						pcDB->GetData( 6, PARAMTYPE_Integer, &psPacket[i].sAngle.iY, 0 );
						pcDB->GetData( 7, PARAMTYPE_String, psPacket[i].sCharacterData.szName, 32 );
						pcDB->GetData( 8, PARAMTYPE_Integer, &bEnabled, 0 );
						pcDB->GetData( 9, PARAMTYPE_Integer, &psPacket[i].sCharacterData.iClanID, 0 );

						// Packet structure
						psPacket[i].iLength = sizeof( PacketUnitInfo );

						// Is Enabled?
						if ( bEnabled )
							psPacket[i].iHeader = PKTHDR_NpcData;

						psPacket[i].sCharacterData.saUnused = 200; // ???
						psPacket[i].sCharacterData.sHP.sCur = 100; // HP default
						psPacket[i].sCharacterData.sHP.sMax = 100; // HP default
						psPacket[i].iID = 8100; // Default ID
						psPacket[i].sPosition.iX *= 256; // X << 8
						psPacket[i].sPosition.iY *= 256; // Y << 8
						psPacket[i].sPosition.iZ *= 256; // Z << 8
						pcDB->Fetch();
					}
				}
			}
			pcDB->Close();
		}
	}

	// Next Stage
	uCurrentStageUnitStand++;
}

void UnitInfoServer::ReadUnitNpcData( CharacterData * pCharInfo, UnitInfo *pMonInfo, char *pDialogMessage )
{
	if ( LOGIN_SERVER && (GAME_SERVER == FALSE) )
		return;

	int iUnitID = atoi( pCharInfo->szName );
	int iGMOnly = pCharInfo->iClanID;
	pCharInfo->iClanID = 0;

	pCharInfo->NPC.szHeadModel[0] = 0;
	pCharInfo->sSize = 0;
	pCharInfo->sViewBoxZoom.sMin = 0;
	pCharInfo->sViewBoxZoom.sMax = 0;
	
	ZeroMemory( ( void* )pMonInfo, sizeof( UnitInfo ) );
	pMonInfo->iMovementRange = 64 * 256;

	//NPC Movement Range
	if( iUnitID == 16 )
		pMonInfo->iMovementRange = 8 * 256;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );

	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM NPCList WHERE ID=?" ) )
		{
			pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iUnitID );
			if ( pcDB->Execute() && pcDB->Fetch() )
			{

				pcDB->GetData( 2, PARAMTYPE_String, pCharInfo->szName, 32 );
				pcDB->GetData( 3, PARAMTYPE_String, pCharInfo->NPC.szBodyModel, 64 );


				// Messages Npc
				{
					typedef char*(__cdecl *t_SetNpcMessage) (char * pszMessage);
					t_SetNpcMessage SetNpcMessage = (t_SetNpcMessage)0x00433390;

					// Set Npc Messages
					for ( int i = 0; i < 4; i++ )
					{
						pcDB->GetData( 4 + i, PARAMTYPE_String, pDialogMessage, 128 );

						// Is string and not null?
						if ( (lstrlenA( pDialogMessage ) > 0) && (!IsNull( pDialogMessage )) )
							pMonInfo->pszaMessage[pMonInfo->iNumMessage++] = SetNpcMessage( pDialogMessage );
					}
				}

				int iEventNpc = 0;
				int iEventParam = 0;
				int iTeleport = 0;

				//NPC Event data
				pcDB->GetData( 8, PARAMTYPE_Integer, &iEventNpc, 0 );
				pcDB->GetData( 9, PARAMTYPE_Integer, &iEventParam, 0 );

				//Skill Quests
				pcDB->GetData( 10, PARAMTYPE_Integer, &pMonInfo->iSkillMasterRank, 0 );

				//Quest ID
				pcDB->GetData( 11, PARAMTYPE_Integer, &pCharInfo->sGlow, 0 );

				//Quest Type
				pcDB->GetData( 12, PARAMTYPE_Integer, &pMonInfo->iQuestTypeID, 0 );

				//Quest TypeSub
				pcDB->GetData( 13, PARAMTYPE_Integer, &pMonInfo->iQuestSubID, 0 );

				//Teleport
				pcDB->GetData( 14, PARAMTYPE_Integer, &iTeleport, 0 );

				//Event NPC
				switch ( iEventNpc )
				{
					case NPCID_EventGirl:
						pMonInfo->iEventGirl = TRUE;
						break;

					case NPCID_ItemDistributor:
						pMonInfo->iItemBox = TRUE;
						break;

					case NPCID_Warehouse:
						pMonInfo->iWarehouse = TRUE;
						break;

					case NPCID_Force:
						pMonInfo->iMixMaster = NPCEVENTCODE_Force;
						break;

					case NPCID_CastleControl:
						pMonInfo->iCastleController = TRUE;
						if ( iEventParam != 0 )
							pMonInfo->iCastleController = iEventParam;
						break;

					case NPCID_Aging:
						pMonInfo->iAgeMaster = TRUE;
						break;

					case NPCID_Smelting:
						pMonInfo->iSmelting = NPCEVENTCODE_Smelting;
						break;

					case NPCID_ClanControl:
						pMonInfo->iClanController = TRUE;
						break;

					case NPCID_Teleport:
						pMonInfo->iTeleport = iTeleport;
						break;

					case NPCID_SkillMaster:
						pMonInfo->iSkillMaster = TRUE;
						break;

					case NPCID_Mixing:
						pMonInfo->iMixMaster = TRUE;
						break;

					case NPCID_Manufacturing:
						pMonInfo->iSmelting = NPCEVENTCODE_Manufacture;
						break;

					case NPCID_ResetItem:
						pMonInfo->iSmelting = NPCEVENTCODE_ResetItem;
						break;

					case NPCID_CoinShop:
						pMonInfo->iSmelting = NPCEVENTCODE_CoinShop;
						break;

					case NPCID_TimeShop:
						pMonInfo->iSmelting = NPCEVENTCODE_TimeShop;
						break;

					case NPCID_FuryArenaNPC:
						pMonInfo->iSmelting = NPCEVENTCODE_FuryArenaNPCTeleportCT2;
						break;

					case NPCID_FuryArenaNPCEnter:
						pMonInfo->iSmelting = NPCEVENTCODE_FuryArenaNPCEnter;
						break;

					case NPCID_SocketSystemNPCDrill:
						pMonInfo->iSmelting = NPCEVENTCODE_SocketSystemNPCDrill;
						break;

					case NPCID_SocketSystemNPCStone:
						pMonInfo->iSmelting = NPCEVENTCODE_SocketSystemNPCStone;
						break;

					case NPCID_NPCEasterEgg:
						pMonInfo->iSmelting = NPCEVENTCODE_EasterNPC;
						break;

					case NPCID_LarryQuests:
						pMonInfo->iSmelting = NPCEVENTCODE_LarryQuestsNPC;
						break;

					case NPCID_IhinEvent:
						pMonInfo->iSmelting = NPCEVENTCODE_IhinEvent;
						break;

					case NPCID_MarinaQuests:
						pMonInfo->iSmelting = NPCEVENTCODE_MarinaQuestsNPC;
						break;

					case NPCID_RudolphXmas:
						pMonInfo->iSmelting = NPCEVENTCODE_RudolphXmasNPC;
						break;

					case NPCID_ActionFieldEnter:
						pMonInfo->iSmelting = NPCEVENTCODE_ActionFieldEnter;
						break;

					case NPCID_ActionFieldFD1:
						pMonInfo->iSmelting = NPCEVENTCODE_ActionFieldFD1;
						break;

					case NPCID_ActionFieldFD2:
						pMonInfo->iSmelting = NPCEVENTCODE_ActionFieldFD2;
						break;

					case NPCID_ActionFieldFD3:
						pMonInfo->iSmelting = NPCEVENTCODE_ActionFieldFD3;
						break;

					case NPCID_ActionFieldDG1:
						pMonInfo->iSmelting = NPCEVENTCODE_ActionFieldDG1;
						break;

					case NPCID_ActionFieldExchange:
						pMonInfo->iSmelting = NPCEVENTCODE_ActionFieldExchange;
						break;

					case NPCID_BeginnerItemTimer:
						pMonInfo->iSmelting = NPCEVENTCODE_NPCBeginnerItemTimer;
						break;

					case NPCID_XmasTreeGift:
						pMonInfo->iSmelting = NPCEVENTCODE_XmasTreeGiftNPC;
						break;

					case NPCID_BattleRoyale:
						pMonInfo->iSmelting = NPCEVENTCODE_BattleRoyaleNPC;
						break;

					case NPCID_Carnival:
						pMonInfo->iSmelting = NPCEVENTCODE_CarnivalNPC;
						break;

					case NPCID_RankingLevel:
						pMonInfo->iSmelting = NPCEVENTCODE_RankingLevel;
						break;

					case NPCID_WarpGate:
						pMonInfo->iWarpgate = TRUE;
						if ( iEventParam != 0 )
							pMonInfo->iWarpgate = iEventParam;
						break;

					case NPCID_Bellatra:
						pMonInfo->iBellatraTypeNPC = TRUE;
						if ( iEventParam != 0 )
							pMonInfo->iBellatraTypeNPC = iEventParam;
						break;

					default:
						break;
				}

				// Weapon Shop
				{
					char szShopBuffer[512] = { 0 };
					ItemData * pItem = NULL;
					pcDB->GetData( 15, PARAMTYPE_String, szShopBuffer, 512 );
					std::vector<std::string> strShopItems = split( szShopBuffer, ' ' );

					for ( auto v : strShopItems )
					{
						pItem = FindItemPointerTable( v.c_str() );
						if ( pItem )
							pMonInfo->dwaWeaponShop[pMonInfo->iWeaponShopCounter++] = (DWORD)pItem->sBaseItemID.ToItemID();

						if ( pMonInfo->iWeaponShopCounter == 32 )
							break;
					}
				}

				// Defense Shop
				{
					char szShopBuffer[512] = { 0 };
					ItemData * pItem = NULL;
					pcDB->GetData( 16, PARAMTYPE_String, szShopBuffer, 512 );
					std::vector<std::string> strShopItems = split( szShopBuffer, ' ' );

					for ( auto v : strShopItems )
					{
						pItem = FindItemPointerTable( v.c_str() );
						if ( pItem )
							pMonInfo->dwaDefenseShop[pMonInfo->iDefenseShopCounter++] = (DWORD)pItem->sBaseItemID.ToItemID();

						if ( pMonInfo->iDefenseShopCounter == 32 )
							break;
					}
				}

				// Misc Shop
				{
					char szShopBuffer[512] = { 0 };
					ItemData * pItem = NULL;
					pcDB->GetData( 17, PARAMTYPE_String, szShopBuffer, 512 );
					std::vector<std::string> strShopItems = split( szShopBuffer, ' ' );

					for ( auto v : strShopItems )
					{
						pItem = FindItemPointerTable( v.c_str() );
						if ( pItem )
							pMonInfo->dwaMiscShop[pMonInfo->iMiscShopCounter++] = (DWORD)pItem->sBaseItemID.ToItemID();

						if ( pMonInfo->iMiscShopCounter == 32 )
							break;
					}
				}

				// GM Only
				pMonInfo->bGMOnly = iGMOnly;

				// Set pcUnitData pointer
				pCharInfo->psUnitInfo = (UnitInfo*)pMonInfo;
			}
		}
		pcDB->Close();
	}

	char szModelFile[ 64 ] = { 0 };
	STRINGCOPY( szModelFile, pCharInfo->NPC.szBodyModel );
	ReadInx( szModelFile );

	STRINGCOPY( szModelFile, pCharInfo->NPC.szBodyModel );
	typedef void( __cdecl *t_ClassMob ) ( char * pszModelFile );
	t_ClassMob LoadOftenMeshPattern = ( t_ClassMob )0x00442860;
	LoadOftenMeshPattern( szModelFile );

}

void UnitInfoServer::OnDescriptionSpawn( int iMapID, int iX, int iZ, char * pszDescription )
{
	SpawnDataInfo::SpawnData * ps = MAPSERVER->GetMapSpawnData( iMapID, iX, iZ );

	if ( ps )
		STRINGCOPYLEN( pszDescription, 32, ps->szName );
}

void UnitInfoServer::SetupMonsterBellatra( UINT BellatraMemory, UINT ConfigMemory )
{
	// Total Rounds
	const int iRound = 8;

	// Set Memory Pointer bellatra
	UINT uMemBellatra = BellatraMemory;

	// Loop Struct Round
	for ( int i = 0; i < iRound; i++ )
	{
		// Rounds
		*( int* )( ConfigMemory + 0x10 ) += 1;

		SQLConnection * pcDB = SQLCONNECTION( DATABASEID_EventDB );


		if ( pcDB->Open() )
		{
			if ( pcDB->Prepare( "SELECT * FROM BellatraMonster WHERE Round=?" ) )
			{
				int iRoundCur = i + 1;
				pcDB->BindParameterInput( 1, PARAMTYPE_Integer, &iRoundCur );
				if ( pcDB->Execute() && pcDB->Fetch() )
				{
					// Set Monsters
					for ( int j = 0; j < 5; j++ )
					{
						char szMonsterName[32] = { 0 };
						int iMonsterCount = 0;
						pcDB->GetData( 3 + (j * 2), PARAMTYPE_String, szMonsterName, 32 );
						pcDB->GetData( 4 + (j * 2), PARAMTYPE_Integer, &iMonsterCount, 0 );
						STRINGCOPYLEN( (char*)(uMemBellatra + (0x28 * j)), 32, szMonsterName );
						(*(UINT*)(uMemBellatra + 0x24 + (0x28 * j))) = iMonsterCount;

						if ( j == 4 ) // Boss?
						{
							// Count Boss to appear in stage
							(*(UINT*)(0x08B80F50 + (i * 4))) = (*(UINT*)(uMemBellatra + 0x24 + (0x28 * j)));

							(*(UINT*)(uMemBellatra + 0x24 + (0x28 * j))) = 0;
						}
					}
					// View Sight
					int iViewSight = 0;
					pcDB->GetData( 13, PARAMTYPE_Integer, &iViewSight, 0 );
					

					// Hour to begin
					int iHour = 0;
					pcDB->GetData( 14, PARAMTYPE_Integer, &iHour, 0 );

					if ( *(UINT*)0x08B81130 == 0 )
					{
						*(UINT*)(uMemBellatra + 0x0CC) = iHour;
						if ( i == 0 )
						{
							*(UINT*)(0x006E3720 + 0xCC) = iHour;
							*(UINT*)(0x006E3EE0 + 0xCC) = iHour;
						}
					}

					// Gold
					int iGold = 0;
					pcDB->GetData( 15, PARAMTYPE_Integer, &iGold, 0 );
					*(UINT*)(uMemBellatra + 0x0D0) = iGold;

					// Delay to next round
					int iDelayRound = 0;
					pcDB->GetData( 16, PARAMTYPE_Integer, &iDelayRound, 0 );
					*(UINT*)(uMemBellatra + 0x0D8) = iDelayRound;

					// Percent Count Total
					int iMaxPercentCount = 0;
					pcDB->GetData( 17, PARAMTYPE_Integer, &iMaxPercentCount, 0 );
					*(UINT*)(uMemBellatra + 0x0C8) = iMaxPercentCount;

					// Max monsters in the round
					int iMaxMonsters = 0;
					pcDB->GetData( 18, PARAMTYPE_Integer, &iMaxMonsters, 0 );
					*(UINT*)(0x08B81078 + (i * 4)) = iMaxMonsters;
					*(UINT*)(uMemBellatra + 0x0D4) = iMaxMonsters;

				}
			}
			pcDB->Close();
		}
		uMemBellatra += 0x0DC;
	}
	UINT uLastHourStart = *(UINT*)(uMemBellatra - 0x0DC + 0x0CC);
	*(UINT*)(uMemBellatra + 0x0CC) = uLastHourStart + 3;
	if ( *(UINT*)(uMemBellatra + 0x0CC) > 23 )
		*(UINT*)(uMemBellatra + 0x0CC) -= 24;

	if ( *(UINT*)0x08B81130 == 1 )
	{
		*(UINT*)0x08B81130 = 0;
		*(UINT*)(BellatraMemory + 0xCC) = 6;
	}
}

void UnitInfoServer::Init()
{

}

void refMem( DWORD addr, DWORD value )
{
	DWORD old;
	VirtualProtect( ( void * )addr, 4, PAGE_EXECUTE_READWRITE, &old );
	*( DWORD* )addr = value;
};

/*
Altera todas as referencias para a nova a Tabela;
*/
void refIT()
{
	refMem( 0x00415E17, ( DWORD )allocIT );
	refMem( 0x00417090, ( DWORD )allocIT );
	refMem( 0x0041E5F9, ( DWORD )allocIT );
	refMem( 0x0041FD7B, ( DWORD )allocIT );
	refMem( 0x0041FDBB, ( DWORD )allocIT );
	refMem( 0x0042A15C, ( DWORD )allocIT );
	refMem( 0x0042E067, ( DWORD )allocIT );
	refMem( 0x0042E0A3, ( DWORD )allocIT );
	refMem( 0x0043686C, ( DWORD )allocIT );
	refMem( 0x00438A7C, ( DWORD )allocIT );
	refMem( 0x00438D6E, ( DWORD )allocIT );
	refMem( 0x00438F04, ( DWORD )allocIT );
	refMem( 0x0043901F, ( DWORD )allocIT );
	refMem( 0x004390C5, ( DWORD )allocIT );
	refMem( 0x00439165, ( DWORD )allocIT );
	refMem( 0x00442A1F, ( DWORD )allocIT );
	refMem( 0x0044FCE9, ( DWORD )allocIT );
	refMem( 0x00480CCF, ( DWORD )allocIT );
	refMem( 0x00480D45, ( DWORD )allocIT );
	refMem( 0x00485918, ( DWORD )allocIT );
	refMem( 0x00485994, ( DWORD )allocIT );
	refMem( 0x0048E63A, ( DWORD )allocIT );
	refMem( 0x0048E65E, ( DWORD )allocIT );
	refMem( 0x0048E844, ( DWORD )allocIT );
	refMem( 0x0048E866, ( DWORD )allocIT );
	refMem( 0x0048EE0A, ( DWORD )allocIT );
	refMem( 0x0048EE2E, ( DWORD )allocIT );
	refMem( 0x00498945, ( DWORD )allocIT );
	refMem( 0x0049896F, ( DWORD )allocIT );
	refMem( 0x004A47D3, ( DWORD )allocIT );
	refMem( 0x004A4826, ( DWORD )allocIT );
	refMem( 0x004A4870, ( DWORD )allocIT );
	refMem( 0x004A49CD, ( DWORD )allocIT );
	refMem( 0x004A4A3C, ( DWORD )allocIT );
	refMem( 0x004BB844, ( DWORD )allocIT );
	refMem( 0x004BB8FC, ( DWORD )allocIT );
	refMem( 0x004BB94F, ( DWORD )allocIT );
	refMem( 0x004BB97C, ( DWORD )allocIT );
	refMem( 0x004BE3D7, ( DWORD )allocIT );
	refMem( 0x004BE407, ( DWORD )allocIT );
	refMem( 0x004C1954, ( DWORD )allocIT );
	refMem( 0x004C1E30, ( DWORD )allocIT );
	refMem( 0x0054801D, ( DWORD )allocIT );
	refMem( 0x005594F3, ( DWORD )allocIT );
	refMem( 0x0055B672, ( DWORD )allocIT );
	refMem( 0x005819FB, ( DWORD )allocIT );
	refMem( 0x0043683B, ( DWORD )allocIT + 0x24 );
	refMem( 0x00438A50, ( DWORD )allocIT + 0x24 );
	refMem( 0x00438D38, ( DWORD )allocIT + 0x24 );
	refMem( 0x00438ED3, ( DWORD )allocIT + 0x24 );
	refMem( 0x00438FE8, ( DWORD )allocIT + 0x24 );
	refMem( 0x00439092, ( DWORD )allocIT + 0x24 );
	refMem( 0x00439132, ( DWORD )allocIT + 0x24 );
	refMem( 0x0043B3E4, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044FCB6, ( DWORD )allocIT + 0x24 );
	refMem( 0x00480CEE, ( DWORD )allocIT + 0x24 );
	refMem( 0x00480D6B, ( DWORD )allocIT + 0x24 );
	refMem( 0x0048594E, ( DWORD )allocIT + 0x24 );
	refMem( 0x0048E686, ( DWORD )allocIT + 0x24 );
	refMem( 0x0048E88A, ( DWORD )allocIT + 0x24 );
	refMem( 0x0048EE55, ( DWORD )allocIT + 0x24 );
	refMem( 0x004A47F5, ( DWORD )allocIT + 0x24 );
	refMem( 0x004A4849, ( DWORD )allocIT + 0x24 );
	refMem( 0x004A49F8, ( DWORD )allocIT + 0x24 );
	refMem( 0x004BB926, ( DWORD )allocIT + 0x24 );
	refMem( 0x004BE248, ( DWORD )allocIT + 0x24 );
	refMem( 0x004BE300, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C1976, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C1E5E, ( DWORD )allocIT + 0x24 );
	refMem( 0x005594C1, ( DWORD )allocIT + 0x24 );
	refMem( 0x00485982, ( DWORD )allocIT + 0x34 );
	refMem( 0x0048E75E, ( DWORD )allocIT + 0x34 );
	refMem( 0x0048E95B, ( DWORD )allocIT + 0x34 );
	refMem( 0x004C1E52, ( DWORD )allocIT + 0x34 );
	refMem( 0x004C1EEF, ( DWORD )allocIT + 0x34 );
	refMem( 0x0048598B, ( DWORD )allocIT + 0x38 );
	refMem( 0x0048E76A, ( DWORD )allocIT + 0x38 );
	refMem( 0x0048E961, ( DWORD )allocIT + 0x38 );
	refMem( 0x004C1E58, ( DWORD )allocIT + 0x38 );
	refMem( 0x004C1EE3, ( DWORD )allocIT + 0x38 );
	refMem( 0x00480CF5, ( DWORD )allocIT + 0x3C );
	refMem( 0x00480D72, ( DWORD )allocIT + 0x3C );
	refMem( 0x00485955, ( DWORD )allocIT + 0x3C );
	refMem( 0x0048E68D, ( DWORD )allocIT + 0x3C );
	refMem( 0x0048E891, ( DWORD )allocIT + 0x3C );
	refMem( 0x0048EE5C, ( DWORD )allocIT + 0x3C );
	refMem( 0x004A47FC, ( DWORD )allocIT + 0x3C );
	refMem( 0x004A4850, ( DWORD )allocIT + 0x3C );
	refMem( 0x004A49FF, ( DWORD )allocIT + 0x3C );
	refMem( 0x004BB931, ( DWORD )allocIT + 0x3C );
	refMem( 0x004C197D, ( DWORD )allocIT + 0x3C );
	refMem( 0x004C1E65, ( DWORD )allocIT + 0x3C );
	refMem( 0x0041E628, ( DWORD )allocIT + 0x7C );
	refMem( 0x00490A4D, ( DWORD )allocIT + 0x7C );
	refMem( 0x00548042, ( DWORD )allocIT + 0x7C );
	refMem( 0x0055B798, ( DWORD )allocIT + 0x7C );
	refMem( 0x004BB865, ( DWORD )allocIT + 0x80 );
	refMem( 0x00480D08, ( DWORD )allocIT + 0xFC );
	refMem( 0x00480D22, ( DWORD )allocIT + 0xFC );
	refMem( 0x00480D28, ( DWORD )allocIT + 0xFC );
	refMem( 0x00480D85, ( DWORD )allocIT + 0xFC );
	refMem( 0x00480D9F, ( DWORD )allocIT + 0xFC );
	refMem( 0x00480DA5, ( DWORD )allocIT + 0xFC );
	refMem( 0x00480DD5, ( DWORD )allocIT + 0xFC );
	refMem( 0x004851AF, ( DWORD )allocIT + 0xFC );
	refMem( 0x00485976, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E6A7, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E6C4, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E71A, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E764, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E781, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E8AB, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E8C8, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E918, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E967, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048E97E, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048EE76, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048EE93, ( DWORD )allocIT + 0xFC );
	refMem( 0x0048EE99, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A488C, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A4892, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A48BE, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A48C4, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A4A16, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A4A30, ( DWORD )allocIT + 0xFC );
	refMem( 0x004A4A36, ( DWORD )allocIT + 0xFC );
	refMem( 0x004BB9C7, ( DWORD )allocIT + 0xFC );
	refMem( 0x004BB9F1, ( DWORD )allocIT + 0xFC );
	refMem( 0x004BBA11, ( DWORD )allocIT + 0xFC );
	refMem( 0x004BBA17, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1990, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C19AA, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C19CF, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1A03, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1A09, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1E88, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1EA2, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1EE9, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1F27, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C1F48, ( DWORD )allocIT + 0xFC );
	refMem( 0x00490C87, ( DWORD )allocIT + 0x118 );
	refMem( 0x00415E2D, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0041E60B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00442A54, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00480CDE, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00480D5B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0048592B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0048E64B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0048E853, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0048EE1B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004967A6, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0049895F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004A47E2, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004A49DC, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004BB853, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004BB90B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004BB9D3, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004C1963, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004C1E3F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0054802C, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0055B693, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00581A53, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00436859, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x00438A69, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x00438D55, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x00438EF0, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x00439009, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x004390AF, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0043914F, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0043B401, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044FCD9, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x004BE26B, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x004BE31F, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x005594D6, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x004980EE, ( DWORD )allocIT + ItemV + 0x24 );
	refMem( 0x00442BAC, ( DWORD )allocIT + 1 );

};

/*
Finaliza a Tabela;
*/
void endIT()
{
	*( DWORD* )( ( DWORD )allocIT + ItemV ) = 0x0A;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 4 ) = 0x64;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 8 ) = 0x03E8;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x0C ) = 0x2710;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x10 ) = 0x0186A0;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x14 ) = 0x0F4240;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x18 ) = 0x989680;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x1C ) = 0x05F5E100;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x20 ) = 0x3B9ACA00;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x24 ) = 0x2710;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x2C ) = 0x12;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x30 ) = 7;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x34 ) = 0x0C;
};

