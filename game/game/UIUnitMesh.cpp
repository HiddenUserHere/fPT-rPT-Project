#include "stdafx.h"
#include "UIUnitMesh.h"

namespace UI
{
UnitMesh::UnitMesh()
{
}

UnitMesh::UnitMesh( Rectangle2D sBox ) : Element(sBox)
{
	uAngle			= 2048;
	bRenderHead		= TRUE;
	bRenderWeapons	= TRUE;
	bRenderBody		= TRUE;
	sCameraPosition = Point3D();
	pcImageMask		= NULL;
	pcUnitDataMesh	= new UnitData;
	ZeroMemory( pcUnitDataMesh, sizeof( UnitData ) );
	pcUnitDataMesh->iID = 0;
}

UnitMesh::~UnitMesh()
{
	if ( pcUnitDataMesh )
		pcUnitDataMesh->Free();

	DELET( pcUnitDataMesh );
}

void UnitMesh::SetHeadModel( const char * pszHeadModel )
{
	LoadUnitData( pcUnitDataMesh, pcUnitDataMesh->sCharacterData.Player.szBodyModel, pszHeadModel );

	pcUnitDataMesh->Animate( ANIMATIONTYPE_Idle );

	if( pcUnitDataMesh->pcHeadMeshContainer )
		pcUnitDataMesh->ChangeAnimationID( 10 );
}

void UnitMesh::SetBodyModel( const char * pszBodyModel )
{
	LoadUnitData( pcUnitDataMesh, pszBodyModel, pcUnitDataMesh->sCharacterData.Player.szHeadModel );

	pcUnitDataMesh->Animate( ANIMATIONTYPE_Idle );

	if( pcUnitDataMesh->pcBodyMeshContainer )
		pcUnitDataMesh->ChangeAnimationID( 10 );
}

NAKED void UnitMesh::LoadUnitData( UnitData * pcUnitData, const char * pszBodyModel, const char * pszHeadModel )
{
	JMP( pfnLoadUnitDataModels );
}

void UnitMesh::Clear()
{
	if ( pcUnitDataMesh )
		pcUnitDataMesh->Free();

	DELET( pcUnitDataMesh );
}

void UnitMesh::FreeUnit()
{
	if ( pcUnitDataMesh )
		pcUnitDataMesh->Free();

	pcUnitDataMesh->iID = 0;
}

void UnitMesh::Animate()
{
}

void UnitMesh::PrepareUnitData( UnitData * pcUnitData )
{
	if( (pcUnitDataMesh->iID - 0x7F000000) == pcUnitData->iID )
		return;

	if ( pcUnitDataMesh->iID != 0 )
		pcUnitDataMesh->Free();

	pcUnitDataMesh->Init();
	pcUnitDataMesh->pcStage = pcUnitData->pcStage;

	LoadUnitData( pcUnitDataMesh, pcUnitData->sCharacterData.Player.szBodyModel, pcUnitData->sCharacterData.Player.szHeadModel );

	pcUnitDataMesh->sCharacterData.iClass = pcUnitData->sCharacterData.iClass;
	pcUnitDataMesh->sCharacterData.sWarpHomeTown = 0;

	pcUnitDataMesh->SetPosition( 0, 0, 0, 0, uAngle, 0 );

	pcUnitDataMesh->iID = pcUnitData->iID + 0x7F000000;

	pcUnitDataMesh->bActive = TRUE;
	pcUnitDataMesh->bVisible = TRUE;

	pcUnitDataMesh->Animate( ANIMATIONTYPE_Idle );

	if( pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player || pcUnitData->iCharacterType == CHARACTERTYPE_Player )
		pcUnitDataMesh->ChangeAnimationID( 10 );

	PTMesh * pcMesh = pcUnitDataMesh->pcBoneContainer->GetMesh( "Bip01 Head" );
	if( pcMesh )
	{
		EXEMatrixI sWorld;
		sWorld = pcMesh->GetWorldMatrix();

		sCameraPosition = Point3D( 0, sWorld.i[3][2] + (2 << 8), 150 << 8 );

		if ( pcMesh->pcNewMesh )
		{
			auto mMatrtix = pcMesh->pcNewMesh->world;
	//		sCameraPosition = Point3D( 0, (int)(mMatrtix.m[3][2] * 256.0f) + (2 << 8), 150 << 8 );
		}
	}
	else
		sCameraPosition = Point3D( 0, pcUnitDataMesh->iModelHeight + (6 << 8), 200 << 8 );
}


void UnitMesh::Render( int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY )
{
	if( pcUnitDataMesh->bVisible == FALSE )
		return;

	int iRenderX = GetX() + iX;
	int iRenderY = GetY() + iY;
	int iRenderWidth = low( GetWidth(), iWidth - GetX() );
	int iRenderHeight = low( GetHeight(), iHeight - GetY() );

	RENDERER->ClearLight();

	//Update Frame of Animation
	pcUnitDataMesh->iFrame += 40;

	//Reset Animation
	if ( pcUnitDataMesh->iFrame > pcUnitDataMesh->iEndFrame )
		pcUnitDataMesh->iFrame = pcUnitDataMesh->iStartFrame;

	static auto renderTarget = Graphics::Graphics::GetInstance()->GetRenderTargetFactory()->Create();

	if( renderTarget && GRAPHICENGINE->GetFrameCamera() )
	{
		if( Graphics::Graphics::GetInstance()->SetRenderTarget( renderTarget ) )
		{
			Graphics::Graphics::GetInstance()->Clear();

			auto backupAmbientColor = Graphics::Graphics::GetInstance()->GetRenderer()->GetAmbientColor();
			GRAPHICENGINE->GetFrameCamera()->SetPosition( Math::Vector3( sCameraPosition.iX / 256.0f, sCameraPosition.iY / 256.0f, sCameraPosition.iZ / 256.0f ), Math::Vector3( sCameraPosition.iX / 256.0f, sCameraPosition.iY / 256.0f, 0.0f ) );

			Graphics::Graphics::GetInstance()->GetRenderer()->SetAmbientColor( Math::Color( 0.0f, 0.0f, 0.0f, 1.0f ) );
			Graphics::Graphics::GetInstance()->GetRenderer()->PushCamera( GRAPHICENGINE->GetFrameCamera() );

			//Render Head of Unit
			if( pcUnitDataMesh->pcHeadMeshContainer && pcUnitDataMesh->pcHeadMeshContainer->pcNewModel && bRenderHead )
			{
				pcUnitDataMesh->sPosi = Point3D( 0, 0, 0 );
				pcUnitDataMesh->pcHeadMeshContainer->SetPosition( &pcUnitDataMesh->sPosi, &Point3D( 0, uAngle, 0 ) );

				if( (pcUnitDataMesh->pcHeadMeshContainer->pcBoneContainer) && (int)pcUnitDataMesh->pcHeadMeshContainer->pcBoneContainer->uMaxFrame > pcUnitDataMesh->iFrame )
					pcUnitDataMesh->pcHeadMeshContainer->uFrame = (UINT)pcUnitDataMesh->iFrame;

				pcUnitDataMesh->pcHeadMeshContainer->pcNewModel->Render( (IO::SMD::FrameInfo *)&pcUnitDataMesh->iStartFrame );
			}

			//Render Body of Unit
			if( pcUnitDataMesh->pcBodyMeshContainer && pcUnitDataMesh->pcBodyMeshContainer->pcNewModel && bRenderBody )
			{
				pcUnitDataMesh->sPosi = Point3D( 0, 0, 0 );
				pcUnitDataMesh->pcBodyMeshContainer->SetPosition( &pcUnitDataMesh->sPosi, &Point3D( 0, uAngle, 0 ) );

				if( (pcUnitDataMesh->pcBodyMeshContainer->pcBoneContainer) && (int)pcUnitDataMesh->pcBodyMeshContainer->pcBoneContainer->uMaxFrame > pcUnitDataMesh->iFrame )
					pcUnitDataMesh->pcBodyMeshContainer->uFrame = (UINT)pcUnitDataMesh->iFrame;

				pcUnitDataMesh->pcBodyMeshContainer->pcNewModel->Render( (IO::SMD::FrameInfo*)&pcUnitDataMesh->iStartFrame );
			}

			Graphics::Graphics::GetInstance()->GetRenderer()->PopCamera();
			Graphics::Graphics::GetInstance()->GetRenderer()->SetAmbientColor( backupAmbientColor );
			Graphics::Graphics::GetInstance()->UnsetRenderTarget();

			if( Graphics::Sprite::Default->Begin( true ) )
			{
				Graphics::Sprite::Default->PushTranslation( Math::Vector2( iRenderX, iRenderY ) );
				Graphics::Sprite::Default->Draw( renderTarget->Texture(), Math::Color::White, Math::RectInt( (RESOLUTION_WIDTH >> 1) - (iRenderWidth >> 1), (RESOLUTION_HEIGHT >> 1) - (iRenderHeight >> 1), iRenderWidth, iRenderHeight ) );
				Graphics::Sprite::Default->End();
				Graphics::Sprite::Default->PopTransform();
			}

			SetCharacterCamera( sCameraPosition.iX, sCameraPosition.iY, sCameraPosition.iZ, 0, 2048, 0 );
		}
	}

	Element::Render( iRenderX, iRenderY, iRenderWidth, iRenderHeight, iSourceX, iSourceY );
}
}