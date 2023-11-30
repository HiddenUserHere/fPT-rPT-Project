#include "stdafx.h"
#include "EXEPoint.h"

void EXEPoint::SetFromVertex( EXEVertex * pVertex )
{
	psVertex = pVertex;

	rx = pVertex->sPosition.iX;
	ry = pVertex->sPosition.iY;
	rz = pVertex->sPosition.iZ;

	sx = pVertex->sPosition2.iX;
	sy = pVertex->sPosition2.iY;
	sz = pVertex->sPosition2.iZ;
}

void EXEPoint::Move( int iDistX, int iDistY, int iDistZ )
{
	ox = iDistX << 8;
	oy = iDistY << 8;
	oz = iDistZ << 8;

	wx = rx + ox;
	wy = ry + oy;
	wz = rz + oz;
}

void EXEPoint::WorldRotate( int * iTrig )
{
	int i = ( wz * iTrig[2] + wx * iTrig[3] ) >> 8;
	int j = wy;
	int k = ( wz * iTrig[3] - wx * iTrig[2] ) >> 8;

	sPosi.iY = ( j * iTrig[1] - k * iTrig[0] ) >> 8;
	sPosi.iZ = ( j * iTrig[0] + k * iTrig[1] ) >> 8;
	j = sPosi.iY;

	sPosi.iX = ( i * iTrig[5] - j * iTrig[4] ) >> 8;
	sPosi.iY = ( i * iTrig[4] + j * iTrig[5] ) >> 8;

	if( sPosi.iZ > 0 )
	{
		sPosi2D.iX = ( RESOLUTION_WIDTH / 2 ) + ( ( sPosi.iX * (int)READDWORD(0x82BF0C) ) / sPosi.iZ );
		sPosi2D.iY = ( RESOLUTION_HEIGHT / 2 ) - ( ( sPosi.iY * (int)READDWORD( 0x82BF0C ) ) / sPosi.iZ );
	}
	else
	{
		sPosi2D.iX = -32767;
		sPosi2D.iY = -32767;
	}
}