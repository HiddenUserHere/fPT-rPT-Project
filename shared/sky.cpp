#include "stdafx.h"
#include "sky.h"

NAKED void Sky::Reset()
{
	JUMP( pfnReset );
}

NAKED void Sky::Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ )
{
	JUMP( pfnRender );
}

NAKED void Sky::Draw( int a, int b, int c, int d, int e, int f )
{
	JUMP( pfnDraw );
}

NAKED void Sky::SetSky( ESkyID iSkyID )
{
	JUMP( pfnSetSky );
}

NAKED void Sky::Update()
{
	JMP( pfnUpdate );
}
