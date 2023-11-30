#pragma once
#include "UI.h"



class CSettingsWindow
{
public:
													CSettingsWindow();
	virtual											~CSettingsWindow();

	void											OnMouseMove( class CMouse * pcMouse );
	BOOL											OnMouseClick( class CMouse * pcMouse );
	BOOL											OnMouseScroll( class CMouse * pcMouse );

	BOOL											OnKeyPress( class CKeyboard * pcKeyboard );
	BOOL											OnKeyChar( class CKeyboard * pcKeyboard );

	void											OnResolutionChanged();

	void											Update( float fTime );
	void											Render();
	void											Init();

	void											OnLoginButtonClick( UIEventArgs e );


	enum
	{
		// Windows
		WINDOWID_Main,

		// Child Windows
		WINDOWID_Video,
		WINDOWID_Audio,
		WINDOWID_General,
		WINDOWID_Interface,

		WINDOWID_Screen,
		WINDOWID_Graphics,
		WINDOWID_Camera,

		LISTID_Main,

		// Buttons
		BUTTONID_Video,
		BUTTONID_Audio,
		BUTTONID_General,
		BUTTONID_Interface,

		// Groups
		GROUPID_Menu,

		GROUPID_Display,
		GROUPID_Renderer,
		GROUPID_Ratio,

		GROUPID_Resolutions4_3,
		GROUPID_Resolutions16_9,

		GROUPID_TextureQuality,
		GROUPID_BitDepth,
		GROUPID_VSync,
		GROUPID_Effects,
		GROUPID_DynamicShadows,
		GROUPID_NormalMapping,
		GROUPID_DamageDebug,
		GROUPID_HPBar,
		GROUPID_FPS,
		GROUPID_Ping,
		GROUPID_Element,
		GROUPID_AgingArmor,
		GROUPID_HideCaravans,
		GROUPID_KillInfo,

		GROUPID_CameraSight,
		GROUPID_InvertedCamera,

		GROUPID_Macro,
		GROUPID_AntiAliasing,

		CHECKBOXID_DirectX9,
		CHECKBOXID_DirectX9Ex,

		// Checkbox
		CHECKBOXID_Fullscreen,
		CHECKBOXID_Windowed,
		CHECKBOXID_AutoAdjust,
		CHECKBOXID_StartMaximized,

		CHECKBOXID_Ratio4_3,
		CHECKBOXID_Ratio16_9,

		CHECKBOXID_1024x768,
		CHECKBOXID_1280x960,
		CHECKBOXID_1400x1050,

		CHECKBOXID_1280x720,
		CHECKBOXID_1366x768,
		CHECKBOXID_1600x900,
		CHECKBOXID_1920x1080,

		CHECKBOXID_TextureHigh,
		CHECKBOXID_TextureLow,

		CHECKBOXID_32BPP,
		CHECKBOXID_16BPP,

		CHECKBOXID_VSyncON,
		CHECKBOXID_VSyncOFF,

		CHECKBOXID_Far,
		CHECKBOXID_Short,
		CHECKBOXID_Full,

		CHECKBOXID_CameraON,
		CHECKBOXID_CameraOFF,

		CHECKBOXID_NoMusic,
		CHECKBOXID_NoEffects,

        CHECKBOXID_EffectsON,
        CHECKBOXID_EffectsOFF,

		CHECKBOXID_DynamicShadowsON,
		CHECKBOXID_DynamicShadowsOFF,

		CHECKBOXID_NormalMappingON,
		CHECKBOXID_NormalMappingOFF,

		CHECKBOXID_DamageDebugON,
		CHECKBOXID_DamageDebugOFF,

		CHECKBOXID_HPBarON,
		CHECKBOXID_HPBarOFF,

		CHECKBOXID_FPSON,
		CHECKBOXID_FPSOFF,

		CHECKBOXID_PingON,
		CHECKBOXID_PingOFF,

		CHECKBOXID_ElementON,
		CHECKBOXID_ElementOFF,

		CHECKBOXID_AgingArmorON,
		CHECKBOXID_AgingArmorOFF,

		CHECKBOXID_HideCaravanON,
		CHECKBOXID_HideCaravanOFF,

		CHECKBOXID_KillInfoON,
		CHECKBOXID_KillInfoOFF,

		CHECKBOXID_LootFilterEnable,
		CHECKBOXID_LootHPPotion,
		CHECKBOXID_LootMPPotion,
		CHECKBOXID_LootSPPotion,
		CHECKBOXID_LootGold,
		CHECKBOXID_LootAmulets,
		CHECKBOXID_LootRings,
		CHECKBOXID_LootSheltoms,
		CHECKBOXID_LootForceOrbs,
		CHECKBOXID_LootPremiums,
		CHECKBOXID_LootMonsterCrystals,
		CHECKBOXID_LootDefenseItems,
		CHECKBOXID_LootOffenseItems,
		CHECKBOXID_LootEverything,

		//Loot Spec
		CHECKBOXID_LootSpecNS,
		CHECKBOXID_LootSpecFS,
		CHECKBOXID_LootSpecMS,
		CHECKBOXID_LootSpecAS,
		CHECKBOXID_LootSpecPS,
		CHECKBOXID_LootSpecATA,
		CHECKBOXID_LootSpecKS,
		CHECKBOXID_LootSpecMGS,
		CHECKBOXID_LootSpecPRS,
		CHECKBOXID_LootSpecASS,
		CHECKBOXID_LootSpecSHA,


		CHECKBOXID_DisableParty,
		CHECKBOXID_DisableTrade,
		CHECKBOXID_NoWhisper,
		CHECKBOXID_NoTrade,

		CHECKBOXID_AntiAliasingON,
		CHECKBOXID_AntiAliasingOFF,

		CHECKBOXID_LockUI,
		CHECKBOXID_Glow,
		CHECKBOXID_SaveUI,
		CHECKBOXID_ShowPartyMembers,
		CHECKBOXID_ShowRaidMembers,
		CHECKBOXID_ShowClanMembers,
		CHECKBOXID_ShowEnemies,
		CHECKBOXID_ShowNPCs,
		CHECKBOXID_ShowNextBossTimer,
		CHECKBOXID_AlwaysActive,
		CHECKBOXID_ShowNotice,
		CHECKBOXID_TargetDistance,
		CHECKBOXID_TargetHPValue,
		CHECKBOXID_TargetHPPercent,

		// Scrolls
		SCROLLID_BGMusic,
		SCROLLID_EffectSound,

		// Texts
		TextID_BossTime,
		TextID_ServerTime,
		TextID_ServerName,
		TEXTID_SiegeWarTime,
		TEXTID_SiegeWarType,

		// InputField
		InputFieldID_Macro1,
		InputFieldID_Macro2,
		InputFieldID_Macro3,
		InputFieldID_Macro4,
		InputFieldID_Macro5,
		InputFieldID_Macro6,
		InputFieldID_Macro7,
		InputFieldID_Macro8,
		InputFieldID_Macro9,
		InputFieldID_Macro10,

		//List
		LISTID_Video,
	};

private:

	struct GroupAddObject
	{
		std::string			strOptionName;

		int					iOptionID;

		GroupAddObject() {};
		GroupAddObject( int iID, const std::string strName )
		{
			iOptionID = iID;

			strOptionName = strName;
		};

		~GroupAddObject() {};
	};

	struct GroupAdd
	{
		int			iGroupID;

		std::string strGroupName;

		std::string strTooltipText;

		bool bHorizontal;

		GroupAdd() {};
		GroupAdd( int iID, const std::string strName, const std::string _strTooltipText, bool _bHorizontal )
		{
			iGroupID = iID;
			strGroupName = strName;
			strTooltipText = _strTooltipText;

			bHorizontal = _bHorizontal;
		};
		~GroupAdd() {};

		GroupAdd & Add( GroupAddObject sAdd )
		{
			vOptions.push_back( sAdd );

			return *this;
		}

		std::vector<GroupAddObject> vOptions;
	};

	void											SetResolutionSettings();
	int												GetResolutionCheckBox();
	void											SetResolutionCheckBox();

	void											Load();
	void											Save();

	void											OnCheckBoxRatio43Click( UIEventArgs eArgs );
	void											OnCheckBoxRatio169Click( UIEventArgs eArgs );
	void											OnVideoButtonClick( UIEventArgs eArgs );
	void											OnAudioButtonClick( UIEventArgs eArgs );
	void											OnGeneralButtonClick( UIEventArgs eArgs );
	void											OnInterfaceButtonClick( UIEventArgs eArgs );

	void											OnCloseButtonClick( UIEventArgs eArgs );
	void											OnSaveButtonClick( UIEventArgs eArgs );
	void											OnResetButtonClick( UIEventArgs eArgs );

	void											OnMenuButtonClick( UIEventArgs eArgs );

	void											BuildWindow1();
	void											BuildWindow1Video();
	void											BuildWindow1Audio();
	void											BuildWindow1General();
	void											BuildWindow1Interface();

	UI::List_ptr									GetListMain();

	UI::Window_ptr									GetWindow( int iID );

	int												AddGroup( UI::Window_ptr pWindowToAdd, bool bLeft, bool bAddY, int iHorizontalPadding, int iBaseX, int iBaseY, GroupAdd & sGroupAdd );

	int 											AddTitle( UI::Window_ptr pWindowToAdd, int & iBaseX, int & iBaseY, const std::string strTitleImage );
	int 											AddCheckBoxTwoOptions( UI::Window_ptr pWindowToAdd, bool bLeft, int & iBaseX, int & iBaseY, int iGroupID, int iCheckBoxOpt1ID, int iCheckBoxOpt2ID, const std::string strTitle, const std::string strOpt1, const std::string strOpt2, const std::string strToolboxText = "" );

protected:
	UI::Window_ptr									pWindow1 = NULL;
	UI::Window_ptr									pWindowLogin = NULL;

	UI::Window_ptr									pWindowMenu = NULL;

	const int										iWindowSettingsWidth  = 560;
	const int										iWindowSettingsHeight = 450;

};

