#include "stdafx.h"
#include "CPartyCoreHandler.h"

CPartyCoreHandler::CPartyCoreHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 100, 100 ) );

}

CPartyCoreHandler::~CPartyCoreHandler()
{
}

void CPartyCoreHandler::Init()
{
	pWindow->Hide();
}

BOOL CPartyCoreHandler::IsOpen()
{
	return pWindow->IsOpen();
}

void CPartyCoreHandler::Open( ItemData * pcItemData )
{
	if ( IsOpen() )
		return;

	Clear();

	CHARACTERGAME->CancelAttack();

	auto & sData = PARTYHANDLER->GetPartyData();
	if ( sData.iMembersCount > 0 )
	{
		int iX = pcItemData->sPosition.iX + (pcItemData->iWidth + 10);
		int iY = pcItemData->sPosition.iY;
		iY += READDWORD( 0x036286EC ) == 2 ? (RESOLUTION_HEIGHT - 600) : 0;
		if ( iX <= 0 )
			iX = 4;

		pWindow->SetPosition( iX, iY );

		//Dropdown Menu
		UI::DropdownMenu_ptr pDropdownMenu = std::make_shared<UI::DropdownMenu>( Rectangle2D( 0, 0, 94, 24 ) );
		pDropdownMenu->SetID( DROPDOWNMENU_Main );
		pDropdownMenu->SetDropdownPosition( UI::DROPDOWNPOSITION_Top );
		pDropdownMenu->SetMouseEvent( ClickDownR );
		pDropdownMenu->SetEvent( UI::Event::Build( std::bind( &CPartyCoreHandler::OnChooseMemberParty, this ) ) );
		pDropdownMenu->SetMenuBackground( "game\\images\\UI\\menu\\topleft.png", "game\\images\\UI\\menu\\middle.png", "game\\images\\UI\\menu\\bottom.png", "game\\images\\UI\\menu\\main.png" );
		bool bFound = false;
		for ( int i = 0; i < sData.iMembersCount; i++ )
		{
			if ( sData.saMembers[i].sData.iID != UNITDATA->iID )
				pDropdownMenu->AddOption( pDropdownMenu->MakeOption( sData.saMembers[i].szCharacterName, Rectangle2D( 13, 5, 77, 18 ), FALSE, -1, D3DCOLOR_XRGB( 230, 180, 150 ) ), FALSE, sData.saMembers[i].sData.iID );
		}

		CopyMemory( &sItem, &pcItemData->sItem, sizeof( Item ) );

		pDropdownMenu->Open();

		pWindow->AddElement( pDropdownMenu );

		pWindow->Show();
	}
	else
		TITLEBOX( AY_OBFUSCATE( "You must be in party to use this core!" ) );
}

void CPartyCoreHandler::Close()
{
	Clear();

	pWindow->Hide();
}

void CPartyCoreHandler::Clear()
{
	if ( auto pDropDown = pWindow->GetElement<UI::DropdownMenu>( DROPDOWNMENU_Main ) )
		pWindow->DelElement( pDropDown );

	ZeroMemory( &sItem, sizeof( Item ) );
}

void CPartyCoreHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CPartyCoreHandler::Update( float fTime )
{
	if ( pWindow->IsOpen() )
	{
		if ( auto pDropDown = pWindow->GetElement<UI::DropdownMenu>( DROPDOWNMENU_Main ) )
		{
			//if( pDropDown->IsOpen() )
			//	Close();
		}
	}
}

void CPartyCoreHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CPartyCoreHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( pWindow->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
	{
		//Close();
		return TRUE;
	}

	if ( pcMouse->GetEvent() == EMouseEvent::ClickDownL )
		Close();

	return FALSE;
}

void CPartyCoreHandler::OnResolutionChanged()
{
	Close();
}

void CPartyCoreHandler::OnChooseMemberParty()
{
	if ( UI::DropdownMenu_ptr pDropdownMenu = pWindow->GetElement<UI::DropdownMenu>( DROPDOWNMENU_Main ) )
	{
		int iOptionID = pDropdownMenu->GetSelectedOptionID();
		
		auto & sData = PARTYHANDLER->GetPartyData();
		if ( sData.iMembersCount > 0 )
		{
			bool bFound = false;
			for ( int i = 0; i < sData.iMembersCount; i++ )
			{
				if ( sData.saMembers[i].sData.iID == iOptionID )
				{
					bFound = true;
					break;
				}
			}

			if ( bFound )
			{
				if ( ItemData * pcItemData = ITEMHANDLER->GetItemInventory( sItem.sItemID.ToItemID(), sItem.iChk1, sItem.iChk2 ) )
				{
					PacketPartyCore sPacket;
					sPacket.iID = iOptionID;
					CopyMemory( &sPacket.sItem, &sItem, sizeof( Item ) );

					ProcessPacket( &sPacket );
				}
			}
		}
	}

	Close();
}

void CPartyCoreHandler::ProcessPacket( PacketPartyCore * psPacket )
{
	psPacket->iLength = sizeof( PacketPartyCore );
	psPacket->iHeader = PTKHDR_PartyCore;
	SENDPACKETG( psPacket );
}

