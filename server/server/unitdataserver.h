#pragma once


class UnitDataServer
{
private:
	static void									Init( Unit * pcUnit, UnitData * pcUnitData );
public:
	UnitData									* pcUnitData = NULL;
	DWORD										iID;

	Point3D										sOldPosition;
	DWORD										dwTimePositionUpdate;

	static UnitDataServer						* UnitDataToUnit( UnitData * pcUnitData );
};

