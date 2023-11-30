#pragma once

class CParticleEngine;

class CParticleDebug
{
public:
						  CParticleDebug( CParticleEngine * _pcParticleEngine );
	virtual				 ~CParticleDebug();

	void				  Render();

	BOOL				  GetDisplay() { return bDisplay; }
	void				  SetDisplay( BOOL b ) { bDisplay = b; }

protected:
	CParticleEngine		* pcParticleEngine;

	BOOL				  bDisplay;

};