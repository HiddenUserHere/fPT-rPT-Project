#include "stdafx.h"
#include "TestScreen.h"


CTestScreen::CTestScreen()
{
	dwTimeL = 0;
}


CTestScreen::~CTestScreen()
{
}

Delta3D::Graphics::Model * pcModelBone, * pcModel;

BOOL CTestScreen::InitProg()
{
	SMDModelImporter * pcModelImporter = new SMDModelImporter();

	DX::Model_ptr pBone = nullptr;
	if ( pcModelImporter->Load( "char\\monster\\hopy\\hopy.smb" ) )
		pBone = pcModelImporter->Build();

	if ( pcModelImporter->Load( "char\\monster\\hopy\\hopy.smd" ) )
		pTestItem = pcModelImporter->Build( pBone );
/*
	pcModelBone = new Delta3D::Graphics::Model();
	pcModelBone->Load( "char\\monster\\hopy\\hopy.smb" );

	pcModel = new Delta3D::Graphics::Model();
	pcModel->Load( "char\\monster\\hopy\\hopy.smd", pcModelBone );
*/

//	if ( pcModelImporter->Load( "char\\tmABCD\\tmh_B01b.smd" ) )
//		pTestItemHead = pcModelImporter->Build( pBone );

	if ( pTestItem )
		GetGraphicsDevice()->GetModelFactory()->AddModel( pTestItem );

	if ( pTestItemHead )
		GetGraphicsDevice()->GetModelFactory()->AddModel( pTestItemHead );

	delete pcModelImporter;

	//sPositionCamera = Point3D( -700, 205, -17782 );
	sPositionCamera = Point3D( -199900, 400, 199900 );
//	sPointPosition = Point3D( -606, 205, -17882 );
	sPointPosition = Point3D( -200000, 0, 200000 );
	sAngleCamera = Point3D( 0, 0, 0 );
	sPosition = sPointPosition;
	sAngle = Point3D( 0, 5655, 0 );

	CALL( 0x0048A260 );

	//GRAPHICENGINE->GetDeltaRenderer()->PushCamera( new Delta3D::Graphics::Camera() );


	return TRUE;
}

BOOL CTestScreen::Init()
{
	InitProg();

	iFrame = 1;

	return TRUE;
}

void CTestScreen::Shutdown()
{
}

void CTestScreen::SetCamera( Point3D sPosition, Point3D sLookAt )
{
	DX::Lens_ptr pLens = std::make_shared<DX::Lens>();
	pLens->SetFOVY( 45 );
	pLens->SetRatio( (FLOAT)RESOLUTION_WIDTH / (FLOAT)RESOLUTION_HEIGHT );
	pLens->SetZNear( 20.0f );
	pLens->SetZFar( 4000.0f );
	GRAPHICENGINE->GetSRenderer()->SetLens( pLens );

	DX::Camera_ptr pCamera = std::make_shared<DX::Camera>();

	X3D::Vector3 vEye;
	vEye.x = (float)sPosition.iX;
	vEye.y = (float)sPosition.iY;
	vEye.z = (float)sPosition.iZ;
	pCamera->SetEye( vEye );

	X3D::Vector3 vLookAtPT;
	vLookAtPT.x = (float)sLookAt.iX;
	vLookAtPT.y = (float)sLookAt.iY;
	vLookAtPT.z = (float)sLookAt.iZ;
	pCamera->SetLookAt( vLookAtPT );

	X3D::Vector3 vUp;
	vUp.x = 0.0f;
	vUp.y = 1.0f;
	vUp.z = 0.0f;
	pCamera->SetUp( vUp );
	GRAPHICENGINE->GetSRenderer()->SetCamera( pCamera );

	struct eCAMERA_TRACE
	{
		int x, y, z;
		int tx, ty, tz;

		EXEMatrixE	eRotMatrix;
		int AngX, AngY;
	};

	eCAMERA_TRACE eTrace;

	CALL_WITH_ARG7( 0x0048B540, (DWORD)& eTrace, sPosition.iX, sPosition.iY, sPosition.iZ, sLookAt.iX, sLookAt.iY, sLookAt.iZ );
	CALLT_WITH_ARG4( 0x0047F1A0, (DWORD)RENDERER, sPosition.iX, sPosition.iY, sPosition.iZ, (DWORD)& eTrace.eRotMatrix );


	RENDERER->SetCameraPosition( sPosition.iX, sPosition.iY, sPosition.iZ, 0, 0, 0 );

	sAngleCamera.iX = eTrace.AngX;
	sAngleCamera.iY = eTrace.AngY;

	(*(int *)0x00A17520) = sPosition.iX;
	(*(int *)0x00A17510) = sPosition.iY;
	(*(int *)0x00A1619C) = sPosition.iZ;

	(*(int *)0x00824FA8) = eTrace.AngX;
	(*(int *)0x00A17630) = eTrace.AngY;
}


void CTestScreen::Render2D()
{
}

EScreen CTestScreen::Frame()
{
	pTestItem->SetFrame( iFrame % pTestItem->GetMaxFrame() );

//	if ( pTestItemHead )
	//	pTestItemHead->SetFrame( iFrame % pTestItem->GetMaxFrame() );

	//pcModel->SetFrame( iFrame % 1000000 );

	iFrame += 80;

	return SCREEN_Test;
}

void CTestScreen::Update( float fTime )
{
	//PARTICLEENGINE->Update( fTime );

	//pcModel->Update( fTime );
	//pcModelBone->Update( fTime );
}


void CTestScreen::Render3D()
{
	SetCamera( sPositionCamera, sPointPosition );



	int iX = 0;
	int iZ = 0;
	for ( int i = 0; i < 1; i++ )
	{
		auto vM = X3D::Matrix4().scale( X3D::Vector3( 1.0f, 1.0f, 1.0f ) ) *
			X3D::Matrix4().rotateY( PTAngleToDegrees( 2048 ) ) *
			X3D::Matrix4().translate( X3D::Vector3( (float)(sPosition.iX + iX), (float)sPosition.iY, (float)(sPosition.iZ + iZ) ) );

		GRAPHICENGINE->GetSRenderer()->PushWorldMatrix( vM );


		if ( pTestItemHead )
			GRAPHICENGINE->GetSRenderer()->DrawModel( pTestItemHead );

		GRAPHICENGINE->GetSRenderer()->DrawModel( pTestItem );

		GRAPHICENGINE->GetSRenderer()->PopWorldMatrix();

		iX += 30;

		if ( (i % 10) == 0 )
		{
			iX = 0;

			iZ += 30;
		}
	}

    /*

	//GRAPHICENGINE->GetRenderer()->DrawTerrain( pTerrain );

//	GRAPHICENGINE->GetRenderer()->SetLockViewFrustum( TRUE );

	GRAPHICENGINE->GetDeltaRenderer()->GetCamera()->SetProjection( D3DX_PI / 4.4f, (float)RESOLUTION_WIDTH / (float)RESOLUTION_HEIGHT, 8.0f, 2000.0f );
	GRAPHICENGINE->GetDeltaRenderer()->GetCamera()->SetPosition( Delta3D::Math::Vector3( (float)sPositionCamera.iX, (float)sPositionCamera.iY, (float)sPositionCamera.iZ ), Delta3D::Math::Vector3( (float)(sPosition.iX + iX), (float)sPosition.iY, (float)(sPosition.iZ + iZ) ) );

	pcModel->frame = iFrame % 10000;

	pcModel->SetPositionRotation( &Delta3D::Math::Vector3( (float)(sPosition.iX + iX), (float)sPosition.iY, (float)(sPosition.iZ + iZ) ), &Delta3D::Math::Vector3Int( 0, 0, 0 ) );

	pcModel->Render();

	*/

	//GRAPHICENGINE->GetDevice()->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	//pcStage2->Render( sPositionCamera.iX, sPositionCamera.iY, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY, 0 );
//	pcStage->Render( sPositionCamera.iX, sPositionCamera.iY, sPositionCamera.iZ, sAngleCamera.iX, sAngleCamera.iY, 0 );


	int iY = 10;
	
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Object X:        %d [%d]", sPosition.iX, sPosition.iX >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Object Y:        %d [%d]", sPosition.iY, sPosition.iY >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Object Z:        %d [%d]", sPosition.iZ, sPosition.iZ >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Object AX:       %d [%d]", sAngle.iX, sAngle.iX >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Object AY:       %d [%d]", sAngle.iY, sAngle.iY >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Object AZ:       %d [%d]", sAngle.iZ, sAngle.iZ >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Camera X:        %d [%d]", sPositionCamera.iX, sPositionCamera.iX >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Camera Y:        %d [%d]", sPositionCamera.iY, sPositionCamera.iY >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Camera Z:        %d [%d]", sPositionCamera.iZ, sPositionCamera.iZ >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Camera AX:       %d [%d]", sAngleCamera.iX, sAngleCamera.iX >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Camera AY:       %d [%d]", sAngleCamera.iY, sAngleCamera.iY >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );
	iY += 18;
	GetGraphicsDevice()->GetDefaultFont()->Draw( FormatString( "Camera AZ:       %d [%d]", sAngleCamera.iZ, sAngleCamera.iZ >> 8 ), 10, iY, 0, DX::Color( 255, 255, 255 ) );

	CALLT( 0x0047F200, (DWORD)RENDERER );


	(*(UINT*)0x04B0D364) = TRUE;
}

void CTestScreen::OnMouseMove( CMouse * pcMouse )
{
#ifdef TEST_SCREEN

	Point2D * ps = pcMouse->GetPosition();



#endif
}

BOOL CTestScreen::OnMouseClick( CMouse * pcMouse )
{
	return FALSE;
}

BOOL CTestScreen::OnKeyPress( CKeyboard * pcKeyboard )
{
	const int ADD_CAM_POS = 1;

	const int MULTIPLIER_CAM = 10;

	if ( pcKeyboard->GetKey() == 'Q' )
		sPositionCamera.iY -= ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == 'E' )
		sPositionCamera.iY += ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == 'W' )
		sPositionCamera.iZ += ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == 'S' )
		sPositionCamera.iZ -= ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == 'A' )
		sPositionCamera.iX -= ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == 'D' )
		sPositionCamera.iX += ADD_CAM_POS * MULTIPLIER_CAM;


	if ( pcKeyboard->GetKey() == VK_UP )
		sPointPosition.iY += ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == VK_DOWN )
		sPointPosition.iY -= ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == VK_LEFT )
		sPointPosition.iX -= ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == VK_RIGHT )
		sPointPosition.iX += ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == VK_NUMPAD0 )
		sPointPosition.iZ -= ADD_CAM_POS * MULTIPLIER_CAM;

	if ( pcKeyboard->GetKey() == VK_NUMPAD1 )
		sPointPosition.iZ += ADD_CAM_POS * MULTIPLIER_CAM;

	return FALSE;
}

void CTestScreen::DrawShadowUnit()
{
	DX::cSelectGlow1.Begin( D3DCOLOR_XRGB( 0, 255, 0 ), TRUE );
	pcUnitData->Render();
	DX::cSelectGlow1.End( TRUE );
	DX::cSelectGlow1.Begin( D3DCOLOR_XRGB( 0, 255, 0 ), FALSE );
	pcUnitData->Render();
	DX::cSelectGlow1.End( FALSE );

	DX::cSelectGlow1.Draw();
	pcUnitData->Render();
}
