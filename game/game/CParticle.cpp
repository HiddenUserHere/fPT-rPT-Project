#include "stdafx.h"
#include "CParticle.h"

#include "CParticleEvent.h"
#include "CParticleModifierGroup.h"

CParticle::CParticle()
{
	strName				= "";
	strNameType			= "";

	eType				= PARTICLETYPE_None;
	eVisualType			= PARTICLEVISUALTYPE_None;

	eSourceBlendMode	= BLENDTYPE_None;
	uTextureID			= 0;

	sCurLifeTime		= 0.0f;
	sMaxLifeTime		= -1.0f;

	bLoop				= FALSE;
	fLoopAt				= 0.0f;
	fLoopTime			= 0.0f;

	pOwner				= NULL;

	bActive				= FALSE;
}

CParticle::CParticle( CParticle & c )
{
	strName				= "";

	eType				= c.eType;
	eVisualType			= c.eVisualType;

	eSourceBlendMode	= c.eSourceBlendMode;
	uTextureID			= c.uTextureID;

	sColor				= c.sColor;
	sSize				= c.sSize;

	sPosition			= c.sPosition;
	sVelocity			= c.sVelocity;

	sRotation			= c.sRotation;
	sAngularVelocity	= c.sAngularVelocity;

	sCurLifeTime		= c.sCurLifeTime;
	sMaxLifeTime		= c.sMaxLifeTime;

	bLoop				= c.bLoop;
	fLoopAt				= c.fLoopAt;
	fLoopTime			= c.fLoopTime;

	for( vector<CParticleEvent*>::iterator it = c.vParticleEvents.begin(); it != c.vParticleEvents.end(); it++ )
	{
		CParticleEvent * pcParticleEvent = *it;

		vParticleEvents.push_back( pcParticleEvent->Clone() );
	}

	for( vector<CParticleModifierGroup*>::iterator it = c.vParticleModifierGroups.begin(); it != c.vParticleModifierGroups.end(); it++ )
	{
		CParticleModifierGroup * pcParticleModifierGroup = *it;

		vParticleModifierGroups.push_back( new CParticleModifierGroup( *pcParticleModifierGroup ) );
	}

	pOwner				= (c.pOwner ? c.pOwner->Clone() : NULL);

	bActive				= FALSE;
}

CParticle::~CParticle()
{
	//WRITEDBG( "~CParticle()" );

	for( vector<CParticleEvent*>::iterator it = vParticleEvents.begin(); it != vParticleEvents.end(); it++ )
	{
		CParticleEvent * pcParticleEvent = *it;

		DELET( pcParticleEvent );
	}
	vParticleEvents.clear();
	
	for( vector<CParticleModifierGroup*>::iterator it = vParticleModifierGroups.begin(); it != vParticleModifierGroups.end(); it++ )
	{
		CParticleModifierGroup * pcParticleModifierGroup = *it;

		DELET( pcParticleModifierGroup );
	}
	vParticleModifierGroups.clear();

	DELET( pOwner );
}

CParticle * CParticle::Clone()
{
	CParticle * pcClone = new CParticle( *this );

	return pcClone;
}

void CParticle::Birth()
{
	sColor.Birth();
	sSize.Birth();

	sPosition.Birth();
	sVelocity.Birth();

	sRotation.Birth();
	sAngularVelocity.Birth();

	sCurLifeTime.Birth();
	sMaxLifeTime.Birth();
}

BOOL CParticle::Update( double fTime )
{
	if( pOwner )
	{
		//Update Owner
		pOwner->Update();
	}

	double fCurLifeTime = sCurLifeTime.t.value;
	double fMaxLifeTime = sMaxLifeTime.t.value;

	double fNewCurLifeTime = fCurLifeTime + fTime;

	//Reached Max Life Time?
	if( fNewCurLifeTime > fMaxLifeTime )
		fNewCurLifeTime = fMaxLifeTime;

	//Looping?
	if( bLoop )
	{
		while( fNewCurLifeTime >= fLoopAt )
		{
			//Perform Updates from the Current Life Time to the Loop At Time
			Loop( fCurLifeTime, fLoopAt );

			fCurLifeTime	= fLoopTime;
			fNewCurLifeTime	= fCurLifeTime + (fNewCurLifeTime - fLoopAt);
		}
	}

	Loop( fCurLifeTime, fNewCurLifeTime );

	fCurLifeTime = fNewCurLifeTime;

	sCurLifeTime.t.value = fCurLifeTime;

	if( fCurLifeTime >= fMaxLifeTime )
		return TRUE;

	//Apply Modifiers
	Modify();

	return FALSE;
}

void CParticle::Loop( double fBegin, double fEnd )
{
	//Loop through Events, must be ordered ascending by time
	for( vector<CParticleEvent*>::iterator it = vParticleEvents.begin(); it != vParticleEvents.end(); it++ )
	{
		CParticleEvent * pcParticleEvent = *it;

		double fEventTime = pcParticleEvent->GetTime();
		if( (fEventTime >= fBegin) && (fEventTime < fEnd) )
		{
			double fTimeTillEvent = fEventTime - fBegin;

			//Apply any time based changes to Particle before applying Event
			Apply( fTimeTillEvent );
			fBegin += fTimeTillEvent;

			//Finally, we apply the changes of the Event to this Particle
			pcParticleEvent->Apply( this );
		}
	}

	//Any leftover time we apply to the Particle here
	Apply( fEnd - fBegin );
}

void CParticle::Modify()
{
	for( vector<CParticleModifierGroup*>::iterator it = vParticleModifierGroups.begin(); it != vParticleModifierGroups.end(); it++ )
	{
		CParticleModifierGroup * pcParticleModifierGroup = *it;

		pcParticleModifierGroup->Apply( this );
	}
}

void CParticle::Render()
{
	EXERender::Billboard bb;

	if( (eType == PARTICLETYPE_Follow) && (pOwner != NULL) )
	{
		CPoint<int> & sOwnerPosition = pOwner->GetPosition();

		bb.x = sOwnerPosition.x + sPosition.x.value;
		bb.y = sOwnerPosition.y + sPosition.y.value;
		bb.z = sOwnerPosition.z + sPosition.z.value;
	}
	else
	{
		bb.x	= sPosition.x.value;
		bb.y	= sPosition.y.value;
		bb.z	= sPosition.z.value;
	}

	bb.w	= sSize.GetW();
	bb.h	= sSize.GetH();

	bb.u	= 0.0f;
	bb.u2	= 1.0f;
	bb.v	= 0.0f;
	bb.v2	= 1.0f;

	bb.tid = uTextureID;

	bb.a	= sColor.a.value;
	bb.r	= sColor.r.value;
	bb.g	= sColor.g.value;
	bb.b	= sColor.b.value;

	switch( eVisualType )
	{
	case PARTICLEVISUALTYPE_Billboard:
		RENDERER->AddBillboard( &bb );
		break;
	case PARTICLEVISUALTYPE_BillboardRotation:
		RENDERER->AddBillboardRotation( &bb, (float)sRotation.p.value, (float)sRotation.y.value, (float)sRotation.r.value );
		break;
	case PARTICLEVISUALTYPE_BillboardHorizontal:
		RENDERER->AddBillboardHorizontal( &bb );
		break;
	case PARTICLEVISUALTYPE_BillboardHorizontalRotation:
		RENDERER->AddBillboardHorizontalRotation( &bb, (float)sRotation.p.value, (float)sRotation.y.value, (float)sRotation.r.value );
		break;
	case PARTICLEVISUALTYPE_BillboardVertical:
		RENDERER->AddBillboardVertical( &bb );
		break;
	case PARTICLEVISUALTYPE_BillboardVerticalRotation:
		RENDERER->AddBillboardVerticalRotation( &bb, (float)sRotation.p.value, (float)sRotation.y.value, (float)sRotation.r.value );
		break;
	}
}

void CParticle::AddParticleModifierGroup( CParticleModifierGroup * pcParticleModifierGroup )
{
	vParticleModifierGroups.push_back( pcParticleModifierGroup );
}

CParticleModifierGroup * CParticle::GetParticleModifierGroup( EParticleModifierGroupType eType )
{
	for( vector<CParticleModifierGroup*>::iterator it = vParticleModifierGroups.begin(); it != vParticleModifierGroups.end(); it++ )
	{
		CParticleModifierGroup * pcParticleModifierGroup = *it;

		if( pcParticleModifierGroup->GetType() == eType )
			return pcParticleModifierGroup;
	}

	return NULL;
}

void CParticle::Apply( double fTime )
{
	if( fTime > 0.0f )
	{
		ApplyVelocity( sVelocity, fTime );
		ApplyAngularVelocity( sAngularVelocity, fTime );
	}
}

void CParticle::ApplyVelocity( Point & sVelo, double fTime )
{
	Point sPos = sVelo * fTime;

	sPos.x.value /= 1000;
	sPos.y.value /= 1000;
	sPos.z.value /= 1000;

	sPosition += sPos;
}

void CParticle::ApplyAngularVelocity( Rotation & sAnguVelo, double fTime )
{
	Rotation sRot = sAnguVelo * fTime;

	sRot.p.value /= 1000;
	sRot.y.value /= 1000;
	sRot.r.value /= 1000;

	sRotation += sRot;
}