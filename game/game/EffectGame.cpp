#include "stdafx.h"
#include "EffectGame.h"


NAKED void EffectGame::FireMeteorite( Point3D * psBeginPosition, Point3D * psEndPosition, int iDelay, BOOL bSmallMeteor )
{
	JMP( pfnFireMeteorite );
}

void EffectGame::IceMeteorite( Point3D * psBeginPosition, Point3D * psEndPosition, int iDelay )
{
	CALL_WITH_ARG3( pfnIceMeteorite, (DWORD)psBeginPosition, (DWORD)psEndPosition, iDelay );
}

void EffectGame::ShamanMeteorite( Point3D sBeginPosition, Point3D sEndPosition, int iDelay )
{
	if ( int iIndex = EffectBaseFollow::GetFreeIndex(); iIndex >= 0 )
	{
		CShamanMeteorite * pcShamanMeteorite = new CShamanMeteorite();

		pcShamanMeteorite->Start( sBeginPosition, sEndPosition, iDelay );

		EffectBaseFollow::GetData()[iIndex] = pcShamanMeteorite;
	}
}

void EffectGame::ShamanMeteorite( Point3D sPosition, int iCount, int iDelay )
{
	for ( int i = 0; i < iCount; i++ )
	{
		Point3D sBegin = sPosition;
		Point3D sEnd = sPosition;
		sBegin.iY += 130000;

		if ( iCount > 1 )
		{
			sEnd.iX -= RandomI( 0, 75 ) << 8;
			sEnd.iZ -= RandomI( 0, 75 ) << 8;

			sEnd.iX += RandomI( 0, 150 ) << 8;
			sEnd.iZ += RandomI( 0, 150 ) << 8;
		}

		EffectGame::ShamanMeteorite( sBegin, sEnd, iDelay );
	}
}

NAKED void EffectGame::RegenerationFieldCelestial( UnitData * pcUnitData, float fTime )
{
	JMP( pfnRegenerationFieldCelestial );
}

NAKED void EffectGame::InertiaTarget( const char * pszParticleName, UnitData * pcUnitData, int iCount, int iID )
{
	JMP( pfnInertiaTarget );
}

void EffectGame::HandlePacket( PacketIceMeteorite * psPacket )
{
	for ( UINT i = 0; i < psPacket->uMeteoriteCount; i++ )
	{
		Point3D sEnd = psPacket->sEnd;
		int iDelay = psPacket->iDelay;

		if ( psPacket->uMeteoriteCount > 1 )
		{
			sEnd.iX -= RandomI( 0, 75 ) << 8;
			sEnd.iZ -= RandomI( 0, 75 ) << 8;

			sEnd.iX += RandomI( 0, 150 ) << 8;
			sEnd.iZ += RandomI( 0, 150 ) << 8;

			//iDelay = RandomI( 51, 250 );
		}

		EffectGame::IceMeteorite( &psPacket->sBegin, &sEnd, iDelay );
	}
}

CShamanMeteorite::CShamanMeteorite()
{
	iParticleID = -1;
	iTimeSkillCount = 0;
	bSpawn = false;
}

CShamanMeteorite::~CShamanMeteorite()
{
}

void CShamanMeteorite::Start( Point3D sBeginPosition, Point3D sEndPosition, int iDelay )
{
	sPosition					= X3D::Vector3( (float)sBeginPosition.iX, (float)sBeginPosition.iY, (float)sBeginPosition.iZ );
	X3D::Vector3 sEndPositionF	= X3D::Vector3( (float)sEndPosition.iX, (float)sEndPosition.iY, (float)sEndPosition.iZ );

	X3D::Vector3 sDiff = (sEndPositionF - sPosition) / 256.0f;

	sVelocity.x = sDiff.x / sDiff.length() * 8.0f;
	sVelocity.y = sDiff.y / sDiff.length() * 8.0f;
	sVelocity.z = sDiff.z / sDiff.length() * 8.0f;

	this->iDelay = iDelay;
	if ( iDelay == 1 )
	{
		bSpawn = true;
		iParticleID = PARTICLE->Start( "PressOfDeity2", (int)sPosition.x, (int)sPosition.y, (int)sPosition.z );

		SOUNDHANDLER->SetSkillSound( 0x4846, (int)sPosition.x, (int)sPosition.y, (int)sPosition.z );
	}

	iTimeMax = 2000;
	iTimeSkillCount = 70 * 10;
}

void CShamanMeteorite::Main()
{
	if ( iParticleID >= 0 )
	{
		sPosition += sVelocity * 256.0f;

		PARTICLE->SetPosition( iParticleID, sPosition );

		iTimeSkillCount--;

		if ( iTimeSkillCount <= 0 )
		{
			PARTICLE->Stop( iParticleID );

			iParticleID = -1;

			iTime = iTimeMax;
		}

		int iMapOneY = 0, iMapTwoY = 0;
		if ( STAGE1 )
			iMapOneY = STAGE1->GetHighestPoint( (int)sPosition.x, (int)sPosition.z );
		if ( STAGE2 )
			iMapTwoY = STAGE2->GetHighestPoint( (int)sPosition.x, (int)sPosition.z );

		if ( STAGE1 && STAGE2 )
			if ( iMapOneY < iMapTwoY )
				iMapOneY = iMapTwoY;
		if ( (STAGE1 == nullptr) && STAGE2 )
			iMapOneY = iMapTwoY;

		iMapOneY -= (40 << 8);

		if ( iMapOneY > ( int )sPosition.y )
		{
			PARTICLE->Stop( iParticleID );

			iParticleID = -1;

			iTime = iTimeMax;

			Point3D sHitPosition = Point3D( (int)sPosition.x, (int)sPosition.y + (40 << 8), (int)sPosition.z );
			ParticleHit( sHitPosition );
		}
	}

	if ( bSpawn == false )
	{
		iDelay--;
		if ( iDelay <= 0 )
		{
			Point3D sStartPosition = Point3D( (int)sPosition.x, (int)sPosition.y, (int)sPosition.z );
			iParticleID = PARTICLE->Start( "PressOfDeity2", sStartPosition.iX, sStartPosition.iY, sStartPosition.iZ );
			SOUNDHANDLER->SetSkillSound( 0x4846, sStartPosition.iX, sStartPosition.iY, sStartPosition.iZ );

			bSpawn = true;
		}
	}
}

void CShamanMeteorite::SetDelay()
{
}

void CShamanMeteorite::ParticleHit( Point3D sHitPosition )
{
	X3D::Vector3 sDiff;
	sDiff.x = (float)(sHitPosition.iX - UNITDATA->sPosition.iX) / 256.0f;
	sDiff.y = (float)(sHitPosition.iY - UNITDATA->sPosition.iY) / 256.0f;
	sDiff.z = (float)(sHitPosition.iZ - UNITDATA->sPosition.iZ) / 256.0f;

	float fLength = sDiff.length();
	if ( fLength == 0 )
		fLength = 1;

	SetDynamicLight( sHitPosition.iX, sHitPosition.iY, sHitPosition.iZ, 100, 200, 255, 255, 250, 2 );
	PARTICLE->Start( "PressOfDeity1", sHitPosition.iX, sHitPosition.iY, sHitPosition.iZ );
	SOUNDHANDLER->SetSkillSound( 0x4742, sHitPosition.iX, sHitPosition.iY, sHitPosition.iZ );
}
