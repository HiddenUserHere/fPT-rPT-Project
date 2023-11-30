#pragma once
class CImpulsionHandler
{
public:
	CImpulsionHandler();
	virtual ~CImpulsionHandler();

	BOOL						OnEvent( UnitData * pcUnitData );

private:
	static void					SetImpulsionHitEffect( Point3D * psPosition );
};

