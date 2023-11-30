#include "stdafx.h"
#include "CrystalTowerHandler.h"

CCrystalTowerHandler::CCrystalTowerHandler()
{

}

CCrystalTowerHandler::~CCrystalTowerHandler()
{
}

void CCrystalTowerHandler::Init()
{
	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Arial", 32, 0, TRUE, FALSE );
}

void CCrystalTowerHandler::Update()
{
	if ( dwTimeLeft > 0 )
		dwTimeLeft--;

	if ( (iRound > 0) && (dwTimeLeft == 0) && bStarted )
		bStarted = FALSE;
}

void CCrystalTowerHandler::Render()
{
	if ( bStarted && MAP_ID == MAPID_T5QuestArena )
	{
		int iX = (RESOLUTION_WIDTH >> 1) - 210;
		int iY = 6;

		if ( iRound > 0 )
		{
			pFont->Draw( FormatString( "Round %d", iRound ), iX, iY, 0, DX::Color( 0, 255, 0 ) );
			pFont->Draw( FormatString( "Time Left: %02d:%02d", (dwTimeLeft / 60) % 60, (dwTimeLeft % 60) ), iX + 170, iY, 0, DX::Color( 255, 255, 0 ) );
		}
		else
		{
			pFont->Draw( FormatString( "Round %d", iRound ), iX, iY, 0, DX::Color( 75, 75, 220 ) );
			pFont->Draw( FormatString( "Time Left: %02d:%02d", (dwTimeLeft / 60) % 60, (dwTimeLeft % 60) ), iX + 170, iY, 0, DX::Color( 255, 255, 0 ) );
		}
	}
}

void CCrystalTowerHandler::HandlePacket( PacketQuestArenaTier5 * psPacket )
{
	//Init
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_Round )
	{
		if ( psPacket->iRound == 0 )
		{
			dwTimeLeft	= psPacket->dwTimeLeft;
			iRound		= 0;
			bStarted	= TRUE;
		}
		else
		{
			if ( psPacket->iRound == 1 )
				dwTimeLeft	= psPacket->dwTimeLeft;

			iRound			= psPacket->iRound;
		}
	}

	//Lost
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_Lost )
	{
		dwTimeLeft	= 0;
		iRound		= 0;
		bStarted	= FALSE;
	}
	//Finished
	if ( psPacket->iType == PacketQuestArenaTier5::EType::TYPEID_FinishQuest )
	{
		dwTimeLeft	= 0;
		iRound		= 0;
		bStarted	= FALSE;
	}
}
