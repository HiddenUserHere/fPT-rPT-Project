#pragma once

#include "CPoint.h"
#include "ObjectOwner.h"

class CParticleSystem;
class CParticleEmitter;
class CParticleFactory;

class CParticleEvent;
class CParticleModifierGroup;
enum EParticleModifierGroupType;

enum EParticleVisualType
{
	PARTICLEVISUALTYPE_None,
	PARTICLEVISUALTYPE_Billboard,
	PARTICLEVISUALTYPE_BillboardRotation,
	PARTICLEVISUALTYPE_BillboardHorizontal,
	PARTICLEVISUALTYPE_BillboardHorizontalRotation,
	PARTICLEVISUALTYPE_BillboardVertical,
	PARTICLEVISUALTYPE_BillboardVerticalRotation,
};

enum EParticleType
{
	PARTICLETYPE_None,
	PARTICLETYPE_Free,
	PARTICLETYPE_Follow,
};

enum EBlendType;

class CParticle
{
	friend class CParticleSystem;
	friend class CParticleEmitter;
	friend class CParticleFactory;
	friend class CParticleModifierGroup;

public:
	// PRIMITIVES
	struct Int
	{
							  Int() : low(0), high(0), value(0) {}
							  Int( Int & c ) { low = c.low; high = c.high; value = c.value; }
							  Int( int v ) : low(v), high(v), value(v) {}
							  Int( int l, int h) : low(l), high(h), value(0) {}

		void				  Birth() { value = RandomI( low, high ); }

		int					  low;
		int					  high;

		int					  value;

		Int operator		<<( const int i ) { Int r; r.low = low << i; r.high = high << i; r.value = value << i; return r; }
	};

	struct Dbl
	{
							  Dbl() : low(0), high(0), value(0) {}
							  Dbl( Dbl & c ) { low = c.low; high = c.high; value = c.value; }
							  Dbl( double v ) : low(v), high(v), value(v) {}
							  Dbl( double l, double h ) : low(l), high(h), value(0) {}

		void				  Birth() { value = RandomD( low, high ); }

		double				  low;
		double				  high;

		double				  value;

		Dbl operator		* ( const double f ) { Dbl r; r.low = low * f; r.high = high * f; r.value = value * f; return r; }
	};

	// COLOR STRUCTURE
	struct Color
	{
		void				  Birth() { a.Birth(); r.Birth(); g.Birth(); b.Birth(); }

		Int					  a;
		Int					  r;
		Int					  g;
		Int					  b;
	};

	struct Size
	{
							  Size() : bWH(FALSE) {}
							  Size( Size & c ) { bWH = c.bWH; s = c.s; w = c.w; h = c.h; }

		void				  Birth() { if( bWH ) { w.Birth(); h.Birth(); } else { s.Birth(); } }

		BOOL				  bWH;

		Int					  s;
		Int					  w;
		Int					  h;

		int					  GetW() { if( bWH ) { return w.value; } else { return s.value; } }
		int					  GetH() { if( bWH ) { return h.value; } else { return s.value; } }
	};

	struct Point
	{
							  Point() : x(0), y(0), z(0) {}

		void				  Birth() { x.Birth(); y.Birth(); z.Birth(); }

		Int					  x;
		Int					  y;
		Int					  z;

		void operator		+=( const Point & pos ) { x.value += pos.x.value; y.value += pos.y.value; z.value += pos.z.value; }
		Point operator		* ( const double f ) { Point pos; pos.x.value = int(x.value * f); pos.y.value = int(y.value * f); pos.z.value = int(z.value * f); return pos; }
		Point operator		/ ( const double f ) { Point pos; pos.x.value = int(x.value / f); pos.y.value = int(y.value / f); pos.z.value = int(z.value / f); return pos; }
	};

	struct Rotation
	{
							  Rotation() : p(0), y(0), r(0) {}

		void				  Birth() { p.Birth(); y.Birth(); r.Birth(); }

		Dbl					  p;
		Dbl					  y;
		Dbl					  r;
		
		void operator		+=( const Rotation & rot ) { p.value += rot.p.value; y.value += rot.y.value; r.value += rot.r.value; }
		Rotation operator	* ( const double f ) { Rotation rot; rot.p.value = p.value * f; rot.y.value = y.value * f; rot.r.value = r.value * f; return rot; }
	};

	struct Time
	{
		void				  Birth() { t.Birth(); }

		Dbl					  t;

		double				  Get() { return t.value; }
		void				  Set( double f ) { t.value = f; }

		void operator		= ( const Time & p ) { t.low = p.t.low; t.high = p.t.high; t.value = p.t.value; }
		void operator		= ( const double f ) { t.low = f; t.high = f; t.value = f; }
	};

public:
							  CParticle();
							  CParticle( CParticle & c );
	virtual					 ~CParticle();

	virtual CParticle		* Clone();

	virtual void			  Birth();

	virtual BOOL			  Update( double fTime );
	virtual void			  Loop( double fBegin, double fEnd );
	virtual void			  Modify();

	virtual void			  Render();

	void					  SetActive( BOOL b ) { bActive = b; }

	void					  SetName( std::string _str ) { strName = _str; }
	void					  SetNameType( std::string _str ) { strNameType = _str; }

	void					  SetOwner( Engine::ObjectOwner * pNewOwner ) { pOwner = pNewOwner->Clone(); }

	void					  SetVelocity( Point sNewVelocity ) { sVelocity = sNewVelocity; }

	void					  SetColor( Color sNewColor ) { sColor = sNewColor; }
	void					  SetSize( Size sNewSize ) { sSize = sNewSize; }

	BOOL					  IsActive() { return bActive; }

	std::string				  GetName() { return strName; }
	std::string				  GetNameType() { return strNameType; }

	EParticleType			  GetType() { return eType; }
	EParticleVisualType		  GetVisualType() { return eVisualType; }

	EBlendType				  GetSourceBlendMode() { return eSourceBlendMode; }
	
	void					  AddParticleModifierGroup( CParticleModifierGroup * pcParticleModifierGroup );
	CParticleModifierGroup	* GetParticleModifierGroup( EParticleModifierGroupType eType );

protected:
	virtual void			  Apply( double fTime );
	virtual void			  ApplyVelocity( Point & sVelo, double fTime );
	virtual void			  ApplyAngularVelocity( Rotation & sAnguVelo, double fTime );

protected:

	BOOL							  bActive;

	std::string						  strName;
	std::string						  strNameType;

	EParticleType					  eType;
	EParticleVisualType				  eVisualType;

	EBlendType						  eSourceBlendMode;
	UINT							  uTextureID;

	Color							  sColor;
	Size							  sSize;

	Point							  sPosition;
	Point							  sVelocity;

	Rotation						  sRotation;
	Rotation						  sAngularVelocity;

	Time							  sCurLifeTime;
	Time							  sMaxLifeTime;

	BOOL							  bLoop;
	double							  fLoopAt, fLoopTime;

	std::vector<CParticleEvent*>			  vParticleEvents;
	std::vector<CParticleModifierGroup*>	  vParticleModifierGroups;

	Engine::ObjectOwner				* pOwner;

};