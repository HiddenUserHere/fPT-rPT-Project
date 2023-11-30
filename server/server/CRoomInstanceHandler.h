#pragma once

class CRoomInstanceHandler
{
public:
	CRoomInstanceHandler();
	virtual ~CRoomInstanceHandler();

	void				Init();

	void				Shutdown();

	void				Update();

	void				Test( User * pcUser );

	CRoomInstance		* Add( EMapID iMapID, BOOL bCanViewOtherRoom = TRUE, const char * pszName = "" );

	void				RemoveUnit( UINT lID );
	void				Delete( UINT lID );
	void				Delete( CRoomInstance * pcRoom );

	std::vector<CRoomInstance*> GetRooms( EMapID iMapID = MAPID_Invalid );

	CRoomInstance		* GetRoomUnitID( int iID );
	CRoomTeam			* GetTeamUnitID( int iID );
	CRoomInstance		* GetRoom( int iID );

	BOOL				SameRoom( int iUnitOneID, int iUnitTwoID );

	BOOL				CanSee( int iUnitOneID, int iUnitTwoID );

	std::vector<User*>	GetUsersRoom( UINT lID );



private:
	UINT				uRoomWheelID;

	std::vector<CRoomInstance*>	vRooms;
};

