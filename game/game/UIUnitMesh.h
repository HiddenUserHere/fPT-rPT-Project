#pragma once

#include "UIElement.h"

namespace UI
{
typedef std::shared_ptr<class UnitMesh> UnitMesh_ptr;
class UnitMesh : public Element
{
public:
	//Constructor
							UnitMesh();
							UnitMesh( Rectangle2D sBox );
	virtual				   ~UnitMesh();

	void					PrepareUnitData( UnitData * pcUnitData );

	void					Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY );

	//Getters and Setters
	void					SetAngle( UINT u ){ uAngle = u % 4096; }
	UINT					GetAngle(){ return uAngle; }

	void					SetRenderHead( BOOL b ){ bRenderHead = b; }
	BOOL					GetRenderHead(){ return bRenderHead; }

	void					SetRenderBody( BOOL b ){ bRenderBody = b; }
	BOOL					GetRenderBody(){ return bRenderBody; }

	void					SetRenderWeapons( BOOL b ){ bRenderWeapons = b; }
	BOOL					GetRenderWeapons(){ return bRenderWeapons; }

	void					SetCameraPosition( Point3D s ){ sCameraPosition = s; }
	Point3D					GetCameraPosition(){ return sCameraPosition; }

	void					SetHeadModel( const char * pszHeadModel );
	void					SetBodyModel( const char * pszBodyModel );

	static void				LoadUnitData( UnitData * pcUnitData, const char * pszBodyModel, const char * pszHeadModel );

	void					SetMask( Image * p ){ pcImageMask = p; }

	void					Clear();
	void					FreeUnit();

private:
	UnitData			  * pcUnitDataMesh = NULL;

	Image				  * pcImageMask;

	Point3D					sCameraPosition;
	UINT					uAngle;
	BOOL					bRenderHead;
	BOOL					bRenderWeapons;
	BOOL					bRenderBody;

private:
	void					Animate();

	IMPFNC					pfnLoadUnitDataModels = 0x0043C410;
};
}
