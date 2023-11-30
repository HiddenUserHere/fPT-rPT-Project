#pragma once
class CMonsterHordeHandler
{
public:
	CMonsterHordeHandler();
	virtual ~CMonsterHordeHandler();

	void							Init();

	void							LoadHorde( int iHordeID = -1 );

	BOOL							HandleSpawnMonster( CharacterData * psCharacterData, Map * pcMap );

	void							Update();

	CHordeInstance					* GetInstance( int iID );

	bool							DisableInstance( int iID );

	void							UpdateStatus( BOOL bKillUnits );

private:

	bool							SQLLoadHorde( int iHordeID, CHordeInstance * pcInstance );

	std::vector<int>				SQLGetHordeList();

	void							KillUnits( int iMapID );

	std::vector<CHordeInstance*>	vInstances;

	WORD							wLastHour = 0xFFFF;
};

