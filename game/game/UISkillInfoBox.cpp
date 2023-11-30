#include "stdafx.h"
#include "UISkillInfoBox.h"

int iaUpgradeSkillCost[20];
int iaUpgradeSkillCostPlus[20];

namespace UI
{
SkillInfoBox::SkillInfoBox()
{
	iSkillInfoBoxHeight = 0;
}

SkillInfoBox::~SkillInfoBox()
{
}

BOOL SkillInfoBox::IsActiveSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_Raving:
		case SKILLID_Impact:
		case SKILLID_TripleImpact:
		case SKILLID_BrutalSwing:
		case SKILLID_RageofZecram:
		case SKILLID_AvengingCrash:
		case SKILLID_BoneCrash:
		case SKILLID_Destroyer:
		case SKILLID_CycloneStrike:
		case SKILLID_SeismicImpact:
		case SKILLID_WhirlwindFS01:

		//Mechanician
		case SKILLID_MechanicBomb:
		case SKILLID_GreatSmash:
		case SKILLID_Spark:
		case SKILLID_GrandSmash:
		case SKILLID_Impulsion:
		case SKILLID_Rupture:

		//Archer
		case SKILLID_WindArrow:
		case SKILLID_PerfectAim:
		case SKILLID_ArrowofRage:
		case SKILLID_Avalanche:
		case SKILLID_ElementalShot:
		case SKILLID_BombShot:
		case SKILLID_Perforation:
		case SKILLID_PhoenixShot:
		case SKILLID_FierceWind:
		case SKILLID_Bombardment:

		//Pikeman 
		case SKILLID_PikeWind:
		case SKILLID_CriticalHit:
		case SKILLID_JumpingCrash:
		case SKILLID_Tornado:
		case SKILLID_Expansion:
		case SKILLID_ChainLance:
		case SKILLID_ChargingStrike:
		case SKILLID_ShadowMaster:

		//Atalanta 
		case SKILLID_BiggerSpear:
		case SKILLID_TwistJavelin:
		case SKILLID_FireJavelin:
		case SKILLID_SplitJavelin:
		case SKILLID_LightningJavelin:
		case SKILLID_StormJavelin:
		case SKILLID_ExtremeRage:
		case SKILLID_Vengeance:
		case SKILLID_ComboJavelin:

		//Knight 
		case SKILLID_SwordBlast:
		case SKILLID_DoubleCrash:
		case SKILLID_Brandish:
		case SKILLID_Piercing:
		case SKILLID_GrandCross:
		case SKILLID_SwordofJustice:
		case SKILLID_DivinePiercing:
		case SKILLID_DivineCross:
		case SKILLID_WhirlWind:

		//Magician 
		case SKILLID_Agony:
		case SKILLID_FireBolt:
		case SKILLID_FireBall:
		case SKILLID_Watornado:
		case SKILLID_DeathRay:
		case SKILLID_Diastrophism:
		case SKILLID_FlameWave:
		case SKILLID_Meteorite:
		case SKILLID_StoneSkin:
		case SKILLID_RedRay:
		case SKILLID_Cataclysm:

		//Priestess 
		case SKILLID_Healing:
		case SKILLID_HolyBolt:
		case SKILLID_MultiSpark:
		case SKILLID_DivineLightning:
		case SKILLID_GrandHealing:
		case SKILLID_VigorBall:
		case SKILLID_Resurrection:
		case SKILLID_GlacialSpike:
		case SKILLID_ChainLightning:
		case SKILLID_IceMeteorite:
		case SKILLID_HeavenlyLight:

		//Assassin 
		case SKILLID_Stinger:
		case SKILLID_RunningHit:
		case SKILLID_VenomThorn:
		case SKILLID_SoreBlade:
		case SKILLID_GustSlash:
		case SKILLID_FrostWind:
		case SKILLID_NinjaShadow:
		case SKILLID_ShadowBlink:

		//Shaman 
		case SKILLID_DarkBolt:
		case SKILLID_DarkWave:
		case SKILLID_SpiritualFlare:
		case SKILLID_PhantomCall:
		case SKILLID_Scratch:
		case SKILLID_Judgement:
		case SKILLID_MourningPray:
		case SKILLID_PressDeity:
		case SKILLID_PhantomNail:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillInfoBox::IsPassiveSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_MeleeMastery:
		case SKILLID_FireAttribute:
		case SKILLID_BoostHealth:
		case SKILLID_SurvivalInstinct:

		//Mechanician
		case SKILLID_PoisonAttribute:
		case SKILLID_MechanicWeaponMastery:

		//Archer
		case SKILLID_ShootingMastery:
		case SKILLID_DionsEye:
		case SKILLID_EvasionMastery:

		//Pikeman 
		case SKILLID_IceAttribute:
		case SKILLID_WeaponDefenseMastery:
		case SKILLID_CriticalMastery:

		//Atalanta 
		case SKILLID_ThrowingMaster:
		case SKILLID_GoldenApple:

		//Knight 
		case SKILLID_PhysicalTraining:
		case SKILLID_SwordMastery:

		//Magician 
		case SKILLID_MentalMastery:
		
		//Priestess 
		case SKILLID_Meditation:

		//Assassin 
		case SKILLID_DualWieldMastery:
		case SKILLID_AttackMastery:
		case SKILLID_FatalMastery:

		//Shaman 
		case SKILLID_InnerPeace:
		case SKILLID_OccultLife:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillInfoBox::IsDebuffSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_Roar:

		//Mechanician
		case SKILLID_ParasitShot:

		//Archer
		case SKILLID_EntalingRoots:

		//Pikeman 
		case SKILLID_GroundPike:
		case SKILLID_VenomSpear:
		case SKILLID_AssassinsEye:
		case SKILLID_RingofSpears:

		//Atalanta 
		case SKILLID_ShieldStrike:
		case SKILLID_SoulSucker:
		case SKILLID_PlagueJavelin:

		case SKILLID_Distortion:

		//Priestess 
		case SKILLID_HolyMind:
		case SKILLID_Extinction:

		//Assassin 
		case SKILLID_Wisp:
		case SKILLID_SoulShock:
		case SKILLID_Polluted:

		//Shaman 
		case SKILLID_Inertia:
		case SKILLID_SpiritualManacle:
		case SKILLID_Haunt:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillInfoBox::IsBuffSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Fighter
		case SKILLID_Concentration:
		case SKILLID_SwiftAxe:
		case SKILLID_Berserker:
		case SKILLID_RuthlessofZecram:

		//Mechanician
		case SKILLID_ExtremeShield:
		case SKILLID_PhysicalAbsorb:
		case SKILLID_Maximize:
		case SKILLID_Automation:
		case SKILLID_MetalArmor:
		case SKILLID_SparkShield:
		case SKILLID_Compulsion:
		case SKILLID_MagneticSphere:
		case SKILLID_HardeningShield:
		case SKILLID_MagneticDischarge:

		//Archer
		case SKILLID_ScoutHawk:
		case SKILLID_Falcon:
		case SKILLID_GoldenFalcon:
		case SKILLID_ForceofNature:
		case SKILLID_LethalSight:

		//Pikeman 
		case SKILLID_Vanish:
		case SKILLID_Vague:

		//Atalanta 
		case SKILLID_Farina:
		case SKILLID_Windy:
		case SKILLID_TriumphofJavelin:
		case SKILLID_HallofValhalla:
		case SKILLID_FrostJavelin:

		//Knight 
		case SKILLID_HolyBody:
		case SKILLID_HolyValor:
		case SKILLID_DrasticSpirit:
		case SKILLID_DivineShield:
		case SKILLID_HolyIncantation:
		case SKILLID_GodlyShield:
		case SKILLID_GodsBlessing:
		case SKILLID_GloriousShield:
		case SKILLID_HolyConviction:

		//Magician 
		case SKILLID_Zenith:
		case SKILLID_EnchantWeapon:
		case SKILLID_EnergyShield:
		case SKILLID_SpiritElemental:
		case SKILLID_DancingSword:
		case SKILLID_WizardTrance:

		//Priestess 
		case SKILLID_HolyReflection:
		case SKILLID_VirtualLife:
		case SKILLID_RegenerationField:
		case SKILLID_SummonMuspell:
		case SKILLID_Consecration:

		//Assassin 
		case SKILLID_Alas:
		case SKILLID_Inpes:
		case SKILLID_Blind:

		//Shaman 
		case SKILLID_ChasingHunt:
		case SKILLID_AdventMigal:
		case SKILLID_Rainmaker:
		case SKILLID_AdventMidranda:
		case SKILLID_Creed:
			return TRUE;
	}

	return FALSE;
}

BOOL SkillInfoBox::IsSummonSkill( ESkillID eSkillID )
{
	switch( eSkillID )
	{
		//Mechanician
		case SKILLID_MetalGolem:

		//Archer
		case SKILLID_RecallWolverine:

		//Atalanta
		case SKILLID_SummonTiger:

		//Priestess
		case SKILLID_IceElemental:

		//Magician
		case SKILLID_FireElemental:

		//Shaman
		case SKILLID_RecallHestian:
			return TRUE;
			break;
	}
	return FALSE;
}

BOOL SkillInfoBox::CanPrepareShowSkill( Skill * psSkill, Rectangle2D sBox )
{
	BOOL bRet = TRUE;

	if( cSkillWrapper.sSkillInfo.eSkillID == psSkill->sSkillInfo.eSkillID )
		bRet = FALSE;

	if( !bRet )
	{
		if ( cSkillWrapper.iLevel != psSkill->iLevel )
			bRet = TRUE;

		if( !sSkillBox.Equals( &sBox ) )
			bRet = TRUE;
	}

	return bRet;
}

void SkillInfoBox::AddString( int iSide, const std::string & strString, DWORD dwColor, BOOL bBold )
{
	//Find breakline in String 
	BOOL bMultiLine = FALSE;
	if( strString.find( '\n' ) != std::string::npos )
		bMultiLine = TRUE;

	switch( iSide )
	{
		case 0: //Left Side
			//Is Multiline?
			if( bMultiLine )
			{
				size_t n = std::count( strString.begin(), strString.end(), '\n' );
				iSkillInfoBoxHeight += 14 * n;
			}
			else
			{
				UI::Text_ptr pText = std::make_shared<Text>( strString, Rectangle2D( 14, iSkillInfoBoxHeight, 208, 0 ), bBold, dwColor );
				pText->SetText(trim((std::string)strString));
				pText->SetMultiLine( TRUE );
				pText->SetWordWrap( TRUE );
				iSkillInfoBoxHeight += pText->GetHeightText();
				vLeftText.push_back( pText );
			}
			break;
		case 1: //Right Side
		{
			UI::Text_ptr pText = std::make_shared<Text>( strString, Rectangle2D( 14 + vLeftText[vLeftText.size() - 1]->GetTextWidth() + 6, iSkillInfoBoxHeight - vLeftText[vLeftText.size() - 1]->GetHeightText(), 208, 20 ), bBold, dwColor );
			pText->SetText(trim((std::string)strString));
			pText->SetMultiLine( TRUE );
			pText->SetWordWrap( TRUE );
			vRightText.push_back( pText );
			break;
		}
		case 2:
		{
			UINT uIndex = uSkillStatusPos + vCompareText.size();

			if ( (uIndex < vLeftText.size()) && (uIndex < vRightText.size()) )
			{
				UI::Text_ptr pText = std::make_shared<Text>( strString, Rectangle2D( vLeftText[uIndex]->GetTextWidth() + vRightText[uIndex]->GetTextWidth() + 24, vLeftText[uIndex]->GetY(), 208, 0 ), bBold, dwColor );
				pText->SetText(trim((std::string)strString));
				pText->SetMultiLine( TRUE );
				pText->SetWordWrap( TRUE );
				vCompareText.push_back( pText );
			}
			break;
		}
	}
}

void SkillInfoBox::SetLineColor( DWORD dwColor, int iSide, int iLineCount )
{
	int iSetColor = 0;
	if( iLineCount < 0 )
		iSetColor = vLeftText.size() - 1;
	else
		iSetColor = iLineCount;

	switch( iSide )
	{
	case -1:
		vLeftText[ iSetColor ]->SetColor( dwColor );
		vRightText[ iSetColor ]->SetColor( dwColor );
		break;
	case 0:
		vLeftText[ iSetColor ]->SetColor( dwColor );
		break;
	case 1:
		vRightText[ iSetColor ]->SetColor( dwColor );
		break;
	}
}

void SkillInfoBox::Init()
{
	//Load Images
	pImageMaskLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topleft.png" );
	pImageMaskRightTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\topright.png" );
	pImageMaskLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomleft.png" );
	pImageMaskRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\mask\\bottomright.png" );

	//Load Border Images
	pImageBorderLeftTop				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topleft.png" );
	pImageBorderRightTop			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\topright.png" );
	pImageBorderTop					= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\top.png" );
	pImageBorderRightBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomright.png" );
	pImageBorderLeftBottom			= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottomleft.png" );
	pImageBorderBottom				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\bottom.png" );
	pImageBorderLeft				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\left.png" );
	pImageBorderRight				= ImageLoader::LoadImage( "game\\images\\UI\\box\\border\\right.png" );

	pRenderTargetMask = GetGraphicsDevice()->MakeRenderTarget( 5001, 600, 600 );
	pEffectMask = GetGraphicsDevice()->GetEffectFactory()->MakeEffect( "game\\scripts\\shaders\\UI\\InfoBox.fx" );

	for( int i = 0; i < 12; i++ )
	{
		pImageIconItems[i] = ImageLoader::LoadImage( "image\\Sinimage\\skill\\WeaponIcon\\%d.bmp", i + 1 );
	}
}

void SkillInfoBox::FormatSkillInfo()
{
	BOOL bUpdateSkill = FALSE;

	//Tier 5 Skills? So format skill info here
	if ( (cSkillWrapper.iID & 0x00FF0000) == 0x00100000 )
		bUpdateSkill = TRUE;

	uSkillStatusPos = vLeftText.size();
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;


#ifndef _NEW_DMG__

	NewFormatSkillInfo();

	bUpdateSkill = TRUE;

#else

	switch( cSkillWrapper.iID )
	{		
		case SKILLID_SwiftAxe:
			AddString( 0, "Attack Speed Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT33AttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaFighterT33Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaFighterT33AttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaFighterT33Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Force:
			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 20) )
			{
				AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
				AddString( 1, FormatString( "%d", ForceDamageTable[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

				AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
				AddString( 1, FormatString( "%d%%", ForceDamagePercentTable[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

				AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
				AddString( 1, FormatString( "%dsec", ForceDurationTable[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );
			}

			bUpdateSkill = TRUE;
			break;
		case SKILLID_PvPHonor:
		{
			BOOL bBronze = cSkillWrapper.PlusState[0] == PacketHonor::EHonorType::HONORTYPE_PvPBronze;
			BOOL bSilver = cSkillWrapper.PlusState[0] == PacketHonor::EHonorType::HONORTYPE_PvPSilver;
			BOOL bGold = cSkillWrapper.PlusState[0] == PacketHonor::EHonorType::HONORTYPE_PvPGold;

			AddString( 0, "Bronze Honor:", D3DCOLOR_ARGB( bBronze ? 255 : 128, 205, 127, 50 ), bBronze );
			AddString( 1, "+3% Attack Power", D3DCOLOR_ARGB( bBronze ? 255 : 128, 205, 127, 50 ), bBronze );

			AddString( 0, "Silver Honor:", D3DCOLOR_ARGB( bSilver ? 255 : 128, 192, 192, 192 ), bSilver );
			AddString( 1, "+5% Attack Power", D3DCOLOR_ARGB( bSilver ? 255 : 128, 192, 192, 192 ), bSilver );

			AddString( 0, "Gold Honor:", D3DCOLOR_ARGB( bGold ? 255 : 128, 255, 215, 0 ), bGold );
			AddString( 1, "+7% Attack Power", D3DCOLOR_ARGB( bGold ? 255 : 128, 255, 215, 0 ), bGold );

			bUpdateSkill = TRUE;
		}
		break;
		case SKILLID_PKMode:
		{
			AddString( 0, "Extra Drop Chance:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, "50%", D3DCOLOR_XRGB( 238, 210, 151 ) );
			AddString( 0, "Extra Experience:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, "10%", D3DCOLOR_XRGB( 238, 210, 151 ) );
			AddString( 0, "\n", D3DCOLOR_XRGB( 243, 237, 216 ) );

			int iTime = CHARACTERPVPMODEHANDLER->GetTimeLeft();
			if ( iTime > 0 )
			{
				AddString( 0, "Time left to be disabled:", D3DCOLOR_XRGB( 243, 237, 216 ) );
				AddString( 1, FormatString( "%02d:%02d", iTime / 60, iTime % 60 ), D3DCOLOR_XRGB( 238, 210, 151 ) );
			}
			else
			{
				AddString( 0, "You can disable it now!", D3DCOLOR_XRGB( 238, 210, 151 ) );
				AddString( 1, "", D3DCOLOR_XRGB( 243, 237, 216 ) );
			}

			bUpdateSkill = TRUE;
		}
		break;
		case SKILLID_BellatraHonor:
		{
			BOOL bBronze	= cSkillWrapper.PlusState[0] == PacketHonor::EHonorType::HONORTYPE_BellatraBronze;
			BOOL bSilver	= cSkillWrapper.PlusState[0] == PacketHonor::EHonorType::HONORTYPE_BellatraSilver;
			BOOL bGold		= cSkillWrapper.PlusState[0] == PacketHonor::EHonorType::HONORTYPE_BellatraGold;

			AddString( 0, "Bronze Honor:", D3DCOLOR_ARGB( bBronze ? 255 : 128, 205, 127, 50 ), bBronze );
			AddString( 1, "+3% Attack Rating", D3DCOLOR_ARGB( bBronze ? 255 : 128, 205, 127, 50 ), bBronze );

			AddString( 0, "Silver Honor:", D3DCOLOR_ARGB( bSilver ? 255 : 128, 192, 192, 192 ), bSilver );
			AddString( 1, "+5% Attack Rating", D3DCOLOR_ARGB( bSilver ? 255 : 128, 192, 192, 192 ), bSilver );

			AddString( 0, "Gold Honor:", D3DCOLOR_ARGB( bGold ? 255 : 128, 255, 215, 0 ), bGold );
			AddString( 1, "+7% Attack Rating", D3DCOLOR_ARGB( bGold ? 255 : 128, 255, 215, 0 ), bGold );

			bUpdateSkill = TRUE;
		}
		break;
		case SKILLID_Automation:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT23DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaMechT23AtkSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f", faMechT23MPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f", faMechT23SPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaMechT23DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaMechT23AtkSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1f)", faMechT23MPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1f)", faMechT23SPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}

			bUpdateSkill = TRUE;
			break;
		case SKILLID_GreatSmash:
			AddString( 0, "Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT21Damage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaMechT21Area[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaMechT21Damage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaMechT21Area[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}

			bUpdateSkill = TRUE;
			break;
		case SKILLID_MetalArmor:
			AddString( 0, "Armor Defense Rtg Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT31DefenseRtg[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaMechT31Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaMechT31DefenseRtg[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaMechT31Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}

			bUpdateSkill = TRUE;
			break;
		case SKILLID_Rupture:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT51DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
				AddString( 2, FormatString( "(%d%%)", iaMechT51DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_ParasitShot:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%d%%)", iaMechT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_HardeningShield:
			AddString( 0, "Damage Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT53DMGReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "WP ATK Rate Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT53WPAtkRateRed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaMechT53DMGReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaMechT53WPAtkRateRed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MagneticDischarge:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMechT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%d%%)", iaMechT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_RageofZecram:
			AddString( 0, "DMG Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT24DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaFighterT24Area[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaFighterT24DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaFighterT24Area[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SurvivalInstinct:
			AddString( 0, "Add HP Regen:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f", faFighterT51AddHPRegen[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add HP Potion Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT51AddHPPotionBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add HP Boost Health:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT51BoostHealthBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%.1f)", faFighterT51AddHPRegen[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaFighterT51AddHPPotionBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaFighterT51BoostHealthBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SeismicImpact:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaFighterT52Area[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaFighterT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaFighterT52Area[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RuthlessofZecram:
			AddString( 0, "HP Decrease:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f%%", faFighterT53HPDec[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Hits for boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaFighterT53HitsBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Final DMG Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT53DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%.1f%%)", faFighterT53HPDec[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaFighterT53HitsBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaFighterT53DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_WhirlwindFS01:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaFighterT54DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaFighterT54Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaFighterT54MPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaFighterT54SPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaFighterT54DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaFighterT54Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaFighterT54MPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaFighterT54SPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RingofSpears:
			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaPikeT51Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%dsec)", iaPikeT51Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_LastBreath:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPikeT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaPikeT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DeathMaster:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPikeT53DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "WP ATK Rate Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPikeT53ATKRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaPikeT53DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaPikeT53ATKRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Twister:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPikeT54DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%d%%)", iaPikeT54DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_LethalSight:
			AddString( 0, "Range Add:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaArcherT51RangeAdd[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Critical Rate:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaArcherT51CritRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "After Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaArcherT51DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", iaArcherT51RangeAdd[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaArcherT51CritRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaArcherT51DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FierceWind:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaArcherT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Critical DMG Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaArcherT52CDMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaArcherT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaArcherT52CDMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_EntalingRoots:
			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaArcherT53Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%d)", iaArcherT53Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_Bombardment:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaArcherT54DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaArcherT54Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaArcherT54MPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaArcherT54SPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaArcherT54DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaArcherT54Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaArcherT54MPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaArcherT54SPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GodsBlessing:
			AddString( 0, "Add Atk Pow:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaKnightT43AddAtkPow[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaKnightT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaKnightT43AddAtkPow[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaKnightT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyConviction:
			AddString( 0, "Undead DMG Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaKnightT51DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
				AddString( 2, FormatString( "(%d%%)", iaKnightT51DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_WhirlWind:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaKnightT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaKnightT52Area[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Atk Rtg Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaKnightT52AtkRtg[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaKnightT52Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaKnightT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaKnightT52Area[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaKnightT52AtkRtg[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaKnightT52Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GloriousShield:
			AddString( 0, "Shield Def Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaKnightT53DefBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%d%%)", iaKnightT53DefBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_DivineCross:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaKnightT54DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
				AddString( 2, FormatString( "(%d%%)", iaKnightT54DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_SummonTiger:
			AddString( 0, "Pet Settings:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%s", SKILLMANAGERHANDLER->IsAgressive( SKILLID_SummonTiger ) ? "Automatic" : "Manual" ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d-%d", iaSummonTigerAttackPower[uSkillLevel], iaSummonTigerAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaSummonTigerAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaSummonTigerHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", 300 ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "" ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d-%d)", iaSummonTigerAttackPower[uSkillLevel + 1], iaSummonTigerAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaSummonTigerAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaSummonTigerHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", 300 ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PlagueJavelin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaAtaT51DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Targets:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaAtaT51Targets[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Poison Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaAtaT51PoisonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaAtaT51DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaAtaT51Targets[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaAtaT51PoisonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GoldenApple:
			AddString( 0, "Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaAtaT52Speed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Evade:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaAtaT52Evade[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaAtaT52Speed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaAtaT52Evade[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_IceElemental:
			AddString( 0, "Pet Settings:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%s", SKILLMANAGERHANDLER->IsAgressive( SKILLID_IceElemental ) ? "Automatic" : "Manual" ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d-%d", iaIceElementalAttackPower[uSkillLevel], iaIceElementalAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaIceElementalAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaIceElementalHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", 300 ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "" ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d-%d)", iaIceElementalAttackPower[uSkillLevel + 1], iaIceElementalAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaIceElementalAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaIceElementalHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", 300 ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_IceMeteorite:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPrsT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaPrsT52AreaFake[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "# of Meteorites:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaPrsT52MeteoriteCount[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaPrsT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaPrsT52AreaFake[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaPrsT52MeteoriteCount[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HeavenlyLight:
			AddString( 0, "ATK Rate Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPrsT53ATKRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaPrsT53Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Charged Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaPrsT53Area[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaPrsT53ATKRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaPrsT53Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaPrsT53Area[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Consecration:
			AddString( 0, "Defense:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaPrsT54Defense[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaPrsT54Absorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaPrsT54Defense[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaPrsT54Absorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
        case SKILLID_DeathRay:
            AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
            AddString( 1, FormatString( "%d%%", iaMageT24DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

            AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
            AddString( 1, FormatString( "%d%%", iaMageT24AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

            AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
            AddString( 1, FormatString( "%d", iaMageT24AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

            if ( cSkillWrapper.iLevel < 10 )
            {
                AddString( 2, FormatString( "(%d%%)", iaMageT24DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
                AddString( 2, FormatString( "(%d%%)", iaMageT24AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
                AddString( 2, FormatString( "(%d)", iaMageT24AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
            }
            break;
		case SKILLID_EnchantWeapon:
		{
			BOOL bFireDamage	= FALSE;
			BOOL bIceDamage		= FALSE;
			BOOL bLightDamage	= FALSE;

			if ( bInfoBoxSkillTimer )
			{
				if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_EnchantWeapon ) )
				{
					if ( UNITDATA->iEnchantWeaponType == 3 )
						bFireDamage = TRUE;
					else if ( UNITDATA->iEnchantWeaponType == 4 )
						bIceDamage = TRUE;
					else if ( UNITDATA->iEnchantWeaponType == 5 )
						bLightDamage = TRUE;
				}
			}

			AddString( 0, "Fire Damage:", bFireDamage ? D3DCOLOR_XRGB( 255, 127, 0 ) : D3DCOLOR_XRGB( 243, 237, 216 ), bFireDamage );
			AddString( 1, FormatString( "%d-%d", iaMageT23FireDamage->iaValue[uSkillLevel][0], iaMageT23FireDamage->iaValue[uSkillLevel][1] ), D3DCOLOR_XRGB( 238, 210, 151 ), bFireDamage );

			AddString( 0, "Ice Damage:", bIceDamage ? D3DCOLOR_XRGB( 0, 191, 255 ) : D3DCOLOR_XRGB( 243, 237, 216 ), bIceDamage );
			AddString( 1, FormatString( "%d-%d", iaMageT23IceDamage->iaValue[uSkillLevel][0], iaMageT23IceDamage->iaValue[uSkillLevel][1] ), D3DCOLOR_XRGB( 238, 210, 151 ), bIceDamage );

			AddString( 0, "Lightning Damage:", bLightDamage ? D3DCOLOR_XRGB( 198, 226, 255 ) : D3DCOLOR_XRGB( 243, 237, 216 ), bLightDamage );
			AddString( 1, FormatString( "%d-%d", iaMageT23LightDamage->iaValue[uSkillLevel][0], iaMageT23LightDamage->iaValue[uSkillLevel][1] ), D3DCOLOR_XRGB( 238, 210, 151 ), bLightDamage );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaMageT23Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d-%d)", iaMageT23FireDamage->iaValue[uSkillLevel + 1][0], iaMageT23FireDamage->iaValue[uSkillLevel + 1][1] ), D3DCOLOR_XRGB( 92, 163, 99 ), bFireDamage );
				AddString( 2, FormatString( "(%d-%d)", iaMageT23IceDamage->iaValue[uSkillLevel + 1][0], iaMageT23FireDamage->iaValue[uSkillLevel + 1][1] ), D3DCOLOR_XRGB( 92, 163, 99 ), bIceDamage );
				AddString( 2, FormatString( "(%d-%d)", iaMageT23LightDamage->iaValue[uSkillLevel + 1][0], iaMageT23FireDamage->iaValue[uSkillLevel + 1][1] ), D3DCOLOR_XRGB( 92, 163, 99 ), bLightDamage );
				AddString( 2, FormatString( "(%d-%d)", iaMageT23LightDamage->iaValue[uSkillLevel + 1][0], iaMageT23FireDamage->iaValue[uSkillLevel + 1][1] ), D3DCOLOR_XRGB( 92, 163, 99 ), bLightDamage );
				AddString( 2, FormatString( "(%dsec)", iaMageT23Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
		}
		break;

		case SKILLID_MentalMastery:
			AddString( 0, "MP Potion Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", iaMageT21MPRecoveryBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

            AddString( 0, "MP Capacity:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", iaMageT21MPCapacityBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(+%d%%)", iaMageT21MPRecoveryBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", iaMageT21MPCapacityBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;

		case SKILLID_WizardTrance:
			AddString( 0, "Orb Block Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMageT51BlockBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
				AddString( 2, FormatString( "(%d%%)", iaMageT51BlockBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			break;
		case SKILLID_StoneSkin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMageT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMageT52ATKRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaMageT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaMageT52ATKRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RedRay:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMageT53DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaMageT53AddRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaMageT53DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaMageT53AddRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Cataclysm:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaMageT54DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaMageT54Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaMageT54MPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaMageT54SPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaMageT54DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaMageT54Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaMageT54MPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaMageT54SPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ShadowBlink:
			AddString( 0, "Critical DMG Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaASNT51CritBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaASNT51Range[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaASNT51CritBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaASNT51Range[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Inpes:
			AddString( 0, "Attack Speed Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaASNT33AttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaASNT33Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaASNT33AttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaASNT33Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_KissViper:
		{
			bool bKissViperOwner = false;
			if ( bInfoBoxSkillTimer )
			{
				if ( SKILLMANAGERHANDLER->IsActiveBuff( SKILLID_EnchantWeapon ) )
					bKissViperOwner = KISSVIPERHANDLER->IsOwner();
			}

			AddString( 0, "Critical Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", bKissViperOwner ? iaASNT52CriticalAdd[uSkillLevel] : iaASNT52CriticalAdd[uSkillLevel] >> 1 ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaASNT52Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", bKissViperOwner ? iaASNT52CriticalAdd[uSkillLevel + 1] : iaASNT52CriticalAdd[uSkillLevel + 1] >> 1 ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaASNT52Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
		}
		break;
		case SKILLID_AdventMidranda:
			AddString( 0, "Attack Speed Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaShaT43AttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", iaShaT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaShaT43AttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", iaShaT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Creed:
			AddString( 0, "MP Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaShaT51MPBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "STM Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaShaT51STMBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", 300 ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", iaShaT51MPBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", iaShaT51STMBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", 300 ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;

		case SKILLID_PressDeity:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", iaShaT52DMGBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", iaShaT52AreaFake[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );
			
			if ( cSkillWrapper.iLevel < 10 )
			{
				AddString( 2, FormatString( "(%d%%)", iaShaT52DMGBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", iaShaT52AreaFake[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;


        case SKILLID_EventTimer:
        {
            auto sEventData = EVENTHANDLER->GetData();

            if ( sEventData.bEXP )
            {
                AddString( 0, "EXP Event is Active!", D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
                AddString( 1, FormatString( "%f EXP!", (float)(sEventData.iEXP + 100) / 100.0f ), D3DCOLOR_XRGB( 20, 215, 20 ) );
            }

            if ( sEventData.bExtraDrop )
            {
                AddString( 0, "Extra drop Event is Active!", D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
                AddString( 1, FormatString( "+%d drop!", sEventData.iExtraDrop ), D3DCOLOR_XRGB( 200, 200, 20 ) );
            }

            if ( sEventData.bPvP )
            {
                AddString( 0, "PvP Event is Active!", D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
                AddString( 1, "Have fun!", D3DCOLOR_XRGB( 220, 220, 220 ) );
            }

            for ( int i = 0; i < sEventData.iEventCount; i++ )
            {
                AddString( 0, sEventData.szaMessageEventLeft[i], D3DCOLOR_ARGB( sEventData.baTextColorLeft[i][0], sEventData.baTextColorLeft[i][1],
                                                                                sEventData.baTextColorLeft[i][2], sEventData.baTextColorLeft[i][3] ), sEventData.baBoldLeft[i] );
                AddString( 1, sEventData.szaMessageEventRight[i], D3DCOLOR_ARGB( sEventData.baTextColorRight[i][0], sEventData.baTextColorRight[i][1],
                                                                                sEventData.baTextColorRight[i][2], sEventData.baTextColorRight[i][3] ), sEventData.baBoldRight[i] );
            }
        }
        break;
	}

#endif

	//Upgrade Cost
	if( cSkillWrapper.iLevel < 10 && READDWORD(0x367DE9C) > 0 )
	{
		CopyMemory( iaUpgradeSkillCost, (char*)0x9BBE58, sizeof( int ) * 20 );
		CopyMemory( iaUpgradeSkillCostPlus, (char*)0x9BBEA8, sizeof( int ) * 20 );

		strSkillNextLevelCost = FormatString("%s gp",FormatNumber(iaUpgradeSkillCost[cSkillWrapper.sSkillInfo.iSkillIndex] + (iaUpgradeSkillCostPlus[cSkillWrapper.sSkillInfo.iSkillIndex] * (cSkillWrapper.iLevel))));
	}

	if ( bUpdateSkill == FALSE )
	{
		std::string strBufferLeft = (char*)0x036762A0;
		std::string strBufferRight = (char*)0x03677628;

		std::vector<std::string> vStringLeft;
		vStringLeft = split( strBufferLeft, '\r' );

		std::vector<std::string> vStringRight;
		vStringRight = split( strBufferRight, '\r' );

		for( size_t i = 0; i < vStringLeft.size(); i++ )
		{
			if( vStringLeft[i].empty() )
			{
				vStringLeft[i].erase();
				vStringRight[i].erase();
			}
		}

		bool bNextLevel = false;
		for( size_t i = 0; i < vStringLeft.size(); i++ )
		{
			if( i == 0 )
				uSkillStatusPos = vLeftText.size() + 1;

			if ( (vStringLeft[i].compare( "Next Level" ) == 0) )
				bNextLevel = true;

			if( !bNextLevel )
			{
				AddString( 0, vStringLeft[i], D3DCOLOR_XRGB( 243, 237, 216 ) );
				AddString( 1, vStringRight[i], D3DCOLOR_XRGB( 238, 210, 151 ) );
			}
			else
			{
				if( !vStringRight[i].empty() )
				{
					if( vStringRight[i].find( "gp" ) != std::string::npos )
						strSkillNextLevelCost = vStringRight[i];
					else
					{
						if ( cSkillWrapper.iLevel == 0 )
						{
							AddString( 0, vStringLeft[i], D3DCOLOR_XRGB( 110, 110, 110 ) );
							AddString( 1, vStringRight[i], D3DCOLOR_XRGB( 110, 110, 110 ) );
						}
						else
							AddString( 2, FormatString( "(%s)", vStringRight[i].c_str() ), D3DCOLOR_XRGB( 92, 163, 99 ) );
					}
				}
			}
		}
	}
	else
	{
		if ( (cSkillWrapper.iLevel == 0) )
		{
			if ( vLeftText.size() == vRightText.size() )
			{
				for ( size_t i = uSkillStatusPos; i < vLeftText.size(); i++ )
				{
					Text_ptr pLeftText = vLeftText[i];
					Text_ptr pRightText = vRightText[i];

					pLeftText->SetColor( D3DCOLOR_XRGB( 110, 110, 110 ) );
					pRightText->SetColor( D3DCOLOR_XRGB( 110, 110, 110 ) );

				}
			}
		}
	}
}

BOOL SkillInfoBox::PrepareShowSkill( Skill * pcSkill, BOOL bSkillTimer, Rectangle2D sBox )
{
	bInfoBoxSkillTimer = bSkillTimer;

	//Save Box Position
	sSkillBox = sBox;

	//Clear Vectors
	vLeftText.clear();
	vRightText.clear();
	vCompareText.clear();
	strSkillNextLevelCost.clear();

	uSkillStatusPos = 0;
	iSkillInfoBoxHeight = 20;

	//Copy pcSkill to cSkillWrapper
	CopyMemory( &cSkillWrapper, pcSkill, sizeof(Skill) );

	//Name Skill
	if ( cSkillWrapper.iID == SKILLID_DefaultAttack )
	{
		AddString( 0, "Default Attack", D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
		AddString( 1, "" );
	}
	else
	{
		AddString( 0, cSkillWrapper.sSkillInfo.szName, D3DCOLOR_XRGB( 243, 237, 216 ), TRUE );
		AddString( 1, "" );
	}

	//Level Skill
	if( cSkillWrapper.iLevel > 0 && cSkillWrapper.iID != SKILLID_Force && cSkillWrapper.iID != SKILLID_QUEST1 && cSkillWrapper.iID != SKILLID_QUEST2 && 
		cSkillWrapper.iID != SKILLID_QUEST3 && cSkillWrapper.iID != SKILLID_QUEST4 && cSkillWrapper.iID != SKILLID_QUESTNew && cSkillWrapper.iID != SKILLID_QUESTNewTime && 
		(cSkillWrapper.iID < ITEMID_HopyCrystal || cSkillWrapper.iID > ITEMID_CaravanGoldenHopyCrystal) && cSkillWrapper.iID != SKILLID_BellatraGold && cSkillWrapper.iID != SKILLID_BellatraSilver &&
		cSkillWrapper.iID != SKILLID_BellatraBronze && cSkillWrapper.iID != SKILLID_DefaultAttack && cSkillWrapper.iID != SKILLID_EventTimer && cSkillWrapper.iID != SKILLID_PvPHonor && 
		cSkillWrapper.iID != SKILLID_BellatraHonor && cSkillWrapper.iID != SKILLID_PKMode )
	{
		AddString( 0, "Level" );
		AddString( 1, FormatString( "%d", cSkillWrapper.iLevel ) );
		SetLineColor( D3DCOLOR_XRGB( 243, 237, 216 ) );
	}

	//Required Level
	if( cSkillWrapper.iLevel >= 0 && cSkillWrapper.iLevel < 10 && !bInfoBoxSkillTimer && cSkillWrapper.iID != SKILLID_DefaultAttack )
	{
		int iRequiredLevel = (cSkillWrapper.sSkillInfo.iSkillLevelRequired + (cSkillWrapper.iLevel * 2));

		AddString( 0, "Required Level:" );
		AddString( 1, FormatString( "%d", iRequiredLevel ) );
		SetLineColor( D3DCOLOR_XRGB( 255, 185, 89 ) );
	}

	//Type Skill
	if( cSkillWrapper.iID != SKILLID_Force && cSkillWrapper.iID != SKILLID_QUEST1 && cSkillWrapper.iID != SKILLID_QUEST2 && cSkillWrapper.iID != SKILLID_QUEST3 && cSkillWrapper.iID != SKILLID_QUEST4 )
	{
		int iSkillID = cSkillWrapper.iID;

		if( !iSkillID )
			iSkillID = cSkillWrapper.sSkillInfo.eSkillID;

		if( IsActiveSkill( (ESkillID)iSkillID ) )
			AddString( 0, "Active Skill" );
		else if( IsPassiveSkill( (ESkillID)iSkillID ) )
			AddString( 0, "Passive Skill" );
		else if( IsDebuffSkill( (ESkillID)iSkillID ) )
			AddString( 0, "Debuff Skill" );
		else if( IsBuffSkill( (ESkillID)iSkillID ) )
			AddString( 0, "Buff Skill" );
		else if( IsSummonSkill( (ESkillID)iSkillID ) )
			AddString( 0, "Summon Skill" );
		else
			AddString( 0, "" );

		AddString( 1, "" );
		SetLineColor( D3DCOLOR_XRGB( 255, 185, 89 ) );
	}

	if ( cSkillWrapper.iID == TIMERSKILLID_QuestNew || cSkillWrapper.iID == TIMERSKILLID_QuestNewT )
	{
		int iQuestType = CQUESTGAME->GetQuestType( &cSkillWrapper );

		if ( iQuestType == 1 )
			AddString( 0, "Solo Quest" );
		else if ( iQuestType == 2 )
			AddString( 0, "Party Quest" );
		else if ( iQuestType == 3 )
			AddString( 0, "Solo/Party Quest" );
		else if ( iQuestType == 4 )
			AddString( 0, "Party/Raid Quest" );
		else if ( iQuestType == 5 )
			AddString( 0, "Solo/Party/Raid Quest" );
		else
			AddString( 0, "Normal Quest" );

		AddString( 1, "" );

		if ( (cSkillWrapper.PlusState[4] == QUESTID_TheDemonAngel) || (cSkillWrapper.PlusState[4] == QUESTID_TheMineCommander) || (cSkillWrapper.PlusState[4] == QUESTID_Retrievingbackthelight) ||
			 (cSkillWrapper.PlusState[4] == QUESTID_CommanderSaxhleel) || (cSkillWrapper.PlusState[4] == QUESTID_VestigesOfAncientWar) || (cSkillWrapper.PlusState[4] == QUESTID_TheGreedy) ||
			 (cSkillWrapper.PlusState[4] == QUESTID_Pumpking) || (cSkillWrapper.PlusState[4] == QUESTID_LordSantaGoblin) || (cSkillWrapper.PlusState[4] == QUESTID_LordSantaSkeleton) ||
			 (cSkillWrapper.PlusState[4] == QUESTID_TheStoneGod) || (cSkillWrapper.PlusState[4] == QUESTID_TheReturnOfTheLizards) || (cSkillWrapper.PlusState[4] == QUESTID_TheEasterChef) ||
			 (cSkillWrapper.PlusState[4] == QUESTID_FlamoQuest) || (cSkillWrapper.PlusState[4] == QUESTID_TheunknownplanofHagios) || (cSkillWrapper.PlusState[4] == QUESTID_SeeknDestroyYagditha) )
		{
			AddString( 0, "Works on Roll Dice" );
			AddString( 1, "" );
		}
	}

	AddString( 0, "\n" );


	//Description Skill
	AddString( 0, cSkillWrapper.sSkillInfo.szDescription );
	AddString( 1, "" );
	SetLineColor( D3DCOLOR_XRGB( 169, 169, 169 ) );
	
	AddString( 0, "\n" );

	//Skill Locked
	if( SKILLDATAHANDLER->IsSkillLocked( cSkillWrapper.iID ) )
	{
		AddString( 0, "Skill is locked. Coming soon!" );
		AddString( 1, "" );
		SetLineColor( D3DCOLOR_XRGB( 200, 90, 90 ) );

		AddString( 0, "\n" );
	}

	//Correct Item Group
	if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[0] != ITEMTYPE_None )
	{
		uImageIconsItemsY = iSkillInfoBoxHeight - 4;
		AddString( 0, "\n" );
		AddString( 0, "\n" );
	}

	if ( cSkillWrapper.iID == SKILLID_PKMode )
	{
		AddString( 0, "(Extra Experience in Monsters only)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_Automation )
	{
		AddString( 0, "(+150% Damage VS Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "(Drain MP and SP every hit)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_MetalArmor )
	{
		AddString( 0, "(Physical Absorption Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Rupture )
	{
		AddString( 0, "(Grand Smash 70% ATK Rtg Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_ShadowBlink )
	{
	/*	AddString( 0, "(Needs Blind Active)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" ); */
	}
	else if( cSkillWrapper.iID == SKILLID_GrandSmash )
	{
		AddString( 0, "(+45% Damage VS Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "(Attack Rating Boost only works with Hammers)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_DancingSword )
	{
		AddString( 0, "(+40% Damage VS Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_MagneticSphere )
	{
		AddString( 0, "(+30% Damage VS Machines)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_MagneticDischarge )
	{
		AddString( 0, "(Party Buff)", D3DCOLOR_XRGB( 105, 235, 105 ) );
		AddString( 1, "" );
		AddString( 0, "(Magnetic Sphere Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "(Skill 50% Chain on Members)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "(+30% Damage VS Machines)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_BrutalSwing )
	{
		AddString( 0, "(+40% Damage VS Demons)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_AvengingCrash )
	{
		AddString( 0, "(Brutal Swing 50% Critical Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Destroyer )
	{
		AddString( 0, "(+100% Damage VS Demons)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_SurvivalInstinct )
	{
	}
	else if ( cSkillWrapper.iID == SKILLID_SeismicImpact )
	{
		AddString( 0, "(Brutal Swing 50% Critical Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "(+30% Damage VS Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_JumpingCrash )
	{
		AddString( 0, "(+30% Damage VS Demons)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Twister )
	{
		AddString( 0, "(Tornado Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_WindArrow )
	{
		AddString( 0, "(+30% Damage VS Normal Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_PerfectAim )
	{
		AddString( 0, "(+30% Damage VS Demons)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_FierceWind )
	{
		AddString( 0, "(Perforation Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Bombardment )
	{
		AddString( 0, "(Phoenix Shot Range Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Stinger )
	{
		AddString( 0, "(+30% Damage VS Normal and Mutant)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Alas )
	{
		AddString( 0, "(Party Buff)", D3DCOLOR_XRGB( 105, 235, 105 ) );
		AddString( 1, "" );
		AddString( 0, "(Skill 50% Chain on Members)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_HolyValor )
	{
		AddString( 0, "(Party Buff)", D3DCOLOR_XRGB( 105, 235, 105 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_HolyIncantation )
	{
		AddString( 0, "(Undead Only)", D3DCOLOR_XRGB( 255, 0, 16 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_GodlyShield )
	{
		AddString( 0, "(Divine Shield Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if ( cSkillWrapper.iID == SKILLID_HolyConviction )
	{
		AddString( 0, "(Holy Valor Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "(Holy Body Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "(Party Buff Holy Body 50% Chain)", D3DCOLOR_XRGB( 105, 235, 105 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Extinction )
	{
		AddString( 0, "(Undead Only)", D3DCOLOR_XRGB( 255, 0, 16 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_HolyReflection )
	{
		AddString( 0, "(Undead Only)", D3DCOLOR_XRGB( 255, 0, 16 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_DivineLightning )
	{
		AddString( 0, "(+50% Damage VS Undead)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_VigorBall )
	{
		AddString( 0, "(+100% Damage VS Undead)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_ChainLightning )
	{
		AddString( 0, "(+50% Damage VS Undead)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Farina )
	{
		AddString( 0, "(+20% Damage VS Normal Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_BiggerSpear )
	{
		AddString( 0, "(+30% Damage VS Machines)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_TwistJavelin )
	{
		AddString( 0, "(+30% Damage VS Demons)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_FireJavelin )
	{
		AddString( 0, "(+30% Damage VS Mutants)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_LightningJavelin )
	{
		AddString( 0, "(+30% Damage VS Undead)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_Scratch )
	{
		AddString( 0, "(+30% Damage VS Monsters)", D3DCOLOR_XRGB( 212, 39, 232 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_HallofValhalla )
	{
		AddString( 0, "(Triumph of Valhalla Chain)", D3DCOLOR_XRGB( 105, 144, 176 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}
	else if( cSkillWrapper.iID == SKILLID_AdventMigal )
	{
		AddString( 0, "(Party Buff)", D3DCOLOR_XRGB( 105, 235, 105 ) );
		AddString( 1, "" );
		AddString( 0, "\n" );
	}

	//Next Level
	if( cSkillWrapper.iLevel < 10 && cSkillWrapper.iLevel > 0 && !bInfoBoxSkillTimer && cSkillWrapper.iID != SKILLID_DefaultAttack )
	{
		AddString( 0, "Next Level", D3DCOLOR_XRGB( 92, 163, 99 ), TRUE );
		AddString( 1, "" );
	}

	//Format Skill Info
	FormatSkillInfo();

	if( !bInfoBoxSkillTimer && cSkillWrapper.sSkillInfo.eSkillType != SKILLTYPE_Passive )
		AddString( 0, "\n" );

	if( cSkillWrapper.iLevel > 0 && !bInfoBoxSkillTimer )
	{
		//MP
		if( cSkillWrapper.sSkillInfo.dwUseManaPointer )
		{
			int * piaUseMana = (int*)cSkillWrapper.sSkillInfo.dwUseManaPointer;

			if( piaUseMana[cSkillWrapper.iLevel - 1] != 0 )
			{
				AddString( 0, "MP Cost:", D3DCOLOR_XRGB( 243, 237, 216 ) );

				//Decrease MP Item?
				if( ( READDWORD( 0x035D0C48 ) > 0 ) && ( READDWORD( 0x035D0C4C ) > 0 ) )
				{
					int iDecreaseMP = ( piaUseMana[cSkillWrapper.iLevel - 1] * READDWORD( 0x035D0C4C ) ) / 100;

					AddString( 1, FormatString( "%d(-%d)", piaUseMana[cSkillWrapper.iLevel - 1], iDecreaseMP ) );
				}
				else
					AddString( 1, FormatString( "%d", piaUseMana[cSkillWrapper.iLevel - 1] ) );

				SetLineColor( D3DCOLOR_XRGB( 234, 186, 81 ), 1 );
			}
		}

		//STM
		if( cSkillWrapper.sSkillInfo.iaSkillUseStamina[0] != 0 )
		{
			AddString( 0, "SP Cost:", D3DCOLOR_XRGB( 243, 237, 216 ) );

			//Decrease STM Item?
			if( ( READDWORD( 0x035D0C60 ) > 0 ) && ( READDWORD( 0x035D0C64 ) > 0 ) )
			{
				int iSP = cSkillWrapper.sSkillInfo.iaSkillUseStamina[0] + ( cSkillWrapper.sSkillInfo.iaSkillUseStamina[1] * ( cSkillWrapper.iLevel - 1 ) );

				int iDecreaseSP = ( iSP * READDWORD( 0x035D0C64 ) ) / 100;

				AddString( 1, FormatString( "%d(-%d)", iSP, iDecreaseSP ) );
			}
			else
				AddString( 1, FormatString( "%d", cSkillWrapper.sSkillInfo.iaSkillUseStamina[0] + ( cSkillWrapper.sSkillInfo.iaSkillUseStamina[1] * ( cSkillWrapper.iLevel - 1 ) ) ) );

			SetLineColor( D3DCOLOR_XRGB( 234, 186, 81 ), 1 );
		}
	}

	//Cooldown Skill
	if( !bInfoBoxSkillTimer && (cSkillWrapper.iLevel > 0) && (cSkillWrapper.sSkillInfo.eSkillType != SKILLTYPE_Passive) && cSkillWrapper.iID != SKILLID_DefaultAttack )
	{
		int iMastery = cSkillWrapper.Mastery;

		if ( iMastery > MAX_MASTERY )
			iMastery = MAX_MASTERY;
		else if ( iMastery < 2 )
			iMastery = 2;

		float fSkillCooldown = ((MAX_MASTERY / (MAX_MASTERY / ((float)iMastery / 2.0f))) * 35) * (1000.0f / 60.0f);
		fSkillCooldown /= 1000.0f;

		AddString( 0, "Cooldown:", D3DCOLOR_XRGB( 243, 237, 216 ) );
		AddString( 1, FormatString( "%.2fsec", fSkillCooldown ), D3DCOLOR_XRGB( 234, 186, 81 ) );
	}

	//Next Level Cost
	if( cSkillWrapper.iLevel < 10 && !strSkillNextLevelCost.empty() )
	{
		AddString( 0, "Upgrade Cost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
		AddString( 1, strSkillNextLevelCost, D3DCOLOR_XRGB( 234, 186, 81 ) );
	}

	//Time Left
	if( bInfoBoxSkillTimer && (cSkillWrapper.iID & 0xFFFF0000 ) != 0x096A0000 && cSkillWrapper.iID != SKILLID_AbsorptionOfGlory && cSkillWrapper.iID != SKILLID_PowerSiege && cSkillWrapper.iID != SKILLID_EvasionAdaption )
	{
		int iTimeLeft = 0;

		if( (cSkillWrapper.iCheckTime / 70) > 0 )
			iTimeLeft = cSkillWrapper.iUseTime - (cSkillWrapper.iCheckTime / 70);
		else
			iTimeLeft = (cSkillWrapper.iCheckTime / 70) - cSkillWrapper.iUseTime;

		int iHour = abs( iTimeLeft / 3600 );
		int iMinutes = abs( (iTimeLeft) / 60 ) % 60;
		int iSeconds = abs( (iTimeLeft) % 60 );

		AddString( 0, "\n" );
		AddString( 0, "Time Left:", D3DCOLOR_XRGB( 243, 237, 216 ) );
		AddString( 1, FormatString("%02d:%02d:%02d", iHour, iMinutes, iSeconds), D3DCOLOR_XRGB( 234, 186, 81 ) );
	}

	return TRUE;
}

void SkillInfoBox::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( !READDWORD( 0x367DF48 ) && !bInfoBoxSkillTimer )
		return;

	if( bInfoBoxSkillTimer && !READDWORD( 0x0367DE8C ) )
		return;

	//Set iWidth and iHeight of Box
	int iSkillInfoBoxWidth = 264;

	//Adjust Box Position
	if( !bInfoBoxSkillTimer )
	{
		iX = sSkillBox.iX - ( iSkillInfoBoxWidth >> 1 ) + 20;
		iY = sSkillBox.iY - iSkillInfoBoxHeight;
	}
	else
	{
		iX = sSkillBox.iX;
		iY = sSkillBox.iY;
	}

	//iItemSlot of box is out the window?
	if( iX < 0 )
		iX = 0;

	if( iY < 0 )
		iY = 16;

	if( iX + iSkillInfoBoxWidth > (int)RESOLUTION_WIDTH )
		iX -= ( ( iX + iSkillInfoBoxWidth ) - RESOLUTION_WIDTH ) + 16;

	//Fix position
	if( !bInfoBoxSkillTimer )
		iY += RESOLUTION_HEIGHT - 600;

	//Draw Skill Info Box Background
	DrawSkillInfoBox( iX, iY + 20, iSkillInfoBoxWidth, iSkillInfoBoxHeight );

	//Drawing Border
	UI::ImageRender::Render( pImageBorderLeftTop, iX, iY + 10, pImageBorderLeftTop->GetWidth(), pImageBorderLeftTop->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderTop, iX + pImageBorderLeftTop->GetWidth(), iY + 10, iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderTop->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderRightTop, iX + pImageBorderLeftTop->GetWidth() + iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), iY + 10, pImageBorderRightTop->GetWidth(), pImageBorderRightTop->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderLeft, iX, iY + 10 + pImageBorderLeftTop->GetHeight(), pImageBorderLeft->GetWidth(), iSkillInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
	UI::ImageRender::Render( pImageBorderRight, iX + iSkillInfoBoxWidth - pImageBorderRight->GetWidth(), iY + 10 + pImageBorderLeftTop->GetHeight(), pImageBorderRight->GetWidth(), iSkillInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1 );
	UI::ImageRender::Render( pImageBorderLeftBottom, iX, iY + 10 + iSkillInfoBoxHeight - pImageBorderLeftBottom->GetHeight(), pImageBorderLeftBottom->GetWidth(), pImageBorderLeftBottom->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderRightBottom, iX + iSkillInfoBoxWidth - pImageBorderRightBottom->GetWidth(), iY + 10 + iSkillInfoBoxHeight - pImageBorderRightBottom->GetHeight(), pImageBorderRightBottom->GetWidth(), pImageBorderRightBottom->GetHeight(), -1 );
	UI::ImageRender::Render( pImageBorderBottom, iX + pImageBorderLeftBottom->GetWidth(), iY + 10 + iSkillInfoBoxHeight - pImageBorderBottom->GetHeight(), iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderBottom->GetHeight(), -1 );

	//Render Text
	int iAddTextHeight = 0;
	if( vLeftText.size() == vRightText.size() )
	{
		for( size_t i = 0; i < vLeftText.size(); i++ )
		{
			Text_ptr pLeftText = vLeftText[i];
			Text_ptr pRightText = vRightText[i];

			//Load Texts
			pLeftText->SetNoClip( TRUE );
			pRightText->SetNoClip( TRUE );

			pLeftText->SetHorizontalAlign( ALIGN_Left );
			pRightText->SetHorizontalAlign( ALIGN_Left );

			//Render Texts
			pLeftText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
			pRightText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}

		if( !bInfoBoxSkillTimer )
		{
			for( size_t i = 0; i < vCompareText.size(); i++ )
			{
				Text_ptr pCompareText = vCompareText[i];
				if( pCompareText )
				{
					pCompareText->SetNoClip( TRUE );
					pCompareText->SetHorizontalAlign( ALIGN_Left );
					pCompareText->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
				}
			}
		}
	}

	//Render Correct Item Group
	if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[0] != ITEMTYPE_None && uImageIconsItemsY > 0 )
	{
		for( size_t i = 0; i < 8; i++ )
		{
			if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Axe )
				UI::ImageRender::Render( pImageIconItems[1], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Wand )
				UI::ImageRender::Render( pImageIconItems[2], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Hammer )
				UI::ImageRender::Render( pImageIconItems[3], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Shield )
				UI::ImageRender::Render( pImageIconItems[4], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Scythe )
				UI::ImageRender::Render( pImageIconItems[5], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Sword )
				UI::ImageRender::Render( pImageIconItems[6], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Claw )
				UI::ImageRender::Render( pImageIconItems[7], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Bow )
				UI::ImageRender::Render( pImageIconItems[8], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Javelin )
				UI::ImageRender::Render( pImageIconItems[9], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Dagger )
				UI::ImageRender::Render( pImageIconItems[10], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
			else if( cSkillWrapper.sSkillInfo.eSkillItemAllowed[i] == ITEMTYPE_Phantom )
				UI::ImageRender::Render( pImageIconItems[11], iX + 13 + ( i * 20 ), iY + uImageIconsItemsY, 18, 24, -1 );
		}
	}
}

void SkillInfoBox::DrawSkillInfoBox( int iX, int iY, int iWidth, int iHeight )
{
	//Draw Item Info Box Background
	if ( pRenderTargetMask )
	{
		if ( GetGraphicsDevice()->SetRenderTarget( pRenderTargetMask ) )
		{
			//Clear Render Target
			GetGraphicsDevice()->Clear( TRUE, TRUE, FALSE );

			//Draw Background
			GRAPHICENGINE->DrawRectangle2D( Rectangle2D( 0, 0, iWidth, iHeight ), D3DCOLOR_ARGB( 235, 45, 39, 27 ), D3DCOLOR_ARGB( 245, 9, 23, 22 ) );

			//Drawing Mask Border
			UI::ImageRender::Render( pImageMaskLeftTop, 0, 0, pImageMaskLeftTop->GetWidth(), pImageMaskLeftTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightTop, iWidth - pImageMaskRightTop->GetWidth(), 0, pImageMaskRightTop->GetWidth(), pImageMaskRightTop->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskLeftBottom, 0, iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskLeftBottom->GetWidth(), pImageMaskLeftBottom->GetHeight(), -1 );
			UI::ImageRender::Render( pImageMaskRightBottom, iWidth - pImageMaskRightTop->GetWidth(), iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskRightBottom->GetWidth(), pImageMaskRightBottom->GetHeight(), -1 );

			//Back Old Render Target
			GetGraphicsDevice()->UnsetRenderTarget();
		}

		//Set Sprite Effect...
		UI::ImageRender::SetEffect( pEffectMask );
		pEffectMask->SetTechnique( "Mask" );

		if ( pEffectMask->Begin() >= 0 )
		{
			//Render the Render Target and mask it on effect
			UI::ImageRender::Render( pRenderTargetMask->GetTexture(), iX, iY - 10, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB( 255, 255, 255 ), FALSE );

			pEffectMask->End();
		}


		UI::ImageRender::SetEffect( NULL );
	}
}

void SkillInfoBox::NewFormatSkillInfo()
{
	FormatSkillInfoFighter();
	FormatSkillInfoMechanician();
	FormatSkillInfoArcher();
	FormatSkillInfoPikeman();
	FormatSkillInfoAtalanta();
	FormatSkillInfoKnight();
	FormatSkillInfoMagician();
	FormatSkillInfoPriestess();
	FormatSkillInfoAssassin();
	FormatSkillInfoShaman();
}

void SkillInfoBox::FormatSkillInfoFighter()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_MeleeMastery:
			AddString( 0, "Weapon Attack Power Add:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT11WeaponAttackPowerAdd[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT11WeaponAttackPowerAdd[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FireAttribute:
			AddString( 0, "Fire Resistance Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaFighterT12FireResistanceIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaFighterT12FireResistanceIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Raving:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT13DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT13NumberofHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "HP Decrease:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f%%", sSkillTable.faFighterT13HPDecrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT13DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT13NumberofHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1f%%)", sSkillTable.faFighterT13HPDecrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Impact:
			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaFighterT14AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT14DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaFighterT14AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT14DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_TripleImpact:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT21DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT21NumberofHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT21DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT21NumberofHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_BrutalSwing:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT22DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Critical:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT22AddCritical[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT22DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT22AddCritical[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Roar:
			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT23AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaFighterT23Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT23AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaFighterT23Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RageofZecram:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT24DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT24AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT24DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT24AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Concentration:
			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT31AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaFighterT31Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT31AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaFighterT31Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_AvengingCrash:
			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaFighterT32AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT32DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaFighterT32AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT32DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SwiftAxe:
			AddString( 0, "Attack Speed Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT33AttackSpeedBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaFighterT33Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT33AttackSpeedBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaFighterT33Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_BoneCrash:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT34DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost vs Demons:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT34DamageBoostvsDemons[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT34DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT34DamageBoostvsDemons[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Destroyer:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT41DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Critical:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT41AddCritical[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT41DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT41AddCritical[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Berserker:
			AddString( 0, "Add Base Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaFighterT42AddBaseAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Decrease Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT42DecreaseAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaFighterT42Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaFighterT42AddBaseAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT42DecreaseAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaFighterT42Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_CycloneStrike:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT43DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Maximum Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT43MaximumHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT43Range[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT43DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT43MaximumHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT43Range[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_BoostHealth:
			AddString( 0, "HP Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaFighterT44HPIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaFighterT44HPIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SurvivalInstinct:
			AddString( 0, "Add HP Regen:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f", sSkillTable.faFighterT51AddHPRegen[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Boost HP Potion Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT51BoostHPPotionRecovery[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Boost HP Capacity:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT51BoostHPCapacity[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%.1f)", sSkillTable.faFighterT51AddHPRegen[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT51BoostHPPotionRecovery[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT51BoostHPCapacity[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SeismicImpact:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaFighterT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Impact Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaFighterT52ImpactArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaFighterT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaFighterT52ImpactArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
	}
}

void SkillInfoBox::FormatSkillInfoMechanician()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_ExtremeShield:
			AddString( 0, "Shield Block Boost (% from Current Shield Block):", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT11ShieldBlockBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT11Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT11ShieldBlockBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT11Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MechanicBomb:
			AddString( 0, "Static Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT12StaticDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT12AddAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMechanicianT12AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT12StaticDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT12AddAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMechanicianT12AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PoisonAttribute:
			AddString( 0, "Poison Resistance Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT13PoisonResistanceIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT13PoisonResistanceIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PhysicalAbsorb:
			AddString( 0, "Absorption Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT14AbsorptionBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT14Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT14AbsorptionBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT14Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GreatSmash:
			AddString( 0, "Calculated Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT21CalculatedDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMechanicianT21AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT21CalculatedDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMechanicianT21AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Maximize:
			AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT22AddAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT22AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT22Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT22AddAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT22AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT22Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Automation:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT23DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT23AddAttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Drain per hit:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f", sSkillTable.faMechanicianT23MPDrainperhit[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Drain per hit:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1f", sSkillTable.faMechanicianT23SPDrainperhit[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT23DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT23AddAttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1f)", sSkillTable.faMechanicianT23MPDrainperhit[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1f)", sSkillTable.faMechanicianT23SPDrainperhit[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Spark:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT24DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMechanicianT24AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT24DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMechanicianT24AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MetalArmor:
			AddString( 0, "Armor Defense Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT31ArmorDefenseRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT31Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT31ArmorDefenseRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT31Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GrandSmash:
			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT32AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT32DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT32AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT32DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MechanicWeaponMastery:
			AddString( 0, "Base Attack Power Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT33BaseAttackPowerIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT33BaseAttackPowerIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SparkShield:
			AddString( 0, "Spark Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT34SparkDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Defense Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT34AddDefenseRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT34Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT34SparkDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT34AddDefenseRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT34Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Impulsion:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT41DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMechanicianT41AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Max Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMechanicianT41MaxHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT41DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMechanicianT41AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMechanicianT41MaxHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Compulsion:
			AddString( 0, "Increase Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT42IncreaseAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Boost Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT42BoostAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Monster Sight Range Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT42MonsterSightRangeIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT42Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT42IncreaseAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT42BoostAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT42MonsterSightRangeIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT42Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MagneticSphere:
			AddString( 0, "Spheres Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT43SpheresDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Spheres Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMechanicianT43SpheresAttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Spheres Attack Delay:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT43SpheresAttackDelay[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT43SpheresDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMechanicianT43SpheresAttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT43SpheresAttackDelay[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MetalGolem:
			AddString( 0, "Summon Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMechanicianT44SummonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT44SummonAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT44SummonAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT44SummonAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Defense:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT44SummonDefense[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMechanicianT44SummonHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMechanicianT44SummonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT44SummonAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT44SummonAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT44SummonAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT44SummonDefense[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMechanicianT44SummonHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Rupture:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT51DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT51DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MagneticDischarge:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMechanicianT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMechanicianT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
	}
}

void SkillInfoBox::FormatSkillInfoArcher()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_ScoutHawk:
			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT11AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Hawk Rotations:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%drounds", sSkillTable.iaArcherT11HawkRotations[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT11AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%drounds)", sSkillTable.iaArcherT11HawkRotations[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ShootingMastery:
			AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT12AddAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT12AddAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_WindArrow:
			AddString( 0, "Add Attack Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT13AddAttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT13DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT13AddAttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT13DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PerfectAim:
			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT14AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT14DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT14AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT14DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DionsEye:
			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT21AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT21AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Falcon:
			AddString( 0, "Static Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT22StaticDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Formula Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT22FormulaDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaArcherT22Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT22StaticDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT22FormulaDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaArcherT22Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ArrowofRage:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT23DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaArcherT23AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT23DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaArcherT23AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Avalanche:
			AddString( 0, "Add Attack Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaArcherT24AddAttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT24DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Arrows:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaArcherT24NumberofArrows[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaArcherT24AddAttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT24DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaArcherT24NumberofArrows[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ElementalShot:
			AddString( 0, "Fire Damage Static:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT31FireDamageStatic[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Fire Damage Calculated:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT31FireDamageCalculated[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Lightning Damage Static:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT31LightningDamageStatic[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Lightning Damage Calculated:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT31LightningDamageCalculated[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT31FireDamageStatic[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT31FireDamageCalculated[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT31LightningDamageStatic[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT31LightningDamageCalculated[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GoldenFalcon:
			AddString( 0, "Falcon Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT32FalconDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "HP Regen:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%.1f", sSkillTable.faArcherT32HPRegen[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaArcherT32Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT32FalconDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%.1f)", sSkillTable.faArcherT32HPRegen[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaArcherT32Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_BombShot:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT33DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost vs Demon Monsters:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaArcherT33DamageBoostvsDemonMonsters[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaArcherT33AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT33DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaArcherT33DamageBoostvsDemonMonsters[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaArcherT33AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Perforation:
			AddString( 0, "Increased Shot Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT34IncreasedShotRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT34DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT34IncreasedShotRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT34DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RecallWolverine:
			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaArcherT41Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT41SummonAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT41SummonAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT41SummonAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Defense:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT41SummonDefense[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT41SummonHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaArcherT41Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT41SummonAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT41SummonAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT41SummonAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT41SummonDefense[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT41SummonHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_EvasionMastery:
			AddString( 0, "Increase Evasion:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaArcherT42IncreaseEvasion[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaArcherT42IncreaseEvasion[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PhoenixShot:
			AddString( 0, "Shot Range Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT43ShotRangeIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaArcherT43DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Charge Raise Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1fsec", sSkillTable.faArcherT43ChargeRaiseSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT43ShotRangeIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaArcherT43DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1fsec)", sSkillTable.faArcherT43ChargeRaiseSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ForceofNature:
			AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT44AddAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT44AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaArcherT44Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT44AddAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT44AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaArcherT44Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_LethalSight:
			AddString( 0, "Add Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaArcherT51AddRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Critical Rate Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaArcherT51CriticalRateIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "After Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaArcherT51AfterDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaArcherT51AddRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaArcherT51CriticalRateIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaArcherT51AfterDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
	}
}

void SkillInfoBox::FormatSkillInfoPikeman()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_PikeWind:
			AddString( 0, "Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPikemanT11AttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPikemanT11AttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Pushback Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPikemanT11PushbackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPikemanT11AttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPikemanT11AttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPikemanT11PushbackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_IceAttribute:
			AddString( 0, "Frost Resistance Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPikemanT12FrostResistanceIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPikemanT12FrostResistanceIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_CriticalHit:
			AddString( 0, "Add Critical Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPikemanT13AddCriticalDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPikemanT13AddCriticalDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_JumpingCrash:
			AddString( 0, "Weapon Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT14WeaponAttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT14DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT14WeaponAttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT14DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GroundPike:
			AddString( 0, "Freeze Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPikemanT21FreezeDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Freeze Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPikemanT21FreezeAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Freeze Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPikemanT21FreezeAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Freeze Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPikemanT21FreezeAttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPikemanT21FreezeDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPikemanT21FreezeAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPikemanT21FreezeAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPikemanT21FreezeAttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Tornado:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT22DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPikemanT22Range[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT22DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPikemanT22Range[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_WeaponDefenseMastery:
			AddString( 0, "Weapon Current Block Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT23WeaponCurrentBlockBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT23WeaponCurrentBlockBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Expansion:
			AddString( 0, "Add Size:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT24AddSize[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT24DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT24AddSize[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT24DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_VenomSpear:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT31DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Spears:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dspears", sSkillTable.iaPikemanT31NumberofSpears[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPikemanT31EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPikemanT31EffectRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT31DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dspears)", sSkillTable.iaPikemanT31NumberofSpears[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPikemanT31EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPikemanT31EffectRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Vanish:
			AddString( 0, "After Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPikemanT32AfterDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Monster Sight Decrease:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT32MonsterSightDecrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaPikemanT32Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPikemanT32AfterDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT32MonsterSightDecrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaPikemanT32Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_CriticalMastery:
			AddString( 0, "Weapon Critical Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT33WeaponCriticalRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT33WeaponCriticalRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ChainLance:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT34DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT34DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_AssassinsEye:
			AddString( 0, "Add Critical Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPikemanT41AddCriticalDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Target Critical Decrease:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT41TargetCriticalDecrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Debuff Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPikemanT41DebuffDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPikemanT41AddCriticalDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT41TargetCriticalDecrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPikemanT41DebuffDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ChargingStrike:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT42DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Charged Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPikemanT42ChargedDamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Raise Charging Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPikemanT42RaiseChargingSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT42DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPikemanT42ChargedDamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPikemanT42RaiseChargingSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Vague:
			AddString( 0, "Add Evasion:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPikemanT43AddEvasion[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaPikemanT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPikemanT43AddEvasion[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaPikemanT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ShadowMaster:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT44DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dhits", sSkillTable.iaPikemanT44NumberofHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT44DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dhits)", sSkillTable.iaPikemanT44NumberofHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RingofSpears:
			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPikemanT51Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPikemanT51Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_LastBreath:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPikemanT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Cooldown:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1fsec", sSkillTable.faPikemanT52Cooldown[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPikemanT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1fsec)", sSkillTable.faPikemanT52Cooldown[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
	}
}

void SkillInfoBox::FormatSkillInfoAtalanta()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_ShieldStrike:
			AddString( 0, "Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT11AttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT11AttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT11AttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT11AttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Farina:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT12DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Speed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT12AddAttackSpeed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT12AddAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT12DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT12AddAttackSpeed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT12AddAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ThrowingMaster:
			AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT13AddAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT13AddAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_BiggerSpear:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT14DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT14DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Windy:
			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT21AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT21AddRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT21Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT21AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT21AddRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT21Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_TwistJavelin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT22DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT22AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT22DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT22AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SoulSucker:
			AddString( 0, "Monster Life Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT23MonsterLifeAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT23MonsterLifeAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FireJavelin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT24DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT24DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SplitJavelin:
			AddString( 0, "Number of Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAtalantaT31NumberofHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT31DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT31AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAtalantaT31NumberofHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT31DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT31AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_TriumphofJavelin:
			AddString( 0, "Max Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT32MaxAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT32Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT32MaxAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT32Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_LightningJavelin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT33DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT33DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_StormJavelin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT34DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT34DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HallofValhalla:
			AddString( 0, "Add Evasion:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT41AddEvasion[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT41Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT41AddEvasion[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT41Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ExtremeRage:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT42DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAtalantaT42AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT42DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAtalantaT42AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FrostJavelin:
			AddString( 0, "Add Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT43AddAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT43AddAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Target Running Speed Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT43TargetRunningSpeedReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Target Freeze Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT43TargetFreezeEffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT43AddAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT43AddAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT43TargetRunningSpeedReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT43TargetFreezeEffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Vengeance:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT44DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT44AddRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT44DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT44AddRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SummonTiger:
			AddString( 0, "Summon Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT51SummonAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT51SummonAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT51SummonAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaAtalantaT51SummonHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT51SummonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT51SummonAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT51SummonAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT51SummonAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaAtalantaT51SummonHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT51SummonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GoldenApple:
			AddString( 0, "Speed Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT52SpeedIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Evasion:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT52AddEvasion[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT52SpeedIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT52AddEvasion[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PlagueJavelin:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAtalantaT53DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Targets:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAtalantaT53Targets[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Poison Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAtalantaT53PoisonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAtalantaT53DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAtalantaT53Targets[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAtalantaT53PoisonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
	}
}

void SkillInfoBox::FormatSkillInfoKnight()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_SwordBlast:
			AddString( 0, "Static Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaKnightT11StaticDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT11DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Linear Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT11LinearAreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaKnightT11StaticDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT11DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT11LinearAreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyBody:
			AddString( 0, "Damage Boost vs Undead Monsters:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT12DamageBoostvsUndeadMonsters[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT12Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT12DamageBoostvsUndeadMonsters[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT12Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PhysicalTraining:
			AddString( 0, "SP Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT13SPBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT13SPBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DoubleCrash:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT14DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Critical:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaKnightT14AddCritical[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT14DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaKnightT14AddCritical[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyValor:
			AddString( 0, "Party Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT21PartyArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Final Damage Boost vs Undead Enemies:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaKnightT21FinalDamageBoostvsUndeadEnemies[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT21Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT21PartyArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaKnightT21FinalDamageBoostvsUndeadEnemies[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT21Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Brandish:
			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT22AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT22DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT22AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT22DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Piercing:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT23DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT23AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Pierce Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT23PierceRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT23DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT23AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT23PierceRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DrasticSpirit:
			AddString( 0, "Defense Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT24DefenseBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT24Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT24DefenseBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT24Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SwordMastery:
			AddString( 0, "Base Attack Power Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT31BaseAttackPowerIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT31BaseAttackPowerIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DivineShield:
			AddString( 0, "Add Block Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaKnightT32AddBlockRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Monster Damage Into HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT32MonsterDamageIntoHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT32Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaKnightT32AddBlockRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT32MonsterDamageIntoHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT32Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyIncantation:
			AddString( 0, "Success Rate:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT33SuccessRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Monster Life Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaKnightT33MonsterLifeIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Enslave Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT33EnslaveEffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT33SuccessRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaKnightT33MonsterLifeIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT33EnslaveEffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GrandCross:
			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT34AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT34DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost vs Undead Enemies:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT34DamageBoostvsUndeadEnemies[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT34AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT34DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT34DamageBoostvsUndeadEnemies[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SwordofJustice:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT41DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT41AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT41DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT41AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GodlyShield:
			AddString( 0, "Shield Absorption Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT42ShieldAbsorptionBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT42Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT42ShieldAbsorptionBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT42Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GodsBlessing:
			AddString( 0, "Add Attack Power:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT43AddAttackPower[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT43AddAttackPower[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DivinePiercing:
			AddString( 0, "Add Critical:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaKnightT44AddCritical[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT44DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT44NumberofHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaKnightT44AddCritical[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT44DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT44NumberofHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyConviction:
			AddString( 0, "Undead Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT51UndeadDamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT51UndeadDamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_WhirlWind:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT52AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Atk Rtg Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT52AddAtkRtgBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaKnightT52Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Cooldown:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%.1fsec", sSkillTable.faKnightT52Cooldown[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT52AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT52AddAtkRtgBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaKnightT52Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%.1fsec)", sSkillTable.faKnightT52Cooldown[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GloriousShield:
			AddString( 0, "Shield Defense Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaKnightT53ShieldDefenseBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaKnightT53ShieldDefenseBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DivineCross:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaKnightT54DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaKnightT54DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
	}
}

void SkillInfoBox::FormatSkillInfoMagician()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_Agony:
			AddString( 0, "HP Sacrificed:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT11HPSacrificed[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Increased:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaMagicianT11MPIncreased[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT11HPSacrificed[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaMagicianT11MPIncreased[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FireBolt:
			AddString( 0, "Static Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT12StaticDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT12DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT12AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT12StaticDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT12DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT12AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Zenith:
			AddString( 0, "Add Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT13AddAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Elemental Resistance:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT13AddElementalResistance[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT13Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT13AddAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT13AddElementalResistance[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT13Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FireBall:
			AddString( 0, "Static Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT14StaticDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT14DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT14AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT14AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT14StaticDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT14DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT14AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT14AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MentalMastery:
			AddString( 0, "MP Potion Recovery Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaMagicianT21MPPotionRecoveryBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Capacity Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaMagicianT21MPCapacityIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaMagicianT21MPPotionRecoveryBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaMagicianT21MPCapacityIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Watornado:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT22DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT22AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT22AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT22DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT22AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT22AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_EnchantWeapon:
			AddString( 0, "Fire Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT23FireAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Fire Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT23FireAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Ice Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT23IceAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Ice Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT23IceAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Lightning Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT23LightningAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Lightning Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT23LightningAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT23Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT23FireAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT23FireAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT23IceAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT23IceAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT23LightningAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT23LightningAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT23Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DeathRay:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT24DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT24AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT24DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT24AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_EnergyShield:
			AddString( 0, "Reduce Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT31ReduceDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT31Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT31ReduceDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT31Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Diastrophism:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT32DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT32AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT32DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT32AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SpiritElemental:
			AddString( 0, "Add Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT33AddAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT33AddAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT33Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT33AddAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT33AddAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT33Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DancingSword:
			AddString( 0, "Fire Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT34FireDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Ice Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT34IceDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Sword Attack Delay:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT34SwordAttackDelay[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT34Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT34FireDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT34IceDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT34SwordAttackDelay[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT34Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FireElemental:
			AddString( 0, "Summon Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT41SummonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT41SummonAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT41SummonAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT41SummonAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaMagicianT41SummonHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT41SummonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT41SummonAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT41SummonAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT41SummonAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaMagicianT41SummonHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FlameWave:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT42DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT42AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT42DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT42AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Distortion:
			AddString( 0, "Target Speed Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT43TargetSpeedReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT43AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT43EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT43TargetSpeedReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT43AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT43EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Meteorite:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT44DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT44AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT44DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT44AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_WizardTrance:
			AddString( 0, "Orb Block Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaMagicianT51OrbBlockBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Rate Reduced:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT51AttackRateReduced[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaMagicianT51OrbBlockBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT51AttackRateReduced[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_StoneSkin:
			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT52EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Run Speed Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT52RunSpeedReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT52EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT52RunSpeedReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RedRay:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT53DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Increased Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT53IncreasedAttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT53DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT53IncreasedAttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Cataclysm:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaMagicianT54DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaMagicianT54AttackDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT54MPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Drain:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaMagicianT54SPDrain[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaMagicianT54DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaMagicianT54AttackDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT54MPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaMagicianT54SPDrain[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;

	}
}

void SkillInfoBox::FormatSkillInfoPriestess()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_Healing:
			AddString( 0, "Formula Based Life Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPriestessT11FormulaBasedLifeRecovery[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power Based Life Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT11AttackPowerBasedLifeRecovery[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPriestessT11FormulaBasedLifeRecovery[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT11AttackPowerBasedLifeRecovery[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyBolt:
			AddString( 0, "Formula Based Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPriestessT12FormulaBasedDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT12DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPriestessT12FormulaBasedDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT12DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MultiSpark:
			AddString( 0, "Formula Based Damage per Hit:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPriestessT13FormulaBasedDamageperHit[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power Based Damage per Hit:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPriestessT13AttackPowerBasedDamageperHit[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Min Number of Sparks:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsparks", sSkillTable.iaPriestessT13MinNumberofSparks[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Max Number of Sparks:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsparks", sSkillTable.iaPriestessT13MaxNumberofSparks[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPriestessT13FormulaBasedDamageperHit[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPriestessT13AttackPowerBasedDamageperHit[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsparks)", sSkillTable.iaPriestessT13MinNumberofSparks[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsparks)", sSkillTable.iaPriestessT13MaxNumberofSparks[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyMind:
			AddString( 0, "Target Reduce Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT14TargetReduceDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPriestessT14EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT14TargetReduceDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPriestessT14EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Meditation:
			AddString( 0, "Add MP Potion Boost Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT21AddMPPotionBoostRecovery[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add MP Regeneration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPriestessT21AddMPRegeneration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT21AddMPPotionBoostRecovery[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPriestessT21AddMPRegeneration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DivineLightning:
			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT22AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT22DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Bolts:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT22NumberofBolts[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT22AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT22DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT22NumberofBolts[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HolyReflection:
			AddString( 0, "Monster Damage Reflected:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT23MonsterDamageReflected[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPriestessT23Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT23MonsterDamageReflected[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPriestessT23Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GrandHealing:
			AddString( 0, "Formula Based Life Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaPriestessT24FormulaBasedLifeRecovery[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power Based Life Recovery:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPriestessT24AttackPowerBasedLifeRecovery[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT24Range[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaPriestessT24FormulaBasedLifeRecovery[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPriestessT24AttackPowerBasedLifeRecovery[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT24Range[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_VigorBall:
			AddString( 0, "Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT31AttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT31DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT31AttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT31DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Resurrection:
			AddString( 0, "Attainable Success Rate:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT32AttainableSuccessRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Max Players Resurrected:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dplayers", sSkillTable.iaPriestessT32MaxPlayersResurrected[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT32AttainableSuccessRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dplayers)", sSkillTable.iaPriestessT32MaxPlayersResurrected[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Extinction:
			AddString( 0, "Attainable Success Rate:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT33AttainableSuccessRate[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Monster Health Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT33MonsterHealthReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT33AttainableSuccessRate[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT33MonsterHealthReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_VirtualLife:
			AddString( 0, "Increase Life:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaPriestessT34IncreaseLife[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPriestessT34Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaPriestessT34IncreaseLife[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPriestessT34Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GlacialSpike:
			AddString( 0, "Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT41AttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT41AttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT41AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Freeze Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT41FreezeDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT41AttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT41AttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT41AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT41FreezeDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RegenerationField:
			AddString( 0, "Add HP Regen:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT42AddHPRegen[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add MP Regen:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT42AddMPRegen[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPriestessT42Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT42Range[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT42AddHPRegen[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT42AddMPRegen[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPriestessT42Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT42Range[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ChainLightning:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT43DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Number of Bolts:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT43NumberofBolts[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT43Range[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT43DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT43NumberofBolts[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT43Range[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SummonMuspell:
			AddString( 0, "Muspell Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT44MuspellDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT44AddAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT44AddAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Virtual Life Effect Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT44VirtualLifeEffectReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Muspell Attack Delay:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPriestessT44MuspellAttackDelay[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Undead Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT44AddUndeadAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Evasion:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT44AddEvasion[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT44MuspellDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT44AddAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT44AddAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT44VirtualLifeEffectReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPriestessT44MuspellAttackDelay[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT44AddUndeadAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT44AddEvasion[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_IceElemental:
			AddString( 0, "Summon Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT51SummonAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT51SummonAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT51SummonAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Health Points:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT51SummonHealthPoints[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT51SummonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT51SummonAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT51SummonAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT51SummonAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT51SummonHealthPoints[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT51SummonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_IceMeteorite:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT52AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, " Meteorites:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT52Meteorites[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT52AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT52Meteorites[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_HeavenlyLight:
			AddString( 0, "Target Attack Rating Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT53TargetAttackRatingReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Debuff Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaPriestessT53DebuffEffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Charged Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT53ChargedArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT53TargetAttackRatingReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaPriestessT53DebuffEffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT53ChargedArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Consecration:
			AddString( 0, "Increase Defense:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaPriestessT54IncreaseDefense[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Increase Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaPriestessT54IncreaseAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaPriestessT54IncreaseDefense[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaPriestessT54IncreaseAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;

	}
}

void SkillInfoBox::FormatSkillInfoAssassin()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_Stinger:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT11DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT11DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RunningHit:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT12DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT12DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DualWieldMastery:
			AddString( 0, "Weapon Attack Power Add:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT13WeaponAttackPowerAdd[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT13WeaponAttackPowerAdd[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Wisp:
			AddString( 0, "Reduce Received Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT14ReduceReceivedDamageFromTarget[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAssassinT14AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAssassinT14EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT14ReduceReceivedDamageFromTarget[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAssassinT14AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAssassinT14EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_VenomThorn:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT21DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT21DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Alas:
			AddString( 0, "Add Evasion:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT22AddEvasion[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaAssassinT22Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT22AddEvasion[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaAssassinT22Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SoulShock:
			AddString( 0, "Radius Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAssassinT23RadiusAreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Stun Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAssassinT23StunEffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAssassinT23RadiusAreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAssassinT23StunEffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_AttackMastery:
			AddString( 0, "Weapon Attack Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT24WeaponAttackRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Weapon Block Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT24WeaponBlockRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT24WeaponAttackRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT24WeaponBlockRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SoreBlade:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT31DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT31DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_GustSlash:
			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAssassinT32AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaAssassinT32DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAssassinT32AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaAssassinT32DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Inpes:
			AddString( 0, "Attack Speed Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaAssassinT33AttackSpeedBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaAssassinT33Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaAssassinT33AttackSpeedBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaAssassinT33Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Blind:
			AddString( 0, "After Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaAssassinT34AfterDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Monster Sight Decrease:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT34MonsterSightDecrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dseg", sSkillTable.iaAssassinT34Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaAssassinT34AfterDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT34MonsterSightDecrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dseg)", sSkillTable.iaAssassinT34Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FrostWind:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT41DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT41DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_FatalMastery:
			AddString( 0, "Weapon Critical Rating Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT42WeaponCriticalRatingBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT42WeaponCriticalRatingBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Polluted:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT43DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Maximum Hits:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAssassinT43MaximumHits[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAssassinT43EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAssassinT43EffectRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT43DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAssassinT43MaximumHits[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAssassinT43EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAssassinT43EffectRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_NinjaShadow:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT44DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT44DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ShadowBlink:
			AddString( 0, "Critical Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT51CriticalDamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attainable Movement Teleport Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaAssassinT51AttainableMovementTeleportRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT51CriticalDamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaAssassinT51AttainableMovementTeleportRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_KissViper:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT52DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Poison Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT52PoisonDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Poison Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaAssassinT52PoisonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT52DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT52PoisonDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaAssassinT52PoisonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ViolentStub:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT53DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT53DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ShadowStorm:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaAssassinT54DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaAssassinT54DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;

	}
}

void SkillInfoBox::FormatSkillInfoShaman()
{
	UINT uSkillLevel = cSkillWrapper.iLevel - 1;

	if ( cSkillWrapper.iLevel == 0 )
		uSkillLevel = 0;

	switch ( cSkillWrapper.iID )
	{
		case SKILLID_DarkBolt:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT11DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT11AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT11DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT11AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_DarkWave:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT12DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT12AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT12AttackArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT12DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT12AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT12AttackArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Inertia:
			AddString( 0, "Enemy Speed Reduction:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT13EnemySpeedReduction[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT13EffectArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Effect Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT13EffectDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT13EnemySpeedReduction[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT13EffectArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT13EffectDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_InnerPeace:
			AddString( 0, "MP Potion Recovery Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaShamanT14MPPotionRecoveryBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Capacity Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaShamanT14MPCapacityBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaShamanT14MPPotionRecoveryBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaShamanT14MPCapacityBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SpiritualFlare:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT21DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT21AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT21AttackArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT21DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT21AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT21AttackArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_SpiritualManacle:
			AddString( 0, "Stun Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT22StunDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT22StunDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_ChasingHunt:
			AddString( 0, "Base Add Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT23BaseAddAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Base Add Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT23BaseAddAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Increase Monster Sight Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT23IncreaseMonsterSightRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT23Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT23BaseAddAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT23BaseAddAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT23IncreaseMonsterSightRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT23Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_AdventMigal:
			AddString( 0, "Add Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaShamanT24AddAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaShamanT24AddAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT24Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaShamanT24AddAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaShamanT24AddAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT24Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Rainmaker:
			AddString( 0, "Reduce Received Damage:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT31ReduceReceivedDamage[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Evasion Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaShamanT31AddEvasionRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Reduce MP Points:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT31ReduceMPPoints[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT31Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT31ReduceReceivedDamage[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaShamanT31AddEvasionRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT31ReduceMPPoints[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT31Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PhantomCall:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT32DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT32AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT32DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT32AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Haunt:
			AddString( 0, "Monster Life Absorption:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT33MonsterLifeAbsorption[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT33MonsterLifeAbsorption[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Scratch:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT34DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT34DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_RecallHestian:
			AddString( 0, "Summon Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT41SummonDuration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Min:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaShamanT41SummonAttackPowerMin[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Power Max:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaShamanT41SummonAttackPowerMax[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon Attack Rating:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaShamanT41SummonAttackRating[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Summon HP:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d", sSkillTable.iaShamanT41SummonHP[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT41SummonDuration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaShamanT41SummonAttackPowerMin[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaShamanT41SummonAttackPowerMax[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaShamanT41SummonAttackRating[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d)", sSkillTable.iaShamanT41SummonHP[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Judgement:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT42DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Add Critical Chance:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT42AddCriticalChance[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT42AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT42DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT42AddCriticalChance[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT42AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_AdventMidranda:
			AddString( 0, "Attack Speed Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaShamanT43AttackSpeedBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT43Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaShamanT43AttackSpeedBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT43Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_MourningPray:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT44DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Area of Effect:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT44AreaofEffect[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT44DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT44AreaofEffect[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_Creed:
			AddString( 0, "MP Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaShamanT51MPBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "SP Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "+%d%%", sSkillTable.iaShamanT51SPBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Duration:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%dsec", sSkillTable.iaShamanT51Duration[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaShamanT51MPBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(+%d%%)", sSkillTable.iaShamanT51SPBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%dsec)", sSkillTable.iaShamanT51Duration[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PressDeity:
			AddString( 0, "Damage Boost Main Target:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT52DamageBoostMainTarget[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Damage Boost Surroundings:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT52DamageBoostSurroundings[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Surrounding Impact Area:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT52SurroundingImpactArea[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT52DamageBoostMainTarget[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT52DamageBoostSurroundings[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT52SurroundingImpactArea[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_PhantomNail:
			AddString( 0, "Damage Boost:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT53DamageBoost[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "Attack Range:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d", sSkillTable.iaShamanT53AttackRange[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT53DamageBoost[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d)", sSkillTable.iaShamanT53AttackRange[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;
		case SKILLID_OccultLife:
			AddString( 0, "HP Increase:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT54HPIncrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );

			AddString( 0, "MP Decrease:", D3DCOLOR_XRGB( 243, 237, 216 ) );
			AddString( 1, FormatString( "%d%%", sSkillTable.iaShamanT54MPDecrease[uSkillLevel] ), D3DCOLOR_XRGB( 238, 210, 151 ) );


			if ( (cSkillWrapper.iLevel > 0) && (cSkillWrapper.iLevel < 10) )
			{
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT54HPIncrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
				AddString( 2, FormatString( "(%d%%)", sSkillTable.iaShamanT54MPDecrease[uSkillLevel + 1] ), D3DCOLOR_XRGB( 92, 163, 99 ) );
			}
			break;

	}
}

}