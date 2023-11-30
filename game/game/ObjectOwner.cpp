#include "stdafx.h"
#include "ObjectOwner.h"

namespace Engine
{

//ABSTRACT	
ObjectOwner::ObjectOwner()
{
	eType		= OBJECTOWNERTYPE_None;

	sInitialRotation = Point3D( 0, 0, 0 );
}

ObjectOwner::ObjectOwner( ObjectOwner & c )
{
	eType		= c.eType;

	cPosition.x	= c.cPosition.x;
	cPosition.y	= c.cPosition.y;
	cPosition.z	= c.cPosition.z;
}

ObjectOwner::~ObjectOwner()
{
}


ObjectOwnerUnit::ObjectOwnerUnit( UnitData * _pcUnitData )
{
	eType = OBJECTOWNERTYPE_Unit;

	lID = _pcUnitData->iID;
	pcUnitData = _pcUnitData;

	bWeapon = FALSE;

	sInitialRotation = pcUnitData->sAngle;
}

ObjectOwnerUnit::ObjectOwnerUnit( ObjectOwnerUnit & c )
{
	eType	= c.eType;

	lID		= c.lID;
	pcUnitData	= c.pcUnitData;

	pcMesh = c.pcMesh;

	bWeapon = c.bWeapon;

	bHasTarget = c.bHasTarget;
	sTargetPosition = c.sTargetPosition;
	sInitialRotation = c.sInitialRotation;

	for( vector<EAnimationType>::iterator it = c.vAnimationTypeLostConditions.begin(); it != c.vAnimationTypeLostConditions.end(); it++ )
	{
		EAnimationType eAnimationType = *it;

		vAnimationTypeLostConditions.push_back( eAnimationType );
	}
}

ObjectOwnerUnit::~ObjectOwnerUnit()
{
	pcUnitData	= NULL;
}

void ObjectOwnerUnit::IsWeapon( BOOL b )
{
	bWeapon = b;
}

ObjectOwnerUnit * ObjectOwnerUnit::Clone()
{
	ObjectOwnerUnit * pClone = new ObjectOwnerUnit( *this );

	return pClone;
}

void ObjectOwnerUnit::Update()
{
	if ( lID == UNITDATA->iID )
		pcUnitData = UNITDATA;
	
	Point3D p = pcUnitData->sPosition;

	if ( bWeapon )
		SKILLMANAGERHANDLER->GetWeaponPositionNew( pcUnitData, p, 0.25f );

	if ( pcMesh )
	{
		EXEMatrixI sMatrix = pcMesh->GetWorldMatrix();

		Point3D sPositionNormal = Point3D( 1, 1, 1 );

		Point3D sPositionScale;
		sPositionScale.iX = sPositionNormal.iX * sMatrix.i[0][0] + sPositionNormal.iY * sMatrix.i[1][0] + sPositionNormal.iZ * sMatrix.i[2][0];
		sPositionScale.iY = sPositionNormal.iX * sMatrix.i[0][1] + sPositionNormal.iY * sMatrix.i[1][1] + sPositionNormal.iZ * sMatrix.i[2][1];
		sPositionScale.iZ = sPositionNormal.iX * sMatrix.i[0][2] + sPositionNormal.iY * sMatrix.i[1][2] + sPositionNormal.iZ * sMatrix.i[2][2];

		p.iX = (p.iX + (sPositionScale.iX >> 8) + sMatrix.i[3][0]);
		p.iZ = (p.iZ + (sPositionScale.iY >> 8) + sMatrix.i[3][1]);
		p.iY = (p.iY + (sPositionScale.iZ >> 8) + sMatrix.i[3][2]);
	}

	cPosition.x	= p.iX;
	cPosition.y	= p.iY;
	cPosition.z	= p.iZ;
}

BOOL ObjectOwnerUnit::Lost( DWORD dwTickcount )
{
	if ( lID == UNITDATA->iID )
		pcUnitData = UNITDATA;

	if( pcUnitData == NULL )
		return TRUE;

	if( pcUnitData->iID != lID )
		return TRUE;

	if( pcUnitData->bActive == FALSE )
		return TRUE;

	if( pcUnitData != UNITDATA && (pcUnitData->dwLastActiveTime + 10000) < dwTickcount )
		return TRUE;

	if( pcUnitData->psModelAnimation == NULL )
		return TRUE;
	
	for( vector<EAnimationType>::iterator it = vAnimationTypeLostConditions.begin(); it != vAnimationTypeLostConditions.end(); it++ )
	{
		EAnimationType eAnimationType = *it;

		if( pcUnitData->psModelAnimation->iType == eAnimationType )
			return TRUE;
	}

	return FALSE;
}

void ObjectOwnerUnit::SetBiped( const std::string strMeshName )
{
	if ( pcUnitData->pcBoneContainer )
		pcMesh = pcUnitData->pcBoneContainer->GetMesh( strMeshName.c_str() );
}

ObjectOwnerProjectile::ObjectOwnerProjectile( Unit * _pcUnit ) : ObjectOwner()
{
	eType = OBJECTOWNERTYPE_Projectile;

	lID = _pcUnit->iID;
	pcUnit = _pcUnit;
}

ObjectOwnerProjectile::ObjectOwnerProjectile( ObjectOwnerProjectile & c )
{
	eType = c.eType;

	lID = c.lID;
	pcUnit = c.pcUnit;

	bHasTarget = c.bHasTarget;
	sTargetPosition = c.sTargetPosition;
}

ObjectOwnerProjectile::~ObjectOwnerProjectile()
{
	pcUnit = NULL;
}

ObjectOwnerProjectile * ObjectOwnerProjectile::Clone()
{
	ObjectOwnerProjectile * pClone = new ObjectOwnerProjectile( *this );

	return pClone;
}

void ObjectOwnerProjectile::Update()
{
	Point3D & p = pcUnit->GetPosition();

	cPosition.x = p.iX;
	cPosition.y = p.iY;
	cPosition.z = p.iZ;
}

BOOL ObjectOwnerProjectile::Lost( DWORD dwTickcount )
{
	if( pcUnit == NULL )
		return TRUE;

	if( pcUnit->GetID() != lID )
		return TRUE;

	if( pcUnit->pcUnitData->bActive == FALSE )
		return TRUE;

	if( pcUnit->pcUnitData->psModelAnimation == NULL )
		return TRUE;

	return FALSE;
}

ObjectOwnerPosition::ObjectOwnerPosition( Point3D s )
{
	cPosition.x = s.iX; cPosition.y = s.iY; cPosition.z = s.iZ; eType = OBJECTOWNERTYPE_None;
}

ObjectOwnerPosition::ObjectOwnerPosition( ObjectOwnerPosition & c )
{
	eType		= c.eType;
	cPosition.x = c.cPosition.x;
	cPosition.y = c.cPosition.y;
	cPosition.z = c.cPosition.z;

	bHasTarget = c.bHasTarget;
	sTargetPosition = c.sTargetPosition;
}

ObjectOwnerPosition::~ObjectOwnerPosition()
{
}

ObjectOwnerPosition * ObjectOwnerPosition::Clone()
{
	ObjectOwnerPosition * pClone = new ObjectOwnerPosition( *this );

	return pClone;
}

void ObjectOwnerPosition::Update()
{
}

BOOL ObjectOwnerPosition::Lost( DWORD dwTickcount )
{
	return FALSE;
}

}