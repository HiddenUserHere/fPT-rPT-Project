#pragma once

#include "CPoint.h"
//#include "unit.h"

namespace Engine
{

enum EObjectOwnerType
{
	OBJECTOWNERTYPE_None,
	OBJECTOWNERTYPE_Unit,
	OBJECTOWNERTYPE_Projectile,
};

class ObjectOwner
{
public:
						  ObjectOwner();
						  ObjectOwner( ObjectOwner & c );
						  ObjectOwner( EObjectOwnerType e, Point3D s ) { SetPosition( s ); SetType( e ); };
						  ~ObjectOwner();

	virtual ObjectOwner	* Clone() = 0;

	virtual void		  Update() = 0;

	virtual BOOL		  Lost( DWORD dwTickcount ) { return FALSE; }
	
	EObjectOwnerType	  GetType() { return eType; }
	CPoint<int>			& GetPosition() { return cPosition; }

	void				  SetType( EObjectOwnerType e ) { eType = e; };
	void				  SetPosition( Point3D s ) { cPosition.x = s.iX; cPosition.y = s.iY; cPosition.z = s.iZ; };
	void				  SetTargetPosition( Point3D _sPosition ) { bHasTarget = true; sTargetPosition = _sPosition; }
	void				  SetRotation( Point3D _s ) { sInitialRotation = _s; }

	Point3D				  GetRotation() { return sInitialRotation; }

	bool				  HasTarget() { return bHasTarget; }
	Point3D				  GetTargetPosition() { return sTargetPosition; }

protected:
	EObjectOwnerType	  eType;
	CPoint<int>			  cPosition;

	bool				  bHasTarget = false;
	Point3D				  sTargetPosition;

	Point3D				  sInitialRotation;
};

class ObjectOwnerUnit : public ObjectOwner
{
public:
	ObjectOwnerUnit( UnitData * _pcUnitData );
	ObjectOwnerUnit( ObjectOwnerUnit & c );
	~ObjectOwnerUnit();

	void					  IsWeapon( BOOL b );

	virtual ObjectOwnerUnit * Clone();

	virtual void			  Update();

	virtual BOOL			  Lost( DWORD dwTickcount );

	void					  AddAnimationTypeLostCondition( EAnimationType eAnimationType ) { vAnimationTypeLostConditions.push_back( eAnimationType ); }

	ID						  GetUnitID() { return pcUnitData->iID; }

	void					  SetBiped( const std::string strMeshName );

protected:
	ID						  lID;
	UnitData				* pcUnitData;

	EXEMesh					* pcMesh = nullptr;

	BOOL					bWeapon;

	std::vector<EAnimationType>	  vAnimationTypeLostConditions;

};

class ObjectOwnerPosition : public ObjectOwner
{
public:
	ObjectOwnerPosition( Point3D s );
	ObjectOwnerPosition( ObjectOwnerPosition & c );
	~ObjectOwnerPosition();

	virtual ObjectOwnerPosition	* Clone();

	virtual void			  Update();

	virtual BOOL			Lost( DWORD dwTickcount );


protected:


};

class ObjectOwnerProjectile : public ObjectOwner
{
public:
									  ObjectOwnerProjectile( Unit * _pcUnit );
									  ObjectOwnerProjectile( ObjectOwnerProjectile & c );
									 ~ObjectOwnerProjectile();

	virtual ObjectOwnerProjectile	* Clone();

	virtual void					  Update();

	virtual BOOL					  Lost( DWORD dwTickcount );

	ID								  GetUnitID() { return lID; }

protected:
	ID								  lID;
	Unit							* pcUnit;

};

}