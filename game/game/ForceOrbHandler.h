#pragma once
class CForceOrbHandler
{
private:
	BOOL										bCharacterMain; // Is Cuurent player? = TRUE
	BOOL										bEnd; // ??

	int											iTimeCount; // Time of particle?
	int											iMaterial; // ID of Material 
	
	Point3D		sPosition;	
	int			iAngle;

	BOOL										bMainPosition; // When == TRUE or FALSE, change z of position of force

	BOOL										bActive;
	BOOL										bActive2;
	Point3D										sWeapon[10];
	Point3D										sWeapon2[10];
	int											iWidth;
	int											iHeight;
	int											iWPCount;
	int											iWPCount2;

public:
	
	void												Render();

	void												Start( class UnitData * pcUnitData, int iTime );

	void												Main(class UnitData * pcUnitData);

//	void												Draw( class UnitData * pcUnitData, Point3D * p, Point3D * pa, struct DX::Face2D * psFace );


};

