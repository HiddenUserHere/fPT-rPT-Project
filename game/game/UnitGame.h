#pragma once


#define UNITDATA				(UNITGAME->pcUnitData)
#define UNIT					(UNITGAME->pcUnit)
#define UNITDATABYID(id)		(UNITGAME->GetUnitDataByID(id))
#define UNITDATATOUNIT(ud)		(UNITGAME->UnitDataToUnit(ud))

#define UNITGAME				UnitGame::GetInstance()

#define MAP_ID_PVP              (READINT( 0x03A975F8 ))

#define MAX_CHARACTERID_REQUEST			200
#define MAX_CHARACTERID_REQUEST_TIME	15000

struct CharacterRequestDataID
{
	int					iID;

	DWORD				dwTime;
};

class UnitGame
{
private:
	static UnitGame			  * pcInstance;

	static int					iSpeedHack;
	static DWORD				dwUpdatePacket;

	int							iLastObjectID = 0;
	UnitData				  * pcViewUnitData;

	int							iLastUnitDataViewItemID = 0;

	static Unit					cViewUnit;

	Unit						* pcaUnit;

	UINT						iCharacterRequestIDWheel = 0;
	CharacterRequestDataID		saCharacterRequestID[MAX_CHARACTERID_REQUEST];

	IMPFNC						pfnGetUnitData = 0x0061EF70;
	IMPFNC						pfnUpdateObjectID = 0x00620F80;
	IMPFNC						pfnAddCharacterEXP = 0x00460830;
public:
	static void					CreateInstance() { pcInstance = new UnitGame(); }
	static UnitGame			  * GetInstance() { return pcInstance; }
	static void					DeleteInstance() { delete pcInstance; }

	UnitData				  * pcaUnitData;
	UnitData				  * pcUnitData;

	Unit					  * pcUnit;

	//Constructor
								UnitGame();
	virtual					   ~UnitGame();

	UnitData				  * GetUnitDataByID( ID lID );

	Unit					  * UnitDataToUnit( UnitData * pcUnitData );

	void						DrawLifeBar( int iX, int iY );

	BOOL						SetCharacterHeadModel( const char * pszHeadModel, BOOL bEffect = FALSE );

	void						ClearFarUnits();

	void						ReceiveServerInformation( PacketServerInfo * psPacket );
	
	static void					AddEXP( INT64 iEXP );
	INT64						GetEXPFromLevel( int iLevel );
	BOOL						OnLoseEXP();

	int							GetCharacterAbsorption();

	void						SetCharacterFormula( ItemData * pItem, ItemInventoryData * pInventory, struct CharacterDataLocal & sCharacterDataLocal );

	int							DefendChance( int iDefenseRating, int iAttackRating );
	int							BlockChance( int iBlockRating, int iBlock );

	void						RenderDebugText();
	void						RenderCharacterDebugText();

	void						SendUnitDataEx();

	static void					UpdateObjectID( ID lID );

	BOOL						RenderItemMotionBlur( UnitData * pcUnitData, UnitTool * psHand );

	BOOL						IsBlockedRangeTarget();
	BOOL						SetPVPArea( UnitData * pcUnitData );

	BOOL						Fun();

	void						MoveUnit( UnitData * pcUnitData, int iSpeed );

	void						HandlePacket( PacketSaveData * psPacket );
	void						HandlePacket( PacketCharacterDataSyncEx * psPacket );

	void						HandlePacketUnitStatus( UnitData * pcUnitData, Packet * psPacket );
	void						HandlePacketUnitBuff( UnitData * pcUnitData, PacketBuffData * psPacket );

	int							GetFramesSendCount();

	void						HandleElementalWeapon( Unit * pcUnit );

	void						UpdateElementalWeaponSettings();

	void						HandlePacket( SocketData * pcSocketData, PacketUnitStatusContainer * psPacket );

	UnitData					* GetFreeUnitDataSlot();

	bool						HasActiveCharacterIDRequest( int iID );
	void						AddCharacterIDRequest( int iID );
	void						RemoveCharacterIDRequest( int iID );
};