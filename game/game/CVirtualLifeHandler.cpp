#include "stdafx.h"
#include "CVirtualLifeHandler.h"


CVirtualLifeHandler::CVirtualLifeHandler()
{
}


CVirtualLifeHandler::~CVirtualLifeHandler()
{
}

bool CVirtualLifeHandler::OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData )
{
	if ( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player )
	{
		if ( USESKILL( psSkill ) )
		{
			//Update Skill Data
			UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T34;
			UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

			//Animate...
			UNITDATA->Animate( ANIMATIONTYPE_Skill );

			PROCESSSKILLBUFFT( UNITDATA, pcUnitData );
			CVirtualLifeHandler::AddVirtualLifeEffect( UNITDATA->sPosition );
			return true;
		}
	}
	return false;
}

bool CVirtualLifeHandler::OnCastAnimationStart( Skill * psSkill )
{
	if ( UNITDATA_SELECTED == NULL )
	{
		if ( UNITDATA->GetAnimation() < ANIMATIONTYPE_Attack )
		{
			if ( USESKILL( psSkill ) )
			{
				OnCastEnd( UNITDATA );

				//Update Skill Data
				UNITDATA->sActiveSkill.cSkillTinyID = SKILLPLAYID_T34;
				UNITDATA->sActiveSkill.cLevel = psSkill->iLevel;

				//Animate...
				UNITDATA->Animate( ANIMATIONTYPE_Skill );

				PROCESSSKILLBUFF( UNITDATA );

				//CVirtualLifeHandler::AddVirtualLife( iaPrsT34Duration[psSkill->iLevel - 1], psSkill->iLevel );

				SKILLMANAGERHANDLER->ProcessSkillInParty( SKILLPLAYID_T34, psSkill->iLevel, 300 );

				CastMembers();

				return true;
			}
		}
	}
	return false;
}

void CVirtualLifeHandler::OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime )
{
	if ( pcUnit == UNIT )
	{
		//AddTimer( UNITDATA, uSkillLevel, (int)fOverTime );

		if ( CVirtualLifeHandler::AddVirtualLife( (int)fOverTime, uSkillLevel ) )
		{
			CVirtualLifeHandler::AddVirtualLifeEffect( UNIT->GetPosition() );
			AddVirtualLifeHeart( UNITDATA, 300 );
		}
	}
}

void CVirtualLifeHandler::OnCastEnd( UnitData * pcUnitData )
{
	if ( UNITDATA == pcUnitData )
		SKILLMANAGERHANDLER->ResetSkillTimer( SKILLID_VirtualLife );
}

void CVirtualLifeHandler::CastMembers()
{
	if ( PARTYHANDLER->GetPartyData().iMembersCount > 0 )
	{
		auto v = PARTYHANDLER->GetPartyMembersArea( 300 );
		for ( auto & p : v )
		{
			if ( p != UNITDATA )
			{
				CVirtualLifeHandler::AddVirtualLifeEffect( p->sPosition );
				AddVirtualLifeHeart( p, 300 );
			}
		}
	}
}

void CVirtualLifeHandler::CastMembers( PacketSkillPartyData * psPacket )
{
	for ( int i = 0; i < psPacket->iCount; i++ )
	{
		Unit * pcUnit = UNITDATATOUNIT( UNITDATABYID( psPacket->iaID[i] ) );
		if ( pcUnit )
		{
			if ( pcUnit != UNIT )
			{
				CVirtualLifeHandler::AddVirtualLifeEffect( pcUnit->pcUnitData->sPosition );
				AddVirtualLifeHeart( pcUnit->pcUnitData, 300 );
			}
		}
	}
}

BOOL CVirtualLifeHandler::AddVirtualLife( int iTime, int iLevel )
{
	return (BOOL)CALLT_WITH_ARG2( 0x005044D0, 0x0367E148, iTime, iLevel );
}

void CVirtualLifeHandler::AddVirtualLifeEffect( Point3D sPosition )
{
	SKILLMANAGERHANDLER->SkillPlaySound( 0x1700, sPosition.iX, sPosition.iY, sPosition.iZ );
	CALL_WITH_ARG3( 0x00531E00, (DWORD)&sPosition, 1, 0 );
}

void CVirtualLifeHandler::AddVirtualLifeHeart( UnitData * pcUnitData, int iTime )
{
	CALL_WITH_ARG2( 0x005BE250, (DWORD)pcUnitData, iTime );
}

void CVirtualLifeHandler::AddTimer( UnitData * pcUnitData, UINT uSkillLevel, double fOverTime )
{
	CALLT_WITH_ARG2( 0x005047E0, 0x0367E148, (int)fOverTime, uSkillLevel );
}
