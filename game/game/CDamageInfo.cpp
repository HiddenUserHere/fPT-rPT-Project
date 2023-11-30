#include "stdafx.h"
#include "CDamageInfo.h"
#include "MDamageInfo.h"

bool						bNoShowDebugDamage = false;

void CDamageInfoController::Clear( DamageDataInfo * psData )
{
	if ( psData != NULL )
		ZeroMemory( psData, sizeof( DamageDataInfo ) );
}

CDamageInfoController::CDamageInfoController()
{

}


void CDamageInfoController::load( )
{

}

void CDamageInfoController::Init()
{
	psaTargets = new DamageDataInfo[MAX_DEBUG_TARGETS];
	ZeroMemory( psaTargets, sizeof( DamageDataInfo ) * MAX_DEBUG_TARGETS );

	//Font
	pOrangeFont->ReadFontData( "game\\fonts\\font.dat" );
	pOrangeFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font1.png" ) );

	pRedFont->ReadFontData( "game\\fonts\\font.dat" );
	pRedFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font2.png" ) );

	pBlueFont->ReadFontData( "game\\fonts\\font3.dat" );
	pBlueFont->SetFontImage( UI::ImageLoader::LoadImage( "game\\fonts\\font3.png" ) );

}

void CDamageInfoController::Load()
{
	iDefenseInfoPart = PARTICLE->Load( "game\\scripts\\particles\\DefenseInfo.part" );
	iBlockInfoPart = PARTICLE->Load( "game\\scripts\\particles\\BlockInfo.part" );
	iEvadeInfoPart = PARTICLE->Load( "game\\scripts\\particles\\EvadeInfo.part" );
}


void CDamageInfoController::SetDefenseInfo( int TypeAction, int Value )
{
	static DWORD dwTimeDamageSend = 0;

	if ( TypeAction < 4 )
		CALLT_WITH_ARG1( 0x004A7400, 0, TypeAction );

	PacketDebugDamageInfo sPacket;
	sPacket.iLength		= sizeof( PacketDebugDamageInfo );
	sPacket.iHeader		= PKTHDR_DamageDebugInfo;
	sPacket.dwObjectID	= 0;
	sPacket.iTypeAction = TypeAction;
	sPacket.iValue		= Value;

	if ( (TICKCOUNT - dwTimeDamageSend) > 128 )
	{
		SENDPACKETG( &sPacket );
		dwTimeDamageSend = TICKCOUNT;
	}
	if ( TypeAction == 5 )
	{
		sPacket.dwObjectID = -1;
		AddTarget( &sPacket );
	}
}

void CDamageInfoController::AddTarget( PacketDebugDamageInfo * psPacket )
{
	if ( bNoShowDebugDamage || CHARACTERGAME->IsStageVillage() || GetForegroundWindow() != DX::Graphic::GetWindowHandler() )
		return;

	DamageDataInfo * ps = psaTargets + (uNext % MAX_DEBUG_TARGETS);
	
	Clear( ps );

	if ( psPacket->dwObjectID == (-1) )
		ps->pcUnitData = UNITDATA;
	else
		ps->pcUnitData = UNITDATABYID( psPacket->dwObjectID );

	if ( ps->pcUnitData )
	{
		ps->iID		= ps->pcUnitData->iID;
		ps->iType	= psPacket->iTypeAction;
		ps->iValue	= psPacket->iValue;
		ps->dwTime	= TICKCOUNT + MAX_DEBUG_TIME;
		ps->iX		= ps->pcUnitData->rRenderRectangle.left + ((ps->pcUnitData->rRenderRectangle.right - ps->pcUnitData->rRenderRectangle.left) >> 1) - 20;
		ps->iY		= ps->pcUnitData->rRenderRectangle.top - 64;
		ps->iAlpha	= 0;
		ps->bActive = TRUE;
	}

	uNext++;
}


void CDamageInfoController::Render()
{
	if ( bNoShowDebugDamage == false || CHARACTERGAME->IsStageVillage() == FALSE )
	{
		for ( int i = 0; i < MAX_DEBUG_TARGETS; i++ )
		{
			DamageDataInfo * ps = psaTargets + i;

			if ( ps->bActive )
			{
				UnitData * pcUnitData = ps->pcUnitData;

				if ( pcUnitData )
				{
					if ( ps->iID != pcUnitData->iID )
					{
						Clear( ps );
						continue;
					}

					if ( ps->dwTime < TICKCOUNT )
					{
						ps->bActive = FALSE;
						continue;
					}

					float fScale = (float)(ps->dwTime - TICKCOUNT) / 1000.0f;
					if ( fScale < 0.5f )
						fScale = 0.5f;

					int iTimeDifference = (int)(ps->dwTime - TICKCOUNT);

					ps->iAlpha = 0;

					if ( iTimeDifference > 0 )
					{
						if ( (iTimeDifference >= MAX_DEBUG_TIMEHALF) )
						{
							iTimeDifference -= MAX_DEBUG_TIMEHALF;
							iTimeDifference = MAX_DEBUG_TIMEHALF - iTimeDifference;
							ps->iAlpha = (iTimeDifference * 255) / MAX_DEBUG_TIMEHALF;
						}
						else
						{
							ps->iAlpha = (iTimeDifference * 255) / MAX_DEBUG_TIMEHALF;
						}
					}

					char szText[32] = { 0 };
					STRINGFORMAT( szText, ps->iID != UNITDATA->iID ? "%d" : "-%d", ps->iValue );

					ps->iX = ps->pcUnitData->rRenderRectangle.left + ((ps->pcUnitData->rRenderRectangle.right - ps->pcUnitData->rRenderRectangle.left) >> 1) - 20;
					ps->iY = (ps->pcUnitData->rRenderRectangle.top - 64);

					if ( ps->iType == 1 )
					{
						if ( ps->iParticleID == 0 )
							ps->iParticleID = PARTICLE->Start( iDefenseInfoPart, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

						PARTICLE->SetAttachPosition( ps->iParticleID, D3DVECTOR{ (float)pcUnitData->sPosition.iX, (float)pcUnitData->sPosition.iY, (float)pcUnitData->sPosition.iZ } );

					}
					else if ( ps->iType == 2 )
					{
						if ( ps->iParticleID == 0 )
							ps->iParticleID = PARTICLE->Start( iBlockInfoPart, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

						PARTICLE->SetAttachPosition( ps->iParticleID, D3DVECTOR{ (float)pcUnitData->sPosition.iX, (float)pcUnitData->sPosition.iY, (float)pcUnitData->sPosition.iZ } );

					}
					else if ( ps->iType == 3 )
					{
						if ( ps->iParticleID == 0 )
							ps->iParticleID = PARTICLE->Start( iEvadeInfoPart, pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ );

						PARTICLE->SetAttachPosition( ps->iParticleID, D3DVECTOR{ (float)pcUnitData->sPosition.iX, (float)pcUnitData->sPosition.iY, (float)pcUnitData->sPosition.iZ } );

					}
					else if ( ps->iType == 4 )
					{

					}
					else if ( ps->iType == 5 )
					{
						if ( ps->iID == UNITDATA->iID )
						{
							pRedFont->SetPosition( ps->iX, ps->iY + ps->iDecreaseY );
							pRedFont->SetText( szText );
							pRedFont->SetColor( D3DCOLOR_RGBA( 255, 255, 255, ps->iAlpha ) );
							pRedFont->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
						}
						else
						{
							pOrangeFont->SetPosition( ps->iX, ps->iY + ps->iDecreaseY );
							pOrangeFont->SetText( szText );
							pOrangeFont->SetColor( D3DCOLOR_RGBA( 255, 255, 255, ps->iAlpha ) );
							pOrangeFont->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
						}
					}
					else if ( ps->iType == 6 )
					{
						pBlueFont->SetPosition( ps->iX, ps->iY + ps->iDecreaseY );
						pBlueFont->SetText( szText );
						pBlueFont->SetColor( D3DCOLOR_RGBA( 255, 255, 255, ps->iAlpha ) );
						pBlueFont->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
					}

					ps->iDecreaseY -= GetDecrease();
				}
			}
		}
	}

	if ( (TICKCOUNT - dwTimeUpdate) >= 16 )
	{
		iDecreaseY = (TICKCOUNT - dwTimeUpdate) / 16;
		dwTimeUpdate = TICKCOUNT;
	}
	else
		iDecreaseY = 0;

}

void CDamageInfoController::Update( float fTime )
{
	int i = (1000 / 70);

	int iInc = (int)fTime % 15;
	if ( iInc == 0 )
		iInc = 1;

}

CDamageInfoController::~CDamageInfoController()
{
	DELETA( psaTargets );
}
