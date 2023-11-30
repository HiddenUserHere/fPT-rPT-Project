#include "stdafx.h"
#include "CWorldScreen.h"

BOOL CWorldScreen::bLoadingTime = FALSE;

CWorldScreen::CWorldScreen()
{
}

CWorldScreen::~CWorldScreen()
{
}

BOOL CWorldScreen::Init()
{
	CALL( 0x00542B30 );

	GRAPHICENGINE->DrawLoadingImage();

	GameCore::SetGameScreen( 2 );

	INIT( MINIMAPHANDLER );

	return TRUE;
}

void CWorldScreen::Shutdown()
{
}

void CWorldScreen::Render3D()
{
	CALL( 0x00416960 ); //Render View Sight?

	Point3D sPosition = Point3D( (*(int*)0x00A17520), (*(int*)0x00A17510), (*(int*)0x00A1619C) );

	Point3D sAngle = Point3D( (*(int*)0x00824FA8), (*(int*)0x00A17630) & 0x00000FFF, (*(int*)0x00A17634) & 0x00000FFF );


	//	Render( sPosition.iX, sPosition.iY, sPosition.iZ, sAngle.iX, sAngle.iY, sAngle.iZ );

	CALL( 0x0041B630 );
}

void CWorldScreen::Render2D()
{
	RENDERER->CloseCamera();

	if ( ISBIGMAP )
		CALL( 0x00417FE0 );
	else if ( bHUDInitialized )
	{
		HUDHANDLER->Render();
		BLESSCASTLEHANDLER->Render();
	}
}

EScreen CWorldScreen::Frame()
{
	MOUSEHANDLER->FixedToRealPosition();

	SOCKETGAME->Loop();

	CALL( 0x00416A20 ); //Camera Movement...

	UNITDATA->bRunMode = GameCore::IsWalking();

	if ( !ISBIGMAP )
	{
		Point2D * p = ((Point2D*)0x00A17518);

		CMouse::AdjustMouseToResolution( p, TRUE );
		CALL( 0x004D0740 );
		CMouse::AdjustMouseToResolution( p, FALSE );
	}
	else
	{
		CALL( 0x0050B050 );
	}

	if ( (*(BOOL*)0x03A97614) )
	{
		CALL( 0x00404870 );
	}

	return SCREEN_World;
}

void CWorldScreen::Update( float fTime )
{
	if( UNITDATA && !bHUDInitialized )
	{
		if( UNITDATA->sCharacterData.iFlag > 0 )
		{
			bHUDInitialized = true;
			INIT( HUDHANDLER );
			OnResolutionChanged();
		}
	}
}

void CWorldScreen::OnMouseMove( CMouse * pcMouse )
{
	if( bHUDInitialized )
		HUDHANDLER->OnMouseMove( pcMouse );
}

BOOL CWorldScreen::OnMouseClick( CMouse * pcMouse )
{
	if( bHUDInitialized )
		return HUDHANDLER->OnMouseClick( pcMouse );

	return FALSE;
}

BOOL CWorldScreen::OnKeyPress( CKeyboard * pcKeyboard )
{
	BOOL bRet = FALSE;

	if( (pcKeyboard->GetKey() == VK_F11) && GM_MODE )
	{
		ISSHOW_INTERFACE = FALSE;
		return TRUE;
	}

	if( (pcKeyboard->GetKey() == VK_F12) && GM_MODE )
	{
		ISSHOW_INTERFACE = TRUE;
		return TRUE;
	}

	if( bHUDInitialized )
		bRet = HUDHANDLER->OnKeyPress( pcKeyboard );

	if( pcKeyboard->GetKey() == 'F' && pcKeyboard->GetEvent() == KeyDown )
		if( CURRENT_TARGET && (CURRENT_TARGET->sCharacterData.iType != CHARACTERTYPE_NPC) && (CURRENT_TARGET->sCharacterData.iType != CHARACTERTYPE_Player) && (CURRENT_TARGET->iCharacterType != CHARACTERTYPE_Player) )
			CHARACTERGAME->FocusTargetPet( CURRENT_TARGET->iID );

	return bRet;
}

BOOL CWorldScreen::OnMouseScroll( CMouse * pcMouse )
{
	if( bHUDInitialized )
		return HUDHANDLER->OnMouseScroll( pcMouse );

	return FALSE;
}

BOOL CWorldScreen::OnKeyChar( CKeyboard * pcKeyboard )
{
	if( bHUDInitialized )
		return HUDHANDLER->OnKeyChar( pcKeyboard );

	return FALSE;
}

void CWorldScreen::OnResolutionChanged()
{
	if( bHUDInitialized )
		HUDHANDLER->OnResolutionChanged();
}

std::vector<UnitData *> vUnitsTest;

struct PlayerTestData
{
	int iClass;
	std::string strBody;
	std::string strHead;

	PlayerTestData( int _iClass, std::string _strHead, std::string _strBody )
	{
		iClass = _iClass;
		strHead = _strHead;
		strBody = _strBody;
	};

	~PlayerTestData() {};
};

std::vector<PlayerTestData> vUnitsSave;

void CWorldScreen::GetPlayerUnits()
{
	vUnitsSave.clear();

	for ( int i = 0; i < MAX_UNITS; i++ )
	{
		auto pc = UNITGAME->pcaUnitData + i;
		if ( pc->bActive && pc->IsPlayer() )
		{
			vUnitsSave.push_back( PlayerTestData( pc->sCharacterData.iClass, pc->sCharacterData.Player.szHeadModel, pc->sCharacterData.Player.szBodyModel ) );
		}
	}
}


void CWorldScreen::LoadUnitsTest( int iCount )
{
	for ( int i = 0; i < vUnitsSave.size(); i++ )
	{
		auto pcUnitData = new UnitData();

		pcUnitData->Init();
        if ( (STAGE1 != NULL) && (UNITDATA->iLoadedMapIndex == 0) )
			pcUnitData->pcStage = STAGE1;
		if ( (STAGE2 != NULL) && (UNITDATA->iLoadedMapIndex == 1) )
			pcUnitData->pcStage = STAGE2;

		ECharacterClass iClass = (ECharacterClass)vUnitsSave[i].iClass;
		pcUnitData->sCharacterData.iRank = CHARACTERRANK_Rank5;

		//Hair
		char szNewHeadModel[128] = {0};

		char cTier[2] = {pcUnitData->sCharacterData.iRank ? 0x60 + (char)pcUnitData->sCharacterData.iRank : 0, 0};
		char cChar[2] = {pcUnitData->sCharacterData.iRank == 2 ? '_' : '-', 0};

		switch ( iClass )
		{
			case CHARACTERCLASS_Fighter:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sB01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Mechanician:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sA01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Archer:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sD01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Pikeman:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tmh%sC01%s.inf", cChar, cTier );
				if ( UNITDATA->sCharacterData.iRank == 3 )
					STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\TmhC-01c.inf" );
				break;

			case CHARACTERCLASS_Atalanta:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Knight:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Magician:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Priestess:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Assassin:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Tfh%sE01%s.inf", cChar, cTier );
				break;

			case CHARACTERCLASS_Shaman:
				STRINGFORMAT( szNewHeadModel, "char\\tmABCD\\Mmh%sE01%s.inf", cChar, cTier );
				break;

			default:
				break;
		}

		Point3D sPosition = UNITDATA->sPosition;
		Point3D sAngle = Point3D( 0, 2048, 0 );

		sPosition.iX += (8 * 256) * (i + 1);
		sPosition.iZ += (8 * 256) * (i + 1);

		pcUnitData->sCharacterData.iClass = iClass;

		LoadUnitData( pcUnitData, vUnitsSave[i].strBody.c_str(), szNewHeadModel, &sPosition, &sAngle );
		pcUnitData->eUpdateMode = UPDATEMODE_ClientSelf;

		pcUnitData->sCharacterData.sWarpHomeTown = 0;
		pcUnitData->sCharacterData.iClass = iClass;

		pcUnitData->SetPosition( sPosition.iX, pcUnitData->pcStage->GetHighestPoint( sPosition.iX, sPosition.iZ ), sPosition.iZ, 0, sAngle.iY, 0 );

		pcUnitData->bActive = TRUE;
		pcUnitData->bVisible = TRUE;

		pcUnitData->ChangeAnimationID( 0 );

		vUnitsTest.push_back( pcUnitData );
	}
}

void CWorldScreen::RenderTestUnits()
{
	for ( auto pc : vUnitsTest )
		pc->Render();
}

void CWorldScreen::UpdateTestUnits()
{
    for ( auto pc : vUnitsTest )
        pc->Main();
}

DWORD CWorldScreen::ThreadLoader()
{

	ExitThread( 0 );

	return 0;
}
