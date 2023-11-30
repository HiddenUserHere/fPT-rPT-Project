#pragma once

#include "X3DEasings.h"

#include "X3DVector2.h"
#include "X3DVector3.h"
#include "X3DVector4.h"
#include "X3DQuaternion.h"
#include "X3DMatrix4.h"

#include "X3DTriangle.h"
#include "X3DQuad.h"
#include "X3DPlane.h"
#include "X3DLine.h"
#include "X3DSphere.h"
#include "X3DAABB.h"
#include "X3DRay.h"
#include "X3DFrustum.h"
#include "X3DCircle.h"
#include "X3DCone.h"

#include "X3DCubicBezierCurve.h"
#include "X3DCubicBezierCurveBuilder.h"
#include "X3DBezierPath.h"

namespace X3D
{

#define PI				3.14159265358979f

#define PIOVER180		(PI / 180.0f)
#define PIUNDER180		(180.0f / PI)

#define RADTODEG(a)		(PIUNDER180 * (float)a)
#define DEGTORAD(a)		(PIOVER180 * (float)a)

#define EPSILON			0.005f
#define ZERO_TOLERANCE	0.00001f

#define cotf(a)			(cosf(a) / sinf(a))

int mini( int a, int b );
float minf( float a, float b );
double mind( double a, double b );

int maxi( int a, int b );
float maxf( float a, float b );
double maxd( double a, double b );

}