#pragma once

class CBellatraInstance
{
public:
	CBellatraInstance();
	~CBellatraInstance();

	int						GetID() { return iID; }
	void					SetID( int _iID ) { iID = _iID; }

	UINT					GetDropID() { return iDropID; }
	void					SetDropID( int _iDropID ) { iDropID = _iDropID; }

	SOD::CBellatra			* GetBellatra() { return pcBellatra; }
	void					SetBellatra( SOD::CBellatra * _pcBellatra ) { pcBellatra = _pcBellatra; }

	UINT					GetRoundNumber() { return iRound; }
	void					SetRoundNumber( UINT i ) { iRound = i; }

	CRoomInstance			* GetRoom() { return pcRoom; }
	void					SetRoom( CRoomInstance * _pcRoom ) { pcRoom = _pcRoom; }

	BOOL					IsStarted() { return bStart; }
	void					Start();
	void					Stop() { bStart = FALSE; iDelayTime = 0; }

	UINT					GetDelayTime() { return iDelayTime; }
	void					SetDelayTime( UINT _iDelayTime ) { iDelayTime = _iDelayTime; }


	BOOL					IsFinished() { return bFinish; }
	void					Finish() { bFinish = TRUE; }

	SOD::CBellatraRound		* GetActiveRound();
	SOD::CBellatraRound		* GetNextRound();
	SOD::CBellatraRound		* GetFirstRound();

	std::vector<SOD::BellatraTeam *> & GetTeams() { return vTeams; };

	SOD::BellatraTeam		* GetTeam( int iID );
	SOD::BellatraTeam		* GetTeamByPlayerID( int iID );
	SOD::BellatraTeam		* GetTeamByUnitID( int iID );
	SOD::BellatraTeam		* GetTeamByRoomID( int iRoomID );
	SOD::BellatraPlayer		* GetPlayer( int iID );

	bool					IsSameRoom( int iUserID, int iUnitID );

	bool					GetTeamUsers( SOD::BellatraTeam * psTeam, std::vector<User *> & vUsers, bool bAddInvalid = false );
	bool					GetTeamUnits( SOD::BellatraTeam * psTeam, std::vector<Unit *> & vUnits );

	void					RemoveUnit( int iID );
	void					InvalidateUnit( int iID );
	bool					AddUnit( int iRoomID, int iID );
	bool					AddUser( int iRoomID, int iID, int iCharacterID );
	bool					HasUser( int iID );
	bool					InvalidateUser( int iID );

	void					ClearRound();

	void					Update();

	UINT					iUpdateTime = 0;

private:
	int						iID;

	UINT					iDropID;

	BOOL					bStart			= FALSE;
	BOOL					bFinish			= FALSE;

	SOD::CBellatra			* pcBellatra	= nullptr;

	UINT					iRound			= 0;

	CRoomInstance			* pcRoom		= nullptr;

	UINT					iDelayTime		= 0;

	std::vector<SOD::BellatraTeam *>	vTeams;
};


class CBellatraServerHandler : public CServerEvent
{
private:
	const BOOL				GOLD_SOD_DIVIDE_MEMBERS = FALSE;

	const UINT				MIN_CLANUSERS_SCORE = 3;

	BOOL					bIhin = TRUE;

	std::vector<CBellatraInstance *> vInstances;
	std::vector<SOD::CBellatra *>		vBellatras;

	struct BellatraClanTopScore
	{
		int					iClanID;

		int					iScore;

		int					iGold;

		char				szName[32];
	};

public:
	CBellatraServerHandler();
	virtual ~CBellatraServerHandler();

	void					Clear();

	void					Init();

	void					Load( int iID = -1 );

	void					HandlePacket( User * pcUser, PacketBellatraRequest * psPacket );
	void					HandleNetPacket( PacketNetBellatraRequest * psPacket );
	void					HandleNetPacket( PacketNetBellatraGiveGold * psPacket );

	void					Update( ETickType eTick ) override;

	void					HandleItem( User * pcUser, EItemID eItemID );

	BOOL					IsExclusiveDrop( User * pcUser, int iDropID );

	void					SendRoomScoreNPC( User * pcUser, int iRoomID );

	BOOL					OnUnitKilledPost( Unit * pcUnit );

private:

	friend class CBellatraInstance;

	void					RemoveBellatra( int iID );

	void					UpdateRounds( CBellatraInstance * pcInstance );
	void					UpdateTeams( CBellatraInstance * pcInstance );

	void					HandleEndRound( CBellatraInstance * pcInstance );
	void					HandleStartRound( CBellatraInstance * pcInstance );

	BOOL					OnUnitKilled( User * pcUser, Unit * pcUnit ) override;
	BOOL					OnUserMapChanged( User * pcUser, EMapID eMapID ) override;
	BOOL					OnDisconnected( User * pcUser ) override;
	BOOL					OnNPCClick( User * pcUser, Unit * pcUnit ) override;

	void					HandleScore( CBellatraInstance * pcInstance );

	UnitData				* SpawnMonster( Map * pcMap, SOD::CBellatraRound * pcRound, int iAreaID );
	UnitData				* SpawnBoss( Map * pcMap, SOD::CBellatraRound * pcRound, int iAreaID );

	Point3D					GetRandomPosition( SOD::CBellatraRound * pcRound, int iAreaID );

	void					ClearRounds( CBellatraInstance * pcInstance );

	void					KillUnits( CBellatraInstance * pcInstance );

	bool					SQLGetBellatras( std::vector<int> & vBellatrasID );
	SOD::CBellatra			* SQLLoadBellatra( int iID );
	bool					SQLLoadBellatraRound( SOD::CBellatra * pcBellatra );
	bool					SQLLoadBellatraMonsterRound( SOD::CBellatraRound * pcBellatraRound );
	bool					SQLLoadBellatraAreaRound( SOD::CBellatraRound * pcBellatraRound );
	
	int						SQLGetTeamClanID( int iTeamID );
	void					SQLSetTeamScoreClanID( int iClanID, int iScore );
	bool					SQLGetTopClanScore( BellatraClanTopScore & sClanTopScore );

	int						SQLGetLastInstance();
	int						SQLGetLastTeamInstance();
	bool					SQLCreateInstance( CBellatraInstance * pcInstance );
	bool					SQLEndInstance( CBellatraInstance * pcInstance );

	bool					SQLCreateCharacterInstance( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, User * pcUser );
	bool					SQLCreateTeamInstance( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam );
	bool					SQLLeaveCharacterInstance( CBellatraInstance * pcInstance, SOD::BellatraPlayer * psPlayer, User * pcUser );
	bool					SQLEndCharacterInstance( CBellatraInstance * pcInstance, SOD::BellatraPlayer * psPlayer );

	bool					SQLFailTeam( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam );
	bool					SQLEndTeam( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam );

	CBellatraInstance		* CreateInstance( SOD::CBellatra * pcBellatra );

	CBellatraInstance		* GetRankedInstance();
	CBellatraInstance		* GetUnrankedInstance();

	void					SendGold( CBellatraInstance * pcInstance, User * pcUser, int iGold, bool bWinner = false );
	void					SendGoldTeam( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, bool bWinner = false );
	void					SendStageComplete( CBellatraInstance * pcInstance, User * pcUser, int iAreaID );
	void					SendStageFail( CBellatraInstance * pcInstance, User * pcUser );

	void					SendScore( CBellatraInstance * pcInstance, User * pcUser = nullptr );

	BOOL					IsInPartyTeam( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam );
	BOOL					IsClanTeam( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam );

	bool					KickFailedTeams( CBellatraInstance * pcInstance );

	void					ProcessPacket( User * pcUser, PacketBellatraNPC * psPacket );
	void					ProcessPacket( User * pcUser, PacketBellatraInformation * psPacket );
	void					ProcessPacket( User * pcUser, PacketBellatraScore * psPacket );
	void					ProcessPacket( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, PacketBellatraEffectCast * psPacket );
	void					ProcessPacket( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, PacketEffectSpawn * psPacket );

	void					TeleportUserRoom( CBellatraInstance * pcInstance, User * pcUser, int iAreaID );
	void					TeleportUser( User * pcUser, EMapID iMapID, Point3D sPosition );

	void					SendChatTeam( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, EChatColor eChatColor, const std::string strMessage );

	void					HandleBellatraItem( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam );

	void					HandleQuakeSeal( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, std::vector<Unit *> & vUnits );
	void					HandleStunSeal( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, std::vector<Unit *> & vUnits );
	void					HandleIceFreezeSeal( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, std::vector<Unit *> & vUnits );
	void					HandleRabieSeal( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, std::vector<Unit *> & vUnits );
	void					HandleSummonSeal( CBellatraInstance * pcInstance, SOD::BellatraTeam * psTeam, User * pcUser, EMonsterEffectID eMonsterEffectID, int iLevel );
	void					HandleScoreSeal( CBellatraInstance * pcInstance, User * pcUser, SOD::BellatraPlayer * psPlayer );
};

