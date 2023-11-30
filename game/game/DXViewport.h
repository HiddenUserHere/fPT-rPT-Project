#pragma once

namespace DX
{
	
typedef std::shared_ptr<class Viewport> Viewport_ptr;

class Viewport
{
	friend class GraphicsDevice;

public:
				  Viewport();
				  Viewport( DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ );

private:
	DWORD		  dwX = 0;
	DWORD		  dwY = 0;
	DWORD		  dwWidth = 800;
	DWORD		  dwHeight = 600;
	float		  fMinZ = 0.0f;
	float		  fMaxZ = 1.0f;
};

}