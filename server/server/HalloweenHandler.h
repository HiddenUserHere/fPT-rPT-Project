#pragma once
class CHalloweenHandler
{
protected:
	DWORD												dwDropTime = 0;
	DWORD												dwUpdateCombinationTime = 0;
	DWORD												dwRuneEvil[3];
	DWORD												dwRuneInfernal[3];
	int													iCombinationID;
public:
	CHalloweenHandler();
	virtual ~CHalloweenHandler();


	void												Init();

	BOOL												OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket );

	void												OnSpawnEventMonster( CharacterData * psCharacterData );
	BOOL												OnUnitKilled( UnitData * pcUnitData );

	int													GetCombinationID() { return iCombinationID; };

	void												SetCombinationID( int iID );

	void												Tick();

	void												ClearTickItem() { dwDropTime = 0; }

};

