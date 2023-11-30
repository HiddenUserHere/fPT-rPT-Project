#pragma once




class CDamageInfoModel
{
private:
	enum eActionType
	{
		iDefenseRating						= 1,
		iBlockRating						= 2,
		Evade						= 3,
		iCritical					= 4,
		Damage						= 5,
		CriticalDamage				= 6,
	};
	void						loadParticles();
	int							FindTarget( UINT TargetID );
	void						DeleteTarget( int Position );
	void						OnActionEvent( UINT uPosition, UIEventArgs eArgs );
	void						renderTargets();

	float						fScale = 1.0f;

	//Create Bitmap Font
	UI::Bitmapfont_ptr			pOrangeFont = std::make_shared<UI::BitmapFont>(Rectangle2D(0,0,0,0));
	UI::Bitmapfont_ptr			pBlueFont	= std::make_shared<UI::BitmapFont>(Rectangle2D(0,0,0,0));
	UI::Bitmapfont_ptr			pRedFont	= std::make_shared<UI::BitmapFont>(Rectangle2D(0,0,0,0));
public:
	CDamageInfoModel();
	void						SetScale( float f );
	void						Load();
	void						AddTarget( UINT TargetID, int TypeAction, int Value );
	void						RenderTargets();
	void						Update(); //Every 100ms
	~CDamageInfoModel();
};

