#pragma once

struct RainMakerParticleBip
{
	int													iID;
	int													iTime;
	DWORD												dwObjectID;
	class UnitData										* pcUnitGame;
};

class CShamanHandler
{
private:

protected:
	int													iRainMakerBipParticleID = -1;
public:
	CShamanHandler();
	virtual ~CShamanHandler();

	void												LoadParticles();
	void												Init();

	void												OnRainMakerParticle( class UnitData * pcUnitGame, int iTime );
	void												RenderParticle();
	void												Update(float fTime);
};

