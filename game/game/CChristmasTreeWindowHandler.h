#pragma once
class CChristmasTreeWindowHandler
{
private:
	UI::Window_ptr				pWindow;

	DWORD						dwUpdateTimeClose = 0;

	enum
	{
		ITEMBOX_Items,
		IMAGE_XmasTreeAnimation,

		TEXT_Information,
	};

	static const int			MAX_IMAGE_XMASANIMATION		= 7;
	static const int			MAX_TIMEXMAS_WAITRESPONSE	= 2500;

public:
	CChristmasTreeWindowHandler();
	virtual ~CChristmasTreeWindowHandler();

	void						Init();

	void						Update( float fTime );

	void						Render();

	void						Open( const char * pszCharacterName );
	
	void						Close();

	BOOL						OnMouseClick( CMouse * pcMouse );
	void						OnMouseMove( CMouse * pcMouse );

	void						OnResolutionChanged();

	void						ResetTimeWait() { dwUpdateTimeClose = 0; }

private:
	void						BuildWindow();

	void						OnButtonGiftClick( UIEventArgs eArgs );
	void						OnButtonCloseClick( UIEventArgs eArgs );

	void						OnAcceptSendGift( UIEventArgs eArgs );

	bool						bReverseAnimation = false;
	UINT						uAnimationWheel = 0;

	UI::Image					* pcaImageAnimation[MAX_IMAGE_XMASANIMATION];
};

