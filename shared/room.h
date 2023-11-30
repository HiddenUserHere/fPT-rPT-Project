#pragma once

struct UnitRoom
{
	int					iID;
	BOOL				bCanViewOther;

	bool				bInvalid;
};

class CRoomTeam
{
public:
	CRoomTeam( int iID );
	virtual ~CRoomTeam();

	void						Clear();

	int							GetID() { return iID; }

	std::vector<UnitRoom*>		& GetUnits() { return vUnits; }

	UnitRoom					* GetUnit( int iID );

	UnitRoom					* Add( int iID, BOOL bCanViewOther = TRUE );

	void						Remove( int iID );
	void						Invalidate( int iID );

private:
	int							iID;

	std::vector<UnitRoom*>		vUnits;
};

class CRoomAllies
{
public:
	CRoomAllies( int iID );
	virtual ~CRoomAllies();

	void						Clear();

	int							GetID() { return iID; }
	CRoomTeam					* Get( int iID );

	CRoomTeam					* Add( int iID );

	UnitRoom					* GetUnit( int iID );

	std::vector<CRoomTeam*>		& GetTeams() { return vTeams; }

private:
	int							iID;

	std::vector<CRoomTeam*>		vTeams;
};

class CRoomInstance
{
public:
	CRoomInstance( UINT lID, EMapID iMapID, BOOL bCanView, const char * pszName = "" );
	virtual ~CRoomInstance();

	void						Clear();

	CRoomAllies					* Get( UINT lID );
	CRoomAllies					* Add( UINT lID );

	UINT						GetID() { return lID; }
	EMapID						GetMapID() { return iMapID; }

	BOOL						IsInRoom( int iID );

	BOOL						CanView() { return bVisible; }

	std::vector<CRoomAllies*>	& GetAllies() { return vAllies; }

	UnitRoom					* GetUnit( int iID );

private:
	UINT						lID;

	EMapID						iMapID;

	BOOL						bVisible;

	char						szName[64];

	std::vector<CRoomAllies*>	vAllies;
};
