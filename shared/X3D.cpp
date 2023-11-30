#include "stdafx.h"
#include "X3D.h"

namespace X3D
{

int mini( int a, int b )
{
	if( a < b )
		return a;

	return b;
}

float minf( float a, float b )
{
	if( a < b )
		return a;

	return b;
}

double mind( double a, double b )
{
	if( a < b )
		return a;

	return b;
}

int maxi( int a, int b )
{
	if( a > b )
		return a;

	return b;
}

float maxf( float a, float b )
{
	if( a > b )
		return a;

	return b;
}

double maxd( double a, double b )
{
	if( a > b )
		return a;

	return b;
}

}