#pragma once

#include "CLua.h"

#include "CParticleScript.h"
#include "CParticleEvent.h"
#include "CParticleModifierGroup.h"

class CParticleFactory : public CLua
{
	friend class CParticleDebug;

public:
								  CParticleFactory();
	virtual						 ~CParticleFactory();

	BOOL						  Init( class EXETextureManager * _pcTextureManager );
	void						  Shutdown();

	CParticleScript				* LoadScript( std::string strFilePath, int iScriptID = 0, BOOL bUseCache = TRUE );

	void						  ClearCache();

protected:
	void						  AddScriptToCache( CParticleScript * pcParticleScriptToCache );
	CParticleScript				* GetScriptFromCache( std::string strFilePath );

	static CParticleFactory		* GetThis( LuaState * pLuaState );

	//Integer Handlers
	static int					  LuaInt( LuaState * pLuaState );
	static int					  LuaRandomInt( LuaState * pLuaState );

	//Double Handlers
	static int					  LuaDbl( LuaState * pLuaState );
	static int					  LuaRandomDbl( LuaState * pLuaState );

	//Particle Script Handlers
	static int					  LuaScript( LuaState * pLuaState );
	static int					  LuaAuthor( LuaState * pLuaState );

	//Particle System Handlers
	static int					  LuaParticleSystem( LuaState * pLuaState );
	static int					  LuaParticleSystemEnd( LuaState * pLuaState );

	//Particle Object Handlers
	static int					  LuaParticleObject( LuaState * pLuaState );
	static int					  LuaParticleObjectEnd( LuaState * pLuaState );

	static int					  LuaAddObject( LuaState * pLuaState );

	static int					  LuaName( LuaState * pLuaState );

	//Emitter Handlers
	static int					  LuaEmitType( LuaState * pLuaState );

	static int					  LuaEmitPosition( LuaState * pLuaState );
	static int					  LuaEmitVelocity( LuaState * pLuaState );

	static int					  LuaEmitRotation( LuaState * pLuaState );
	static int					  LuaEmitAngularVelocity( LuaState * pLuaState );

	static int					  LuaEmitDelay( LuaState * pLuaState );
	static int					  LuaEmitLifeTime( LuaState * pLuaState );

	static int					  LuaEmitParticles( LuaState * pLuaState );
	static int					  LuaEmitInterval( LuaState * pLuaState );

	//Emitter Events Handlers


	//Particle Handlers
	static int					  LuaType( LuaState * pLuaState );

	static int					  LuaTexture( LuaState * pLuaState );
	static int					  LuaBlend( LuaState * pLuaState );

	static int					  LuaRGBA( LuaState * pLuaState );
	static int					  LuaSize( LuaState * pLuaState );
	static int					  LuaSizeWH( LuaState * pLuaState );

	static int					  LuaLoop( LuaState * pLuaState );
	static int					  LuaLifeTime( LuaState * pLuaState );

	//Particle Events Handlers
	static int					  LuaEventVelocity( LuaState * pLuaState );
	static int					  LuaEventSize( LuaState * pLuaState );

	//Particle Modifiers Handlers
	static int					  LuaModifierSize( LuaState * pLuaState );
	static int					  LuaModifierRGBA( LuaState * pLuaState );

protected:
	class EXETextureManager				* pcTextureManager;

	std::vector<CParticleScript*>	  vParticleScriptCache;

	CParticleScript				* pcParticleScript;

private:
	CMutex						* pcMutex;

};