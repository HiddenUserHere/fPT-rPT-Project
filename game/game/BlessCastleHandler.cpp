#include "stdafx.h"
#include "BlessCastleHandler.h"

typedef BOOL( __cdecl *tfnWarpField ) (int iFieldID, int * piX, int * piZ);

typedef BOOL( __cdecl *tfnOldWarpCastle )(BOOL bLeader, int * piX, int * piZ);
tfnOldWarpCastle fnOldWarpCastle = (tfnOldWarpCastle)0x004443E0;

ClanData * psaClanData = (ClanData*)0x0362BAD8;
int * iaTimeLeft = (int*)0x362BE5C;
short * saCrystalTowerCount = (short*)0x362BD38;

int GetClanCode( int iClanID )
{
	DWORD iCode, iCode1, iCode2;

	iCode = iClanID % 1000000;
	iCode1 = iClanID / 100000000;
	iCode2 = iCode1 * 100000000;

	return iCode + iCode2;
}

CBlessCastleHandler::CBlessCastleHandler()
{
	iUserScoreTemp			= 0;
	iPvPKills				= 0;
	iPvPDeaths				= 0;

	psCurrentPvPKillInfo	= NULL;
	dwPvPKillInfoTimeOut	= 0;

	iMode					= 0;

	ZeroMemory( szCharacterName, sizeof( szCharacterName ) );
	ZeroMemory( iDamage, sizeof(int) * 3 );
	ZeroMemory( iClass, sizeof(int) * 3 );
}

CBlessCastleHandler::~CBlessCastleHandler()
{
}

NAKED int CBlessCastleHandler::GetClanID( int iCodeClass )
{
    JMP( pfnGetClanIDByClass );
}

bool CBlessCastleHandler::InBattleZone()
{
	if ( MAP_ID == MAPID_BlessCastle )
		if ( UNITDATA->sPosition.iZ <= -6141650 )
			return true;

	return false;
}

void CBlessCastleHandler::Init()
{
	for ( int i = 0; i < _countof( pcaImageCharacterPvPKillInfo ); i++ )
		pcaImageCharacterPvPKillInfo[i] = UI::ImageLoader::LoadImageA( "game\\images\\kills\\class%d.png", i + 1 );

	for ( int i = 0; i < (PVPKILLTYPE_COUNT-1); i++ )
		pcaImageKillPvPKillInfo[i]		= UI::ImageLoader::LoadImageA( "game\\images\\kills\\kt%d.png", i + 1 );

	pcCrystalTowerImage = UI::ImageLoader::LoadImageA( "image\\Sinimage\\Siege\\Tower_image.bmp" );

	pFont = GetGraphicsDevice()->GetFontFactory()->MakeFont( nullptr, "Frutiger LT 55 Roman", 22, 0, TRUE, FALSE );

	BuildWindowRanking();
	BuildWindowSiegeWarInfo();
}

BOOL CBlessCastleHandler::WarpField( int iFieldID, int * piX, int * piZ )
{
	tfnWarpField fnWarpField = (tfnWarpField)0x00443E20;
	return fnWarpField( iFieldID, piX, piZ );
}

BOOL CBlessCastleHandler::WarpCastle( BOOL bLeader, int * piX, int * piZ )
{
	int iX, iZ = 0;

	int iRandPosition = RandomI( -140, 140 );

	if ( WarpField( 33, &iX, &iZ ) )
	{
		if ( InSiegeWar() )
		{
			if ( GetSiegeWarMode() == SIEGEWARMODE_TheMassacre )
			{
				iX = RandomI( 33770, 36480 );
				iZ = RandomI( -24276, -23439 );
			}
			else
			{
				if ( bLeader )
				{
					if ( iPhaseID == 1 )
					{
						iX = 34548;
						iZ = -25965;
					}
					else if ( iPhaseID == 2 )
					{
						iX = 34549;
						iZ = -28715;
					}
					else if ( iPhaseID == 3 )
					{
						iX = 34545;
						iZ = -29562;
					}
					else
					{
						if ( RandomI( 0, 100 ) <= 50 )
						{
							iX = 35142;
							iZ = -32539;
						}
						else
						{
							iX = 33979;
							iZ = -32532;
						}
					}
				}
				else
				{
					if ( iPhaseID == 1 )
					{
						return fnOldWarpCastle( FALSE, piX, piZ );
					}
					else if ( iPhaseID == 2 )
					{
						return fnOldWarpCastle( FALSE, piX, piZ );
					}
					else if ( iPhaseID == 3 )
					{
						iX = 34548;
						iZ = -25965;
					}
					else
					{
						if ( RandomI( 0, 100 ) <= 50 )
						{
							iX = 36859;
							iZ = -29044;
						}
						else
						{
							iX = 32306;
							iZ = -29085;
						}
					}
				}
			}

			iX = (iX + iRandPosition) << 8;
			iZ = (iZ + iRandPosition) << 8;

			if ( piX && piZ )
			{
				*piX = iX;
				*piZ = iZ;
			}

			UNITDATA->SetPosition( iX, 0, iZ );

			CAMERAPOSITION->iX		 = UNITDATA->sPosition.iX;
			CAMERAPOSITION->iY		 = UNITDATA->sPosition.iY;
			CAMERAPOSITION->iZ		 = UNITDATA->sPosition.iZ;
			CAMERATARGETPOSITION->iX = UNITDATA->sPosition.iX;
			CAMERATARGETPOSITION->iY = UNITDATA->sPosition.iY;
			CAMERATARGETPOSITION->iZ = UNITDATA->sPosition.iZ;

			return TRUE;
		}

		else
		{
			if ( bLeader )
			{
				iX = 34548;
				iZ = -25965;
				iX = (iX + iRandPosition) << 8;
				iZ = (iZ + iRandPosition) << 8;

				if ( piX && piZ )
				{
					*piX = iX;
					*piZ = iZ;
				}

				UNITDATA->SetPosition( iX, 0, iZ );

				CAMERAPOSITION->iX		 = UNITDATA->sPosition.iX;
				CAMERAPOSITION->iY		 = UNITDATA->sPosition.iY;
				CAMERAPOSITION->iZ		 = UNITDATA->sPosition.iZ;
				CAMERATARGETPOSITION->iX = UNITDATA->sPosition.iX;
				CAMERATARGETPOSITION->iY = UNITDATA->sPosition.iY;
				CAMERATARGETPOSITION->iZ = UNITDATA->sPosition.iZ;

				return TRUE;
			}
			else
				return fnOldWarpCastle( FALSE, piX, piZ );
		}
	}

	return FALSE;
}

void CBlessCastleHandler::Render()
{
	// BC Alive?
	if ( CHARACTERGAME->GetStageID() == 33 )
	{
		UNITDATA->sPVPKills = (short)iPvPKills;

		if ( InSiegeWar() )
		{
			int iCrystalTowerCount = 0;

			if( pWindowRanking )
			{
				//Battle Points
				auto pBattlePointsValue = pWindowRanking->GetElement<UI::Text>( TEXTID_BattlePointsValue );
				if( pBattlePointsValue )
					pBattlePointsValue->SetTextEx( "%d", iPvPKills );

				//User Score
				auto pUserScoreValue = pWindowRanking->GetElement<UI::Text>( TEXTID_UserScoreValue );
				if( pUserScoreValue )
					pUserScoreValue->SetTextEx( "%d", iUserScoreTemp );

				//Hit Rate and Con Rate
				for( size_t i = 0; i < 10; i++ )
				{
					ClanData * psClanData = psaClanData + i;
					if( psClanData->iClanID == GetClanCode(UNITDATA->sCharacterData.iClanID) )
					{
						auto pHitRateValue = pWindowRanking->GetElement<UI::Text>( TEXTID_HitRateValue );
						if( pHitRateValue && READDWORD(0x362BE58) )
							pHitRateValue->SetTextEx( "%d", (psClanData->iScore * 100) / READDWORD(0x362BE58) );

						auto pConRateValue = pWindowRanking->GetElement<UI::Text>( TEXTID_ConRateValue );
						if( pConRateValue && psClanData->iScore )
							pConRateValue->SetTextEx( "%d", (int)iUserScoreTemp * 100 / psClanData->iScore );				

						break;
					}
				}
		
				//Clan Ranking
				for( size_t i = 0; i < 5; i++ )
				{
					ClanData * psClanData = psaClanData + i;
					if( psClanData->bFlag )
					{
						DWORD dwTextColor = D3DCOLOR_XRGB( 228, 144, 63 );
						if( psClanData->iClanID == GetClanCode( UNITDATA->sCharacterData.iClanID ) )
							dwTextColor = D3DCOLOR_XRGB( 255, 226, 89 );

						auto pClanIcon = pWindowRanking->GetElement<UI::ImageBox>( IMAGEID_ClanIcon1 + i );
						auto pClanName = pWindowRanking->GetElement<UI::Text>( TEXTID_ClanName1 + i );
						auto pClanValue = pWindowRanking->GetElement<UI::Text>( TEXTID_ClanValue1 + i );
						if( pClanIcon && pClanName && pClanValue && psClanData->szClanName[0] != 0 )
						{
							/*if( psClanData->psTexture )
								pClanIcon->SetTexture( psClanData->psTexture->iReferenceCount ? psClanData->psTexture->sTexture.pcD3DTexture : NULL );*/

							pClanName->SetText( psClanData->szClanName );
							pClanName->SetColor( dwTextColor );

							pClanValue->SetTextEx( "%d", psClanData->iScore );
							pClanValue->SetColor( dwTextColor );
						}
					}

					if( saCrystalTowerCount[i] && i < 4 )
						iCrystalTowerCount += saCrystalTowerCount[i];
				}


				//Characters Ranking
				for( int i = 0; i < 3; i++ )
				{
					if( iDamage[i] > 0 && szCharacterName[i] != 0 )
					{
						DWORD dwTextColor = D3DCOLOR_XRGB( 228, 144, 63 );
						if( STRINGCOMPARE( szCharacterName[i], UNITDATA->sCharacterData.szName ) )
							dwTextColor = D3DCOLOR_XRGB( 255, 226, 89 );
						
						auto pCharacterName = pWindowRanking->GetElement<UI::Text>( TEXTID_CharacterName1 + i );
						auto pCharacterValue = pWindowRanking->GetElement<UI::Text>( TEXTID_CharacterValue1 + i );
						if( pCharacterName && pCharacterValue )
						{
							pCharacterName->SetText( szCharacterName[i] );
							pCharacterName->SetColor( dwTextColor );

							pCharacterValue->SetTextEx( "%d", iDamage[i] );
							pCharacterValue->SetColor( dwTextColor );
						}
					}
				}

				pWindowRanking->Show();
			}	

			if( pWindowSiegeWarInfo && iPhaseID )
			{
				pWindowSiegeWarInfo->Show();

				auto pTimeLeftText = pWindowSiegeWarInfo->GetElement<UI::Text>( TEXTID_TimeLeft );
				if( pTimeLeftText )
					pTimeLeftText->SetTextEx( "Time Left: %02d:%02d:%02d", iaTimeLeft[2], iaTimeLeft[1], iaTimeLeft[0] );

				auto pPhaseText = pWindowSiegeWarInfo->GetElement<UI::Text>( TEXTID_Phase );
				if( pPhaseText )
					pPhaseText->SetTextEx( iPhaseID == 5 ? "Final Phase" : "Phase %d", iPhaseID );

				for( size_t i = 0; i < 6; i++ )
				{
					auto pCrystalTowerImage = pWindowSiegeWarInfo->GetElement<UI::ImageBox>( IMAGEID_CrystalTowerIcon1 + i );
					if( pCrystalTowerImage )
					{
						pCrystalTowerImage->SetPosition( (pWindowSiegeWarInfo->GetWidth() >> 1) - ((20 * iCrystalTowerCount) >> 1) + (20 * i), 6 );
						pCrystalTowerImage->Show();

						if( i > (UINT)(iCrystalTowerCount - 1) || (iCrystalTowerCount <= 0) )
							pCrystalTowerImage->Hide();
					}
				}
			}

			iUserScoreTemp = (*(int*)0x04B06BB0);

			if( pWindowRanking )
				pWindowRanking->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

			if( pWindowSiegeWarInfo )
				pWindowSiegeWarInfo->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
		}
		else
		{
			if( pWindowRanking )
				pWindowRanking->Hide();

			if( pWindowSiegeWarInfo )
				pWindowSiegeWarInfo->Hide();

            if ( UNITDATA->sPosition.iZ >= -6027850 )
			{
				pFont->Draw( "Safe Zone", RECT{ 0, 98, RESOLUTION_WIDTH, 123 }, DT_NOCLIP | DT_CENTER, DX::Color( 0, 255, 0 ) );
				*(BOOL*)(0x04B06F30) = FALSE;
			}
			else if ( UNITDATA->sPosition.iZ <= -6141650 )
			{
				pFont->Draw( "Battle Zone", RECT{ 0, 98, RESOLUTION_WIDTH, 123 }, DT_NOCLIP | DT_CENTER, DX::Color( 255, 0, 0 ) );
				*(BOOL*)(0x04B06F30) = TRUE;
			}
			else
			{
				pFont->Draw( "Battle Zone", RECT{ 0, 98, RESOLUTION_WIDTH, 123 }, DT_NOCLIP | DT_CENTER, DX::Color( 255, 255, 0 ) );
				*(BOOL*)(0x04B06F30) = TRUE;
			}

			iPhaseID = 0;
		}
	}
	else
		*(BOOL*)(0x04B06F30) = FALSE;

    if ( ((MAP_ID == MAPID_BlessCastle) && (InSiegeWar() == FALSE)) || (MAP_ID == MAP_ID_PVP) )
        RenderBattleStatus();

	//PvP Kill
	if ( psCurrentPvPKillInfo )
	{
		if ( (psCurrentPvPKillInfo->bDelete == FALSE) )
		{
			UI::Image * pc1 = pcaImageKillPvPKillInfo[psCurrentPvPKillInfo->iKillType - 1];
			UI::Image * pc2 = pcaImageCharacterPvPKillInfo[psCurrentPvPKillInfo->iKillerClass - 1];
			UI::Image * pc3 = pcaImageCharacterPvPKillInfo[psCurrentPvPKillInfo->iVictimClass - 1];

			int iX = (RESOLUTION_WIDTH >> 1) - (pc1->GetWidth() >> 1);
			int iY = (RESOLUTION_HEIGHT >> 2);

			int iCharacterX = iX + (pc1->GetWidth() >> 1) - (pc2->GetWidth() >> 1);

			UI::ImageRender::Render( pc1, iX, iY, pc1->GetWidth(), pc1->GetHeight(), RGBA( psCurrentPvPKillInfo->iAlpha, 255, 255, 255 ) );
			UI::ImageRender::Render( pc2, iCharacterX, iY - 70, pc2->GetWidth(), pc2->GetHeight(), RGBA( psCurrentPvPKillInfo->iAlpha, 255, 255, 255 ) );

			GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Killer: %s", psCurrentPvPKillInfo->szKillerName ), iX + 30, iY + pc1->GetHeight() - 28, 0,
				DX::Color( psCurrentPvPKillInfo->iAlpha, 255, 255, 255 ) );

			GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Victim: %s", psCurrentPvPKillInfo->szVictimName ), iX + 240, iY + pc1->GetHeight() - 28, 0,
				DX::Color( psCurrentPvPKillInfo->iAlpha, 255, 255, 255 ) );
		}
	}
}

BOOL CBlessCastleHandler::OnMouseClick( CMouse * pcMouse )
{
	if ( HUDHANDLER->IsWindowOldHUDOpen() == FALSE )
	{
		if ( pWindowRanking->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
			return TRUE;

		if ( pWindowSiegeWarInfo->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 ) )
			return TRUE;
	}

	return FALSE;
}

void CBlessCastleHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindowRanking->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
	pWindowSiegeWarInfo->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CBlessCastleHandler::OnResolutionChanged()
{
	if( pWindowSiegeWarInfo && pWindowRanking )
	{
		pWindowSiegeWarInfo->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowSiegeWarInfo->GetWidth() >> 1), 85 );
		pWindowRanking->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowRanking->GetWidth() >> 1), 164 );
	}
}

void CBlessCastleHandler::PHUpdate( PacketPvPData * psPacket )
{
	iPvPKills	    = psPacket->iPvPKills;
    iPvPDeaths      = psPacket->iPvPDeaths;
    iPvPExperience  = psPacket->iPvPExperience;

    UNIT->iPvPKills         = iPvPKills;
    UNIT->iPvPExperience    = iPvPExperience;

	if ( psPacket->bKill )
		CQUESTGAME->OnUnitPvPKilled( UNITDATABYID( psPacket->iID ) );
}

void CBlessCastleHandler::PHUpdate( PacketSiegeWarPhase * psPacket )
{
	iPhaseID = psPacket->iPhaseID;
}

void CBlessCastleHandler::HandlePacket( PacketPvPKillType * psPacket )
{
	if ( SETTINGSHANDLER->GetModel()->GetSettings().bDebugKill == FALSE )
		return;

	PvPKillInfoData * ps = new PvPKillInfoData;

	ps->bDelete			= FALSE;

	STRINGCOPY( ps->szKillerName, psPacket->szKillerName );
	STRINGCOPY( ps->szVictimName, psPacket->szVictimName );

	ps->iKillerClass	= psPacket->iKillerClass;
	ps->iVictimClass	= psPacket->iVictimClass;

	ps->iKillType		= psPacket->iKillType;

	ps->iAlpha			= 0;

	vPvPKillData.push_back( ps );
}

void CBlessCastleHandler::HandlePacket( PacketBlessCastleTopIndividual * psPacket )
{
	CopyMemory( szCharacterName, psPacket->szCharacterName, sizeof( char ) * 3 * 32 );
	CopyMemory( iClass, psPacket->iClass, sizeof( int ) * 3 );
	CopyMemory( iDamage, psPacket->iDamage, sizeof( int ) * 3 );

	iMode = psPacket->iMode;
}

void CBlessCastleHandler::HandlePacket( PacketBlessCastleUserScoreUpdate * psPacket )
{
	(*(int*)0x04B06BB0) = psPacket->iUserScore;
}

void CBlessCastleHandler::Update( float fTime )
{
	if ( psCurrentPvPKillInfo )
	{
		if ( (psCurrentPvPKillInfo->bDelete == FALSE) )
		{
			if ( dwPvPKillInfoTimeOut >= TICKCOUNT )
			{
				DWORD dwTime = dwPvPKillInfoTimeOut - TICKCOUNT;

				if ( dwTime > 2000 )
				{
					if ( psCurrentPvPKillInfo->iAlpha < 255 )
					{
						psCurrentPvPKillInfo->iAlpha += 40;

						if ( psCurrentPvPKillInfo->iAlpha > 255 )
							psCurrentPvPKillInfo->iAlpha = 255;
					}
				}
				else if ( dwTime <= 500 )
				{
					if ( psCurrentPvPKillInfo->iAlpha > 0 )
					{
						psCurrentPvPKillInfo->iAlpha -= 40;

						if ( psCurrentPvPKillInfo->iAlpha < 0 )
							psCurrentPvPKillInfo->iAlpha = 0;
					}
				}
			}
			else
			{
				psCurrentPvPKillInfo->bDelete = TRUE;
			}
		}
		else
		{
			for ( std::vector<PvPKillInfoData*>::iterator it = vPvPKillData.begin(); it != vPvPKillData.end(); )
			{
				PvPKillInfoData * ps = *it;

				if ( ps && (ps == psCurrentPvPKillInfo) )
				{
					DELET( ps );

					it = vPvPKillData.erase( it );
				}
				else
					it++;
			}

			psCurrentPvPKillInfo = NULL;
		}
	}
	else
	{
		if ( vPvPKillData.size() > 0 )
		{
			psCurrentPvPKillInfo = vPvPKillData[0];
			dwPvPKillInfoTimeOut = TICKCOUNT + 3000;

			SOUNDHANDLER->SetSkillSound( 0x484B + (psCurrentPvPKillInfo->iKillType - 1), UNITDATA->sPosition.iX, UNITDATA->sPosition.iY, UNITDATA->sPosition.iZ );
			SOUNDHANDLER->ChangeFrequencySoundSkill( 0x484B + (psCurrentPvPKillInfo->iKillType - 1), 4410 );
		}
		else
			psCurrentPvPKillInfo = NULL;
	}
}

void CBlessCastleHandler::UpdateObjectSettings()
{
	pWindowSiegeWarInfo->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
	pWindowRanking->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
}

void CBlessCastleHandler::BuildWindowSiegeWarInfo()
{
	pWindowSiegeWarInfo = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (262 >> 1), 85, 262, 72) );
	pWindowSiegeWarInfo->SetImage( UI::ImageLoader::LoadImageA( "game\\images\\misc\\bgsiegewar.png" ) );
	pWindowSiegeWarInfo->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
	pWindowSiegeWarInfo->SetMoveBox( Rectangle2D(0,0,pWindowSiegeWarInfo->GetWidth(),pWindowSiegeWarInfo->GetHeight()) );

	UI::Text_ptr pTimeLeftText = std::make_shared<UI::Text>( "Time Left: 00:00:00", Rectangle2D( 0, 48, 0, 0 ), TRUE, -1 );
	pTimeLeftText->SetID( TEXTID_TimeLeft );
	pTimeLeftText->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, TRUE, D3DCOLOR_XRGB( 137, 179, 255 ) );
	pTimeLeftText->SetDropdownShadow( TRUE );
	pTimeLeftText->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowSiegeWarInfo->AddElement( pTimeLeftText );

	UI::Text_ptr pPhaseText = std::make_shared<UI::Text>( "Phase 1", Rectangle2D( 184, 48, 0, 0 ), TRUE, -1 );
	pPhaseText->SetID( TEXTID_Phase );
	pPhaseText->SetFont( "Frutiger LT 55 Roman", 22, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 165, 0 ) );
	pPhaseText->SetDropdownShadow( TRUE );
	pPhaseText->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowSiegeWarInfo->AddElement( pPhaseText );

	for( size_t i = 0; i < 6; i++ )
	{
		UI::ImageBox_ptr pCrystalTowerIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 67 + (20 * i), 6, 16, 32 ) );
		pCrystalTowerIcon->SetImage( pcCrystalTowerImage );
		pCrystalTowerIcon->SetID( IMAGEID_CrystalTowerIcon1 + i );
		pWindowSiegeWarInfo->AddElement( pCrystalTowerIcon );
	}

	pWindowSiegeWarInfo->Hide();
}

void CBlessCastleHandler::BuildWindowRanking()
{
	pWindowRanking = std::make_shared<UI::Window>( Rectangle2D( (RESOLUTION_WIDTH >> 1) - (530 >> 1), 164, 530, 82) );
	pWindowRanking->SetCanMove( !SETTINGSHANDLER->GetModel()->GetSettings().bLockUI );
	pWindowRanking->SetMoveBox( Rectangle2D(0,0,pWindowRanking->GetWidth(),pWindowRanking->GetHeight()) );

	UI::Text_ptr pBattlePointsLabel = std::make_shared<UI::Text>( "Battle Points", Rectangle2D( 0, 0, 0, 0 ), TRUE, -1 );
	pBattlePointsLabel->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 84, 173, 63 ) );
	pBattlePointsLabel->SetDropdownShadow( TRUE );
	pBattlePointsLabel->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pBattlePointsLabel );

	UI::Text_ptr pHitRateLabel = std::make_shared<UI::Text>( "Hit Rate", Rectangle2D( 0, 16, 0, 0 ), TRUE, -1 );
	pHitRateLabel->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 226, 89 ) );
	pHitRateLabel->SetDropdownShadow( TRUE );
	pHitRateLabel->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pHitRateLabel );

	UI::Text_ptr pConRateLabel = std::make_shared<UI::Text>( "Con Rate", Rectangle2D( 0, 16 * 2, 0, 0 ), TRUE, -1 );
	pConRateLabel->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 226, 89 ) );
	pConRateLabel->SetDropdownShadow( TRUE );
	pConRateLabel->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pConRateLabel );

	UI::Text_ptr pUserCoreLabel = std::make_shared<UI::Text>( "User Score", Rectangle2D( 0, 16 * 3, 0, 0 ), TRUE, -1 );
	pUserCoreLabel->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 226, 89 ) );
	pUserCoreLabel->SetDropdownShadow( TRUE );
	pUserCoreLabel->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pUserCoreLabel );

	UI::Text_ptr pBattlePointsValue = std::make_shared<UI::Text>( "0", Rectangle2D( 130, 0, 0, 0 ), TRUE, -1 );
	pBattlePointsValue->SetID( TEXTID_BattlePointsValue );
	pBattlePointsValue->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 224, 39, 39 ) );
	pBattlePointsValue->SetDropdownShadow( TRUE );
	pBattlePointsValue->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pBattlePointsValue );

	UI::Text_ptr pHitRateValue = std::make_shared<UI::Text>( "0", Rectangle2D( 130, 16, 0, 0 ), TRUE, -1 );
	pHitRateValue->SetID( TEXTID_HitRateValue );
	pHitRateValue->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 226, 89 ) );
	pHitRateValue->SetDropdownShadow( TRUE );
	pHitRateValue->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pHitRateValue );

	UI::Text_ptr pConRateValue = std::make_shared<UI::Text>( "0", Rectangle2D( 130, 16 * 2, 0, 0 ), TRUE, -1 );
	pConRateValue->SetID( TEXTID_ConRateValue );
	pConRateValue->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 226, 89 ) );
	pConRateValue->SetDropdownShadow( TRUE );
	pConRateValue->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pConRateValue );

	UI::Text_ptr pUserScoreValue = std::make_shared<UI::Text>( "0", Rectangle2D( 130, 16 * 3, 0, 0 ), TRUE, -1 );
	pUserScoreValue->SetID( TEXTID_UserScoreValue );
	pUserScoreValue->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 255, 226, 89 ) );
	pUserScoreValue->SetDropdownShadow( TRUE );
	pUserScoreValue->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
	pWindowRanking->AddElement( pUserScoreValue );

	//Clan Ranking
	for( size_t i = 0; i < 5; i++ )
	{
		UI::ImageBox_ptr pClanIcon = std::make_shared<UI::ImageBox>( Rectangle2D( 165, 16 * i, 16, 16 ) );
		pClanIcon->SetID( IMAGEID_ClanIcon1 + i );
		pWindowRanking->AddElement( pClanIcon );

		UI::Text_ptr pClanName = std::make_shared<UI::Text>( "", Rectangle2D( 180, 16 * i, 0, 0 ), TRUE, -1 );
		pClanName->SetID( TEXTID_ClanName1 + i );
		pClanName->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 228, 144, 63 ) );
		pClanName->SetDropdownShadow( TRUE );
		pClanName->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pWindowRanking->AddElement( pClanName );

		UI::Text_ptr pClanValue = std::make_shared<UI::Text>( "", Rectangle2D( 285, 16 * i, 0, 0 ), TRUE, -1 );
		pClanValue->SetID( TEXTID_ClanValue1 + i );
		pClanValue->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 228, 144, 63 ) );
		pClanValue->SetDropdownShadow( TRUE );
		pClanValue->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pWindowRanking->AddElement( pClanValue );
	}

	//Characters Ranking
	for( size_t i = 0; i < 3; i++ )
	{
		UI::Text_ptr pCharacterName = std::make_shared<UI::Text>( "", Rectangle2D( 330, 16 * i, 0, 0 ), TRUE, -1 );
		pCharacterName->SetID( TEXTID_CharacterName1 + i );
		pCharacterName->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 228, 144, 63 ) );
		pCharacterName->SetDropdownShadow( TRUE );
		pCharacterName->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pWindowRanking->AddElement( pCharacterName );

		UI::Text_ptr pCharacterValue = std::make_shared<UI::Text>( "", Rectangle2D( 470, 16 * i, 0, 0 ), TRUE, -1 );
		pCharacterValue->SetID( TEXTID_CharacterValue1 + i );
		pCharacterValue->SetFont( "Arial", 18, 0, FALSE, TRUE, D3DCOLOR_XRGB( 228, 144, 63 ) );
		pCharacterValue->SetDropdownShadow( TRUE );
		pCharacterValue->SetDropdownColor( D3DCOLOR_ARGB( 200, 0, 0, 0 ) );
		pWindowRanking->AddElement( pCharacterValue );
	}

	pWindowRanking->Hide();
}

void CBlessCastleHandler::RenderBattleStatus()
{
    pFont->Draw( std::to_string( iPvPKills ), (RESOLUTION_WIDTH >> 1) + 186, 11, 0, DX::Color( 128, 0, 0, 0 ) );
    pFont->Draw( std::to_string( iPvPKills ), (RESOLUTION_WIDTH >> 1) + 185, 10, 0, DX::Color( 70, 175, 70 ) );
    pFont->Draw( "Battle Points", (RESOLUTION_WIDTH >> 1) + 246, 11, 0, DX::Color( 128, 0, 0, 0 ) );
    pFont->Draw( "Battle Points", (RESOLUTION_WIDTH >> 1) + 245, 10, 0, DX::Color( 160, 14, 14 ) );

    pFont->Draw( std::to_string( iPvPExperience ), (RESOLUTION_WIDTH >> 1) + 186, 37, 0, DX::Color( 128, 0, 0, 0 ) );
    pFont->Draw( std::to_string( iPvPExperience ), (RESOLUTION_WIDTH >> 1) + 185, 36, 0, DX::Color( 70, 175, 70 ) );
    pFont->Draw( "Battle EXP", (RESOLUTION_WIDTH >> 1) + 246, 37, 0, DX::Color( 128, 0, 0, 0 ) );
    pFont->Draw( "Battle EXP", (RESOLUTION_WIDTH >> 1) + 245, 36, 0, DX::Color( 160, 14, 14 ) );
}
