#include "stdafx.h"
#include "EXERefactorEngine3D.h"
#include "RawSMDHeader.h"
#include "RawSMDObjectInfo.h"

namespace UglyRefactor
{
bool __cdecl SearchObjLocalPos( UnitData * pcUnitData, PTMesh * pcMesh, Point3D * psMeshPositionOut )
{
	if ( (pcUnitData == NULL) || (pcMesh == NULL) )
		return false;

	EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

	psMeshPositionOut->iX = mWorld->i[3][0];
	psMeshPositionOut->iZ = mWorld->i[3][1];
	psMeshPositionOut->iY = mWorld->i[3][2];

	return true;
}
bool __cdecl SearchObjLocalPosEx( UnitData * pcUnitData, PTMesh * pcMesh, Point3D * psMeshPositionOut, int iLength )
{
	return SearchObjLocalPos( pcUnitData, pcMesh, psMeshPositionOut );
}


void AssaSkillTriumphOfValhallaPart::Start( UnitData * pcUnitDataEffect, int iTime )
{
	pcUnitData = pcUnitDataEffect;
	if ( (pcUnitData == NULL) || (pcUnitData->bVisible == FALSE) || (pcUnitData->bActive == FALSE) )
		return;
	
	if ( UNITDATA == pcUnitData )
		MyCharacterFlag = TRUE;

	sPosition = pcUnitData->sPosition;
	sPosition.iY += 10000;

	ParticleID = PARTICLE->StartPTMode( "Skill3AtalantaTriumphOfValhalla", sPosition );
	iTimeMax = 2000;

	pcMesh = pcUnitData->pcBoneContainer->GetMesh( "Bip01 R Hand" );


	iTimeMax = iTime * 70 + 150;
	dwID = SKILLID_TriumphofJavelin;
	EndFlag = FALSE;
}
void AssaSkillTriumphOfValhallaPart::StartHallOfValhalla( UnitData * pcUnitDataEffect, int iTime )
{
	pcUnitData = pcUnitDataEffect;
	if ( (pcUnitData == NULL) || (pcUnitData->bVisible == FALSE) || (pcUnitData->bActive == FALSE) )
		return;

	if ( UNITDATA == pcUnitData )
		MyCharacterFlag = TRUE;

	sPosition = pcUnitData->sPosition;
	sPosition.iY += 10000;

	ParticleID = PARTICLE->StartPTMode( "Skill3AtalantaTriumphOfValhalla", sPosition );
	ParticleIDExt = PARTICLE->StartPTMode( "Skill3AtalantaTriumphOfValhalla", sPosition );
	iTimeMax = 2000;

	if ( pcUnitData->pcBoneContainer )
	{
		pcMesh = pcUnitData->pcBoneContainer->GetMesh( "Bip01 R Hand" );
		pcMeshEx = pcUnitData->pcBoneContainer->GetMesh( "Bip01 R Forearm" );
	}

	iTimeMax = iTime * 70 + 150;
	dwID = SKILLID_HallofValhalla;
	EndFlag = FALSE;
}
void AssaSkillTriumphOfValhallaPart::MainFunction()
{
	if ( iTime == iTimeMax )
		return;

	if ( (PARTICLE->IsRunning( ParticleID ) == false) && EndFlag )
	{
		ParticleID = -1;
		ParticleIDExt = -1;
		return;
	}

	if ( MyCharacterFlag )
	{
		if ( UNITDATA != pcUnitData )
			pcUnitData = UNITDATA;
	}

	if ( ParticleID != -1 )
	{
		if ( pcMesh )
		{
			EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

			if ( ParticleID != -1 )
			{
				Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
				if ( pcMesh->pcNewMesh )
				{
					auto mMatrix = pcMesh->pcNewMesh->world;

					sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
				}

				X3D::Vector3 sCharacterPosition;
				sCharacterPosition.x = (float)(pcUnitData->sPosition.iX + sMeshPosition.iX);
				sCharacterPosition.y = (float)(pcUnitData->sPosition.iY + sMeshPosition.iY);
				sCharacterPosition.z = (float)(pcUnitData->sPosition.iZ + sMeshPosition.iZ);

				PARTICLE->SetAttachPosition( ParticleID, sCharacterPosition );
				PARTICLE->SetRendering( ParticleID, pcUnitData->bVisible );
			}
		}
	}

	if ( ParticleIDExt != -1 )
	{
		if ( pcMeshEx )
		{
			EXEMatrixI * mWorld = &pcMeshEx->GetWorldMatrix();

			if ( ParticleID != -1 )
			{
				Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
				if ( pcMeshEx->pcNewMesh )
				{
					auto mMatrix = pcMeshEx->pcNewMesh->world;

					sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
				}

				X3D::Vector3 sCharacterPosition;
				sCharacterPosition.x = (float)(pcUnitData->sPosition.iX + sMeshPosition.iX);
				sCharacterPosition.y = (float)(pcUnitData->sPosition.iY + sMeshPosition.iY);
				sCharacterPosition.z = (float)(pcUnitData->sPosition.iZ + sMeshPosition.iZ);

				PARTICLE->SetAttachPosition( ParticleIDExt, sCharacterPosition );
				PARTICLE->SetRendering( ParticleIDExt, pcUnitData->bVisible );
			}
		}
	}

	if ( EndFlag == FALSE )
	{
		if ( (iTime > (iTimeMax - 140)) ||
			(!MyCharacterFlag && pcUnitData->bActive == FALSE) ||
			(pcUnitData->GetAnimation() == ANIMATIONTYPE_Die) ||
			((pcUnitData->iLoadedMapIndex >= 0) && FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType == MAPTYPE_Town) )
		{
			iTime = iTimeMax - 140;
			if ( ParticleID != -1 )
				PARTICLE->SetFastStop( ParticleID );

			if ( ParticleIDExt != -1 )
				PARTICLE->SetFastStop( ParticleIDExt );

			EndFlag = TRUE;
		}
	}
}

void AssaSkill3SparkShield::Start( UnitData * pcUnitDataEffect, int iTime )
{
	pcUnitData = pcUnitDataEffect;
	if ( (pcUnitData == NULL) || (pcUnitData->bVisible == FALSE) || (pcUnitData->bActive == FALSE) )
		return;
	
	if ( UNITDATA == pcUnitData )
		MyCharacterFlag = TRUE;

	Point3D sCharacterPosition;
	sCharacterPosition = pcUnitData->sPosition;
	sCharacterPosition.iY += 10000;

	ParticleID = PARTICLE->StartPTMode( "Skill3MechanicianSparkShield1", sCharacterPosition );
	EndFlag = FALSE;
	iTimeMax = iTime * 70 + 140;
	dwID = SKILLID_SparkShield;

	pcMesh = pcUnitData->pcBoneContainer->GetMesh( "Bip01 L Hand" );
}

void AssaSkill3SparkShield::MainFunction()
{
	if ( ParticleID == -1 )
		return;

	if ( PARTICLE->IsRunning( ParticleID ) == false && EndFlag )
	{
		ParticleID = -1;
		return;
	}

	if ( MyCharacterFlag )
	{
		if ( UNITDATA != pcUnitData )
			pcUnitData = UNITDATA;
	}

	EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

	Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
	if ( pcMesh->pcNewMesh )
	{
		auto mMatrix = pcMesh->pcNewMesh->world;

		sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
	}

	X3D::Vector3 sCharacterPosition;
	sCharacterPosition.x = (float)(pcUnitData->sPosition.iX + sMeshPosition.iX);
	sCharacterPosition.y = (float)(pcUnitData->sPosition.iY + sMeshPosition.iY);
	sCharacterPosition.z = (float)(pcUnitData->sPosition.iZ + sMeshPosition.iZ);

	PARTICLE->SetAttachPosition( ParticleID, sCharacterPosition );
	PARTICLE->SetRendering( ParticleID, !((pcUnitData->bVisible == FALSE) || (pcUnitData->sLeftHandTool.eItemID == ITEMID_None)) );

	if ( EndFlag == FALSE )
	{
		if ( iTime > (iTimeMax - 140) ||
			(!MyCharacterFlag && pcUnitData->bActive == FALSE) ||
			(pcUnitData->GetAnimation() == ANIMATIONTYPE_Die) ||
			!(pcUnitData->eSkillDisplayFlag & SKILLMASK_SparkShield) ||
			(SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_ExtremeShield ) && iTime > 70) ||
			((pcUnitData->iLoadedMapIndex >= 0) && FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType == MAPTYPE_Town) )
		{
			iTime = iTimeMax - 140;
			if ( ParticleID != -1 )
				PARTICLE->SetFastStop( ParticleID );

			EndFlag = TRUE;
		}
	}
}

void AssaSkill3DivineInhalation::Start( UnitData * pcUnitDataEffect, int iTimeEffect )
{
	pcUnitData = pcUnitDataEffect;
	if ( (pcUnitData == NULL) || (pcUnitData->bVisible == FALSE) || (pcUnitData->bActive == FALSE) )
		return;

	if ( UNITDATA == pcUnitData )
		MyCharacterFlag = TRUE;

	Point3D sCharacterPosition;
	sCharacterPosition = pcUnitData->sPosition;
	sCharacterPosition.iY += 10000;

	ParticleID = PARTICLE->StartPTMode( "Skill3KnightDivineInhalation2", sCharacterPosition );
	EndFlag = FALSE;
	iTimeMax = iTimeEffect * 70 + 140;
	dwID = SKILLID_DivineShield;

	pcMesh = pcUnitData->pcBoneContainer->GetMesh( "Bip01 L Hand" );
}

void AssaSkill3DivineInhalation::MainFunction()
{
	if ( ParticleID == -1 )
		return;

	if ( PARTICLE->IsRunning( ParticleID ) == false && EndFlag )
	{
		ParticleID = -1;
		return;
	}

	if ( MyCharacterFlag )
	{
		if ( UNITDATA != pcUnitData )
			pcUnitData = UNITDATA;
	}

	EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

	Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
	if ( pcMesh->pcNewMesh )
	{
		auto mMatrix = pcMesh->pcNewMesh->world;

		sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
	}

	X3D::Vector3 sCharacterPosition;
	sCharacterPosition.x = (float)(pcUnitData->sPosition.iX + sMeshPosition.iX);
	sCharacterPosition.y = (float)(pcUnitData->sPosition.iY + sMeshPosition.iY);
	sCharacterPosition.z = (float)(pcUnitData->sPosition.iZ + sMeshPosition.iZ);

	PARTICLE->SetAttachPosition( ParticleID, sCharacterPosition );
	PARTICLE->SetRendering( ParticleID, !((pcUnitData->bVisible == FALSE) || (pcUnitData->sLeftHandTool.eItemID == ITEMID_None)) );

	if ( EndFlag == FALSE )
	{
		if ( iTime > (iTimeMax - 140) ||
			(!MyCharacterFlag && pcUnitData->bActive == FALSE) ||
			(pcUnitData->GetAnimation() == ANIMATIONTYPE_Die) ||
			!(pcUnitData->eSkillDisplayFlag & SKILLMASK_SparkShield) ||
			((pcUnitData->iLoadedMapIndex >= 0) && FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType == MAPTYPE_Town) )
		{
			iTime = iTimeMax - 140;
			if ( ParticleID != -1 )
				PARTICLE->SetFastStop( ParticleID );

			EndFlag = TRUE;
		}
	}
}

void AssaSkill3HolyIncantationPartHand::Start( UnitData * pcUnitDataEffect, int iTimeEffect )
{
	pcUnitData = pcUnitDataEffect;
	if ( (pcUnitData == NULL) || (pcUnitData->bVisible == FALSE) || (pcUnitData->bActive == FALSE) )
		return;

	if ( UNITDATA == pcUnitData )
		MyCharacterFlag = TRUE;

	Point3D sCharacterPosition;
	sCharacterPosition = pcUnitData->sPosition;
	sCharacterPosition.iY += 10000;

	ParticleID = PARTICLE->StartPTMode( "Skill3KnightHolyIncantationHand1", sCharacterPosition );
	ParticleIDExt = PARTICLE->StartPTMode( "Skill3KnightHolyIncantationHand2", sCharacterPosition );

	EndFlag = FALSE;
	iTimeMax = iTimeEffect * 70 + 140;

	pcMesh = pcUnitData->pcBoneContainer->GetMesh( "Bip01 L Hand" );
}
void AssaSkill3HolyIncantationPartHand::MainFunction()
{
	if ( ParticleID == -1 )
		return;

	if ( PARTICLE->IsRunning( ParticleID ) == false && EndFlag )
	{
		ParticleID = -1;
		return;
	}

	if ( MyCharacterFlag )
	{
		if ( UNITDATA != pcUnitData )
			pcUnitData = UNITDATA;
	}

	EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

	Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
	if ( pcMesh->pcNewMesh )
	{
		auto mMatrix = pcMesh->pcNewMesh->world;

		sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
	}

	X3D::Vector3 sCharacterPosition;
	sCharacterPosition.x = (float)(pcUnitData->sPosition.iX + sMeshPosition.iX);
	sCharacterPosition.y = (float)(pcUnitData->sPosition.iY + sMeshPosition.iY);
	sCharacterPosition.z = (float)(pcUnitData->sPosition.iZ + sMeshPosition.iZ);

	PARTICLE->SetAttachPosition( ParticleID, sCharacterPosition );
	PARTICLE->SetRendering( ParticleID, pcUnitData->bVisible );

	if ( ParticleIDExt != -1 )
	{
		PARTICLE->SetPosition( ParticleIDExt, sCharacterPosition );
		PARTICLE->SetRendering( ParticleIDExt, pcUnitData->bVisible );
	}

	if ( EndFlag == FALSE )
	{
		if ( iTime > iTimeMax - 140 ||
			(!MyCharacterFlag && pcUnitData->bActive == FALSE) ||
			(pcUnitData->GetAnimation() == ANIMATIONTYPE_Die) )

		{
			iTime = iTimeMax - 140;
			if ( ParticleID != -1 )
			{
				PARTICLE->SetFastStop( ParticleID );
			}

			if ( ParticleIDExt != -1 )
			{
				PARTICLE->Stop( ParticleIDExt );
				ParticleIDExt = -1;
			}

			EndFlag = TRUE;
		}
	}
}

void AssaSkillTriumphOfValhallaShot::Start( UnitData * pcUnitDataEffect, UnitData * pcUnitDataTarget, X3D::Vector3 * psVelocity, int iTimeEffect )
{
	int * piMaterialID = (int *)CALL_WITH_ARG2( 0x0053F1B0, (DWORD)(char*)"shield1.bmp", 4 );
	if ( piMaterialID == NULL )
		return;

	if ( (pcUnitDataEffect == NULL) || (pcUnitDataEffect->bVisible == FALSE) || (pcUnitDataEffect->bActive == FALSE) )
		return;

	if ( (pcUnitDataTarget == NULL) || (pcUnitDataTarget->bVisible == FALSE) || (pcUnitDataTarget->bActive == FALSE) )
		return;

	pcMesh = pcUnitDataTarget->pcBoneContainer->GetMesh( "Bip01 R Hand" );

	sPosition.x = (float)pcUnitDataEffect->sPosition.iX;
	sPosition.y = (float)(pcUnitDataEffect->sPosition.iY + 10000);
	sPosition.z = (float)pcUnitDataEffect->sPosition.iZ;

	pcTarget = pcUnitDataTarget;

	if ( psVelocity )
	{
		sVelocity.x = psVelocity->x;
		sVelocity.y = psVelocity->y;
		sVelocity.z = psVelocity->z;
	}
	Point3D sCharacterPosition;
	sCharacterPosition.iX = (int)sPosition.x;
	sCharacterPosition.iY = (int)sPosition.y;
	sCharacterPosition.iZ = (int)sPosition.z;
	ParticleID = PARTICLE->StartPTMode( "Skill3AtalantaTriumphOfValhalla", sCharacterPosition );
	iTimeMax = 2000;
	iDurationParticle = iTimeEffect;

	sFace.iMaterialID = *piMaterialID;
	sFace.iRed = 255;
	sFace.iGreen = 150;
	sFace.iBlue = 50;
	sFace.iTransparency = 200;
	sFace.iWidth = 5000;
	pcEffectTrace->SetLength( 20 );
}

void AssaSkillTriumphOfValhallaShot::MainFunction()
{
	EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

	Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
	if ( pcMesh->pcNewMesh )
	{
		auto mMatrix = pcMesh->pcNewMesh->world;

		sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
	}

	X3D::Vector3 sTargetPosition;
	sTargetPosition.x = float( pcTarget->sPosition.iX + sMeshPosition.iX );
	sTargetPosition.y = float( pcTarget->sPosition.iY + sMeshPosition.iY );
	sTargetPosition.z = float( pcTarget->sPosition.iZ + sMeshPosition.iZ );

	X3D::Vector3 sDifference;
	sDifference.x = float( (sTargetPosition.x - sPosition.x) / 256.0f );
	sDifference.y = float( (sTargetPosition.y - sPosition.y) / 256.0f );
	sDifference.z = float( (sTargetPosition.z - sPosition.z) / 256.0f );

	float length = sDifference.x * sDifference.x + sDifference.y * sDifference.y + sDifference.z * sDifference.z;
	length = (float)sqrt( length );
	if ( length < 15 || (pcTarget->GetAnimation() == ANIMATIONTYPE_Die) ||
		iTime > 100 )
	{
		if ( ParticleID != -1 )
		{
			PARTICLE->SetRendering( ParticleID, FALSE );
			PARTICLE->Stop( ParticleID );
			PARTICLE->SetFastStop( ParticleID );
			ParticleID = -1;
		}

		CALL_WITH_ARG2( 0x005BE5E0, (DWORD)pcTarget, iDurationParticle );

		iTime = iTimeMax;
	}

	X3D::Vector3 velocity;
	velocity.x = sDifference.x / length;
	velocity.y = sDifference.y / length;
	velocity.z = sDifference.z / length;

	sVelocity.x += velocity.x;
	sVelocity.y += velocity.y;
	sVelocity.z += velocity.z;


	if ( length < 100 )
		sVelocity *= 0.8f;

	sPosition += sVelocity * 256.0f;

	Point3D pos;
	pos.iX = (int)sPosition.x;
	pos.iY = (int)sPosition.y;
	pos.iZ = (int)sPosition.z;

	pcEffectTrace->AddData( &pos );
	if ( ParticleID != -1 )
		PARTICLE->SetAttachPosition( ParticleID, sPosition );
}

void __cdecl AssaParticle_DivineInhalationStart( UnitData * pcUnitDataEffect, int iTimeEffect )
{
	if ( (pcUnitDataEffect == NULL) || (pcUnitDataEffect->bVisible == FALSE) || (pcUnitDataEffect->bActive == FALSE) )
		return;

	CALL_WITH_ARG3( 0x005B48A0, (DWORD)pcUnitDataEffect, SKILLID_DivineShield, 0 );

	Point3D sCharacterPosition;
	Point3D sCurrentPosition;
	Point3D sRandomPosition;
	if ( pcUnitDataEffect->pcBoneContainer )
	{
		auto pcMesh = pcUnitDataEffect->pcBoneContainer->GetMesh( "Bip01 L Hand" );
		EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

		Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
		if ( pcMesh->pcNewMesh )
		{
			auto mMatrix = pcMesh->pcNewMesh->world;

			sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
		}

		sCharacterPosition.iX = (float)(pcUnitDataEffect->sPosition.iX + sMeshPosition.iX);
		sCharacterPosition.iY = (float)(pcUnitDataEffect->sPosition.iY + sMeshPosition.iY);
		sCharacterPosition.iZ = (float)(pcUnitDataEffect->sPosition.iZ + sMeshPosition.iZ);
	}
	else
	{
		sCharacterPosition = pcUnitDataEffect->sPosition;
		sCharacterPosition.iY += 10000;
	}

	SetDynamicLight( sCharacterPosition.iX, sCharacterPosition.iY, sCharacterPosition.iZ, 50, 100, 255, 0, 250, 2 );
	
	for ( int index = 0; index < 30; index++ )
	{
		sRandomPosition.iX = RandomI( -100, 100 );
		sRandomPosition.iY = RandomI( -100, 100 );
		sRandomPosition.iZ = RandomI( -100, 100 );

		double length = sqrt( (float)sRandomPosition.iX * (float)sRandomPosition.iX + (float)sRandomPosition.iY * (float)sRandomPosition.iY + (float)sRandomPosition.iZ * (float)sRandomPosition.iZ );

		if ( length == 0 )
			length = 1;

		int range = RandomI( 5000, 7000 );
		sCurrentPosition.iX = int( float( sRandomPosition.iX ) / length * (float)range + (float)sCharacterPosition.iX );
		sCurrentPosition.iY = int( float( sRandomPosition.iY ) / length * (float)range + (float)sCharacterPosition.iY );
		sCurrentPosition.iZ = int( float( sRandomPosition.iZ ) / length * (float)range + (float)sCharacterPosition.iZ );

		int iIndex = CALL_WITH_ARG6( 0x00534FC0, 130 + index, (DWORD)(char*)"flare.bmp", NULL, (DWORD)&sCurrentPosition, 0, 0 );

		EffectBaseFollow::GetData()[iIndex]->iTypeEffect = 2;
		EffectBaseFollow::GetData()[iIndex]->iTime = -(index / 2);

		EffectBaseFollow::GetData()[iIndex]->dwUnk01 = 0x000000F0;
		EffectBaseFollow::GetData()[iIndex]->sMovementSpeed.iX = int( float( sRandomPosition.iX ) / length * (150.f) );
		EffectBaseFollow::GetData()[iIndex]->sMovementSpeed.iY = int( float( sRandomPosition.iY ) / length * (150.f) );
		EffectBaseFollow::GetData()[iIndex]->sMovementSpeed.iZ = int( float( sRandomPosition.iZ ) / length * (150.f) );

		EffectBaseFollow::GetData()[iIndex]->sSize.iWidth = 500;
		EffectBaseFollow::GetData()[iIndex]->sFace.iRed = 100;
		EffectBaseFollow::GetData()[iIndex]->sFace.iGreen = 255;
		EffectBaseFollow::GetData()[iIndex]->sFace.iBlue = 255;
		EffectBaseFollow::GetData()[iIndex]->sFace.iTransparency = 255;

		Point3D sTargetPosition;
		sTargetPosition.iX = int( float( sRandomPosition.iX ) / length * 12000.f );
		sTargetPosition.iY = int( float( sRandomPosition.iY ) / length * 12000.f );
		sTargetPosition.iZ = int( float( sRandomPosition.iZ ) / length * 12000.f );

		EffectBaseFollow::GetData()[iIndex]->sScalePosition.iX = -sTargetPosition.iX;
		EffectBaseFollow::GetData()[iIndex]->sScalePosition.iY = -sTargetPosition.iY;
		EffectBaseFollow::GetData()[iIndex]->sScalePosition.iZ = -sTargetPosition.iZ;
	}

	PARTICLE->StartPTMode( "Skill3KnightDivineInhalation1", sCurrentPosition );
	CALL_WITH_ARG2( 0x005BDF80, (DWORD)(char *)pcUnitDataEffect, iTimeEffect );
}

void __cdecl SkillSaintGodlyShield( UnitData * pcUnitDataEffect, float fTimeEffect )
{
	if ( (pcUnitDataEffect == NULL) || (pcUnitDataEffect->bVisible == FALSE) || (pcUnitDataEffect->bActive == FALSE) )
		return;

	int iTimeEffect = (int)(fTimeEffect * 70.0f);

	Point3D sCharacterPosition;
	Point3D sCurrentPosition;
	Point3D sRandomPosition;
	if ( pcUnitDataEffect->pcBoneContainer )
	{
		auto pcMesh = pcUnitDataEffect->pcBoneContainer->GetMesh( "Bip01 L Hand" );
		EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();
		Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
		if ( pcMesh->pcNewMesh )
		{
			auto mMatrix = pcMesh->pcNewMesh->world;

			sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
		}

		sCharacterPosition.iX = (float)(pcUnitDataEffect->sPosition.iX + sMeshPosition.iX);
		sCharacterPosition.iY = (float)(pcUnitDataEffect->sPosition.iY + sMeshPosition.iY);
		sCharacterPosition.iZ = (float)(pcUnitDataEffect->sPosition.iZ + sMeshPosition.iZ);
	}
	else
	{
		sCharacterPosition = pcUnitDataEffect->sPosition;
		sCharacterPosition.iY += 10000;
	}

	SetDynamicLight( sCharacterPosition.iX, sCharacterPosition.iY, sCharacterPosition.iZ, 50, 100, 255, 0, 250, 2 );

	for ( int index = 0; index < 30; index++ )
	{
		sRandomPosition.iX = RandomI( -100, 100 );
		sRandomPosition.iY = RandomI( -100, 100 );
		sRandomPosition.iZ = RandomI( -100, 100 );

		double length = sqrt( (float)sRandomPosition.iX * (float)sRandomPosition.iX + (float)sRandomPosition.iY * (float)sRandomPosition.iY + (float)sRandomPosition.iZ * (float)sRandomPosition.iZ );

		if ( length == 0 )
			length = 1;

		int range = RandomI( 5000, 7000 );
		sCurrentPosition.iX = int( float( sRandomPosition.iX ) / length * (float)range + (float)sCharacterPosition.iX );
		sCurrentPosition.iY = int( float( sRandomPosition.iY ) / length * (float)range + (float)sCharacterPosition.iY );
		sCurrentPosition.iZ = int( float( sRandomPosition.iZ ) / length * (float)range + (float)sCharacterPosition.iZ );

		int iIndex = CALL_WITH_ARG6( 0x00534FC0, 130 + index, (DWORD)(char *)"flare.bmp", NULL, (DWORD)& sCurrentPosition, 0, 0 );

		EffectBaseFollow::GetData()[iIndex]->iTypeEffect = 2;
		EffectBaseFollow::GetData()[iIndex]->iTime = -(index / 2);

		EffectBaseFollow::GetData()[iIndex]->dwUnk01 = 0x000000F0;
		EffectBaseFollow::GetData()[iIndex]->sMovementSpeed.iX = int( float( sRandomPosition.iX ) / length * (150.f) );
		EffectBaseFollow::GetData()[iIndex]->sMovementSpeed.iY = int( float( sRandomPosition.iY ) / length * (150.f) );
		EffectBaseFollow::GetData()[iIndex]->sMovementSpeed.iZ = int( float( sRandomPosition.iZ ) / length * (150.f) );

		EffectBaseFollow::GetData()[iIndex]->sSize.iWidth = 500;
		EffectBaseFollow::GetData()[iIndex]->sFace.iRed = 100;
		EffectBaseFollow::GetData()[iIndex]->sFace.iGreen = 255;
		EffectBaseFollow::GetData()[iIndex]->sFace.iBlue = 255;
		EffectBaseFollow::GetData()[iIndex]->sFace.iTransparency = 255;

		Point3D sTargetPosition;
		sTargetPosition.iX = int( float( sRandomPosition.iX ) / length * 12000.f );
		sTargetPosition.iY = int( float( sRandomPosition.iY ) / length * 12000.f );
		sTargetPosition.iZ = int( float( sRandomPosition.iZ ) / length * 12000.f );

		EffectBaseFollow::GetData()[iIndex]->sScalePosition.iX = -sTargetPosition.iX;
		EffectBaseFollow::GetData()[iIndex]->sScalePosition.iY = -sTargetPosition.iY;
		EffectBaseFollow::GetData()[iIndex]->sScalePosition.iZ = -sTargetPosition.iZ;
	}

	PARTICLE->StartPTMode( "Skill3KnightDivineInhalation1", sCurrentPosition );
	CALL_WITH_ARG2( 0x005BDF80, (DWORD)pcUnitDataEffect, iTimeEffect );
}

BOOL __cdecl AssaParticleClanMonsterHit1( UnitData * pcUnitDataEffect )
{
	if ( (pcUnitDataEffect == NULL) || (pcUnitDataEffect->bVisible == FALSE) || (pcUnitDataEffect->bActive == FALSE) )
		return FALSE;

	if ( pcUnitDataEffect->pcBoneContainer == NULL )
		return FALSE;

	auto pcMesh = pcUnitDataEffect->pcBoneContainer->GetMesh( "bip01 s01" );
	if ( pcMesh == NULL )
		return FALSE;

	EXEMatrixI * mWorld = &pcMesh->GetWorldMatrix();

	Point3D sMeshPosition = Point3D( mWorld->i[3][0], mWorld->i[3][2], mWorld->i[3][1] );
	if ( pcMesh->pcNewMesh )
	{
		auto mMatrix = pcMesh->pcNewMesh->world;

		sMeshPosition = Point3D( (int)(mMatrix.m[3][0] * 256.0f), (int)(mMatrix.m[3][2] * 256.0f), (int)(mMatrix.m[3][1] * 256.0f) );
	}

	Point3D sCharacterPosition;
	sCharacterPosition.iX = (float)(pcUnitDataEffect->sPosition.iX + sMeshPosition.iX);
	sCharacterPosition.iY = (float)(pcUnitDataEffect->sPosition.iY);
	sCharacterPosition.iZ = (float)(pcUnitDataEffect->sPosition.iZ + sMeshPosition.iZ);

	Point3D sCurrentPosition;
	Point3D sTargetPosition;

	int iIndex = EffectBaseFollow::GetFreeIndex();
	auto pcEffectShot = new EffectBaseFollowShot;

	sCurrentPosition.iX = sCharacterPosition.iX;
	sCurrentPosition.iY = sCharacterPosition.iY + 50000;
	sCurrentPosition.iZ = sCharacterPosition.iZ;

	sTargetPosition.iX = sCharacterPosition.iX;
	sTargetPosition.iY = sCharacterPosition.iY + 5000;
	sTargetPosition.iZ = sCharacterPosition.iZ - 20000;
	pcEffectShot->Start( &sCurrentPosition, &sTargetPosition, 0x00AA );
	EffectBaseFollow::GetData()[iIndex] = pcEffectShot;

	iIndex = EffectBaseFollow::GetFreeIndex();
	pcEffectShot = new EffectBaseFollowShot;

	sCurrentPosition.iX = sCharacterPosition.iX;
	sCurrentPosition.iY = sCharacterPosition.iY + 50000;
	sCurrentPosition.iZ = sCharacterPosition.iZ;

	sTargetPosition.iX = sCharacterPosition.iX;
	sTargetPosition.iY = sCharacterPosition.iY + 5000;
	sTargetPosition.iZ = sCharacterPosition.iZ + 20000;
	pcEffectShot->Start( &sCurrentPosition, &sTargetPosition, 0x00AA );
	EffectBaseFollow::GetData()[iIndex] = pcEffectShot;

	iIndex = EffectBaseFollow::GetFreeIndex();
	pcEffectShot = new EffectBaseFollowShot;

	sCurrentPosition.iX = sCharacterPosition.iX;
	sCurrentPosition.iY = sCharacterPosition.iY + 50000;
	sCurrentPosition.iZ = sCharacterPosition.iZ;

	sTargetPosition.iX = sCharacterPosition.iX + 20000;
	sTargetPosition.iY = sCharacterPosition.iY + 5000;
	sTargetPosition.iZ = sCharacterPosition.iZ;
	pcEffectShot->Start( &sCurrentPosition, &sTargetPosition, 0x00AA );
	EffectBaseFollow::GetData()[iIndex] = pcEffectShot;

	iIndex = EffectBaseFollow::GetFreeIndex();
	pcEffectShot = new EffectBaseFollowShot;

	sCurrentPosition.iX = sCharacterPosition.iX;
	sCurrentPosition.iY = sCharacterPosition.iY + 50000;
	sCurrentPosition.iZ = sCharacterPosition.iZ;

	sTargetPosition.iX = sCharacterPosition.iX - 20000;
	sTargetPosition.iY = sCharacterPosition.iY + 5000;
	sTargetPosition.iZ = sCharacterPosition.iZ;
	pcEffectShot->Start( &sCurrentPosition, &sTargetPosition, 0x00AA );
	EffectBaseFollow::GetData()[iIndex] = pcEffectShot;

	return TRUE;
}

void ComputeStageNormals( Stage * pcStage )
{
    std::vector<int> vNormalStageCount;
    vNormalStageCount.resize( pcStage->iVertexCount );
    pcStage->vNormalVertices.resize( pcStage->iVertexCount );

    for ( int i = 0; i < pcStage->iVertexCount; i++ )
    {
        pcStage->vNormalVertices[i].faNormal[0] = 0.0f;
        pcStage->vNormalVertices[i].faNormal[1] = 256.0f;
        pcStage->vNormalVertices[i].faNormal[2] = 0.0f;

        pcStage->vNormalVertices[i].faTangent[0] = 0.0f;
        pcStage->vNormalVertices[i].faTangent[1] = 0.0f;
        pcStage->vNormalVertices[i].faTangent[2] = 0.0f;

        pcStage->vNormalVertices[i].faBitangent[0] = 0.0f;
        pcStage->vNormalVertices[i].faBitangent[1] = 0.0f;
        pcStage->vNormalVertices[i].faBitangent[2] = 0.0f;
    }

    for ( int m = 0; m < pcStage->iFaceCount; m++ )
    {

        auto sVertex0 = X3D::Vector3( pcStage->pVertex[pcStage->pFace[m].wVertex[0]].sPosition.iX / 256.0f,
                                      pcStage->pVertex[pcStage->pFace[m].wVertex[0]].sPosition.iY / 256.0f,
                                      pcStage->pVertex[pcStage->pFace[m].wVertex[0]].sPosition.iZ / 256.0f );

        auto sVertex1 = X3D::Vector3( pcStage->pVertex[pcStage->pFace[m].wVertex[1]].sPosition.iX / 256.0f,
                                      pcStage->pVertex[pcStage->pFace[m].wVertex[1]].sPosition.iY / 256.0f,
                                      pcStage->pVertex[pcStage->pFace[m].wVertex[1]].sPosition.iZ / 256.0f );

        auto sVertex2 = X3D::Vector3( pcStage->pVertex[pcStage->pFace[m].wVertex[2]].sPosition.iX / 256.0f,
                                      pcStage->pVertex[pcStage->pFace[m].wVertex[2]].sPosition.iY / 256.0f,
                                      pcStage->pVertex[pcStage->pFace[m].wVertex[2]].sPosition.iZ / 256.0f );

        auto vDeltaPosition1 = sVertex1 - sVertex0;
        auto vDelttaPosition2 = sVertex2 - sVertex0;


        X3D::Vector3 vNormal = vDeltaPosition1.cross( vDelttaPosition2 );

        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faNormal[0] += vNormal.x;
        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faNormal[1] += vNormal.y;
        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faNormal[2] += vNormal.z;
        vNormalStageCount[pcStage->pFace[m].wVertex[0]]++;

        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faNormal[0] += vNormal.x;
        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faNormal[1] += vNormal.y;
        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faNormal[2] += vNormal.z;
        vNormalStageCount[pcStage->pFace[m].wVertex[1]]++;

        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faNormal[0] += vNormal.x;
        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faNormal[1] += vNormal.y;
        pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faNormal[2] += vNormal.z;
        vNormalStageCount[pcStage->pFace[m].wVertex[2]]++;

        if ( pcStage->pFace[m].pTexture )
        {
            auto pTextureLink = (EXETextureLink *)pcStage->pFace[m].pTexture;

            auto sVertex0 = X3D::Vector3( pcStage->pVertex[pcStage->pFace[m].wVertex[0]].sPosition.iX / 256.0f,
                                          pcStage->pVertex[pcStage->pFace[m].wVertex[0]].sPosition.iY / 256.0f,
                                          pcStage->pVertex[pcStage->pFace[m].wVertex[0]].sPosition.iZ / 256.0f );

            auto sVertex1 = X3D::Vector3( pcStage->pVertex[pcStage->pFace[m].wVertex[1]].sPosition.iX / 256.0f,
                                          pcStage->pVertex[pcStage->pFace[m].wVertex[1]].sPosition.iY / 256.0f,
                                          pcStage->pVertex[pcStage->pFace[m].wVertex[1]].sPosition.iZ / 256.0f );

            auto sVertex2 = X3D::Vector3( pcStage->pVertex[pcStage->pFace[m].wVertex[2]].sPosition.iX / 256.0f,
                                          pcStage->pVertex[pcStage->pFace[m].wVertex[2]].sPosition.iY / 256.0f,
                                          pcStage->pVertex[pcStage->pFace[m].wVertex[2]].sPosition.iZ / 256.0f );

            auto uv0 = X3D::Vector2( pTextureLink->u[0], pTextureLink->v[0] );
            auto uv1 = X3D::Vector2( pTextureLink->u[1], pTextureLink->v[1] );
            auto uv2 = X3D::Vector2( pTextureLink->u[2], pTextureLink->v[2] );


            auto vDeltaPosition1 = sVertex1 - sVertex0;
            auto vDelttaPosition2 = sVertex2 - sVertex0;

            auto sDeltaUV1 = uv1 - uv0;
            auto sDeltaUV2 = uv2 - uv0;

            float r = 1.0f / (sDeltaUV1.x * sDeltaUV2.y - sDeltaUV1.y * sDeltaUV2.x);
            auto vTangent = (vDeltaPosition1 * sDeltaUV2.y - vDelttaPosition2 * sDeltaUV1.y) * r;
            auto vBitangent = (vDelttaPosition2 * sDeltaUV1.x - vDeltaPosition1 * sDeltaUV2.x) * r;

            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faTangent[0] += vTangent.x;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faTangent[1] += vTangent.y;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faTangent[2] += vTangent.z;

            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faTangent[0] += vTangent.x;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faTangent[1] += vTangent.y;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faTangent[2] += vTangent.z;

            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faTangent[0] += vTangent.x;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faTangent[1] += vTangent.y;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faTangent[2] += vTangent.z;

            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faBitangent[0] += vBitangent.x;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faBitangent[1] += vBitangent.y;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[0]].faBitangent[2] += vBitangent.z;

            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faBitangent[0] += vBitangent.x;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faBitangent[1] += vBitangent.y;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[1]].faBitangent[2] += vBitangent.z;

            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faBitangent[0] += vBitangent.x;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faBitangent[1] += vBitangent.y;
            pcStage->vNormalVertices[pcStage->pFace[m].wVertex[2]].faBitangent[2] += vBitangent.z;
        }
    }

    for ( int i = 0; i < pcStage->iVertexCount; i++ )
    {
        if ( vNormalStageCount[i] > 0 )
        {
            pcStage->vNormalVertices[i].faNormal[0] /= (float)vNormalStageCount[i];
            pcStage->vNormalVertices[i].faNormal[1] /= (float)vNormalStageCount[i];
            pcStage->vNormalVertices[i].faNormal[2] /= (float)vNormalStageCount[i];

            pcStage->vNormalVertices[i].faTangent[0] /= (float)vNormalStageCount[i];
            pcStage->vNormalVertices[i].faTangent[1] /= (float)vNormalStageCount[i];
            pcStage->vNormalVertices[i].faTangent[2] /= (float)vNormalStageCount[i];

            pcStage->vNormalVertices[i].faBitangent[0] /= (float)vNormalStageCount[i];
            pcStage->vNormalVertices[i].faBitangent[1] /= (float)vNormalStageCount[i];
            pcStage->vNormalVertices[i].faBitangent[2] /= (float)vNormalStageCount[i];

            pcStage->vNormalVertices[i].faNormal[0] /= 256.0f;
            pcStage->vNormalVertices[i].faNormal[1] /= 256.0f;
            pcStage->vNormalVertices[i].faNormal[2] /= 256.0f;

            pcStage->vNormalVertices[i].faTangent[0] /= 256.0f;
            pcStage->vNormalVertices[i].faTangent[1] /= 256.0f;
            pcStage->vNormalVertices[i].faTangent[2] /= 256.0f;

            pcStage->vNormalVertices[i].faBitangent[0] /= 256.0f;
            pcStage->vNormalVertices[i].faBitangent[1] /= 256.0f;
            pcStage->vNormalVertices[i].faBitangent[2] /= 256.0f;

			//Smooth Process		
			/*X3D::Vector3 vNormal  = X3D::Vector3( pcStage->vNormalVertices[i].faNormal[0],
												  pcStage->vNormalVertices[i].faNormal[1],
												  pcStage->vNormalVertices[i].faNormal[2] );

			X3D::Vector3 vBitangent = X3D::Vector3( pcStage->vNormalVertices[i].faBitangent[0],
												    pcStage->vNormalVertices[i].faBitangent[1],
												    pcStage->vNormalVertices[i].faBitangent[2] );

			X3D::Vector3 vTangent = X3D::Vector3( pcStage->vNormalVertices[i].faTangent[0],
												  pcStage->vNormalVertices[i].faTangent[1],
												  pcStage->vNormalVertices[i].faTangent[2] );

			X3D::Vector3 vSmoothedTangent = X3D::Vector3( vTangent - vNormal * vNormal.dot( vTangent ) );
			X3D::Vector3 vSmoothedBitangent = X3D::Vector3( vBitangent - vNormal * vNormal.dot( vBitangent ) );

			pcStage->vNormalVertices[i].faTangent[0] = vSmoothedTangent.x;
			pcStage->vNormalVertices[i].faTangent[1] = vSmoothedTangent.y;
			pcStage->vNormalVertices[i].faTangent[2] = vSmoothedTangent.z;

			pcStage->vNormalVertices[i].faBitangent[0] = vSmoothedBitangent.x;
			pcStage->vNormalVertices[i].faBitangent[1] = vSmoothedBitangent.y;
			pcStage->vNormalVertices[i].faBitangent[2] = vSmoothedBitangent.z;

			*/
		}
    }

    vNormalStageCount.clear();
}


BOOL LoadStageOld( Stage * pcStage, const char * pszFileName )
{
	BOOL bRet = FALSE;
	
	DWORD dwFileSize = 0;

	RawSMDHeader sRawHeader;
	if ( HANDLE hFile = CreateFile( pszFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); hFile != INVALID_HANDLE_VALUE )
	{
		CEncryptXOR * pcEncrypter = new CEncryptXOR();

		//Get the Header
		ReadFile( hFile, &sRawHeader, sizeof( RawSMDHeader ), &dwFileSize, NULL );

		PTStageVersion sVersion = PTStageVersion( sRawHeader.szHeader );

		if ( sVersion.IsValid() )
		{
			bool bEncrypted = false;

			//Encrypted?
			if ( sVersion.IsEncrypted() )
			{
				//So Encrypt
				bEncrypted = true;

				if ( bEncrypted )
					pcEncrypter->EncDecrypt( (BYTE *)&sRawHeader.iMaterialCount, sizeof( int ) );
			}

			//Read Stage File
			ReadFile( hFile, pcStage, sizeof( StageDataOld ), &dwFileSize, NULL );

			if ( bEncrypted )
			{
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iVertexCount, sizeof( int ) );
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iFaceCount, sizeof( int ) );
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iTextureCount, sizeof( int ) );
			}

			StageTexture * pTextureOld = pcStage->pTexture;

			//Has Material?
			if ( sRawHeader.iMaterialCount > 0 )
			{
				char * pData = new char[0x0058];

				CALLT_WITH_ARG1( 0x00489AE0, (DWORD)pData, (DWORD)hFile );

				pcStage->pMaterial = (void*)pData;

				pcStage->pMaterialData = (void*)READDWORD( ((DWORD)pData + 0x04) );
			}

			pcStage->pVertex = new StageVertex[pcStage->iVertexCount];
			ReadFile( hFile, pcStage->pVertex, sizeof( StageVertex ) * pcStage->iVertexCount, &dwFileSize, NULL );

            if ( sVersion.HasNormals() )
            {
                pcStage->vNormalVertices.resize( pcStage->iVertexCount );
                ReadFile( hFile, pcStage->vNormalVertices.data(), sizeof( StageNormalVertex ) * pcStage->iVertexCount, &dwFileSize, NULL );
            }

			pcStage->pFace = new StageFace[pcStage->iFaceCount];
			ReadFile( hFile, pcStage->pFace, sizeof( StageFace ) * pcStage->iFaceCount, &dwFileSize, NULL );

			pcStage->pTexture = new StageTexture[pcStage->iTextureCount];
			ReadFile( hFile, pcStage->pTexture, sizeof( StageTexture ) * pcStage->iTextureCount, &dwFileSize, NULL );

			if ( bEncrypted )
			{
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pVertex, sizeof( StageVertex ) * pcStage->iVertexCount );
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pFace, sizeof( StageFace ) * pcStage->iFaceCount );
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pTexture, sizeof( StageTexture ) * pcStage->iTextureCount );
			}

			if ( pcStage->iLightCount > 0 )
			{
				struct smLIGHT3D
				{
					int type;
					int x, y, z;
					int Range;
					short r, g, b;

				};

				pcStage->pLight = (void *)(new smLIGHT3D[pcStage->iLightCount]);
				ReadFile( hFile, pcStage->pLight, sizeof( smLIGHT3D ) * pcStage->iLightCount, &dwFileSize, NULL );

				if ( bEncrypted )
					pcEncrypter->EncDecrypt( (BYTE *)pcStage->pLight, sizeof( smLIGHT3D ) * pcStage->iLightCount );
			}

			//Fix Textures Coordinates
			int iPointerDifference = pcStage->pTexture - pTextureOld;
			for ( int i = 0; i < pcStage->iTextureCount; i++ )
			{
				if ( pcStage->pTexture[i].pNextTexture )
				{
					iPointerDifference = (StageTexture *)pcStage->pTexture[i].pNextTexture - pTextureOld;
					pcStage->pTexture[i].pNextTexture = pcStage->pTexture + iPointerDifference;
				}
			}

			//Fix Textures Coordinates in Faces
			for ( int i = 0; i < pcStage->iFaceCount; i++ )
			{
				if ( pcStage->pFace[i].pTexture )
				{
					iPointerDifference = (StageTexture *)pcStage->pFace[i].pTexture - pTextureOld;
					pcStage->pFace[i].pTexture = pcStage->pTexture + iPointerDifference;
				}
			}

			pcStage->pStageObject = new char[0x4B020];

			CALLT( 0x00488830, (DWORD)pcStage->pStageObject );

			pcStage->pdwAreaBuffer = new DWORD[pcStage->iAreaBufferSize];

			//Compute Area
			int iAreaBuffer = 0;
			for ( int i = 0; i < 256; i++ )
			{
				for ( int j = 0; j < 256; j++ )
				{
					if ( pcStage->pdwaArea[j][i] )
					{
						int iLengthArea = 0;
						ReadFile( hFile, &iLengthArea, sizeof( int ), &dwFileSize, NULL );
						
						pcStage->pdwaArea[j][i] = pcStage->pdwAreaBuffer + iAreaBuffer;

						ReadFile( hFile, pcStage->pdwaArea[j][i], sizeof( DWORD ) * iLengthArea, &dwFileSize, NULL );

						iAreaBuffer += iLengthArea;
					}
				}
			}

            if ( !sVersion.HasNormals() )
                ComputeStageNormals( pcStage );

			bRet = TRUE;
		}

		DELET( pcEncrypter );

		CloseHandle( hFile );

		pcStage->iCalcSum++;
	}


	return bRet;
}

BOOL SaveStageOld( Stage * pcStage, const char * pszFileName )
{
	const bool bEncrypt = FileExist( std::string( AY_OBFUSCATE( "decsmd.ini" ) ) ) ? false : true;

	BOOL bRet = FALSE;

	DWORD dwFileSize = 0;

	pcStage->bState = FALSE;

	RawSMDHeader sRawHeader;
	ZeroMemory( &sRawHeader, sizeof( RawSMDHeader ) );

	PTStageVersion sVersion = PTStageVersion( bEncrypt ? PTStageVersion::EPTStageVersion::PTSTAGEVERSION_0076 : PTStageVersion::EPTStageVersion::PTSTAGEVERSION_0072 );

	if ( HANDLE hFile = CreateFile( pszFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); hFile != INVALID_HANDLE_VALUE )
	{
		CEncryptXOR * pcEncrypter = new CEncryptXOR();

		STRINGCOPY( sRawHeader.szHeader, sVersion.GetStringVersion().c_str() );

		sRawHeader.iMaterialCount = 0;
		if ( pcStage->pMaterial )
			sRawHeader.iMaterialCount = READDWORD( ((DWORD)pcStage->pMaterial + 0x08) );

		int iFileLength = sizeof( RawSMDHeader );
		sRawHeader.iFilePointerToMaterials = iFileLength;

		if ( pcStage->pMaterial )
			iFileLength += CALLT( 0x00489700, (DWORD)pcStage->pMaterial );

		sRawHeader.iFilePointerToFirstObject = iFileLength;

		//Header
		{
			if ( bEncrypt )
				pcEncrypter->EncDecrypt( (BYTE *)&sRawHeader.iMaterialCount, sizeof( int ) );

			WriteFile( hFile, &sRawHeader, sizeof( RawSMDHeader ), &dwFileSize, NULL );

			if ( bEncrypt )
				pcEncrypter->EncDecrypt( (BYTE *)&sRawHeader.iMaterialCount, sizeof( int ) );

		}

		//Stage
		{
			if ( bEncrypt )
			{
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iVertexCount, sizeof( int ) );
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iFaceCount, sizeof( int ) );
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iTextureCount, sizeof( int ) );
			}

			WriteFile( hFile, pcStage, sizeof( StageDataOld ), &dwFileSize, NULL );

			if ( bEncrypt )
			{
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iVertexCount, sizeof( int ) );
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iFaceCount, sizeof( int ) );
				pcEncrypter->EncDecrypt( (BYTE *)&pcStage->iTextureCount, sizeof( int ) );
			}
		}

		if ( pcStage->pMaterial )
			CALLT_WITH_ARG1( 0x00489830, (DWORD)pcStage->pMaterial, (DWORD)hFile );

		//Vertex, Face and Textures
		{
            if ( sVersion.HasNormals() )
                ComputeStageNormals( pcStage );

			if ( bEncrypt )
			{
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pVertex, sizeof( StageVertex ) * pcStage->iVertexCount );
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pFace, sizeof( StageFace ) * pcStage->iFaceCount );
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pTexture, sizeof( StageTexture ) * pcStage->iTextureCount );
			}

			WriteFile( hFile, (BYTE *)pcStage->pVertex, sizeof( StageVertex ) * pcStage->iVertexCount, &dwFileSize, NULL );

            if ( sVersion.HasNormals() )
                WriteFile( hFile, pcStage->vNormalVertices.data(), sizeof( StageNormalVertex ) * pcStage->iVertexCount, &dwFileSize, NULL );


			WriteFile( hFile, (BYTE *)pcStage->pFace, sizeof( StageFace ) * pcStage->iFaceCount, &dwFileSize, NULL );
			WriteFile( hFile, (BYTE *)pcStage->pTexture, sizeof( StageTexture ) * pcStage->iTextureCount, &dwFileSize, NULL );

			if ( bEncrypt )
			{
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pVertex, sizeof( StageVertex ) * pcStage->iVertexCount );
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pFace, sizeof( StageFace ) * pcStage->iFaceCount );
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pTexture, sizeof( StageTexture ) * pcStage->iTextureCount );
			}
		}

		if ( pcStage->iLightCount > 0 )
		{
			struct smLIGHT3D
			{
				int type;
				int x, y, z;
				int Range;
				short r, g, b;

			};

			if ( bEncrypt )
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pLight, sizeof( smLIGHT3D ) * pcStage->iLightCount );

			WriteFile( hFile, pcStage->pLight, sizeof( smLIGHT3D ) * pcStage->iLightCount, &dwFileSize, NULL );

			if ( bEncrypt )
				pcEncrypter->EncDecrypt( (BYTE *)pcStage->pLight, sizeof( smLIGHT3D ) * pcStage->iLightCount );
		}

		//Compute Area
		for ( int i = 0; i < 256; i++ )
		{
			for ( int j = 0; j < 256; j++ )
			{
				if ( pcStage->pdwaArea[j][i] )
				{
					int iLengthArea = pcStage->pdwaArea[j][i][0] + 1;

					WriteFile( hFile, &iLengthArea, sizeof( int ), &dwFileSize, NULL );
					WriteFile( hFile, pcStage->pdwaArea[j][i], sizeof( DWORD ) * iLengthArea, &dwFileSize, NULL );
				}
			}
		}

		DELET( pcEncrypter );

		CloseHandle( hFile );
	}

	return bRet;
}

const BOOL CANUSE_FAKEBONE = FALSE;
bool FAKE_BONE_LOADER = false;

BOOL LoadModelOld( EXEModel * pcModel, const std::string strFileName, const std::string strNodeName )
{
	FAKE_BONE_LOADER = false;

	char * pszHash = AY_OBFUSCATE( "J4<:^;(|3<5:2#}|]0^0-,7$}8.!>,0%[J$,.>.{}}?>5)>|)[03PR(8*4-#[+}-;!9?|63}($*?%|51:^+;1;)$+15-:)!,;+#,}**@^|1!:7@<2:.4.!(87;}674]>" );

	pcModel->Init();

	auto pcBoneContainer = EXEModel::GetBone();

	BOOL bRet = FALSE;

	DWORD dwFileSize = 0;

	bool bIsBone = strFileName.find( ".smb" ) != std::string::npos;


	RawSMDHeader sRawHeader;
	if ( HANDLE hFile = CreateFile( strFileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ); hFile != INVALID_HANDLE_VALUE )
	{
		CEncryptXOR * pcEncrypter = new CEncryptXOR();

		//Get the Header
		ReadFile( hFile, &sRawHeader, sizeof( RawSMDHeader ), &dwFileSize, NULL );

		PTModelVersion sModelVersion = PTModelVersion( sRawHeader.szHeader );

		if ( sModelVersion.IsValid() )
		{
			bool bEncrypted = false;

			//Encrypted?
			if ( sModelVersion.IsEncrypted() )
			{
				//So Decrypt
				bEncrypted = true;
			}

			auto psaModelMeshes = new RawSMDObjectInfo[sRawHeader.iObjectCount];
			ReadFile( hFile, psaModelMeshes, sizeof( RawSMDObjectInfo ) * sRawHeader.iObjectCount, &dwFileSize, NULL );

			pcModel->iFrameCount = sRawHeader.iFrameCount;
			CopyMemory( pcModel->saFrame, sRawHeader.aFrame, sizeof( RawSMDFrame ) * pcModel->iFrameCount );

			if ( sRawHeader.iMaterialCount > 0 )
			{
				pcModel->pcTextureManager = new EXETextureManager();
				CALLT_WITH_ARG1( 0x00489AE0, (DWORD)pcModel->pcTextureManager, (DWORD)hFile );
			}

			if ( strNodeName.length() > 0 )
			{
				for ( int i = 0; i < sRawHeader.iObjectCount; i++ )
				{
					if ( STRINGCOMPAREI( strNodeName.c_str(), psaModelMeshes[i].szObjectName ) )
					{
						auto pcMesh = new EXEMesh();
						SetFilePointer( hFile, psaModelMeshes[i].iFilePointerToObject, NULL, FILE_BEGIN );
						LoadMeshOld( pcMesh, hFile, sModelVersion, pcBoneContainer, bIsBone );
						pcModel->AddMesh( pcMesh );
						break;
					}
				}
			}
			else
			{
				for ( int i = 0; i < sRawHeader.iObjectCount; i++ )
				{
					auto pcMesh = new EXEMesh();
					SetFilePointer( hFile, psaModelMeshes[i].iFilePointerToObject, NULL, FILE_BEGIN );
					LoadMeshOld( pcMesh, hFile, sModelVersion, pcBoneContainer, bIsBone );
					pcModel->AddMesh( pcMesh );
				}

				pcModel->UpdateMeshes();
			}

			DELET( psaModelMeshes );

			pcModel->pcNewModel = nullptr;
			pcModel->pcBoneContainer = pcBoneContainer;

			bRet = TRUE;
		}

		DELET( pcEncrypter );
		CloseHandle( hFile );

		pcModel->pcNewModel = nullptr;

		if( RENDERER->IsUsingNewRenderToLoad() && bRet )
		{
			pcModel->pcNewModel = new Graphics::Model();
			pcModel->pcNewModel->Load( strFileName, pcBoneContainer ? pcBoneContainer->pcNewModel : nullptr, false, (Delta3D::Legacy::Model*)pcModel );
			pcModel->pcNewModel->CleanMemory();
		}
	}

	return bRet;
}

BOOL SaveModelOld( EXEModel * pcModel, const std::string strFileName )
{
	bool bEncrypted = FileExist(std::string(AY_OBFUSCATE("decsmd.ini"))) ? false : true;

	PTModelVersion sModelVersion = PTModelVersion( bEncrypted ? PTModelVersion::EPTModelVersion::PTMODELVERSION_0066 : PTModelVersion::EPTModelVersion::PTMODELVERSION_0065 );

	if ( FileExist( std::string( AY_OBFUSCATE( "decsmd2.ini" ) ) ) )
		sModelVersion.eVersion = PTModelVersion::EPTModelVersion::PTMODELVERSION_0064;


	//Same Loader
	char * pszHash = AY_OBFUSCATE( "J4<:^;(|3<5:2#}|]0^0-,7$}8.!>,0%[J$,.>.{}}?>5)>|)[03PR(8*4-#[+}-;!9?|63}($*?%|51:^+;1;)$+15-:)!,;+#,}**@^|1!:7@<2:.4.!(87;}674]>" );

	BOOL bRet = FALSE;

	DWORD dwFileSize = 0;


	RawSMDHeader sRawHeader;
	ZeroMemory( &sRawHeader, sizeof( RawSMDHeader ) );

	if ( HANDLE hFile = CreateFile( strFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); hFile != INVALID_HANDLE_VALUE )
	{
		bRet = TRUE;

		CEncryptXOR * pcEncrypter = new CEncryptXOR();

		STRINGCOPY( sRawHeader.szHeader, sModelVersion.GetStringVersion().c_str() );

		sRawHeader.iMaterialCount = pcModel->pcTextureManager ? pcModel->pcTextureManager->iTextureCount : 0;

		sRawHeader.iObjectCount = pcModel->uMeshCount;

		auto pMeshInfo = new RawSMDObjectInfo[sRawHeader.iObjectCount];
		ZeroMemory( pMeshInfo, sizeof( RawSMDObjectInfo ) * sRawHeader.iObjectCount );

		int iPointerToObject = sizeof( RawSMDHeader ) + (sizeof( RawSMDObjectInfo ) * sRawHeader.iObjectCount);

		sRawHeader.iFilePointerToMaterials = iPointerToObject;
		iPointerToObject += pcModel->pcTextureManager ? pcModel->pcTextureManager->GetSaveSize() : 0;

		sRawHeader.iFilePointerToFirstObject = iPointerToObject;

		sRawHeader.iFrameCount = pcModel->iFrameCount;
		CopyMemory( sRawHeader.aFrame, pcModel->saFrame, sizeof( RawSMDFrame ) * sRawHeader.iFrameCount );

		for ( auto i = 0; i < pcModel->uMeshCount; i++ )
		{
			STRINGCOPY( pMeshInfo[i].szObjectName, pcModel->paMesh[i]->szName );
			pMeshInfo[i].iLength = pcModel->paMesh[i]->GetSaveSize( (sModelVersion.eVersion == PTModelVersion::EPTModelVersion::PTMODELVERSION_0062) || (sModelVersion.eVersion == PTModelVersion::EPTModelVersion::PTMODELVERSION_0064) );
			pMeshInfo[i].iFilePointerToObject = iPointerToObject;

			iPointerToObject += pMeshInfo[i].iLength;
		}

		WriteFile( hFile, &sRawHeader, sizeof( RawSMDHeader ), &dwFileSize, NULL );

		WriteFile( hFile, pMeshInfo, sizeof( RawSMDObjectInfo ) * sRawHeader.iObjectCount, &dwFileSize, NULL );

		if ( pcModel->pcTextureManager )
			pcModel->pcTextureManager->SaveFile( hFile );

		for ( auto i = 0; i < pcModel->uMeshCount; i++ )
		{
			if ( SaveMeshOld( pcModel->paMesh[i], hFile, sModelVersion ) == FALSE )
			{
				bRet = FALSE;
				break;
			}
		}

		DELETA( pMeshInfo );
		DELET( pcEncrypter );

		CloseHandle( hFile );
	}

	return bRet;
}

BOOL LoadMeshOld( EXEMesh * pcMesh, HANDLE hFile, PTModelVersion sModelVersion, EXEModel * pcBoneContainer, bool bIsBone )
{
	CEncryptXOR * pcEncrypter = new CEncryptXOR();

	DWORD dwFileSize = 0;
	int iLength = ReadFile( hFile, pcMesh, 0x8BC, &dwFileSize, NULL );
	if ( sModelVersion.HasInfluences() )
	{
		CopyMemory( pcMesh->sTransformationMatrixF.f, pcMesh->sTm.i, sizeof( float ) * 16 );

		for ( int i = 0; i < 4; i++ )
			for ( int j = 0; j < 4; j++ )
				pcMesh->sTm.i[i][j] = (int)(pcMesh->sTransformationMatrixF.m[i][j] * 256.0f);
	}

	auto psTextureLinkOld = pcMesh->pMeshFacePosition;

	if ( sModelVersion.IsEncrypted() )
	{
		//Face, Vertex, UV, Position, Rotation, PreviousRotation, Scale
		pcMesh->pMeshFace = new EXEFace[pcMesh->iFaceCount];
		iLength += ReadFile( hFile, pcMesh->pMeshFace, sizeof( EXEFace ) * pcMesh->iFaceCount, &dwFileSize, NULL );

		pcMesh->pMeshVertex = new EXEVertex[pcMesh->iVertexCount];

        if ( sModelVersion.HasInfluences() )
		{
			if ( pcMesh->iVertexCount > 0 )
				pcMesh->vFloatVertices.resize( pcMesh->iVertexCount );
			
			iLength += ReadFile( hFile, pcMesh->vFloatVertices.data(), sizeof( EXEVertexTangentF ) * pcMesh->iVertexCount, &dwFileSize, NULL );
        }
		else
            iLength += ReadFile( hFile, pcMesh->pMeshVertex, sizeof( EXEVertex ) * pcMesh->iVertexCount, &dwFileSize, NULL );

		for ( int i = 0; i < pcMesh->iVertexCount; i++ )
		{
			int aux = pcMesh->pMeshVertex[i].sPosition.iX;
			pcMesh->pMeshVertex[i].sPosition.iX = pcMesh->pMeshVertex[i].sPosition.iZ;
			pcMesh->pMeshVertex[i].sPosition.iZ = aux;

			pcMesh->pMeshVertex[i].sPosition.iY -= 0x6A6B;
		}

		pcMesh->pMeshFacePosition = new EXEFacePosition[pcMesh->iTextureLinkCount];
		iLength += ReadFile( hFile, pcMesh->pMeshFacePosition, sizeof( EXEFacePosition ) * pcMesh->iTextureLinkCount, &dwFileSize, NULL );

		pcMesh->sPos = new EXEAnimationPosition[pcMesh->iPosCount];
		iLength += ReadFile( hFile, pcMesh->sPos, sizeof( EXEAnimationPosition ) * pcMesh->iPosCount, &dwFileSize, NULL );

		pcMesh->sRot = new EXEAnimationRotation[pcMesh->iRotateCount];
		iLength += ReadFile( hFile, pcMesh->sRot, sizeof( EXEAnimationRotation ) * pcMesh->iRotateCount, &dwFileSize, NULL );

		pcMesh->sPreviousRotate = new EXEMatrixF[pcMesh->iRotateCount];
		iLength += ReadFile( hFile, pcMesh->sPreviousRotate, sizeof( EXEMatrixF ) * pcMesh->iRotateCount, &dwFileSize, NULL );

		pcMesh->sScale = new EXEAnimationScale[pcMesh->iScaleCount];
		iLength += ReadFile( hFile, pcMesh->sScale, sizeof( EXEAnimationScale ) * pcMesh->iScaleCount, &dwFileSize, NULL );
	}
	else
	{
		pcMesh->pMeshVertex = new EXEVertex[pcMesh->iVertexCount];

		if ( sModelVersion.HasInfluences() )
		{
			if ( pcMesh->iVertexCount > 0 )
				pcMesh->vFloatVertices.resize( pcMesh->iVertexCount );

			iLength += ReadFile( hFile, pcMesh->vFloatVertices.data(), sizeof( EXEVertexF ) * pcMesh->iVertexCount, &dwFileSize, NULL );
		}
		else
            iLength += ReadFile( hFile, pcMesh->pMeshVertex, sizeof( EXEVertex ) * pcMesh->iVertexCount, &dwFileSize, NULL );

		pcMesh->pMeshFace = new EXEFace[pcMesh->iFaceCount];
		iLength += ReadFile( hFile, pcMesh->pMeshFace, sizeof( EXEFace ) * pcMesh->iFaceCount, &dwFileSize, NULL );

		pcMesh->pMeshFacePosition = new EXEFacePosition[pcMesh->iTextureLinkCount];
		iLength += ReadFile( hFile, pcMesh->pMeshFacePosition, sizeof( EXEFacePosition ) * pcMesh->iTextureLinkCount, &dwFileSize, NULL );

		pcMesh->sRot = new EXEAnimationRotation[pcMesh->iRotateCount];
		iLength += ReadFile( hFile, pcMesh->sRot, sizeof( EXEAnimationRotation ) * pcMesh->iRotateCount, &dwFileSize, NULL );

		pcMesh->sPos = new EXEAnimationPosition[pcMesh->iPosCount];
		iLength += ReadFile( hFile, pcMesh->sPos, sizeof( EXEAnimationPosition ) * pcMesh->iPosCount, &dwFileSize, NULL );

		pcMesh->sScale = new EXEAnimationScale[pcMesh->iScaleCount];
		iLength += ReadFile( hFile, pcMesh->sScale, sizeof( EXEAnimationScale ) * pcMesh->iScaleCount, &dwFileSize, NULL );

		pcMesh->sPreviousRotate = new EXEMatrixF[pcMesh->iRotateCount];
		iLength += ReadFile( hFile, pcMesh->sPreviousRotate, sizeof( EXEMatrixF ) * pcMesh->iRotateCount, &dwFileSize, NULL );
	}

	int iAdditionalTextureLinkPointer = pcMesh->pMeshFacePosition - psTextureLinkOld;

	//Fix Texture
	for ( int i = 0; i < pcMesh->iTextureLinkCount; i++ )
	{
		if ( pcMesh->pMeshFacePosition[i].pcNextTexture )
		{
			iAdditionalTextureLinkPointer = (EXEFacePosition *)pcMesh->pMeshFacePosition[i].pcNextTexture - psTextureLinkOld;

			pcMesh->pMeshFacePosition[i].pcNextTexture = (DWORD *)(pcMesh->pMeshFacePosition + iAdditionalTextureLinkPointer);
		}
	}

	//Fix Texture Root
	for ( int i = 0; i < pcMesh->iFaceCount; i++ )
	{
		if ( pcMesh->pMeshFace[i].pcTextureLink )
		{
			iAdditionalTextureLinkPointer = (EXEFacePosition *)pcMesh->pMeshFace[i].pcTextureLink - psTextureLinkOld;

			pcMesh->pMeshFace[i].pcTextureLink = (EXEFacePosition *)(pcMesh->pMeshFacePosition + iAdditionalTextureLinkPointer);
		}
	}

	if ( (pcMesh->pVertexBoneList && pcBoneContainer) || (CANUSE_FAKEBONE && !bIsBone) )
	{
		pcMesh->pVertexBoneList = new EXEMesh * [pcMesh->iVertexCount];

		auto szaBoneNames = new char[pcMesh->iVertexCount * 32];
		{
			iLength += ReadFile( hFile, szaBoneNames, pcMesh->iVertexCount * 32, &dwFileSize, NULL );


			for ( int i = 0; i < pcMesh->iVertexCount; i++ )
			{
				if ( pcBoneContainer )
					pcMesh->pVertexBoneList[i] = pcBoneContainer->GetMesh( szaBoneNames + (i * 32) );
				else if ( CANUSE_FAKEBONE )
				{
					pcMesh->pVertexBoneList[i] = new EXEMesh();
					STRINGCOPY( pcMesh->pVertexBoneList[i]->szName, szaBoneNames + (i * 32) );
				}
			}

			if ( pcBoneContainer == nullptr )
			{
				FAKE_BONE_LOADER = true;

				if ( pcMesh->pVertexBoneList[0] )
				{
					if ( (pcMesh->pVertexBoneList[0]->szName[0] != 'B') && (pcMesh->pVertexBoneList[0]->szName[1] == 'i') && (pcMesh->pVertexBoneList[2]->szName[2] == 'p') )
					{
						for ( int i = 0; i < pcMesh->iVertexCount; i++ )
							DELET( pcMesh->pVertexBoneList[i] );

						DELETA( pcMesh->pVertexBoneList );

						FAKE_BONE_LOADER = false;
					}
				}
			}
		}

        if ( sModelVersion.HasInfluences() )
        {
            pcMesh->bUseInfluences = TRUE;
            pcMesh->vInfluences.resize( pcMesh->iVertexCount );
            iLength += ReadFile( hFile, pcMesh->vInfluences.data(), pcMesh->iVertexCount * sizeof( EXEMeshInfluenceVertex ), &dwFileSize, NULL );
        }
	}
	else if ( bIsBone )
		pcMesh->pVertexBoneList = nullptr;

	DELET( pcEncrypter );
	pcMesh->pcNewMesh = nullptr;

	return iLength > 0;
}

BOOL SaveMeshOld( EXEMesh * pcMesh, HANDLE hFile, PTModelVersion sModelVersion )
{
	int iLength = 0;

	DWORD dwFileSize = 0;

	pcMesh->dwHeader = 0x41424344 | 0x80000000;

	CEncryptXOR * pcEncrypter = new CEncryptXOR();

	if ( sModelVersion.IsEncrypted() )
	{
		if ( sModelVersion.HasInfluences() )
			CopyMemory( pcMesh->sTm.i, pcMesh->sTransformationMatrixF.f, sizeof( float ) * 16 );

		//Same Hash Loader
		iLength += WriteFile( hFile, pcMesh, 0x8BC, &dwFileSize, NULL );

		//Face, Vertex, UV, Position, Rotation, PreviousRotation, Scale
		iLength += WriteFile( hFile, pcMesh->pMeshFace, sizeof( EXEFace ) * pcMesh->iFaceCount, &dwFileSize, NULL );

		for ( int i = 0; i < pcMesh->iVertexCount; i++ )
		{
			int aux = pcMesh->pMeshVertex[i].sPosition.iX;
			pcMesh->pMeshVertex[i].sPosition.iX = pcMesh->pMeshVertex[i].sPosition.iZ;
			pcMesh->pMeshVertex[i].sPosition.iZ = aux;

			pcMesh->pMeshVertex[i].sPosition.iY += 0x6A6B;
		}
		
		if ( sModelVersion.HasInfluences() )
		{
			iLength += WriteFile( hFile, pcMesh->vFloatVertices.data(), sizeof( EXEVertexTangentF ) * pcMesh->iVertexCount, &dwFileSize, NULL );

			pcMesh->vFloatVertices.clear();
		}
		else
			iLength += WriteFile( hFile, pcMesh->pMeshVertex, sizeof( EXEVertex ) * pcMesh->iVertexCount, &dwFileSize, NULL );
		
		for ( int i = 0; i < pcMesh->iVertexCount; i++ )
		{
			int aux = pcMesh->pMeshVertex[i].sPosition.iX;
			pcMesh->pMeshVertex[i].sPosition.iX = pcMesh->pMeshVertex[i].sPosition.iZ;
			pcMesh->pMeshVertex[i].sPosition.iZ = aux;

			pcMesh->pMeshVertex[i].sPosition.iY -= 0x6A6B;
		}

		iLength += WriteFile( hFile, pcMesh->pMeshFacePosition, sizeof( EXEFacePosition ) * pcMesh->iTextureLinkCount, &dwFileSize, NULL );

		iLength += WriteFile( hFile, pcMesh->sPos, sizeof( EXEAnimationPosition ) * pcMesh->iPosCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->sRot, sizeof( EXEAnimationRotation ) * pcMesh->iRotateCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->sPreviousRotate, sizeof( EXEMatrixF ) * pcMesh->iRotateCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->sScale, sizeof( EXEAnimationScale ) * pcMesh->iScaleCount, &dwFileSize, NULL );
	}
	else
	{
		iLength += WriteFile( hFile, pcMesh, 0x8BC, &dwFileSize, NULL );
		
        if ( sModelVersion.HasInfluences() )
            iLength += WriteFile( hFile, pcMesh->vFloatVertices.data(), sizeof( EXEVertexF ) * pcMesh->iVertexCount, &dwFileSize, NULL );
        else
            iLength += WriteFile( hFile, pcMesh->pMeshVertex, sizeof( EXEVertex ) * pcMesh->iVertexCount, &dwFileSize, NULL );

		iLength += WriteFile( hFile, pcMesh->pMeshFace, sizeof( EXEFace ) * pcMesh->iFaceCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->pMeshFacePosition, sizeof( EXEFacePosition ) * pcMesh->iTextureLinkCount, &dwFileSize, NULL );

		iLength += WriteFile( hFile, pcMesh->sRot, sizeof( EXEAnimationRotation ) * pcMesh->iRotateCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->sPos, sizeof( EXEAnimationPosition ) * pcMesh->iPosCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->sScale, sizeof( EXEAnimationScale ) * pcMesh->iScaleCount, &dwFileSize, NULL );
		iLength += WriteFile( hFile, pcMesh->sPreviousRotate, sizeof( EXEMatrixF ) * pcMesh->iRotateCount, &dwFileSize, NULL );
	}


	if ( pcMesh->pVertexBoneList && pcMesh->pVertexBoneList[0] )
	{
		for ( int i = 0; i < pcMesh->iVertexCount; i++ )
		{
			char szName[32] = {0};
			STRINGCOPY( szName, pcMesh->pVertexBoneList[i]->szName );
			iLength += WriteFile( hFile, szName, 32, &dwFileSize, NULL );

			if ( FAKE_BONE_LOADER )
				DELET( pcMesh->pVertexBoneList[i] );
		}
	}

    if ( sModelVersion.HasInfluences() && pcMesh->bUseInfluences )
        iLength += WriteFile( hFile, pcMesh->vInfluences.data(), pcMesh->vInfluences.size() * sizeof( EXEMeshInfluenceVertex ), &dwFileSize, NULL );

	return iLength > 0;
}

void EncryptAll()
{
	/*
	auto IsInVector = []( std::string strFile, std::vector<std::string> & v ) -> bool
	{
		for ( auto & s : v )
			if ( s.compare( strFile ) == 0 )
				return true;

		return false;
	};

	auto LoadSMD = [IsInVector]( std::string strFileName, std::vector<std::string> & vExcludeSMDFiles, bool bBip = false ) -> bool
	{
		if ( IsInVector( strFileName, vExcludeSMDFiles ) == false )
		{
			WRITEERR( "%s", strFileName.c_str() );

			char szBoneFile[64] = {0};
//			STRINGCOPY( szBoneFile, strFileName.c_str() );

			//PathRenameExtensionA( szBoneFile, ".smb" );

			//EXEModel::SetBone( EXEModel::ReadBone( szBoneFile ) );

			EXEModel::SetBone( nullptr );

			bool bFound = false;
			if ( auto pcModel = bBip ? EXEModel::ReadBone( (char *)strFileName.c_str() ) : EXEModel::ReadModel( (char *)strFileName.c_str() ) )
			{
				SaveModelOld( pcModel, strFileName );
				vExcludeSMDFiles.push_back( strFileName );
				DELET( pcModel );
				bFound = true;
			}

			if ( EXEModel::GetBone() )
			{
				auto p = EXEModel::GetBone();
				EXEModel::SetBone( NULL );
				SaveModelOld( p, szBoneFile );
				vExcludeSMDFiles.push_back( szBoneFile );
				DELET( p );
			}
		}

		return true;
	};

	auto vFiles = SYSTEM->ListFilesInDirectory( "char\\", "inx" );

	std::vector<std::string> vExcludeSMDFiles;

	vFiles.clear();

	vFiles = SYSTEM->ListFilesInDirectory( "char\\", "smd" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "game\\", "smd" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "image\\", "smd" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "Effect\\", "smd" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "StartImage\\", "smd" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "char\\", "smb" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles, true );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "game\\", "smb" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles, true );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "image\\", "smb" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles, true );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "Effect\\", "smb" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles, true );

	vFiles.clear();
	vFiles = SYSTEM->ListFilesInDirectory( "StartImage\\", "smb" );
	for ( auto strFile : vFiles )
		LoadSMD( strFile, vExcludeSMDFiles, true );

	MessageBoxA( 0, AY_OBFUSCATE( "Done!" ), AY_OBFUSCATE( "Encrypt all" ), MB_OK | MB_ICONINFORMATION );

	*/
}

void ExpansionSkillRender( UnitData * pcUnitData )
{
	if ( (pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Pikeman) && (pcUnitData->bRendered) && (pcUnitData->sActiveSkill.cSkillTinyID == SKILLSHORTID_Expansion) &&
		 (pcUnitData->sRightHandTool.pModel) && (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) && (pcUnitData->sActiveSkill.cLevel > 0) && (pcUnitData->sActiveSkill.cLevel <= 10) )
	{
		int iSize = ((int*)0x00997800)[pcUnitData->sActiveSkill.cLevel - 1];

		Point3D sAngle = pcUnitData->sAngle;
		sAngle.iY = (-sAngle.iY + PTANGLE_180) & PTANGLE_Mask;

		pcUnitData->sPosi = pcUnitData->sPosition;

		pcUnitData->sRightHandTool.pModel->SetPosition( &pcUnitData->sPosi, &sAngle );
		pcUnitData->sRightHandTool.pModel->uFrame = -1;
		if ( pcUnitData->sRightHandTool.pModel->pcNewModel )
		{
			pcUnitData->sRightHandTool.pModel->pcNewModel->SetFrame( -1 );

			int s = pcUnitData->iFrameTimer << 3;
			int iMaxSize = (iSize * 256) / 100;
			if ( s > iMaxSize )
				s = iMaxSize;

			s += (256 / 4);
			auto sScaleOld = pcUnitData->sRightHandTool.pModel->pcNewModel->scaling;
			pcUnitData->sRightHandTool.pModel->pcNewModel->scaling = Math::Vector3( 1.0f + ((float)s / 256.0f) );
			
			pcUnitData->sRightHandTool.pModel->pcNewModel->Render();

			pcUnitData->sRightHandTool.pModel->pcNewModel->scaling = sScaleOld;
		}
	}
}

void ImportASEData( EXEMesh * pcMesh, const char * pszLineBuffer )
{
#define PT_MODELASEIMPORT_NEWBRACKET	(*(BOOL*)0x04B0E260)

	PT_MODELASEIMPORT_NEWBRACKET = FALSE;

    std::string strLine = trim( std::string( pszLineBuffer ) );
	if ( size_t iStrPos = strLine.find_first_of( ' ', 0 ); iStrPos != std::string::npos )
		strLine[iStrPos] = '\xFF';

	auto vCommands = split( strLine, '\xFF' );
	if ( vCommands.size() == 2 )
	{
		const std::string & strCommand = vCommands[0];
		auto vArguments = split( vCommands[1], '\t' );

		for( auto & v : vArguments )
			v.erase( std::remove( v.begin(), v.end(), '"' ), v.end() );

		//The Last Vertex ID
		static int iVertexID = -1;

		//Float Vertices
		if ( strCommand.compare( "*MESH_VERTEX" ) == 0 )
		{
			if ( vArguments.size() == 4 )
			{
				//XYZ
				EXEVertexTangentF	sVertex;
	
				sVertex.sPosition.x = (float)atof( vArguments[1].c_str() );
	
				sVertex.sPosition.y = (float)atof( vArguments[2].c_str() );
	
				sVertex.sPosition.z = (float)atof( vArguments[3].c_str() );

				sVertex.sPosition2 = X3D::Vector3( 0.0f, 0.0f, 0.0f );

			//	pcMesh->AddVertex( (int)(sVertex.sPosition.x * 256.0f), (int)(sVertex.sPosition.y * 256.0f), (int)(sVertex.sPosition.z * 256.0f) );

				pcMesh->vFloatVertices.push_back( sVertex );
			}
		}

		//Float Matrix
		else if ( strCommand.compare( "*TM_ROW0" ) == 0 )
		{
			if ( vArguments.size() == 3 )
			{
				pcMesh->sTransformationMatrixF.m[0][0] = (float)atof( vArguments[0].c_str() );

				pcMesh->sTransformationMatrixF.m[0][1] = (float)atof( vArguments[1].c_str() );

				pcMesh->sTransformationMatrixF.m[0][2] = (float)atof( vArguments[2].c_str() );
			}
		}
		if ( strCommand.compare( "*TM_ROW1" ) == 0 )
		{
			if ( vArguments.size() == 3 )
			{
				pcMesh->sTransformationMatrixF.m[1][0] = (float)atof( vArguments[0].c_str() );

				pcMesh->sTransformationMatrixF.m[1][1] = (float)atof( vArguments[1].c_str() );

				pcMesh->sTransformationMatrixF.m[1][2] = (float)atof( vArguments[2].c_str() );
			}
		}
		if ( strCommand.compare( "*TM_ROW2" ) == 0 )
		{
			if ( vArguments.size() == 3 )
			{
				pcMesh->sTransformationMatrixF.m[2][0] = (float)atof( vArguments[0].c_str() );

				pcMesh->sTransformationMatrixF.m[2][1] = (float)atof( vArguments[1].c_str() );

				pcMesh->sTransformationMatrixF.m[2][2] = (float)atof( vArguments[2].c_str() );
			}
		}
		if ( strCommand.compare( "*TM_ROW3" ) == 0 )
		{
			if ( vArguments.size() == 3 )
			{
				pcMesh->sTransformationMatrixF.m[3][0] = (float)atof( vArguments[0].c_str() );

				pcMesh->sTransformationMatrixF.m[3][1] = (float)atof( vArguments[1].c_str() );

				pcMesh->sTransformationMatrixF.m[3][2] = (float)atof( vArguments[2].c_str() );
			}
		}


		//tEST
		if ( strCommand.compare( "*TM_POS" ) == 0 )
		{
//			for ( int i = 0; i < 4; i++ )
//				for ( int j = 0; j < 4; j++ )
//					pcMesh->sTransformationMatrixF.m[i][j] = (float)pcMesh->sTm.i[i][j] / 256.0f;
		}
		else if ( strCommand.compare( "*PHYSIQUE_VERTEXASSIGNMENT_NONBLENDED_RIGIDTYPE" ) == 0 )
		{
			if ( vArguments.size() == 2 )
			{
				iVertexID = atoi( vArguments[0].c_str() );

				if ( EXEModel::GetBone() )
				{
					pcMesh->pVertexBoneList[iVertexID] = EXEModel::GetBone()->GetMesh( vArguments[1].c_str() );

					//First Time?
					if ( pcMesh->bUseInfluences == FALSE )
					{
						//Resize Influences
						pcMesh->vInfluences.resize( pcMesh->iVertexCount );

						pcMesh->bUseInfluences = TRUE;
					}

					EXEMeshInfluenceVertex sData;

					for ( int i = 0; i < MAX_PTMESH_INFLUENCES; i++ )
					{
						sData.saBoneIndex[i] = -1;
						sData.faWeight[i] = 0.0f;
					}

					pcMesh->vInfluences[iVertexID] = sData;

					pcMesh->vInfluences[iVertexID].saBoneIndex[0] = EXEModel::GetBone()->GetMeshIndex( vArguments[1].c_str() );
					pcMesh->vInfluences[iVertexID].faWeight[0] = 1.0f;
				}

			}
		}
		else if ( strCommand.compare( "*PHYSIQUE_VERTEXASSIGNMENT_BLENDED_RIGIDTYPE" ) == 0 )
		{
			if ( vArguments.size() == 1 )
			{
				//First Time?
				if ( pcMesh->bUseInfluences == FALSE )
				{
					//Resize Influences
					pcMesh->vInfluences.resize( pcMesh->iVertexCount );

					pcMesh->bUseInfluences = TRUE;
				}


				iVertexID = atoi( vArguments[0].c_str() );
			}

			PT_MODELASEIMPORT_NEWBRACKET = TRUE;
		}
		else if ( strCommand.compare( "*PHYSIQUE_VERTEXASSIGNMENT_NUMNODE" ) == 0 )
		{
			if ( vArguments.size() == 1 )
			{
				EXEMeshInfluenceVertex sData;

				int iInfluencesCount = atoi( vArguments[0].c_str() );

				for ( int i = 0; i < MAX_PTMESH_INFLUENCES; i++ )
				{
					sData.saBoneIndex[i] = -1;
					sData.faWeight[i] = 0.0f;
				}

				pcMesh->vInfluences[iVertexID] = sData;
			}
		}
		else if ( strCommand.compare( "*PHYSIQUE_VERTEXASSIGNMENT_NODELIST" ) == 0 )
		{
	
		}
		else if ( strCommand.compare( "*PHYSIQUE_VERTEXASSIGNMENT_NODE" ) == 0 )
		{
			if ( vArguments.size() == 3 )
			{
				int iInfluenceIndex = atoi( vArguments[0].c_str() );

				float fWeight = (float)atof( vArguments[1].c_str() );

				if ( EXEModel::GetBone() )
				{
					if ( iInfluenceIndex >= 0 && iInfluenceIndex < 4 )
					{
						if ( iInfluenceIndex == 0 )
							pcMesh->pVertexBoneList[iVertexID] = EXEModel::GetBone()->GetMesh( vArguments[2].c_str() );

						pcMesh->vInfluences[iVertexID].saBoneIndex[iInfluenceIndex] = EXEModel::GetBone()->GetMeshIndex( vArguments[2].c_str() );
						pcMesh->vInfluences[iVertexID].faWeight[iInfluenceIndex] = fWeight;
					}
				}
			}
		}
	}
}

void ImportASEDataStageMesh( Stage * pcStage, const char * pszLineBuffer )
{
	static size_t iVerticesWheel = 0;

#define PT_MODELASEIMPORT_NEWBRACKET	(*(BOOL*)0x04B0E260)

    PT_MODELASEIMPORT_NEWBRACKET = FALSE;

    std::string strLine = trim( std::string( pszLineBuffer ) );
    if ( size_t iStrPos = strLine.find_first_of( ' ', 0 ); iStrPos != std::string::npos )
        strLine[iStrPos] = '\xFF';

    auto vCommands = split( strLine, '\xFF' );
	if ( vCommands.size() == 2 )
	{
		const std::string & strCommand = vCommands[0];
		auto vArguments = split( vCommands[1], '\t' );

		for ( auto & v : vArguments )
			v.erase( std::remove( v.begin(), v.end(), '"' ), v.end() );

        if ( strCommand.compare( "*MESH_NUMVERTEX" ) == 0 )
        {
			if ( vArguments.size() == 1 )
			{
				size_t iCount = pcStage->vNormalVertices.size();
				pcStage->vNormalVertices.resize( iCount + atoi( vArguments[0].c_str() ) );

				iVerticesWheel = iCount;
			}
        }
 /*       else if ( strCommand.compare( "*MESH_VERTEXNORMAL" ) == 0 )
        {
			if ( vArguments.size() == 4 )
			{
				int iVertexID = atoi( vArguments[0].c_str() );

				pcStage->vNormalVertices[iVerticesWheel + iVertexID].faNormal[0] = (float)atof( vArguments[1].c_str() );
				pcStage->vNormalVertices[iVerticesWheel + iVertexID].faNormal[1] = (float)atof( vArguments[3].c_str() );
				pcStage->vNormalVertices[iVerticesWheel + iVertexID].faNormal[2] = (float)atof( vArguments[2].c_str() );
			}
        }
		*/
	}
}

};