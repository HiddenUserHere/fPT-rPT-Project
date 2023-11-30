#include "stdafx.h"
#include "CParticleModifier.h"

CParticleModifier::CParticleModifier( double f ) : fTime(f)
{
}

CParticleModifier::CParticleModifier( CParticleModifier & c )
{
	fTime = c.fTime;
}

CParticleModifier::~CParticleModifier()
{
	//WRITEDBG( "~CParticleModifier()" );
}

CParticleModifierSize::CParticleModifierSize( CParticle::Size s, double f ) : sSize(s), CParticleModifier( f )
{
	sSize.Birth();
}

CParticleModifierSize::CParticleModifierSize( CParticleModifierSize & c ) : CParticleModifier( c )
{
	sSize = c.sSize;

	sSize.Birth();
}

CParticleModifierSize::~CParticleModifierSize()
{
	//WRITEDBG( "~CParticleModifierSize()" );
}

CParticleModifierSize * CParticleModifierSize::Clone()
{
	CParticleModifierSize * pcClone = new CParticleModifierSize( *this );

	return pcClone;
}

void CParticleModifierSize::Apply( CParticle * pcParticle )
{
	pcParticle->SetSize( sSize );
}

void CParticleModifierSize::Apply( CParticle * pcParticle, CParticleModifier * pcOther, int iDiffP )
{
	CParticleModifierSize * pcOtherSize = (CParticleModifierSize*)pcOther;
	CParticle::Size * psSize = &pcOtherSize->sSize;

	if( iDiffP <= 0 )
	{
		pcParticle->SetSize( sSize );
		return;
	}

	if( iDiffP >= 100 )
	{
		pcParticle->SetSize( *psSize );
		return;
	}

	int s;

	if( sSize.s.value != psSize->s.value )
		s = sSize.s.value + (((pcOtherSize->sSize.s.value - sSize.s.value) * iDiffP) / 100);
	else
		s = sSize.s.value;

	CParticle::Size sNewSize;
	sNewSize.s = s;

	pcParticle->SetSize( sNewSize );
}

CParticleModifierColor::CParticleModifierColor( CParticle::Color c, double f ) : sColor(c), CParticleModifier( f )
{
	sColor.Birth();
}

CParticleModifierColor::CParticleModifierColor( CParticleModifierColor & c ) : CParticleModifier( c )
{
	sColor = c.sColor;
	
	sColor.Birth();
}

CParticleModifierColor::~CParticleModifierColor()
{
	//WRITEDBG( "~CParticleModifierColor()" );
}

CParticleModifierColor * CParticleModifierColor::Clone()
{
	CParticleModifierColor * pcClone = new CParticleModifierColor( *this );

	return pcClone;
}

void CParticleModifierColor::Apply( CParticle * pcParticle )
{
	pcParticle->SetColor( sColor );
}

void CParticleModifierColor::Apply( CParticle * pcParticle, CParticleModifier * pcOther, int iDiffP )
{
	CParticleModifierColor * pcOtherColor = (CParticleModifierColor*)pcOther;
	CParticle::Color * psColor = &pcOtherColor->sColor;

	if( iDiffP <= 0 )
	{
		pcParticle->SetColor( sColor );
		return;
	}

	if( iDiffP >= 100 )
	{
		pcParticle->SetColor( *psColor );
		return;
	}

	int r, g, b, a;

	if( sColor.r.value != psColor->r.value )
		r = sColor.r.value + (((pcOtherColor->sColor.r.value - sColor.r.value) * iDiffP) / 100);
	else
		r = sColor.r.value;
	
	if( sColor.g.value != psColor->g.value )
		g = sColor.g.value + (((pcOtherColor->sColor.g.value - sColor.g.value) * iDiffP) / 100);
	else
		g = sColor.g.value;
	
	if( sColor.b.value != psColor->b.value )
		b = sColor.b.value + (((pcOtherColor->sColor.b.value - sColor.b.value) * iDiffP) / 100);
	else
		b = sColor.b.value;
	
	if( sColor.a.value != psColor->a.value )
		a = sColor.a.value + (((pcOtherColor->sColor.a.value - sColor.a.value) * iDiffP) / 100);
	else
		a = sColor.a.value;

	CParticle::Color sNewColor;
	sNewColor.r = r;
	sNewColor.g = g;
	sNewColor.b = b;
	sNewColor.a = a;

	pcParticle->SetColor( sNewColor );
}