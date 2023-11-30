#include "stdafx.h"
#include "point.h"

bool Point2D::Inside( const Rectangle2D * psRectangle2D ) const
{
	if( (iX >= psRectangle2D->iX) && (iX < (psRectangle2D->iX + psRectangle2D->iWidth)) )
		if( (iY >= psRectangle2D->iY) && (iY < (psRectangle2D->iY + psRectangle2D->iHeight)) )
			return true;

	return false;
}

bool Point2D::Equals( const Point2D * psPoint2D ) const
{
	if( (iX == psPoint2D->iX) && (iY == psPoint2D->iY) )
		return true;

	return false;
}

Point2D Point2D::operator<<( int i )
{
	Point2D s;
	s.iX = iX << i;
	s.iY = iY << i;

	return s;
}

Point2D Point2D::operator>>( int i )
{
	Point2D s;
	s.iX = iX >> i;
	s.iY = iY >> i;

	return s;
}

int Point3D::GetPTDistance( const Point3D & sCompare ) const
{
	int iXD = (sCompare.iX - iX) >> 8;
	int iZD = (sCompare.iZ - iZ) >> 8;
	int iYD = (sCompare.iY - iY) >> 8;

	iXD *= iXD;
	iZD *= iZD;
	iYD *= iYD;

	return (iXD + iZD + iYD);
}

int Point3D::GetPTDistanceXZ( const Point3D * psCompare, int & a, int & b, int & c ) const
{
	int iXD = (psCompare->iX - iX) >> 8;
	int iZD = (psCompare->iZ - iZ) >> 8;

	int iXDM = iXD * iXD;
	int iZDM = iZD * iZD;

	a = (iXDM + iZDM);
	b = abs( iXD );
	c = abs( iZD );

	return a;
}

int Point3D::GetPTDistanceXZY( const Point3D * psCompare, int & a, int & b, int & c, int & d ) const
{
	int iXD = (psCompare->iX - iX) >> 8;
	int iZD = (psCompare->iZ - iZ) >> 8;
	int iYD = (psCompare->iY - iY) >> 8;
	
	int iXDM = iXD * iXD;
	int iZDM = iZD * iZD;
	int iYDM = iYD * iYD;

	a = (iXDM + iZDM + iYDM);
	b = abs( iXD );
	c = abs( iZD );
	d = abs( iYD );

	return a;
}

BOOL Point3D::WithinPTDistance( const Point3D * psCompare, int iDistance, BOOL bFlatY ) const
{
	int iDistanceA, iDistanceB, iDistanceC, iDistanceD;
	GetPTDistanceXZY( psCompare, iDistanceA, iDistanceB, iDistanceC, iDistanceD );

	if( bFlatY )
	{
		if( (iDistanceA < (iDistance * iDistance)) && (iDistanceB < MISC_DISTANCE) && (iDistanceC < MISC_DISTANCE) && (iDistanceD < (MISC_DISTANCE_Y >> 1)) )
			return TRUE;
	}
	else
	{
		if( (iDistanceA < (iDistance * iDistance)) && (iDistanceB < MISC_DISTANCE) && (iDistanceC < MISC_DISTANCE) && (iDistanceD < MISC_DISTANCE) )
			return TRUE;
	}

	return FALSE;
}

int Point3D::GetLength() const
{
	return (int)sqrt((iX * iX) + (iY * iY) + (iZ * iZ));
}

Point3D Point3D::GetNormal( Point3D v1 )
{
	const Point3D v0( iX, iY, iZ );

	Point3D s;

	//Cross product
	s.iX = (v0.iY * v1.iZ) - (v0.iZ * v1.iY);
	s.iY = (v0.iZ * v1.iX) - (v0.iX * v1.iZ);
	s.iZ = (v0.iX * v1.iY) - (v0.iY * v1.iX);

	//Dot
	int i = s.GetLength();
	i += (i == 0 ? 1 : 0);
	

	//Normalize
	s.iX /= i;
	s.iY /= i;
	s.iZ /= i;

	return s;
}

void Point3D::operator=( const Point3D & sOther )
{
	this->iX	= sOther.iX;
	this->iY	= sOther.iY;
	this->iZ	= sOther.iZ;
}

Point3D Point3D::operator<<( int i ) const
{
	Point3D s;
	s.iX = iX << i;
	s.iY = iY << i;
	s.iZ = iZ << i;

	return s;
}

Point3D Point3D::operator>>( int i ) const
{
	Point3D s;
	s.iX = iX >> i;
	s.iY = iY >> i;
	s.iZ = iZ >> i;

	return s;
}

void Point3D::operator<<=( int i )
{
	iX <<= i;
	iY <<= i;
	iZ <<= i;
}

void Point3D::operator>>=( int i )
{
	iX >>= i;
	iY >>= i;
	iZ >>= i;
}

bool Point3D::operator==( const Point3D & sOther ) const
{
	if( this->iX == sOther.iX )
		if( this->iY == sOther.iY )
			if( this->iZ == sOther.iZ )
				return true;

	return false;
}

bool Point3D::operator!=( const Point3D & sOther ) const
{
	return !(*this == sOther);
}

void Point3D::operator+=( const Point3D & sOther )
{
	this->iX += sOther.iX;
	this->iY += sOther.iY;
	this->iZ += sOther.iZ;
}

void Point3D::operator-=( const Point3D & sOther )
{
	this->iX -= sOther.iX;
	this->iY -= sOther.iY;
	this->iZ -= sOther.iZ;
}

Point3D Point3D::operator-( const Point3D & sOther ) const
{
	Point3D sReturn( iX, iY, iZ );
	sReturn -= sOther;
	return sReturn;
}

Point3D Point3D::operator+( const Point3D & sOther ) const
{
	Point3D sReturn( iX, iY, iZ );
	sReturn += sOther;
	return sReturn;
}

bool Rectangle2D::Inside( const Point2D * psPosition, int iLowX, int iLowY ) const
{
	if( ((psPosition->iX - iLowX) >= iX) && ((psPosition->iX - iLowX) < (iX + iWidth)) )
		if( ((psPosition->iY - iLowY) >= iY) && ((psPosition->iY - iLowY) < (iY + iHeight)) )
			return true;

	return false;
}

bool Rectangle2D::Equals( const Rectangle2D * psRectangle2D ) const
{
	if( (iX == psRectangle2D->iX) && (iY == psRectangle2D->iY) )
		if( (iWidth == psRectangle2D->iWidth) && (iHeight == psRectangle2D->iHeight) )
			return true;

	return false;
}

void Rectangle2D::CopyFrom( const Rectangle2D * psRectangle2D )
{
	iX = psRectangle2D->iX;
	iY = psRectangle2D->iY;
	iWidth = psRectangle2D->iWidth;
	iHeight = psRectangle2D->iHeight;
}

bool Rectangle2D::Overlaps( const Rectangle2D * psRectangle2D ) const
{
	int iX11 = iX;
	int iX12 = iX + iWidth;
	int iY11 = iY;
	int iY12 = iY + iHeight;
	int iX21 = psRectangle2D->iX;
	int iX22 = psRectangle2D->iX + psRectangle2D->iWidth;
	int iY21 = psRectangle2D->iY;
	int iY22 = psRectangle2D->iY + psRectangle2D->iHeight;
	
    bool noOverlap = iX11 >= iX22 ||
                     iX21 >= iX12 ||
                     iY11 >= iY22 ||
                     iY21 >= iY12;

	return !noOverlap;
}

bool Rectangle2D::Contains( const Rectangle2D * psRectangle2D ) const
{
	if( (psRectangle2D->iX >= iX) && (psRectangle2D->iX + psRectangle2D->iWidth) <= (iX + iWidth) )
		if( (psRectangle2D->iY >= iY) && (psRectangle2D->iY + psRectangle2D->iHeight) <= (iY + iHeight) )
			return true;

	return false;
}

Rectangle2D Rectangle2D::operator+( const Rectangle2D & sOther ) const
{
	Rectangle2D sReturn;

	sReturn.iX			= iX + sOther.iX;
	sReturn.iY			= iY + sOther.iY;
	sReturn.iWidth		= high( iWidth, sOther.iWidth );
	sReturn.iHeight		= high( iHeight, sOther.iHeight );

	return sReturn;
}

Rectangle2D & Rectangle2D::operator+=( const Rectangle2D & sOther )
{
	Rectangle2D sReturn( iX, iY, iWidth, iHeight );

	sReturn.iX		+= sOther.iX;
	sReturn.iY		+= sOther.iY;
	sReturn.iWidth	+= sOther.iWidth;
	sReturn.iHeight	+= sOther.iHeight;

	return sReturn;
}

bool Rectangle3D::Inside( const Point3D * psPosition ) const
{
	if( (psPosition->iX >= iX) && (psPosition->iX < (iX + iWidth)) )
		if( (psPosition->iY >= iY) && (psPosition->iY < (iY + iHeight)) )
			if( (psPosition->iZ >= iZ) && (psPosition->iZ < (iZ + iDepth)) )
				return true;

	return false;
}

float Vector4DF::Dot( const Vector4DF & c ) const
{
	float fReturn = fX * c.fX;
	fReturn += fY * c.fY;
	fReturn += fZ * c.fZ;
	fReturn += fW * c.fW;

	return fReturn;
}

void Vector4DF::Normalize()
{
	float f = (float)sqrt( (fX * fX) + (fY * fY) + (fZ * fZ) + (fW * fW) );

	float inverse = 1.0f / f;

	if ( inverse != 0.0f )
	{
		fX *= inverse;
		fY *= inverse;
		fZ *= inverse;
		fW *= inverse;
	}
}

void Vector4DF::Scale( float f )
{
	fX *= f;
	fY *= f;
	fZ *= f;
	fW *= f;
}

void Vector4DF::Negative()
{
	fX = -fX;
	fY = -fY;
	fZ = -fZ;
	fW = -fW;
}

Vector4DF Vector4DF::operator-( const Vector4DF & c ) const
{
	Vector4DF s;

	s.fX = fX - c.fX;
	s.fY = fY - c.fY;
	s.fZ = fZ - c.fZ;
	s.fW = fW - c.fW;

	return s;
}

Vector4DF Vector4DF::operator+( const Vector4DF & c ) const
{
	Vector4DF s;

	s.fX = fX + c.fX;
	s.fY = fY + c.fY;
	s.fZ = fZ + c.fZ;
	s.fW = fW + c.fW;

	return s;
}

void Vector4DF::operator=( const Vector4DF & c )
{
	this->fX = c.fX;
	this->fY = c.fY;
	this->fZ = c.fZ;
	this->fW = c.fW;
}

Vector4DF Vector4DF::operator*( const float c ) const
{
	Vector4DF s;

	s.fX = fX * c;
	s.fY = fY * c;
	s.fZ = fZ * c;
	s.fW = fW * c;

	return s;
}
