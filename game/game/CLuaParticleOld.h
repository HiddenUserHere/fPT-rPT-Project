#pragma once

class CEffectGroupLua
{
public:
	char						cPadding001[0x2C];
};

class CEffectModelControllerLua
{
public:
	char						cPadding001[0x50];
};

class CEffectMainBaseLua
{
private:
	char						cPaddingDeconstructor[0x04];
	bool						bActive;
	int							iID;
	float						fEndTime;
	UnitData					* pcUnitData;

	void						Constructor();
public:
	CEffectMainBaseLua() { Constructor(); };

	bool						IsActive() { return bActive; }

	int							GetID() { return iID; }
	void						SetID( int _iID ) { iID = _iID; }

	int							GetUnitID() { return (pcUnitData ? pcUnitData->iID : -1); }

	void						Kill() { bActive = false; }

	void						Init( CEffectGroupLua * pcGroup, int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ );
};

class CEffectMainBaseLuaMagneticSphere : CEffectMainBaseLua
{
public:
	UnitData					* pcUnitData;
	bool						bSelfUnitData;
	PTModel					* pcModel;
	PTMesh						* pcaMesh[3];

	int							iaParticleID[3];

	float						fTime;
	float						fEndTime;

	float						fCurrentFrame;
	float						fDamageTime;
	float						fDamageTimeCount;
};

class CEffectUnitFollowLua : public CEffectMainBaseLua
{
private:
	bool						bIsMyCharacter;
	CEffectGroupLua				* pcGroup;
	float						fHeight;
	float						fTime;

	int							iParticleID;
	bool						bEndParticle;

	void						Constructor();
public:
	CEffectUnitFollowLua()		{ Constructor(); };

	void						Init( UnitData * pcUnitData, CEffectGroupLua * pcGroup, float fTime, float fHeight = 0.0f );

	void						SetParticleID( int i ) { iParticleID = i; };
};

class CLuaParticleOld
{
private:
	CEffectGroupLua				* pcGroup;
	CEffectModelControllerLua	* pcModel;
	int							iType;
	int							iSpecificType;
	char						cPadding001[4];

public:
	CLuaParticleOld();
	~CLuaParticleOld();

	static CLuaParticleOld		* GetInstance() { return (CLuaParticleOld*)0x03916BB0; }

	CEffectGroupLua				* Load( const char * pszFileName );
};

class CEffectManagerHandlerLua
{
private:
#ifndef _DEBUG
	char						cPadding001[0x08];
#endif

	std::vector<CEffectMainBaseLua *> vPostEffects;
	std::vector<CEffectMainBaseLua *> vEffects;
	std::vector<CEffectMainBaseLua *> vKeepEffects;

public:
	static CEffectManagerHandlerLua * GetInstance();

	void						AddEffect( CEffectMainBaseLua * pcEffect );

	bool						UpdateEffectTime( int iID, UnitData * pcUnitData, float fTime );

	void						KillEffectInside( int iID, int iUnitDataID );
	static void					KillEffect( int iID, int iUnitDataID );

	void						Clear();
};

