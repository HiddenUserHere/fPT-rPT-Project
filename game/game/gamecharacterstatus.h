#pragma once

struct CharacterDataLocal
{
	int							iAttackRatingSkill;			//0x0014

	int							iMinDamage;
	int							iMaxDamage;

	int							iCritical;

	int							iDefenseSkills;				//0x0030

	float						fAbsorbAdd;					//0x0044

	float						fIncreaseHPAdd;				//0x0048
	
	float						fRegenMP;					//0x0058

	float						fRegenHP;					//0x005C

	int							iAttackSpeedAdd;			//0x0060

	float						fRegenSP;					//0x0074

	int							iLevelMP;					//0x0078

	char						fPercentSPAdd;				//0x007C

	float						fIncreaseMPAdd;				//0x0080

	int							iWeaponSpeed;				//0x0084

	int							iCriticalItems;				//0x0088

	int							iLevelHP;					//0x008C

	float 						fBlockRatingAdd;			//0x0090

	int							iDefenseAdd;				//0x0094

	int							iCountAbsorbItems;			//0x0098

	char						iSkillWeaponSpeed;			//0x009C

	int							iAttackRange;				//0x00A0

	float						fMovementSpeed;				//0x00A4

	int							iViewSight;					//0x00E0

	int							iLevelAttackRating;			//0x00E4

	float						fPercentHPAdd;				//0x00E8

	float						fBlockRating;				//0x00EC

	int							iCriticalSkills;			//0x00F0

	int							iEvade;						//0x00F4

	float 						fAbsorbSkills;				//0x00F8

	float						fMovementSpeedAdd;			//0x00FC

	float						fPercentMPAdd;				//0x0100

	float						fIncreaseSPAdd;				//0x0104

	int							iAttackRangeAdd;			//0x0108

	float						fMagicMastery;

	int							iLevelDamage;

	int							iaElementalSpecDef[8];
	int							iaElementalSpecAtk[8];
	int							iaResistance[8];

	int							iTempDefense;
	float						fTempCritical;

	int							iTempWeaponSpeed;

	float						fTempDamageMin;
	float						fTempDamageMax;

	int							iTempAttackRating;

	int							iTempDefense2;

	int							iTempBlockRating;

	int							iTempAbsorption;

	int							iMinDamageSkill;
	int							iMaxDamageSkill;

	int							iPotionSpace;
};

struct ElementalAttackData
{
	IMinMax			sOrganic;
	IMinMax			sFire;
	IMinMax			sIce;
	IMinMax			sLightning;
	IMinMax			sPoison;
};

class CGameCharacterStatus
{
protected:
	IMPFNC						pfnOnMouseLClickDown = 0x004993B0;

public:
	CGameCharacterStatus();
	~CGameCharacterStatus();

	static CGameCharacterStatus	* GetInstance() { return (CGameCharacterStatus*)0x035D0EA0; }

	void						OnMouseLDownClick( int iX, int iY );

	void						UpdateCharacterStatus( CharacterDataLocal * psData );

	BOOL						UpdateCharacterStatusItem( BOOL bCheckHack = FALSE );

	static int					SPEC_HPPOTION_EFFICIENCE;
	static int					SPEC_MPPOTION_EFFICIENCE;
	static int					SPEC_SPPOTION_EFFICIENCE;

	bool						HasRequirementItem( ItemData * pcItemData );

private:
	bool						AddStatusItem( CharacterDataLocal & sCharacterDataLocal, ItemData * pcItemData );

	bool						AddSkillStatus( CharacterDataLocal & sCharacterDataLocal );
	bool						AddSkillStatusPost( CharacterDataLocal & sCharacterDataLocal );

	bool						UpdateHasRequirementItem();

	BOOL						CharacterStatusVerify( CharacterData * psCharacterData, CharacterData * psCharacterDataDest );

	static BOOL					UPDATE_BUFFSKILLS;
};

