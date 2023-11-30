#include "stdafx.h"
#include "unit.h"

Unit pcaUnitServer[MAX_UNITS];

UnitData::UnitData()
{
	pcBody = nullptr;
	pcHead = nullptr;
#if defined(_GAME)
	Init();
#endif
}

UnitData::~UnitData()
{
}

ECharacterGender UnitData::GetCharacterGender()
{
	if( this->sCharacterData.iClass == 3 || this->sCharacterData.iClass == 5 || this->sCharacterData.iClass == 8 || this->sCharacterData.iClass == 9 )
		return CHARACTERGENDER_Female;

	return CHARACTERGENDER_Male;
}

BOOL UnitData::IsPlayer( bool bVerifyPvP )
{
    BOOL b = (sCharacterData.iType == CHARACTERTYPE_Player);
    if ( bVerifyPvP )
        b = (sCharacterData.iType == CHARACTERTYPE_Player) || (iCharacterType == CHARACTERTYPE_Player);

    return b;
}

NAKED void UnitData::Main()
{
	JMP( pfnMain );
}

NAKED void UnitData::Init()
{
	JMP( pfnInit );
}

NAKED void UnitData::SetPosition( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JMP( pfnSetPosition );
}

NAKED BOOL UnitData::Animate( EAnimationType eAnimationType )
{
	JMP( pfnAnimate );
}

NAKED BOOL UnitData::ChangeAnimationID( int iID, int iModelType )
{
	JMP( pfnChangeAnimationID );
}

NAKED int UnitData::HandleUnitDataBuffer( char * pBuffer, int iDelayTime, int iSleep )
{
	JMP( pfnHandleBufferData );
}

NAKED BOOL UnitData::NextTarget( int iX, int iY, int iZ )
{
	JMP( pfnSetNextTarget );
}

NAKED BOOL UnitData::SetTarget( int iX, int iZ )
{
	JMP( pfnSetTarget );
}

NAKED BOOL UnitData::SetTargetEx( int iX, int iY, int iZ )
{
	JMP( pfnSetTargetEx );
}

NAKED int UnitData::SetAngleByPosition( int iX, int iZ )
{
	JMP( pfnSetAngleByPosition );
}

NAKED BOOL UnitData::GetAnimationAttackPosition( int * iX, int * iY, int * iZ )
{
	JMP( pfnGetAnimAttackPosition );
}

NAKED void UnitData::LoadModel( char * pszModelFilePath )
{
	JMP( pfnLoadModel );
}

NAKED void UnitData::UpdateAttack( BOOL bUpdateTime )
{
	JMP( pfnUpdateAttack );
}

NAKED BOOL UnitData::Render()
{
	JMP( pfnRender );
}

NAKED void UnitData::SetHeadModelData( EXEModelData * psModelData )
{
	JMP( pfnSetHeadModelData );
}

NAKED void UnitData::SetBodyModelData( EXEModelData * psModelData )
{
	JMP( pfnSetBodyModelData );
}


NAKED void UnitData::Free()
{
	JMP( pfnCloseUnitData );
}

BOOL UnitData::Move( int iDistance )
{
	const int MAX_STAGE_INGAME = 2;
	const int FLAGSTAGE_OUT = (-32767);

	int iMeshWidth = 1000;
	int iMeshHeight = 1000;

	if ( pcBodyMeshContainer )
	{
		iMeshWidth = (*(int*)((DWORD)pcBodyMeshContainer + 0x294));
		iMeshHeight = (*(int*)((DWORD)pcBodyMeshContainer + 0x298));
	}

#if defined(_GAME)
	if ( GM_MODE )
	{
		if ( (*(bool*)0x00A16366) )
		{
			int * SINPT = (int*)(*(int*)0x0082C104);
			int * COSPT = (int*)(*(int*)0x0082C108);

			sPosition.iX += ((SINPT[sAngle.iY & 4095] >> 8) * iDistance) >> 8;
			sPosition.iZ += ((COSPT[sAngle.iY & 4095] >> 8) * iDistance) >> 8;

			return TRUE;
		}
	}

	Stage ** pcaStageGame = (Stage**)0x00CF4740;
	BaseMap ** pcaBaseMap = (BaseMap**)0x00CF4748;

	int & iStageWater = (*(int*)0x03500488);
#else
	if ( iSpecialType )
		return FALSE;

	Stage ** pcaStageGame = nullptr;
	BaseMap ** pcaBaseMap = nullptr;


	int & iStageWater = (*(int*)0x07366608);
#endif

	Point3D sPositionOld = sPosition;
	Point3D sPositionSafeOld = sPosition;
	Point3D sPositionChange = sPosition;

	int iLoadedMapIndexOld = iLoadedMapIndex;
	iLoadedMapIndex = -1;

	int iHeight = 0;

	auto FallingUpdate = [this, &sPositionChange]()-> void
	{
		if ( (sPosition.iY - sPositionChange.iY) > (8 << 8) )
		{
			if ( (GetAnimation() != ANIMATIONTYPE_Falling) && ((sPosition.iY - sPositionChange.iY) > (32 << 8)) )
			{
				iLastHeight = sPosition.iY - sPositionChange.iY;
				Animate( ANIMATIONTYPE_Falling );
			}

			iFloorHeight = sPositionChange.iY;
			sPosition.iY -= (8 << 8);
		}
		else
		{
			sPosition.iY = sPositionChange.iY;
			iFloorHeight = sPositionChange.iY;

			if ( GetAnimation() == ANIMATIONTYPE_Falling )
			{
				if ( iLastHeight > (200 << 8) )
					Animate( ANIMATIONTYPE_FallHigh );
				else
					Animate( ANIMATIONTYPE_FallLow );
			}
		}
	};

	auto UpdateDenyArea = [this, &sPositionSafeOld, &pcaBaseMap]()-> void
	{
		auto IsInArea = [this]( Point3D sDenyPosition, int iRange ) -> bool
		{
			bool bRet = false;

			Point2D & p = Point2D( sDenyPosition.iX, sDenyPosition.iZ );

			int iX = sPosition.iX >> 8;
			int iZ = sPosition.iZ >> 8;

			int iDX = iX - p.iX;
			int iDZ = iZ - p.iY;

			iDX *= iDX;
			iDZ *= iDZ;

			iRange *= iRange;

			int iRD = iDX + iDZ;

			if ( iRD < iRange )
				bRet = true;

			return bRet;
		};

		BaseMap * pcBaseMap = nullptr;
		if ( pcStage )
		{
			if ( pcMap && pcMap->pcBaseMap )
				pcBaseMap = pcMap->pcBaseMap;
		}
		else
		{
			if ( pcaBaseMap )
			{
				if ( (iLoadedMapIndex >= 0) )
					pcBaseMap = pcaBaseMap[iLoadedMapIndex];
			}
		}

		if ( pcBaseMap )
		{
			//Prevention Crash Death Island
			if ( pcBaseMap->iMapID == MAPID_DeathIsland )
			{
				Point2D saPoints[] = { Point2D( 113165, 93472 ), Point2D( 114656, 93810 ), Point2D( 113423, 94606 ) };

				for ( int i = 0; i < _countof( saPoints ); i++ )
				{
					if ( IsInArea( Point3D( saPoints[i].iX, 0, saPoints[i].iY ), 155 ) )
						sPosition = sPositionSafeOld;
				}
			}

			//Prevention bypassing BRoyale
			if ( pcBaseMap->iMapID == MAPID_OldRuinen2 )
			{
				const Point3D sPositionPass = Point3D( 10624, 0, 11076 );
				if ( (sPosition.iZ >> 8) < sPositionPass.iZ )
					sPosition = sPositionSafeOld;
			}
		}
	};

	if ( (iDistance == 0) && (iFloorHeight < sPositionChange.iY) )
	{
		if ( pcStage )
			iHeight = pcStage->GetHighestPoint( sPositionChange.iX, sPositionChange.iY, sPositionChange.iZ, iMeshHeight );
		else
		{
			if ( pcaStageGame )
			{
				for ( int i = 0; i < MAX_STAGE_INGAME; i++ )
				{
					if ( pcaStageGame[i] )
					{
						int iH = pcaStageGame[i]->GetHighestPoint( sPositionChange.iX, sPositionChange.iY, sPositionChange.iZ, iMeshHeight );
						if ( i == 0 )
						{
							iHeight = iH;
							iLoadedMapIndex = i;
						}
						else
						{
							if ( iHeight < iH )
							{
								iHeight = iH;
								iLoadedMapIndex = i;
							}
						}
					}
				}
			}
		}

		if ( iHeight != FLAGSTAGE_OUT )
			sPositionChange.iY = iHeight;
		else
			sPositionChange.iY = (-80) << 8;

		FallingUpdate();

		UpdateDenyArea();

		return TRUE;
	}
	else
	{
		Point3D sMovePosition;
		if ( pcStage && (pcStage->CanMoveToPosition( &sPositionOld, &sAngle, &sMovePosition, iDistance, iMeshWidth, iMeshHeight, bCheckCollision ) > 0) )
			sPositionChange = sMovePosition;
		else
		{
			int iaMoveLevel[MAX_STAGE_INGAME] = {0,0};

			if ( pcaStageGame )
			{
				for ( int i = 0; i < MAX_STAGE_INGAME; i++ )
				{
					if ( pcaStageGame[i] && ((iaMoveLevel[i] = pcaStageGame[i]->CanMoveToPosition( &sPositionOld, &sAngle, &sMovePosition, iDistance, iMeshWidth, iMeshHeight, bCheckCollision )) > 0) )
					{
						if ( iStageWater != FLAGSTAGE_OUT )
							iWaterHeight = iStageWater;

						if ( pcaBaseMap[i]->iLevel <= sCharacterData.iLevel )
						{
							if ( i == 0 )
							{
								iLoadedMapIndex = i;

								iHeight			= sMovePosition.iY;

								sPositionChange = sMovePosition;
							}
							else
							{
								bool bUpdatePosition = false;

								if ( iaMoveLevel[0] && (abs( iHeight - sMovePosition.iY ) < (8 << 8)) )
								{
									if ( iaMoveLevel[0] > iaMoveLevel[1] )
										bUpdatePosition = true;
								}
								else if ( iHeight < sMovePosition.iY )
									bUpdatePosition = true;

								if ( bUpdatePosition )
								{
									sPositionChange = sMovePosition;

									iLoadedMapIndex = i;
								}
							}
						}
					}
				}
			}
		}
	}

	sPosition.iX = sPositionChange.iX;
	sPosition.iZ = sPositionChange.iZ;

	FallingUpdate();

	if ( (iLoadedMapIndex >= 0) && (iLoadedMapIndexOld != iLoadedMapIndex) )
	{
		iMapType = pcaBaseMap[iLoadedMapIndex]->iMapType;
		if ( (iLoadedMapIndexOld < 0) || (iMapType != pcaBaseMap[iLoadedMapIndexOld]->iMapType) )
			Animate( GetAnimation() );
	}

	if ( iLoadedMapIndex < 0 )
		iLoadedMapIndex = iLoadedMapIndexOld;

	UpdateDenyArea();

	return TRUE;
}

void Unit::Init( Unit * pcUnit, UnitData * pcUnitData )
{
	ZeroMemory( pcUnit, sizeof( Unit ) );
	pcUnit->iID = pcUnitData->iID;
	pcUnit->pcUnitData = pcUnitData;
	CopyMemory( &pcUnit->sLastRunningPosition, &pcUnitData->sPosition, sizeof( Point3D ) );
}


void Unit::Load()
{
	iID						= pcUnitData->iID;

	sLastRunningPosition	= Point3D( 0, 0, 0 );

	dwTimePositionUpdate	= 0;

	uLastUpdate				= 0;

	//Skills
	bFierceWind				= FALSE;
	
	bLethalSight			= FALSE;

	bHolyConviction			= FALSE;

	bWizardTrance			= FALSE;

	bWhirlWind				= false;

	bMagneticDischarge		= false;

	bKissViper				= false;

	iInertiaCount			= 0;

	uElementWeaponID		= 0;

	bPartyLeader			= false;

	for ( std::vector<CAchievementUser*>::iterator it = vAchievement.begin(); it != vAchievement.end(); it++ )
	{
		CAchievementUser * pc = (*it);
		DELET( pc );
	}
	vAchievement.clear();

	bFollowUnitType			= false;

	bCaravanFollow			= false;

	iCharacterXmasTreeID	= -1;

    iPvPKills               = 0;
    iPvPExperience          = 0;

    DELET( pcUnitDataMount );

	bSpecialEffect			= false;

#if defined(_GAME)
	fAnimationBlendingTime  = 1.0f;

	vMeshesAnimation.clear();
#endif

	bPvPMode				= false;

	iMountType				= 0;
}

void Unit::Load( UnitData * pcUnitData )
{
	this->pcUnitData = pcUnitData;

	Load();
}

UINT Unit::GetArrayPosition( UnitData * pcUnitData )
{
	UINT u = -1;

#if defined(_SERVER)
	UINT uaUnits = *(DWORD*)0x7AC3E78;
	u = ((UINT)pcUnitData - uaUnits) / 0x4B3C;
#endif

	return u;
}

EMapID Unit::GetMapID()
{
	if ( pcUnitData->pcMap == NULL )
		return MAPID_Invalid;

	if ( pcUnitData->pcMap->pcBaseMap == NULL )
		return MAPID_Invalid;

	return pcUnitData->pcMap->pcBaseMap->iMapID;
}
