#pragma once

#define MAX_DAMAGETIME_COUNT	30
#define MAX_DAMAGETIME_TIME		1500


class CDamageHandler
{
private:
	IMPFNC						pfnSetSkillRange			= 0x00578130;
	IMPFNC						pfnSingleDamageRangeHandler = 0x0057CB90;
	IMPFNC						pfnDamageRangeHandler		= 0x0057AFF0;
	IMPFNC						pfnDamageRangeUserHandler	= 0x0057A630;
	IMPFNC						pfnSkillRangeMonsterSet		= 0x005572E0;
	IMPFNC						pfnSendDamageBySocket		= 0x0041C7D0;

	BOOL						bAreaDamage = FALSE;

	std::vector<Packet *>		vPvPPackets;

	static BOOL					SkillRangeMonsterSet( UnitData * pcUnitData, UserData * pcTarget, int iRange, int iDamagePercentDecrease );

	static BOOL					SendDamageBySocket( UnitData * pcThis, UnitData * pcUnitData, SocketData * pcSocketData, int iAttackID = 0, int iDamageAdd = 0, int iResistance = 0 );

	static BOOL					DecryptPacket( UserData * pcUserData, PacketSkillDamageNormalEx * psPacket );

	static DWORD				GetPacketChecksum( PacketSkillDamageNormalEx * psPacket );

	static int					GetCriticalChance( UserData * pcUserData, UnitData * pcUnitData, int iCritical );
	static int					GetCriticalChanceUser( UserData * pcUserData, UserData * pcTarget, int iCritical );

	BOOL						ProcessDamage( UserData * pcUserData, UserData * pcTarget, PacketSkillDamageNormal * psPacket );

	BOOL						bNewPvPHandler = FALSE;

public:
	CDamageHandler();
	virtual ~CDamageHandler();

	void						SetIsAreaDamage( BOOL b ) { bAreaDamage = b; }
	BOOL						IsAreaDamage() { return bAreaDamage; }

	static BOOL					SkillRangeHandler( UserData * pcUserData, UnitData * psUnitData, PacketSkillDamageNormal * psSkillNormal, PacketSkillDamageRange * psRangeData, PacketSkillDamageRangeEx * psRangeEx );
	static BOOL					SingleDamageRangeHandler( UserData * pcUserData, PacketSkillDamageNormal * psSkillNormal, UnitData * psUnitData, int iType );
	static BOOL					DamageRangeHandler( UserData * pcUserData, PacketSkillDamageRange * psRangeData, int iMapID, int iSkillID );
	static BOOL					DamageRangeUserHandler( UserData * pcUserData, PacketSkillDamageRange * psRangeData, int iMapID, int iSkillID );
	static BOOL					SkillSingleHandler( UserData * pcUserData, UnitData * pcUnitData, PacketSkillDamageNormal * psPacketDamageNormal, PacketSkillDamageNormalEx * psPacket );

	BOOL						HandlePacket( User * pcUser, PacketSkillDamageNormalEx * psPacket );
	BOOL						HandlePacket( User * pcUser, PacketSkillDamageRangeEx * psPacket );
	BOOL						HandlePacket( User * pcUser, PacketCancelSkill * psPacket );

	BOOL						UpdateDamageDataPVP( UserData * pcKiller, UserData * pcVictim, int iDamage );

	BOOL						IsMonsterAreaDamage( UnitData * pcUnitData );

	static BOOL					SendDamagePvP( UserData * pcUserData, UserData * pcTarget, int iDamage, int iID, int iParam1 = 0, int iParam2 = 0 );
	BOOL						SendDamagePvPData( UserData * pcUserData, UserData * pcTarget, int iDamage, int iID, int iParam1 = 0, int iParam2 = 0 );
	BOOL						UpdateDamagePvPData( User * pcUser );

	void						SetNewPVP( BOOL b ) { bNewPvPHandler = b; }

	void						SendCancelSkill( User * pcUser, int iPlayID );

	BOOL						CanHitTargetAreaDamage( UserData * pcUserData, UnitData * pcUnitData );
};

