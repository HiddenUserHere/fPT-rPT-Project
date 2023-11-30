#pragma once

class CKnockupUnit
{
public:
	CKnockupUnit( Unit * pcUnit, float fTimeUp, float fTimeDown, float fTimeInAir, float fMaxY );
	~CKnockupUnit();

	void						AddAnimationLostCondition( EAnimationType eAnimationType );

	int							GetID() { return iID; }
	Unit						* GetUnit() { return pcUnit; }
	float						GetDuration() { return fTimeUp + fTimeDown + fTimeInAir; }
	float						GetTimeUp() { return fTimeUp; }
	float						GetTimeDown() { return fTimeDown; }
	float						GetTimeInAir() { return fTimeInAir; }
	float						GetMaxY() { return fMaxY; }
	float						GetTime() { return fTime; }
	int							GetBeginY() { return iBeginY; }

	void						Update( float fTime );

	BOOL						Lost( DWORD dwTickCount );

private:
	int							iID;
	Unit						* pcUnit;
	float						fTimeUp;
	float						fTimeDown;
	float						fTimeInAir;
	float						fMaxY;
	std::vector<EAnimationType>	vAnimationTypeLostConditions;

	float						fTime;

	int							iBeginY;

	int							iCurrentY;
};


class CKnockupUnitHandler
{
public:
	CKnockupUnitHandler();
	virtual ~CKnockupUnitHandler();

	void						Init();
	void						Shutdown();

	void						Update( float fTime );

	void						AddUnit( CKnockupUnit * pcKnockupUnit );

	void						RemoveUnit( Unit * pcUnit );
	void						RemoveUnit( int iID );

	BOOL						CanChangeUnitY( Unit * pcUnit );

private:
	std::vector<CKnockupUnit *>	vKnockupUnits;

	BOOL						Lost( CKnockupUnit * pcKnockupUnit );

	void						UpdateUnitOver();
};

