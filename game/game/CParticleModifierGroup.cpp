#include "stdafx.h"
#include "CParticleModifierGroup.h"

CParticleModifierGroup::CParticleModifierGroup( EParticleModifierGroupType _eType ) : eType(_eType)
{
	pcLast1		= NULL;
	pcLast2		= NULL;
	iLastDiffP	= -1;
}

CParticleModifierGroup::CParticleModifierGroup( CParticleModifierGroup & c )
{
	eType		= c.eType;

	pcLast1		= NULL;
	pcLast2		= NULL;
	iLastDiffP	= -1;
	
	for( vector<CParticleModifier*>::iterator it = c.vModifiers.begin(); it != c.vModifiers.end(); it++ )
	{
		CParticleModifier * pcParticleModifier = *it;

		vModifiers.push_back( pcParticleModifier->Clone() );
	}
}

CParticleModifierGroup::~CParticleModifierGroup()
{
	for( vector<CParticleModifier*>::iterator it = vModifiers.begin(); it != vModifiers.end(); it++ )
	{
		CParticleModifier * pcParticleModifier = *it;

		DELET( pcParticleModifier );
	}
	vModifiers.clear();
}

void CParticleModifierGroup::Apply( CParticle * pcParticle )
{
	double fCurLifeTime = pcParticle->sCurLifeTime.t.value;

	CParticleModifier * pcMod1 = pcLast1, * pcMod2 = pcLast2;

	if( (pcMod1 == NULL) || (pcMod2 == NULL) || !((fCurLifeTime >= pcMod1->GetTime()) && (fCurLifeTime < pcMod2->GetTime())) )
	{
		for( vector<CParticleModifier*>::iterator it = vModifiers.begin(); it != vModifiers.end(); it++ )
		{
			CParticleModifier * pcParticleModifier = *it;

			if( fCurLifeTime >= pcParticleModifier->GetTime() )
			{
				pcMod1 = pcParticleModifier;
			}
			else
			{
				pcMod2 = pcParticleModifier;
				break;
			}
		}
	}

	if( pcMod1 && pcMod2 )
	{
		//Between two Modifiers
		double fClc = fCurLifeTime - pcMod1->GetTime();
		double fMax = pcMod2->GetTime() - pcMod1->GetTime();
		int iDiffP = (int)((100 * fClc) / fMax);

		if( iDiffP != iLastDiffP )
		{
			pcMod1->Apply( pcParticle, pcMod2, iDiffP );

			iLastDiffP = iDiffP;
		}
	}
	else if( pcMod1 )
	{
		//All Modifiers passed..
		if( pcLast1 != pcMod1 )
		{
			//Apply Final Modifications
			pcMod1->Apply( pcParticle );
		}
	}

	pcLast1 = pcMod1;
	pcLast2 = pcMod2;
}