#pragma once

#define SAVE									CharacterGame::Save()
#define SAVEEX									CharacterGame::SaveEx()

#define UPDATE_CHARACTER_CHECKSUM				CharacterGame::ChecksumUpdate( &UNITDATA->sCharacterData )
#define CHECK_CHARACTER_CHECKSUM				CharacterGame::ChecksumCheck()

#define MAP_ID									(EMapID)CharacterGame::GetStageID()
#define MAP_TYPE								(EMapType)CharacterGame::GetMapType()

#define CHARACTER_PLAYCOUNTER					(*(DWORD*)0x00CF4824)
#define CHARACTER_LASTPLAYCOUNTER				(*(DWORD*)0x03A3E3BC)
#define CHARACTER_LASTSENDPLAYCOUNTER			(*(DWORD*)0x03A9777C)
#define CHARACTER_LASTTIMESENDPLAYDATA			(*(DWORD*)0x03A97780)
#define CHARACTER_VISIBLECOUNT					(*(DWORD*)0x03A97664)

#define TIME_LASTSAVE							(*(DWORD*)0x00D0FC34)

#define CHEAT_WARNING							(*(int*)0x03A97678)

class CharacterGame
{
protected:
	DWORD										dwLastItemCode = 0;

	int											iLastMapID = -1;

	int											iLastFocusID = -1;

private:
	static const int							iMaxRunSpeed = (((25 * 10) + 250) * 460) >> 8; // Run fMovementSpeed = 25

	static const DWORD							pfnSave				= 0x00627D20;
	static const DWORD							pfnChecksumUpdate	= 0x0045BC20;

	IMPFNC										pfnResetAllStats	= 0x00497B20;
	IMPFNC										pfnResetStats		= 0x00497E60;

	//Phoenix Pet
	const DWORD									pbShowPhoenixPet	= 0x0362B590;
	const DWORD									pfnSetPhoenix		= 0x004D65B0;
	const DWORD									pfnDeletePhoenix	= 0x004D6640;
	const DWORD									pcPhoenix			= 0x0362B388;
	const DWORD									piActivePhoenix		= pcPhoenix + 0x204;

	const DWORD									pdwMPDownTime		= 0x035D0C48;
	const DWORD									pdwMPDownPercent	= 0x035D0C4C;
	const DWORD									pdwSPDownTime		= 0x035D0C60;
	const DWORD									pdwSPDownPercent	= 0x035D0C64;

	INT64										ExpXor();

	int											iDecreaseAutomation = 0;

public:
	BOOL										bMPDown;
	BOOL										bSPDown;
	BOOL										bHPUp;
	BOOL										bMPUp;
	BOOL										bTopPVP;
	BOOL										bWeightStone;
	int											iCooldownT1;
	int											iCooldownT2;
	int											iCooldownT3;
	int											iCooldownT4;
	int											iCooldownT5;

	BOOL										bHPBooster;
	BOOL										bMPBooster;
	BOOL										bSTMBooster;
	BOOL										bRidingMount;


	CharacterDataEx								sCharacterDataEx;

												CharacterGame();
	virtual									   ~CharacterGame();

	void										OnCharacterUpdateData();
	void										OnCharacterSetCharInfoEx();
	BOOL										IsStageVillage();
	static int									GetStageID();
    static int                                  GetMapType();

	int											GetLastStageID() { return iLastMapID; };
	void										SetLastStageID( int i ) { iLastMapID = i; };

	void										SetDecreaseAutomation() { iDecreaseAutomation += 16; }


	int											GetCurrentMP();
	int											GetCurrentHP();
	int											GetCurrentSP();

	void										SetCurrentMP( int iMP );
	void										SetCurrentHP( int iHP );
	void										SetCurrentSP( int iSP );

	int											GetFSP() { return sCharacterDataEx.iFSP; }
	void										SetFSP( int i ) { sCharacterDataEx.iFSP = i; }

	void										SetPhoenix( BOOL bActive, int iPhoenixID = 0 );

	void										Die();

	static BOOL									Save();

	static void									SaveEx();

	static BOOL									ChecksumUpdate( CharacterData * psCharacterData );
	static BOOL __cdecl							ChecksumCheck();

	void										UpdateWeight();
	BOOL										CheckInventorySpace( ItemData * psItemData, BOOL bUseTemporaryItem = TRUE );
	BOOL										CheckInventoryWeight( ItemData * psItemData );

	int											OnMotionChange( UnitData * pcUnitGame, int iMotion, int iAnimationID, EAnimationType eAnimationType = ANIMATIONTYPE_None, bool bRidingMount = false );

	void										ResetHead( BOOL bDefault = FALSE );

	void										PHDamage( struct PacketSkillDamageNormal * psPacket );

	BOOL										PHTeleport( struct PacketTeleportEvent * psPacket );

	void										CancelAttack();

	void										OnUseManaPotion( int iMP );
	void										OnUseHPPotion( int iHP );
	void										OnUseSPPotion( int iSP );

	BOOL										CanUseGold( int iGold );

	void										ResetStats( int iNum );

	void										FocusTargetPet( int iID );

	void										HandlePacket( PacketUpdatePetDataEx * psPacket );
	void										HandlePacket( PacketCharacterDataEx * psPacket );
	void										HandlePacket( PacketUpdateIntegrity * psPacket );

	void										SetEXP( INT64 iEXP );

	INT64										GetEXP();

	int											GetCanCarryGold();
	BOOL										CanCarryGold( int iGold );

	DWORD										GetCharacterColorByClass( int iClass );

	void										UpdateCharacterPlayData();

	void										OnCooldownSkillUpdate();
};

