#pragma once
class CChristmasGiftWindowHandler
{
private:
	UI::Window_ptr				pWindow;

	DWORD						dwUpdateTimeClose = 0;

	enum
	{
		WINDOW_ReceiveEmpty,
		WINDOW_ReceiveItems,
		ITEMBOX_Items,
		TEXT_Sender,
		IMAGE_XmasTreeAnimation,
	};

	static const int			MAX_IMAGE_XMASANIMATION = 7;
	static const int			MAX_TIMEXMAS_WAITRESPONSE = 2500;

	bool						bReverseAnimation = false;
	UINT						uAnimationWheel = 0;

	UI::Image					* pcaImageAnimation[MAX_IMAGE_XMASANIMATION];

public:
	CChristmasGiftWindowHandler();
	virtual ~CChristmasGiftWindowHandler();

	void						Init();

	void						Update( float fTime );

	void						Render();

	void						Open( int iItemCount );

	void						Close();

	BOOL						OnMouseClick( CMouse * pcMouse );
	void						OnMouseMove( CMouse * pcMouse );

	void						OnResolutionChanged();

	void						ResetTimeWait() { dwUpdateTimeClose = 0; }

private:
	void						BuildWindow();

	void						OnButtonAcceptClick( UIEventArgs eArgs );
	void						OnButtonCloseClick( UIEventArgs eArgs );

	void						OnAcceptReceiveGift( UIEventArgs eArgs );

};

