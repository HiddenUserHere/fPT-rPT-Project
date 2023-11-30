#include "stdafx.h"
#include "CAgingWindow.h"


CAgingWindow::CAgingWindow()
{
}


CAgingWindow::~CAgingWindow()
{
}

void CAgingWindow::Render()
{
	pWindow->SetPosition( 18 - ( 256 + 128 - READDWORD( 0x367E84C ) ), 88 );

	if ( (*(BOOL*)(0x036834F0)) )
	{
		auto pTextFailChance = pWindow->GetElement<UI::Text>( TextID_Fail );

		// Set Chance
		if ( (*(BOOL*)(0x03686CD4)) )
		{
			DWORD dwCode = 0;
			if ( (*(BOOL*)(0x036897EC)) )
				dwCode = (*(DWORD*)(0x003689838));

			iAging = (int)(*(short*)(0x03686EE4));

			int iChance = iaAgingChance[iAging];

			if ( dwCode == ITEMID_AgingStone )
				iChance -= 10;
			else if ( dwCode == ITEMID_RipeStone )
				iChance -= 20;
			else if ( dwCode == ITEMID_FluenceStone )
				iChance -= 30;
			else if ( dwCode == ITEMID_ArcaneStone )
				iChance -= 40;

			if ( iReduceTax > 0 )
				iChance -= iReduceTax;

			if ( iChance < 0 )
				iChance = 0;

			char szChanceFail[32] = { 0 };
			STRINGFORMAT( szChanceFail, "Fail Chance: %d%%", iChance );
			pTextFailChance->SetText( szChanceFail );
		}
		else
		{
			pTextFailChance->SetText( "Fail Chance: 0%" );
			iAging = -1;
		}
	}

	pWindow->Render( 0, 0, GRAPHICENGINE->GetWidthBackBuffer(), GRAPHICENGINE->GetHeightBackBuffer(), 0, 0 );
}

void CAgingWindow::Init()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 18, 88, 320, 208 ) );
	UI::Text_ptr pTextChanceFail = std::make_shared<UI::Text>( Rectangle2D( 20, 176, 156, 24 ) );
	pTextChanceFail->SetID( TextID_Fail );
	pTextChanceFail->SetFont( "Arial", 14, 0, FALSE, FALSE, -1 );
	pTextChanceFail->SetText( "" );
	pWindow->AddElement( pTextChanceFail );
}

void CAgingWindow::HandlePacket( PacketAgingFreeEvent * psPacket )
{
	iReduceTax = psPacket->iReduceTax;

	AGING_FREE = psPacket->bFree;
}

