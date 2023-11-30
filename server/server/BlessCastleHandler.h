#pragma once
class CBlessCastleHandler
{
protected:
	int													iCountTower = 0;
	BOOL												bCastleDoor = TRUE;
	BOOL												bTowers = 0;
	BOOL												bInit = FALSE;
	BOOL												bPhase3 = FALSE;
	BOOL												bPhase4 = FALSE;
	UnitData											* pcUnitDataDoor = NULL;
	int													iPhaseID	= 0;
	int													iOldPhaseID = 0;
	DWORD												dwTickUpdate = 0;
	WORD												sLastDay = 0;

public:
	CBlessCastleHandler();
	virtual ~CBlessCastleHandler();

	void												Init();

	int													UpdateTower();
	void												UpdatePhase( int iPhase );

	void												UpdateSiegeWar();
};

