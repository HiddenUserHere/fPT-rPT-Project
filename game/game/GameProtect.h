#pragma once

struct RegenPointerData
{
	float					fHP;
	float					fSP;
	float					fMP;

	void operator=(const RegenPointerData & sData)
	{
		this->fHP = sData.fHP;
		this->fSP = sData.fSP;
		this->fMP = sData.fMP;
	}
};

class CGameProtect
{
private:
	const static DWORD			pfnValidateCaracterCheck	= 0x0045BC90;
	const static DWORD			ppDecreaseSP				= 0x0367E87C;

public:
	CGameProtect();
	~CGameProtect();


	static void					Update();

	static void					OnRegen();
	static void					OnSetRegen();

	static void					SetDecreaseMP( float fMP );
	static float				GetDecreaseMP();

	static void					SetDecreaseSP( float fSP );
	static float				GetDecreaseSP();

	static BOOL					ValidateStatusPoints( CharacterData & sCharacterData, UINT iQuestFlag );

	static BOOL					AddStatusLevelUP( int iLevelCount, UINT iQuestFlag );

	static BOOL					ValidateCharacterCheck();
	
private:
	static int					GetStatusPointQuest( int iLevel, UINT iQuestFlag );

	static RegenPointerData		saRegenData[5];

	static int					iRegenWheel;

	static float				fDecreaseMPData;
	static float				fDecreaseSPData;
};

