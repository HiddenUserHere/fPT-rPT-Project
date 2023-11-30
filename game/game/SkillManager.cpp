#include "stdafx.h"
#include "SkillManager.h"


typedef				bool( __cdecl *t_IsActiveBuff )( UINT BuffID );
t_IsActiveBuff		IsActiveBuffFunc = ( t_IsActiveBuff )0x005B4810;
typedef				void( __cdecl *tfnStartSkillEffect )(int x, int y, int z, int angleX, int angleY, int angleZ, int skillType, int level);
tfnStartSkillEffect	fnStartSkillEffect = (tfnStartSkillEffect)0x00574250;
typedef				void( __cdecl *tfnSkillPlaySound )(DWORD dwCode, int iX, int iY, int iZ, int iFlag);
tfnSkillPlaySound fnSkillPlaySound = (tfnSkillPlaySound)0x00544870;

extern Skill sTempSkillTimerData[81];

static DebuffSkill saDebufSkills[]
{
	{ CHARACTERCLASS_Fighter, SKILLPLAYID_T23 },
	{ CHARACTERCLASS_Pikeman, SKILLPLAYID_T21 },
	{ CHARACTERCLASS_Atalanta, SKILLPLAYID_T11 },
	{ CHARACTERCLASS_Magician, SKILLPLAYID_T43 },
	{ CHARACTERCLASS_Priestess, SKILLPLAYID_T41 },
	{ CHARACTERCLASS_Shaman, SKILLPLAYID_T13 },
	{ CHARACTERCLASS_Pikeman, SKILLPLAYID_T51 },
};

SkillManager::SkillManager()
{
	ZeroMemory( baPetAggressive, sizeof( BOOL ) * 8 );
}

SkillManager::~SkillManager()
{
}

BOOL SkillManager::ResetSkills( int iNum )
{
	return CALLT_WITH_ARG1( pfnResetSkills, 0x0367E148, iNum );
}

BOOL SkillManager::IsActiveBuff( UINT uBuffID )
{
	return (BOOL)IsActiveBuffFunc( uBuffID );
}

NAKED BOOL SkillManager::ResetSkillTimer( int iSkillID )
{
	JMP( pfnClearSkillBuff );
}

void SkillManager::UseManaStamina( Skill * psSkill )
{
	CALL_WITH_ARG1( 0x00520DE0, (DWORD)psSkill );
}

int SkillManager::GetLevelSkill( UINT uSkillCode )
{
	for ( int i = 0; i < 20; i++ )
	{
		if ( SKILLSCHARACTER[ i ].iID == uSkillCode )
			return SKILLSCHARACTER[ i ].iLevel;
	}
	return 0;
}

void SkillManager::CancelForceOrb( int iForceOrbID )
{
	if( MESSAGEBOX->GetType() == FALSE )
		return;
	else
	{
		if ( sTempSkillTimerData[iForceOrbID].iID == 0x09010001 )
		{
			TIMERSKILLHANDLER->KillTimer( 0x09010001 );
			PacketSimple sPacket;
			sPacket.iLength = sizeof( PacketSimple );
			sPacket.iHeader = PKTHDR_CancelForceOrb;
			SENDPACKETG( &sPacket );
			DWORD dwCode;
			dwCode = UNITDATA->eForceOrbItemID;
			__asm
			{
				PUSH dwCode;
				PUSH 0;
				PUSH DWORD PTR DS : [0x00CF481C];
				MOV EAX, 0x005C16B0;
				CALL EAX;
				ADD ESP, 0x0C;
			}
			UNITDATA->eForceOrbItemID = ITEMID_None;
			UNITDATA->dwForceOrbTimeOut = 0;
		}
	}
}

BOOL SkillManager::IsDebuffSkill( UnitData * pcUnit, DWORD dwSkillCode )
{
	if ( pcUnit )
	{
		for ( int i = 0; i < _countof( saDebufSkills ); i++ )
		{
			if ( pcUnit->sCharacterData.iClass == saDebufSkills[i].iClass && saDebufSkills[i].SkillCode == dwSkillCode )
				return TRUE;
		}
	}
	return FALSE;
}

int SkillManager::GetResistanceTime( int iTime, int iResistanceID )
{
	return CALL_WITH_ARG2( 0x0041E610, iTime, iResistanceID );
}

void SkillManager::StunEffect( UnitData * pcUnitData, int iTime )
{
	CALL_WITH_ARG3( 0x005377B0, (DWORD)pcUnitData, iTime, 0 );
}

void SkillManager::GetInfoBoxSize( struct sSKILLBOX * psSkillBox, int iColumn, int iIcon )
{
	typedef void( __stdcall *tfnGetInfoBoxSize )( struct sSKILLBOX * psSkillBox, int iColumn, int iUnk );
	tfnGetInfoBoxSize fnGetInfoBoxSize = (tfnGetInfoBoxSize)0x004EB9C0;
	fnGetInfoBoxSize( psSkillBox, iColumn, iIcon );
}

void SkillManager::AddSkillInfo( int iColumn, const char * pszText, ... )
{
	char szBuffer[256] = { 0 };
	va_list ap;
	va_start( ap, pszText );
	_vsnprintf_s( szBuffer, 256, 255, pszText, ap );
	va_end( ap );

	if ( iColumn == 1 )
	{
		// Left
		lstrcatA( (char*)(0x036762A0), szBuffer );
	}
	else if ( iColumn == 2 )
	{
		// Right
		lstrcatA( (char*)(0x03677628), szBuffer );
	}
}

void SkillManager::StartSkillEffect( int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ, int iSkillType, int iLevel )
{
	fnStartSkillEffect( iX, iY, iZ, iAngleX, iAngleY, iAngleZ, iSkillType, iLevel );
}

NAKED void SkillManager::StartSkillEffectOld( int iX, int iY, int iZ, int iID, int iLevel )
{
	JMP( 0x0057CE60 );
}

void SkillManager::SkillPlaySound( DWORD dwCode, int iX, int iY, int iZ, int iFlag )
{
	fnSkillPlaySound( dwCode, iX, iY, iZ, iFlag );
}

BOOL SkillManager::CanUseSkill( Skill * psSkill )
{
	if ( UNITDATA && UNITDATA->iLoadedMapIndex >= 0 && FIELDGAME[UNITDATA->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
		return FALSE;

	if ( psSkill->iLevel > 10 )
		return FALSE;

	if( SKILLDATAHANDLER->IsSkillLocked( psSkill->iID ) )
	{
		TITLEBOX( "Skill is locked!" );
		return FALSE;
	}

	return (BOOL)CALL_WITH_ARG1( 0x00520A00, (DWORD)psSkill );
}

bool SkillManager::SkillPlay( struct Skill * psSkill, UnitData * pcUnitData )
{
	const DWORD pfnPvPModePlayer = 0x00CF47AC;

	bool bFlag = (psSkill->iID == SKILLID_Healing) || (psSkill->iID == SKILLID_EnchantWeapon) ||
		(psSkill->iID == SKILLID_VirtualLife) || (psSkill->iID == SKILLID_TriumphofJavelin);

	//Use Target?
	if ( (pcUnitData->sCharacterData.iType != CHARACTERTYPE_Monster) && (READDWORD( pfnPvPModePlayer ) == 0) && (bFlag == false) )
		return false;

	if ( CanUseSkill( psSkill ) == FALSE )
		return false;

	//Use Skill
	WRITEDWORD( 0x03673B78, (DWORD)psSkill );

	switch( UNITDATA->sCharacterData.iClass )
	{
		/* Fighter Skills */
		case CHARACTERCLASS_Fighter:
			switch ( psSkill->iID )
			{
				case SKILLID_SeismicImpact:
					return SESMICIMPACTHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

		/* Pikeman Skills */
		case CHARACTERCLASS_Pikeman:
			switch( psSkill->iID )
			{
				case SKILLID_RingofSpears:
					return RINGOFSPEARSHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;

				case SKILLID_LastBreath:
					return LASTBREATHHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

		/* Mechanician Skills */
		case CHARACTERCLASS_Mechanician:
			switch ( psSkill->iID )
			{
				case SKILLID_Rupture:
					return RUPTUREHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

		/* Archer Skills */
		case CHARACTERCLASS_Archer:
			switch( psSkill->iID )
			{
			case SKILLID_FierceWind:
				return FIERCEWINDHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
				break;
			}
			break;

		/* Priestess Skills */
		case CHARACTERCLASS_Priestess:
			switch ( psSkill->iID )
			{
				case SKILLID_VirtualLife:
					return VIRTUALLIFEHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
				case SKILLID_IceElemental:
					return ICEELEMENTALHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

		/* Atalanta Skills */
		case CHARACTERCLASS_Atalanta:
			switch( psSkill->iID )
			{
				case SKILLID_SummonTiger:
					return SUMMONTIGERHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

		/* Magician Skills */
		case CHARACTERCLASS_Magician:
			switch ( psSkill->iID )
			{
				case SKILLID_EnchantWeapon:
					return ENCHANTWEAPONHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
				case SKILLID_StoneSkin:
					return FLAMEBALLHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
				case SKILLID_RedRay:
					return REDRAYHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
				case SKILLID_WizardTrance:
					break;
			}
			break;

		/* Assassin Skills */
		case CHARACTERCLASS_Assassin:
			switch ( psSkill->iID )
			{
				case SKILLID_ShadowBlink:
					return SHADOWBLINKHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

		/* Shaman Skills */
		case CHARACTERCLASS_Shaman:
			switch ( psSkill->iID )
			{
				case SKILLID_Creed:
					return CREEDHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;

				case SKILLID_PressDeity:
					return PRESSDEITYHANDLER->OnCastAnimationStart( psSkill, pcUnitData );
					break;
			}
			break;

	}
	return false;
}

bool SkillManager::OpenPlaySkill( struct Skill * psSkill )
{
	if ( CanUseSkill( psSkill ) == FALSE )
		return false;

	//Use Skill
	WRITEDWORD( 0x03673B78, (DWORD)psSkill );

	switch( UNITDATA->sCharacterData.iClass )
	{
		/* Archer Skills */
		case CHARACTERCLASS_Archer:
			switch( psSkill->iID )
			{
				case SKILLID_LethalSight:
					return LETHALSIGHTHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
					break;
			}
			break;

		/* Fighter Skills */
		case CHARACTERCLASS_Fighter:
			switch ( psSkill->iID )
			{
				case SKILLID_SeismicImpact:
					break;
			}
			break;

		/* Mechanician Skills */
		case CHARACTERCLASS_Mechanician:
			switch( psSkill->iID )
			{
				case SKILLID_Rupture:
					return false;
					break;
				case SKILLID_MagneticDischarge:
					return MAGNETICDISCHARGEHANDLER->OnCastAnimationStart( psSkill );
					break;
			}
			break;

		/* Knight Skills */
		case CHARACTERCLASS_Knight:
			switch ( psSkill->iID )
			{
				case SKILLID_HolyConviction:
					return HOLYCONVICTIONHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
					break;

				case SKILLID_WhirlWind:
					return WHIRLWINDHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
					break;
			}
			break;

		/* Priestess Skills */
		case CHARACTERCLASS_Priestess:
			switch ( psSkill->iID )
			{
				case SKILLID_VirtualLife:
					return VIRTUALLIFEHANDLER->OnCastAnimationStart( psSkill );
					break;
				case SKILLID_IceElemental:
					return ICEELEMENTALHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
					break;
				case SKILLID_GrandHealing:
					break;

				case SKILLID_IceMeteorite:
					return ICEMETEORITE->OnCastAnimationStart( psSkill, UNITDATA );
					break;
				default:
					break;
			}
			break;

		/* Assassin Skills */
		case CHARACTERCLASS_Assassin:
			switch( psSkill->iID )
			{
				case SKILLID_KissViper:
					return KISSVIPERHANDLER->OnCastAnimationStart( psSkill );
					break;

				default:
					break;
			}
			break;

		/* Atalanta Skills */
		case CHARACTERCLASS_Atalanta:
			switch( psSkill->iID )
			{
				case SKILLID_SummonTiger:
					return SUMMONTIGERHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
					break;
			}
			break;

		/* Magician Skills */
		case CHARACTERCLASS_Magician:
			switch ( psSkill->iID )
			{
				case SKILLID_EnchantWeapon:
					return ENCHANTWEAPONHANDLER->OnCastAnimationStart( psSkill );
					break;
				case SKILLID_StoneSkin:
//					return STONESKINHANDLER->OnCast( psSkill, UNITDATA );
					break;
				case SKILLID_WizardTrance:
					return WIZARDTRANCEHANDLER->OnCastAnimationStart( psSkill, UNITDATA );
					break;
			}
			break;
		/* Shaman Skills */
		case CHARACTERCLASS_Shaman:
			switch ( psSkill->iID )
			{
				case SKILLID_Creed:
					return CREEDHANDLER->OnCastAnimationStart( psSkill );
					break;

				case SKILLID_ChasingHunt:
					return CHASINGHUNTHANDLER->OnCastAnimationStart( psSkill );
					break;
			}
			break;
	}
	return false;
}

BOOL SkillManager::HandleDebuffSkill( UnitData * pcUnitData, int iID, int iValue )
{
	switch ( pcUnitData->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Pikeman:
			switch ( iID )
			{
				case SKILLPLAYID_T51:
				{
					if ( UNITDATA->iStunTimeLeft <= 0 )
						UNITDATA->iStunTimeLeft = iValue;
				} break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;

		case CHARACTERCLASS_Magician:
			switch ( iID )
			{
				case SKILLPLAYID_T43:
				{
					if ( UNITDATA->iFreezeTimeLeft <= 0 )
					{
						int iTime = iValue;
						UNITDATA->iFreezeTimeLeft = iTime;
						UNITDATA->iUpdateMoveReduce = 0xD2;
						UNITDATA->bDistortion = (BOOL)UNITDATA->iFreezeTimeLeft;
						UNITDATA->iIceOverlay = 0;
					}
				} break;
			}
			break;

		case CHARACTERCLASS_Shaman:
			switch ( iID )
			{
				case SKILLPLAYID_T13:
				{
					if ( UNITDATA->iFreezeTimeLeft <= 0 )
					{
						int iTime = iValue;
						UNITDATA->iFreezeTimeLeft = iTime;
						UNITDATA->iUpdateMoveReduce = 0xD2;
						UNITDATA->iCurseLazy = (BOOL)UNITDATA->iFreezeTimeLeft;
						UNITDATA->iIceOverlay = 0;
					}
				} break;
				case SKILLPLAYID_T22:
				{
					if ( UNITDATA->iStunTimeLeft <= 0 )
						UNITDATA->iStunTimeLeft = iValue;
				} break;
			}
			break;

		default:
			break;
	}

	return TRUE;
}

int SkillManager::GetSkillRange( Skill * psSkill, int iAttackRange, int iAttackMode )
{
	int iRange = 0;

	switch ( psSkill->iID )
	{
		case SKILLID_ShadowBlink:
			if ( psSkill->iLevel )
				iRange = iaASNT51Range[psSkill->iLevel - 1] << 8;
			break;

		case SKILLID_Scratch:
		case SKILLID_PressDeity:
			if ( psSkill->iLevel )
				iRange = iAttackRange;
			break;

		default:
			break;
	}

	return iRange;
}

BOOL SkillManager::EventSkill( UnitData * pcUnitData )
{
	switch ( pcUnitData->sCharacterData.iClass )
	{
		case CHARACTERCLASS_Archer:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T52:
					return FIERCEWINDHANDLER->OnEvent( pcUnitData );
					break;
			}
			break;
		case CHARACTERCLASS_Pikeman:
			switch( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T51:
					return RINGOFSPEARSHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T52:
					return LASTBREATHHANDLER->OnEvent( pcUnitData );
					break;
			}
			break;

		case CHARACTERCLASS_Knight:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T52:
					return WHIRLWINDHANDLER->OnEvent( pcUnitData );
					break;
			}
			break;

		case CHARACTERCLASS_Priestess:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T24:
					if( pcUnitData == UNITDATA )
					{
						for( size_t i = 0; i < MAX_PARTY_MEMBERS; i++ )
						{
							UnitData * p = UNITGAME->GetUnitDataByID( PARTYHANDLER->GetPartyData().saMembers[i].sData.iID );
							if( p )
							{
								if( p->bActive && p->psModelAnimation->iType != ANIMATIONTYPE_Die )
								{
									int iX = ( UNITDATA->sPosition.iX - p->sPosition.iX ) >> 8;
									int iY = ( UNITDATA->sPosition.iY - p->sPosition.iY ) >> 8;
									int iZ = ( UNITDATA->sPosition.iZ - p->sPosition.iZ ) >> 8;
									int iDistance = iX*iX + iY*iY + iZ*iZ;

									if( iDistance < (300 * 300) )
										CALL_WITH_ARG1( 0x0053BE20,(DWORD)p );
								}
							}
						}
					}
					break;
				case SKILLPLAYID_T32:
					if ( pcUnitData == UNITDATA )
						return RESSURECTIONHANDLER->OnUseSkill( pcUnitData );
					break;
				case SKILLPLAYID_T42:
					if( pcUnitData == UNITDATA )
					{
					//	if( GetForegroundWindow() != DX::Graphic::GetWindowHandler() )
					//		ResetSkillTimer( SKILLID_RegenerationField );
					}
					else
					{
						if ( pcUnitData->sActiveSkill.cLevel > 0 )
						{
							EffectGame::RegenerationFieldCelestial( pcUnitData, 2.0f );

							//Is not Party or Raid member? Don't use skill
							if ( (PARTYHANDLER->IsPartyMember( pcUnitData->iID ) == FALSE) && (PARTYHANDLER->IsRaidMember( pcUnitData->iID ) == FALSE) )
								return TRUE;
						}
					}
					break;

				case SKILLPLAYID_T52:
					return ICEMETEORITE->OnEvent( pcUnitData );
					break;
			}
			break;

		/* Fighter Skills */
		case CHARACTERCLASS_Fighter:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T51:
					return SESMICIMPACTHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;
		/* Mechanician Skills */
		case CHARACTERCLASS_Mechanician:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T41:
					return IMPULSIONHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T51:
					return RUPTUREHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T52:
					return MAGNETICDISCHARGEHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
		/* Magician Skills */
		case CHARACTERCLASS_Magician:
			switch( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T51:
					return WIZARDTRANCEHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T52:
					return FLAMEBALLHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T53:
					return TRUE;
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
			/* Assassin Skills */
		case CHARACTERCLASS_Assassin:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T51:
					return SHADOWBLINKHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					return TRUE;
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
			/* Assassin Skills */
		case CHARACTERCLASS_Shaman:
			switch ( pcUnitData->sActiveSkill.cSkillTinyID )
			{
				case SKILLPLAYID_T52:
					return PRESSDEITYHANDLER->OnEvent( pcUnitData );
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
	}
	return FALSE;
}

BOOL SkillManager::BeginSkillMonster( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Acero:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "AceroStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Chalybs:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ChalybsStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Greedy:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GreedyStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Arhdyra:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ArhdyraStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Dey:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "DeyStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Faugn:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FaugnStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Gurkob:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GurkobStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Koon:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "KoonStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Sathla:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "SathlaStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Yagditha:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "YagdithaStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaArmor:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaArmorStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaArcher:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaArcherStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaGiant:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaGiantStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_FlameMaiden:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FlamemaidenStart", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		default:
			break;
	}

	if ( bRet == TRUE )
	{
		PLAYUNITSOUND( pcUnitData );

		pcUnitData->UpdateAttack();
	}


	return bRet;
}

BOOL SkillManager::EventSkillMonster( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Acero:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "AceroSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Chalybs:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ChalybsSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Greedy:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GreedySkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_IElite:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "IEliteSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_ISoldier:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ISoldierSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Nihil:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "NihilSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Arhdyra:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "ArhdyraSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Dey:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "DeySkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Faugn:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FaugnSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Gurkob:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GurkobSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Koon:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "KoonSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Sathla:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "SathlaSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Yagditha:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "YagdithaSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Igniculus:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "IgniculusSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaArmor:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaArmorSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaGolem:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaGolemSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaArcher:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaArcherSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaGiant:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaGiantSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Insec:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "InsecSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Flamo:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FlamoSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_FlameMaiden:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "FlamemaindenSkill", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_IceElemental:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				CGameSkill::FrozenMistSkillHit( pcUnitData, pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_TropicalRanger:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				CGameSkill::GoblinShamanSkill( pcUnitData, 200 );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_AncientGuardian:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				PacketFireMeteorite sPacket;
				sPacket.iLength = sizeof( PacketFireMeteorite );
				sPacket.iHeader = PKTHDR_FireMeteorite;
				sPacket.bSmallMeteor = FALSE;
				sPacket.iDelay = 5;
				sPacket.sBegin = pcUnitData->sPosition;
				sPacket.sBegin.iY += 150000;
				sPacket.sEnd = pcUnitData->sPosition;
				sPacket.iCount = RandomI( 2, 3 );

				for ( int i = 0; i < sPacket.iCount; i++ )
				{
					sPacket.iDelay += 40;
					sPacket.sEnd = pcUnitData->pcTarget->sPosition;
					sPacket.sEnd.iX += RandomI( -32, 32 ) << 8;
					sPacket.sEnd.iZ += RandomI( -32, 32 ) << 8;
					EffectGame::FireMeteorite( &sPacket.sBegin, &sPacket.sEnd, sPacket.iDelay, sPacket.bSmallMeteor );
				}
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_DarkGuardian:
			if ( pcUnitData->psModelAnimation )
			{
				CGameSkill::DarkGuardianSkillHit( pcUnitData );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Yeti:
			if ( pcUnitData->psModelAnimation )
			{
				CGameSkill::IceGolemHit( pcUnitData );
				bRet = TRUE;
			}
			break;
		default:
			break;
	}

	if ( bRet == FALSE )
	{
		if ( STRINGCOMPARE( pcUnitData->sCharacterData.szName, "Cursed Sand" ) )
		{
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				if ( pcUnitData->psModelAnimation->uaValue[0] == 'J' )
				{
					if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\CursedSand.efk" ) )
					{
						pcScript->SetID( PARTICLEID_MONSTERS_CURSED_DESERT + 7 );

						auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
						pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
						pcOwner->SetBiped( "Bip01 00" );
						pcScript->SetOwner( pcOwner );

						EFFEKSEERHANDLER->AddScript( pcScript );

						bRet = TRUE;
					}
				}
			}
		}
		if ( STRINGCOMPARE( pcUnitData->sCharacterData.szName, "Volcanos" ) || STRINGCOMPARE( pcUnitData->sCharacterData.szName, "Mini Volcanos" ) )
		{
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				if ( pcUnitData->psModelAnimation->uaValue[0] == 'J' )
				{
					if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\VolcanosSkill1.efk" ) )
					{
						pcScript->SetID( PARTICLEID_MONSTERS_CURSED_DESERT + 4 );

						auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
						pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
						pcOwner->SetBiped( "Bip01 00" );
						pcScript->SetOwner( pcOwner );

						EFFEKSEERHANDLER->AddScript( pcScript );

						bRet = TRUE;
					}
				}
				if ( pcUnitData->psModelAnimation->uaValue[0] == 'K' )
				{
					if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\VolcanosSkill2.efk" ) )
					{
						pcScript->SetID( PARTICLEID_MONSTERS_CURSED_DESERT + 5 );

						auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
						pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
						pcOwner->SetBiped( "Bip01 54" );
						pcScript->SetOwner( pcOwner );

						EFFEKSEERHANDLER->AddScript( pcScript );

						bRet = TRUE;
					}
				}
				if ( pcUnitData->psModelAnimation->uaValue[0] == 'L' )
				{
					if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\VolcanosSkill3.efk" ) )
					{
						pcScript->SetID( PARTICLEID_MONSTERS_CURSED_DESERT + 8 );

						auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData );
						pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
						pcOwner->SetBiped( "Bip01 33" );
						pcScript->SetOwner( pcOwner );

						EFFEKSEERHANDLER->AddScript( pcScript );

						bRet = TRUE;
					}
				}
			}
		}
		if (STRINGCOMPARE(pcUnitData->sCharacterData.szName, "Hagios") || STRINGCOMPARE(pcUnitData->sCharacterData.szName, "Mini Hagios") )
		{
			if (pcUnitData->pcTarget && pcUnitData->psModelAnimation)
			{
				if (pcUnitData->psModelAnimation->uaValue[0] == 'L')
				{
					if (auto pcScript = EFFEKSEERHANDLER->LoadScript("game\\scripts\\particles\\HagiosSkill2.efk"))
					{
						pcScript->SetID(PARTICLEID_MONSTERS_CURSED_DESERT + 9 );

						auto pcOwner = new Engine::ObjectOwnerUnit(pcUnitData);
						pcOwner->AddAnimationTypeLostCondition(ANIMATIONTYPE_Die);
						pcOwner->SetBiped("Bip01 00");
						pcScript->SetOwner(pcOwner);

						EFFEKSEERHANDLER->AddScript(pcScript);

						bRet = TRUE;
					}
				}
				if (pcUnitData->psModelAnimation->uaValue[0] == 'J')
				{
					if (auto pcScript = EFFEKSEERHANDLER->LoadScript("game\\scripts\\particles\\HagiosSkill3.efk"))
					{
						pcScript->SetID(PARTICLEID_MONSTERS_CURSED_DESERT + 5);

						auto pcOwner = new Engine::ObjectOwnerUnit(pcUnitData);
						pcOwner->AddAnimationTypeLostCondition(ANIMATIONTYPE_Die);
						pcOwner->SetBiped("Bip01 39");
						pcScript->SetOwner(pcOwner);

						EFFEKSEERHANDLER->AddScript(pcScript);

						bRet = TRUE;
					}
				}
				
			}
		}

	}

	if ( bRet == TRUE )
	{
		PLAYUNITSOUND( pcUnitData );

		pcUnitData->UpdateAttack();
	}

	return bRet;
}

BOOL SkillManager::EventHitMonster( UnitData * pcUnitData )
{
	BOOL bRet = FALSE;

	switch ( pcUnitData->sCharacterData.iMonsterEffectID )
	{
		case MONSTEREFFECTID_Acero:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "AceroHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Chalybs:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "ChalybsHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Greedy:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "GreedyHit1", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			else if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'J') )
			{
				PARTICLE->StartEffectParticleSkill( "GreedyHit2", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_IElite:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "IEliteHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Nihil:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "NihilHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Arhdyra:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "ArhdyraHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Gurkob:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "GurkobHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Dey:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "DeyHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Koon:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "KoonHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Faugn:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "FaugnHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Sathla:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "SathlaHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_Yagditha:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				if ( pcUnitData->psModelAnimation->uaValue[0] == 'H' )
				{
					PARTICLE->StartEffectParticleSkill( "YagdithaHit1", pcUnitData->pcTarget );
					bRet = TRUE;
				}
				else if ( pcUnitData->psModelAnimation->uaValue[0] == 'K' )
				{
					PARTICLE->StartEffectParticleSkill( "YagdithaHit2", pcUnitData->pcTarget );
					bRet = TRUE;
				}
			}
			break;

		case MONSTEREFFECTID_Igniculus:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "IgniculusHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaArmor:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaArmorHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaArcher:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaArcherHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaGolem:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaGolemHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_LavaGiant:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "LavaGiantHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Insec:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "InsecHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_Flamo:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "FlamoHit1", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_FlameMaiden:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation && (pcUnitData->psModelAnimation->uaValue[0] == 'H') )
			{
				PARTICLE->StartEffectParticleSkill( "FlamemaidenHit", pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;

		case MONSTEREFFECTID_IceElemental:
			if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
			{
				CGameSkill::FrozenMistHit( pcUnitData, pcUnitData->pcTarget );
				bRet = TRUE;
			}
			break;
		case MONSTEREFFECTID_DarkGuardian:
			if ( pcUnitData->psModelAnimation )
			{
				CGameSkill::DarkGuardianHit( pcUnitData );
				bRet = TRUE;
			}
			break;
		default:
			break;
	}

	if ( bRet == FALSE )
	{
		if ( pcUnitData->pcTarget && pcUnitData->psModelAnimation )
		{
			if ( STRINGCOMPARE( pcUnitData->sCharacterData.szName, "Cursed Golem" ) )
			{
				if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\CursedGolem.efk" ) )
				{
					pcScript->SetID( PARTICLEID_MONSTERS_CURSED_DESERT );

					auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData->pcTarget );
					pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
					pcScript->SetOwner( pcOwner );

					EFFEKSEERHANDLER->AddScript( pcScript );

					bRet = TRUE;
				}
			}
			else if ( STRINGCOMPARE( pcUnitData->sCharacterData.szName, "Cursed Witch" ) )
			{
				if ( auto pcScript = EFFEKSEERHANDLER->LoadScript( "game\\scripts\\particles\\CursedWitch.efk" ) )
				{
					pcScript->SetID( PARTICLEID_MONSTERS_CURSED_DESERT + 1 );

					auto pcOwner = new Engine::ObjectOwnerUnit( pcUnitData->pcTarget );
					pcOwner->AddAnimationTypeLostCondition( ANIMATIONTYPE_Die );
					pcScript->SetOwner( pcOwner );

					EFFEKSEERHANDLER->AddScript( pcScript );

					bRet = TRUE;
				}
			}
		}
	}

	if ( bRet == TRUE )
	{
		PLAYUNITSOUND( pcUnitData );

		pcUnitData->UpdateAttack();
	}

	return bRet;
}

BOOL SkillManager::EndSkill( class UnitData * pcUnitData )
{
	switch ( pcUnitData->sCharacterData.iClass )
	{
		/* Priestess Skills */
		case CHARACTERCLASS_Archer:
			switch ( pcUnitData->sActiveSkill.iValue & 0xFF )
			{
				case SKILLPLAYID_T52:
					return FIERCEWINDHANDLER->OnAnimationEnd( pcUnitData );
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;

		case CHARACTERCLASS_Knight:
			switch ( pcUnitData->sActiveSkill.iValue & 0xFF )
			{
				case SKILLPLAYID_T52:
					return WHIRLWINDHANDLER->OnAnimationEnd( pcUnitData );
					break;
			}
			break;

	}
	
	return FALSE;
}

BOOL SkillManager::SkillAnimationStart( class UnitData * pcThis, int iSkillID, int iLevel, class UnitData * pcUnitData, int iX, int iY, int iZ )
{
	if ( pcUnitData == NULL )
		pcUnitData = pcThis;

	if ( pcThis->sCharacterData.iType == CHARACTERTYPE_Monster )
	{
		if ( pcThis->iCharacterType != CHARACTERTYPE_Player )
		{
			if ( SKILLMANAGERHANDLER->BeginSkillMonster( pcThis ) )
				return TRUE;
		}
	}

	__asm
	{
		PUSH iZ;
		PUSH iY;
		PUSH iX;
		PUSH pcUnitData;
		PUSH iLevel;
		PUSH iSkillID;
		MOV ECX, pcThis;
		MOV EAX, 0x00418CD4;
		CALL EAX
	}

	switch( pcThis->sCharacterData.iClass )
	{
		/* Priestess Skills */
		case CHARACTERCLASS_Priestess:
			switch( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:

					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					MIRACULOUSLIGHTHANDLER->OnCastAnimationStart( pcThis );
					break;
			}
			break;
		/* Pikeman Skills */
		case CHARACTERCLASS_Pikeman:
			switch( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:
					return RINGOFSPEARSHANDLER->OnBeginSkillUse( pcThis, pcUnitData, iX, iY, iZ );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
		/* Archer Skills */
		case CHARACTERCLASS_Archer:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:
					
					break;
				case SKILLPLAYID_T52:
					return FIERCEWINDHANDLER->OnCastMember( pcThis );
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:
					break;
			}
			break;
		/* Fighter Skills */
		case CHARACTERCLASS_Fighter:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:

					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;
		/* Knight Skills */
		case CHARACTERCLASS_Knight:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T12:
				case SKILLPLAYID_T21:
					ResetSkillTimer( SKILLID_HolyConviction );
					HOLYCONVICTIONHANDLER->OnCastEnd( UNITDATATOUNIT( pcUnitData ) );
					break;
				case SKILLPLAYID_T51:
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;

			/* Assassin Skills */
		case CHARACTERCLASS_Assassin:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:
					return KISSVIPERHANDLER->OnBeginSkillUse( pcThis, pcUnitData, iX, iY, iZ );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;

		/* Shaman Skills */
		case CHARACTERCLASS_Shaman:
			switch ( iSkillID & 0xFF )
			{
				case SKILLPLAYID_T51:
					return CREEDHANDLER->OnBeginSkillUse( pcThis, pcUnitData, iX, iY, iZ );
					break;
				case SKILLPLAYID_T52:
					break;
				case SKILLPLAYID_T53:
					break;
				case SKILLPLAYID_T54:

					break;
			}
			break;
	}
	return FALSE;
}

void SkillManager::OnEffect()
{
}

void SkillManager::LoadParticles()
{
	SKILLEFFECTHANDLER->GetLethalSight()->LoadParticle();
	SKILLEFFECTHANDLER->GetFierceWind()->LoadParticle();
	SKILLEFFECTHANDLER->GetSesmicImpact()->LoadParticle();
	SKILLEFFECTHANDLER->GetStoneSkin()->LoadParticle();
	SKILLEFFECTHANDLER->GetHolyConviction()->LoadParticle();
	SKILLEFFECTHANDLER->GetRupture()->LoadParticle();
	SKILLEFFECTHANDLER->GetWizardTrance()->LoadParticle();
	SKILLEFFECTHANDLER->GetRingOfSpears()->LoadParticle();
	SKILLEFFECTHANDLER->GetShadowBlink()->LoadParticle();
}

int SkillManager::FormatSkillTimerInfo( Skill * pSkill )
{
	return TIMERSKILLHANDLER->OnRenderTimer( pSkill );
}

BOOL SkillManager::ResetForceOrb()
{
	Skill * psaSkill = (Skill*)0x036789F8;
	for ( int i = 0; i < 20; i++ )
	{
		if ( (psaSkill[i].iID & 0xFFFF0000) == 0x03060000 )
		{
			ZeroMemory( &psaSkill[i], sizeof( Skill ) );
			return TRUE;
		}
	}
	return FALSE;
}

void SkillManager::ReformSkill( Skill * psSkill, int iSkill )
{
	typedef void( __thiscall *tfnReformSkill )(DWORD dwClass, Skill * psSkill, int iSkill);
	tfnReformSkill fnReformSkill = (tfnReformSkill)0x004EC0B0;
	fnReformSkill( 0x0367E148, psSkill, iSkill );
}

BOOL SkillManager::OnMouseClick( class CMouse * pcMouse )
{
	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownR && *(int*)(0x0367DE8C) )
	{
		int iID = *(int*)(0x0367DE8C) - 1;
		if ( sTempSkillTimerData[iID].iID == 0x09010001 )
		{
			// Show Box to accept the quest
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Are you sure?" );
			MESSAGEBOX->SetDescription( "Do you want to cancel the Force Orb?" );
			MESSAGEBOX->SetEvent( std::bind( &SkillManager::CancelForceOrb, this, *(int*)(0x0367DE8C) - 1 ) );
			MESSAGEBOX->Show();

			return TRUE;
		}
		if ( sTempSkillTimerData[iID].iID == 0x096A0003 || sTempSkillTimerData[iID].iID == 0x096B0003 )
		{
			// Show Box to accept the quest
			MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
			MESSAGEBOX->SetTitle( "Are you sure?" );
			MESSAGEBOX->SetDescription( "Do you want to cancel the Active Quest?" );
			MESSAGEBOX->SetEvent( std::bind( &CQuestGame::HandleCancelQuest, CQUESTGAME, sTempSkillTimerData[iID].iID, sTempSkillTimerData[iID].PlusState[4] ) );
			MESSAGEBOX->Show();

			return TRUE;
		}
	}
	return FALSE;
}

BOOL SkillManager::OnMouseClickSkillPet()
{
	int iID = *(int*)(0x0367DE8C) - 1;
	if ( iID >= 0 )
	{
		for ( int i = 0; i < _countof( iaSkillPet ); i++ )
		{
			if ( sTempSkillTimerData[iID].iID == iaSkillPet[i] )
			{
				baPetAggressive[i] = baPetAggressive[i] ? FALSE : TRUE;
				
				if ( i <= 4 )
					WRITEDWORD( (0x0367E940 + (i*4)), baPetAggressive[i] );

				CALL_WITH_ARG4( 0x00628E60, iaSkillPet[i], baPetAggressive[i], 0, 0 );
				break;
			}
		}
	}
	return TRUE;
}

BOOL SkillManager::IsAgressive( int iSkillID )
{
	for ( int i = 0; i < _countof( iaSkillPet ); i++ )
	{
		if ( iaSkillPet[i] == iSkillID )
			return baPetAggressive[i];
	}

	return FALSE;
}

void SkillManager::ResetSkillEffectParticle( DWORD dwSkillID )
{
	struct EffectStruct
	{
		DWORD				dwID;
		char				cPadding[0x1B4];
	};

	EffectStruct * ps = (EffectStruct*)0x0373F8F8;

	for ( int i = 0; i < 1000; i++ )
	{
		if ( ps[i].dwID != 0 )
		{
			if ( ps[i].dwID == dwSkillID )
			{
				ZeroMemory( &ps[i], 0x1B8 );
			}
		}
	}

	struct AssaEffect
	{
		char				cPadding[0xC];
		DWORD				dwID;
		int					iCurTime;
		int					iTotalTime;
	};

	AssaEffect	** pASSA = (AssaEffect**)0x37B80F0;
	for ( int i = 0; i < 5000; i++ )
	{
		AssaEffect * p = pASSA[i];
		if ( p )
		{
			if ( p->dwID == dwSkillID )
			{
				p->iCurTime = p->iTotalTime;
			}
		}
	}
}


void SkillManager::ResetBuff( DWORD dwSkillID, UnitData * pcUnitData )
{
	BOOL bUpdate = FALSE;

	Unit * pcUnit = UNITDATATOUNIT( pcUnitData );

	if ( SKILLMANAGERHANDLER->IsActiveBuff( dwSkillID ) )
	{
		switch ( dwSkillID )
		{
			case SKILLID_WhirlWind:
				WHIRLWINDHANDLER->OnCastEnd( pcUnit );
				bUpdate = TRUE;
				break;
			case SKILLID_WizardTrance:
				WIZARDTRANCEHANDLER->OnCastEnd( pcUnit );
				bUpdate = TRUE;
				break;
			case SKILLID_LethalSight:
				LETHALSIGHTHANDLER->OnCastEnd( pcUnit );
				bUpdate = TRUE;
				break;
			case SKILLID_Rainmaker:
			case SKILLID_DancingSword:
			case SKILLID_SpiritElemental:
			case SKILLID_Zenith:
			case SKILLID_EnchantWeapon:
			case SKILLID_HolyReflection:
				if ( dwSkillID == SKILLID_EnchantWeapon )
				{
					if ( UNITDATA_SELECTED == pcUnitData )
						return;
					if ( UNITDATA == pcUnitData && UNITDATA_SELECTED )
						return;

					ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponFire );
					ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponIce );
					ResetSkillEffectParticle( SKILLEFFECTID_EnchantWeaponLightning );
				}
				ResetSkillEffectParticle( dwSkillID );
				bUpdate = TRUE;
				break;
			case SKILLID_AdventMigal:
			case SKILLID_AdventMidranda:
			case SKILLID_SummonMuspell:
				if ( *(DWORD*)(0x4B07A00) && dwSkillID != SKILLID_AdventMidranda )
				{
					CALLT_WITH_ARG1( 0x005C9590, *(DWORD*)(0x4B07A00), 6 );
					*(DWORD*)(0x4B07A00) = 0;
				}
				if ( *(DWORD*)(0x4B07A04) && dwSkillID == SKILLID_AdventMidranda )
				{
					CALLT_WITH_ARG1( 0x005C9590, *(DWORD*)(0x4B07A04), 6 );
					*(DWORD*)(0x4B07A04) = 0;
				}
				bUpdate = TRUE;
				break;

			default:
				break;
		}

		if ( bUpdate )
		{
			if ( dwSkillID == SKILLID_EnchantWeapon )
			{
				if ( UNITDATA_SELECTED == pcUnitData )
					return;
				if ( UNITDATA == pcUnitData && UNITDATA_SELECTED )
					return;
			}

			ResetSkillTimer( dwSkillID );
		}
	}

}

void SkillManager::StartSkillEffect( Point3D sPosition, Point3D sTargetPosition, int iSkillID, int iLevel )
{
	CALL_WITH_ARG8( pfnStartSkillEffect, sPosition.iX, sPosition.iY, sPosition.iZ, sTargetPosition.iX, sTargetPosition.iY, sTargetPosition.iZ, iSkillID, iLevel );
}

NAKED bool SkillManager::GetWeaponPosition( UnitData * pcUnitData, Point3D * psPositionOut, float fU )
{
	JMP( pfnGetWeaponPosition );
}

BOOL SkillManager::GetWeaponPositionNew( UnitData * pcUnitData, Point3D & sPositionOut, float fScaleSize )
{
	EXEMatrixI & sMatrix = pcUnitData->sRightHandMatrix;
	
	//Get Weapon Right
	if ( pcUnitData->sRightHandTool.eItemID != 0 )
	{
		sMatrix			= pcUnitData->sRightHandMatrix;
		sPositionOut = Point3D( 0, 0, pcUnitData->sRightHandTool.iSizeMax - (int)((float)pcUnitData->sRightHandTool.iSizeMax * fScaleSize) );
		
		if ( ItemID( pcUnitData->sRightHandTool.eItemID ).ToItemType() == ITEMTYPE_Javelin )
			sPositionOut = Point3D( 0, 0, -pcUnitData->sRightHandTool.iSizeMax + (int)((float)pcUnitData->sRightHandTool.iSizeMax * fScaleSize) );
	}
	else
	{
		//Get Weapon Left
		if ( pcUnitData->sLeftHandTool.eItemID != 0 )
		{
			sMatrix			= pcUnitData->sLeftHandMatrix;
			sPositionOut = Point3D( 0, 0, pcUnitData->sLeftHandTool.iSizeMax - (int)((float)pcUnitData->sLeftHandTool.iSizeMax * fScaleSize) );
			
			if ( ItemID( pcUnitData->sLeftHandTool.eItemID ).ToItemType() == ITEMTYPE_Javelin )
				sPositionOut = Point3D( 0, 0, -pcUnitData->sLeftHandTool.iSizeMax + (int)((float)pcUnitData->sLeftHandTool.iSizeMax * fScaleSize) );
		}
		else
			return FALSE;
	}

	//Radian
	Point3D sRadian;
	sRadian.iX		= sPositionOut.iX * sMatrix.i[0][0] + sPositionOut.iY * sMatrix.i[1][0] + sPositionOut.iZ * sMatrix.i[2][0];
	sRadian.iY		= sPositionOut.iX * sMatrix.i[0][1] + sPositionOut.iY * sMatrix.i[1][1] + sPositionOut.iZ * sMatrix.i[2][1];
	sRadian.iZ		= sPositionOut.iX * sMatrix.i[0][2] + sPositionOut.iY * sMatrix.i[1][2] + sPositionOut.iZ * sMatrix.i[2][2];

	//Get Weapon Position
	sPositionOut.iX	= (pcUnitData->sPosition.iX + (sRadian.iX >> 8) + sMatrix.i[3][0]);
	sPositionOut.iZ	= (pcUnitData->sPosition.iZ + (sRadian.iY >> 8) + sMatrix.i[3][1]);
	sPositionOut.iY	= (pcUnitData->sPosition.iY + (sRadian.iZ >> 8) + sMatrix.i[3][2]);

	return TRUE;
}

void SkillManager::HandlePacket( PacketSkillPartyData * psPacket )
{
	switch ( psPacket->iClass )
	{
		case CHARACTERCLASS_Mechanician:
			//Holy Valor??
			if ( psPacket->iSkillID == SKILLPLAYID_T52 )
			{
				MAGNETICDISCHARGEHANDLER->OnCastEffect( UNIT, psPacket->iLevel, psPacket->iParam1 );

				MAGNETICDISCHARGEHANDLER->CastMembers( psPacket );
			}
			break;



		case CHARACTERCLASS_Knight:
			//Holy Valor??
			if ( psPacket->iSkillID == 0x2A )
			{
				Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_HolyValor );
				if ( psSkill && psSkill->iUseTime > (70 * 3) )
				{
					ResetSkillEffectParticle( SKILLEFFECTID_HolyValorAction );
					ResetSkillTimer( SKILLID_HolyValor );
					ResetSkillTimer( SKILLID_HolyConviction );
					HOLYCONVICTIONHANDLER->OnCastEnd( UNIT );
				}
			}

			//Holy Conviction?
			if ( psPacket->iSkillID == SKILLPLAYID_T51 )
			{
				HOLYCONVICTIONHANDLER->OnCastEffect( UNIT, psPacket->iLevel, 300.0f );
			}
			break;
		case CHARACTERCLASS_Magician:
			//Holy Valor??
			if ( psPacket->iSkillID == SKILLPLAYID_T22 )
			{
				Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_EnchantWeapon );
				if ( psSkill && psSkill->iUseTime > (70 * 3) )
					ENCHANTWEAPONHANDLER->OnCastEnd( UNITDATA );
				
				ENCHANTWEAPONHANDLER->OnCastEffect( UNIT, psPacket->iLevel, psPacket->iParam1, 300 );

				ENCHANTWEAPONHANDLER->CastMembers( psPacket );
			}
			break;

		case CHARACTERCLASS_Priestess:
			//Holy Valor??
			if ( psPacket->iSkillID == SKILLPLAYID_T34 )
			{
				Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_VirtualLife );
				if ( psSkill && psSkill->iUseTime > (70 * 3) )
					VIRTUALLIFEHANDLER->OnCastEnd( UNITDATA );

				VIRTUALLIFEHANDLER->OnCastEffect( UNIT, psPacket->iLevel, 300 );

				VIRTUALLIFEHANDLER->CastMembers( psPacket );
			}
			break;

		case CHARACTERCLASS_Assassin:
			//Kiss Viper
			if ( psPacket->iSkillID == SKILLPLAYID_T52 )
			{
				Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( SKILLID_KissViper );
				if ( psSkill && psSkill->iUseTime > (70 * 3) )
					KISSVIPERHANDLER->OnCastEnd( UNIT );

				KISSVIPERHANDLER->OnCastEffect( UNIT, psPacket->iLevel, 300, false );
			}
			break;

	}
}

void SkillManager::CancelSkillOnServer( int iID, int iP1, int iP2, int iP3 )
{
	CALL_WITH_ARG4( 0x00628E30, iID, iP1, iP2, iP3 );
}

void SkillManager::ProcessSkillInParty( int iID, int iLevel, int iArea, int iP1, int iP2, int iP3, int iP4 )
{
	PacketSkillPartyData sPacket;
	ZeroMemory( &sPacket, sizeof(PacketSkillPartyData) );
	sPacket.iHeader = PKTHDR_BuffSkillsParty;
	sPacket.iLength = sizeof(PacketSkillPartyData);
	sPacket.iSkillID = iID;
	sPacket.iParam1 = iP1;
	sPacket.iParam2 = iP2;
	sPacket.iParam3 = iP3;
	sPacket.iClass = iP4;
	sPacket.iCount = 0;

	sPacket.iaID[sPacket.iCount++] = UNITDATA->iID;

	if ( PARTYHANDLER->GetPartyData().iMembersCount > 0 )
	{
		std::vector<UnitData*> vUnitData = PARTYHANDLER->GetPartyMembers();
		for ( auto &p : vUnitData )
		{
			if ( p != UNITDATA )
			{
				if ( p && p->bActive )
				{
					int iX = (UNITDATA->sPosition.iX - p->sPosition.iX) >> 8;
					int iY = (UNITDATA->sPosition.iY - p->sPosition.iY) >> 8;
					int iZ = (UNITDATA->sPosition.iZ - p->sPosition.iZ) >> 8;
					int iDistance = iX*iX + iY*iY + iZ*iZ;

					if ( iDistance < (iArea * iArea) )
					{
						sPacket.iaID[sPacket.iCount++] = p->iID;

						if ( sPacket.iCount == _countof( sPacket.iaID ) )
							break;
					}
				}
			}
		}
	}

	sPacket.iClass = UNITDATA->sCharacterData.iClass;
	sPacket.iLevel = iLevel;

	if( SOCKETG )
		SENDPACKETG( &sPacket, TRUE );
}

void SkillManager::DamageUnitHandle( UnitData * pcUnitData, int iPP1, int iPP2, int iAttackState, int iResistance, int iID, BOOL bAttackRatingUpdate )
{
	CALL_WITH_ARG7( 0x004097F0, (DWORD)pcUnitData, iPP1, iPP2, iAttackState, iResistance, iID, bAttackRatingUpdate );
}

void SkillManager::DamageAreaHandle( UnitData * pcUnitData, Point3D & sPosition, int iPP1, int iPP2, int iAttackState, int iResistance, int iID )
{
	CALL_WITH_ARG9( 0x004098D0, sPosition.iX, sPosition.iY, sPosition.iZ, (DWORD)pcUnitData, iPP1, iPP2, iAttackState, iResistance, iID );

}

void SkillManager::SetUnitsRangeRect( UnitData * pcUnitData, RECT & rRect, BOOL bUseAttackRating )
{
	CALL_WITH_ARG3( 0x004085E0, (DWORD)pcUnitData, (DWORD)&rRect, (DWORD)bUseAttackRating );
}

void SkillManager::SetUnitsRange( Point3D & sPosition, int iRange, BOOL bUseAttackRating )
{
	CALL_WITH_ARG5( 0x004085B0, sPosition.iX, sPosition.iY, sPosition.iZ, iRange, bUseAttackRating );
}

void SkillManager::SetUnitsRangeCount( Point3D & sPosition, int iRange, BOOL bUseAttackRating, int iTargetCount )
{
	CALL_WITH_ARG6( 0x00408450, sPosition.iX, sPosition.iY, sPosition.iZ, iRange, bUseAttackRating, iTargetCount );
}

int SkillManager::GetFrameSpeed( int iAttackSpeed )
{
	return CALL_WITH_ARG1( 0x0045F800, iAttackSpeed );
}

int SkillManager::GetAttackFrameSpeed( int iAttackSpeed, int iAddSpeed )
{
	//Skills...
	return CALL_WITH_ARG2( 0x0045F7D0, iAttackSpeed, iAddSpeed );
}

void SkillManager::CheckSkillValidate( Skill * psSkill )
{
	CALLT_WITH_ARG1( 0x004EC230, 0x0367E148, (DWORD)psSkill );
}

void SkillManager::ValidateSkill( Skill * psSkill )
{
	CALLT_WITH_ARG1( 0x004EC2C0, 0x0367E148, (DWORD)psSkill );
}

void SkillManager::SkillEffectLearn( UnitData * pcUnitData )
{
	CALL_WITH_ARG1( 0x0040FBC0, (DWORD)pcUnitData );
}

BOOL SkillManager::OnBuySkill()
{
	if ( CURRENTSKILL )
	{
		//T5?
		if ( (CURRENTSKILL->iID & 0x00FF0000) == 0x00100000 )
		{
			//Locked
			if ( GM_MODE == FALSE )
			{
				if( SKILLDATAHANDLER->IsSkillLocked( CURRENTSKILL->iID ) )
				{
					TITLEBOX( "Skill is Locked!" );
					return TRUE;
				}
			}

			if ( (CHARACTERGAME->GetFSP() > 0) && ((CURRENTSKILL->sSkillInfo.iSkillLevelRequired + (CURRENTSKILL->iLevel * 2)) <= PLAYERDATA->iLevel) )
			{
				//Skill Price
				int * piaPrice = ((int*)0x009BBEA8);
				int * piaPriceP = ((int*)0x009BBE58);

				int iCost = piaPrice[CURRENTSKILL->sSkillInfo.iSkillIndex] + (piaPriceP[CURRENTSKILL->sSkillInfo.iSkillIndex] * CURRENTSKILL->iLevel);

				if ( CHARACTERGAME->CanUseGold( iCost ) && (UNITDATA->sCharacterData.iGold - iCost) > 0 )
				{
					UNITDATA->sCharacterData.iGold -= iCost;

					UPDATE_CHARACTER_CHECKSUM;

					ITEMHANDLER->SendGoldUse( iCost );

					CheckSkillValidate( CURRENTSKILL );
					CURRENTSKILL->iLevel++;
					ValidateSkill( CURRENTSKILL );

					//Decrease FSP
					CHARACTERGAME->SetFSP( CHARACTERGAME->GetFSP() - 1 );

					UPDATE_CHARACTER_CHECKSUM;

					ITEMHANDLER->ProcessGold();
					PLAYMINISOUND( 18 );

					ITEMHANDLER->UpdateItemCharacterStatus();
					ITEMHANDLER->CheckDamageItems();
					SkillEffectLearn();

					int iTalent = PLAYERDATA->iTalent / 3;

					if ( iTalent > 50 )
						iTalent = 50;

					int iLearn = 10000 - (iTalent * 100);

					if ( iLearn >= CURRENTSKILL->UseSkillCount )
						CURRENTSKILL->UseSkillCount /= 2;
					else
						CURRENTSKILL->UseSkillCount = iLearn / 2;

					SAVE;
				}
				else
					TITLEBOX( "Not enough gold" );
			}
			else
				TITLEBOX( "Not enough Regnum Skill Points" );

			return TRUE;
		}
	}

	return FALSE;
}

void SkillManager::UpdateSkillCanBuy()
{
	int iCount = 0;

	switch ( PLAYERDATA->iRank )
	{
		case CHARACTERRANK_Rank1:
			iCount = 5;
			break;

		case CHARACTERRANK_Rank2:
			iCount = 9;
			break;

		case CHARACTERRANK_Rank3:
			iCount = 13;
			break;

		case CHARACTERRANK_Rank4:
			iCount = 17;
			break;

		case CHARACTERRANK_Rank5:
			iCount = 21;
			break;

		default:
			break;
	}

	SKILLS_TIERCOUNT = iCount;

	if ( SKILLS_TIERCOUNT != SKILLS_TIERCOUNT_BACKUP )
	{
		SKILLS_TIERCOUNT_BACKUP = SKILLS_TIERCOUNT;

		ITEMHANDLER->UpdateItemCharacterStatus( FALSE );
	}

	BOOL bCanUpdate = FALSE;

	for ( int i = 0; i < iCount; i++ )
	{
		Skill * ps = SKILLSCHARACTER + i;

		bCanUpdate = FALSE;

		//Tier 1, 2 & 3
		if ( i <= 12 )
		{
			//SP
			if ( READDWORD( 0x03673B70 ) > 0 )
			{
				ps->bActive = TRUE;
				bCanUpdate	= TRUE;
			}
		}
		//Tier 4
		else if ( (i > 12) && (i <= 16) )
		{
			//EP
			if ( READDWORD( 0x03673B74 ) > 0 )
			{
				ps->bActive = TRUE;
				bCanUpdate	= TRUE;
			}
		}
		//Tier 5
		else if ( i > 16 )
		{
			//FSP
			if ( CHARACTERGAME->GetFSP() > 0 )
			{
				ps->bActive = TRUE;
				bCanUpdate	= TRUE;
			}
		}

		ps->bCanUse = FALSE;

		//Can Use Skill
		if ( ps->bActive && bCanUpdate )
		{
			if ( ps->iLevel <= 10 )
			{
				//Not First Skill?
				if ( i > 0 )
				{
					if ( SKILLSCHARACTER[i-1].iLevel > 0 )
					{
						if ( (ps->sSkillInfo.iSkillLevelRequired + (ps->iLevel * 2)) <= PLAYERDATA->iLevel )
							ps->bCanUse = TRUE;
					}
				}
			}
		}
	}
}

void SkillManager::OnClickSkill()
{
	WRITEDWORD( 0x0367DF94, 0 );
	WRITEDWORD( 0x0367DF98, 0 );

	Skill * psaSkills = ((Skill*)0x03670B68);

	if ( SKILL_INDEXBUTTON && SKILL_SHORTCUT_CLICK == FALSE )
	{
		Skill * ps = &psaSkills[SKILL_INDEXBUTTON];

		if ( ((ps->sSkillInfo.eSkillType == SKILLTYPE_Left) || (ps->sSkillInfo.eSkillType == SKILLTYPE_All)) &&
			READDWORD( 0x0367DE9C ) == FALSE && ps->iLevel )
		{
			SKILL_LEFTSIDE = ps;
		}
	}

	if ( SKILL_INDEXBUTTON != 1 && READDWORD( 0x0367DE9C ) == TRUE )
	{
		BOOL bCanBuy = FALSE;

		if ( SKILL_INDEXBUTTON <= 13 )
		{
			//SP
			if ( READDWORD( 0x03673B70 ) > 0 )
				bCanBuy = TRUE;
		}
		//Tier 4
		else if ( (SKILL_INDEXBUTTON > 13) && (SKILL_INDEXBUTTON <= 17) )
		{
			//EP
			if ( READDWORD( 0x03673B74 ) > 0 )
				bCanBuy = TRUE;
		}
		//Tier 5
		else if ( SKILL_INDEXBUTTON > 17 )
		{
			//FSP
			if ( CHARACTERGAME->GetFSP() > 0 )
				bCanBuy = TRUE;
		}

		//Check if previous skill has level
		if ( bCanBuy && (SKILL_INDEXBUTTON > 1) && (psaSkills[SKILL_INDEXBUTTON - 1].iLevel == 0) )
			bCanBuy = FALSE;

		if ( bCanBuy )
		{
			int iRequiredLevel = (psaSkills[SKILL_INDEXBUTTON].sSkillInfo.iSkillLevelRequired + (psaSkills[SKILL_INDEXBUTTON].iLevel * 2));
			if( iRequiredLevel <= PLAYERDATA->iLevel )
			{
				if ( (psaSkills[SKILL_INDEXBUTTON].iLevel >= 0) && (psaSkills[SKILL_INDEXBUTTON].iLevel < 10) )
				{
					CURRENTSKILL = &psaSkills[SKILL_INDEXBUTTON];

					CALLT_WITH_ARG1( 0x004D32D0, 0x0362B210, 0 );

					WRITEDWORD( 0x0367DE90, 0 );
				}
			}
		}
	}
}

void SkillManager::OnEndSkillTimer( Skill * psSkill )
{
	switch ( psSkill->iID )
	{
		case SKILLID_QUEST1:
			if ( (*(short*)0x0362B5B2) > 0 )
			{
				(*(short*)0x0362B5B0) = 0;
				(*(int*)0x0362B5AC)--;
				(*(int*)0x0362B5A8) = 0;
				CALL( 0x0041A280 );
				CTITLEBOX->SetText( 0x37 );
				PLAYMINISOUND( 0x17 );
			}
			break;

		case SKILLID_LethalSight:
			LETHALSIGHTHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_HolyConviction:
			HOLYCONVICTIONHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_WizardTrance:
			WIZARDTRANCEHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_WhirlWind:
			WHIRLWINDHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_MagneticDischarge:
			MAGNETICDISCHARGEHANDLER->OnCastEnd( UNIT );
			break;

		case SKILLID_KissViper:
			KISSVIPERHANDLER->OnCastEnd( UNIT );
			break;

		default:
			break;
	}
}

BOOL SkillManager::OnRankUPButtonClick( int iX, int iY )
{
	BOOL bRet = FALSE;

	//Open?
	if ( READDWORD( 0x0367DE9C ) == TRUE )
	{
		//Inside Button?
		if ( (iX > 248) && (iX < 292) )
		{
			if ( (iY > 310) && (iY < 354) )
			{
				BOOL * baCanRankup = ((BOOL*)0x0362BA6C);

				for ( int i = 0; i < 5; i++ )
				{
					if ( baCanRankup[i] )
					{
						if ( (UNITDATA->sCharacterData.iRank >= CHARACTERRANK_Rank1) && (UNITDATA->sCharacterData.iRank < CHARACTERRANK_Rank4) )
						{
							//Check Quest Rank
							UNITDATA->sCharacterData.iRank = (ECharacterRank)(UNITDATA->sCharacterData.iRank + 1);
							CALLT( 0x004ED7F0, 0x0367E148 );

							CHARACTERGAME->ResetHead( TRUE );

							TITLEBOX( "You Rankup!" );
						}
						else if ( UNITDATA->sCharacterData.iRank == CHARACTERRANK_Rank4 )
						{
							CQUESTGAME->OnQuestT5Start();
						}

						bRet = TRUE;

						break;
					}
				}
			}
		}
	}

	return bRet;
}

void SkillManager::HandlePacket( PacketUseSkillNew * psPacket )
{
	Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->lTargetID ) );

	if ( pcUnit )
	{
		if ( (psPacket->iSkillLevel > 0) && (psPacket->iSkillLevel <= 10) )
		{
			switch ( psPacket->iSkillID )
			{
				case SKILLID_LethalSight:
					LETHALSIGHTHANDLER->OnCastEffect( pcUnit, psPacket->iSkillLevel, 300.0f );
					break;

				case SKILLID_HolyConviction:
					HOLYCONVICTIONHANDLER->OnCastEffect( pcUnit, psPacket->iSkillLevel, 300.0f );
					break;

				case SKILLID_WizardTrance:
					WIZARDTRANCEHANDLER->OnCastEffect( pcUnit, psPacket->iSkillLevel, 300.0f );
					break;

				default:
					break;
			}
		}
	}
}

void SkillManager::HandlePacket( PacketSkillStatus * psPacket )
{
	Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iID ) );
	if ( pcUnit )
	{
		if ( pcUnit->pcUnitData->sPosition.WithinPTDistance( &UNITDATA->sPosition, USER_UPDATERANGE ) )
		{
			//Magnetic Discharge
			if ( pcUnit->bMagneticDischarge )
			{
				//Cast End Time
				if ( psPacket->dwMagneticDischargeTimeLeft == 0 )
				{
					MAGNETICDISCHARGEHANDLER->OnCastEnd( pcUnit );
				}
			}
			else
			{
				//Add New Cast
				if ( psPacket->dwMagneticDischargeTimeLeft > 0 )
				{
					MAGNETICDISCHARGEHANDLER->OnCastEffect( pcUnit, psPacket->sMagneticDischargeLevel, psPacket->dwMagneticDischargeTimeLeft / 1000 );
				}
			}

			//Lethal Sight
			if ( pcUnit->bLethalSight )
			{
				//Cast End Time
				if ( psPacket->dwLethalSightTimeLeft == 0 )
				{
					LETHALSIGHTHANDLER->OnCastEnd( pcUnit );
				}
			}
			else
			{
				//Add New Cast
				if ( psPacket->dwLethalSightTimeLeft > 0 )
				{
					LETHALSIGHTHANDLER->OnCastEffect( pcUnit, psPacket->sLethalSightLevel, (float)psPacket->dwLethalSightTimeLeft / 1000 );
				}
			}

			//Holy Conviction
			if ( pcUnit->bHolyConviction )
			{
				//Cast End Time
				if ( psPacket->dwHolyConvictionTimeLeft == 0 )
				{
					HOLYCONVICTIONHANDLER->OnCastEnd( pcUnit );
				}
			}
			else
			{
				//Add New Cast
				if ( psPacket->dwHolyConvictionTimeLeft > 0 )
				{
					HOLYCONVICTIONHANDLER->OnCastEffect( pcUnit, psPacket->sHolyConvictionLevel, (float)psPacket->dwHolyConvictionTimeLeft / 1000.0f );
				}
			}

			//Orb Mastery
			if ( pcUnit->bWizardTrance )
			{
				//Cast End Time
				if ( psPacket->dwWizardTranceTimeLeft == 0 )
				{
					WIZARDTRANCEHANDLER->OnCastEnd( pcUnit );
				}
			}
			else
			{
				//Add New Cast
				if ( psPacket->dwWizardTranceTimeLeft > 0 )
				{
					WIZARDTRANCEHANDLER->OnCastEffect( pcUnit, psPacket->sWizardTranceLevel, 300.0f );
				}
			}

			//Whirl Wind
			if ( pcUnit->bWhirlWind )
			{
				//Cast End Time
				if ( psPacket->bWhirlWind == false )
					WHIRLWINDHANDLER->OnCastEnd( pcUnit );
			}
			else
			{
				//Add New Cast
				if ( psPacket->bWhirlWind )
					WHIRLWINDHANDLER->OnCastEffect( pcUnit );
			}

			//Kiss Viper
			if ( pcUnit->bKissViper )
			{
				//Cast End Time
				if ( psPacket->dwKissViperTimeLeft == 0 )
					KISSVIPERHANDLER->OnCastEnd( pcUnit );
			}
			else
			{
				//Add New Cast
				if ( psPacket->dwKissViperTimeLeft > 0 )
					KISSVIPERHANDLER->OnCastEffect( pcUnit, psPacket->sKissViperLevel, 1.0f, false );
			}
		}
	}
}

void SkillManager::HandlePacket( PacketSkillStatusContainer * psPacket )
{
	UINT uPosition = 0;

	for ( int i = 0; i < psPacket->iAmount; i++ )
	{
		PacketSkillStatus * ps = (PacketSkillStatus *)(psPacket->baBuffer + uPosition);

		HandlePacket( ps );

		uPosition += ps->iLength;
	}
}

void SkillManager::HandlePacket( PacketSkillCancel * psPacket )
{
	if ( (psPacket->iUnk == 0) && (psPacket->iUnk2 == 0) && (psPacket->iUnk3 == 0) )
	{
		switch ( UNITDATA->sCharacterData.iClass )
		{
			case CHARACTERCLASS_Archer:
				switch ( psPacket->iPlayID )
				{
					case SKILLPLAYID_T51:
						LETHALSIGHTHANDLER->OnCastEnd( UNIT );
						break;
				}
				break;
			case CHARACTERCLASS_Shaman:
				switch ( psPacket->iPlayID )
				{
					case SKILLPLAYID_T51:
						CREEDHANDLER->OnCast( psPacket->iUnk );
						break;
				}
				break;

			default:
				break;
		}
	}

	switch ( psPacket->iPlayID )
	{
		case SKILLID_KissViper:
			KISSVIPERHANDLER->OnCastEnd( UNIT );
			break;
	}
}

void SkillManager::HandlePacket( PacketSkillCast * psPacket )
{
	switch ( psPacket->iClass )
	{
		case CHARACTERCLASS_Shaman:
			if ( psPacket->sActiveSkill.cSkillTinyID == SKILLPLAYID_T51 )
				CREEDHANDLER->OnCast( psPacket->sActiveSkill.cLevel );
			break;

		default:
			break;
	}
}

void SkillManager::HandlePacket( PacketSkillTable * psPacket )
{
	static UINT uTablePosition = 0;

	if ( psPacket->IsFirst() )
		uTablePosition = 0;

	BYTE * pbSkillTable = (BYTE *)&sSkillTable;

	CopyMemory( pbSkillTable + uTablePosition, psPacket->baBuffer, psPacket->iSize );

	uTablePosition += psPacket->iSize;
}

BOOL SkillManager::IsActiveParticleSkill( UnitData * pcUnitData, int iSkillID )
{
	return (BOOL)CALL_WITH_ARG2( 0x005B4800, (DWORD)pcUnitData, iSkillID );
}
