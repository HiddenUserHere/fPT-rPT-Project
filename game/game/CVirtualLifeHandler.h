#pragma once
class CVirtualLifeHandler
{
public:
	CVirtualLifeHandler();
	virtual ~CVirtualLifeHandler();

	bool					OnCastAnimationStart( Skill * psSkill, UnitData * pcUnitData );
	bool					OnCastAnimationStart( Skill * psSkill );

	void					OnCastEffect( Unit * pcUnit, UINT uSkillLevel, double fOverTime );
	void					OnCastEnd( UnitData * pcUnitData );

	void					CastMembers();
	void					CastMembers( PacketSkillPartyData * psPacket );

private:
	int						iTimerID = 0;

	static BOOL				AddVirtualLife( int iTime, int iLevel );

	static void				AddVirtualLifeEffect( Point3D sPosition );

	static void				AddVirtualLifeHeart( UnitData * pcUnitData, int iTime );

	void					AddTimer( UnitData * pcUnitData, UINT uSkillLevel, double fOverTime );
};

