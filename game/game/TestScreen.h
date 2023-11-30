#pragma once

#include "Screen.h"

#include "SMDDXModelImporter.h"
#include "SMDDXTerrainImporter.h"

class CTestScreen : public CScreen
{
public:

	UnitData						* pcUnitData = NULL;
	UnitData						* pcUnitData2 = NULL;
	Stage * pcStage = NULL;
	Stage * pcStage2 = NULL;

	DWORD							dwMap = 0;

	int								iBeginFrame;
	int								iEndFrame;

	DWORD							dwTime = 0;
	DWORD							dwTimeL = 0;
	BOOL							bUpLogin = FALSE;
	BOOL							bRightLogin = FALSE;

	int								iFrame;

	Point3D							sPosition;
	Point3D							sAngle;
	Point3D							sPositionCamera;
	Point3D							sPositionNew;
	Point3D							sPointPosition;
	Point3D							sAngleCamera;


	int								iOldXMouse = 0;
	int								iOldYMouse = 0;

	PTModelCache					* pcModelCache;

	DX::IndexBuffer_ptr				pcIndexBuffer;
	DX::VertexBuffer_ptr			pcVertexBuffer;
	DX::VertexDeclaration_ptr		pcVertexDeclaration;

	DX::RenderTarget_ptr			pRenderGlow1;
	DX::RenderTarget_ptr			pRenderGlow2;

	DX::Model_ptr					pTestItem = nullptr;
	DX::Model_ptr					pTestItemHead = nullptr;
	DX::Terrain_ptr					pTerrain = nullptr;

public:
	CTestScreen();
	virtual ~CTestScreen();

	BOOL				InitProg();

	BOOL				Init();
	void				Shutdown();

	void				SetCamera( Point3D sPosition, Point3D sLookAt );

	void				SetCamera( Point3D sPositionCamera, Point3D sPositionObject, Point3D sLookAt );

	void				Render3D();
	void				Render2D();

	EScreen				Frame();

	void				Update( float fTime );

	void				OnMouseMove( class CMouse * pcMouse );
	BOOL				OnMouseClick( class CMouse * pcMouse );
	BOOL				OnKeyPress( class CKeyboard * pcKeyboard );

	void				DrawShadowUnit();
};

