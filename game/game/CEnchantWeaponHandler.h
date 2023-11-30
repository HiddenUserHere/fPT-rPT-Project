#pragma once
class CEnchantWeaponHandler
{
public:
	CEnchantWeaponHandler();
	virtual ~CEnchantWeaponHandler();

	bool					OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
	bool					OnCastAnimationStart( Skill * psSkill );

	void					OnCastEffect( Unit * pcUnit, UINT uSkillLevel, int iElementID, double fOverTime );
	void					OnCastEnd( UnitData * pcUnitData );

	void					CastMembers( int iElementID );
	void					CastMembers( PacketSkillPartyData * psPacket );

private:
	int						iTimerID = 0;

	static void				AddEnchantWeaponEffect( Unit * pcUnit, int iElementID );
	static void				AddEnchantWeaponFire( UnitData * pcUnitData, int iTime );
	static void				AddEnchantWeaponIce( UnitData * pcUnitData, int iTime );
	static void				AddEnchantWeaponLight( UnitData * pcUnitData, int iTime );

	void					AddTimer( UnitData * pcUnitData, UINT uSkillLevel, int iElementID, double fOverTime );

};

