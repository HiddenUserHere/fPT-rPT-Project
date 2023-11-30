#pragma once

#include "EXEVertex.h"

class EXEPoint
{
public:
	EXEVertex	  * psVertex;

	int				rx, ry, rz;
	int				wx, wy, wz;
	int				sx, sy, sz;
	int				ox, oy, oz;

	Point3D			sPosi;
	Point2D			sPosi2D;

	BYTE			baClipping2D[4];

	void			SetFromVertex( EXEVertex * pVertex );
	void			Move( int iDistX, int iDistY, int iDistZ );
	void			WorldRotate( int * iTrig );
};