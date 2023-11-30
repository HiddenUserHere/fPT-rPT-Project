#pragma once

class MapGame
{
public:
	//Constructor
											MapGame();
	virtual								   ~MapGame();

	void									Load();

	void									SetMessageBoxMap( int iMap, BOOL bUsingCore = FALSE );

	void									UpdateBlockedPosition( UnitData * pcUnitData );

    static void                             TeleportMapPosition( EMapID eMapID, int iX, int iZ );
    void                                    TeleportMapPositionHandle( EMapID eMapID, int iX, int iZ );

	BaseMap									* GetBaseMapGame() { return pcaBaseMap; }

	BaseMap								  * pcaBaseMap;
};