#pragma once

enum ETeleportMapInfoType
{
	TELEPORTINFOTYPE_Town,
	TELEPORTINFOTYPE_EXP,
	TELEPORTINFOTYPE_Hunt,
	TELEPORTINFOTYPE_PVP,
	TELEPORTINFOTYPE_Solo,
	TELEPORTINFOTYPE_Mana,
};

struct TeleportMapInfo
{
	BOOL					bEnabled;

	EMapID					iMapID;

	ETeleportMapInfoType	iMapTypeFirst;
	ETeleportMapInfoType	iMapTypeSecond;
};


static const TeleportMapInfo saTeleportMapInfo[] =
{
	{ TRUE, MAPID_RicartenTown, TELEPORTINFOTYPE_Town, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_PhillaiTown, TELEPORTINFOTYPE_Town, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_NaviskoTown, TELEPORTINFOTYPE_Town, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_Eura, TELEPORTINFOTYPE_Town, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_Atlantis, TELEPORTINFOTYPE_Town, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_BattleTown, TELEPORTINFOTYPE_Town, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_RoadToTheWind, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_ValleyOfTranquility, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_LandOfDusk, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_ForestOfSpirits, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AcasiaForest, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_BambooForest, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_GardenOfFreedom, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_RefugeOfTheAncients, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_CastleOfTheLost, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_RuinenVillage, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_CursedLand, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_ForgottenLand, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_Oasis, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MysteryForest1, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MysteryForest2, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientsBattlefield, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_ForbiddenLand, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_BeehiveCave, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MushroomCave, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MysteryForest3, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_DarkSanctuary, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientPrisonF1, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientPrisonF2, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientPrisonF3, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_GreedyLake, TELEPORTINFOTYPE_Mana, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_CursedTempleF1, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_CursedTempleF2, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_RailwayOfChaos, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_HeartOfPerum, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_GallubiaValley, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MysteryDesert1, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_FrozenSanctuary, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MysteryDesert2, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_KelvezuLair, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_LandOfChaos, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_MysteryDesert3, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_LostTemple, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_EndlessTowerF1, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientDungeonF1, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_EndlessTowerF2, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientDungeonF2, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientDungeonF3, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_EndlessTowerF3, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_CursedTempleF3, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_DeathIsland, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_ForgottenTempleF1, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_ForgottenTempleF2, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_IceMineF1, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_SecretLaboratory, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AncientWeapon, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_IronCore, TELEPORTINFOTYPE_EXP, TELEPORTINFOTYPE_Town },
	{ TRUE, MAPID_AbyssSea, TELEPORTINFOTYPE_Hunt, TELEPORTINFOTYPE_Town },
};

class CTeleportCoreHandler
{
public:
	CTeleportCoreHandler();
	virtual ~CTeleportCoreHandler();

	void					Init();

	BOOL							OnMouseClick( class CMouse * pcMouse );
	void							OnMouseMove( class CMouse * pcMouse );
	BOOL							OnMouseScroll( class CMouse * pcMouse );

	void							OnResolutionChanged();

	void							Render();

	void							Update( float fTime );

	void							Open( ItemData * pcItemData );
	void							Close();

	BOOL							IsOpen();

private:

	enum
	{
		WINDOWID_Main,
		IMAGEID_Minimap,
		IMAGEID_MapTown,
		IMAGEID_MapEXP,
		IMAGEID_MapHunt,
		IMAGEID_MapPVP,
		IMAGEID_MapSolo,
		IMAGEID_MapMana,
		TEXTID_MapName,
		TEXTID_MapType,
		TEXTID_MapLevelText,
		TEXTID_MapLevel,

		BUTTONID_Teleport,

		LISTID_Map,
		WINDOWID_Map = 100,

		TEXTID_MapNameList	= 500,
		TEXTID_MapLevelList = 800,
	};

	void					BuildWindow();

	void					OnClickMap( int iMapID, int iIndex, UIEventArgs eArgs );
	void					OnClickTeleport( UIEventArgs eArgs );
	void					OnAcceptTeleport( int iMapID, UIEventArgs eArgs );

	void					OnButtonCloseClick( UIEventArgs eArgs );

	int						GetMapTypeImageID( ETeleportMapInfoType iType );
	const char				* GetMapTypeImageFile( ETeleportMapInfoType iType );

	UI::Window_ptr			pWindow;

	EItemID					iItemID = ITEMID_None;
	int						iChk1 = 0;
	int						iChk2 = 0;

	int						iMapID = -1;
};

