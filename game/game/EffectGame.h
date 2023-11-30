#pragma once

#include "gameeffectfollow.h"

class CShamanMeteorite : public EffectBaseFollow
{
public:
	CShamanMeteorite();
	~CShamanMeteorite();

	void				Start( Point3D sBeginPosition, Point3D sEndPosition, int iDelay = 0 );

	void				Main();

	void				SetDelay();

	void				ParticleHit( Point3D sHitPosition );

private:
	int					iParticleID;
	X3D::Vector3		sPosition;
	X3D::Vector3		sVelocity;
	int					iTimeSkillCount;

	int					iDelay;
	bool				bSpawn;
};


class EffectGame
{
private:
	IMPFNC							pfnFireMeteorite				= 0x005B9B30;
	IMPFNC							pfnIceMeteorite					= 0x005B92F0;
	IMPFNC							pfnRegenerationFieldCelestial	= 0x005D4850;
	IMPFNC							pfnInertiaTarget				= 0x005B6EC0;
public:
	
	static void						FireMeteorite( Point3D * psBeginPosition, Point3D * psEndPosition, int iDelay, BOOL bSmallMeteor );
	static void						IceMeteorite( Point3D * psBeginPosition, Point3D * psEndPosition, int iDelay );
	static void						ShamanMeteorite( Point3D sBeginPosition, Point3D sEndPosition, int iDelay );
	static void						ShamanMeteorite( Point3D sPosition, int iCount, int iDelay = 0 );

	static void						RegenerationFieldCelestial( UnitData * pcUnitData, float fTime );

	static void						InertiaTarget( const char * pszParticleName, UnitData * pcUnitData, int iCount = 5, int iID = SKILLID_Inertia );

	static void						HandlePacket( PacketIceMeteorite * psPacket );

};

