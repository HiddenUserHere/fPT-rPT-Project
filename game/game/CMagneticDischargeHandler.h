#pragma once
class CMagneticDischargeHandler
{
private:
	int						iTimerID = -1;

public:
	bool					OnCastAnimationStart( Skill * psSkill );

	void					CastMembers( PacketSkillPartyData * psPacket );

	static void				AddMagneticSphere( UnitData * pcUnitData, float fTime, float fDamageDelay );

	BOOL					OnEvent( UnitData * pcUnitData );

	void					OnCastEnd( Unit * pcUnit );

	void					OnCastEffect( Unit * pcUnit, int iLevel, int iTime );

	BOOL					Damage( UnitData * pcUnitData );

private:
	void					AddTimer( int iTime, int iLevel );
};

