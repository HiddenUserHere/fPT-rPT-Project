#include "stdafx.h"
#include "TimerSkill.h"

Skill sSkillTimerData[MAX_SKILLS_TIMER + 1];
Skill sTempSkillTimerData[MAX_SKILLS_TIMER + 1];

CTimerSkill::CTimerSkill()
{
}

CTimerSkill::~CTimerSkill()
{
}

void CTimerSkill::Hooks()
{
	WritePointer( 0x00504C05, &sTempSkillTimerData[0] );
	WritePointer( 0x00504C11, &sTempSkillTimerData[0] );
	WritePointer( 0x00509D2A, &sTempSkillTimerData[0 - 1].iID );
	WritePointer( 0x00504D08, &sTempSkillTimerData[0].iCheckTime );
	WritePointer( 0x0050522F, &sTempSkillTimerData[0].MatIcon );
	WritePointer( 0x005052A2, &sTempSkillTimerData[_countof( sTempSkillTimerData )].MatIcon );

	WritePointer( 0x004E981B, &sSkillTimerData[0] );
	WritePointer( 0x004ED2BD, &sSkillTimerData[0] );
	WritePointer( 0x004ED542, &sSkillTimerData[0] );
	WritePointer( 0x004ED5D2, &sSkillTimerData[0] );
	WritePointer( 0x004ED662, &sSkillTimerData[0] );
	WritePointer( 0x004ED6F2, &sSkillTimerData[0] );
	WritePointer( 0x00504543, &sSkillTimerData[0] );
	WritePointer( 0x00504629, &sSkillTimerData[0] );
	WritePointer( 0x00504770, &sSkillTimerData[0] );
	WritePointer( 0x00504852, &sSkillTimerData[0] );
	WritePointer( 0x005048DA, &sSkillTimerData[0] );
	WritePointer( 0x0050490A, &sSkillTimerData[0] );
	WritePointer( 0x005049AA, &sSkillTimerData[0] );
	WritePointer( 0x005049DA, &sSkillTimerData[0] );
	WritePointer( 0x00504ABC, &sSkillTimerData[0] );
	WritePointer( 0x00520FC5, &sSkillTimerData[0] );
	WritePointer( 0x00520FF6, &sSkillTimerData[0] );
	WritePointer( 0x0052127B, &sSkillTimerData[0] );
	WritePointer( 0x005212C4, &sSkillTimerData[0] );
	WritePointer( 0x005223BC, &sSkillTimerData[0] );
	WritePointer( 0x0052273F, &sSkillTimerData[0] );
	WritePointer( 0x005228E8, &sSkillTimerData[0] );
	WritePointer( 0x0052290D, &sSkillTimerData[0] );
	WritePointer( 0x00522C76, &sSkillTimerData[0] );



	WritePointer( 0x00434497, &sSkillTimerData[0].iID );
	WritePointer( 0x0043E801, &sSkillTimerData[0].iID );
	WritePointer( 0x00453763, &sSkillTimerData[0].iID );
	WritePointer( 0x004B96D2, &sSkillTimerData[0].iID );
	WritePointer( 0x004BD5BA, &sSkillTimerData[0].iID );
	WritePointer( 0x004BD955, &sSkillTimerData[0].iID );
	WritePointer( 0x004BDA32, &sSkillTimerData[0].iID );
	WritePointer( 0x004BE9BC, &sSkillTimerData[0].iID );
	WritePointer( 0x004EC387, &sSkillTimerData[0].iID );
	WritePointer( 0x004EC3C7, &sSkillTimerData[0].iID );
	WritePointer( 0x004EC407, &sSkillTimerData[0].iID );
	WritePointer( 0x004EC436, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED288, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED4D1, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED532, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED561, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED5C2, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED5F1, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED652, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED681, &sSkillTimerData[0].iID );
	WritePointer( 0x004ED6E2, &sSkillTimerData[0].iID );
	WritePointer( 0x005044D1, &sSkillTimerData[0].iID );
	WritePointer( 0x00504533, &sSkillTimerData[0].iID );
	WritePointer( 0x005045B9, &sSkillTimerData[0].iID );
	WritePointer( 0x00504617, &sSkillTimerData[0].iID );
	WritePointer( 0x005046F1, &sSkillTimerData[0].iID );
	WritePointer( 0x0050475A, &sSkillTimerData[0].iID );
	WritePointer( 0x005047E1, &sSkillTimerData[0].iID );
	WritePointer( 0x00504842, &sSkillTimerData[0].iID );
	WritePointer( 0x00504876, &sSkillTimerData[0].iID );
	WritePointer( 0x00504919, &sSkillTimerData[0].iID );
	WritePointer( 0x00504946, &sSkillTimerData[0].iID );
	WritePointer( 0x005049E9, &sSkillTimerData[0].iID );
	WritePointer( 0x00504A23, &sSkillTimerData[0].iID );
	WritePointer( 0x00504AAC, &sSkillTimerData[0].iID );
	WritePointer( 0x00504E1F, &sSkillTimerData[0].iID );
	WritePointer( 0x00520C17, &sSkillTimerData[0].iID );
	WritePointer( 0x00520F82, &sSkillTimerData[0].iID );
	WritePointer( 0x005210CC, &sSkillTimerData[0].iID );
	WritePointer( 0x0052111A, &sSkillTimerData[0].iID );
	WritePointer( 0x00521167, &sSkillTimerData[0].iID );
	WritePointer( 0x005215D3, &sSkillTimerData[0].iID );
	WritePointer( 0x00522355, &sSkillTimerData[0].iID );
	WritePointer( 0x00522514, &sSkillTimerData[0].iID );
	WritePointer( 0x005225AB, &sSkillTimerData[0].iID );
	WritePointer( 0x00522781, &sSkillTimerData[0].iID );
	WritePointer( 0x00522800, &sSkillTimerData[0].iID );
	WritePointer( 0x00522818, &sSkillTimerData[0].iID );
	WritePointer( 0x0052282F, &sSkillTimerData[0].iID );
	WritePointer( 0x00522847, &sSkillTimerData[0].iID );
	WritePointer( 0x0052289A, &sSkillTimerData[0].iID );
	WritePointer( 0x005229A0, &sSkillTimerData[0].iID );
	WritePointer( 0x005229E6, &sSkillTimerData[0].iID );
	WritePointer( 0x00522A26, &sSkillTimerData[0].iID );
	WritePointer( 0x00522A45, &sSkillTimerData[0].iID );
	WritePointer( 0x00522B10, &sSkillTimerData[0].iID );
	WritePointer( 0x00522B41, &sSkillTimerData[0].iID );
	WritePointer( 0x00522B7D, &sSkillTimerData[0].iID );
	WritePointer( 0x00522BE8, &sSkillTimerData[0].iID );
	WritePointer( 0x00522C2F, &sSkillTimerData[0].iID );
	WritePointer( 0x00522C5B, &sSkillTimerData[0].iID );
	WritePointer( 0x005B4815, &sSkillTimerData[0].iID );
	WritePointer( 0x00628F1B, &sSkillTimerData[0].iID );
	WritePointer( 0x00629070, &sSkillTimerData[0].iID );
	WritePointer( 0x006292E8, &sSkillTimerData[0].iID );
	WritePointer( 0x00629576, &sSkillTimerData[0].iID );
	WritePointer( 0x006296F2, &sSkillTimerData[0].iID );
	WritePointer( 0x006297D5, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0BB0E, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0BB3A, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0BC9A, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0C2F0, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0C51A, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0C5AF, &sSkillTimerData[0].iID );
	WritePointer( 0x04B0C600, &sSkillTimerData[0].iID );

	//Flag
	WritePointer( 0x004BD5AC, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED4F3, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED528, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED583, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED5B8, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED613, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED648, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED6A3, &sSkillTimerData[0].bActive );
	WritePointer( 0x004ED6D8, &sSkillTimerData[0].bActive );
	WritePointer( 0x005044F3, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504529, &sSkillTimerData[0].bActive );
	WritePointer( 0x005045DB, &sSkillTimerData[0].bActive );
	WritePointer( 0x0050460D, &sSkillTimerData[0].bActive );
	WritePointer( 0x0050471D, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504750, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504803, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504838, &sSkillTimerData[0].bActive );
	WritePointer( 0x0050489F, &sSkillTimerData[0].bActive );
	WritePointer( 0x005048C1, &sSkillTimerData[0].bActive );
	WritePointer( 0x005048FA, &sSkillTimerData[0].bActive );
	WritePointer( 0x0050496F, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504991, &sSkillTimerData[0].bActive );
	WritePointer( 0x005049CA, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504A47, &sSkillTimerData[0].bActive );
	WritePointer( 0x00504A98, &sSkillTimerData[0].bActive );
	WritePointer( 0x00520FCE, &sSkillTimerData[0].bActive );
	WritePointer( 0x0052100D, &sSkillTimerData[0].bActive );
	WritePointer( 0x005215A3, &sSkillTimerData[0].bActive );
	WritePointer( 0x005215C9, &sSkillTimerData[0].bActive );
	WritePointer( 0x00522393, &sSkillTimerData[0].bActive );
	WritePointer( 0x005223CE, &sSkillTimerData[0].bActive );
	WritePointer( 0x00522442, &sSkillTimerData[0].bActive );
	WritePointer( 0x005224FA, &sSkillTimerData[0].bActive );
	WritePointer( 0x00522865, &sSkillTimerData[0].bActive );

	//Alpha
	WritePointer( 0x00504C16, &sSkillTimerData[0].IconAlpha );
	WritePointer( 0x00522889, &sSkillTimerData[0].IconAlpha );

	//Plus State
	WritePointer( 0x00448EB0, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x00448ED3, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x004BD622, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x004BD753, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x0052293F, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x0052297A, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x00522AAE, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x00522AC5, &sSkillTimerData[0].PlusState[0] );
	WritePointer( 0x00522BC8, &sSkillTimerData[0].PlusState[0] );

	//Party Flag
	WritePointer( 0x005215E3, &sSkillTimerData[0].PartyFlag );
	WritePointer( 0x004BD9DA, &sSkillTimerData[0].PartyFlag );

	//Time
	WritePointer( 0x005215DD, &sSkillTimerData[0].Time[1] );

	//Icon
	WritePointer( 0x0052287D, &sSkillTimerData[0].IconPosiX );
	WritePointer( 0x00522883, &sSkillTimerData[0].IconTime );
	WritePointer( 0x00522877, &sSkillTimerData[0].IconFlag );

	//Weapon
	WritePointer( 0x004BDA49, &sSkillTimerData[0].sSkillInfo.eSkillItemAllowed[0] );
	WritePointer( 0x004BDF90, &sSkillTimerData[0].sSkillInfo.eSkillItemAllowed[0] );
	WritePointer( 0x004BEA65, &sSkillTimerData[0].sSkillInfo.eSkillItemAllowed[0] );
	WritePointer( 0x004C06C9, &sSkillTimerData[0].sSkillInfo.eSkillItemAllowed[0] );

	//Mat Icon
	WritePointer( 0x004ED205, &sSkillTimerData[0].MatIcon );
	WritePointer( 0x004ED272, &sSkillTimerData[0].MatIcon );
	WritePointer( 0x0052107B, &sSkillTimerData[0].MatIcon );
	WritePointer( 0x0052288F, &sSkillTimerData[0].MatIcon );

	//Level
	WritePointer( 0x00448F03, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00448F2D, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BD770, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BD817, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BD854, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BD8C0, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BD98A, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BD9E4, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDA20, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDACA, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDB1A, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDB60, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDBE5, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDBFB, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDC8A, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDCF5, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDD07, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDD24, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDDAC, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDE3F, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDE79, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BDEB8, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BE029, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BE04D, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004BE1D5, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004C03E2, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004C0625, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004EC3B6, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004ED53C, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004ED5CC, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004ED65C, &sSkillTimerData[0].iLevel );
	WritePointer( 0x004ED6EC, &sSkillTimerData[0].iLevel );
	WritePointer( 0x0050453D, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00504623, &sSkillTimerData[0].iLevel );
	WritePointer( 0x0050476A, &sSkillTimerData[0].iLevel );
	WritePointer( 0x0050484C, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00504910, &sSkillTimerData[0].iLevel );
	WritePointer( 0x005049E0, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00504AB6, &sSkillTimerData[0].iLevel );
	WritePointer( 0x005211D0, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00521210, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522990, &sSkillTimerData[0].iLevel );
	WritePointer( 0x005229D6, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522A16, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522A35, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522AE8, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522B00, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522B31, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522B6D, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522BD8, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522C1F, &sSkillTimerData[0].iLevel );
	WritePointer( 0x00522C4B, &sSkillTimerData[0].iLevel );
	WritePointer( 0x04B0BE32, &sSkillTimerData[0].iLevel );
	WritePointer( 0x04B0BF0C, &sSkillTimerData[0].iLevel );


	//Use Time
	WritePointer( 0x004BDCD7, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x004BDDE0, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x004ED51E, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x004ED5AE, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x004ED63E, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x004ED6CE, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x0050451F, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x0050461D, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x00504764, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x0050482E, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x00504904, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x005049D4, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x00504A72, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x00504AA2, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x005223D8, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x0052259C, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x0052269C, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x005226BC, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x005226E1, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x00522727, &sSkillTimerData[0].iUseTime );
	WritePointer( 0x0052286B, &sSkillTimerData[0].iUseTime );

	//Check Time
	WritePointer( 0x004BDCE0, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x004BDDF1, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x005225A5, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x005226A5, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x005226C5, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x005226EA, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x005226F0, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x005226FC, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x00522721, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x0052272D, &sSkillTimerData[0].iCheckTime );
	WritePointer( 0x00522871, &sSkillTimerData[0].iCheckTime );

	//Plus State
	WritePointer( 0x00522E81, &sSkillTimerData[0 - 1].PlusState[0] );
	WritePointer( 0x0052314A, &sSkillTimerData[0 - 1].PlusState[0] );
	WritePointer( 0x00523436, &sSkillTimerData[0 - 1].PlusState[0] );
	WritePointer( 0x005235E0, &sSkillTimerData[0 - 1].PlusState[0] );


	//End Pointer
	WritePointer( 0x0043457C, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x0045387B, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004B96EB, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004BD976, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004BEA95, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004EC3A2, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004EC3E2, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004EC422, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED2A2, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED4EA, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED57A, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED60A, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED69A, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005044EA, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005045D1, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504714, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005047FA, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504896, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504966, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504A3E, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005050A6, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00520C37, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00520FAE, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00522374, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005B4831, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00628F35, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x006291BD, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x006293A3, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00629595, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x0062970F, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x006297F9, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00520FDE, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0BB2F, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0BB5B, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0BCBD, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0C322, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0C56C, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0C5E1, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x04B0C632, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED505, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED595, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED625, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED6B5, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504506, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005045F6, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504730, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504815, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005048B1, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504981, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504A5E, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005215B3, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005223A3, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x005224C4, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004BE337, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004BEE4A, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004C0606, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004C0A61, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00521207, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00521259, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x004ED253, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );
	WritePointer( 0x00504CED, &sSkillTimerData[_countof( sSkillTimerData ) - 1].iID );


	WritePointer( 0x0052110E, &sSkillTimerData[_countof( sSkillTimerData ) - 1] );
	WritePointer( 0x0052115E, &sSkillTimerData[_countof( sSkillTimerData ) - 1] );
	WritePointer( 0x005211AE, &sSkillTimerData[_countof( sSkillTimerData ) - 1] );

	WritePointer( 0x005212B3, &sSkillTimerData[_countof( sSkillTimerData ) - 1] );
	WritePointer( 0x0052130F, &sSkillTimerData[_countof( sSkillTimerData ) - 1] );
	WritePointer( 0x005211AE, &sSkillTimerData[_countof( sSkillTimerData ) - 1] );
}


void CTimerSkill::WritePointer( DWORD Addr, void * ptr )
{
	DWORD dwOld = 0;
	VirtualProtect( (void*)Addr, 8, PAGE_EXECUTE_READWRITE, &dwOld );
	*(DWORD*)Addr = (DWORD)ptr;
}

BOOL CTimerSkill::KillTimer( DWORD dwCode )
{
	for ( int i = 0; i < _countof( sSkillTimerData ); i++ )
	{
		if ( sSkillTimerData[i].bActive && sSkillTimerData[i].iID == dwCode )
		{
			ZeroMemory( &sSkillTimerData[i], sizeof( Skill ) );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTimerSkill::KillTimerQuest( DWORD dwCode, int iQuestID )
{
	for ( int i = 0; i < _countof( sSkillTimerData ); i++ )
	{
		if ( sSkillTimerData[i].bActive && sSkillTimerData[i].iID == dwCode && sSkillTimerData[i].PlusState[4] == iQuestID )
		{
			ZeroMemory( &sSkillTimerData[i], sizeof( Skill ) );
			return TRUE;
		}
	}
	return FALSE;
}

DWORD CTimerSkill::LoadSkillTimerTexture( const char * pszFileName )
{
	typedef DWORD( __cdecl *tfnCreateTexture )(const char * pszFile, int iUnk1, int iUnk2, int iUnk3, int iUnk4, int iBlend);
	tfnCreateTexture fnCreateTexture = (tfnCreateTexture)0x0048A140;
	DWORD dwTexture = fnCreateTexture( pszFileName, 0, 0, 0, 0, 1 );
	CALL( 0x0048A260 );
	return dwTexture;
}

Skill * CTimerSkill::FindSkillTimer( DWORD dwCode )
{
	for ( int i = 0; i < _countof( sSkillTimerData ); i++ )
	{
		if ( sSkillTimerData[i].bActive && sSkillTimerData[i].iID == dwCode )
			return &sSkillTimerData[i];
	}
	return NULL;
}

Skill * CTimerSkill::GetPointerSkillTimer()
{
	return sSkillTimerData;
}

BOOL CTimerSkill::AddSkillTimer( Skill * psSkill )
{
	typedef DWORD( __cdecl *tfnSetSkillBuffer )(Skill * psSkill);
	tfnSetSkillBuffer fnSetSkillBuffer = (tfnSetSkillBuffer)0x00520F60;
	fnSetSkillBuffer( psSkill );
	return TRUE;
}

int CTimerSkill::OnRenderTimer( Skill * psSkill )
{
	int iLine = 0;
	if ( psSkill->iID == TIMERSKILLID_QuestNew || psSkill->iID == TIMERSKILLID_QuestNewT )
	{
		iLine += DAILYQUESTHANDLER->OnRenderTimer( psSkill );
		iLine += WAREHOUSEHANDLER->OnRenderTime( psSkill );
		iLine += CQUESTGAME->OnRenderTimer( psSkill );
	}

	if ( psSkill->iID == SKILLID_PKMode )
		iLine += CHARACTERPVPMODEHANDLER->OnRenderTime( psSkill );

	return iLine;
}

int CTimerSkill::GetArraySize()
{
	return _countof( sSkillTimerData );
}

BOOL CTimerSkill::OnAddChasingHunt( int iTime, int iLevel )
{
	if ( !dwChasingHuntTimerTexture )
		dwChasingHuntTimerTexture = LoadSkillTimerTexture( "game\\images\\skills\\timers\\chasinghunt.tga" );

	Skill * psSkill = TIMERSKILLHANDLER->FindSkillTimer( TIMERSKILLID_ChasingHunt );
	
	if ( psSkill )
		ZeroMemory( psSkill, sizeof( Skill ) );

	if ( !TIMERSKILLHANDLER->FindSkillTimer( TIMERSKILLID_ChasingHunt ) )
	{
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );
		sSkill.iID	= TIMERSKILLID_ChasingHunt;
		sSkill.iUseTime	= iTime;
		sSkill.iLevel	= iLevel;
		sSkill.MatIcon	= dwChasingHuntTimerTexture;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
		PLAYMINISOUND( 25 );
		return TRUE;
	}
	return FALSE;
}

void CTimerSkill::PacketHandlerCrystal( struct PacketCrystalUse * psPacket )
{
	char * pszName = NULL, * pszFile = NULL;

	switch ( psPacket->dwItemID )
	{
		case ITEMID_HopyCrystal:
			pszName = "Hopy Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\hopy_crystal.tga";
			break;
		case ITEMID_HobgoblinCrystal:
			pszName = "Hobgoblin Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\hobgoblin_crystal.tga";
			break;
		case ITEMID_DecoyCrystal:
			pszName = "Decoy Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\Decoy_crystal.tga";
			break;
		case ITEMID_BargonCrystal:
			pszName = "Bargon Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\Bargon_crystal.tga";
			break;
		case ITEMID_HeadCutterCrystal:
			pszName = "Head Cutter Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\headcutter_crystal.tga";
			break;
		case ITEMID_FigonCrystal:
			pszName = "Figon Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\Figon_crystal.tga";
			break;
		case ITEMID_KingHopyCrystal:
			pszName = "King Hopy Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\king_hopy_crystal.tga";
			break;
		case ITEMID_HulkCrystal:
			pszName = "Hulk Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\hulk_crystal.tga";
			break;
		case ITEMID_MysticCrystal:
			pszName = "Mystic Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\mystic_crystal.tga";
			break;
		case ITEMID_GuardianSaintCrystal:
			pszName = "Guardian Saint Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\gsaint_crystal.tga";
			break;
		case ITEMID_WebCrystal:
			pszName = "Web Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\web_crystal.tga";
			break;
		case ITEMID_DarkSpecterCrystal:
			pszName = "Dark Specter Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\dspecter_crystal.tga";
			break;
		case ITEMID_IronGuardCrystal:
			pszName = "Iron Guard Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\iron_guard_crystal.tga";
			break;
		case ITEMID_RicartenMilitiaGuardCrystal:
			pszName = "Ricarten Militia Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\ricarten_militia_crystal.tga";
			break;
		case ITEMID_RicartenGuardCrystal:
			pszName = "Ricarten Guard Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\ricarten_guard_crystal.tga";
			break;
		case ITEMID_RoyalBlessGuardCrystal:
			pszName = "Royal Bless Guard Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\royal_bless_guard_crystal.tga";
			break;
		case ITEMID_OccultCrystal:
			pszName = "Occult Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\occult_crystal.tga";
			break;
		case ITEMID_DoomedCrystal:
			pszName = "Doomed Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\doomed_crystal.tga";
			break;
		case ITEMID_DuskyCrystal:
			pszName = "Dusky Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\dusky_crystal.tga";
			break;
		case ITEMID_ArcticCrystal:
			pszName = "Arctic Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\arctic_crystal.tga";
			break;
		case ITEMID_ValentoCrystal:
			pszName = "Valento Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\valento_crystal.tga";
			break;
		case ITEMID_KelvezuCrystal:
			pszName = "Kelvezu Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\kelvezu_crystal.tga";
			break;
		case ITEMID_TullaCrystal:
			pszName = "Tulla Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\tulla_crystal.tga";
			break;

		case ITEMID_SoulCrystal:
		case ITEMID_SoulSummonCrystal:
			pszName = "Soul Crystal";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\soul_crystal.tga";
			break;

		case SKILLID_HolyIncantation:
			pszName = "Holy Incantation";
			pszFile = "game\\images\\skills\\timers\\holyincantation.tga";
			break;

		case SKILLID_IceElemental:
			pszName = "Ice Elemental";
			pszFile = "game\\images\\skills\\timers\\Pr5_IceElemental.tga";
			break;

		case SKILLID_SummonTiger:
			pszName = "Summon Tiger";
			pszFile = "game\\images\\skills\\timers\\Ata5_SummonTiger.tga";
			break;

		case SKILLID_WizardTrance:
			pszName = "Orb Mastery";
			pszFile = "game\\images\\skills\\timers\\Mg5_OrbMastery.tga";
			break;

		case ITEMID_SnowManCrystal:
			pszName = "Snowman";
			pszFile = "game\\images\\skills\\timers\\monstercrystals\\snowman_crystal.tga";
			break;

		default:
			break;
	}

	if ( pszName )
	{
		DWORD dwTexture = LoadSkillTimerTexture( pszFile );
		Skill sSkill;
		ZeroMemory( &sSkill, sizeof( sSkill ) );

		if ( (psPacket->dwItemID != SKILLID_HolyIncantation) || (psPacket->dwItemID != SKILLID_IceElemental) || (psPacket->dwItemID != SKILLID_SummonTiger) )
		{
			STRINGCOPY( sSkill.sSkillInfo.szName, pszName );
			STRINGCOPY( sSkill.sSkillInfo.szDescription, "A summoned crystal" );
		}

		sSkill.iID		= psPacket->dwItemID;
		sSkill.iUseTime		= psPacket->dwTime;
		if ( (psPacket->dwItemID == SKILLID_HolyIncantation) || (psPacket->dwItemID == SKILLID_IceElemental) || (psPacket->dwItemID == SKILLID_SummonTiger) )
		{
			sSkill.iLevel = SKILLMANAGERHANDLER->GetLevelSkill( psPacket->dwItemID );
		}
		else
			sSkill.iLevel = 11;

		sSkill.PlusState[0] = psPacket->dwObjectID;
		sSkill.MatIcon		= dwTexture;
		TIMERSKILLHANDLER->AddSkillTimer( &sSkill );
		PLAYMINISOUND( 25 );
	}
}

void CTimerSkill::PacketHandlerCrystal( struct PacketCrystalUseDie * psPacket )
{
	for ( int i = 0; i < _countof( sSkillTimerData ); i++ )
	{
		if ( sSkillTimerData[i].bActive && (((sSkillTimerData[i].iID & 0xFFFF0000) == 0x08020000) ||
			(sSkillTimerData[i].iID == SKILLID_HolyIncantation) || (sSkillTimerData[i].iID == SKILLID_IceElemental) ||
			(sSkillTimerData[i].iID == SKILLID_SummonTiger)) )
		{
			if ( sSkillTimerData[i].PlusState[0] == psPacket->dwObjectID )
			{
				ZeroMemory( &sSkillTimerData[i], sizeof( Skill ) );
				break;
			}
		}
	}
}

void CTimerSkill::OnCharacterKilled()
{

	UNITDATA->bDistortion			= FALSE;
	UNITDATA->iUpdateMoveReduce		= 0;
	UNITDATA->iStunTimeLeft			= 0;
	UNITDATA->iStopMotionCounter	= 0;
	UNITDATA->iFreezeTimeLeft		= 0;

	for ( int i = 0; i < _countof( sSkillTimerData ); i++ )
	{
		if ( sSkillTimerData[i].bActive )
		{
			DWORD dwCode = (sSkillTimerData[i].iID & 0xFFFF0000);
			if ( sSkillTimerData[i].iLevel == 11 )
				continue;

			switch ( dwCode )
			{
				//Quests
				case 0x096A0000:
				case 0x096B0000:
					break;

				//Fighter
				case 0x02010000:
				case 0x02020000:
				case 0x02040000:
				case 0x02080000:
				case 0x02100000:
				//Mechanician
				case 0x01010000:
				case 0x01020000:
				case 0x01040000:
				case 0x01080000:
					if ( sSkillTimerData[i].iID == 0x01080004 )
						break;
				case 0x01100000:
				//Archer
				case 0x04010000:
				case 0x04020000:
				case 0x04040000:
				case 0x04080000:
					if ( sSkillTimerData[i].iID == 0x04080001 )
						break;
				case 0x04100000:
				//Pikeman
				case 0x03010000:
				case 0x03020000:
				case 0x03040000:
				case 0x03080000:
				case 0x03100000:
				//Atalanta
				case 0x06010000:
				case 0x06020000:
				case 0x06040000:
				case 0x06080000:
				case 0x06100000:
				//Knight
				case 0x05010000:
				case 0x05020000:
				case 0x05040000:
				case 0x05080000:
				case 0x05100000:
				//Magician
				case 0x08010000:
				case 0x08020000:
				case 0x08040000:
				case 0x08080000:
					if ( sSkillTimerData[i].iID == 0x08080001 )
						break;
				case 0x08100000:
				//Priestess
				case 0x07010000:
				case 0x07020000:
				case 0x07040000:
				case 0x07080000:
				case 0x07100000:
				//Assassin
				case 0x10010000:
				case 0x10020000:
				case 0x10040000:
				case 0x10080000:
				case 0x10100000:
				//Shaman
				case 0x50010000:
				case 0x50020000:
				case 0x50040000:
				case 0x50080000:
				case 0x50100000:
					SKILLMANAGERHANDLER->OnEndSkillTimer( &sSkillTimerData[i] );
					ZeroMemory( &sSkillTimerData[i], sizeof( Skill ) );
					break;
				
				default:
					break;
			}
		}
	}
}

void CTimerSkill::DrawTimer()
{
	typedef void( __cdecl *tfnDrawTexture )(int iMat, int x, int y, int w, int h, DWORD t, int specular);
	tfnDrawTexture fnDrawTexture = (tfnDrawTexture)0x0040A3A0;
	typedef void( __stdcall *tfnDrawSkillBar )(Skill * psSkill, int x, int y, int a);
	tfnDrawSkillBar fnDrawSkillBar = (tfnDrawSkillBar)0x004ECFD0;
	int iSkillCount = 0, iY = 0, iX = 0;
	for ( int i = 0; i < _countof( sTempSkillTimerData ); i++ )
	{
		if ( sTempSkillTimerData[i].bActive )
		{
			if ( sTempSkillTimerData[i].MatIcon )
			{
				if ( i == 0 )
						iX = sTempSkillTimerData[i].IconPosiX;

				if ( !sTempSkillTimerData[i].IconFlag )
				{
					fnDrawTexture( (*(DWORD*)0x0367E06C), iX + 10, iY + 10, 32, 32, sTempSkillTimerData[i].IconAlpha, 0 );
					fnDrawSkillBar( &sTempSkillTimerData[i], iX + 10 + 16, iY + 10 + 16, sTempSkillTimerData[i].IconAlpha );
					fnDrawTexture( sTempSkillTimerData[i].MatIcon, iX + 10, iY + 10, 32, 32, sTempSkillTimerData[i].IconAlpha, 0 );
					
				}

				iSkillCount++;
				iX += 32;

				if ( iSkillCount == 18 )
				{
					iSkillCount = 0;
					iX			= 0;
					iY			+= 32;
				}

			}
		}
	}
}

void CTimerSkill::DrawTimerBox()
{
	int iSkillCount = 0, iY = 0, iX = 0;
	SkillBox TempSkillBox;
	for ( int i = 0; i < _countof( sTempSkillTimerData ); i++ )
	{
		if ( sTempSkillTimerData[i].bActive )
		{
			if ( sTempSkillTimerData[i].MatIcon )
			{
				int iXMouse = MOUSEHANDLER->GetPosition()->iX;
				int iYMouse = MOUSEHANDLER->GetPosition()->iY;
				if ( iXMouse > ((iSkillCount * 32) + 10) && iXMouse < ((iSkillCount * 32) + 10 + 32) )
				{
					if ( (*(WORD*)0x04B0B22A) < 300 && iYMouse > (iY + 10) && iYMouse < (iY + 10 + 32) )
					{
						TempSkillBox.BoxRect.left = 10 + (iSkillCount * 32);
						TempSkillBox.BoxRect.top = 10 + 32 + iY;
						WRITEDWORD( 0x0367DE8C, i + 1 );

						CALLT_WITH_ARG3( 0x004EDC90, 0x0367E148, (DWORD)&sTempSkillTimerData[i], (DWORD)&TempSkillBox, sTempSkillTimerData[i].MatIcon );
						SKILLINFOBOX->PrepareShowSkill( &sTempSkillTimerData[i], TRUE, Rectangle2D( TempSkillBox.BoxRect.left, TempSkillBox.BoxRect.top, TempSkillBox.BoxRect.right, TempSkillBox.BoxRect.bottom ) );
					}
				}
				iSkillCount++;

				if ( iSkillCount == 18 )
				{
					iSkillCount = 0;
					iY			+= 32;
				}
			}
		}
	}
}

void CTimerSkill::SwapSkillTimer( Skill * psSkill )
{
	int iLine = 0;

	for ( int i = 0; i < SKILLREPLACE_MAX_LINES; i++ )
	{
		for ( int j = 0; j < SKILLREPLACE_MAX_COLUMNS; j++ )
		{
			if ( iaSkillReplaceTimer[i][j] )
			{
				if ( iaSkillReplaceTimer[i][j] == psSkill->iID )
				{
					iLine = i + 1;
					break;
				}
			}
		}
	}

	if ( iLine )
	{
		for ( int i = 0; i < 80; i++ )
		{
			if ( sSkillTimerData[i].iID )
			{
				for ( int j = 0; j < SKILLREPLACE_MAX_COLUMNS; j++ )
				{
					if ( sSkillTimerData[i].iID == iaSkillReplaceTimer[iLine - 1][j] )
					{
						if ( iaSkillReplaceTimer[iLine - 1][j] == psSkill->iID )
							continue;

						sSkillTimerData[i].bActive = FALSE;
						sSkillTimerData[i].iUseTime = 0;
						sSkillTimerData[i].iCheckTime = 0;
						sSkillTimerData[i].IconFlag = 0;
						sSkillTimerData[i].IconPosiX = 0;
						sSkillTimerData[i].IconTime = 0;
						sSkillTimerData[i].IconAlpha = 0;
						sSkillTimerData[i].MatIcon = 0;

						ITEMHANDLER->UpdateItemCharacterStatus();

						SKILLMANAGERHANDLER->CancelSkillOnServer( iaSkillReplaceTimer[iLine - 1][j], sSkillTimerData[i].iLevel );
					}
				}
			}
		}
	}
}

void CTimerSkill::Loop()
{//Soon... 005224E0
	for ( int i = 0; i < _countof( sSkillTimerData ); i++ )
	{
		Skill * p = sSkillTimerData + i;

		//Died?
		if ( CHARACTERGAME->GetCurrentHP() == 0 )
		{
			//Can Update End Time?
			if ( p->iID != SKILLID_Force && p->iID != SKILLID_QUEST2 && p->iID != SKILLID_QUEST3 )
			{

			}
		}
	}
}
