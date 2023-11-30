#include "stdafx.h"
#include "CRankingHandler.h"

CRankingHandler::CRankingHandler()
{
    pcWindowRankingLevel = new CRankingLevelWindowHandler();
}

CRankingHandler::~CRankingHandler()
{
    DELET( pcWindowRankingLevel );
}

void CRankingHandler::Init()
{
    pcWindowRankingLevel->Init();
}

void CRankingHandler::Update( float fTime )
{
    pcWindowRankingLevel->Update( fTime );
}

void CRankingHandler::Render2D()
{
    pcWindowRankingLevel->Render();
}

void CRankingHandler::OnMouseMove( CMouse * pcMouse )
{
    pcWindowRankingLevel->OnMouseMove( pcMouse );
}

BOOL CRankingHandler::OnMouseClick( CMouse * pcMouse )
{
    return pcWindowRankingLevel->OnMouseClick( pcMouse );
}

BOOL CRankingHandler::OnMouseScroll( CMouse * pcMouse )
{
    return pcWindowRankingLevel->OnMouseScroll( pcMouse );
}

BOOL CRankingHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
    return pcWindowRankingLevel->OnKeyPress( pcKeyboard );
}

BOOL CRankingHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
    return pcWindowRankingLevel->OnKeyChar( pcKeyboard );
}

void CRankingHandler::OnResolutionChanged()
{
    pcWindowRankingLevel->OnResolutionChanged();
}

void CRankingHandler::HandlePacket( PacketRankingLevel * psPacket )
{
    if ( psPacket->IsFirst() )
    {
        pcWindowRankingLevel->Close();
        pcWindowRankingLevel->ClearData();
    }

    for ( int i = 0; i < psPacket->iCount; i++ )
        pcWindowRankingLevel->AddData( psPacket->saRankingData[i] );

    if ( psPacket->IsOnly() )
    {
        pcWindowRankingLevel->Open();
    }
}
