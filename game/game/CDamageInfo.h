#pragma once
#include "MDamageInfo.h"

#define MAX_DEBUG_TARGETS			350
#define MAX_DEBUG_TIME				1000
#define MAX_DEBUG_TIMEHALF			(MAX_DEBUG_TIME >> 1)
#define MAX_DEBUG_TIMEHALFMINOR		(MAX_DEBUG_TIME >> 2)
#define MAX_DEBUG_TIMEHALFMAJOR		(MAX_DEBUG_TIMEHALFMINOR * 3)

struct DamageDataInfo
{
	BOOL					bActive;

	int						iID;
	UnitData				* pcUnitData;


	int						iParticleID;

	int						iType;

	DWORD					dwTime;

	int						iValue;

	int						iX;
	int						iY;
	int						iDecreaseY;

	int						iAlpha;
};

class CDamageInfoController
{
private:
	void						load();
	class CDamageInfoModel		* pcDamageInfoModel = NULL;

	DamageDataInfo				* psaTargets = NULL;
	UINT						uNext = 0;

	int							iDecreaseY = 0;

	int							iDefenseInfoPart;
	int							iBlockInfoPart;
	int							iEvadeInfoPart;

	DWORD						dwTimeUpdate = 0;

	//Create Bitmap Font
	UI::Bitmapfont_ptr			pOrangeFont = std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pBlueFont	= std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );
	UI::Bitmapfont_ptr			pRedFont	= std::make_shared<UI::BitmapFont>( Rectangle2D( 0, 0, 0, 0 ) );

	void						Clear( DamageDataInfo * psData );

public:
	CDamageInfoController();
	virtual ~CDamageInfoController();

	class CDamageInfoModel		* GetModel() { return pcDamageInfoModel; };

	int							GetDecrease() { return iDecreaseY; };

	void						Init();

	void						Load();
	void						SetDefenseInfo( int TypeAction, int Value );
	void						AddTarget( struct PacketDebugDamageInfo * psPacket );
	void						Render();

	void						Update( float fTime );
};

