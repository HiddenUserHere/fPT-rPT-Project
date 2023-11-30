#pragma once

enum ELootFilter : unsigned int
{
	LOOTFILTER_None				= 0x0,
	LOOTFILTER_HPPotion			= 0x0002,
	LOOTFILTER_MPPotion			= 0x0004,
	LOOTFILTER_SPPotion			= 0x0008,
	LOOTFILTER_Gold				= 0x0010,
	LOOTFILTER_Amulets			= 0x0020,
	LOOTFILTER_Rings			= 0x0040,
	LOOTFILTER_Sheltoms			= 0x0080,
	LOOTFILTER_ForceOrbs		= 0x0100,
	LOOTFILTER_Premiums			= 0x0200,
	LOOTFILTER_MonsterCrystals	= 0x0400,
	LOOTFILTER_DefenseItems		= 0x0800,
	LOOTFILTER_OffenseItems		= 0x1000,
	LOOTFILTER_Everything		= 0x2000,
};

enum ELootFilterSpec : unsigned int
{
	LOOTFILTERSPEC_None			= 0x0,
	LOOTFILTERSPEC_NS			= 0x0002,
	LOOTFILTERSPEC_FS			= 0x0004,
	LOOTFILTERSPEC_MS			= 0x0008,
	LOOTFILTERSPEC_AS			= 0x0010,
	LOOTFILTERSPEC_PS			= 0x0020,
	LOOTFILTERSPEC_ATA			= 0x0040,
	LOOTFILTERSPEC_KS			= 0x0080,
	LOOTFILTERSPEC_MGS			= 0x0100,
	LOOTFILTERSPEC_PRS			= 0x0200,
	LOOTFILTERSPEC_ASS			= 0x0400,
	LOOTFILTERSPEC_SHA			= 0x0800,
};

class CSettingsModel
{
private:
	struct sSettings
	{
		int												iRendererVersion;
		BOOL											bWindowed;
		BOOL											bWidescreen;
		BOOL											bAutoAdjust;
		DWORD											dwWidth;
		DWORD											dwHeight;

		DWORD											dwBitDepth;
		BOOL											bVSync;
		BOOL											bHighTextureQuality;
        BOOL											bNoEffects;
        BOOL											bDynamicShadows;
		BOOL											bNormalMapping;

		BOOL											bFarCameraSight;
		BOOL											bInvertedCamera;
		
		BOOL											bNoSoundEffects;
		BOOL											bNoMusic;
		int												iMusicVolume;
		int												iEffectVolume;

		char											szaMacro[10][100];
		char											szLastAccount[32];

		BOOL											bNoTradeChat;
		BOOL											bNoWhisper;
		BOOL											bNoTradeRequest;
		BOOL											bNoPartyRequest;

		BOOL											bDebugDamage;
		BOOL											bHPBarOnHead;
		BOOL											bFPS;
		BOOL											bPing;
		BOOL											bDebugKill;

		BOOL											bElementEffect;
		BOOL											bArmorAgeEffect;
		BOOL											bHideCaravans;

		BOOL											bStartMaximized;
		BOOL											bAntiAliasing;
		int												iCameraRange;

		BOOL											bEnableLootFilter;
		UINT											iLootFilterFlags;
		UINT											iLootFilterSpecFlags;

		BOOL											bLockUI;
		BOOL											bGlow;
		BOOL											bSaveUI;
		BOOL											bShowPartyMembers;
		BOOL											bShowClanMembers;
		BOOL											bShowEnemies;
		BOOL											bShowNPCs;
		BOOL											bMinimapAlwaysActive;
		BOOL											bShowRaidMembers;
		BOOL											bShowNextBossTimer;
		BOOL											bShowNotice;
		BOOL											bTargetDistance;
		BOOL											bTargetHPValue;
		BOOL											bTargetHPPercent;
	};
	sSettings											sSettingsData;
	sSettings											sOldSettingsData;

	BOOL												bMaximizedWindow = FALSE;

	BOOL												bFirstRead = FALSE;

public:
	CSettingsModel();
	virtual ~CSettingsModel();

	void												OnCheckWindowState();

	BOOL												IsMaximizedWindow() { return bMaximizedWindow; }


	sSettings &											GetSettings();
	sSettings &											GetOldSettings();
	BOOL												ReadSettings();
	void												WriteSettings();

	BOOL												CanViewLoot( ELootFilter e );
	void												ResetViewLoot();
	void												RemoveViewLoot( ELootFilter e );
	void												AddViewLoot( ELootFilter e );
	void												SetViewLoot( ELootFilter e, BOOL b );

	BOOL												CanViewLootSpec( ELootFilterSpec e );
	void												RemoveViewLootSpec( ELootFilterSpec e );
	void												AddViewLootSpec( ELootFilterSpec e );
	void												SetViewLootSpec( ELootFilterSpec e, BOOL b );
	BOOL												HaveViewLootSpec();

	void												ProcessGeneralSettings( BOOL bRead = FALSE );

	void												Init();
};

