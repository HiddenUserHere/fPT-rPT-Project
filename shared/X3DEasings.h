#pragma once

namespace X3D
{

float lerpf( float a, float b, float t );
double lerpd( double a, double b, double t );

float easeInQuadf( float a, float b, float t );
double easeInQuadd( double a, double b, double t );

float easeOutQuadf( float a, float b, float t );
double easeOutQuadd( double a, double b, double t );

float easeInOutQuadf( float a, float b, float t );
double easeInOutQuadd( double a, double b, double t );

float easeInOutCircf( float a, float b, float t );
double easeInOutCircd( double a, double b, double t );

float easeInCircf( float a, float b, float t );
double easeInCircd( double a, double b, double t );

float easeOutCircf( float a, float b, float t );
double easeOutCircd( double a, double b, double t );

}