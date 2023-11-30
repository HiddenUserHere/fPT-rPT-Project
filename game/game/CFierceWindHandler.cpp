#include "stdafx.h"
#include "CFierceWindHandler.h"

NAKED BOOL CFierceWindHandler::SetProjectile( Point3D * psPosition, Point3D * psTargetPosition )
{
	JMP( pfnSetStormProjectile );
}

CFierceWindHandler::CFierceWindHandler()
{
}

CFierceWindHandler::~CFierceWindHandler()
{
}

void CFierceWindHandler::LoadParticle()
{
	iParticleID = PARTICLE->Load( "game\\scripts\\particles\\Ac5FierceWind.part" );
	iParticleAID = PARTICLE->Load( "game\\scripts\\particles\\Ac5FierceWindKeep.part" );
	iParticleProjectileID = PARTICLE->Load( "game\\scripts\\particles\\Ac5FierceWindProjectile.part" );
}

void CFierceWindHandler::RenderEffect( class UnitData * pcUnitData )
{
#ifndef _TIER5SKILLS_
	return;
#endif
	/*Unit * u = GETUNIT( pcUnitData );
	if ( u )
	{
		if ( u->IsUsingFierceWind() )
		{
			// Get Weapon iItemSlot
			if ( (pcUnitData->sLeftHand.dwItemCode & 0xFFFF0000) == ITEMTYPE_Bow )
			{
				smMATRIX * psWorld = &pcUnitData->HvLeftHand_Matrix;
				POINT3D sWeapon{ (int)((float)pcUnitData->sLeftHand.SizeMax * 0.3f), 0, 0 };

				// Radian
				int rx, ry, rz;
				rx = sWeapon.x * psWorld->_11 +
					sWeapon.y * psWorld->_21 +
					sWeapon.z * psWorld->_31;

				ry = sWeapon.x * psWorld->_12 +
					sWeapon.y * psWorld->_22 +
					sWeapon.z * psWorld->_32;

				rz = sWeapon.x * psWorld->_13 +
					sWeapon.y * psWorld->_23 +
					sWeapon.z * psWorld->_33;

				sWeapon.x = (pcUnitData->sPosition.iX + (rx >> 8) + psWorld->_41);
				sWeapon.z = (pcUnitData->sPosition.iZ + (ry >> 8) + psWorld->_42);
				sWeapon.y = (pcUnitData->sPosition.iY + (rz >> 8) + psWorld->_43);

				// Set Particle iItemSlot
				PARTICLE->SetPosition( u->GetFierceWindAEffect(), D3DVECTOR{ (float)sWeapon.x, (float)sWeapon.y, (float)sWeapon.z } );
			}
		}
	}*/
}

void CFierceWindHandler::OnKeep( class UnitData * pcUnitData )
{

}

bool CFierceWindHandler::OnCastAnimationStart( struct Skill * psSkill, class UnitData * pcUnitData )
{
	Unit * p = UNITDATATOUNIT( pcUnitData );

	if ( p )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID	= SKILLPLAYID_T51;
			UNITDATA->sActiveSkill.cLevel		= psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			//New FPS based on Weapon Speed
			UNITDATA->iNewFPS					= SKILLMANAGERHANDLER->GetAttackFrameSpeed( UNITDATA->sCharacterData.iAttackSpeed );
			UNITDATA->bOverrideFPS				= TRUE;

			SKILLMANAGERHANDLER->SkillAnimationStart( UNITDATA, UNITDATA->sActiveSkill.iValue, psSkill->iLevel, UNITDATA->pcTarget, UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );

			return true;
		}
	}

	return false;
}

BOOL CFierceWindHandler::OnCastEffect( Unit * pcUnit, short sTimeLeft )
{
	int iID = PARTICLE->Start( iParticleAID, pcUnit->pcUnitData->sPosition.iX, pcUnit->pcUnitData->sPosition.iY - (100 << 8) + pcUnit->pcUnitData->iModelHeight, pcUnit->pcUnitData->sPosition.iZ );

	PARTICLE->AddScript( iID, iParticleAID, pcUnit->pcUnitData,(float)sTimeLeft, TRUE );

	return TRUE;
}

BOOL CFierceWindHandler::OnCastEnd( Unit * pcUnit )
{
	pcUnit->bFierceWind = FALSE;

	PARTICLE->Delete( pcUnit->GetID(), iParticleAID );

	return TRUE;
}

BOOL CFierceWindHandler::OnEvent( UnitData * pcUnitData )
{
	Point3D s;

	if ( pcUnitData->pcTarget )
	{
		UnitData * pcTarget = pcUnitData->pcTarget;

		if ( pcUnitData->iAnimationEvent == 1 )
		{
			if ( pcUnitData->GetAnimationAttackPosition( &s.iX, &s.iY, &s.iZ ) )
			{
				if ( pcUnitData->iTargetCritical >= 0 )
				{
					Point3D sWeaponPosition;

					//Get Position of Bow, scaling on Front side angle...
					SKILLMANAGERHANDLER->GetWeaponPositionNew( pcUnitData, sWeaponPosition, 0.3f );

					PARTICLE->Start( iParticleProjectileID, sWeaponPosition.iX, sWeaponPosition.iY - (100 << 8), sWeaponPosition.iZ );

					return TRUE;
				}
			}
		}
		else 
		{
			//Hit...
			if ( pcUnitData->iAnimationEvent == 2 )
			{ 
				if ( pcUnitData->GetAnimationAttackPosition( &s.iX, &s.iY, &s.iZ ) )
				{
					//Set Projectile Effect based on character position
					Point3D sTargetPosition = pcTarget->sPosition;						
					sTargetPosition.iY		+= (24 << 8);

					//Use Extreme Rage Sound
					SKILLMANAGERHANDLER->SkillPlaySound( 0x4521, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

					//Set Storm Projectile
					CFierceWindHandler::SetProjectile( &s, &sTargetPosition );

					if ( pcUnitData == UNITDATA )
					{
						RECT rRect;
						rRect.left		= -50;
						rRect.right		= 50;
						rRect.top		= 0;
						rRect.bottom	= 340;

						SKILLMANAGERHANDLER->SetUnitsRangeRect( UNITDATA, rRect );

						SKILLMANAGERHANDLER->DamageAreaHandle( pcTarget, pcTarget->sPosition, 0, 0, 0, 0, UNITDATA->sActiveSkill.iValue );
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CFierceWindHandler::OnAnimationEnd( UnitData * pcUnitData )
{
	SOUNDHANDLER->SetSkillSound( 0x482D, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

	return TRUE;
}

BOOL CFierceWindHandler::OnCastMember( UnitData * pcUnitData )
{
	int iID = PARTICLE->Start( iParticleID, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

	PARTICLE->AddScript( iID, iParticleID, pcUnitData, 1.0f, TRUE );

	SOUNDHANDLER->SetSkillSound( 0x4849, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

	return TRUE;
}
