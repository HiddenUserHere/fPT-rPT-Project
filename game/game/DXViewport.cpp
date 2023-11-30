#include "stdafx.h"
#include "DXViewport.h"

namespace DX
{

Viewport::Viewport()
{
}

Viewport::Viewport( DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ ) : dwX(dwX), dwY(dwY), dwWidth(dwWidth), dwHeight(dwHeight), fMinZ(fMinZ), fMaxZ(fMaxZ)
{
}

}