#include "stdafx.h"
#include "CLuaParticleOld.h"

CLuaParticleOld::CLuaParticleOld()
{
}

CLuaParticleOld::~CLuaParticleOld()
{
}

NAKED CEffectGroupLua * CLuaParticleOld::Load( const char * pszFileName )
{
	JMP( 0x005C78A0 );
}

NAKED void CEffectUnitFollowLua::Constructor()
{
	JMP( 0x005D31B0 );
}

NAKED void CEffectUnitFollowLua::Init( UnitData * pcUnitData, CEffectGroupLua * pcGroup, float fTime, float fHeight )
{
	JMP( 0x005CA5C0 );
}

NAKED CEffectManagerHandlerLua * CEffectManagerHandlerLua::GetInstance()
{
	JMP( 0x005CCA70 );
}

NAKED void CEffectManagerHandlerLua::AddEffect( CEffectMainBaseLua * pcEffect )
{
	JMP( 0x005CD250 );
}

NAKED bool CEffectManagerHandlerLua::UpdateEffectTime( int iID, UnitData * pcUnitData, float fTime )
{
	JMP( 0x005CBFB0 );
}

void CEffectManagerHandlerLua::KillEffectInside( int iID, int iUnitDataID )
{
	for ( auto pcEffect : GetInstance()->vEffects )
	{
		if ( pcEffect )
		{
			if ( pcEffect->IsActive() )
			{
				if ( pcEffect->GetID() == iID )
				{
					bool bKill = (pcEffect->GetUnitID() == iUnitDataID);

					if ( pcEffect->GetID() == SKILLID_MagneticSphere )
					{
						auto pcUnitData = ((CEffectMainBaseLuaMagneticSphere *)pcEffect)->pcUnitData;

						if ( bKill = (pcUnitData ? (pcUnitData->iID == iUnitDataID ? true : false) : false) )
						{
							((CEffectMainBaseLuaMagneticSphere *)pcEffect)->fEndTime = 1.0f;

							bKill = false;
						}
					}

					if ( bKill )
						pcEffect->Kill();
				}
			}
		}
	}
}

void CEffectManagerHandlerLua::KillEffect( int iID, int iUnitDataID )
{
	CEffectManagerHandlerLua::GetInstance()->KillEffectInside( iID, iUnitDataID );
}


NAKED void CEffectManagerHandlerLua::Clear()
{
	JMP( 0x005CC310 );
}

NAKED void CEffectMainBaseLua::Constructor()
{
	JMP( 0x005D3150 );
}

NAKED void CEffectMainBaseLua::Init( CEffectGroupLua * pcGroup, int iX, int iY, int iZ, int iAngleX, int iAngleY, int iAngleZ )
{
	JMP( 0x005CA4A0 );
}
