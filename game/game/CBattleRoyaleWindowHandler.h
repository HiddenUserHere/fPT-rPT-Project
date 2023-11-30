#pragma once
class CBattleRoyaleWindowHandler
{
private:
	friend class CBattleRoyaleHandler;

	enum
	{
		WINDOW_Main,
		LIST_Main,
		LIST_Reward,
		LIST_Rules,
		WINDOW_Reward,
		WINDOW_Rules,
		LIST_RewardWindow,
		BUTTON_Roll,
		BUTTON_Claim,
		BUTTON_Back,
		BUTTON_Reward,
		BUTTON_Rules,
		TEXT_NoRewards,
		IMAGE_GoldExp,

		WINDOW_BattleID = 500,
	};

	enum EWindowOpenID : int
	{
		WINDOWOPENID_Main,
		WINDOWOPENID_Reward,
		WINDOWOPENID_Rules,
	};


	UI::Window_ptr					pWindow1 = nullptr;

	struct RewardChance
	{
		int						iID;
		int						iChance;

		RewardChance( int _iID, int _iChance ) { iID = _iID; iChance = _iChance; };
		~RewardChance() {};
	};

	struct BattleStatus
	{
		int				iBattleID;
		bool			bOpen;

		BattleStatus( int _iBattleID, bool _bOpen ) { iBattleID = _iBattleID; bOpen = _bOpen; };
		~BattleStatus() {};
	};

	std::vector<RewardChance>		vRewardChances;
	std::vector<BattleStatus>		vBattleStatus;
	int								iTotalChance = 0;
public:
	CBattleRoyaleWindowHandler();
	virtual ~CBattleRoyaleWindowHandler();

	void							Init();

	void							Open( EWindowOpenID eWindowID, bool bClear = false );
	void							Close();
	void							Clear();

	void							AddBattle( int iBattleID, int iMapID, bool bOpen, IMinMax sRequiredLevel, INT64 iTimeLeft, const std::string strName );
	void							AddReward( int iID, int iChance, const std::string strName, const std::string strImagePath );

	void							SetDataID( int iInstanceID, int iBattleID );

	void							Render();

	void							Update( float fTime );

	BOOL							OnMouseClick( CMouse * pcMouse );
	void							OnMouseMove( CMouse * pcMouse );
	BOOL							OnMouseScroll( CMouse * pcMouse );

	void							UpdateReward();

	void							OnResolutionChanged();

private:
	void							BuildWindow();

	void							OnButtonCloseClick( UIEventArgs eArgs );
	void							OnButtonRequestClick( int iBattleID, UIEventArgs eArgs );
	void							OnButtonRollClick( UIEventArgs eArgs );
	void							OnButtonClaimClick( UIEventArgs eArgs );
	void							OnButtonBackClick( UIEventArgs eArgs );
	void							OnButtonRewardClick( UIEventArgs eArgs );
	void							OnButtonRulesClick( UIEventArgs eArgs );

	void							OnAcceptRequest( int iBattleID );
	void							OnAcceptRoll();

	void							MoveRewardList( int iSpeed );

	void							ScaleImage( UI::Window_ptr pWindowReward, float fScale );

	int								GetRewardID() { return iRewardID; }
	int								GetNewRewardID();

	int								GetCurrentRewardID();

	int								GetCountToReward();

	float							fTimeSending = 0.0f;
	bool							bRoll = false;
	float							fUpdateRoll = 0.0f;
	float							fDurationRoll = 0.0f;
	int								iRollCount = 0;
	float							fLastEase = 0.0f;

	int								iRewardID = 0;

	int								iInstanceServerID = -1;
	int								iBattleServerID = -1;

protected:
	UI::List_ptr					GetRewardWindowList() { return pWindow1->GetElement<UI::List>( LIST_Reward )->GetWindow( WINDOW_Reward )->GetElement<UI::List>( LIST_RewardWindow ); }
};

