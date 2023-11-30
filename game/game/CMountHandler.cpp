#include "stdafx.h"
#include "CMountHandler.h"

CMountHandler::CMountHandler()
{
	pcWindow = new CMountWindowHandler();
}

CMountHandler::~CMountHandler()
{
	for ( auto pc : vInstances )
		DELET( pc );

	vInstances.clear();

	DELET( pcWindow );
}

void CMountHandler::Init()
{
	pcWindow->Init();
}

bool CMountHandler::AddMount( UnitData * pcUnitData, EMountType eMountType )
{
	if ( auto pcInstance = GetInstanceByOwnerID( pcUnitData->iID ) )
	{
		if ( pcInstance->GetMountType() == eMountType )
			return false;
	}

	RemoveMount( pcUnitData );

	const auto & sMount = GetMount( eMountType );

	if ( pcUnitData == UNITDATA )
	{
		UNITDATA->Animate( ANIMATIONTYPE_Idle );

		UNITDATA->bNoDeath = 5 * 70;
	}

	auto pcMount = new CMountInstance();
	pcMount->SetID( iWheelID++ );
	pcMount->SetOwnerID( pcUnitData->iID );
	pcMount->SetMountType( sMount.iMountType );
	pcMount->SetUnitData( GetNewUnitData( sMount.szMountPath, pcUnitData->sPosition ) );
	pcMount->GetUnitData()->iID = pcUnitData->iID;

	vInstances.push_back( pcMount );

	UpdateAnimation( pcUnitData );

	pcUnitData->iModelHeight += sMount.iHeight;

	OnSpawnMount( pcUnitData, pcMount->GetUnitData(), sMount.iMountType );

	if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\MountSpawn.efk" ) )
	{
		pcScript->SetID( PARTICLEID_MAGNETIC_DISCHARGE + 2 );

		auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
		pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
		pcScript->SetOwner( pcOwner );

		EFFEKSEERHANDLER->AddScript( pcScript );
	}

	return true;
}

bool CMountHandler::RemoveMount( UnitData * pcUnitData )
{
	bool bRet = false;

	for ( auto it = vInstances.begin(); it != vInstances.end(); )
	{
		auto pcInstance = (*it);
		if ( pcInstance->GetOwnerID() == pcUnitData->iID )
		{
			const auto & sMount = GetMount( pcInstance->GetMountType() );

			pcUnitData->iModelHeight -= sMount.iHeight;

			DELET( pcInstance );
			it = vInstances.erase( it );

			bRet = true;

			if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\MountSpawn.efk" ) )
			{
				pcScript->SetID( PARTICLEID_MAGNETIC_DISCHARGE + 2 );

				auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
				pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
				pcScript->SetOwner( pcOwner );

				EFFEKSEERHANDLER->AddScript( pcScript );
			}
		}
		else
			it++;
	}

	if ( bRet )
		pcUnitData->Animate( pcUnitData->GetAnimation() );

	return bRet;
}

void CMountHandler::Render( UnitData * pcUnitData )
{
	UpdateAnimation( pcUnitData );

	if ( auto pcInstance = GetInstanceByOwnerID( pcUnitData->iID ) )
	{
		if ( auto pcUnitDataMount = pcInstance->GetUnitData() )
		{
			//Update Position
			Point3D sPosition = pcUnitData->sPosition;
			Point3D sAngle = pcUnitData->sAngle;

			sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

			pcUnitDataMount->pcStage = nullptr;

		//	pcUnitDataMount->SetPosition( sPosition.iX, sPosition.iY, sPosition.iZ, sAngle.iX, sAngle.iY, sAngle.iZ );

			//if ( auto [pcMeshOwner, pcMeshMount] = GetMeshes( pcUnitData, pcUnitDataMount ); (pcMeshOwner && pcMeshMount) )
			{
				Point3D sCameraPosition;
				sCameraPosition.iX = UNITTARGETPOSITION_X;
				sCameraPosition.iY = UNITTARGETPOSITION_Y;
				sCameraPosition.iZ = UNITTARGETPOSITION_Z;

				Point3D sCameraAngle;
				sCameraAngle.iX = UNITTARGETANGLE_X;
				sCameraAngle.iY = UNITTARGETANGLE_Y;
				sCameraAngle.iZ = UNITTARGETANGLE_Z;

				pcUnitDataMount->dwLastActiveTime = TICKCOUNT;

				//Update Frame of Animation
				pcUnitDataMount->iFrame = pcUnitDataMount->iStartFrame + (pcUnitData->iFrame - pcUnitData->iStartFrame);

				//Reset Animation
				if ( pcUnitDataMount->iFrame > (pcUnitDataMount->iEndFrame - 1) )
					pcUnitDataMount->iFrame = pcUnitDataMount->iEndFrame - 1;

				if ( pcUnitDataMount->iFrame < pcUnitDataMount->iStartFrame )
					pcUnitDataMount->iFrame = pcUnitDataMount->iStartFrame;

				//Render Body of Unit
				if ( pcUnitDataMount->pcBodyMeshContainer )
				{
					pcUnitDataMount->pcBodyMeshContainer->SetPosition( &sPosition, &Point3D( sAngle.iX, sAngle.iY, sAngle.iZ ) );

					if ( (pcUnitDataMount->pcBodyMeshContainer->pcBoneContainer) && (int)pcUnitDataMount->pcBodyMeshContainer->pcBoneContainer->uMaxFrame > pcUnitDataMount->iFrame )
						pcUnitDataMount->pcBodyMeshContainer->uFrame = (UINT)pcUnitDataMount->iFrame;

					if ( pcUnitDataMount->pcBodyMeshContainer->pcBoneContainer )
						pcUnitDataMount->pcBodyMeshContainer->pcBoneContainer->iLastFrame = -1;
					else
						pcUnitDataMount->pcBodyMeshContainer->iLastFrame = -1;

					//pcUnitDataMount->pcBodyMeshContainer->pcNewModel->GetMaterialCollection()->SetBlendingMaterial( GRAPHICENGINE->GetRainbowMaterial() );

					pcUnitDataMount->pcBodyMeshContainer->pcNewModel->Render( (IO::SMD::FrameInfo *)&pcUnitData->iStartFrame );
				}
			}
		}
	}
}

void CMountHandler::Update( UnitData * pcUnitData )
{

}

void CMountHandler::Update( float fTime )
{
	static float fKeyTimePressed = 0.0f;

	if ( Game::GetGameMode() == GAMEMODE_InGame )
	{
		if ( KEYBOARDHANDLER->IsKeyDown( VK_SPACE ) )
		{
			fKeyTimePressed += fTime;

			if ( fKeyTimePressed >= 2000.0f )
			{
				if ( UNITDATA->GetAnimation() != ANIMATIONTYPE_Die )
					Mount();

				fKeyTimePressed = 0.0f;
			}
		}
		else
			fKeyTimePressed = 0.0f;

		if ( IsRiding( UNITDATA ) )
		{
			if ( CanMount( UNITDATA ) == false )
			{
				RemoveMount( UNITDATA );

				TITLEBOX( AY_OBFUSCATE( "You can't mount at this place!" ) );
			}
		}
	}
}

void CMountHandler::UpdateAnimation( UnitData * pcUnitData )
{
	auto UpdateMountAnimation = []( UnitData * pcUnitDataOwner, UnitData * pcUnitDataMount ) -> void
	{
		if ( pcUnitDataMount->GetAnimation() != pcUnitDataOwner->GetAnimation() )
		{
			pcUnitDataMount->Animate( pcUnitDataOwner->GetAnimation() );
		}
	};

	if ( auto pcInstance = GetInstanceByOwnerID( pcUnitData->iID ) )
	{
		if ( auto pcUnitDataMount = pcInstance->GetUnitData(); pcUnitDataMount && (pcUnitDataMount != pcUnitData) )
		{
			const auto & sMountAnimation = GetCharacterAnimationID( pcInstance->GetMountType(), pcUnitData->sCharacterData.iClass, pcUnitData->GetAnimation() );

			if ( pcUnitData == UNITDATA )
			{
				if ( pcUnitData->iAnimationID != sMountAnimation.iCharacterAnimationID )
				{
					pcUnitData->ChangeAnimationID( sMountAnimation.iCharacterAnimationID );
				}

				UpdateMountAnimation( pcUnitData, pcUnitDataMount );
			}
			else
			{
				if ( pcUnitData->iAnimationID != sMountAnimation.iCharacterAnimationID )
					pcUnitData->ChangeAnimationID( sMountAnimation.iCharacterAnimationID );

				UpdateMountAnimation( pcUnitData, pcUnitDataMount );
			}
		}
	}
}

void CMountHandler::OnRenderCharacter( UnitData * pcUnitData )
{

}

bool CMountHandler::IsRiding( UnitData * pcUnitData )
{
	return GetInstanceByOwnerID( pcUnitData->iID ) != nullptr ? true : false;
}

EMountType CMountHandler::GetMountType( UnitData * pcUnitData )
{
	if ( auto pcInstance = GetInstanceByOwnerID( pcUnitData->iID ) )
		return pcInstance->GetMountType();

	return EMountType::MOUNTTYPE_None;
}

int CMountHandler::GetMountModelHeight( UnitData * pcUnitData )
{
	if ( auto pcInstance = GetInstanceByOwnerID( pcUnitData->iID ) )
		return GetMount( pcInstance->GetMountType() ).iHeight;

	return 0;
}

void CMountHandler::ShowMessage( EMountMessage iMessage )
{
	static UINT uTimeMessage = 0;

	if ( uTimeMessage > TICKCOUNT )
		return;

	switch ( iMessage )
	{
		case MOUNTMESSAGE_ErrorEnterNPC:
			TITLEBOX( AY_OBFUSCATE( "You can't enter while on a Mount!" ) );
			CHATGAMEHANDLER->AddChatBoxText( AY_OBFUSCATE( "> You can't enter while on a Mount!" ), CHATCOLOR_Global );
			break;

		case MOUNTMESSAGE_ErrorWalk:
			TITLEBOX( AY_OBFUSCATE( "You can't walk while on a Mount!" ) );
			CHATGAMEHANDLER->AddChatBoxText( AY_OBFUSCATE( "> You can't walk while on a Mount!" ), CHATCOLOR_Error );
			break;

		case MOUNTMESSAGE_ErrorBC:
			TITLEBOX( AY_OBFUSCATE( "You can't ride while at Bless Castle!" ) );
			CHATGAMEHANDLER->AddChatBoxText( AY_OBFUSCATE( "> You can't ride while at Bless Castle!" ), CHATCOLOR_Error );
			break;

		default:
			break;
	}

	uTimeMessage = TICKCOUNT + 2000;
}

BOOL CMountHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
	return FALSE;
}

BOOL CMountHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	if ( (Game::GetGameMode() == GAMEMODE_InGame) && (MAP_ID != MAPID_GhostCastle) )
	{
		if ( toupper( pcKeyboard->GetChar() ) == 'X' )
		{
			if ( GetWindow()->IsOpen() == FALSE )
				GetWindow()->Open();
			else
				GetWindow()->Close();
		}
	}

	return GetWindow()->OnKeyChar( pcKeyboard );
}

bool CMountHandler::CanMount( UnitData * pcUnitData )
{
	if ( (MAP_ID == MAPID_BlessCastle) || (MAP_ID == MAPID_OldRuinen2) || (MAP_ID == MAPID_RoyalDesert) )
		return false;

	return true;
}

void CMountHandler::HandlePacket( PacketMountUserList * psPacket )
{
	if ( psPacket->IsFirst() )
		GetWindow()->ClearMounts();

	if ( (psPacket->iCount > 0) && (psPacket->iCount <= MAX_MOUNTS_PER_DATA) )
	{
		for ( int i = 0; i < psPacket->iCount; i++ )
			GetWindow()->AddMount( psPacket->saMounts[i].iID, psPacket->saMounts[i].szName, psPacket->saMounts[i].szImageFile, psPacket->saMounts[i].eMountType, psPacket->saMounts[i].eRareType, psPacket->saMounts[i].saItemList, psPacket->saMounts[i].iItemCount,
								   psPacket->saMounts[i].bIsSelected );
	}
}

void CMountHandler::HandlePacket( PacketNewMount * psPacket )
{
	GetWindow()->AddMount( psPacket->iID, psPacket->szName, psPacket->szImageFile, psPacket->eMountType, psPacket->eRareType, psPacket->saItemList, psPacket->iItemCount, psPacket->bIsSelected );

	SAVE;
	SAVEEX;
}

void CMountHandler::SaveMounts()
{
	PacketMountUserList sPacket;
	std::vector<PacketMountUserList> vPackets;
	sPacket.iCount = 0;
	sPacket.sPacketCount.sMax = sPacket.sPacketCount.sMin = 0;

	for ( auto psData : GetWindow()->GetMounts() )
	{
		sPacket.saMounts[sPacket.iCount].iID = psData->iMountID;
		STRINGCOPY( sPacket.saMounts[sPacket.iCount].szName, psData->strName.c_str() );
		sPacket.saMounts[sPacket.iCount].eMountType = psData->eMountType;
		sPacket.saMounts[sPacket.iCount].eRareType = psData->eRareType;
		sPacket.saMounts[sPacket.iCount].bIsSelected = psData->bIsSelected;

		for ( UINT i = 0; i < psData->vItems.size(); i++ )
			CopyMemory( &sPacket.saMounts[sPacket.iCount].saItemList[i], &psData->vItems[i], sizeof( Item ) );

		sPacket.saMounts[sPacket.iCount].iItemCount = psData->vItems.size();

		sPacket.iCount++;

		if ( sPacket.iCount == MAX_MOUNTS_PER_DATA )
		{
			sPacket.sPacketCount.sMin++;
			vPackets.push_back( sPacket );

			sPacket.iCount = 0;
		}
	}

	if ( sPacket.iCount > 0 )
	{
		sPacket.sPacketCount.sMin++;
		vPackets.push_back( sPacket );
	}

	for ( auto & sSendPacket : vPackets )
	{
		sSendPacket.sPacketCount.sMax = (short)vPackets.size();

		ProcessPacket( &sSendPacket );
	}

	vPackets.clear();
}

bool CMountHandler::HasMount( EMountType eMountType )
{
	for ( auto psMount : GetWindow()->GetMounts() )
	{
		if ( psMount->eMountType == eMountType )
			return true;
	}

	return false;
}

void CMountHandler::Mount()
{
	if ( MOUNTHANDLER->IsRiding( UNITDATA ) == FALSE )
	{
		if ( GetWindow()->GetMountType() != EMountType::MOUNTTYPE_None )
		{
			if ( CanMount( UNITDATA ) )
				AddMount( UNITDATA, GetWindow()->GetMountType() );
			else
				TITLEBOX( AY_OBFUSCATE( "You can't mount at this place!" ) );
		}
	}
	else
		RemoveMount( UNITDATA );
}

UnitData * CMountHandler::GetNewUnitData( const std::string strBodyFile, Point3D sPosition )
{
	RENDERER->SetUseNewRenderToLoad( true );

	UnitData * pcUnitData = new UnitData;

	pcUnitData->Init();

	pcUnitData->pcStage = nullptr;

	UI::UnitMesh::LoadUnitData( pcUnitData, strBodyFile.c_str(), "" );
	pcUnitData->eUpdateMode = UPDATEMODE_ClientSelf;

	STRINGCOPY( pcUnitData->sCharacterData.szName, "Test" );
	pcUnitData->sCharacterData.sWarpHomeTown = 0;

	pcUnitData->SetPosition( sPosition.iX, sPosition.iY, sPosition.iZ, 0, 2048, 0 );

	pcUnitData->Animate( ANIMATIONTYPE_Idle );


	pcUnitData->bActive = TRUE;
	pcUnitData->bVisible = TRUE;

	pcUnitData->Main();

	RENDERER->ResetUseNewRenderToLoad();

    return pcUnitData;
}

CMountInstance * CMountHandler::GetInstanceByOwnerID( int iOwnerID )
{
	for ( auto pc : vInstances )
		if ( pc->GetOwnerID() == iOwnerID )
			return pc;

	return nullptr;
}

std::tuple<EXEMesh *, EXEMesh *> CMountHandler::GetMeshes( UnitData * pcUnitData, UnitData * pcUnitDataMount )
{
	EXEMesh * pcMountMesh = nullptr;
	EXEMesh * pcOwnerMesh = nullptr;

	if ( auto pcBoneMount = pcUnitDataMount->pcBoneContainer )
	{
		if ( pcUnitData )
		{
			if ( auto pcOwnerBone = pcUnitData->pcBoneContainer )
			{
				//Get Mount Bip
				pcMountMesh = pcBoneMount->GetMesh( "Bip01 Mount" );
				pcOwnerMesh = pcOwnerBone->GetMesh( "Bip01" );
			}
		}
	}

	return std::make_tuple( pcOwnerMesh, pcMountMesh );
}


const MountModelPath & CMountHandler::GetMount( EMountType eMountType )
{
	for ( const auto & sMount : saMountModels )
		if ( sMount.iMountType == eMountType )
			return sMount;

	return saMountModels[0];
}

const MountCharacterAnimation & CMountHandler::GetCharacterAnimationID( EMountType eMountType, ECharacterClass eCharacterClass, EAnimationType eAnimationType )
{
	const auto & sMount = GetMount( eMountType );

	for ( const auto & sMountAnimation : saMountCharacterAnimation )
		if ( (sMountAnimation.iMountType == sMount.eAnimationByMountType) && (sMountAnimation.eCharacterClass == eCharacterClass) && (sMountAnimation.eMountAnimationType == eAnimationType) )
			return sMountAnimation;

	return saMountCharacterAnimation[0];
}

void CMountHandler::OnSpawnMount( UnitData * pcUnitData, UnitData * pcUnitDataMount, EMountType eMountType )
{
	struct MountParticleKeep
	{
		std::string strParticleName;
		std::string strBipedName;

		MountParticleKeep( const std::string _strParticleName, const std::string _strBipedName )
		{
			strParticleName = _strParticleName;
			strBipedName	= _strBipedName;
		};

		~MountParticleKeep() {};
	};

	std::vector<MountParticleKeep> vParticleMounts;

	switch ( eMountType )
	{
		case EMountType::MOUNTTYPE_Hopy:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountHoptKeep_1.efk", "" ) );
			break;
		case EMountType::MOUNTTYPE_Unicorn:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_1.efk", "Bip01 Particle 1" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_2.efk", "Bip01 Particle 2" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_2.efk", "Bip01 Particle 3" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_2.efk", "Bip01 Particle 4" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_2.efk", "Bip01 Particle 5" ) );
			break;
		case EMountType::MOUNTTYPE_Unicorn_2:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_3.efk", "Bip01 Particle 1" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_4.efk", "Bip01 Particle 2" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_4.efk", "Bip01 Particle 3" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_4.efk", "Bip01 Particle 4" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountUnicornKeep_4.efk", "Bip01 Particle 5" ) );
			break;
		case EMountType::MOUNTTYPE_Wolf_2:
		case EMountType::MOUNTTYPE_Wolf_6:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountWolfKeep_1.efk", "Bip01 Particle 1" ) );
			break;
		case EMountType::MOUNTTYPE_Wolf_4:
		case EMountType::MOUNTTYPE_Wolf_8:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountWolfKeep_2.efk", "Bip01 Particle 2" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountWolfKeep_2.efk", "Bip01 Particle 3" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountWolfKeep_2.efk", "Bip01 Particle 4" ) );
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountWolfKeep_2.efk", "Bip01 Particle 5" ) );
			break;
		case EMountType::MOUNTTYPE_Pingu_1:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountPinguKeep_1.efk", "Bip01 Particle 1" ) );
			break;
		case EMountType::MOUNTTYPE_Pingu_2:
			vParticleMounts.push_back( MountParticleKeep( "game\\scripts\\particles\\MountPinguKeep_2.efk", "Bip01 Particle 1" ) );
			break;
		default:
			break;
	}

	if ( vParticleMounts.size() > 0 )
	{
		for ( const auto & s : vParticleMounts )
		{
			if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( s.strParticleName.c_str() ) )
			{
				pcScript->SetID( PARTICLEID_MOUNT_TYPE + (UINT)eMountType );

				auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitDataMount );
				pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
				if ( s.strBipedName.length() > 0 )
					pcOwner->SetBiped( s.strBipedName.c_str() );
				pcScript->SetOwner( pcOwner );

				EFFEKSEERHANDLER->AddScript( pcScript );
			}
		}
	}

	vParticleMounts.clear();
}

void CMountHandler::ProcessPacket( PacketMountUserList * psPacket )
{
	psPacket->iLength = sizeof( Packet ) + sizeof( MinMax ) + sizeof( int ) + (sizeof( MountUserListData ) * psPacket->iCount);
	psPacket->iHeader = PKTHDR_MountsList;
	SENDPACKETL( psPacket, TRUE );
}


CMountInstance::CMountInstance()
{
}

CMountInstance::~CMountInstance()
{
	if ( EFFEKSEERHANDLER )
		EFFEKSEERHANDLER->Kill( PARTICLEID_MOUNT_TYPE + (UINT)GetMountType(), pcUnitData->iID );

	pcUnitData->Free();

	DELET( pcUnitData );
}
