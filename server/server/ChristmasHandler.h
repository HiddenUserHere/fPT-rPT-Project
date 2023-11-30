#pragma once
class CChristmasHandler
{
private:
	BOOL												IsChristmasRune( DWORD dwCode );
public:
	CChristmasHandler();
	virtual ~CChristmasHandler();

	void												OnSpawnMonsterEvent( struct CharacterData * psCharacterData, Map * pcMap );

	BOOL												OnManufactureItem( User * pcUser, struct PacketManufactureItem * psPacket );

	void												OnSayTime( UnitData * pcUnitData );

};

