#pragma once

#define MISC_DISTANCE	0x1000
#define MISC_DISTANCE_Y	0x8C

#define fONE		256

class Point2D;
class Point3D;
class Rectangle2D;
class Rectangle3D;

class Point2D
{
public:
	int iX;
	int iY;

						  Point2D() : iX(0), iY(0) {}
						  Point2D( const Point2D & c ) : iX(c.iX), iY(c.iY) {}
						  Point2D( int _iX, int _iY ) : iX(_iX), iY(_iY) {}

	bool				  Inside( const Rectangle2D * psRectangle2D ) const;
	bool				  Inside( const Rectangle2D & sRectangle2D ) const { return Inside( &sRectangle2D ); }
	bool				  Equals( const Point2D * psPoint2D ) const;
	
	Point2D operator	<<( int i );
	Point2D operator	>>( int i );
};

class Point3D
{
public:
	union
	{
		struct
		{
			int iX;
			int iY;
			int iZ;
		};
		struct
		{
			int x;
			int y;
			int z;
		};

	};
						  Point3D() : iX(0), iY(0), iZ(0) {}
						  Point3D( const Point3D & c ) : iX(c.iX), iY(c.iY), iZ(c.iZ) {}
						  Point3D( int _iX, int _iY, int _iZ ) : iX(_iX), iY(_iY), iZ(_iZ) {}

	int					  GetPTDistance( const Point3D & sCompare ) const;
	int					  GetPTDistanceXZ( const Point3D * psCompare, int & a, int & b, int & c ) const;
	int					  GetPTDistanceXZY( const Point3D * psCompare, int & a, int & b, int & c, int & d ) const;

	BOOL				  WithinPTDistance( const Point3D * psCompare, int iDistance, BOOL bFlatY = FALSE ) const;

	int					  GetLength() const;

	Point3D				  GetNormal( Point3D v1 );

	void operator		= ( const Point3D & sOther );

	Point3D operator	<<( int i ) const;
	Point3D operator	>>( int i ) const;
	void operator		<<=( int i );
	void operator		>>=( int i );

	bool operator		==( const Point3D & sOther ) const;
	bool operator		!=( const Point3D & sOther ) const;
	
	void operator		+=( const Point3D & sOther );
	void operator		-=( const Point3D & sOther );

	Point3D operator	- ( const Point3D & sOther ) const;
	Point3D operator	+ ( const Point3D & sOther ) const;
};

class Rectangle2D
{
public:
	int iX;
	int iY;
	int iWidth;
	int iHeight;

						  Rectangle2D() : iX(0), iY(0), iWidth(0), iHeight(0) {}
						  Rectangle2D( const Rectangle2D & c ) : iX(c.iX), iY(c.iY), iWidth(c.iWidth), iHeight(c.iHeight) {}
						  Rectangle2D( int _iX, int _iY ) : iX(_iX), iY(_iY), iWidth(0), iHeight(0) {}
						  Rectangle2D( int _iX, int _iY, int _iWidth, int _iHeight ) : iX(_iX), iY(_iY), iWidth(_iWidth), iHeight(_iHeight) {}

	bool				  Inside( const Point2D * psPosition, int iLowX = 0, int iLowY = 0 ) const;
	bool				  Equals( const Rectangle2D * psRectangle2D ) const;
	void				  CopyFrom( const Rectangle2D * psRectangle2D );
	bool				  Overlaps( const Rectangle2D * psRectangle2D ) const;
	bool				  Contains( const Rectangle2D * psRectangle2D ) const;

	Rectangle2D			  operator+( const Rectangle2D & sOther ) const;
	Rectangle2D			& operator+=( const Rectangle2D & sOther );
};

class Rectangle2DF
{
public:
	float fX;
	float fY;
	float fWidth;
	float fHeight;

						  Rectangle2DF() : fX(0.0f), fY(0.0f), fWidth(0.0f), fHeight(0.0f) {}
						  Rectangle2DF( const Rectangle2DF & c ) { *this = c; }
						  Rectangle2DF( float _fX, float _fY ) : fX(_fX), fY(_fY), fWidth(0), fHeight(0) {}
						  Rectangle2DF( float _fX, float _fY, float _fWidth, float _fHeight ) : fX(_fX), fY(_fY), fWidth(_fWidth), fHeight(_fHeight) {}
};

class Rectangle3D
{
public:
	int iX;
	int iY;
	int iZ;
	int iWidth;
	int iHeight;
	int iDepth;

						  Rectangle3D();
						  Rectangle3D( int x, int y, int z, int w, int h, int d ) : iX(x), iY(y), iZ(z), iWidth(w), iHeight(h), iDepth(d) {}

	bool				  Inside( const Point3D * psPosition ) const;
};

class Vector4DF
{
public:
	float fX;
	float fY;
	float fZ;
	float fW;

	Vector4DF() : fX( 0 ), fY( 0 ), fZ( 0 ), fW( 0 ) {};

	Vector4DF( float x, float y, float z, float w ) : fX( x ), fY( y ), fZ( z ), fW( w ) {};

	float				Dot( const Vector4DF & c ) const;

	void				Normalize();

	void				Scale( float f );

	void				Negative();

	Vector4DF			operator-( const Vector4DF & c ) const;
	Vector4DF			operator+( const Vector4DF & c ) const;
	void				operator=( const Vector4DF & c );
	Vector4DF			operator*( const float c ) const;

};
