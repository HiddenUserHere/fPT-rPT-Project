#include "stdafx.h"
#include "X3DEasings.h"

#include "X3D.h"

namespace X3D
{
float lerpf( float a, float b, float t )
{
	return ((1.f - t) * a) + (t * b);
}

double lerpd( double a, double b, double t )
{
	return ((1. - t) * a) + (t * b);
}

float easeInQuadf( float a, float b, float t )
{
	const float c = b - a;

	return c * (t * t) + a;
}

double easeInQuadd( double a, double b, double t )
{
	const double c = b - a;

	return c * (t * t) + a;
}

float easeOutQuadf( float a, float b, float t )
{
	const float c = b - a;

	return -c * (t * (t - 2.f)) + a;
}

double easeOutQuadd( double a, double b, double t )
{
	const double c = b - a;

	return -c * (t * (t - 2.f)) + a;
}

float easeInOutQuadf( float a, float b, float t )
{
	const float c = b - a;

	t *= 2.f;

	if( t < 1.f )
		return (c/2.f) * (t * t) + a;

	t -= 1.f;

	return (-c/2.f) * (t * (t - 2.f) - 1.f) + a;
}

double easeInOutQuadf( double a, double b, double t )
{
	const double c = b - a;

	t *= 2.;

	if( t < 1. )
		return (c/2.) * (t*t) + a;

	t -= 1.;

	return (-c/2.) * (t * (t - 2.) - 1.) + a;
}

float easeInOutCircf( float a, float b, float t )
{
	const float c = b - a;

	t *= 2.f;

	if( t < 1.f )
		return (-c/2.f) * (sqrt(1.f - (t * t)) - 1.f) + a;

	t -= 2.f;

	return (c/2.f) * (sqrt(1.f - (t * t)) + 1.f) + a;
}

double easeInOutCircd( double a, double b, double t )
{
	const double c = b - a;

	t *= 2.;

	if( t < 1. )
		return (-c/2.) * (sqrt(1. - (t * t)) - 1.) + a;

	t -= 2.;

	return (c/2.) * (sqrt(1. - (t * t)) + 1.) + a;
}

float easeInCircf( float a, float b, float t )
{
	return -b * (sqrt(1.f - (t * t)) - 1.f) + a;
}

double easeInCircd( double a, double b, double t )
{
	return -b * (sqrt(1. - (t * t)) - 1.) + a;
}

float easeOutCircf( float a, float b, float t )
{
	t -= 1.f;

	return ((b-a) * sqrt(1.f - (t * t))) + a;
}

double easeOutCircd( double a, double b, double t )
{
	t -= 1.;

	return ((b-a) * sqrt(1. - (t * t))) + a;
}
}