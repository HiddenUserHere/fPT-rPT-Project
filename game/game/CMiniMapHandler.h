#pragma once

//Size of Box of Minimap(in pixels)
#define MINIMAP_SIZE			128

//Size of Image of Minimap (0~100%)
#define MINIMAP_TEXTURE_SIZE	512
#define MINIMAP_TEXTURE_SCALE	84

struct MiniMapData
{
	struct EXETextureHandle		* psTexture;
	struct EXETextureHandle		* psTextureTitle;

	RECT						sRectangle;

	BaseMap						* pcBaseMap;

	Point2D						sPosition;
};

class CMiniMapHandler
{
public:
							CMiniMapHandler();
	virtual				   ~CMiniMapHandler();

	void					Init();
	void					Render();
	void					Update();

	BOOL					OnMouseClick( class CMouse * pcMouse );
	void					OnMouseMove( class CMouse * pcMouse );
	void					OnResolutionChanged();

	void					HandlePacket( PacketBossTimeUpdate * psPacket );
	void					ProcessPacket( PacketBossTimeUpdate * psPacket );

	void					OnChangeMap( int iMapID );

private:
	BOOL					bInitialized = FALSE;
	BOOL					bActive = FALSE;

	UI::Image				* pcBattleRoyaleBackground = NULL;
	UI::Image				* pcMaskBattleRoyaleBackground = NULL;
	UI::Image				* pcMaskBattleRoyaleMeteorite = NULL;

	UI::Image				* pcMaskBackground = NULL;
	UI::Image				* pcMaskBackgroundActive = NULL;
	UI::Image				* pcBorderImage = NULL;
	UI::Image				* pcMapImage = NULL;
	UI::Image				* pcUserPoint = NULL;
	UI::Image				* pcNPCPoint = NULL;
	UI::Image				* pcPKChar = NULL;
	UI::Image			   * pcImageNextBoss = NULL;

	UI::Image			   * pcImageEnemyIcon = NULL;
	UI::Image			   * pcImageFriendlyIcon = NULL;
	UI::Image			   * pcaImagesPartyMemberIcon[MAX_PARTY_MEMBERS];
	UI::Image			   * pcImageRaidIcon = NULL;

	DX::Effect_ptr			pEffectMask;
	DX::RenderTarget_ptr	pRenderTargetMask;

	BOOL					bSelected;
	Point2D					sPointMouse;

	Rectangle2D				sBox; 
	Point2D					saMinimaps[4];

	DX::Font_ptr			pFontMap = nullptr;
	int						iMinimapSize = 138;

	BOOL					bIsHoverUnit = FALSE;
	Rectangle2D				sHoverUnitBox = Rectangle2D();
	std::string				strHoverUnit;
	UI::Color				cHoverColor = UI::Color();
	UI::Tooltip_ptr			pTooltipUnit;

	BOOL					bShowBossTime = FALSE;
	std::string				strNextBossName;
	DWORD					dwNextBossTime = 0;
	DWORD					dwTickTime = 0;

	BOOL					bIsBattleRoyale = FALSE;

	void					RenderEffect();

	void					RenderMap( Point3D sUserPosition );
	void					RenderUser( Point3D sUserAngle );
	BOOL					RenderNPC( UnitData * pcUnitData, Point3D sUserPosition, int iSize );
	BOOL					RenderPlayer( UnitData * pcUnitData, Point3D sUserPosition, int iUserClanID, int iSize );
	BOOL					RenderParty( Point3D sUserPosition, int iUserID, int iSize );

	BOOL					RenderMapTime();

	MiniMapData				* GetMiniMapData( int i ) { return &((MiniMapData*)0x02085FB0)[i]; }

	BOOL					IsCurrentUserMap( MiniMapData  * psMap );

	Point2D					GetMiniMapPostion() { return Point2D(sBox.iX,sBox.iY); }
	void					SetMiniMapPostion( Point2D sPosition ){ sBox.iX = sPosition.iX; sBox.iY = sPosition.iY; }

	void					BeginViewport();
	void					EndViewport();

	static void				DrawMinimap( MiniMapData  * psMap, int iX, int iY, int iSize );

	void					SetMinimapSize( float fSize );

	Point2D					GetMinimapPositionFromCenter( Point3D sCenterPosition, Point3D sObjectPosition, int iWidthPoint, int iHeightPoint, int iSize );

};

