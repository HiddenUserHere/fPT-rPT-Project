#include "DXGraphicEngine.h"
#include "DXGraphicsDevice.h"
#include "DXFont.h"
#include "CSound.h"
#include "DXCamera.h"
#include "DXLogger.h"
#include "DXPostProcess.h"
#include "DXGlow.h"
#include "DXTexture.h"
#include "DXImageSceneRenderer.h"
#include "DXTextureEngine.h"
#include "DXSprite.h"
#include "DXSelectGlow.h"
#include "DXSoundDevice.h"
#include "DXSoundCaptureDevice.h"
#include "DXShadow.h"
#include "DXShadowMap.h"


#define DSFAILED(hr) (hr != DS_OK)

namespace DX
{
	// DXGraphicEngine
	typedef std::shared_ptr<class DXGraphicEngine>	Graphic_ptr;
	typedef class DXGraphicEngine					Graphic;

	// DXFont
	typedef std::shared_ptr<class CSound>			SoundOld_ptr;
	typedef class CSound							SoundOld;
	
	// DXLogger
	typedef class DXLogger							Logger;
	typedef std::shared_ptr<class DXLogger>			Logger_ptr;

	// DXPostProcess
	typedef class DXPostProcess						PostProcess;
	typedef class DXGlow							Glow;

	// DXImageSceneRenderer
	typedef class DXImageSceneRenderer					ImageSceneRenderer;
	typedef std::shared_ptr<class DXImageSceneRenderer>	ImageSceneRenderer_ptr;

	extern const BOOL NEW_ENGINE;
};

const D3DXMATRIX X3DMatrixToD3DXMatrix( const X3D::Matrix4 & mMatrix );
const X3D::Matrix4 D3DXMatrixToX3DMatrix( const D3DXMATRIX & mMatrix );
const D3DXVECTOR3 X3DVector3ToD3DXVector3( const X3D::Vector3 & vVector );
const X3D::Vector3 D3DXVector3ToX3DVector3( const D3DXVECTOR3 & vVector );