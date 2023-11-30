#include "stdafx.h"
#include "CSettingsWindow.h"


extern PacketServerInfo sServerInfo;

CSettingsWindow::CSettingsWindow()
{
}


CSettingsWindow::~CSettingsWindow()
{
}

void CSettingsWindow::SetResolutionSettings()
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );

	if ( pGroupRatio->GetIndexObject() == CHECKBOXID_Ratio4_3 )
	{
		int iNum = pGroupResolution43->GetIndexObject();
		if ( iNum == CHECKBOXID_1024x768 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1024;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 768;
		}
		else if ( iNum == CHECKBOXID_1280x960 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1280;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 960;
		}
		else if ( iNum == CHECKBOXID_1400x1050 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1400;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 1050;
		}
		else
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 800;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 600;
		}
	}
	else
	{
		int iNum = pGroupResolution169->GetIndexObject();
		if ( iNum == CHECKBOXID_1280x720 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1280;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 720;
		}
		else if ( iNum == CHECKBOXID_1366x768 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1366;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 768;
		}
		else if ( iNum == CHECKBOXID_1600x900 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1600;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 900;
		}
		else if ( iNum == CHECKBOXID_1920x1080 )
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 1920;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 1080;
		}
		else
		{
			SETTINGSHANDLER->GetModel()->GetSettings().dwWidth = 800;
			SETTINGSHANDLER->GetModel()->GetSettings().dwHeight = 600;
		}
	}
}

int CSettingsWindow::GetResolutionCheckBox()
{
	DWORD dwWidth	= SETTINGSHANDLER->GetModel()->GetSettings().dwWidth;
	DWORD dwHeight	= SETTINGSHANDLER->GetModel()->GetSettings().dwHeight;

	// 4:3
	if ( dwWidth == 1024 && dwHeight == 768 )
		return CHECKBOXID_1024x768;
	if ( dwWidth == 1280 && dwHeight == 960 )
		return CHECKBOXID_1280x960;
	if ( dwWidth == 1400 && dwHeight == 1050 )
		return CHECKBOXID_1400x1050;

	// 16:9
	if ( dwWidth == 1280 && dwHeight == 720 )
		return CHECKBOXID_1280x720;
	if ( dwWidth == 1366 && dwHeight == 768 )
		return CHECKBOXID_1366x768;
	if ( dwWidth == 1600 && dwHeight == 900 )
		return CHECKBOXID_1600x900;
	if ( dwWidth == 1920 && dwHeight == 1080 )
		return CHECKBOXID_1920x1080;

	return -1;
}

void CSettingsWindow::SetResolutionCheckBox()
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );

	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
	

	if ( SETTINGSHANDLER->GetModel()->GetSettings().bWidescreen )
	{
		pGroupResolution169->SetIndexObject( GetResolutionCheckBox() );
		pGroupRatio->SetIndexObject( CHECKBOXID_Ratio16_9 );
		pGroupResolution43->Hide();
		pGroupResolution169->Show();
	}
	else
	{
		pGroupResolution43->SetIndexObject( GetResolutionCheckBox() );
		pGroupRatio->SetIndexObject( CHECKBOXID_Ratio4_3 );
		pGroupResolution169->Hide();
		pGroupResolution43->Show();
	}
}

void CSettingsWindow::Load()
{
	SETTINGSHANDLER->GetModel()->ReadSettings();
	
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	auto pWindowGeneral = GetWindow( WINDOWID_General );
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pWindowInterface = GetWindow( WINDOWID_Interface );

	auto pButtonVideo = pWindow1->GetElement<UI::Button>( BUTTONID_Video );
	auto pButtonAudio = pWindow1->GetElement<UI::Button>( BUTTONID_Audio );
	auto pButtonGeneral = pWindow1->GetElement<UI::Button>( BUTTONID_General );
	auto pButtonInterface = pWindow1->GetElement<UI::Button>( BUTTONID_Interface );
	auto pGroupMenu = pWindow1->GetElement<UI::Group>( GROUPID_Menu );

	if( pWindowAudio->IsOpen() )
	{
		pButtonAudio->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_Audio );
	}
	else if( pWindowGeneral->IsOpen() )
	{
		pButtonGeneral->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_General );
	}
	else if( pWindowVideo->IsOpen() )
	{
		pButtonVideo->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_Video );
	}
	else if( pWindowInterface->IsOpen() )
	{
		pButtonInterface->SetSelected( TRUE );
		pGroupMenu->SetIndexObject( BUTTONID_Interface );
	}

	// Video
	// Screen
	{
		auto pCheckBoxAutoAdjust = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_AutoAdjust );
		pCheckBoxAutoAdjust->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bAutoAdjust );

		auto pCheckBoxStartMaximized = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_StartMaximized );
		pCheckBoxStartMaximized->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bStartMaximized );

		auto pGroupDisplay = pWindowVideo->GetElement<UI::Group>( GROUPID_Display );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed )
			pGroupDisplay->SetIndexObject( CHECKBOXID_Windowed );
		else
			pGroupDisplay->SetIndexObject( CHECKBOXID_Fullscreen );

		auto pGroupRenderer = pWindowVideo->GetElement<UI::Group>( GROUPID_Renderer );
		if ( (Delta3D::Graphics::RendererVersion)SETTINGSHANDLER->GetModel()->GetSettings().iRendererVersion == Delta3D::Graphics::RendererVersion::RENDERERVERSION_DirectX9Ex )
			pGroupRenderer->SetIndexObject( CHECKBOXID_DirectX9Ex );
		else
			pGroupRenderer->SetIndexObject( CHECKBOXID_DirectX9 );

		SetResolutionCheckBox();
	}
	// Graphics
	{
		auto pGroupTexture = pWindowVideo->GetElement<UI::Group>( GROUPID_TextureQuality );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bHighTextureQuality )
			pGroupTexture->SetIndexObject( CHECKBOXID_TextureHigh );
		else
			pGroupTexture->SetIndexObject( CHECKBOXID_TextureLow );


		auto pGroupBitDepth = pWindowVideo->GetElement<UI::Group>( GROUPID_BitDepth );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().dwBitDepth == 16 )
			pGroupBitDepth->SetIndexObject( CHECKBOXID_16BPP );
		else
			pGroupBitDepth->SetIndexObject( CHECKBOXID_32BPP );

        auto pGroupEffects = pWindowVideo->GetElement<UI::Group>( GROUPID_Effects );
        if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects )
            pGroupEffects->SetIndexObject( CHECKBOXID_EffectsOFF );
        else
            pGroupEffects->SetIndexObject( CHECKBOXID_EffectsON );

		auto pGroupDynamicShadows = pWindowVideo->GetElement<UI::Group>( GROUPID_DynamicShadows );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bDynamicShadows == FALSE )
			pGroupDynamicShadows->SetIndexObject( CHECKBOXID_DynamicShadowsOFF );
		else
			pGroupDynamicShadows->SetIndexObject( CHECKBOXID_DynamicShadowsON );

		auto pGroupNormalMapping = pWindowVideo->GetElement<UI::Group>( GROUPID_NormalMapping );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNormalMapping == FALSE )
			pGroupNormalMapping->SetIndexObject( CHECKBOXID_NormalMappingOFF );
		else
			pGroupNormalMapping->SetIndexObject( CHECKBOXID_NormalMappingON );

		auto pGroupVSync = pWindowVideo->GetElement<UI::Group>( GROUPID_VSync );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bVSync == TRUE )
			pGroupVSync->SetIndexObject( CHECKBOXID_VSyncON );
		else
			pGroupVSync->SetIndexObject( CHECKBOXID_VSyncOFF );

		auto pGroupAntiAliasing = pWindowVideo->GetElement<UI::Group>( GROUPID_AntiAliasing );

		if( SETTINGSHANDLER->GetModel()->GetSettings().bAntiAliasing == TRUE )
			pGroupAntiAliasing->SetIndexObject( CHECKBOXID_AntiAliasingON );
		else
			pGroupAntiAliasing->SetIndexObject( CHECKBOXID_AntiAliasingOFF );
	}
	// Camera
	{
		auto pGroupCameraSight = pWindowVideo->GetElement<UI::Group>( GROUPID_CameraSight );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 0 )
			pGroupCameraSight->SetIndexObject( CHECKBOXID_Short );
		else if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 1 )
			pGroupCameraSight->SetIndexObject( CHECKBOXID_Far );
		else if( SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange == 2 )
			pGroupCameraSight->SetIndexObject( CHECKBOXID_Full );

		auto pGroupCameraInvert = pWindowVideo->GetElement<UI::Group>( GROUPID_InvertedCamera );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bInvertedCamera )
			pGroupCameraInvert->SetIndexObject( CHECKBOXID_CameraON );
		else
			pGroupCameraInvert->SetIndexObject( CHECKBOXID_CameraOFF );	
	}

	//Audio
	// BG Music
	{
		auto pScrollBGMusic = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_BGMusic );
		auto pCheckBoxNoMusic = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoMusic );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic )
		{
			pCheckBoxNoMusic->SetCheck( TRUE );
		}
		else
		{
			pCheckBoxNoMusic->SetCheck( FALSE );
		}
		pScrollBGMusic->SetPercent( SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume );
	}
	// Effect iSound
	{
		auto pScrollEffectSound = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_EffectSound );
		auto pCheckBoxNoEffects = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoEffects );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bNoSoundEffects )
		{
			pCheckBoxNoEffects->SetCheck( TRUE );
		}
		else
		{
			pCheckBoxNoEffects->SetCheck( FALSE );
		}
		pScrollEffectSound->SetPercent( SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume );
	}

	// Boss Time and Server Time
	{
		char szBuf[64] = { 0 };
		STRINGFORMAT( szBuf, "xx:%02d", sServerInfo.iBossTime );
		auto pTextBossTime = pWindow1->GetElement<UI::Text>( TextID_BossTime );
		pTextBossTime->SetText( szBuf );
	
		auto pTextServerTime = pWindow1->GetElement<UI::Text>( TextID_ServerTime );
		STRINGFORMAT( szBuf, "%02d:%02d:%02d\n%02d-%02d-%04d", GameCore::GetInstance()->sServerTime.wHour, GameCore::GetInstance()->sServerTime.wMinute, GameCore::GetInstance()->sServerTime.wSecond, GameCore::GetInstance()->sServerTime.wDay, GameCore::GetInstance()->sServerTime.wMonth, GameCore::GetInstance()->sServerTime.wYear );
		pTextServerTime->SetText( szBuf );

		auto pTextServerName = pWindow1->GetElement<UI::Text>( TextID_ServerName );
		STRINGFORMAT( szBuf, "%s", (char *)(0x038FEF40 + ((*( int* )0x4B06960 - 1 ) * 0x68)) );
		pTextServerName->SetText( szBuf );

		auto pTextSiegeWarTime = pWindow1->GetElement<UI::Text>( TEXTID_SiegeWarTime );
		STRINGFORMAT( szBuf, "%02d:00h, %s", sServerInfo.sSiegeWarHour, GetDayOfWeek(sServerInfo.sSiegeWarDay).c_str() );
		pTextSiegeWarTime->SetText( szBuf );

		auto pTextSiegeWarType = pWindow1->GetElement<UI::Text>( TEXTID_SiegeWarType );		
		auto strSiegeWarMode = "";
		if( sServerInfo.bSiegeWarType == SIEGEWARMODE_Normal )
			strSiegeWarMode = "Normal";
		else if( sServerInfo.bSiegeWarType == SIEGEWARMODE_TheConquest )
			strSiegeWarMode = "The Conquest";
		else if( sServerInfo.bSiegeWarType == SIEGEWARMODE_TheMassacre )
			strSiegeWarMode = "The Massacre";

		STRINGFORMAT( szBuf, "%s", strSiegeWarMode );
		pTextSiegeWarType->SetText( szBuf );
	}

	// Macro
	{
		UI::InputField_ptr pInputField;

		for ( int i = 0; i < 10; i++ )
		{
			pInputField = pWindowGeneral->GetElement<UI::InputField>( InputFieldID_Macro1 + i );
			pInputField->SetText( SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[i] );
		}

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoWhisper )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoWhisper );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoTrade )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeChat );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableParty )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoPartyRequest );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableTrade )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeRequest );
	}

	//Personalization
	{
		auto pGroupDebugDamage = pWindowGeneral->GetElement<UI::Group>( GROUPID_DamageDebug );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bDebugDamage )
			pGroupDebugDamage->SetIndexObject( CHECKBOXID_DamageDebugON );
		else
			pGroupDebugDamage->SetIndexObject( CHECKBOXID_DamageDebugOFF );

		auto pGroupHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_HPBar );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnHead )
			pGroupHPBar->SetIndexObject( CHECKBOXID_HPBarON );
		else
			pGroupHPBar->SetIndexObject( CHECKBOXID_HPBarOFF );

		auto pGroupFPS = pWindowGeneral->GetElement<UI::Group>( GROUPID_FPS );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bFPS )
			pGroupFPS->SetIndexObject( CHECKBOXID_FPSON );
		else
			pGroupFPS->SetIndexObject( CHECKBOXID_FPSOFF );

		auto pGroupPing = pWindowGeneral->GetElement<UI::Group>( GROUPID_Ping );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bPing )
			pGroupPing->SetIndexObject( CHECKBOXID_PingON );
		else
			pGroupPing->SetIndexObject( CHECKBOXID_PingOFF );

		auto pGroupKillInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_KillInfo );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bDebugKill )
			pGroupKillInfo->SetIndexObject( CHECKBOXID_KillInfoON );
		else
			pGroupKillInfo->SetIndexObject( CHECKBOXID_KillInfoOFF );

		auto pGroupElement = pWindowGeneral->GetElement<UI::Group>( GROUPID_Element );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bElementEffect )
			pGroupElement->SetIndexObject( CHECKBOXID_ElementON );
		else
			pGroupElement->SetIndexObject( CHECKBOXID_ElementOFF );

		auto pGroupAgingArmor = pWindowGeneral->GetElement<UI::Group>( GROUPID_AgingArmor );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bArmorAgeEffect )
			pGroupAgingArmor->SetIndexObject( CHECKBOXID_AgingArmorON );
		else
			pGroupAgingArmor->SetIndexObject( CHECKBOXID_AgingArmorOFF );


		auto pGroupHideCaravans = pWindowGeneral->GetElement<UI::Group>( GROUPID_HideCaravans );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bHideCaravans )
			pGroupHideCaravans->SetIndexObject( CHECKBOXID_HideCaravanON );
		else
			pGroupHideCaravans->SetIndexObject( CHECKBOXID_HideCaravanOFF );

		//Loot
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootFilterEnable )->SetCheck( SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootHPPotion )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_HPPotion ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMPPotion )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_MPPotion ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSPPotion )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_SPPotion ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGold )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Gold ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootAmulets )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Amulets ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootRings )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Rings ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSheltoms )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Sheltoms ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootForceOrbs )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_ForceOrbs ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootPremiums )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Premiums ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMonsterCrystals )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_MonsterCrystals ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootDefenseItems )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_DefenseItems ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootOffenseItems )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_OffenseItems ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootEverything )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLoot( LOOTFILTER_Everything ) );

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecNS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_NS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecFS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_FS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_MS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecAS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_AS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_PS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecATA )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_ATA ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecKS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_KS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMGS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_MGS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPRS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_PRS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecASS )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_ASS ) );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecSHA )->SetCheck( SETTINGSHANDLER->GetModel()->CanViewLootSpec( LOOTFILTERSPEC_SHA ) );
	}

	//User Interface
	{
		auto pCheckBoxLockUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_LockUI );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bLockUI )
			pCheckBoxLockUI->SetCheck( TRUE );

		auto pCheckBoxGlow = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_Glow );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bGlow )
			pCheckBoxGlow->SetCheck( TRUE );

		//	auto pCheckBoxSaveUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_SaveUI );
	//	if( SETTINGSHANDLER->GetModel()->GetSettings().bSaveUI )
	//		pCheckBoxSaveUI->SetCheck( TRUE );

		auto pCheckBoxShowParty = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowPartyMembers );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowPartyMembers )
			pCheckBoxShowParty->SetCheck( TRUE );

		auto pCheckBoxShowRaid = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowRaidMembers );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowRaidMembers )
			pCheckBoxShowRaid->SetCheck( TRUE );

		auto pCheckBoxShowClan = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowClanMembers );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowClanMembers )
			pCheckBoxShowClan->SetCheck( TRUE );

		auto pCheckBoxShowEnemies = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowEnemies );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies )
			pCheckBoxShowEnemies->SetCheck( TRUE );

		auto pCheckBoxShowNPCs = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNPCs );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowNPCs )
			pCheckBoxShowNPCs->SetCheck( TRUE );

		auto pCheckBoxShowAlwaysActive = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_AlwaysActive );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bMinimapAlwaysActive )
			pCheckBoxShowAlwaysActive->SetCheck( TRUE );

		auto pCheckBoxShowNextBossTimer = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNextBossTimer );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowNextBossTimer )
			pCheckBoxShowNextBossTimer->SetCheck( TRUE );

		auto pCheckBoxShowNotice = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNotice );
		if( SETTINGSHANDLER->GetModel()->GetSettings().bShowNotice )
			pCheckBoxShowNotice->SetCheck( TRUE );

		auto pCheckBoxTargetDistance = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetDistance );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue )
			pCheckBoxTargetDistance->SetCheck( TRUE );

		auto pCheckBoxTargetHPValue = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPValue );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue )
			pCheckBoxTargetHPValue->SetCheck( TRUE );

		auto pCheckBoxTargetHPPercent = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPPercent );
		if ( SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPPercent )
			pCheckBoxTargetHPPercent->SetCheck( TRUE );
	}
}

void CSettingsWindow::Save()
{
	if( MESSAGEBOX->GetType() == FALSE )
	{
		pWindow1->Show();
		return;
	}

	SetResolutionSettings();

	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	auto pWindowGeneral = GetWindow( WINDOWID_General );
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pWindowInterface = GetWindow( WINDOWID_Interface );

	// Video
	// Screen
	{
		auto pCheckBoxAutoAdjust = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_AutoAdjust );
		SETTINGSHANDLER->GetModel()->GetSettings().bAutoAdjust = pCheckBoxAutoAdjust->IsChecked();

		auto pCheckBoxStartMaximized = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_StartMaximized );
		SETTINGSHANDLER->GetModel()->GetSettings().bStartMaximized = pCheckBoxStartMaximized->IsChecked();

		auto pGroupDisplay = pWindowVideo->GetElement<UI::Group>( GROUPID_Display );
		SETTINGSHANDLER->GetModel()->GetSettings().bWindowed = pGroupDisplay->GetIndexObject() == CHECKBOXID_Windowed ? TRUE : FALSE;

		auto pGroupRenderer = pWindowVideo->GetElement<UI::Group>( GROUPID_Renderer );
		SETTINGSHANDLER->GetModel()->GetSettings().iRendererVersion = pGroupRenderer->GetIndexObject() == CHECKBOXID_DirectX9 ? (int)Delta3D::Graphics::RendererVersion::RENDERERVERSION_DirectX9 : (int)Delta3D::Graphics::RendererVersion::RENDERERVERSION_DirectX9Ex;

		auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
		SETTINGSHANDLER->GetModel()->GetSettings().bWidescreen = pGroupRatio->GetIndexObject() == CHECKBOXID_Ratio4_3 ? FALSE : TRUE;

		SetResolutionCheckBox();		
	}
	// Graphics
	{
		auto pGroupTexture = pWindowVideo->GetElement<UI::Group>( GROUPID_TextureQuality );
		SETTINGSHANDLER->GetModel()->GetSettings().bHighTextureQuality = pGroupTexture->GetIndexObject() == CHECKBOXID_TextureHigh ? TRUE : FALSE;

		auto pGroupBitDepth = pWindowVideo->GetElement<UI::Group>( GROUPID_BitDepth );
		SETTINGSHANDLER->GetModel()->GetSettings().dwBitDepth = pGroupBitDepth->GetIndexObject() == CHECKBOXID_16BPP ? 16 : 32;

        auto pGroupEffects = pWindowVideo->GetElement<UI::Group>( GROUPID_Effects );
        SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects = pGroupEffects->GetIndexObject() == CHECKBOXID_EffectsON ? FALSE : TRUE;

		auto pGroupDynamicShadows = pWindowVideo->GetElement<UI::Group>( GROUPID_DynamicShadows );
		SETTINGSHANDLER->GetModel()->GetSettings().bDynamicShadows = pGroupDynamicShadows->GetIndexObject() == CHECKBOXID_DynamicShadowsON ? TRUE : FALSE;

		auto pGroupNormalMapping = pWindowVideo->GetElement<UI::Group>( GROUPID_NormalMapping );
		SETTINGSHANDLER->GetModel()->GetSettings().bNormalMapping = pGroupNormalMapping->GetIndexObject() == CHECKBOXID_NormalMappingON ? TRUE : FALSE;

		auto pGroupVSync = pWindowVideo->GetElement<UI::Group>( GROUPID_VSync );
		SETTINGSHANDLER->GetModel()->GetSettings().bVSync = pGroupVSync->GetIndexObject() == CHECKBOXID_VSyncON ? TRUE : FALSE;

		auto pGroupAntiAliasing = pWindowVideo->GetElement<UI::Group>( GROUPID_AntiAliasing );
		SETTINGSHANDLER->GetModel()->GetSettings().bAntiAliasing = pGroupAntiAliasing->GetIndexObject() == CHECKBOXID_AntiAliasingON ? TRUE : FALSE;
	}
	// Camera
	{
		auto pGroupCameraSight = pWindowVideo->GetElement<UI::Group>( GROUPID_CameraSight );
		if( pGroupCameraSight->GetIndexObject() == CHECKBOXID_Short )
			SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange = 0;
		else if( pGroupCameraSight->GetIndexObject() == CHECKBOXID_Far )
			SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange = 1;
		else if( pGroupCameraSight->GetIndexObject() == CHECKBOXID_Full )
			SETTINGSHANDLER->GetModel()->GetSettings().iCameraRange = 2;

		auto pGroupCameraInvert = pWindowVideo->GetElement<UI::Group>( GROUPID_InvertedCamera );
		SETTINGSHANDLER->GetModel()->GetSettings().bInvertedCamera = pGroupCameraInvert->GetIndexObject() == CHECKBOXID_CameraON ? TRUE : FALSE;
	}

	//Personalization
	{
		auto pGroupDebugDamage = pWindowGeneral->GetElement<UI::Group>( GROUPID_DamageDebug );
		SETTINGSHANDLER->GetModel()->GetSettings().bDebugDamage = pGroupDebugDamage->GetIndexObject() == CHECKBOXID_DamageDebugON ? TRUE : FALSE;

		auto pGroupHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_HPBar );
		SETTINGSHANDLER->GetModel()->GetSettings().bHPBarOnHead = pGroupHPBar->GetIndexObject() == CHECKBOXID_HPBarON ? TRUE : FALSE;

		auto pGroupFPS = pWindowGeneral->GetElement<UI::Group>( GROUPID_FPS );
		SETTINGSHANDLER->GetModel()->GetSettings().bFPS = pGroupFPS->GetIndexObject() == CHECKBOXID_FPSON ? TRUE : FALSE;

		auto pGroupPing = pWindowGeneral->GetElement<UI::Group>( GROUPID_Ping );
		SETTINGSHANDLER->GetModel()->GetSettings().bPing = pGroupPing->GetIndexObject() == CHECKBOXID_PingON ? TRUE : FALSE;

		auto pGroupKillInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_KillInfo );
		SETTINGSHANDLER->GetModel()->GetSettings().bDebugKill = pGroupKillInfo->GetIndexObject() == CHECKBOXID_KillInfoON ? TRUE : FALSE;

		auto pGroupElement = pWindowGeneral->GetElement<UI::Group>( GROUPID_Element );
		SETTINGSHANDLER->GetModel()->GetSettings().bElementEffect = pGroupElement->GetIndexObject() == CHECKBOXID_ElementON ? TRUE : FALSE;

		auto pGroupAgingArmor = pWindowGeneral->GetElement<UI::Group>( GROUPID_AgingArmor );
		SETTINGSHANDLER->GetModel()->GetSettings().bArmorAgeEffect = pGroupAgingArmor->GetIndexObject() == CHECKBOXID_AgingArmorON ? TRUE : FALSE;

		auto pGroupHideCaravan = pWindowGeneral->GetElement<UI::Group>( GROUPID_HideCaravans );
		SETTINGSHANDLER->GetModel()->GetSettings().bHideCaravans = pGroupHideCaravan->GetIndexObject() == CHECKBOXID_HideCaravanON ? TRUE : FALSE;

		//Loot
		SETTINGSHANDLER->GetModel()->GetSettings().bEnableLootFilter = pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootFilterEnable )->IsChecked();
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_HPPotion, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootHPPotion )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_MPPotion, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMPPotion )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_SPPotion, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSPPotion )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Gold, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGold )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Amulets, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootAmulets )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Rings, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootRings )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Sheltoms, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSheltoms )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_ForceOrbs, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootForceOrbs )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Premiums, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootPremiums )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_MonsterCrystals, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMonsterCrystals )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_DefenseItems, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootDefenseItems )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_OffenseItems, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootOffenseItems )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLoot( LOOTFILTER_Everything, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootEverything )->IsChecked() );

		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_NS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecNS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_FS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecFS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_MS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_AS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecAS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_PS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_ATA, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecATA )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_KS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecKS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_MGS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMGS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_PRS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPRS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_ASS, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecASS )->IsChecked() );
		SETTINGSHANDLER->GetModel()->SetViewLootSpec( LOOTFILTERSPEC_SHA, pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecSHA )->IsChecked() );
	}

	//Interface
	{
		auto pCheckBoxLockUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_LockUI );
		auto pCheckBoxGlow = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_Glow );
		//	auto pCheckBoxSaveUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_SaveUI );
		auto pCheckBoxShowParty = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowPartyMembers );
		auto pCheckBoxShowRaid = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowRaidMembers );
		auto pCheckBoxShowClan = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowClanMembers );
		auto pCheckBoxShowEnemies = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowEnemies );
		auto pCheckBoxShowNPCs = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNPCs );
		auto pCheckBoxAlwaysActive = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_AlwaysActive );
		auto pCheckBoxNextBossTimer = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNextBossTimer );
		auto pCheckBoxShowNotice = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNotice );
		auto pCheckBoxTargetDistance = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetDistance );
		auto pCheckBoxTargetHPValue = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPValue );
		auto pCheckBoxTargetHPPercent = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPPercent );


		SETTINGSHANDLER->GetModel()->GetSettings().bLockUI = pCheckBoxLockUI->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bGlow = pCheckBoxGlow->IsChecked() ? TRUE : FALSE;
		//	SETTINGSHANDLER->GetModel()->GetSettings().bSaveUI = pCheckBoxSaveUI->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowPartyMembers = pCheckBoxShowParty->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowRaidMembers = pCheckBoxShowRaid->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowClanMembers = pCheckBoxShowClan->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNPCs = pCheckBoxShowNPCs->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowEnemies = pCheckBoxShowEnemies->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bMinimapAlwaysActive = pCheckBoxAlwaysActive->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNextBossTimer = pCheckBoxNextBossTimer->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bShowNotice = pCheckBoxShowNotice->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bTargetDistance = pCheckBoxTargetDistance->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPValue = pCheckBoxTargetHPValue->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().bTargetHPPercent = pCheckBoxTargetHPPercent->IsChecked() ? TRUE : FALSE;
	}

	//Audio
	// BG Music
	{
		auto pScrollBGMusic = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_BGMusic );
		auto pCheckBoxNoMusic = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoMusic );

		if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic != pCheckBoxNoMusic->IsChecked() )
		{
			if ( pCheckBoxNoMusic->IsChecked() == FALSE )
				SOUNDHANDLER->SetVolumeMusic( pScrollBGMusic->GetPercent() );
		}

		SETTINGSHANDLER->GetModel()->GetSettings().bNoMusic = pCheckBoxNoMusic->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().iMusicVolume = pScrollBGMusic->GetPercent();
	}
	// Effect iSound
	{
		auto pScrollEffectSound = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_EffectSound );
		auto pCheckBoxNoEffects = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoEffects );
		SETTINGSHANDLER->GetModel()->GetSettings().bNoSoundEffects = pCheckBoxNoEffects->IsChecked() ? TRUE : FALSE;
		SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume = pScrollEffectSound->GetPercent();
	}

	// General Settings
	{
		UI::InputField_ptr pInputField;

		for ( int i = 0; i < 10; i++ )
		{
			pInputField = pWindowGeneral->GetElement<UI::InputField>( InputFieldID_Macro1 + i );
			STRINGCOPY( SETTINGSHANDLER->GetModel()->GetSettings().szaMacro[i], pInputField->GetText().c_str() );
		}

		SETTINGSHANDLER->GetModel()->GetSettings().bNoWhisper		= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoWhisper )->IsChecked();
		SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeChat		= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_NoTrade )->IsChecked();
		SETTINGSHANDLER->GetModel()->GetSettings().bNoTradeRequest	= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableTrade )->IsChecked();
		SETTINGSHANDLER->GetModel()->GetSettings().bNoPartyRequest	= pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_DisableParty )->IsChecked();
	}

	SETTINGSHANDLER->GetModel()->WriteSettings();
	SETTINGSHANDLER->GetModel()->ProcessGeneralSettings();

	DXGraphicEngine::ResolutionChange( FALSE, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight );
	GRAPHICENGINE->Reset( SETTINGSHANDLER->GetModel()->GetSettings().bWindowed, SETTINGSHANDLER->GetModel()->GetSettings().dwWidth, SETTINGSHANDLER->GetModel()->GetSettings().dwHeight, TRUE );

	HUDHANDLER->UpdateObjectsSettings();
	BLESSCASTLEHANDLER->UpdateObjectSettings();

	DXGraphicEngine::ResizeWindow();
	SETTINGSHANDLER->GetModel()->ReadSettings();
	pWindow1->Hide();

	UNITGAME->UpdateElementalWeaponSettings();

	GRAPHICS->GetRenderer()->SetUseTextureEffects( SETTINGSHANDLER->GetModel()->GetSettings().bNormalMapping ? true : false );

	MOUSEHANDLER->UnblockMouse();
}

void CSettingsWindow::OnMouseMove( class CMouse * pcMouse )
{
	pWindow1->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	if ( (*(BOOL*)(0x035E10D0)) )
		pWindowMenu->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( Game::GetGameMode() == GAMEMODE_Login )
	{
		pWindowLogin->OnMouseMove( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	}
}

BOOL CSettingsWindow::OnMouseClick( class CMouse * pcMouse )
{
	if ( pWindow1->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;
	if ( pWindowMenu->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
		return TRUE;

	if ( Game::GetGameMode() == GAMEMODE_Login )
	{
		if ( pWindowLogin->OnMouseClick( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CSettingsWindow::OnMouseScroll( CMouse * pcMouse )
{
	return pWindow1->OnMouseScroll( pcMouse, 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
}

BOOL CSettingsWindow::OnKeyPress( class CKeyboard * pcKeyboard )
{
	if ( pcKeyboard->GetKey() == VK_F10 )
	{
		if ( !pWindow1->IsOpen() && GameCore::GetInstance()->sServerTime.wDay && pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown )
		{
			OnMenuButtonClick( UIEventArgs{} );
			return TRUE;
		}
		else if ( pWindow1->IsOpen() && pcKeyboard->GetEvent() == EKeyboardEvent::KeyDown )
		{
			pWindow1->Hide();
			MOUSEHANDLER->UnblockMouse();
			*(BOOL*)(0x035E10D0) = FALSE;
		}
	}
	else if ( pWindow1->OnKeyPress( pcKeyboard ) )
		return TRUE;

	return FALSE;
}

BOOL CSettingsWindow::OnKeyChar( class CKeyboard * pcKeyboard )
{
	if ( pWindow1->OnKeyChar( pcKeyboard ) )
		return TRUE;
	return FALSE;
}

void CSettingsWindow::Update( float fTime )
{
	pWindowLogin->Update( fTime );
	pWindow1->Update( fTime );
}

void CSettingsWindow::Render()
{
	pWindowMenu->SetPosition( 698 + 16, GRAPHICENGINE->GetHeightBackBuffer() - 96 - 30 );

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_World )
	{
		if ( (*(BOOL*)(0x035E10D0)) )
			pWindowMenu->Show();
		else
			pWindowMenu->Hide();
	}
	else
		(*(BOOL*)(0x035E10D0)) = FALSE;

	pWindowMenu->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	pWindow1->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );

	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
	{
		pWindowLogin->Render( 0, 0, DX::Graphic::GetWidthBackBuffer(), DX::Graphic::GetHeightBackBuffer(), 0, 0 );
	}
}

void CSettingsWindow::OnResolutionChanged()
{
	pWindow1->SetPosition( ( RESOLUTION_WIDTH >> 1 ) - ( pWindow1->GetWidth() >> 1 ), ( RESOLUTION_HEIGHT >> 1 ) - ( pWindow1->GetHeight() >> 1 ) );
	if ( GAMESCREEN->GetActiveScreenType() == SCREEN_Login )
	{
		pWindowLogin->SetPosition( (RESOLUTION_WIDTH >> 1) - (pWindowLogin->GetWidth() >> 1), 30 );
	}
}

void CSettingsWindow::OnVideoButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Hide();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Show();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Hide();
}

void CSettingsWindow::OnCheckBoxRatio43Click( UIEventArgs eArgs )
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
	pGroupResolution43->SetIndexObject( GetResolutionCheckBox() );
	pGroupRatio->SetIndexObject( CHECKBOXID_Ratio4_3 );
	pGroupResolution169->Hide();
	pGroupResolution43->Show();
}

void CSettingsWindow::OnCheckBoxRatio169Click( UIEventArgs eArgs )
{
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
	auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
	auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
	pGroupResolution169->SetIndexObject( GetResolutionCheckBox() );
	pGroupRatio->SetIndexObject( CHECKBOXID_Ratio16_9 );
	pGroupResolution43->Hide();
	pGroupResolution169->Show();
}

void CSettingsWindow::OnAudioButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Show();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Hide();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Hide();
}

void CSettingsWindow::OnGeneralButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Hide();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Show();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Hide();
}

void CSettingsWindow::OnInterfaceButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	pWindowAudio->Hide();

	auto pWindowVideo = GetWindow( WINDOWID_Video );
	pWindowVideo->Hide();

	auto pWindowGeneral = GetWindow( WINDOWID_General );
	pWindowGeneral->Hide();

	auto pWindowInterface = GetWindow( WINDOWID_Interface );
	pWindowInterface->Show();
}

void CSettingsWindow::OnCloseButtonClick( UIEventArgs eArgs )
{	
	pWindow1->Hide();
	MOUSEHANDLER->UnblockMouse();
}

void CSettingsWindow::OnSaveButtonClick( UIEventArgs eArgs )
{	
	// Show Box to Save
	MESSAGEBOX->SetBoxType( CMessageBox::BOXTYPEID_YesNo );
	MESSAGEBOX->SetTitle( "Save Settings" );
	MESSAGEBOX->SetDescription( "Do you want to save the changes?" );
	MESSAGEBOX->SetEvent( std::bind( &CSettingsWindow::Save, this ) );
	MESSAGEBOX->SetAutoClose( 5 );
	MESSAGEBOX->Show();

	pWindow1->Hide();
}

void CSettingsWindow::OnResetButtonClick( UIEventArgs eArgs )
{
	auto pWindowAudio = GetWindow( WINDOWID_Audio );
	auto pWindowGeneral = GetWindow( WINDOWID_General );
	auto pWindowVideo = GetWindow( WINDOWID_Video );
	auto pWindowInterface = GetWindow( WINDOWID_Interface );

	// Video
	// Screen
	{
		auto pCheckBoxAutoAdjust = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_AutoAdjust );
		pCheckBoxAutoAdjust->SetCheck( TRUE );

		auto pCheckBoxStartMaximized = pWindowVideo->GetElement<UI::CheckBox>( CHECKBOXID_StartMaximized );
		pCheckBoxStartMaximized->SetCheck( TRUE );

		auto pGroupDisplay = pWindowVideo->GetElement<UI::Group>( GROUPID_Display );
		pGroupDisplay->SetIndexObject( CHECKBOXID_Windowed );

		auto pGroupRenderer = pWindowVideo->GetElement<UI::Group>( GROUPID_Renderer );
		pGroupRenderer->SetIndexObject( CHECKBOXID_DirectX9Ex );

		auto pGroupResolution43 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions4_3 );
		auto pGroupRatio = pWindowVideo->GetElement<UI::Group>( GROUPID_Ratio );
		auto pGroupResolution169 = pWindowVideo->GetElement<UI::Group>( GROUPID_Resolutions16_9 );
		pGroupResolution43->SetIndexObject( CHECKBOXID_1024x768 );
		pGroupRatio->SetIndexObject( CHECKBOXID_Ratio4_3 );
		pGroupResolution169->Hide();
		pGroupResolution43->Show();
	}
	// Graphics
	{
		auto pGroupTexture = pWindowVideo->GetElement<UI::Group>( GROUPID_TextureQuality );
		pGroupTexture->SetIndexObject( CHECKBOXID_TextureHigh );

		auto pGroupBitDepth = pWindowVideo->GetElement<UI::Group>( GROUPID_BitDepth );
		pGroupBitDepth->SetIndexObject( CHECKBOXID_32BPP );	

		auto pGroupVSync = pWindowVideo->GetElement<UI::Group>( GROUPID_VSync );
		pGroupVSync->SetIndexObject( CHECKBOXID_VSyncON );

        auto pGroupEffects = pWindowVideo->GetElement<UI::Group>( GROUPID_Effects );
        pGroupEffects->SetIndexObject( CHECKBOXID_EffectsON );

		auto pGroupDynamicShadows = pWindowVideo->GetElement<UI::Group>( GROUPID_DynamicShadows );
		pGroupDynamicShadows->SetIndexObject( CHECKBOXID_DynamicShadowsON );

		auto pGroupNormalMapping = pWindowVideo->GetElement<UI::Group>( GROUPID_NormalMapping );
		pGroupNormalMapping->SetIndexObject( CHECKBOXID_NormalMappingON );

		auto pGroupAntiAliasing = pWindowVideo->GetElement<UI::Group>( GROUPID_AntiAliasing );
		pGroupAntiAliasing->SetIndexObject( CHECKBOXID_AntiAliasingOFF );
	}
	// Camera
	{
		auto pGroupCameraSight = pWindowVideo->GetElement<UI::Group>( GROUPID_CameraSight );
		pGroupCameraSight->SetIndexObject( CHECKBOXID_Far );

		auto pGroupCameraInvert = pWindowVideo->GetElement<UI::Group>( GROUPID_InvertedCamera );
		pGroupCameraInvert->SetIndexObject( CHECKBOXID_CameraOFF );
	}

	//Personalization
	{
		auto pGroupDebugDamage = pWindowGeneral->GetElement<UI::Group>( GROUPID_DamageDebug );
		pGroupDebugDamage->SetIndexObject( CHECKBOXID_DamageDebugON );

		auto pGroupHPBar = pWindowGeneral->GetElement<UI::Group>( GROUPID_HPBar );
		pGroupHPBar->SetIndexObject( CHECKBOXID_HPBarON );

		auto pGroupFPS = pWindowGeneral->GetElement<UI::Group>( GROUPID_FPS );
		pGroupFPS->SetIndexObject( CHECKBOXID_FPSOFF );

		auto pGroupPing = pWindowGeneral->GetElement<UI::Group>( GROUPID_Ping );
		pGroupPing->SetIndexObject( CHECKBOXID_PingOFF );

		auto pGroupKillInfo = pWindowGeneral->GetElement<UI::Group>( GROUPID_KillInfo );
		pGroupKillInfo->SetIndexObject( CHECKBOXID_KillInfoON );

		auto pGroupElement = pWindowGeneral->GetElement<UI::Group>( GROUPID_Element );
		pGroupElement->SetIndexObject( CHECKBOXID_ElementON );

		auto pGroupAgingArmor = pWindowGeneral->GetElement<UI::Group>( GROUPID_AgingArmor );
		pGroupAgingArmor->SetIndexObject( CHECKBOXID_AgingArmorON );

		auto pGroupHideCaravan = pWindowGeneral->GetElement<UI::Group>( GROUPID_HideCaravans );
		pGroupHideCaravan->SetIndexObject( CHECKBOXID_HideCaravanOFF );

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootFilterEnable )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootHPPotion )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMPPotion )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSPPotion )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootGold )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootAmulets )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootRings )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSheltoms )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootForceOrbs )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootPremiums )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootMonsterCrystals )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootDefenseItems )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootOffenseItems )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootEverything )->SetCheck( TRUE );

		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecNS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecFS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecAS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecATA )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecKS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecMGS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecPRS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecASS )->SetCheck( TRUE );
		pWindowGeneral->GetElement<UI::CheckBox>( CHECKBOXID_LootSpecSHA )->SetCheck( TRUE );


	}

	//Interface
	{
		auto pCheckBoxLockUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_LockUI );
		auto pCheckBoxGlow = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_Glow );
		//	auto pCheckBoxSaveUI = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_SaveUI );
		auto pCheckBoxShowParty = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowPartyMembers );
		auto pCheckBoxShowRaid = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowRaidMembers );
		auto pCheckBoxShowClan = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowClanMembers );
		auto pCheckBoxShowEnemies = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowEnemies );
		auto pCheckBoxShowNPCs = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNPCs );
		auto pCheckBoxAlwaysActive = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_AlwaysActive );
		auto pCheckBoxNextBossTimer = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNextBossTimer );
		auto pCheckBoxShowNotice = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_ShowNotice );
		auto pCheckBoxTargetDistance = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetDistance );
		auto pCheckBoxTargetHPValue = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPValue );
		auto pCheckBoxTargetHPPercent = pWindowInterface->GetElement<UI::CheckBox>( CHECKBOXID_TargetHPPercent );


		pCheckBoxLockUI->SetCheck( TRUE );
	//	pCheckBoxSaveUI->SetCheck( TRUE );
		pCheckBoxGlow->SetCheck( TRUE );

		pCheckBoxShowParty->SetCheck( TRUE );
		pCheckBoxShowClan->SetCheck( TRUE );
		pCheckBoxShowEnemies->SetCheck( TRUE );
		pCheckBoxShowNPCs->SetCheck( TRUE );
		pCheckBoxAlwaysActive->SetCheck( TRUE );
		pCheckBoxShowRaid->SetCheck( TRUE );
		pCheckBoxNextBossTimer->SetCheck( TRUE );
		pCheckBoxShowNotice->SetCheck( TRUE );
		pCheckBoxTargetDistance->SetCheck( TRUE );
		pCheckBoxTargetHPValue->SetCheck( TRUE );
		pCheckBoxTargetHPPercent->SetCheck( TRUE );
	}

	//Audio
	// BG Music
	{
		auto pScrollBGMusic = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_BGMusic );
		auto pCheckBoxNoMusic = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoMusic );

		pCheckBoxNoMusic->SetCheck( FALSE );
		pScrollBGMusic->SetPercent( 50 );
	}
	// Effect iSound
	{
		auto pScrollEffectSound = pWindowAudio->GetElement<UI::Scroll>( SCROLLID_EffectSound );
		auto pCheckBoxNoEffects = pWindowAudio->GetElement<UI::CheckBox>( CHECKBOXID_NoEffects );
		
		pCheckBoxNoEffects->SetCheck( FALSE );
		pScrollEffectSound->SetPercent( 50 );
	}
}

void CSettingsWindow::OnMenuButtonClick( UIEventArgs eArgs )
{
	pWindow1->Show();
	MOUSEHANDLER->BlockMouse();
	Load();
	*(BOOL*)(0x035E10D0) = FALSE;
}

void CSettingsWindow::OnLoginButtonClick( UIEventArgs e )
{
	pWindow1->Show();
	MOUSEHANDLER->BlockMouse();
	Load();
}

void CSettingsWindow::Init()
{
	BuildWindow1();
	{
		BuildWindow1Video();
		BuildWindow1Audio();
		BuildWindow1General();
		BuildWindow1Interface();
	}

	pWindow1->Hide();

	// Menu
	{
		pWindowMenu = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 96, 19 ) );

		UI::Button_ptr pButtonSettings = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 96, 19 ) );
		pButtonSettings->SetImage( UI::ImageLoader::LoadImage( "game\\images\\misc\\settings.png" ) );
		pButtonSettings->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\misc\\settings.png", "game\\images\\misc\\settings_.png" ) );
		pButtonSettings->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnMenuButtonClick, this, std::placeholders::_1 ) ) );
		pWindowMenu->AddElement( pButtonSettings );
		pWindowMenu->Hide();
	}
}

void CSettingsWindow::BuildWindow1()
{
	pWindowLogin = std::make_shared<UI::Window>( Rectangle2D( (GRAPHICENGINE->GetWidthBackBuffer() >> 1) - (220 >> 1), 30, 220, 23 ) );
	pWindowLogin->Hide();

	UI::Button_ptr pButtonSettingsLogin = std::make_shared<UI::Button>( Rectangle2D( 0, 0, 220, 23 ) );
	pButtonSettingsLogin->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\settings.png", "game\\images\\settings\\settings_.png" ) );
	pButtonSettingsLogin->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnLoginButtonClick, this, std::placeholders::_1 ) ) );
	//pWindowLogin->AddElement( pButtonSettingsLogin );

	pWindow1 = std::make_shared<UI::Window>( Rectangle2D( ( GRAPHICENGINE->GetWidthBackBuffer() >> 1 ) - ( 560 >> 1 ), ( GRAPHICENGINE->GetHeightBackBuffer() >> 1 ) - ( 450 >> 1 ), 572, 450 ) );
	pWindow1->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\window.png" ) );
	pWindow1->SetID( WINDOWID_Main );

	UI::List_ptr pListMain = std::make_shared<UI::List>( Rectangle2D( 135, 40, 410, 395 ) );
	pListMain->SetID( LISTID_Main );
	pListMain->SetNoUpdatePosition( TRUE );
	pListMain->MakeScrollBar( Rectangle2D( 401, (395 >> 1) - (385 >> 1), 6, 385 ), "game\\images\\settings\\scrollbar.png", "game\\images\\UI\\scroll\\icon_2.png" );
	pListMain->SetScrollFirst( TRUE );
	pWindow1->AddElement( pListMain );

	// Menu Settings
	{
		// Group Menu
		UI::Group_ptr pGroupMenu = std::make_shared<UI::Group>();
		pGroupMenu->SetID( GROUPID_Menu );
		pWindow1->AddElement( pGroupMenu );

		UI::Button_ptr pButtonVideo = std::make_shared<UI::Button>( Rectangle2D( 14, 50, 121, 29 ) );
		pButtonVideo->SetID( BUTTONID_Video );
		pButtonVideo->SetGroup( pGroupMenu );
		pButtonVideo->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\video.png", "game\\images\\settings\\video_.png" ) );
		pButtonVideo->SetSelect( TRUE );
		pButtonVideo->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnVideoButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonVideo );

		UI::Button_ptr pButtonAudio = std::make_shared<UI::Button>( Rectangle2D( 14, 50 + 29, 121, 29 ) );
		pButtonAudio->SetID( BUTTONID_Audio );
		pButtonAudio->SetGroup( pGroupMenu );
		pButtonAudio->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\audio.png", "game\\images\\settings\\audio_.png" ) );
		pButtonAudio->SetSelect( TRUE );
		pButtonAudio->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnAudioButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonAudio );

		UI::Button_ptr pButtonGeneral = std::make_shared<UI::Button>( Rectangle2D( 14, 50 + ( 29 * 2 ), 121, 29 ) );
		pButtonGeneral->SetID( BUTTONID_General );
		pButtonGeneral->SetGroup( pGroupMenu );
		pButtonGeneral->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\general.png", "game\\images\\settings\\general_.png" ) );
		pButtonGeneral->SetSelect( TRUE );
		pButtonGeneral->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnGeneralButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonGeneral );

		UI::Button_ptr pButtonInterface = std::make_shared<UI::Button>( Rectangle2D( 14, 50 + ( 29 * 3 ), 121, 29 ) );
		pButtonInterface->SetID( BUTTONID_Interface );
		pButtonInterface->SetGroup( pGroupMenu );
		pButtonInterface->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\ui.png", "game\\images\\settings\\ui_.png" ) );
		pButtonInterface->SetSelect( TRUE );
		pButtonInterface->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnInterfaceButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonInterface );

		UI::Button_ptr pButtonReset = std::make_shared<UI::Button>( Rectangle2D( 28, 374, 93, 23 ) );
		pButtonReset->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\btnreset.png", "game\\images\\settings\\btnreset_.png" ) );
		pButtonReset->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnResetButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonReset );

		UI::Button_ptr pButtonSave = std::make_shared<UI::Button>( Rectangle2D( 28, 374 + 26, 93, 23 ) );
		pButtonSave->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\btnsave.png", "game\\images\\settings\\btnsave_.png" ) );
		pButtonSave->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnSaveButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonSave );

		UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 536, 4, 32, 47 ) );
		pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\settings\\btnclose.png", "game\\images\\settings\\btnclose_.png" ) );
		pButtonClose->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnCloseButtonClick, this, std::placeholders::_1 ) ) );
		pWindow1->AddElement( pButtonClose );
	}

	// Boss Time and Server time
	{
		UI::Text_ptr pTextBossTimeHeader = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + (26 * 2) + 20, 100, 20 ) );
		pTextBossTimeHeader->SetText( "Boss Time:" );
		pTextBossTimeHeader->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextBossTimeHeader );

		UI::Text_ptr pTextBossTime = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + (26 * 2) + 36, 100, 20 ) );
		pTextBossTime->SetText( "" );
		pTextBossTime->SetID( TextID_BossTime );
		pTextBossTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextBossTime );

		UI::Text_ptr pTextServerTimeHeader = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + (26 * 2) + 56, 100, 20 ) );
		pTextServerTimeHeader->SetText( "Server Time:" );
		pTextServerTimeHeader->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextServerTimeHeader );

		UI::Text_ptr pTextServerTime = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + (26 * 2) + 72, 100, 20 ) );
		pTextServerTime->SetText( "" );
		pTextServerTime->SetMultiLine( TRUE );
		pTextServerTime->SetNoClip( TRUE );
		pTextServerTime->SetID( TextID_ServerTime );
		pTextServerTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextServerTime );

		UI::Text_ptr pTextServerNameHeader = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + (26 * 2) + 72 + 36, 100, 20 ) );
		pTextServerNameHeader->SetText( "Server:" );
		pTextServerNameHeader->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextServerNameHeader );

		UI::Text_ptr pTextServerName = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + (26 * 2) + 88 + 36, 100, 20 ) );
		pTextServerName->SetText( "" );
		pTextServerName->SetID( TextID_ServerName );
		pTextServerName->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextServerName );

		UI::Text_ptr pTextSiegeWar = std::make_shared<UI::Text>( Rectangle2D( 24, 114 + ( 26 * 2 ) + 108 + 36, 100, 20 ) );
		pTextSiegeWar->SetText( "Siege War:" );
		pTextSiegeWar->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pWindow1->AddElement( pTextSiegeWar );

		UI::Text_ptr pTextSiegeWarTime = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + ( 26 * 2 ) + 124 + 36, 100, 20 ) );
		pTextSiegeWarTime->SetText( "" );
		pTextSiegeWarTime->SetID( TEXTID_SiegeWarTime );
		pTextSiegeWarTime->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextSiegeWarTime );

		UI::Text_ptr pTextSiegeWarType = std::make_shared<UI::Text>( Rectangle2D( 28, 114 + ( 26 * 2 ) + 140 + 36, 100, 20 ) );
		pTextSiegeWarType->SetText( "" );
		pTextSiegeWarType->SetID( TEXTID_SiegeWarType );
		pTextSiegeWarType->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
		pWindow1->AddElement( pTextSiegeWarType );
	}

	pWindow1->SetMoveBox( Rectangle2D( 0, 0, pWindow1->GetWidth() - 40, 40 ) );
	pWindow1->SetCanMove( TRUE );
}

void CSettingsWindow::BuildWindow1Video()
{
	UI::Window_ptr pWindowVideo = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 570 ) );
	pWindowVideo->SetID( WINDOWID_Video );

	int iNewBaseX = 24;
	int iNewBaseY = 11;

	// Title Screen
	UI::ImageBox_ptr pScreenBar = std::make_shared<UI::ImageBox>( Rectangle2D( iNewBaseX + 10, iNewBaseY, 339, 23 ) );
	pScreenBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowVideo->AddElement( pScreenBar );

	UI::ImageBox_ptr pScreenTitle = std::make_shared<UI::ImageBox>( Rectangle2D( iNewBaseX + 106, iNewBaseY + 5, 150, 14 ) );
	pScreenTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\screen.png" ) );
	pWindowVideo->AddElement( pScreenTitle );

	iNewBaseY += 31;

	//Display
	iNewBaseY += AddGroup( pWindowVideo, true, true, 38, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_Renderer, "DirectX Version", "", true )
						   .Add( GroupAddObject( CHECKBOXID_DirectX9, "DirectX 9" ) )
						   .Add( GroupAddObject( CHECKBOXID_DirectX9Ex, "DirectX 9Ex" ) )
	);


	//Display
	iNewBaseY += AddGroup( pWindowVideo, true, true, 38, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_Display, "Display", "", true )
						   .Add( GroupAddObject( CHECKBOXID_Fullscreen, "Fullscreen" ) )
						   .Add( GroupAddObject( CHECKBOXID_Windowed, "Windowed" ) )
	);


	UI::CheckBox_ptr pCheckBoxStartMaximized = std::make_shared<UI::CheckBox>( Rectangle2D( 270, 85, 13, 13 ) );
	pCheckBoxStartMaximized->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxStartMaximized->SetText( "Start Maximized" );
	pCheckBoxStartMaximized->SetID( CHECKBOXID_StartMaximized );
	pCheckBoxStartMaximized->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxStartMaximized );

	UI::CheckBox_ptr pCheckBoxAutoAdjust = std::make_shared<UI::CheckBox>( Rectangle2D( 270, 67, 13, 13 ) );
	pCheckBoxAutoAdjust->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAutoAdjust->SetText( "Auto Adjust" );
	pCheckBoxAutoAdjust->SetID( CHECKBOXID_AutoAdjust );
	pCheckBoxAutoAdjust->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBoxAutoAdjust );

	// Group Ratio
	UI::Group_ptr pGroupRatio = std::make_shared<UI::Group>();
	pGroupRatio->SetID( GROUPID_Ratio );
	pWindowVideo->AddElement( pGroupRatio );

	// Text Ratio
	UI::Text_ptr pTextRatio = std::make_shared<UI::Text>( Rectangle2D( iNewBaseX, iNewBaseY, 0, 0 ) );
	pTextRatio->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextRatio->SetText( "Ratio:" );
	pWindowVideo->AddElement( pTextRatio );

	UI::CheckBox_ptr pCheckBox4_3 = std::make_shared<UI::CheckBox>( Rectangle2D( iNewBaseX, iNewBaseY + 20, 13, 13 ) );
	pCheckBox4_3->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox4_3->SetText( "4:3" );
	pCheckBox4_3->SetGroup( pGroupRatio );
	pCheckBox4_3->SetID( CHECKBOXID_Ratio4_3 );
	pCheckBox4_3->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnCheckBoxRatio43Click, this, std::placeholders::_1 ) ) );
	pCheckBox4_3->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox4_3 );

	UI::CheckBox_ptr pCheckBox16_9 = std::make_shared<UI::CheckBox>( Rectangle2D( iNewBaseX + 126, iNewBaseY + 20, 13, 13 ) );
	pCheckBox16_9->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBox16_9->SetText( "16:9" );
	pCheckBox16_9->SetGroup( pGroupRatio );
	pCheckBox16_9->SetID( CHECKBOXID_Ratio16_9 );
	pCheckBox16_9->SetEvent( UI::Event::Build( std::bind( &CSettingsWindow::OnCheckBoxRatio169Click, this, std::placeholders::_1 ) ) );
	pCheckBox16_9->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowVideo->AddElement( pCheckBox16_9 );

	iNewBaseY += 40;

	//Resolutions
	int iResY1 = AddGroup( pWindowVideo, true, true, 38, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_Resolutions4_3, "Resolutions", "", true )
						   .Add( GroupAddObject( CHECKBOXID_1024x768, "1024x768" ) )
						   .Add( GroupAddObject( CHECKBOXID_1280x960, "1280x960" ) )
						   .Add( GroupAddObject( CHECKBOXID_1400x1050, "1400x1050" ) )
	);

	int iResY2 = AddGroup( pWindowVideo, true, true, 38, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_Resolutions16_9, "", "", true )
						   .Add( GroupAddObject( CHECKBOXID_1280x720, "1280x720" ) )
						   .Add( GroupAddObject( CHECKBOXID_1366x768, "1280x960" ) )
						   .Add( GroupAddObject( CHECKBOXID_1600x900, "1600x900" ) )
						   .Add( GroupAddObject( CHECKBOXID_1920x1080, "1920x1080" ) )
	);

	iNewBaseY += (iResY1 > iResY2) ? iResY1 : iResY2;

	iNewBaseY += 10;

	// Title Graphics
	UI::ImageBox_ptr pGraphicsBar = std::make_shared<UI::ImageBox>( Rectangle2D( iNewBaseX + 10, iNewBaseY, 339, 23 ) );
	pGraphicsBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowVideo->AddElement( pGraphicsBar );

	UI::ImageBox_ptr pGraphicsTitle = std::make_shared<UI::ImageBox>( Rectangle2D( iNewBaseX + 106, iNewBaseY + 5, 150, 14 ) );
	pGraphicsTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\graphics.png" ) );
	pWindowVideo->AddElement( pGraphicsTitle );

	iNewBaseY += 31;

	//Texture Quality
	iNewBaseY += AddGroup( pWindowVideo, true, false, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_TextureQuality, "Texture Quality", "", true )
						   .Add( GroupAddObject( CHECKBOXID_TextureLow, "Low" ) )
						   .Add( GroupAddObject( CHECKBOXID_TextureHigh, "High" ) )
	);

	//Anti Aliasing
	iNewBaseY += AddGroup( pWindowVideo, false, true, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_AntiAliasing, "Anti Aliasing", "", true )
						   .Add( GroupAddObject( CHECKBOXID_AntiAliasingON, "ON" ) )
						   .Add( GroupAddObject( CHECKBOXID_AntiAliasingOFF, "OFF" ) )
	);

	//V-Sync
	iNewBaseY += AddGroup( pWindowVideo, true, false, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_VSync, "V-Sync", "", true )
						   .Add( GroupAddObject( CHECKBOXID_VSyncON, "ON" ) )
						   .Add( GroupAddObject( CHECKBOXID_VSyncOFF, "OFF" ) )
	);


	//Bit Depth
	iNewBaseY += AddGroup( pWindowVideo, false, true, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_BitDepth, "Bit Depth", "", true )
						   .Add( GroupAddObject( CHECKBOXID_16BPP, "16 bit" ) )
						   .Add( GroupAddObject( CHECKBOXID_32BPP, "32 bit" ) )
	);

	//Effects
	iNewBaseY += AddGroup( pWindowVideo, true, false, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_Effects, "Effects", "", true )
						   .Add( GroupAddObject( CHECKBOXID_EffectsON, "ON" ) )
						   .Add( GroupAddObject( CHECKBOXID_EffectsOFF, "OFF" ) )
	);


	//Dynamic Shadows
	iNewBaseY += AddGroup( pWindowVideo, false, true, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_DynamicShadows, "Dynamic Shadows", "", true )
						   .Add( GroupAddObject( CHECKBOXID_DynamicShadowsON, "ON" ) )
						   .Add( GroupAddObject( CHECKBOXID_DynamicShadowsOFF, "OFF" ) )
	);

	//Texture Effects
	iNewBaseY += AddGroup( pWindowVideo, true, true, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_NormalMapping, "Texture Effects", "", true )
						   .Add( GroupAddObject( CHECKBOXID_NormalMappingON, "ON" ) )
						   .Add( GroupAddObject( CHECKBOXID_NormalMappingOFF, "OFF" ) )
	);

	iNewBaseY += 10;

	// Title Camera
	UI::ImageBox_ptr pCameraBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, iNewBaseY, 339, 23 ) );
	pCameraBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowVideo->AddElement( pCameraBar );

	UI::ImageBox_ptr pCameraTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, iNewBaseY + 5, 150, 14 ) );
	pCameraTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\camera.png" ) );
	pWindowVideo->AddElement( pCameraTitle );

	iNewBaseY += 31;

	//Inverted Camera
	iNewBaseY += AddGroup( pWindowVideo, false, false, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_InvertedCamera, "Inverted Camera", "", true )
						   .Add( GroupAddObject( CHECKBOXID_CameraON, "ON" ) )
						   .Add( GroupAddObject( CHECKBOXID_CameraOFF, "OFF" ) )
	);

	//Camera Range
	iNewBaseY += AddGroup( pWindowVideo, true, true, 0, iNewBaseX, iNewBaseY, GroupAdd( GROUPID_CameraSight, "Camera Range", "", true )
						   .Add( GroupAddObject( CHECKBOXID_Full, "Full" ) )
						   .Add( GroupAddObject( CHECKBOXID_Far, "Far" ) )
						   .Add( GroupAddObject( CHECKBOXID_Short, "Short" ) )
	);

	GetListMain()->AddWindow( pWindowVideo );
}

void CSettingsWindow::BuildWindow1Audio()
{
	UI::Window_ptr pWindowAudio = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 395 ) );
	pWindowAudio->SetID( WINDOWID_Audio );

	// Title Background Music
	UI::ImageBox_ptr pBackgroundMusicBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pBackgroundMusicBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowAudio->AddElement( pBackgroundMusicBar );

	UI::ImageBox_ptr pBackgroundMusicTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pBackgroundMusicTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgmusic.png" ) );
	pWindowAudio->AddElement( pBackgroundMusicTitle );

	// Text Background Music
	UI::Text_ptr pTextBackgroundMusic = std::make_shared<UI::Text>( Rectangle2D( 24, 47, 0, 0 ) );
	pTextBackgroundMusic->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextBackgroundMusic->SetText( "Background Music:" );
	pWindowAudio->AddElement( pTextBackgroundMusic );

	// Background Controller
	UI::ImageBox_ptr pBackgroundControllerMusic = std::make_shared<UI::ImageBox>( Rectangle2D( 13, 70, 386, 56 ) );
	pBackgroundControllerMusic->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgcontroller.png" ) );
	pWindowAudio->AddElement( pBackgroundControllerMusic );

	// Scroller Background
	UI::Scroll_ptr pScrollBackgroundMusic = std::make_shared<UI::Scroll>( Rectangle2D( 110, 95, 265, 8 ) );
	pScrollBackgroundMusic->SetVertical( FALSE );
	pScrollBackgroundMusic->SetMinMax( 0, 100 );
	pScrollBackgroundMusic->SetID( SCROLLID_BGMusic );
	pScrollBackgroundMusic->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\barbg.png" ) );
	pScrollBackgroundMusic->SetImageBackground( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\bar.png" ) );
	pScrollBackgroundMusic->SetScroller( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\icon.png" ), 20, 20 );
	pWindowAudio->AddElement( pScrollBackgroundMusic );

	// Checkbox No iSound
	UI::CheckBox_ptr pCheckBoxNoMusic = std::make_shared<UI::CheckBox>( Rectangle2D( 23, 93, 13, 13 ) );
	pCheckBoxNoMusic->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoMusic->SetText( "No Music" );
	pCheckBoxNoMusic->SetID( CHECKBOXID_NoMusic );
	pCheckBoxNoMusic->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowAudio->AddElement( pCheckBoxNoMusic );

	// Title iSound Effects
	UI::ImageBox_ptr pSoundEffectsBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 145, 339, 23 ) );
	pSoundEffectsBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowAudio->AddElement( pSoundEffectsBar );

	UI::ImageBox_ptr pSoundEffectsTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 150, 150, 14 ) );
	pSoundEffectsTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgsound.png" ) );
	pWindowAudio->AddElement( pSoundEffectsTitle );

	// Text iSound Effects
	UI::Text_ptr pTextSoundEffects = std::make_shared<UI::Text>( Rectangle2D( 24, 181, 0, 0 ) );
	pTextSoundEffects->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextSoundEffects->SetText( "Sound Effects:" );
	pWindowAudio->AddElement( pTextSoundEffects );

	// Background Controller
	UI::ImageBox_ptr pBackgroundControllerSound = std::make_shared<UI::ImageBox>( Rectangle2D( 13, 204, 386, 56 ) );
	pBackgroundControllerSound->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\bgcontroller.png" ) );
	pWindowAudio->AddElement( pBackgroundControllerSound );

	// Scroller Background
	UI::Scroll_ptr pScrollEffectSound = std::make_shared<UI::Scroll>( Rectangle2D( 110, 230, 265, 8 ) );
	pScrollEffectSound->SetVertical( FALSE );
	pScrollEffectSound->SetMinMax( 0, 100 );
	pScrollEffectSound->SetID( SCROLLID_EffectSound );
	pScrollEffectSound->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\barbg.png" ) );
	pScrollEffectSound->SetImageBackground( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\bar.png" ) );
	pScrollEffectSound->SetScroller( UI::ImageLoader::LoadImage( "game\\images\\UI\\scroll\\icon.png" ), 20, 20 );
	pScrollEffectSound->SetPercent( 20 );
	pWindowAudio->AddElement( pScrollEffectSound );

	// Checkbox No Effects
	UI::CheckBox_ptr pCheckBoxNoEffects = std::make_shared<UI::CheckBox>( Rectangle2D( 23, 228, 13, 13 ) );
	pCheckBoxNoEffects->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoEffects->SetText( "No Effects" );
	pCheckBoxNoEffects->SetID( CHECKBOXID_NoEffects );
	pCheckBoxNoEffects->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowAudio->AddElement( pCheckBoxNoEffects );


	pWindowAudio->Hide();

	GetListMain()->AddWindow( pWindowAudio );
}

void CSettingsWindow::BuildWindow1General()
{
	int iY = 0;

	UI::Window_ptr pWindowGeneral = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 890 ) );
	pWindowGeneral->SetID( WINDOWID_General );

	// Title Social
	UI::ImageBox_ptr pSocialBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pSocialBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowGeneral->AddElement( pSocialBar );

	UI::ImageBox_ptr pSocialTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pSocialTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\social.png" ) );
	pWindowGeneral->AddElement( pSocialTitle );

	// Disable Party Requests
	UI::CheckBox_ptr pCheckBoxDisableParty = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 47, 13, 13 ) );
	pCheckBoxDisableParty->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDisableParty->SetText( "Disable Party Requests" );
	pCheckBoxDisableParty->SetID( CHECKBOXID_DisableParty );
	pCheckBoxDisableParty->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDisableParty );

	// Disable Trade Requests
	UI::CheckBox_ptr pCheckBoxDisableTrade = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 67, 13, 13 ) );
	pCheckBoxDisableTrade->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDisableTrade->SetText( "Disable Trade Requests" );
	pCheckBoxDisableTrade->SetID( CHECKBOXID_DisableTrade );
	pCheckBoxDisableTrade->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDisableTrade );

	// No Whisper
	UI::CheckBox_ptr pCheckBoxNoWhisper = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 87, 13, 13 ) );
	pCheckBoxNoWhisper->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoWhisper->SetText( "No Whisper" );
	pCheckBoxNoWhisper->SetID( CHECKBOXID_NoWhisper );
	pCheckBoxNoWhisper->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxNoWhisper );

	// No Trade chat in All
	UI::CheckBox_ptr pCheckBoxNoTrade = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 107, 13, 13 ) );
	pCheckBoxNoTrade->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxNoTrade->SetText( "No Trade Chat in All" );
	pCheckBoxNoTrade->SetID( CHECKBOXID_NoTrade );
	pCheckBoxNoTrade->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxNoTrade );

	// Title Keyboard
	UI::ImageBox_ptr pPersonalizationBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 130 + iY, 339, 23 ) );
	pPersonalizationBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowGeneral->AddElement( pPersonalizationBar );

	UI::ImageBox_ptr pPersonalizationTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 135 + iY, 150, 14 ) );
	pPersonalizationTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\personalization.png" ) );
	pWindowGeneral->AddElement( pPersonalizationTitle );

	// Text
	UI::Text_ptr pTextDamageDebug = std::make_shared<UI::Text>( Rectangle2D( 24, 167, 0, 0 ) );
	pTextDamageDebug->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextDamageDebug->SetText( "Damage Debug:" );
	pWindowGeneral->AddElement( pTextDamageDebug );

	// Group
	UI::Group_ptr pGroupDamageDebug = std::make_shared<UI::Group>();
	pGroupDamageDebug->SetID( GROUPID_DamageDebug );
	pWindowGeneral->AddElement( pGroupDamageDebug );

	UI::CheckBox_ptr pCheckBoxDamageDebugON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 187, 13, 13 ) );
	pCheckBoxDamageDebugON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDamageDebugON->SetText( "ON" );
	pCheckBoxDamageDebugON->SetGroup( pGroupDamageDebug );
	pCheckBoxDamageDebugON->SetID( CHECKBOXID_DamageDebugON );
	pCheckBoxDamageDebugON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDamageDebugON );

	UI::CheckBox_ptr pCheckBoxDamageDebugOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 187, 13, 13 ) );
	pCheckBoxDamageDebugOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxDamageDebugOFF->SetText( "OFF" );
	pCheckBoxDamageDebugOFF->SetGroup( pGroupDamageDebug );
	pCheckBoxDamageDebugOFF->SetID( CHECKBOXID_DamageDebugOFF );
	pCheckBoxDamageDebugOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxDamageDebugOFF );


	// Text
	UI::Text_ptr pTextHPBar = std::make_shared<UI::Text>( Rectangle2D( 204, 167, 0, 0 ) );
	pTextHPBar->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextHPBar->SetText( "HP Bar on Head:" );
	pWindowGeneral->AddElement( pTextHPBar );

	// Group
	UI::Group_ptr pGroupHPBar = std::make_shared<UI::Group>();
	pGroupHPBar->SetID( GROUPID_HPBar );
	pWindowGeneral->AddElement( pGroupHPBar );

	UI::CheckBox_ptr pCheckBoxHPBarON = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 187, 13, 13 ) );
	pCheckBoxHPBarON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHPBarON->SetText( "ON" );
	pCheckBoxHPBarON->SetGroup( pGroupHPBar );
	pCheckBoxHPBarON->SetID( CHECKBOXID_HPBarON );
	pCheckBoxHPBarON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHPBarON );

	UI::CheckBox_ptr pCheckBoxHPBarOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 187, 13, 13 ) );
	pCheckBoxHPBarOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHPBarOFF->SetText( "OFF" );
	pCheckBoxHPBarOFF->SetGroup( pGroupHPBar );
	pCheckBoxHPBarOFF->SetID( CHECKBOXID_HPBarOFF );
	pCheckBoxHPBarOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHPBarOFF );

	// Text
	UI::Text_ptr pTextFPS = std::make_shared<UI::Text>( Rectangle2D( 24, 207, 0, 0 ) );
	pTextFPS->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextFPS->SetText( "FPS:" );
	pWindowGeneral->AddElement( pTextFPS );

	// Group
	UI::Group_ptr pGroupFPS = std::make_shared<UI::Group>();
	pGroupFPS->SetID( GROUPID_FPS );
	pWindowGeneral->AddElement( pGroupFPS );

	UI::CheckBox_ptr pCheckBoxFPSON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 227, 13, 13 ) );
	pCheckBoxFPSON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFPSON->SetText( "ON" );
	pCheckBoxFPSON->SetGroup( pGroupFPS );
	pCheckBoxFPSON->SetID( CHECKBOXID_FPSON );
	pCheckBoxFPSON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxFPSON );

	UI::CheckBox_ptr pCheckBoxFPSOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 227, 13, 13 ) );
	pCheckBoxFPSOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxFPSOFF->SetText( "OFF" );
	pCheckBoxFPSOFF->SetGroup( pGroupFPS );
	pCheckBoxFPSOFF->SetID( CHECKBOXID_FPSOFF );
	pCheckBoxFPSOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxFPSOFF );


	// Text
	UI::Text_ptr pTextPing = std::make_shared<UI::Text>( Rectangle2D( 204, 207, 0, 0 ) );
	pTextPing->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextPing->SetText( "Ping:" );
	pWindowGeneral->AddElement( pTextPing );

	// Group
	UI::Group_ptr pGroupPing = std::make_shared<UI::Group>();
	pGroupPing->SetID( GROUPID_Ping );
	pWindowGeneral->AddElement( pGroupPing );

	UI::CheckBox_ptr pCheckBoxPingON = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 227, 13, 13 ) );
	pCheckBoxPingON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxPingON->SetText( "ON" );
	pCheckBoxPingON->SetGroup( pGroupPing );
	pCheckBoxPingON->SetID( CHECKBOXID_PingON );
	pCheckBoxPingON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxPingON );

	UI::CheckBox_ptr pCheckBoxPingOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 227, 13, 13 ) );
	pCheckBoxPingOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxPingOFF->SetText( "OFF" );
	pCheckBoxPingOFF->SetGroup( pGroupPing );
	pCheckBoxPingOFF->SetID( CHECKBOXID_PingOFF );
	pCheckBoxPingOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxPingOFF );

	// Text
	UI::Text_ptr pTextKillInfo = std::make_shared<UI::Text>( Rectangle2D( 24, 247, 0, 0 ) );
	pTextKillInfo->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextKillInfo->SetText( "Kill Debug:" );
	pWindowGeneral->AddElement( pTextKillInfo );

	// Group
	UI::Group_ptr pGroupKillInfo = std::make_shared<UI::Group>();
	pGroupKillInfo->SetID( GROUPID_KillInfo );
	pWindowGeneral->AddElement( pGroupKillInfo );

	UI::CheckBox_ptr pCheckBoxKillInfoON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 267, 13, 13 ) );
	pCheckBoxKillInfoON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxKillInfoON->SetText( "ON" );
	pCheckBoxKillInfoON->SetGroup( pGroupKillInfo );
	pCheckBoxKillInfoON->SetID( CHECKBOXID_KillInfoON );
	pCheckBoxKillInfoON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxKillInfoON );

	UI::CheckBox_ptr pCheckBoxKillInfoOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 267, 13, 13 ) );
	pCheckBoxKillInfoOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxKillInfoOFF->SetText( "OFF" );
	pCheckBoxKillInfoOFF->SetGroup( pGroupKillInfo );
	pCheckBoxKillInfoOFF->SetID( CHECKBOXID_KillInfoOFF );
	pCheckBoxKillInfoOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxKillInfoOFF );

	// Text
	UI::Text_ptr pTextElement = std::make_shared<UI::Text>( Rectangle2D( 204, 247, 0, 0 ) );
	pTextElement->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextElement->SetText( "Elemental Effects:" );
	pWindowGeneral->AddElement( pTextElement );

	// Group
	UI::Group_ptr pGroupElement = std::make_shared<UI::Group>();
	pGroupElement->SetID( GROUPID_Element );
	pWindowGeneral->AddElement( pGroupElement );

	UI::CheckBox_ptr pCheckBoxElementON = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 267, 13, 13 ) );
	pCheckBoxElementON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxElementON->SetText( "ON" );
	pCheckBoxElementON->SetGroup( pGroupElement );
	pCheckBoxElementON->SetID( CHECKBOXID_ElementON );
	pCheckBoxElementON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxElementON );

	UI::CheckBox_ptr pCheckBoxElementOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 267, 13, 13 ) );
	pCheckBoxElementOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxElementOFF->SetText( "OFF" );
	pCheckBoxElementOFF->SetGroup( pGroupElement );
	pCheckBoxElementOFF->SetID( CHECKBOXID_ElementOFF );
	pCheckBoxElementOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxElementOFF );

	// Text
	UI::Text_ptr pTextAgingArmor = std::make_shared<UI::Text>( Rectangle2D( 24, 287, 0, 0 ) );
	pTextAgingArmor->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextAgingArmor->SetText( "Aging Armor Effects:" );
	pWindowGeneral->AddElement( pTextAgingArmor );

	// Group
	UI::Group_ptr pGroupAgingArmor = std::make_shared<UI::Group>();
	pGroupAgingArmor->SetID( GROUPID_AgingArmor );
	pWindowGeneral->AddElement( pGroupAgingArmor );

	UI::CheckBox_ptr pCheckBoxAgingArmorON = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 307, 13, 13 ) );
	pCheckBoxAgingArmorON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAgingArmorON->SetText( "ON" );
	pCheckBoxAgingArmorON->SetGroup( pGroupAgingArmor );
	pCheckBoxAgingArmorON->SetID( CHECKBOXID_AgingArmorON );
	pCheckBoxAgingArmorON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxAgingArmorON );

	UI::CheckBox_ptr pCheckBoxAgingArmorOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 112, 307, 13, 13 ) );
	pCheckBoxAgingArmorOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxAgingArmorOFF->SetText( "OFF" );
	pCheckBoxAgingArmorOFF->SetGroup( pGroupAgingArmor );
	pCheckBoxAgingArmorOFF->SetID( CHECKBOXID_AgingArmorOFF );
	pCheckBoxAgingArmorOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxAgingArmorOFF );

	// Text
	UI::Text_ptr pTextHideCaravans = std::make_shared<UI::Text>( Rectangle2D( 204, 287, 0, 16 ) );
	pTextHideCaravans->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextHideCaravans->SetText( "Hide Caravans:" );
	pTextHideCaravans->SetWidth( pTextHideCaravans->GetTextWidth() + 8 );
	pWindowGeneral->AddElement( pTextHideCaravans );

	// Group
	UI::Group_ptr pGroupHideCaravan = std::make_shared<UI::Group>();
	pGroupHideCaravan->SetID( GROUPID_HideCaravans );
	pWindowGeneral->AddElement( pGroupHideCaravan );

	UI::CheckBox_ptr pCheckBoxHideCaravanON = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 307, 13, 13 ) );
	pCheckBoxHideCaravanON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHideCaravanON->SetText( "ON" );
	pCheckBoxHideCaravanON->SetGroup( pGroupHideCaravan );
	pCheckBoxHideCaravanON->SetID( CHECKBOXID_HideCaravanON );
	pCheckBoxHideCaravanON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHideCaravanON );

	UI::CheckBox_ptr pCheckBoxHideCaravanOFF = std::make_shared<UI::CheckBox>( Rectangle2D( 292, 307, 13, 13 ) );
	pCheckBoxHideCaravanOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHideCaravanOFF->SetText( "OFF" );
	pCheckBoxHideCaravanOFF->SetGroup( pGroupHideCaravan );
	pCheckBoxHideCaravanOFF->SetID( CHECKBOXID_HideCaravanOFF );
	pCheckBoxHideCaravanOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowGeneral->AddElement( pCheckBoxHideCaravanOFF );


	UI::Tooltip_ptr pTooltipHideCaravans = std::make_shared<UI::Tooltip>( Rectangle2D() );
	pTooltipHideCaravans->Init();
	pTooltipHideCaravans->SetLinkObject( pTextHideCaravans );
	pTooltipHideCaravans->SetAddAxis( -86, -20 );
	pTooltipHideCaravans->SetFont( "Arial", 16, -1 );
	pTooltipHideCaravans->SetPosition( TOOLTIPPOSITION_Left );
	pTooltipHideCaravans->SetFollowMouse( FALSE );
	pTooltipHideCaravans->SetText( "Hide caravans that aren't yours (except in cities)" );
	pWindowGeneral->AddElement( pTooltipHideCaravans );

	iY += 336;

	// Text
	UI::Text_ptr pTextLootFilter = std::make_shared<UI::Text>( Rectangle2D( 24, iY, 0, 0 ) );
	pTextLootFilter->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextLootFilter->SetText( "Loot Filter:" );
	pWindowGeneral->AddElement( pTextLootFilter );
	iY += 20;

	//Loot
	{
		UI::CheckBox_ptr pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, TRUE, -1 );
		pCheckBoxLoot->SetText( "Enable Loot Filter" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootFilterEnable );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 30;

		int iYLootSpec = iY;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "HP Potion" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootHPPotion );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "MP Potion" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootMPPotion );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "SP Potion" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSPPotion );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Gold" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootGold );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Amulets" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootAmulets );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Rings" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootRings );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Sheltoms" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSheltoms );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Force Orbs" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootForceOrbs );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Premiums" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootPremiums );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Monster Crystals" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootMonsterCrystals );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Defense Items" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootDefenseItems );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Offense Items" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootOffenseItems );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 24, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Everything Else" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootEverything );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		int iOldY = iY;

		//Spec Loot
		iY = iYLootSpec;
		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Non Spec" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecNS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Fighter" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecFS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Mechanician" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecMS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Archer" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecAS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Pikeman" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecPS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Atalanta" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecATA );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Knight" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecKS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Magician" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecMGS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Priestess" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecPRS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Assassin" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecASS );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		pCheckBoxLoot = std::make_shared<UI::CheckBox>( Rectangle2D( 204, iY, 13, 13 ) );
		pCheckBoxLoot->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxLoot->SetText( "Shaman" );
		pCheckBoxLoot->SetID( CHECKBOXID_LootSpecSHA );
		pCheckBoxLoot->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowGeneral->AddElement( pCheckBoxLoot );
		iY += 20;

		iY = iOldY;
	}

	// Title Keyboard
	UI::ImageBox_ptr pKeyboardBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, iY, 339, 23 ) );
	pKeyboardBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowGeneral->AddElement( pKeyboardBar );

	UI::ImageBox_ptr pKeyboardTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, iY + 5, 150, 14 ) );
	pKeyboardTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\keyboard.png" ) );
	pWindowGeneral->AddElement( pKeyboardTitle );

	iY += 36;

	// Macro
	{
		// Group 
		UI::Group_ptr pGroupMacro = std::make_shared<UI::Group>();
		pGroupMacro->SetID( GROUPID_Macro );
		pWindowGeneral->AddElement( pGroupMacro );

		char szBuf[32] = { 0 };
		for ( int i = 0; i < 10; i++ )
		{
			// Text
			UI::Text_ptr pTextMacro = std::make_shared<UI::Text>( Rectangle2D( 24, (i * 20) + iY, 20, 16 ) );
			STRINGFORMAT( szBuf, "CTRL+F%d", i + 1 );
			pTextMacro->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
			pTextMacro->SetText( szBuf );
			pWindowGeneral->AddElement( pTextMacro );

			// InputField
			UI::InputField_ptr pInputField = std::make_shared<UI::InputField>( Rectangle2D( 88, (i * 20) + iY, 290, 18 ) );
			pInputField->SetID( InputFieldID_Macro1 + i );
			pInputField->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\textbox.png" ) );
			pInputField->SetFont( "Arial", 16, 0, FALSE, FALSE, -1 );
			pInputField->SetText( "" );
			pInputField->SetMaxLength( 32 );
			pInputField->SetSelect( TRUE );
			pInputField->SetGroup( pGroupMacro );
			pWindowGeneral->AddElement( pInputField );
		}
	}

	pWindowGeneral->Hide();

	GetListMain()->AddWindow( pWindowGeneral );
}

void CSettingsWindow::BuildWindow1Interface()
{
	int iY = 0;

	UI::Window_ptr pWindowInterface = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 400, 500 ) );
	pWindowInterface->SetID( WINDOWID_Interface );

	//Title General
	UI::ImageBox_ptr pGeneralBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 11, 339, 23 ) );
	pGeneralBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pGeneralBar );

	UI::ImageBox_ptr pGeneralTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 16, 150, 14 ) );
	pGeneralTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\generalt.png" ) );
	pWindowInterface->AddElement( pGeneralTitle );

	//Lock User Interface
	UI::CheckBox_ptr pCheckBoxLockUI = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 48, 13, 13 ) );
	pCheckBoxLockUI->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxLockUI->SetText( "Lock User Interface" );
	pCheckBoxLockUI->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxLockUI->SetID( CHECKBOXID_LockUI );
	pWindowInterface->AddElement( pCheckBoxLockUI );

	//Lock User Interface
	UI::CheckBox_ptr pCheckBoxGlow = std::make_shared<UI::CheckBox>( Rectangle2D( 204, 48, 13, 13 ) );
	pCheckBoxGlow->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxGlow->SetText( "Show Glow Effect" );
	pCheckBoxGlow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxGlow->SetID( CHECKBOXID_Glow );
	pWindowInterface->AddElement( pCheckBoxGlow );

	//Title Minimap
	UI::ImageBox_ptr pMinimapBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 72, 339, 23 ) );
	pMinimapBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pMinimapBar );

	UI::ImageBox_ptr pMinimapTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 77, 150, 14 ) );
	pMinimapTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\minimap.png" ) );
	pWindowInterface->AddElement( pMinimapTitle );

	//Show Party Members
	UI::CheckBox_ptr pCheckBoxShowParty = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 112, 13, 13 ) );
	pCheckBoxShowParty->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowParty->SetText( "Show Party Members" );
	pCheckBoxShowParty->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowParty->SetID( CHECKBOXID_ShowPartyMembers );
	pWindowInterface->AddElement( pCheckBoxShowParty );

	//Show Party Members
	UI::CheckBox_ptr pCheckBoxShowRaid = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 132, 13, 13 ) );
	pCheckBoxShowRaid->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowRaid->SetText( "Show Raid Members" );
	pCheckBoxShowRaid->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowRaid->SetID( CHECKBOXID_ShowRaidMembers );
	pWindowInterface->AddElement( pCheckBoxShowRaid );

	//Show Clan Members
	UI::CheckBox_ptr pCheckBoxShowClan = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 152, 13, 13 ) );
	pCheckBoxShowClan->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowClan->SetText( "Show Clan Members" );
	pCheckBoxShowClan->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowClan->SetID( CHECKBOXID_ShowClanMembers );
	pWindowInterface->AddElement( pCheckBoxShowClan );

	//Show Enemies
	UI::CheckBox_ptr pCheckBoxShowEnemies = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 172, 13, 13 ) );
	pCheckBoxShowEnemies->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowEnemies->SetText( "Show Enemies at Bless Castle" );
	pCheckBoxShowEnemies->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowEnemies->SetID( CHECKBOXID_ShowEnemies );
	pWindowInterface->AddElement( pCheckBoxShowEnemies );

	//Show NPCs
	UI::CheckBox_ptr pCheckBoxShowNPCs = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 192, 13, 13 ) );
	pCheckBoxShowNPCs->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowNPCs->SetText( "Show NPCs" );
	pCheckBoxShowNPCs->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowNPCs->SetID( CHECKBOXID_ShowNPCs );
	pWindowInterface->AddElement( pCheckBoxShowNPCs );

	//Show Next Boss Timer
	UI::CheckBox_ptr pCheckBoxShowBossTimer = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 212, 13, 13 ) );
	pCheckBoxShowBossTimer->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxShowBossTimer->SetText( "Show Next Boss Timer" );
	pCheckBoxShowBossTimer->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxShowBossTimer->SetID( CHECKBOXID_ShowNextBossTimer );
	pWindowInterface->AddElement( pCheckBoxShowBossTimer );

	//Minimap Active
	UI::CheckBox_ptr pCheckBoxMinimapActive = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 232, 13, 13 ) );
	pCheckBoxMinimapActive->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxMinimapActive->SetText( "Always Active" );
	pCheckBoxMinimapActive->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxMinimapActive->SetID( CHECKBOXID_AlwaysActive );
	pWindowInterface->AddElement( pCheckBoxMinimapActive );

	//Title Chat Box
	UI::ImageBox_ptr pChatboxBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 260, 339, 23 ) );
	pChatboxBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pChatboxBar );

	UI::ImageBox_ptr pChatboxTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 265, 150, 14 ) );
	pChatboxTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\chatbox.png" ) );
	pWindowInterface->AddElement( pChatboxTitle );

	//Hide Notices
	UI::CheckBox_ptr pCheckBoxHideNotices = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 300, 13, 13 ) );
	pCheckBoxHideNotices->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxHideNotices->SetText( "Show 'Notice' info in Chatbox" );
	pCheckBoxHideNotices->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxHideNotices->SetID( CHECKBOXID_ShowNotice );
	pWindowInterface->AddElement( pCheckBoxHideNotices );

	//Title Target
	UI::ImageBox_ptr pTargetBar = std::make_shared<UI::ImageBox>( Rectangle2D( 34, 324, 339, 23 ) );
	pTargetBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowInterface->AddElement( pTargetBar );

	UI::ImageBox_ptr pTargetTitle = std::make_shared<UI::ImageBox>( Rectangle2D( 130, 329, 150, 14 ) );
	pTargetTitle->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\target.png" ) );
	pWindowInterface->AddElement( pTargetTitle );

	//Target Distance
	UI::CheckBox_ptr pCheckBoxTargetDistance = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 358, 13, 13 ) );
	pCheckBoxTargetDistance->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTargetDistance->SetText( "Show Target Distance" );
	pCheckBoxTargetDistance->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxTargetDistance->SetID( CHECKBOXID_TargetDistance );
	pWindowInterface->AddElement( pCheckBoxTargetDistance );

	//Target HP Value
	UI::CheckBox_ptr pCheckBoxTargetHPValue = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 378, 13, 13 ) );
	pCheckBoxTargetHPValue->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTargetHPValue->SetText( "Show Target HP Value" );
	pCheckBoxTargetHPValue->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxTargetHPValue->SetID( CHECKBOXID_TargetHPValue );
	pWindowInterface->AddElement( pCheckBoxTargetHPValue );

	//Target HP in percentage
	UI::CheckBox_ptr pCheckBoxTargetHPPercent = std::make_shared<UI::CheckBox>( Rectangle2D( 24, 398, 13, 13 ) );
	pCheckBoxTargetHPPercent->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxTargetHPPercent->SetText( "Show Target HP in %" );
	pCheckBoxTargetHPPercent->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pCheckBoxTargetHPPercent->SetID( CHECKBOXID_TargetHPPercent );
	pWindowInterface->AddElement( pCheckBoxTargetHPPercent );


	pWindowInterface->Hide();

	GetListMain()->AddWindow( pWindowInterface );
}

UI::List_ptr CSettingsWindow::GetListMain()
{
	auto pList = pWindow1->GetElement<UI::List>( LISTID_Main );
	if ( pList )
		return pList;

	return NULL;
}

UI::Window_ptr CSettingsWindow::GetWindow( int iID )
{
	return GetListMain()->GetWindow( iID );
}

int CSettingsWindow::AddGroup( UI::Window_ptr pWindowToAdd, bool bLeft, bool bAddY, int iHorizontalPadding, int iBaseX, int iBaseY, GroupAdd & sGroupAdd )
{
	int iSafeBaseX = iBaseX;
	int iSafeBaseY = iBaseY;
	iSafeBaseX += bLeft ? 0 : 180;

	if ( sGroupAdd.strGroupName.length() > 0 )
	{
		UI::Text_ptr pTextAdd = std::make_shared<UI::Text>( Rectangle2D( iSafeBaseX, iSafeBaseY, 0, 0 ) );
		pTextAdd->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
		pTextAdd->SetText( sGroupAdd.strGroupName + ":" );
		pWindowToAdd->AddElement( pTextAdd );

		if ( sGroupAdd.strTooltipText.length() > 0 )
		{
			UI::Tooltip_ptr pTooltipText = std::make_shared<UI::Tooltip>( Rectangle2D() );
			pTooltipText->Init();
			pTooltipText->SetLinkObject( pTextAdd );
			pTooltipText->SetAddAxis( -86, -20 );
			pTooltipText->SetFont( "Arial", 16, -1 );
			pTooltipText->SetPosition( TOOLTIPPOSITION_Left );
			pTooltipText->SetFollowMouse( FALSE );
			pTooltipText->SetText( sGroupAdd.strTooltipText );
			pWindowToAdd->AddElement( pTooltipText );
		}
	}

	UI::Group_ptr pGroupAdd = nullptr;
	if ( sGroupAdd.iGroupID > 0 )
	{
		pGroupAdd = std::make_shared<UI::Group>();
		pGroupAdd->SetID( sGroupAdd.iGroupID );
		pWindowToAdd->AddElement( pGroupAdd );
	}

	iSafeBaseY += 20;

	int iCount = 0;

	int iObjectX = iSafeBaseX;
	int iObjectY = iSafeBaseY;

	int iColumn = 0;
	int iRow = 0;

	for ( auto & sOption : sGroupAdd.vOptions )
	{
		if ( sGroupAdd.bHorizontal )
		{
			if ( iColumn == 2 )
			{
				iObjectX = iSafeBaseX;

				iObjectY += 20;

				iColumn = 0;
			}
			else if ( iColumn > 0 )
				iObjectX += 88 + iHorizontalPadding;

			iColumn++;
		}
		else
		{
			if ( iRow == ((sGroupAdd.vOptions.size() / 2) + (sGroupAdd.vOptions.size() % 2)) )
			{
				iObjectY = iSafeBaseY;

				iObjectX += 88 + iHorizontalPadding;

				iRow = 0;
			}
			else if ( iRow > 0 )
				iObjectY += 20;

			iRow++;
		}

		UI::CheckBox_ptr pCheckBoxON = std::make_shared<UI::CheckBox>( Rectangle2D( iObjectX, iObjectY, 13, 13 ) );
		pCheckBoxON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
		pCheckBoxON->SetText( sOption.strOptionName );
		
		if ( pGroupAdd )
			pCheckBoxON->SetGroup( pGroupAdd );

		pCheckBoxON->SetID( sOption.iOptionID );
		pCheckBoxON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
		pWindowToAdd->AddElement( pCheckBoxON );

		iCount++;
	}

	iBaseX = iSafeBaseX;

	int iNewY = 0;
	if ( bAddY )
		iNewY += (((sGroupAdd.vOptions.size() / 2) + (sGroupAdd.vOptions.size() % 2)) * 20) + 20;

	sGroupAdd.vOptions.clear();

	return iNewY;
}

int CSettingsWindow::AddTitle( UI::Window_ptr pWindowToAdd, int & iBaseX, int & iBaseY, const std::string strTitleImage )
{
	auto pImageTitle = UI::ImageLoader::LoadImage( strTitleImage.c_str() );
	int iImageX = iBaseX + (339 >> 1) - (pImageTitle->GetWidth() >> 1);

	UI::ImageBox_ptr pBar = std::make_shared<UI::ImageBox>( Rectangle2D( iBaseX + 10, iBaseY, 339, 23 ) );
	pBar->SetImage( UI::ImageLoader::LoadImage( "game\\images\\settings\\title.png" ) );
	pWindowToAdd->AddElement( pBar );

	UI::ImageBox_ptr pTitle = std::make_shared<UI::ImageBox>( Rectangle2D( iBaseX + iImageX + 10, iBaseY + 5, 150, 14 ) );
	pTitle->SetImage( pImageTitle );
	pWindowToAdd->AddElement( pTitle );

	iBaseY += 31;

	return iBaseY;
}

int CSettingsWindow::AddCheckBoxTwoOptions( UI::Window_ptr pWindowToAdd, bool bLeft, int & iBaseX, int & iBaseY, int iGroupID, int iCheckBoxOpt1ID, int iCheckBoxOpt2ID, const std::string strTitle, const std::string strOpt1, const std::string strOpt2, const std::string strToolboxText )
{
	int iNewBaseX = iBaseX + (bLeft ? 0 : 180);

	UI::Text_ptr pTextAdd = std::make_shared<UI::Text>( Rectangle2D( iNewBaseX, iBaseY, 0, 0 ) );
	pTextAdd->SetFont( "Arial", 16, 0, FALSE, TRUE, -1 );
	pTextAdd->SetText( strTitle + ":" );
	pWindowToAdd->AddElement( pTextAdd );

	UI::Group_ptr pGroupAdd = std::make_shared<UI::Group>();
	pGroupAdd->SetID( iGroupID );
	pWindowToAdd->AddElement( pGroupAdd );

	UI::CheckBox_ptr pCheckBoxON = std::make_shared<UI::CheckBox>( Rectangle2D( iNewBaseX, iBaseY + 20, 13, 13 ) );
	pCheckBoxON->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxON->SetText( strOpt1 );
	pCheckBoxON->SetGroup( pGroupAdd );
	pCheckBoxON->SetID( iCheckBoxOpt1ID );
	pCheckBoxON->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowToAdd->AddElement( pCheckBoxON );

	UI::CheckBox_ptr pCheckBoxOFF = std::make_shared<UI::CheckBox>( Rectangle2D( iNewBaseX + 88, iBaseY + 20, 13, 13 ) );
	pCheckBoxOFF->SetFont( "Arial", 16, 6, FALSE, FALSE, -1 );
	pCheckBoxOFF->SetText( strOpt2 );
	pCheckBoxOFF->SetGroup( pGroupAdd );
	pCheckBoxOFF->SetID( iCheckBoxOpt2ID );
	pCheckBoxOFF->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox.png" ), UI::ImageLoader::LoadImage( "game\\images\\UI\\checkbox_.png" ) );
	pWindowToAdd->AddElement( pCheckBoxOFF );

	iBaseY += bLeft ? 40 : 0;

	return iBaseY;
}
