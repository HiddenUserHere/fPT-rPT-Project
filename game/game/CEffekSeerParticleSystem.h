#pragma once
class CEffekSeerParticleSystem
{
private:
	Effekseer::Effect			* pcEffect = nullptr;
	Effekseer::Handle			iHandle = -1;

	UINT						iID = 0;

	Engine::ObjectOwner			* pcOwner = nullptr;

	BOOL						bKill = FALSE;

public:
	CEffekSeerParticleSystem( Effekseer::Effect * _pcEffect );
	CEffekSeerParticleSystem( const CEffekSeerParticleSystem & c );
	~CEffekSeerParticleSystem();

	Effekseer::Effect			* GetEffect() { return pcEffect; }

	Effekseer::Handle			GetHandle() { return iHandle; }
	void						SetHandle( Effekseer::Handle _iHandle ) { iHandle = _iHandle; }

	UINT						GetID() { return iID; }
	void						SetID( UINT _iID ) { iID = _iID; }

	Engine::ObjectOwner			* GetOwner() { return pcOwner; }
	void						SetOwner( Engine::ObjectOwner * _pcOwner );

	UINT						GetOwnerID();

	void						Kill( BOOL bForce );

	void						Render( Effekseer::Manager * pcManager, BOOL bBack );

	BOOL						Update( float fTime );
};