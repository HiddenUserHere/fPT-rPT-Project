#include "stdafx.h"
#include "CDropItemAreaHandler.h"

CDropItemAreaHandler::CDropItemAreaHandler()
{
}

CDropItemAreaHandler::~CDropItemAreaHandler()
{
	Clear();
}

void CDropItemAreaHandler::Init()
{
	EXEModel::SetBone( nullptr );
    pcModel = EXEModel::ReadModel( "game\\meshes\\droparea\\circle.ASE" );
    iRadius = 256;
}

void CDropItemAreaHandler::Clear()
{
	for ( auto ps : vCircles )
		DELET( ps );

	vCircles.clear();
}

void CDropItemAreaHandler::AddCircle( int iID, Point3D sPosition, int iRadius, bool bNoTime )
{
	if ( auto psCircle = GetCircle( iID ) )
	{
		psCircle->sPosition = sPosition;
		psCircle->iRadius = iRadius;
		psCircle->fTimeUpdate = 10000.0f;
		psCircle->bNoTime = bNoTime;
	}
	else
	{
		auto psNewCircle = new CircleArea();
		psNewCircle->iID = iID;
		psNewCircle->sPosition = sPosition;
		psNewCircle->iRadius = iRadius;
		psNewCircle->fTimeEase = 1000.0f;
		psNewCircle->fTimeUpdate = 10000.0f;
		psNewCircle->bNoTime = bNoTime;
		vCircles.push_back( psNewCircle );
	}
}

void CDropItemAreaHandler::Render3D()
{
	auto SetRadius = []( int _iRadius )-> void
	{
		RENDERER->iObjectVertexScale = _iRadius;
	};

	auto RenderModel = []( PTModel * _pcModel, Point3D sPosition )-> void
	{
		Point3D sCameraPosition;
		sCameraPosition.iX = UNITTARGETPOSITION_X;
		sCameraPosition.iY = UNITTARGETPOSITION_Y;
		sCameraPosition.iZ = UNITTARGETPOSITION_Z;

		Point3D sCameraAngle;
		sCameraAngle.iX = UNITTARGETANGLE_X;
		sCameraAngle.iY = UNITTARGETANGLE_Y;
		sCameraAngle.iZ = UNITTARGETANGLE_Z;

		_pcModel->SetPosition( &sPosition, &Point3D( 0, 0, 0 ) );
		_pcModel->Render( &sCameraPosition, &sCameraAngle );
	};

	int iAlphaOld = RENDERER->a;
	RENDERER->a = -160;

	int iObjectScale = RENDERER->iObjectVertexScale;

	for ( UINT i = 0; i < pcModel->uMeshCount; i++ )
		pcModel->paMesh[i]->dwUnused = 0x3F;

	RENDERER->bNoClipFaceRender = TRUE;

	DX::cSelectGlow3.SetSizeGlow( 256.0f );

	if ( DX::cSelectGlow3.Begin( D3DCOLOR_ARGB( 255, 255, 215, 0 ), TRUE ) )
	{
		for ( auto ps : vCircles )
		{
			SetRadius( (int)X3D::easeInOutCircf( 1.0f, (float)ps->iRadius, (1000.0f - ps->fTimeEase) / 1000.0f ) * 256 / 10 );
			RenderModel( pcModel, ps->sPosition );
		}

		if ( DX::cSelectGlow3.Begin( D3DCOLOR_ARGB( 180, 255, 215, 0 ), FALSE ) )
		{
			for ( auto ps : vCircles )
			{
				SetRadius( (int)X3D::easeInOutCircf( 1.0f, (float)ps->iRadius, (1000.0f - ps->fTimeEase) / 1000.0f ) * 256 / 10 );
				RenderModel( pcModel, ps->sPosition );
			}

			DX::cSelectGlow3.End( FALSE );
		}

		DX::cSelectGlow3.End( TRUE );
	}

	RENDERER->bNoClipFaceRender = FALSE;

	RENDERER->iObjectVertexScale = iObjectScale;

	RENDERER->a = iAlphaOld;
}

void CDropItemAreaHandler::Update( float fTime )
{
	for ( auto ps : vCircles )
	{
		if ( ps->fTimeUpdate > 0.0f )
		{
			if ( ps->fTimeEase > 0.0f )
			{
				ps->fTimeEase -= fTime;

				if ( ps->fTimeEase <= 0.0f )
					ps->fTimeEase = 0.0f;
			}

			if ( ps->bNoTime == false )
			{
				ps->fTimeUpdate -= fTime;
				if ( ps->fTimeUpdate <= 0.0f )
					ps->fTimeUpdate = 0.0f;
			}
		}
	}

	for ( auto it = vCircles.begin(); it != vCircles.end(); )
	{
		auto ps = (*it);
		if ( ps->fTimeUpdate == 0.0f )
		{
			DELET( ps );
			it = vCircles.erase( it );
		}
		else
			it++;
	}
}

void CDropItemAreaHandler::HandlePacket( PacketItemAreaDrop * psPacket )
{
	for ( int i = 0; i < psPacket->iCount; i++ )
		AddCircle( psPacket->saArea[i].iUserID, psPacket->saArea[i].sPosition, psPacket->saArea[i].iRadius, true );
}

void CDropItemAreaHandler::KillCircle( int iID )
{
	for ( auto it = vCircles.begin(); it != vCircles.end(); )
	{
		auto ps = (*it);
		if ( ps->iID == iID )
		{
			DELET( ps );
			it = vCircles.erase( it );
		}
		else
			it++;
	}
}

CDropItemAreaHandler::CircleArea * CDropItemAreaHandler::GetCircle( int iID )
{
	for ( auto ps : vCircles )
		if ( ps->iID == iID )
			return ps;

	return nullptr;
}
