#include "stdafx.h"
#include "CParticleFactory.h"

//#define LUAPT_DEBUG

#ifdef LUAPT_DEBUG
char szLuaFileDebug[8192];
#endif

CParticleFactory::CParticleFactory() : CLua()
{
	pcMutex				= new CMutex( "Particle Factory" );

	pcTextureManager	= NULL;

	pcParticleScript	= NULL;
}

CParticleFactory::~CParticleFactory()
{
	//WRITEDBG( "~CParticleFactory()" );

	for( vector<CParticleScript*>::iterator it = vParticleScriptCache.begin(); it != vParticleScriptCache.end(); it++ )
	{
		CParticleScript * pcCachedParticleScript = *it;

		DELET( pcCachedParticleScript );
	}
	vParticleScriptCache.clear();

	pcParticleScript	= NULL;

	pcTextureManager	= NULL;

	DELET( pcMutex );
}

BOOL CParticleFactory::Init( EXETextureManager * _pcTextureManager )
{
	pcTextureManager	= _pcTextureManager;

	//Integer Handlers
	Register( "Int", LuaInt );
	Register( "RandomInt", LuaRandomInt );

	//Double Handlers
	Register( "Dbl", LuaDbl );
	Register( "RandomDbl", LuaRandomDbl );

	//Particle Script Handlers
	Register( "Script", LuaScript );
	Register( "Author", LuaAuthor );

	//Particle System Handlers
	Register( "ParticleObject", LuaParticleObject );
	Register( "ParticleObjectEnd", LuaParticleObjectEnd );

	Register( "ParticleSystem", LuaParticleSystem );
	Register( "ParticleSystemEnd", LuaParticleSystemEnd );

	Register( "Name", LuaName );

	//Emitter Handlers
	Register( "EmitType", LuaEmitType );

	Register( "Position", LuaEmitPosition );
	Register( "Velocity", LuaEmitVelocity );

	Register( "EmitRotation", LuaEmitRotation );
	Register( "EmitAngularVelocity", LuaEmitAngularVelocity );

	Register( "EmitDelay", LuaEmitDelay );
	Register( "EmitLifeTime", LuaEmitLifeTime );

	Register( "EmitParticles", LuaEmitParticles );
	Register( "EmitInterval", LuaEmitInterval );

	//Emitter Events Handlers

	//Particle Handlers
	Register( "Type", LuaType );

	Register( "Texture", LuaTexture );
	Register( "Blend", LuaBlend );
	
	Register( "RGBA", LuaRGBA );
	Register( "Size", LuaSize );
	Register( "SizeWH", LuaSizeWH );

	Register( "Loop", LuaLoop );
	Register( "LifeTime", LuaLifeTime );

	//Particle Events Handlers
	Register( "EventVelocity", LuaEventVelocity );
	Register( "EventSize", LuaEventSize );

	//Particle Modifiers Handlers
	Register( "ModifierSize", LuaModifierSize );
	Register( "ModifierRGBA", LuaModifierRGBA );

	return TRUE;
}

void CParticleFactory::Shutdown()
{
}

CParticleScript * CParticleFactory::LoadScript( string strFilePath, int iScriptID, BOOL bUseCache )
{
	pcMutex->Lock();

	CParticleScript * pcRet = NULL;

	if( bUseCache )
		pcParticleScript = GetScriptFromCache( strFilePath );
	else
		pcParticleScript = NULL;

	if( pcParticleScript == NULL )
	{
		pcParticleScript = new CParticleScript( strFilePath );

		DoFile( strFilePath, true );

		AddScriptToCache( pcParticleScript );
	}

	pcRet = new CParticleScript( *pcParticleScript );


#ifdef LUAPT_DEBUG
	FILE * fp = NULL;
	std::string str = FormatString( "%sD", strFilePath.c_str() );
	fopen_s( &fp, str.c_str(), "wb" );
	if ( fp )
	{
		fwrite( szLuaFileDebug, STRLEN( szLuaFileDebug ), 1, fp );
		fclose( fp );
	}

	ZeroMemory( szLuaFileDebug, _countof( szLuaFileDebug ) );
#endif

	pcMutex->Unlock();

	return pcRet;
}

void CParticleFactory::ClearCache()
{
	for( vector<CParticleScript*>::iterator it = vParticleScriptCache.begin(); it != vParticleScriptCache.end(); it++ )
	{
		CParticleScript * pcCachedParticleScript = *it;

		DELET( pcCachedParticleScript );
	}
	vParticleScriptCache.clear();
}

void CParticleFactory::AddScriptToCache( CParticleScript * pcParticleScriptToCache )
{
	vParticleScriptCache.push_back( pcParticleScriptToCache );
}

CParticleScript * CParticleFactory::GetScriptFromCache( string strFilePath )
{
	for( vector<CParticleScript*>::iterator it = vParticleScriptCache.begin(); it != vParticleScriptCache.end(); it++ )
	{
		CParticleScript * pcCachedParticleScript = *it;

		if( pcCachedParticleScript->GetFilePath().compare( strFilePath ) == 0 )
			return pcCachedParticleScript;
	}

	return NULL;
}

CParticleFactory * CParticleFactory::GetThis( LuaState * pLuaState )
{
	CParticleFactory * pThis = (CParticleFactory*)pLuaState->lua;

	return pThis;
}

int CParticleFactory::LuaInt( LuaState * pLuaState )
{
	int iInt = ReadParamInteger( pLuaState, 1 );

	CParticle::Int * pcParticleInt = new CParticle::Int( iInt );

	lua_pushinteger( pLuaState->get_ptr(), (int)pcParticleInt );

	return 1;
}

int CParticleFactory::LuaRandomInt( LuaState * pLuaState )
{
	int iIntL = ReadParamInteger( pLuaState, 1 );
	int iIntH = ReadParamInteger( pLuaState, 2 );

	CParticle::Int * pcParticleInt = new CParticle::Int( iIntL, iIntH );

	lua_pushinteger( pLuaState->get_ptr(), (int)pcParticleInt );

	return 1;
}

int CParticleFactory::LuaDbl( LuaState * pLuaState )
{
	double dDbl = ReadParamDouble( pLuaState, 1 );

	CParticle::Dbl * pcParticleDbl = new CParticle::Dbl( dDbl );

	lua_pushinteger( pLuaState->get_ptr(), (int)pcParticleDbl );

	return 1;
}

int CParticleFactory::LuaRandomDbl( LuaState * pLuaState )
{
	double dDblL = ReadParamDouble( pLuaState, 1 );
	double dDblH = ReadParamDouble( pLuaState, 2 );

	CParticle::Dbl * pcParticleDbl = new CParticle::Dbl( dDblL, dDblH );

	lua_pushinteger( pLuaState->get_ptr(), (int)pcParticleDbl );

	return 1;
}

int CParticleFactory::LuaScript( LuaState * pLuaState )
{
	CParticleFactory * pThis			= GetThis( pLuaState );
	CParticleScript * pcParticleScript	= pThis->pcParticleScript;

	if( pcParticleScript )
	{
		string strScript = ReadParamString( pLuaState, 1 );

		pcParticleScript->strScript = strScript;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Script( \"%s\" )\r\n", strScript.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaAuthor( LuaState * pLuaState )
{
	CParticleFactory * pThis			= GetThis( pLuaState );
	CParticleScript * pcParticleScript	= pThis->pcParticleScript;

	if( pcParticleScript )
	{
		string strAuthor = ReadParamString( pLuaState, 1 );

		pcParticleScript->strAuthor = strAuthor;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Author( \"%s\" )\r\n", strAuthor.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaParticleSystem( LuaState * pLuaState )
{
	CParticleFactory * pThis			= GetThis( pLuaState );
	CParticleScript * pcParticleScript	= pThis->pcParticleScript;

	if( pcParticleScript )
	{
		string strParticleType = ReadParamString( pLuaState, 1 );

		CParticleSystem * pcParticleSystem	= new CParticleSystem();
		pcParticleSystem->pcParticle			= new CParticle();

		if( strParticleType.compare( "Billboard" ) == 0 )
		{
			//A Simple Billboard
			pcParticleSystem->pcParticle	->eVisualType	= PARTICLEVISUALTYPE_Billboard;
		}
		else if( strParticleType.compare( "BillboardRotation" ) == 0 )
		{
			//A Simple Billboard Rotation
			pcParticleSystem->pcParticle->eVisualType = PARTICLEVISUALTYPE_BillboardRotation;
		}
		else if( strParticleType.compare( "BillboardHorizontal" ) == 0 )
		{
			//A Horizontal Billboard
			pcParticleSystem->pcParticle	->eVisualType	= PARTICLEVISUALTYPE_BillboardHorizontal;
		}
		else if( strParticleType.compare( "BillboardHorizontalRotation" ) == 0 )
		{
			//A Horizontal Billboard with Rotation Attributes
			pcParticleSystem->pcParticle	->eVisualType	= PARTICLEVISUALTYPE_BillboardHorizontalRotation;
		}
		else if( strParticleType.compare( "BillboardVertical" ) == 0 )
		{
			//A Horizontal Billboard with Rotation Attributes
			pcParticleSystem->pcParticle	->eVisualType	= PARTICLEVISUALTYPE_BillboardVertical;
		}
		else if( strParticleType.compare( "BillboardVerticalRotation" ) == 0 )
		{
			//A Horizontal Billboard with Rotation Attributes
			pcParticleSystem->pcParticle	->eVisualType	= PARTICLEVISUALTYPE_BillboardVerticalRotation;
		}
		else
			ERRORBOX( "ParticleSystem: Parameter 1 is not a valid Particle Type" );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ParticleSystem( \"%s\" )\r\n", strParticleType.c_str() ) );
#endif

		pcParticleScript->pcParticleSystem = pcParticleSystem;
	}

	return 0;
}

int CParticleFactory::LuaParticleSystemEnd( LuaState * pLuaState )
{
	CParticleFactory * pThis			= GetThis( pLuaState );
	CParticleScript * pcParticleScript	= pThis->pcParticleScript;

	if( pcParticleScript )
	{
		pcParticleScript->Add( pcParticleScript->pcParticleSystem );

		pcParticleScript->pcParticleSystem = NULL;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ParticleSystemEnd()\r\n" ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaParticleObject( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;

	if ( pcParticleSystem && pcParticleEmitter )
	{
		if ( pcParticleSystem->pcParticle )
			DELET( pcParticleSystem->pcParticle );

		pcParticleSystem->pcParticle = new CParticle();

		pcParticleSystem->pcParticle->SetName( ReadParamString( pLuaState, 1 ) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ParticleObject( \"%s\" )\r\n", ReadParamString( pLuaState, 1 ).c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaParticleObjectEnd( LuaState * pLuaState )
{
	CParticleFactory * pThis = GetThis( pLuaState );
	CParticleScript * pcParticleScript = pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem = pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter = pcParticleSystem->pcParticleEmitter;

	if ( pcParticleSystem && pcParticleEmitter )
	{
		if ( pcParticleSystem->pcParticle )
		{
			pcParticleSystem->Add( pcParticleSystem->pcParticle );

			DELET( pcParticleSystem->pcParticle );
		}

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ParticleObjectEnd()\r\n" ) );
#endif

		pcParticleSystem->pcParticle = NULL;
	}

	return 0;
}

int CParticleFactory::LuaAddObject( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;

	if ( pcParticleSystem && pcParticleEmitter )
	{
		string strName		= ReadParamString( pLuaState, 1 );
		string strNameType	= ReadParamString( pLuaState, 2 );

		for ( std::vector<CParticle*>::iterator it = pcParticleSystem->vActiveParticles.begin(); it != pcParticleSystem->vActiveParticles.end(); it++ )
		{
			CParticle * pcParticle = (*it);

			if ( !pcParticle->IsActive() )
			{
				if ( STRINGCOMPARE( pcParticle->GetName().c_str(), strName.c_str() ) )
				{
					pcParticle->SetNameType( strNameType );

					pcParticle->SetActive( TRUE );
					break;
				}
			}
		}

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ParticleObject( \"%s\" )\r\n", ReadParamString( pLuaState, 1 ).c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaName( LuaState * pLuaState )
{
	CParticleFactory * pThis			= GetThis( pLuaState );
	CParticleScript * pcParticleScript	= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem	= pcParticleScript->pcParticleSystem;

	if( pcParticleSystem )
	{
		string strName = ReadParamString( pLuaState, 1 );

		pcParticleSystem->strName = strName;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Name( \"%s\" )\r\n", strName.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaEmitType( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		string strEmitterType = ReadParamString( pLuaState, 1 );

		if( strEmitterType.compare( "Follow" ) == 0 )
			pcParticleEmitter->eType	= PARTICLEEMITTERTYPE_Follow;
		else if( strEmitterType.compare( "Free" ) == 0 )
			pcParticleEmitter->eType	= PARTICLEEMITTERTYPE_Free;
		else
			ERRORBOX( "EmitType: Parameter 1 is not a valid Emitter Type" );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitType( \"%s\" )\r\n", strEmitterType.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaEmitPosition( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Int * x = (CParticle::Int*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * y = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );
		CParticle::Int * z = (CParticle::Int*)ReadParamPointer( pLuaState, 3 );

		pcParticleEmitter->sPosition.x = CParticle::Int( ((*x) << 8) );
		pcParticleEmitter->sPosition.y = CParticle::Int( ((*y) << 8) );
		pcParticleEmitter->sPosition.z = CParticle::Int( ((*z) << 8) );


#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Position( Int( %d ), Int( %d ), Int( %d ) )\r\n",
				pcParticleEmitter->sPosition.x.value >> 8, pcParticleEmitter->sPosition.y.value >> 8, pcParticleEmitter->sPosition.z.value >> 8 ) );
#endif

		DELET( x );
		DELET( y );
		DELET( z );
	}

	return 0;
}

int CParticleFactory::LuaEmitVelocity( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Int * x = (CParticle::Int*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * y = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );
		CParticle::Int * z = (CParticle::Int*)ReadParamPointer( pLuaState, 3 );

		pcParticleEmitter->sVelocity.x = CParticle::Int( ((*x) << 8) );
		pcParticleEmitter->sVelocity.y = CParticle::Int( ((*y) << 8) );
		pcParticleEmitter->sVelocity.z = CParticle::Int( ((*z) << 8) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Velocity( Int( %d ), Int( %d ), Int( %d ) )\r\n",
				pcParticleEmitter->sVelocity.x.value >> 8, pcParticleEmitter->sVelocity.y.value >> 8, pcParticleEmitter->sVelocity.z.value >> 8 ) );
#endif


		DELET( x );
		DELET( y );
		DELET( z );
	}

	return 0;
}

int CParticleFactory::LuaEmitRotation( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * p = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Dbl * y = (CParticle::Dbl*)ReadParamPointer( pLuaState, 2 );
		CParticle::Dbl * r = (CParticle::Dbl*)ReadParamPointer( pLuaState, 3 );

		pcParticleEmitter->sRotation.p = *p;
		pcParticleEmitter->sRotation.y = *y;
		pcParticleEmitter->sRotation.r = *r;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitRotation( Dbl( %lf ), Dbl( %lf ), Dbl( %lf ) )\r\n",
				pcParticleEmitter->sRotation.p.value, pcParticleEmitter->sRotation.y.value, pcParticleEmitter->sRotation.r.value ) );
#endif

		DELET( r );
		DELET( y );
		DELET( p );
	}

	return 0;
}

int CParticleFactory::LuaEmitAngularVelocity( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * p = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Dbl * y = (CParticle::Dbl*)ReadParamPointer( pLuaState, 2 );
		CParticle::Dbl * r = (CParticle::Dbl*)ReadParamPointer( pLuaState, 3 );

		pcParticleEmitter->sAngularVelocity.p = *p;
		pcParticleEmitter->sAngularVelocity.y = *y;
		pcParticleEmitter->sAngularVelocity.r = *r;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitAngularVelocity( Dbl( %lf ), Dbl( %lf ), Dbl( %lf ) )\r\n",
				pcParticleEmitter->sAngularVelocity.p.value, pcParticleEmitter->sAngularVelocity.y.value, pcParticleEmitter->sAngularVelocity.r.value ) );
#endif

		DELET( r );
		DELET( y );
		DELET( p );
	}

	return 0;
}

int CParticleFactory::LuaEmitDelay( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );

		pcParticleEmitter->sMaxDelay.t = CParticle::Dbl( ((*d) * 1000.0f) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitDelay( Dbl( %lf ) )\r\n", (*d) ) );
#endif

		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaEmitLifeTime( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );

		pcParticleEmitter->sMaxLifeTime.t = CParticle::Dbl( ((*d) * 1000.0f) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitLifeTime( Dbl( %lf ) )\r\n", (*d) ) );
#endif

		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaEmitParticles( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Int * p = (CParticle::Int*)ReadParamPointer( pLuaState, 1 );

		pcParticleEmitter->sParticles = CParticle::Int( *p );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitParticles( Int( %d ) )\r\n", (*p) ) );
#endif

		DELET( p );
	}

	return 0;
}

int CParticleFactory::LuaEmitInterval( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );

		pcParticleEmitter->sInterval = CParticle::Dbl( ((*d) * 1000.0f) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "EmitInterval( Dbl( %lf ) )\r\n", (*d) ) );
#endif

		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaType( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		string strEmitterType = ReadParamString( pLuaState, 1 );

		if( strEmitterType.compare( "Follow" ) == 0 )
			pcParticle->eType	= PARTICLETYPE_Follow;
		else if( strEmitterType.compare( "Free" ) == 0 )
			pcParticle->eType	= PARTICLETYPE_Free;
		else
			ERRORBOX( "Type: Parameter 1 is not a valid Particle Type" );


#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Type( \"%s\" )\r\n", strEmitterType.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaTexture( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		std::string strFilePath = ReadParamString( pLuaState, 1 );

		pcParticle->uTextureID = pThis->pcTextureManager->LoadTexture( strFilePath );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Texture( \"%s\" )\r\n", strFilePath.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaBlend( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		string strBlendMode = ReadParamString( pLuaState, 1 );

		EBlendType e = BLENDTYPE_None;

		if( strBlendMode.compare( "Lamp" ) == 0 )
		{
			//?
			e = BLENDTYPE_Lamp;
		}
		else if( strBlendMode.compare( "Alpha" ) == 0 )
		{
			//?
			e = BLENDTYPE_Alpha;
		}
		else if( strBlendMode.compare( "Inverse Shadow" ) == 0 )
		{
			//?
			e = BLENDTYPE_InvShadow;
		}
		else if( strBlendMode.compare( "Shadow" ) == 0 )
		{
			//?
			e = BLENDTYPE_Shadow;
		}
		else if( strBlendMode.compare( "Color" ) == 0 )
		{
			//?
			e = BLENDTYPE_Color;
		}
		else if( strBlendMode.compare( "Add Color" ) == 0 )
		{
			//?
			e = BLENDTYPE_AddColor;
		}
		else
		{
			ERRORBOX( "Blend: Parameter 1 is not a valid Particle Blend Mode" );
			
			//Default to Lamp Blend Mode
			e = BLENDTYPE_Lamp;
		}

		pcParticle->eSourceBlendMode = e;

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Blend( \"%s\" )\r\n", strBlendMode.c_str() ) );
#endif
	}

	return 0;
}

int CParticleFactory::LuaRGBA( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Int * r = (CParticle::Int*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * g = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );
		CParticle::Int * b = (CParticle::Int*)ReadParamPointer( pLuaState, 3 );
		CParticle::Int * a = (CParticle::Int*)ReadParamPointer( pLuaState, 4 );

		pcParticle->sColor.a = CParticle::Int( *a );
		pcParticle->sColor.r = CParticle::Int( *r );
		pcParticle->sColor.g = CParticle::Int( *g );
		pcParticle->sColor.b = CParticle::Int( *b );


#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "RGBA( Int( %d ), Int( %d ), Int( %d ), Int( %d ) )\r\n", (*r), (*g), (*b), (*a) ) );
#endif

		DELET( r );
		DELET( g );
		DELET( b );
		DELET( a );
	}

	return 0;
}

int CParticleFactory::LuaSize( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Int * size = (CParticle::Int*)ReadParamPointer( pLuaState, 1 );

		pcParticle->sSize.s = CParticle::Int( ((*size) << 8) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "Size( Int( %d ) )\r\n", (*size) ) );
#endif

		DELET( size );
	}

	return 0;
}

int CParticleFactory::LuaSizeWH( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Int * w = (CParticle::Int*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * h = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );

		pcParticle->sSize.bWH	= TRUE;
		pcParticle->sSize.w		= CParticle::Int( ((*w) << 8) );
		pcParticle->sSize.h		= CParticle::Int( ((*h) << 8) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "SizeWH( Int( %d ), Int( %d ) )\r\n", (*w), (*h) ) );
#endif

		DELET( w );
		DELET( h );
	}

	return 0;
}

int CParticleFactory::LuaLoop( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * b = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Dbl * e = (CParticle::Dbl*)ReadParamPointer( pLuaState, 2 );

		b->Birth();
		e->Birth();

		pcParticle->bLoop		= TRUE;
		pcParticle->fLoopAt		= b->value * 1000.0f;
		pcParticle->fLoopTime	= e->value * 1000.0f;

		DELET( e );
		DELET( b );
	}

	return 0;
}

int CParticleFactory::LuaLifeTime( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );

		pcParticle->sMaxLifeTime.t = CParticle::Dbl( ((*d) * 1000.0f) );


#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "LifeTime( Dbl( %lf ) )\r\n", (*d) ) );
#endif

		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaEventVelocity( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * x = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );
		CParticle::Int * y = (CParticle::Int*)ReadParamPointer( pLuaState, 3 );
		CParticle::Int * z = (CParticle::Int*)ReadParamPointer( pLuaState, 4 );

		d->Birth();

		CParticle::Point sVelocity;
		sVelocity.x = CParticle::Int( ((*x) << 8) );
		sVelocity.y = CParticle::Int( ((*y) << 8) );
		sVelocity.z = CParticle::Int( ((*z) << 8) );

		pcParticle->vParticleEvents.push_back( new CParticleEventVelocity( sVelocity, d->value * 1000.0f ) );

		DELET( z );
		DELET( y );
		DELET( x );
		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaEventSize( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * i = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );

		d->Birth();

		CParticle::Size sSize;
		sSize.s = CParticle::Int( ((*i) << 8) );

		pcParticle->vParticleEvents.push_back( new CParticleEventSize( sSize, d->value * 1000.0f ) );

		DELET( i );
		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaModifierSize( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * s = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );

		d->Birth();

		CParticle::Size sSize;
		sSize.s = CParticle::Int( ((*s) << 8) );

		CParticleModifierGroup * pcGroup = pcParticle->GetParticleModifierGroup( PARTICLEMODIFIERGROUPTYPE_Size );

		if( pcGroup == NULL )
		{
			pcGroup = new CParticleModifierGroup( PARTICLEMODIFIERGROUPTYPE_Size );

			pcParticle->AddParticleModifierGroup( pcGroup );
		}
		
		pcGroup->AddModifier( new CParticleModifierSize( sSize, d->value * 1000.0f ) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ModifierSize( Dbl( %f ), Int( %d ) )\r\n", (float)d->value, sSize.s.value >> 8 ) );
#endif

		DELET( s );
		DELET( d );
	}

	return 0;
}

int CParticleFactory::LuaModifierRGBA( LuaState * pLuaState )
{
	CParticleFactory * pThis				= GetThis( pLuaState );
	CParticleScript * pcParticleScript		= pThis->pcParticleScript;
	CParticleSystem * pcParticleSystem		= pcParticleScript->pcParticleSystem;
	CParticleEmitter * pcParticleEmitter	= pcParticleSystem->pcParticleEmitter;
	CParticle * pcParticle					= pcParticleSystem->pcParticle;

	if( pcParticleSystem && pcParticleEmitter && pcParticle )
	{
		CParticle::Dbl * d = (CParticle::Dbl*)ReadParamPointer( pLuaState, 1 );
		CParticle::Int * r = (CParticle::Int*)ReadParamPointer( pLuaState, 2 );
		CParticle::Int * g = (CParticle::Int*)ReadParamPointer( pLuaState, 3 );
		CParticle::Int * b = (CParticle::Int*)ReadParamPointer( pLuaState, 4 );
		CParticle::Int * a = (CParticle::Int*)ReadParamPointer( pLuaState, 5 );

		d->Birth();

		CParticle::Color sColor;
		sColor.r = *r;
		sColor.g = *g;
		sColor.b = *b;
		sColor.a = *a;

		CParticleModifierGroup * pcGroup = pcParticle->GetParticleModifierGroup( PARTICLEMODIFIERGROUPTYPE_Color );

		if( pcGroup == NULL )
		{
			pcGroup = new CParticleModifierGroup( PARTICLEMODIFIERGROUPTYPE_Color );

			pcParticle->AddParticleModifierGroup( pcGroup );
		}
		
		pcGroup->AddModifier( new CParticleModifierColor( sColor, d->value * 1000.0f ) );

#ifdef LUAPT_DEBUG
		StringCbCatA( szLuaFileDebug, _countof( szLuaFileDebug ),
			FormatString( "ModifierRGBA( Dbl( %f ), Int( %d ), Int( %d ), Int( %d ), Int( %d ) )\r\n", (float)d->value, sColor.r.value & 0xFF, sColor.g.value & 0xFF, sColor.b.value & 0xFF, sColor.a.value & 0xFF ) );
#endif

		DELET( a );
		DELET( b );
		DELET( g );
		DELET( r );
		DELET( d );
	}

	return 0;
}