#include "stdafx.h"
#include "CHonorHandler.h"

void CHonorHandler::AddTimer( ESkillID eSkillID, PacketHonor::EHonorType eType, UINT uTime )
{
	if ( iTimerPvPHonorID < 0 )
		iTimerPvPHonorID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\PvPHonor.tga" );

	if ( iTimerBellatraHonorID < 0 )
		iTimerBellatraHonorID = TIMERSKILLHANDLER->LoadSkillTimerTexture( "game\\images\\skills\\timers\\BellatraHonor.tga" );

	if ( Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( eSkillID ) )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( eSkillID ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID = eSkillID;
		sSkill.iUseTime = uTime;
		sSkill.iLevel = 1;
		sSkill.MatIcon = eSkillID == SKILLID_PvPHonor ? iTimerPvPHonorID : iTimerBellatraHonorID;
		sSkill.PlusState[0] = eType;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
	}
}

CHonorHandler::CHonorHandler()
{
}

CHonorHandler::~CHonorHandler()
{
}

void CHonorHandler::HandlePacket( PacketHonor * psPacket )
{
	if ( psPacket->ePvPHonor != PacketHonor::EHonorType::HONORTYPE_None )
		AddTimer( SKILLID_PvPHonor, psPacket->ePvPHonor, psPacket->uPvPTime );

	if ( psPacket->eBellatraHonor != PacketHonor::EHonorType::HONORTYPE_None )
		AddTimer( SKILLID_BellatraHonor, psPacket->eBellatraHonor, psPacket->uBellatraTime );
}

void CHonorHandler::Test()
{
	PacketHonor sPacket;
	sPacket.ePvPHonor = PacketHonor::EHonorType::HONORTYPE_PvPGold;
	sPacket.uPvPTime = 10;
	sPacket.eBellatraHonor = PacketHonor::EHonorType::HONORTYPE_BellatraSilver;
	sPacket.uBellatraTime = 10;
	HandlePacket( &sPacket );
}
