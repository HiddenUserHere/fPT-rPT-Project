#include "stdafx.h"
#include "DX.h"

namespace DX
{

const BOOL NEW_ENGINE = TRUE;

GraphicsDevice_ptr pGraphicsDevice;
SoundDevice_ptr pSoundDevice;
SoundCaptureDevice_ptr pSoundCaptureDevice;

void Init( LPDIRECT3DDEVICE9 lpD3DDevice, CWindow * pWindow )
{
	//Graphics
	{
		pGraphicsDevice = std::make_shared<GraphicsDevice>( lpD3DDevice );
	}

	//Sound
	{
		pSoundDevice = std::make_shared<SoundDevice>();
		pSoundDevice->Init( pWindow );
	}

	//Sound Capture
	{
		pSoundCaptureDevice = std::make_shared<SoundCaptureDevice>();
		pSoundCaptureDevice->Init( pWindow );
	}
}

void Shutdown()
{
	//Sound Capture
	if( pSoundCaptureDevice )
	{
		pSoundCaptureDevice->Shutdown();

		pSoundCaptureDevice = nullptr;
	}

	//Sound
	if( pSoundDevice )
	{
		pSoundDevice->Shutdown();

		pSoundDevice = nullptr;
	}

	//Graphics
	if( pGraphicsDevice )
	{
		pGraphicsDevice = nullptr;
	}
}

void OnLostDevice()
{
	pGraphicsDevice->OnLostDevice();
}

void OnResetDevice()
{
	pGraphicsDevice->OnResetDevice();
}

GraphicsDevice_ptr GetGraphicsDevice()
{
	return pGraphicsDevice;
}

SoundDevice_ptr GetSoundDevice()
{
	return pSoundDevice;
}

SoundCaptureDevice_ptr GetSoundCaptureDevice()
{
	return pSoundCaptureDevice;
}

}

const D3DXMATRIX X3DMatrixToD3DXMatrix( const X3D::Matrix4 & mMatrix )
{
	return D3DXMATRIX( (FLOAT *)mMatrix.m );
}

const X3D::Matrix4 D3DXMatrixToX3DMatrix( const D3DXMATRIX & mMatrix )
{
	return X3D::Matrix4( mMatrix.m );
}

const D3DXVECTOR3 X3DVector3ToD3DXVector3( const X3D::Vector3 & vVector )
{
	return D3DXVECTOR3( vVector.x, vVector.y, vVector.z );
}

const X3D::Vector3 D3DXVector3ToX3DVector3( const D3DXVECTOR3 & vVector )
{
	return X3D::Vector3( vVector.x, vVector.y, vVector.z );
}
