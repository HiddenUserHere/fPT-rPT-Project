#pragma once
class CActionFieldRequestWindow
{
public:
	CActionFieldRequestWindow();
	virtual ~CActionFieldRequestWindow();

	void						Init();

	void						Render();

	void						Open();
	void						Close();

	void						OnResolutionChanged();

	void						OnMouseMove( class CMouse * pcMouse );
	BOOL						OnMouseClick( class CMouse * pcMouse );

	void						AddModeList( std::string strName, IMinMax sRequiredLevel, int iInstanceCount, int iInstanceLimitPerDay, EActionFieldModeID eMode );
	void						ClearList();

private:

	enum
	{
		LIST_Main = 100,
		IMAGE_ModeAFLogo,
		WINDOW_ModesAF = 150,
		TEXT_ModesAF = 200,
	};

	struct ImageLogoEventMode
	{
		EActionFieldModeID		eModeID;
		UI::Image				* pcImage;

		ImageLogoEventMode( EActionFieldModeID iModeID, UI::Image * pcImg ) { eModeID = iModeID; pcImage = pcImg; }
		~ImageLogoEventMode() {};
	};

	std::vector<ImageLogoEventMode> vLogos;

	UI::Window_ptr				pWindow;

	void						BuildWindow();

	void						OnButtonCloseClick( UIEventArgs eArgs );
	void						OnButtonYesClick( EActionFieldModeID eModeID, UIEventArgs eArgs );
	void						OnButtonRequestEventClick( std::string strName, EActionFieldModeID eModeID, UIEventArgs eArgs );
};

