#include "stdafx.h"
#include "CSkillServerHandler.h"

std::vector<PacketSkillTable> CSkillServerHandler::vSkillTablePackets;

CSkillServerHandler::CSkillServerHandler()
{
}

CSkillServerHandler::~CSkillServerHandler()
{
}

void CSkillServerHandler::Init()
{
	vSkillTablePackets.clear();

	Load();
}

void CSkillServerHandler::Load()
{
	LoadFighter();
	LoadMechanician();
	LoadArcher();
	LoadPikeman();
	LoadAtalanta();
	LoadKnight();
	LoadMagician();
	LoadPriestess();
	LoadAssassin();
	LoadShaman();

	//Save Skill Table
	vSkillTablePackets.clear();

	static PacketSkillTable sPacket;
	ZeroMemory( &sPacket, sizeof( PacketSkillTable ) );

	sPacket.iLength = sizeof( PacketSkillTable );
	sPacket.iHeader = PKTHDR_SkillTable;

	sPacket.sPacketCount.sMin = 1;
	sPacket.sPacketCount.sMax = sizeof( SkillTable ) / SKILLTABLE_LEN;

	if ( sizeof( SkillTable ) % SKILLTABLE_LEN )
		sPacket.sPacketCount.sMax++;

	BYTE * pbSkillTable = (BYTE *)&sSkillTable;

	UINT uPosition = 0;
	while ( uPosition < sizeof( SkillTable ) )
	{
		sPacket.iSize = (sizeof( SkillTable ) - uPosition) >= SKILLTABLE_LEN ? SKILLTABLE_LEN : (sizeof( SkillTable ) - uPosition);

		CopyMemory( sPacket.baBuffer, pbSkillTable + uPosition, sPacket.iSize );

		vSkillTablePackets.push_back( sPacket );

		sPacket.sPacketCount.sMin++;
		uPosition += sPacket.iSize;
	}
}

void CSkillServerHandler::SendSkillTable( User * pcUser )
{
	if ( vSkillTablePackets.size() > 0 )
	{
		for ( auto & sPacket : vSkillTablePackets )
			SENDPACKET( pcUser, &sPacket );
	}
}

void CSkillServerHandler::LoadFighter()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT WeaponAttackPowerAdd FROM MeleeMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT11WeaponAttackPowerAdd[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FireResistanceIncrease FROM FireAttribute ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT12FireResistanceIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, NumberofHits, HPDecrease, MPCost, SPCost FROM Raving ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT13DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT13NumberofHits[i] );
					pcDB->GetData( 3, PARAMTYPE_Float, &sSkillTable.faFighterT13HPDecrease[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT13MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaFighterT13SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackRating, DamageBoost, MPCost, SPCost FROM Impact ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT14AddAttackRating[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT14DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT14MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, NumberofHits, MPCost, SPCost FROM TripleImpact ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT21DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT21NumberofHits[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT21MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddCritical, MPCost, SPCost FROM BrutalSwing ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT22DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT22AddCritical[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT22MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AreaofEffect, Duration, MPCost, SPCost FROM Roar ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT23AreaofEffect[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT23Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT23MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM RageofZecram ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT24DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT24AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT24MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackRating, Duration, MPCost, SPCost FROM Concentration ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT31AddAttackRating[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT31Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT31MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackRating, DamageBoost, MPCost, SPCost FROM AvengingCrash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT32AddAttackRating[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT32DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackSpeedBoost, Duration, MPCost, SPCost FROM SwiftAxe ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT33AttackSpeedBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT33Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT33MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, DamageBoostvsDemons, MPCost, SPCost FROM BoneCrash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT34DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT34DamageBoostvsDemons[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT34MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddCritical, MPCost, SPCost FROM Destroyer ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT41DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT41AddCritical[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT41MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddBaseAttackPower, DecreaseAbsorption, Duration, MPCost, SPCost FROM Berserker ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT42AddBaseAttackPower[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT42DecreaseAbsorption[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT42Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT42MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaFighterT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MaximumHits, Range, MPCost, SPCost FROM CycloneStrike ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT43DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT43MaximumHits[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT43Range[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT43MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaFighterT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT HPIncrease FROM BoostHealth ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT44HPIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddHPRegen, BoostHPPotionRecovery, BoostHPCapacity FROM SurvivalInstinct ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Float, &sSkillTable.faFighterT51AddHPRegen[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT51BoostHPPotionRecovery[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT51BoostHPCapacity[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, ImpactArea, MPCost, SPCost FROM SeismicImpact ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT52ImpactArea[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT52MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT HPDecrease, HitsNeededtoTrigger, FinalDamageBoostIncrease, Duration, MPCost, SPCost FROM MightofZecram ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT53HPDecrease[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT53HitsNeededtoTrigger[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT53FinalDamageBoostIncrease[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT53Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaFighterT53MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaFighterT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, Duration, MPDrain, SPDrain FROM RagingStrikes ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaFighterT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaFighterT54Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaFighterT54MPDrain[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaFighterT54SPDrain[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadMechanician()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT ShieldBlockBoost, Duration, MPCost, SPCost FROM ExtremeShield ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT11ShieldBlockBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT11Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT11MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT StaticDamage, AddAttackPower, AreaofEffect, MPCost, SPCost FROM MechanicBomb ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT12StaticDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT12AddAttackPower[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT12AreaofEffect[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT12MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT PoisonResistanceIncrease FROM PoisonAttribute ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT13PoisonResistanceIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AbsorptionBoost, Duration, MPCost, SPCost FROM PhysicalAbsorption ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT14AbsorptionBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT14Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT14MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT CalculatedDamage, AreaofEffect, MPCost, SPCost FROM GreatSmash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT21CalculatedDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT21AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT21MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPower, AddAttackRating, Duration, MPCost, SPCost FROM Maximize ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT22AddAttackPower[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT22AddAttackRating[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT22Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT22MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddAttackSpeed, MPDrainperhit, SPDrainperhit, MPCost, SPCost FROM Automation ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT23DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT23AddAttackSpeed[i] );
					pcDB->GetData( 3, PARAMTYPE_Float, &sSkillTable.faMechanicianT23MPDrainperhit[i] );
					pcDB->GetData( 4, PARAMTYPE_Float, &sSkillTable.faMechanicianT23SPDrainperhit[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT23MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM Spark ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT24DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT24AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT24MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ArmorDefenseRatingBoost, Duration, MPCost, SPCost FROM MetalArmor ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT31ArmorDefenseRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT31Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT31MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackRatingBoost, DamageBoost, MPCost, SPCost FROM GrandSmash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT32AttackRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT32DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT BaseAttackPowerIncrease FROM MechanicWeaponMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT33BaseAttackPowerIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SparkDamage, AddDefenseRating, Duration, MPCost, SPCost FROM SparkShield ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT34SparkDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT34AddDefenseRating[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT34Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT34MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MaxHits, MPCost, SPCost FROM Impulsion ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT41DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT41AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT41MaxHits[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT41MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT IncreaseAbsorption, BoostAbsorption, MonsterSightRangeIncrease, Duration, MPCost, SPCost FROM Compulsion ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT42IncreaseAbsorption[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT42BoostAbsorption[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT42MonsterSightRangeIncrease[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT42Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT42MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SpheresDamage, SpheresAttackRange, SpheresAttackDelay, Duration, MPCost, SPCost FROM MagneticSphere ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT43SpheresDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT43SpheresAttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT43SpheresAttackDelay[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT43Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT43MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SummonDuration, SummonAttackPowerMin, SummonAttackPowerMax, SummonAttackRating, SummonDefense, SummonHP, MPCost, SPCost FROM MetalGolem ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SummonDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SummonAttackPowerMin[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SummonAttackPowerMax[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SummonAttackRating[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SummonDefense[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SummonHP[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44MPCost[i] );
					pcDB->GetData( 8, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM Rupture ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT51DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT51MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM MagneticDischarge ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT52MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ReceivedDamageReduction, AttackRatingReduction, Duration, MPCost, SPCost FROM HardenedWard ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT53ReceivedDamageReduction[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT53AttackRatingReduction[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT53Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT53MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, PoisonDuration, MPCost, SPCost FROM SwarmShot ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT54PoisonDuration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT54MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMechanicianT54SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadArcher()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AddAttackRating, HawkRotations, MPCost, SPCost FROM ScoutHawk ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT11AddAttackRating[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT11HawkRotations[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT11MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPower FROM ShootingMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT12AddAttackPower[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackSpeed, DamageBoost, MPCost, SPCost FROM WindArrow ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT13AddAttackSpeed[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT13DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT13MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT13SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackRating, DamageBoost, MPCost, SPCost FROM PerfectAim ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT14AddAttackRating[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT14DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT14MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackRating FROM DionsEye ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT21AddAttackRating[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT StaticDamage, FormulaDamage, Duration, MPCost, SPCost FROM Falcon ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT22StaticDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT22FormulaDamage[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT22Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT22MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM ArrowofRage ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT23DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT23AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT23MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackSpeed, DamageBoost, NumberofArrows, MPCost, SPCost FROM Avalanche ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT24AddAttackSpeed[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT24DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT24NumberofArrows[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT24MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FireDamageStatic, FireDamageCalculated, LightningDamageStatic, LightningDamageCalculated, MPCost, SPCost FROM ElementalShot ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT31FireDamageStatic[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT31FireDamageCalculated[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT31LightningDamageStatic[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT31LightningDamageCalculated[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT31MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaArcherT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FalconDamage, HPRegen, Duration, MPCost, SPCost FROM GoldenFalcon ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT32FalconDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Float, &sSkillTable.faArcherT32HPRegen[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT32Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT32MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, DamageBoostvsDemonMonsters, AreaofEffect, MPCost, SPCost FROM BombShot ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT33DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT33DamageBoostvsDemonMonsters[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT33AreaofEffect[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT33MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT IncreasedShotRange, DamageBoost, MPCost, SPCost FROM Perforation ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT34IncreasedShotRange[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT34DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT34MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT Duration, SummonAttackPowerMin, SummonAttackPowerMax, SummonAttackRating, SummonDefense, SummonHP, MPCost, SPCost FROM RecallWolverine ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT41Duration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT41SummonAttackPowerMin[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT41SummonAttackPowerMax[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT41SummonAttackRating[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT41SummonDefense[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaArcherT41SummonHP[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaArcherT41MPCost[i] );
					pcDB->GetData( 8, PARAMTYPE_Integer, &sSkillTable.iaArcherT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT IncreaseEvasion FROM EvasionMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT42IncreaseEvasion[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ShotRangeIncrease, DamageBoost, ChargeRaiseSpeed, MPCost, SPCost FROM PhoenixShot ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT43ShotRangeIncrease[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT43DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Float, &sSkillTable.faArcherT43ChargeRaiseSpeed[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT43MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPower, AddAttackRating, Duration, MPCost, SPCost FROM ForceofNature ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT44AddAttackPower[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT44AddAttackRating[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT44Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT44MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddRange, CriticalRateIncrease, AfterDamage, MPCost, SPCost FROM LethalSight ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT51AddRange[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT51CriticalRateIncrease[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT51AfterDamage[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT51MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaArcherT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, CriticalDamageBoost, MPCost, SPCost FROM MistralsArrow ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT52CriticalDamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT52MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT EffectDuration, MPCost, SPCost FROM BloomingLotus ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT53EffectDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT53MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, EffectDuration, MPDrain, SPDrain FROM SaggitariusRain ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaArcherT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaArcherT54EffectDuration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaArcherT54MPDrain[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaArcherT54SPDrain[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadPikeman()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AttackPowerMin, AttackPowerMax, PushbackRange, MPCost, SPCost FROM PikeWind ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT11AttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT11AttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT11PushbackRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT11MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPikemanT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FrostResistanceIncrease FROM IceAttribute ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT12FrostResistanceIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddCriticalDamage, MPCost, SPCost FROM CriticalHit ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT13AddCriticalDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT13MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT13SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT WeaponAttackRatingBoost, DamageBoost, MPCost, SPCost FROM JumpingCrash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT14WeaponAttackRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT14DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT14MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FreezeDuration, FreezeAttackPowerMin, FreezeAttackPowerMax, FreezeAttackRange, MPCost, SPCost FROM GroundPike ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT21FreezeDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT21FreezeAttackPowerMin[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT21FreezeAttackPowerMax[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT21FreezeAttackRange[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPikemanT21MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPikemanT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, Range, MPCost, SPCost FROM Tornado ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT22DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT22Range[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT22MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT WeaponCurrentBlockBoost FROM WeaponDefenseMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT23WeaponCurrentBlockBoost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddSize, DamageBoost, MPCost, SPCost FROM Expansion ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT24AddSize[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT24DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT24MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, NumberofSpears, EffectDuration, EffectRange, MPCost, SPCost FROM VenomSpear ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT31DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT31NumberofSpears[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT31EffectDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT31EffectRange[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPikemanT31MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPikemanT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AfterDamage, MonsterSightDecrease, Duration, MPCost, SPCost FROM Vanish ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT32AfterDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT32MonsterSightDecrease[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT32Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT32MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPikemanT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT WeaponCriticalRatingBoost FROM CriticalMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT33WeaponCriticalRatingBoost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM ChainLance ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT34DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT34MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddCriticalDamage, TargetCriticalDecrease, DebuffDuration, MPCost, SPCost FROM AssasinsEye ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT41AddCriticalDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT41TargetCriticalDecrease[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT41DebuffDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT41MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPikemanT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, ChargedDamageBoost, RaiseChargingSpeed, MPCost, SPCost FROM ChargingStrike ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT42DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT42ChargedDamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT42RaiseChargingSpeed[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT42MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPikemanT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddEvasion, Duration, MPCost, SPCost FROM Vague ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT43AddEvasion[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT43Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT43MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, NumberofHits, MPCost, SPCost FROM ShadowMaster ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT44DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT44NumberofHits[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT44MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT Duration, MPCost, SPCost FROM RingofSpears ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT51Duration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT51MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, Cooldown, MPCost, SPCost FROM LastBreath ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Float, &sSkillTable.faPikemanT52Cooldown[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT52MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, WeaponAttackRatingBoost, MPCost, SPCost FROM ExDeath ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT53DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT53WeaponAttackRatingBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT53MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPikemanT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM SinisterSpin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPikemanT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPikemanT54MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPikemanT54SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadAtalanta()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT AttackPowerMin, AttackPowerMax, MPCost, SPCost FROM ShieldStrike ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT11AttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT11AttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT11MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddAttackSpeed, AddAttackRating, MPCost, SPCost FROM Farina ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT12DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT12AddAttackSpeed[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT12AddAttackRating[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT12MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPower FROM ThrowingMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT13AddAttackPower[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM BiggerSpear ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT14DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT14MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackRatingBoost, AddRange, Duration, MPCost, SPCost FROM Windy ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT21AttackRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT21AddRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT21Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT21MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRatingBoost, MPCost, SPCost FROM TwistJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT22DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT22AttackRatingBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT22MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MonsterLifeAbsorption, MPCost, SPCost FROM SoulSucker ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT23MonsterLifeAbsorption[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT23MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM FireJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT24DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT24MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT NumberofHits, DamageBoost, AttackRatingBoost, MPCost, SPCost FROM SplitJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT31NumberofHits[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT31DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT31AttackRatingBoost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT31MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MaxAttackPower, Duration, MPCost, SPCost FROM TriumphofValhalla ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT32MaxAttackPower[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT32Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM LightningJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT33DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT33MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM StormJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT34DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT34MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddEvasion, Duration, MPCost, SPCost FROM HallofValhalla ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT41AddEvasion[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT41Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT41MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM ExtremeRage ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT42DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT42AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT42MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPowerMin, AddAttackPowerMax, TargetRunningSpeedReduction, TargetFreezeEffectDuration, Duration, MPCost, SPCost FROM FrostJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43AddAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43AddAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43TargetRunningSpeedReduction[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43TargetFreezeEffectDuration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43Duration[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43MPCost[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddRange, MPCost, SPCost FROM Vengeance ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT44DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT44AddRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT44MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SummonAttackPowerMin, SummonAttackPowerMax, SummonAttackRating, SummonHP, SummonDuration, MPCost, SPCost FROM NaturesCall ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51SummonAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51SummonAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51SummonAttackRating[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51SummonHP[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51SummonDuration[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51MPCost[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SpeedIncrease, AddEvasion FROM GoldenApple ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT52SpeedIncrease[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT52AddEvasion[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, Targets, PoisonDuration, MPCost, SPCost FROM PlagueJavelin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT53DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT53Targets[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT53PoisonDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT53MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM TripleThreat ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT54MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAtalantaT54SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadKnight()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT StaticDamage, DamageBoost, LinearAreaofEffect, MPCost, SPCost FROM SwordBlast ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT11StaticDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT11DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT11LinearAreaofEffect[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT11MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoostvsUndeadMonsters, Duration, MPCost, SPCost FROM HolyBody ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT12DamageBoostvsUndeadMonsters[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT12Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT12MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SPBoost FROM PhysicalTraining ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT13SPBoost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddCritical, MPCost, SPCost FROM DoubleCrash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT14DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT14AddCritical[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT14MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT PartyArea, FinalDamageBoostvsUndeadEnemies, Duration, MPCost, SPCost FROM HolyValor ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT21PartyArea[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT21FinalDamageBoostvsUndeadEnemies[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT21Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT21MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AreaofEffect, DamageBoost, MPCost, SPCost FROM Brandish ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT22AreaofEffect[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT22DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT22MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, PierceRange, MPCost, SPCost FROM Piercing ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT23DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT23AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT23PierceRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT23MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DefenseBoost, Duration, MPCost, SPCost FROM DrasticSpirit ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT24DefenseBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT24Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT24MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT BaseAttackPowerIncrease FROM SwordMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT31BaseAttackPowerIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddBlockRating, MonsterDamageIntoHP, Duration, MPCost, SPCost FROM DivineShield ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT32AddBlockRating[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT32MonsterDamageIntoHP[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT32Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT32MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SuccessRate, MonsterLifeIncrease, EnslaveEffectDuration, MPCost, SPCost FROM HolyIncantation ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT33SuccessRate[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT33MonsterLifeIncrease[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT33EnslaveEffectDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT33MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackRatingBoost, DamageBoost, DamageBoostvsUndeadEnemies, MPCost, SPCost FROM GrandCross ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT34AttackRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT34DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT34DamageBoostvsUndeadEnemies[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT34MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM SwordofJustice ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT41DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT41AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT41MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ShieldAbsorptionBoost, Duration, MPCost, SPCost FROM GodlyShield ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT42ShieldAbsorptionBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT42Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT42MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPower, Duration, MPCost, SPCost FROM GodsBlessing ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT43AddAttackPower[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT43Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT43MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddCritical, DamageBoost, NumberofHits, MPCost, SPCost FROM DivinePiercing ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT44AddCritical[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT44DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT44NumberofHits[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT44MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT UndeadDamageBoost, MPCost, SPCost FROM HolyConviction ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT51UndeadDamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT51MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, AddAtkRtgBoost, Duration, MPCost, SPCost, Cooldown FROM DivineInquisition ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT52AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT52AddAtkRtgBoost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaKnightT52Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaKnightT52MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaKnightT52SPCost[i] );
					pcDB->GetData( 7, PARAMTYPE_Float, &sSkillTable.faKnightT52Cooldown[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ShieldDefenseBoost, MPCost, SPCost FROM GloriousShield ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT53ShieldDefenseBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT53MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM DivineCross ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaKnightT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaKnightT54MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaKnightT54SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadMagician()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT HPSacrificed, MPIncreased, MPCost, SPCost FROM Agony ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT11HPSacrificed[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT11MPIncreased[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT11MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT StaticDamage, DamageBoost, AttackRange, MPCost, SPCost FROM FireBolt ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT12StaticDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT12DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT12AttackRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT12MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAbsorption, AddElementalResistance, Duration, MPCost, SPCost FROM Zenith ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT13AddAbsorption[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT13AddElementalResistance[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT13Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT13MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT13SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT StaticDamage, DamageBoost, AttackRange, AreaofEffect, MPCost, SPCost FROM FireBall ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT14StaticDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT14DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT14AttackRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT14AreaofEffect[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT14MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMagicianT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MPPotionRecoveryBoost, MPCapacityIncrease FROM MentalMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT21MPPotionRecoveryBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT21MPCapacityIncrease[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRange, AreaofEffect, MPCost, SPCost FROM Watornado ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT22DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT22AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT22AreaofEffect[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT22MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FireAttackPowerMin, FireAttackPowerMax, IceAttackPowerMin, IceAttackPowerMax, LightningAttackPowerMin, LightningAttackPowerMax, Duration, MPCost, SPCost FROM EnchantWeapon ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23FireAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23FireAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23IceAttackPowerMin[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23IceAttackPowerMax[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23LightningAttackPowerMin[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23LightningAttackPowerMax[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23Duration[i] );
					pcDB->GetData( 8, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23MPCost[i] );
					pcDB->GetData( 9, PARAMTYPE_Integer, &sSkillTable.iaMagicianT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRange, MPCost, SPCost FROM DeathRay ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT24DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT24AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT24MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ReduceDamage, Duration, MPCost, SPCost FROM EnergyShield ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT31ReduceDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT31Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT31MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM Diastrophism ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT32DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT32AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPowerMin, AddAttackPowerMax, Duration, MPCost, SPCost FROM SpiritElemental ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT33AddAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT33AddAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT33Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT33MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FireDamage, IceDamage, SwordAttackDelay, Duration, MPCost, SPCost FROM DancingSword ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT34FireDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT34IceDamage[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT34SwordAttackDelay[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT34Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT34MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMagicianT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SummonDuration, SummonAttackPowerMin, SummonAttackPowerMax, SummonAttackRating, SummonHP, MPCost, SPCost FROM FireElemental ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41SummonDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41SummonAttackPowerMin[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41SummonAttackPowerMax[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41SummonAttackRating[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41SummonHP[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41MPCost[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaMagicianT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM FlameWave ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT42DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT42AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT42MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT TargetSpeedReduction, AreaofEffect, EffectDuration, MPCost, SPCost FROM Distortion ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT43TargetSpeedReduction[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT43AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT43EffectDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT43MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaMagicianT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM Meteorite ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT44DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT44AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT44MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT OrbBlockBoost, AttackRateReduced, MPCost, SPCost FROM WizardsTrance ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT51OrbBlockBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT51AttackRateReduced[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT51MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT EffectDuration, RunSpeedReduction, MPCost, SPCost FROM StoneSkin ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT52EffectDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT52RunSpeedReduction[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT52MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, IncreasedAttackRange, MPCost, SPCost FROM ElementalRay ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT53DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT53IncreasedAttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT53MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackDuration, MPDrain, SPDrain FROM Cataclysm ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaMagicianT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaMagicianT54AttackDuration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaMagicianT54MPDrain[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaMagicianT54SPDrain[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadPriestess()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT FormulaBasedLifeRecovery, AttackPowerBasedLifeRecovery, MPCost, SPCost FROM Healing ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT11FormulaBasedLifeRecovery[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT11AttackPowerBasedLifeRecovery[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT11MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FormulaBasedDamage, DamageBoost, MPCost, SPCost FROM HolyBolt ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT12FormulaBasedDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT12DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT12MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FormulaBasedDamageperHit, AttackPowerBasedDamageperHit, MinNumberofSparks, MaxNumberofSparks, MPCost, SPCost FROM MultiSpark ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT13FormulaBasedDamageperHit[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT13AttackPowerBasedDamageperHit[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT13MinNumberofSparks[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT13MaxNumberofSparks[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT13MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPriestessT13SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT TargetReduceDamage, EffectDuration, MPCost, SPCost FROM HolyMind ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT14TargetReduceDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT14EffectDuration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT14MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddMPPotionBoostRecovery, AddMPRegeneration FROM Meditation ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT21AddMPPotionBoostRecovery[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT21AddMPRegeneration[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackRange, DamageBoost, NumberofBolts, MPCost, SPCost FROM DivineLightning ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT22AttackRange[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT22DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT22NumberofBolts[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT22MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MonsterDamageReflected, Duration, MPCost, SPCost FROM HolyReflection ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT23MonsterDamageReflected[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT23Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT23MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT FormulaBasedLifeRecovery, AttackPowerBasedLifeRecovery, Range, MPCost, SPCost FROM GrandHealing ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT24FormulaBasedLifeRecovery[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT24AttackPowerBasedLifeRecovery[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT24Range[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT24MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackRatingBoost, DamageBoost, MPCost, SPCost FROM VigorBall ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT31AttackRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT31DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT31MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttainableSuccessRate, MaxPlayersResurrected, MPCost, SPCost FROM Resurrection ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT32AttainableSuccessRate[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT32MaxPlayersResurrected[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttainableSuccessRate, MonsterHealthReduction, MPCost, SPCost FROM Extinction ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT33AttainableSuccessRate[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT33MonsterHealthReduction[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT33MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT IncreaseLife, Duration, MPCost, SPCost FROM VirtualLife ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT34IncreaseLife[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT34Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT34MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackPowerMin, AttackPowerMax, AttackRange, FreezeDuration, MPCost, SPCost FROM GlacialSpike ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT41AttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT41AttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT41AttackRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT41FreezeDuration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT41MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPriestessT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddHPRegen, AddMPRegen, Duration, Range, MPCost, SPCost FROM RegenerationField ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT42AddHPRegen[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT42AddMPRegen[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT42Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT42Range[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT42MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPriestessT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, NumberofBolts, Range, MPCost, SPCost FROM ChainLightning ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT43DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT43NumberofBolts[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT43Range[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT43MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MuspellDamage, AddAttackPowerMin, AddAttackPowerMax, VirtualLifeEffectReduction, MuspellAttackDelay, AddUndeadAbsorption, AddEvasion, MPCost, SPCost FROM SummonMuspell ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44MuspellDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44AddAttackPowerMin[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44AddAttackPowerMax[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44VirtualLifeEffectReduction[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44MuspellAttackDelay[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44AddUndeadAbsorption[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44AddEvasion[i] );
					pcDB->GetData( 8, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44MPCost[i] );
					pcDB->GetData( 9, PARAMTYPE_Integer, &sSkillTable.iaPriestessT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SummonAttackPowerMin, SummonAttackPowerMax, SummonAttackRating, SummonHealthPoints, SummonDuration, MPCost, SPCost FROM IceElemental ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51SummonAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51SummonAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51SummonAttackRating[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51SummonHealthPoints[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51SummonDuration[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51MPCost[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaPriestessT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, Meteorites, MPCost, SPCost FROM IceMeteorite ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT52AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT52Meteorites[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT52MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT TargetAttackRatingReduction, DebuffEffectDuration, ChargedArea, MPCost, SPCost FROM HeavenlyLight ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT53TargetAttackRatingReduction[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT53DebuffEffectDuration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT53ChargedArea[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT53MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaPriestessT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT IncreaseDefense, IncreaseAbsorption, MPCost, SPCost FROM Consecration ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaPriestessT54IncreaseDefense[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaPriestessT54IncreaseAbsorption[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaPriestessT54MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaPriestessT54SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadAssassin()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM Stinger ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT11DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT11MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM RunningHit ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT12DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT12MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT WeaponAttackPowerAdd FROM DualWieldMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT13WeaponAttackPowerAdd[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ReduceReceivedDamageFromTarget, AttackRange, EffectDuration, MPCost, SPCost FROM Wisp ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT14ReduceReceivedDamageFromTarget[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT14AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT14EffectDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT14MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAssassinT14SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM VenomThorn ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT21DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT21MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddEvasion, Duration, MPCost, SPCost FROM Alas ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT22AddEvasion[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT22Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT22MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT RadiusAreaofEffect, StunEffectDuration, MPCost, SPCost FROM SoulShock ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT23RadiusAreaofEffect[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT23StunEffectDuration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT23MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT WeaponAttackRatingBoost, WeaponBlockRatingBoost FROM AttackMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT24WeaponAttackRatingBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT24WeaponBlockRatingBoost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM SoreBlade ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT31DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT31MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AreaofEffect, DamageBoost, MPCost, SPCost FROM GustSlash ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT32AreaofEffect[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT32DamageBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackSpeedBoost, Duration, MPCost, SPCost FROM Inpes ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT33AttackSpeedBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT33Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT33MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AfterDamage, MonsterSightDecrease, Duration, MPCost, SPCost FROM Blind ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT34AfterDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT34MonsterSightDecrease[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT34Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT34MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAssassinT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM FrostWind ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT41DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT41MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT WeaponCriticalRatingBoost FROM FatalMastery ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT42WeaponCriticalRatingBoost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MaximumHits, EffectDuration, EffectRange, MPCost, SPCost FROM Pollute ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT43DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT43MaximumHits[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT43EffectDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT43EffectRange[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAssassinT43MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaAssassinT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM NinjaShadow ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT44DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT44MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT CriticalDamageBoost, AttainableMovementTeleportRange, MPCost, SPCost FROM Shunpo ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT51CriticalDamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT51AttainableMovementTeleportRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT51MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, PoisonDamage, PoisonDuration, MPCost, SPCost FROM KissoftheViper ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT52DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT52PoisonDamage[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT52PoisonDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaAssassinT52MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaAssassinT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM ShadeKick ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT53DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT53MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM ShadowStorm ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaAssassinT54DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaAssassinT54MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaAssassinT54SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}

void CSkillServerHandler::LoadShaman()
{
	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_SkillDBNew );
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRange, MPCost, SPCost FROM DarkBolt ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT11DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT11AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT11MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT11SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRange, AttackArea, MPCost, SPCost FROM DarkWave ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT12DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT12AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT12AttackArea[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT12MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT12SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT EnemySpeedReduction, EffectArea, EffectDuration, MPCost, SPCost FROM Inertia ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT13EnemySpeedReduction[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT13EffectArea[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT13EffectDuration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT13MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT13SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MPPotionRecoveryBoost, MPCapacityBoost FROM InnerPeace ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT14MPPotionRecoveryBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT14MPCapacityBoost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRange, AttackArea, MPCost, SPCost FROM SpiritualFlare ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT21DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT21AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT21AttackArea[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT21MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT21SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT StunDuration, MPCost, SPCost FROM SpiritualManacle ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT22StunDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT22MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT22SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT BaseAddAttackPowerMin, BaseAddAttackPowerMax, IncreaseMonsterSightRange, Duration, MPCost, SPCost FROM ChasingHunt ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT23BaseAddAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT23BaseAddAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT23IncreaseMonsterSightRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT23Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT23MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaShamanT23SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AddAttackPowerMin, AddAttackPowerMax, Duration, MPCost, SPCost FROM AdventMigal ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT24AddAttackPowerMin[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT24AddAttackPowerMax[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT24Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT24MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT24SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT ReduceReceivedDamage, AddEvasionRating, ReduceMPPoints, Duration, MPCost, SPCost FROM Rainmaker ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT31ReduceReceivedDamage[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT31AddEvasionRating[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT31ReduceMPPoints[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT31Duration[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT31MPCost[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaShamanT31SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM PhantomCall ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT32DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT32AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT32MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT32SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MonsterLifeAbsorption, MPCost, SPCost FROM Haunt ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT33MonsterLifeAbsorption[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT33MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT33SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, MPCost, SPCost FROM Scratch ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT34DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT34MPCost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT34SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT SummonDuration, SummonAttackPowerMin, SummonAttackPowerMax, SummonAttackRating, SummonHP, MPCost, SPCost FROM SummonHestian ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT41SummonDuration[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT41SummonAttackPowerMin[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT41SummonAttackPowerMax[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT41SummonAttackRating[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT41SummonHP[i] );
					pcDB->GetData( 6, PARAMTYPE_Integer, &sSkillTable.iaShamanT41MPCost[i] );
					pcDB->GetData( 7, PARAMTYPE_Integer, &sSkillTable.iaShamanT41SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AddCriticalChance, AttackRange, MPCost, SPCost FROM Judgement ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT42DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT42AddCriticalChance[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT42AttackRange[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT42MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT42SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT AttackSpeedBoost, Duration, MPCost, SPCost FROM AdventMidranda ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT43AttackSpeedBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT43Duration[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT43MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT43SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AreaofEffect, MPCost, SPCost FROM MourningPray ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT44DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT44AreaofEffect[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT44MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT44SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT MPBoost, SPBoost, Duration, MPCost, SPCost FROM Creed ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT51MPBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT51SPBoost[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT51Duration[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT51MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT51SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoostMainTarget, DamageBoostSurroundings, SurroundingImpactArea, MPCost, SPCost FROM PressDeity ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT52DamageBoostMainTarget[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT52DamageBoostSurroundings[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT52SurroundingImpactArea[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT52MPCost[i] );
					pcDB->GetData( 5, PARAMTYPE_Integer, &sSkillTable.iaShamanT52SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT DamageBoost, AttackRange, MPCost, SPCost FROM PhantomNail ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT53DamageBoost[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT53AttackRange[i] );
					pcDB->GetData( 3, PARAMTYPE_Integer, &sSkillTable.iaShamanT53MPCost[i] );
					pcDB->GetData( 4, PARAMTYPE_Integer, &sSkillTable.iaShamanT53SPCost[i] );
					i++;
				}
			}
		}

		pcDB->Clear();
		if ( pcDB->Prepare( "SELECT HPIncrease, MPDecrease FROM OccultLife ORDER BY Level ASC" ) )
		{
			if ( pcDB->Execute() )
			{
				int i = 0;
				while ( pcDB->Fetch() )
				{
					pcDB->GetData( 1, PARAMTYPE_Integer, &sSkillTable.iaShamanT54HPIncrease[i] );
					pcDB->GetData( 2, PARAMTYPE_Integer, &sSkillTable.iaShamanT54MPDecrease[i] );
					i++;
				}
			}
		}

		pcDB->Close();
	}
}
