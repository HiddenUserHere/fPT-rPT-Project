#include "stdafx.h"
#include "BotShopServerHandler.h"


CBotShopServerHandler::CBotShopServerHandler()
{
}


CBotShopServerHandler::~CBotShopServerHandler()
{
}

void CBotShopServerHandler::UpdateUnitStatus( UserData * pcUserData )
{
}

BOOL CBotShopServerHandler::AddBot( User * pcUser, PacketPersonalBotShopData * psPacket )
{
	if ( pcUser == NULL || pcUser->pcUserData == NULL )
		return FALSE;

	if ( iBotCount >= 300 )
		return FALSE;

	UserData * ud = pcUser->pcUserData;

	SShopDataHandler * ps = psaShopData + (iBotCount++);
	ZeroMemory( ps, sizeof( SShopDataHandler ) );

	CopyMemory( &ps->sCharacterData, &ud->sCharacterData, sizeof( CharacterData ) );

	ps->iID = CALL( 0x0054CEE0 );

	ps->iPositionItemHandB = psPacket->iaItems[0];
	ps->iPositionItemHandL = psPacket->iaItems[1];
	ps->iPositionItemHandR = psPacket->iaItems[2];

	return TRUE;
}

void CBotShopServerHandler::Init()
{
	psaShopData = new SShopDataHandler[MAX_BOTS];
	ZeroMemory( psaShopData, sizeof( SShopDataHandler ) * MAX_BOTS );


}
