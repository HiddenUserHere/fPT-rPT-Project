#include "stdafx.h"
#include "CPremiumHandler.h"
#include "PremiumModel.h"
#include "PremiumView.h"


CPremiumControllerHandler::CPremiumControllerHandler()
{
	psPacketPremium = new PacketItemPremiumData;

	pcPremiumModel = new CPremiumModel;

	pcPremiumView = new CPremiumView;
}


CPremiumControllerHandler::~CPremiumControllerHandler()
{
	DELET( psPacketPremium );
	DELET( pcPremiumModel );
	DELET( pcPremiumView );
}

void CPremiumControllerHandler::SetPremiumServer( struct PacketActivePremium * psPacket )
{
	pcPremiumModel->SetPremiumServer( psPacket );
}

void CPremiumControllerHandler::Init()
{
	pcPremiumModel->Init();
}

void CPremiumControllerHandler::Update(float fTime)
{
	if ( pcPremiumModel )
		pcPremiumModel->Update( fTime );
}

void CPremiumControllerHandler::UpdateTimers()
{
	for ( int i = 0; i < PREMIUM_ITEM_TOTAL; i++ )
	{
		if ( GetData()->saItemList[i].dwTimeLeft )
			GetData()->saItemList[i].dwTimeLeft *= 1000;
	}
}

BOOL CPremiumControllerHandler::OnMouseClick( CMouse * pcMouse )
{
	BOOL bRet = FALSE;
	return bRet;
}

void CPremiumControllerHandler::OnDraw()
{
	pcPremiumModel->OnDraw();
}