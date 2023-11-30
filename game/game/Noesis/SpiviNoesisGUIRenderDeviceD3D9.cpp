/*
-----------------------------------------------------------------------------
This source file is part of spivi runtime.
For the latest info, see http://www.spiviTech.com

Copyright (c) SpiviTech Ltd
-----------------------------------------------------------------------------
*/

#include "stdafx.h"

#ifdef _NOESIS_GUI
#include "SpiviNoesisGUIRenderDeviceD3D9.h"
#include "SpiviNoesisGUIRenderTargetD3D9.h"
#include "SpiviNoesisGUITextureD3D9.h"
#include "SpiviNoesisGUIShadersD3D9.hlsl.h"

#include <DxErr.h>

#pragma comment (lib, "DxErr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#ifndef DYNAMIC_VB_SIZE
#define DYNAMIC_VB_SIZE 512 * 1024
#endif

#ifndef DYNAMIC_IB_SIZE
#define DYNAMIC_IB_SIZE 128 * 1024
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

const UINT VFPos = 0;
const UINT VFColor = 1 << 0;
const UINT VFTex0 = 1 << 1;
const UINT VFTex1 = 1 << 2;
const UINT VFCoverage = 1 << 3;


struct Program
{
	int8_t vShaderIdx;
	int8_t pShaderIdx;
};

// Map from batch shader-ID to vertex and pixel shader objects
const Program Programs[] =
{
	{ 0, 0 },    // RGBA
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ 0, 1 },    // Mask
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ 1, 2 },    // PathSolid
	{ 2, 3 },    // PathLinear
	{ 2, 4 },    // PathRadial
	{ 2, 5 },    // PathPattern
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ 3, 6 },    // PathAASolid
	{ 4, 7 },    // PathAALinear
	{ 4, 8 },    // PathAARadial
	{ 4, 9 },    // PathAAPattern
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ 5, 10 },   // ImagePaintOpacitySolid
	{ 6, 11 },   // ImagePaintOpacityLinear
	{ 6, 12 },   // ImagePaintOpacityRadial
	{ 6, 13 },   // ImagePaintOpacityPattern
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ -1, -1 },
	{ 5, 14 },   // TextSolid
	{ 6, 15 },   // TextLinear
	{ 6, 16 },   // TextRadial
	{ 6, 17 },   // TextPattern
};

namespace Spivi
{
	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::NoesisGUIRenderDeviceD3D9(IDirect3DDevice9* pDeviceD3D9) : m_pDevice(pDeviceD3D9)
	{
		m_pStateBlock = NULL;
		m_pColorBuffer = NULL;
		m_pDepthBuffer = NULL;
		m_oVertexBuffer.pVertexBuffer = NULL;
		m_oIndexBuffer.pIndexBuffer = NULL;

		configureRenderDeviceCaps();
		createDynamicVertexBuffer();
		createDynamicIndexBuffer();
		createShaders();
		createVertexDeclarations();
		createStateBlock();
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::~NoesisGUIRenderDeviceD3D9()
	{
		releaseRenderSurfaces();
		releaseStateBlock();
		releaseVertexDeclarations();
		releaseShaders();
		releaseDynamicVertexBuffer();
		releaseDynamicIndexBuffer();
	}
	
	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::checkOperationResult(HRESULT hr, const char* lpszMethod, const char* lpszOperationName)
	{
		if (FAILED(hr))
		{
			//CString strErrorMessage = DXGetErrorDescription(hr); 
			//OutCStringStream os; 
			//spivi_log_error(os << "NoesisGUIRenderDeviceD3D9::" << lpszMethod << " Failed on: " << lpszOperationName << ". Error desc: " << strErrorMessage);
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::configureRenderDeviceCaps()
	{
		m_pDevice->GetDeviceCaps(&m_oDeviceCaps);

		m_oCaps = {};

		m_oCaps.centerPixelOffset = 0.5f;
		m_oCaps.dynamicVerticesSize = DYNAMIC_VB_SIZE;
		m_oCaps.dynamicIndicesSize = DYNAMIC_IB_SIZE;
		m_oCaps.linearRendering = false;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createDynamicVertexBuffer()
	{
		HRESULT hr = m_pDevice->CreateVertexBuffer(DYNAMIC_VB_SIZE, 
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_DEFAULT, 
			&m_oVertexBuffer.pVertexBuffer, 
			NULL);

		checkOperationResult(hr, __FUNCTION__, "CreateVertexBuffer");

		m_oVertexBuffer.pos = 0;
		m_oVertexBuffer.drawPos = 0;
		m_oVertexBuffer.size = DYNAMIC_VB_SIZE;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::releaseDynamicVertexBuffer()
	{
		SAFE_RELEASE(m_oVertexBuffer.pVertexBuffer);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createDynamicIndexBuffer()
	{
		HRESULT hr = m_pDevice->CreateIndexBuffer(DYNAMIC_IB_SIZE, 
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16, 
			D3DPOOL_DEFAULT,
			&m_oIndexBuffer.pIndexBuffer,
			NULL);

		checkOperationResult(hr, __FUNCTION__, "CreateIndexBuffer");

		m_oIndexBuffer.pos = 0;
		m_oIndexBuffer.drawPos = 0;
		m_oIndexBuffer.size = DYNAMIC_IB_SIZE;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::releaseDynamicIndexBuffer()
	{
		SAFE_RELEASE(m_oIndexBuffer.pIndexBuffer);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createShaders()
	{
		const char* aVertexShaders[] = 
		{
			"Pos_VS",
			"PosColor_VS",
			"PosTex0_VS",
			"PosColorCoverage_VS",
			"PosTex0Coverage_VS",
			"PosColorTex1_VS",
			"PosTex0Tex1_VS",
		};

		const char* aPixelShaders[] =
		{
			"RGBA_FS",
			"MASK_FS",
			
			"PathSolid_FS",
			"PathLinear_FS", 
			"PathRadial_FS",
			"PathPattern_FS",
			"PathAASolid_FS",
			"PathAALinear_FS",
			"PathAARadial_FS",
			"PathAAPattern_FS",
			
			"ImagePaintOpacitySolid_FS",
			"ImagePaintOpacityLinear_FS",
			"ImagePaintOpacityRadial_FS",
			"ImagePaintOpacityPattern_FS",
			
			"TextSolid_FS",
			"TextLinear_FS",
			"TextRadial_FS",
			"TextPattern_FS",
		};

		// Create the vertex shaders.
		for (UINT nIndex=0; nIndex < _countof(aVertexShaders); ++nIndex)
		{
			if (compileShader(aVertexShaders[nIndex], "vs_3_0", &m_aVertexShaders[nIndex]))
			{
				createVertexShader(&m_aVertexShaders[nIndex]);
			}
		}

		// Create the pixel shaders.
		for (UINT nIndex = 0; nIndex < _countof(aPixelShaders); ++nIndex)
		{
			if (compileShader(aPixelShaders[nIndex], "ps_3_0", &m_aPixelShaders[nIndex]))
			{
				createPixelShader(&m_aPixelShaders[nIndex]);
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::releaseShaders()
	{
		// Release the vertex shaders.
		for (UINT nIndex = 0; nIndex < _countof(m_aVertexShaders); ++nIndex)
		{
			SAFE_RELEASE(m_aVertexShaders[nIndex].lpMicroCode);
			SAFE_RELEASE(m_aVertexShaders[nIndex].lpConstantTable);
			SAFE_RELEASE(m_aVertexShaders[nIndex].pVertexShader);
		}

		// Release the pixel shaders.
		for (UINT nIndex = 0; nIndex < _countof(m_aPixelShaders); ++nIndex)
		{
			SAFE_RELEASE(m_aPixelShaders[nIndex].lpMicroCode);
			SAFE_RELEASE(m_aPixelShaders[nIndex].lpConstantTable);
			SAFE_RELEASE(m_aPixelShaders[nIndex].pPixelShader);
		}
	}


	//-----------------------------------------------------------------------------
	bool NoesisGUIRenderDeviceD3D9::compileShader(const char* lpszEntryPoint, const char* lpszProfile, BaseShader* pShader)
	{
		DWORD dwCompileFlags = D3DXSHADER_PACKMATRIX_COLUMNMAJOR;
		LPCTSTR lpszSource = reinterpret_cast<LPCTSTR>(SpiviNoesisGUIShadersD3D9_hlsl);
		LPD3DXBUFFER pErrorMessages;
		HRESULT hr;

		hr = D3DXCompileShader(lpszSource, sizeof(SpiviNoesisGUIShadersD3D9_hlsl),
			NULL, NULL,
			lpszEntryPoint,
			lpszProfile,
			dwCompileFlags,
			&pShader->lpMicroCode,
			&pErrorMessages,
			&pShader->lpConstantTable);

		checkOperationResult(hr, __FUNCTION__, "D3DXCompileShader");

		if (FAILED(hr))
		{
			const char* lpszError = static_cast<const char*>(pErrorMessages->GetBufferPointer());
			//spivi_log_error(lpszError);
		}
			
		SAFE_RELEASE(pErrorMessages);

		return SUCCEEDED(hr);
	}


	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createVertexShader(VertexShader* pShader)
	{
		//assert(pShader->lpMicroCode != nullptr);
		//assert(pShader->lpConstantTable != nullptr);

		HRESULT hr;

		// Create the shader interface.
		hr = m_pDevice->CreateVertexShader(static_cast<DWORD*>(pShader->lpMicroCode->GetBufferPointer()), &pShader->pVertexShader);
		checkOperationResult(hr, __FUNCTION__, "CreateVertexShader");

		// Resolve constants.
		pShader->hProjMatrix = pShader->lpConstantTable->GetConstantByName(NULL, "$uProjectionMtx");
		//assert(pShader->hProjMatrix != NULL);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createPixelShader(PixelShader* pShader)
	{
		//assert(pShader->lpMicroCode != nullptr);
		//assert(pShader->lpConstantTable != nullptr);

		HRESULT hr;

		// Create the shader interface.
		hr = m_pDevice->CreatePixelShader(static_cast<DWORD*>(pShader->lpMicroCode->GetBufferPointer()), &pShader->pPixelShader);
		checkOperationResult(hr, __FUNCTION__, "CreatePixelShader");

		// Resolve constants.
		pShader->hColor = pShader->lpConstantTable->GetConstantByName(NULL, "$uColor");
		pShader->hOpacity = pShader->lpConstantTable->GetConstantByName(NULL, "$uOpacity");
		pShader->hRadialGrad = pShader->lpConstantTable->GetConstantByName(NULL, "$uRadialGrad");
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setShaders(const Noesis::Batch& batch)
	{
		const Program& rkShaderPrograms = Programs[batch.shader.v];

		setVertexShader(batch, &m_aVertexShaders[rkShaderPrograms.vShaderIdx]);
		setPixelShader(batch, &m_aPixelShaders[rkShaderPrograms.pShaderIdx]);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setVertexShader(const Noesis::Batch& batch, VertexShader* pShader)
	{
		HRESULT hr;

		// Case source shader changed.
		if (m_pCurVertexShader != pShader)
		{
			hr = m_pDevice->SetVertexShader(pShader->pVertexShader);
			checkOperationResult(hr, __FUNCTION__, "SetVertexShader");
			m_pCurVertexShader = pShader;
		}
		
		// Case constant buffer changed.
		if (batch.projMtxHash != m_nVertexCBHash)
		{
			m_nVertexCBHash = batch.projMtxHash;

			float aValues[16] = { 0.0f };

			memcpy(aValues, batch.projMtx, 16 * sizeof(float));

			hr = pShader->lpConstantTable->SetFloatArray(m_pDevice, pShader->hProjMatrix, aValues, 16);
			checkOperationResult(hr, __FUNCTION__, "SetFloatArray");
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setPixelShader(const Noesis::Batch & batch, PixelShader * pShader)
	{
		HRESULT hr;

		// Case source shader changed.
		if (m_pCurPixelShader != pShader)
		{
			hr = m_pDevice->SetPixelShader(pShader->pPixelShader);
			checkOperationResult(hr, __FUNCTION__, "SetPixelShader");
			m_pCurPixelShader = pShader;
		}

		// Case constant buffer changed.
		if (batch.rgba != nullptr || batch.opacity != nullptr || batch.radialGrad != nullptr)
		{
			UINT nPixelCBHash = batch.rgbaHash ^ batch.radialGradHash ^ batch.opacityHash;
			
			if (m_nPixelCBHash != nPixelCBHash)
			{
				m_nPixelCBHash = nPixelCBHash;

				// Update the color constant.
				if (batch.rgba != nullptr)
				{
					float aColor[4];

					memcpy(aColor, batch.rgba, 4 * sizeof(float));

					hr = pShader->lpConstantTable->SetFloatArray(m_pDevice, pShader->hColor, aColor, 4);
					checkOperationResult(hr, __FUNCTION__, "SetFloatArray");
				}

				// Update the opacity constant.
				if (batch.opacity != nullptr)
				{
					hr = pShader->lpConstantTable->SetFloat(m_pDevice, pShader->hOpacity, *batch.opacity);
					checkOperationResult(hr, __FUNCTION__, "SetFloatArray");
				}

				// Update the radial grad constant.
				if (batch.radialGrad != nullptr)
				{
					float aRadialGrad[8];

					memcpy(aRadialGrad, batch.radialGrad, 8 * sizeof(float));

					hr = pShader->lpConstantTable->SetFloatArray(m_pDevice, pShader->hRadialGrad, aRadialGrad, 8);
					checkOperationResult(hr, __FUNCTION__, "SetFloatArray");
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createVertexDeclarations()
	{
		UINT aVertexFormats[] = 
		{
			VFPos,
			VFPos | VFColor,
			VFPos | VFTex0,
			VFPos | VFColor | VFCoverage,
			VFPos | VFTex0 | VFCoverage,
			VFPos | VFColor | VFTex1,
			VFPos | VFTex0 | VFTex1
		};

		for (UINT nIndex=0; nIndex < _countof(aVertexFormats); ++nIndex)
		{
			createVertexDeclaration(aVertexFormats[nIndex], &m_aVertexDecls[nIndex]);
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createVertexDeclaration(const UINT nFlags, VertexDeclaration* pVertexDecl)
	{
		D3DVERTEXELEMENT9 aVertexElements[5 + 1];
		UINT nElementIndex = 0;
		WORD wOffset = 0;

		// The position element.
		aVertexElements[nElementIndex].Stream = 0;
		aVertexElements[nElementIndex].Method = D3DDECLMETHOD_DEFAULT;
		aVertexElements[nElementIndex].Offset = wOffset;
		aVertexElements[nElementIndex].Type = D3DDECLTYPE_FLOAT2;
		aVertexElements[nElementIndex].Usage = D3DDECLUSAGE_POSITION;
		aVertexElements[nElementIndex].UsageIndex = 0;
		wOffset += 8;
		nElementIndex++;

		// Add color component.
		if (nFlags & VFColor)
		{
			aVertexElements[nElementIndex].Stream = 0;
			aVertexElements[nElementIndex].Method = D3DDECLMETHOD_DEFAULT;
			aVertexElements[nElementIndex].Offset = wOffset;
			aVertexElements[nElementIndex].Type = D3DDECLTYPE_D3DCOLOR;
			aVertexElements[nElementIndex].Usage = D3DDECLUSAGE_COLOR;
			aVertexElements[nElementIndex].UsageIndex = 0;
			wOffset += 4;
			nElementIndex++;
		}

		// Add Tex0 component.
		if (nFlags & VFTex0)
		{
			aVertexElements[nElementIndex].Stream = 0;
			aVertexElements[nElementIndex].Method = D3DDECLMETHOD_DEFAULT;
			aVertexElements[nElementIndex].Offset = wOffset;
			aVertexElements[nElementIndex].Type = D3DDECLTYPE_FLOAT2;
			aVertexElements[nElementIndex].Usage = D3DDECLUSAGE_TEXCOORD;
			aVertexElements[nElementIndex].UsageIndex = 0;
			wOffset += 8;
			nElementIndex++;
		}

		// Add Tex1 component.
		if (nFlags & VFTex1)
		{
			aVertexElements[nElementIndex].Stream = 0;
			aVertexElements[nElementIndex].Method = D3DDECLMETHOD_DEFAULT;
			aVertexElements[nElementIndex].Offset = wOffset;
			aVertexElements[nElementIndex].Type = D3DDECLTYPE_FLOAT2;
			aVertexElements[nElementIndex].Usage = D3DDECLUSAGE_TEXCOORD;
			aVertexElements[nElementIndex].UsageIndex = 1;
			wOffset += 8;
			nElementIndex++;
		}

		// Add Coverage component.
		if (nFlags & VFCoverage)
		{
			aVertexElements[nElementIndex].Stream = 0;
			aVertexElements[nElementIndex].Method = D3DDECLMETHOD_DEFAULT;
			aVertexElements[nElementIndex].Offset = wOffset;
			aVertexElements[nElementIndex].Type = D3DDECLTYPE_FLOAT1;
			aVertexElements[nElementIndex].Usage = D3DDECLUSAGE_TEXCOORD;
			aVertexElements[nElementIndex].UsageIndex = 2;
			wOffset += 4;
			nElementIndex++;
		}

		aVertexElements[nElementIndex].Stream = 0xff;
		aVertexElements[nElementIndex].Method = 0;
		aVertexElements[nElementIndex].Offset = 0;
		aVertexElements[nElementIndex].Type = D3DDECLTYPE_UNUSED;
		aVertexElements[nElementIndex].Usage = 0;
		aVertexElements[nElementIndex].UsageIndex = 0;

		pVertexDecl->nStride = wOffset;

		HRESULT hr = m_pDevice->CreateVertexDeclaration(aVertexElements, &pVertexDecl->pVertexDeclaration);
		checkOperationResult(hr, __FUNCTION__, "CreateVertexDeclaration");
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::releaseVertexDeclarations()
	{
		for (UINT nIndex = 0; nIndex < _countof(m_aVertexDecls); ++nIndex)
		{
			SAFE_RELEASE(m_aVertexDecls[nIndex].pVertexDeclaration);
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setVertexStageResources(const Noesis::Batch & batch)
	{
		const Program& rkShaderPrograms = Programs[batch.shader.v];

		setVertexDeclaration(&m_aVertexDecls[rkShaderPrograms.vShaderIdx]);

		HRESULT hr;
		const unsigned int stride = m_aVertexDecls[rkShaderPrograms.vShaderIdx].nStride;
		const unsigned int offset = m_oVertexBuffer.drawPos + batch.vertexOffset;

		hr = m_pDevice->SetStreamSource(0, m_oVertexBuffer.pVertexBuffer, offset, stride);
		checkOperationResult(hr, __FUNCTION__, "SetStreamSource");

		//assert(offset + (stride * batch.numVertices) <= m_oVertexBuffer.size);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setVertexDeclaration(VertexDeclaration* pVertexDecl)
	{
		HRESULT hr;

		// Case source shader changed.
		if (m_pCurVertexDecl != pVertexDecl)
		{
			hr = m_pDevice->SetVertexDeclaration(pVertexDecl->pVertexDeclaration);
			checkOperationResult(hr, __FUNCTION__, "SetVertexDeclaration");
			m_pCurVertexDecl = pVertexDecl;
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setRenderState(const Noesis::RenderState& rkRenderState, uint8_t uStencilRef)
	{	
		// Update fill mode state.
		setFillModeRenderState(rkRenderState);
		
		// Update scissor test state.
		setScissorRenderState(rkRenderState);

		// Update stencil mode state.
		setStencilModeRenderState(rkRenderState, uStencilRef);
		
		// Update blend state.
		setBlendModeRenderState(rkRenderState);

		// Update color write state.
		setColorWriteRenderState(rkRenderState);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setFillModeRenderState(const Noesis::RenderState& rkRenderState)
	{
		if (m_oCurRenderState.f.wireframe != rkRenderState.f.wireframe)
		{
			m_oCurRenderState.f.wireframe = rkRenderState.f.wireframe;

			HRESULT hr;

			if (rkRenderState.f.wireframe)
			{
				hr = m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{
				hr = m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_FILLMODE");
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setScissorRenderState(const Noesis::RenderState& rkRenderState)
	{
		if (m_oCurRenderState.f.scissorEnable != rkRenderState.f.scissorEnable)
		{
			m_oCurRenderState.f.scissorEnable = rkRenderState.f.scissorEnable;

			HRESULT hr;

			if (rkRenderState.f.scissorEnable)
			{
				hr = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			}
			else
			{
				hr = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			}
			checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SCISSORTESTENABLE");
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setStencilModeRenderState(const Noesis::RenderState& rkRenderState, uint8_t uStencilRef)
	{
		if (m_oCurRenderState.f.stencilMode != rkRenderState.f.stencilMode || m_uCurStencilRef != uStencilRef)
		{
			m_oCurRenderState.f.stencilMode = rkRenderState.f.stencilMode;
			m_uCurStencilRef = uStencilRef;

			HRESULT hr;

			// Stencil disabled.
			if (rkRenderState.f.stencilMode == Noesis::StencilMode::Disabled)
			{
				hr = m_pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILENABLE");
			}

			// Stencil enabled.
			else
			{
				// Enable stencil test.
				hr = m_pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILENABLE");

				// Setup stencil ref value.
				hr = m_pDevice->SetRenderState(D3DRS_STENCILREF, uStencilRef);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILREF");

				// Configure stencil pass op -> Keep.
				if (rkRenderState.f.stencilMode == Noesis::StencilMode::Equal_Keep)
				{
					hr = m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
					checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILPASS");
				}

				// Configure stencil pass op -> Increase.
				else if (rkRenderState.f.stencilMode == Noesis::StencilMode::Equal_Incr)
				{
					hr = m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
					checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILPASS");
				}

				// Configure stencil pass op -> Decrease.
				else if (rkRenderState.f.stencilMode == Noesis::StencilMode::Equal_Decr)
				{
					hr = m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
					checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILPASS");
				}				
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setBlendModeRenderState(const Noesis::RenderState& rkRenderState)
	{
		if (m_oCurRenderState.f.blendMode != rkRenderState.f.blendMode)
		{
			m_oCurRenderState.f.blendMode = rkRenderState.f.blendMode;

			HRESULT hr;

			if (rkRenderState.f.blendMode == Noesis::BlendMode::Src)
			{
				hr = m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_ALPHABLENDENABLE");
			}
			else if (rkRenderState.f.blendMode == Noesis::BlendMode::SrcOver)
			{
				hr = m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_ALPHABLENDENABLE");
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setColorWriteRenderState(const Noesis::RenderState& rkRenderState)
	{
		if (m_oCurRenderState.f.colorEnable != rkRenderState.f.colorEnable)
		{
			m_oCurRenderState.f.colorEnable = rkRenderState.f.colorEnable;

			HRESULT hr;

			if (rkRenderState.f.colorEnable)
			{
				hr = m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_COLORWRITEENABLE");
			}
			else
			{
				hr = m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
				checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_COLORWRITEENABLE");
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setTextures(const Noesis::Batch & batch)
	{
		// Apply pattern texture.
		if (batch.pattern != nullptr)
		{
			setTexture(0, batch.pattern);
			setSampleState(0, batch.patternSampler);
		}

		// Apply ramps texture.
		if (batch.ramps != nullptr)
		{
			setTexture(0, batch.ramps);
			setSampleState(0, batch.rampsSampler);
		}

		// Apply image texture.
		if (batch.image != nullptr)
		{
			setTexture(1, batch.image);
			setSampleState(1, batch.imageSampler);
		}

		// Apply glyphs texture.
		if (batch.glyphs != nullptr)
		{
			setTexture(1, batch.glyphs);
			setSampleState(1, batch.glyphsSampler);
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setTexture(UINT nStage, Noesis::Texture* pTexture)
	{
		NoesisGUITextureD3D9* pTextureImpl = (NoesisGUITextureD3D9*)(pTexture);
		HRESULT hr;

		hr = m_pDevice->SetTexture(nStage, pTextureImpl->GetTexture());
		checkOperationResult(hr, __FUNCTION__, "SetTexture");
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::setSampleState(UINT nStage, const Noesis::SamplerState& rkSampleState)
	{
		HRESULT hr;

		// Setup min filter.
		hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_MINFILTER, rkSampleState.f.minmagFilter == Noesis::MinMagFilter::Nearest ? D3DTEXF_POINT : D3DTEXF_LINEAR);
		checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_MINFILTER");

		// Setup mag filter.
		hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_MAGFILTER, rkSampleState.f.minmagFilter == Noesis::MinMagFilter::Nearest ? D3DTEXF_POINT : D3DTEXF_LINEAR);
		checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_MAGFILTER");

		// Setup mip filter.
		if (rkSampleState.f.mipFilter == Noesis::MipFilter::Disabled)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_MIPFILTER");
		}
		else if (rkSampleState.f.mipFilter == Noesis::MipFilter::Nearest)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_MIPFILTER");
		}
		else if (rkSampleState.f.mipFilter == Noesis::MipFilter::Linear)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_MIPFILTER");
		}

		// Setup wrap modes.
		// Clamp to edge - texture coordinates are clamped to range [0.0 1.0]
		if (rkSampleState.f.wrapMode == Noesis::WrapMode::ClampToEdge)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSU");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSV");
		}

		// Clamp to zero - texture coordinates out of the [0.0 1.0] range are set to the given border color.
		else if (rkSampleState.f.wrapMode == Noesis::WrapMode::ClampToZero)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSU");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSV");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_BORDERCOLOR, 0x00000000);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_BORDERCOLOR");
		}

		// Repeat - texture coordinates are wrapped around the range [0.0 1.0].
		else if (rkSampleState.f.wrapMode == Noesis::WrapMode::Repeat)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSU");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSV");
		}

		// Mirror U.
		else if (rkSampleState.f.wrapMode == Noesis::WrapMode::MirrorU)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSU");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSV");
		}

		// Mirror V.
		else if (rkSampleState.f.wrapMode == Noesis::WrapMode::MirrorV)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSU");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSV");
		}

		// Mirror.
		else if (rkSampleState.f.wrapMode == Noesis::WrapMode::Mirror)
		{
			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSU");

			hr = m_pDevice->SetSamplerState(nStage, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
			checkOperationResult(hr, __FUNCTION__, "SetSamplerState - D3DSAMP_ADDRESSV");
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::captureRenderState()
	{
		if (m_pStateBlock != NULL)
		{
			m_pStateBlock->Capture();
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::restoreRenderState()
	{
		if (m_pStateBlock != NULL)
		{
			m_pStateBlock->Apply();
		}
	}

	//-----------------------------------------------------------------------------
	bool NoesisGUIRenderDeviceD3D9::canRender() const
	{
		HRESULT hr;

		hr = m_pDevice->TestCooperativeLevel();

		return SUCCEEDED(hr);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::handleDeviceLost()
	{		
		releaseStateBlock();
		releaseRenderSurfaces();	
		releaseDynamicVertexBuffer();
		releaseDynamicIndexBuffer();

		// Release render target resources.
		for (UINT nIndex=0; nIndex < m_aRenderTargets.size(); ++nIndex)
		{
			NoesisGUIRenderTargetD3D9* pRenderTarget = static_cast<NoesisGUIRenderTargetD3D9*>(m_aRenderTargets[nIndex]);
			NoesisGUITextureD3D9* pTexture = static_cast<NoesisGUITextureD3D9*>(pRenderTarget->GetTexture());

			pRenderTarget->releaseResources();
			pTexture->releaseResources();
		}

		// Release external texture resources.
		for (UINT nIndex = 0; nIndex < NoesisGUITextureD3D9::s_Instances.size(); ++nIndex)
		{
			NoesisGUITextureD3D9* pTexture = NoesisGUITextureD3D9::s_Instances[nIndex];
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::handleDeviceReset()
	{
		createStateBlock();
		createDynamicVertexBuffer();
		createDynamicIndexBuffer();

		// Re-create render target resources.
		for (UINT nIndex = 0; nIndex < m_aRenderTargets.size(); ++nIndex)
		{
			NoesisGUIRenderTargetD3D9* pRenderTarget = static_cast<NoesisGUIRenderTargetD3D9*>(m_aRenderTargets[nIndex]);
			NoesisGUITextureD3D9* pTexture = static_cast<NoesisGUITextureD3D9*>(pRenderTarget->GetTexture());
			IDirect3DTexture9* pTextureImpl = nullptr;
			IDirect3DSurface9* pDepthStencilSurfaceImpl = nullptr;

			createRenderTargetResources(pRenderTarget->GetWidth(), pRenderTarget->GetHeight(), pRenderTarget->GetSampleCount(), &pTextureImpl, &pDepthStencilSurfaceImpl);

			pRenderTarget->acquireResources(pDepthStencilSurfaceImpl);
			pTexture->acquireResources(pTextureImpl);
		}

		// Re-acquire external texture resources.
		for (UINT nIndex = 0; nIndex < NoesisGUITextureD3D9::s_Instances.size(); ++nIndex)
		{
			NoesisGUITextureD3D9* pTexture = NoesisGUITextureD3D9::s_Instances[nIndex];
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createStateBlock()
	{
		releaseStateBlock();

		m_pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::releaseStateBlock()
	{
		if (m_pStateBlock != NULL)
		{
			m_pStateBlock->Release();
			m_pStateBlock = NULL;
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::captureRenderSurfaces()
	{
		IDirect3DSurface9* renderTarget = NULL;
		IDirect3DSurface9* depthBuffer = NULL;

		IDirect3DDevice9* pDeviceD3D9 = m_pDevice;

		pDeviceD3D9->GetRenderTarget(0, &renderTarget);		
		pDeviceD3D9->GetDepthStencilSurface(&depthBuffer);

		if (renderTarget != NULL && depthBuffer != NULL)
		{
			// update render targets
			if (m_pColorBuffer != renderTarget || m_pDepthBuffer != depthBuffer)
			{
				releaseRenderSurfaces();
				
				m_pColorBuffer = renderTarget;
				m_pColorBuffer->AddRef();

				m_pDepthBuffer = depthBuffer;
				m_pDepthBuffer->AddRef();				
			}
		}

		if (renderTarget != NULL)
		{
			renderTarget->Release();
			renderTarget = NULL;
		}

		if (depthBuffer != NULL)
		{
			depthBuffer->Release();
			depthBuffer = NULL;
		}		
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::restoreRenderSurfaces()
	{		
		IDirect3DDevice9* pDeviceD3D9 = m_pDevice;

		pDeviceD3D9->SetRenderTarget(0, m_pColorBuffer);		
		pDeviceD3D9->SetDepthStencilSurface(m_pDepthBuffer);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::releaseRenderSurfaces()
	{
		if (m_pColorBuffer != NULL)
		{
			m_pColorBuffer->Release();
			m_pColorBuffer = NULL;
		}

		if (m_pDepthBuffer != NULL)
		{
			m_pDepthBuffer->Release();
			m_pDepthBuffer = NULL;
		}
	}

	//-----------------------------------------------------------------------------
	const Noesis::DeviceCaps & NoesisGUIRenderDeviceD3D9::GetCaps() const
	{
		return m_oCaps;
	}

	//-----------------------------------------------------------------------------
	Noesis::Ptr<Noesis::RenderTarget> NoesisGUIRenderDeviceD3D9::CreateRenderTarget(const char* label, UINT width, UINT height, UINT sampleCount)
	{
		Noesis::Ptr<Noesis::RenderTarget> pRenderTarget;
		IDirect3DTexture9* pTextureImpl = nullptr;
		IDirect3DSurface9* pDepthStencilSurfaceImpl = nullptr;

		// Create the required resources.
		createRenderTargetResources(width, height, sampleCount, &pTextureImpl, &pDepthStencilSurfaceImpl);

		// Case resources successfully created -> create the render target interface.
		if (pTextureImpl != nullptr && pDepthStencilSurfaceImpl != nullptr)
		{
			pRenderTarget = createRenderTarget(label, width, height, sampleCount, pTextureImpl, pDepthStencilSurfaceImpl);
		}

		return pRenderTarget;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::createRenderTargetResources(UINT width, UINT height, UINT sampleCount, IDirect3DTexture9** ppTextureImpl, IDirect3DSurface9** ppDepthStencilSurfaceImpl)
	{
		HRESULT hr;

		// Create the target color buffer.
		if (ppTextureImpl != nullptr)
		{
			hr = m_pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ppTextureImpl, NULL);
			checkOperationResult(hr, __FUNCTION__, "CreateTexture");
		}
	
		// Create the target depth stencil buffer.
		if (ppDepthStencilSurfaceImpl != nullptr)
		{
			D3DMULTISAMPLE_TYPE eMSType = D3DMULTISAMPLE_NONE;

			// Create the depth-stencil buffer.
			hr = m_pDevice->CreateDepthStencilSurface(width, height, D3DFMT_D24S8, eMSType, 0, TRUE, ppDepthStencilSurfaceImpl, NULL);
			if (FAILED(hr))
			{
				hr = m_pDevice->CreateDepthStencilSurface(width, height, D3DFMT_D24FS8, eMSType, 0, TRUE, ppDepthStencilSurfaceImpl, NULL);
			}
			if (FAILED(hr))
			{
				hr = m_pDevice->CreateDepthStencilSurface(width, height, D3DFMT_D24X8, eMSType, 0, TRUE, ppDepthStencilSurfaceImpl, NULL);
			}
			checkOperationResult(hr, __FUNCTION__, "CreateDepthStencilSurface");
		}
	}

	//-----------------------------------------------------------------------------
	Noesis::Ptr<Noesis::RenderTarget> NoesisGUIRenderDeviceD3D9::createRenderTarget(const char* label, UINT width, UINT height, UINT sampleCount, IDirect3DTexture9* pTextureImpl, IDirect3DSurface9* pDepthStencilSurfaceImpl)
	{
		Noesis::Ptr<Noesis::RenderTarget> pRenderTarget;
		std::string oOutString;

		oOutString = label + std::string("_texture");

		Noesis::Ptr<NoesisGUITextureD3D9> pTexture;

		// Create the texture resource.
		pTexture = *new NoesisGUITextureD3D9(oOutString.c_str(), pTextureImpl);

		// Create the render target implementation.
		pRenderTarget = *new NoesisGUIRenderTargetD3D9(label, width, height, sampleCount, pTexture, pDepthStencilSurfaceImpl);

		// Add to array.
		m_aRenderTargets.push_back(pRenderTarget.GetPtr());

		return pRenderTarget;
	}


	//-----------------------------------------------------------------------------
	Noesis::Ptr<Noesis::RenderTarget> NoesisGUIRenderDeviceD3D9::CloneRenderTarget(const char* label, Noesis::RenderTarget* surface)
	{
		NoesisGUIRenderTargetD3D9* pRenderTargetImp = static_cast<NoesisGUIRenderTargetD3D9*>(surface);
		IDirect3DTexture9* pTextureImpl = nullptr;
		IDirect3DSurface9* pDepthStencilSurfaceImpl = pRenderTargetImp->GetDepthStencilSurface();
		pDepthStencilSurfaceImpl->AddRef();
	
		// Create the required resources.
		createRenderTargetResources(pRenderTargetImp->GetWidth(), pRenderTargetImp->GetHeight(), pRenderTargetImp->GetSampleCount(), &pTextureImpl, nullptr);

		// Case resources successfully created -> create the render target interface.
		Noesis::Ptr<Noesis::RenderTarget> pClonedRenderTarget;

		if (pTextureImpl != nullptr && pDepthStencilSurfaceImpl != nullptr)
		{
			pClonedRenderTarget = createRenderTarget(label, pRenderTargetImp->GetWidth(), pRenderTargetImp->GetHeight(), pRenderTargetImp->GetSampleCount(), pTextureImpl, pDepthStencilSurfaceImpl);
		}

		return pClonedRenderTarget;
	}

	//-----------------------------------------------------------------------------
	Noesis::Ptr<Noesis::Texture> NoesisGUIRenderDeviceD3D9::CreateTexture(const char* label, UINT width, UINT height, UINT numLevels, Noesis::TextureFormat::Enum format)
	{
		Noesis::Ptr<Noesis::Texture> pTexture;
		
		D3DFORMAT eTargetFormat = D3DFMT_UNKNOWN;

		switch (format)
		{
		case Noesis::TextureFormat::RGBA8:
			eTargetFormat = D3DFMT_A8R8G8B8;
			break;
		case Noesis::TextureFormat::R8:
			eTargetFormat = D3DFMT_L8;
			break;
		}

		if (eTargetFormat != D3DFMT_UNKNOWN)
		{
			IDirect3DTexture9* pTextureImpl = nullptr;
			HRESULT hr;

			hr = m_pDevice->CreateTexture(width, height, numLevels, 0, eTargetFormat, D3DPOOL_MANAGED, &pTextureImpl, NULL);
			checkOperationResult(hr, __FUNCTION__, "CreateTexture");
			
			if (pTextureImpl != nullptr)
			{
				pTexture = *new NoesisGUITextureD3D9(label, pTextureImpl);
			}
		}


		return pTexture;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::UpdateTexture(Noesis::Texture* texture, UINT level, UINT x, UINT y, UINT width, UINT height, const void * data)
	{
		NoesisGUITextureD3D9* pTexture = static_cast<NoesisGUITextureD3D9*>(texture);

		pTexture->UpdateContent(level, x, y, width, height, data);
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::BeginRender(bool offscreen)
	{	
		
		invalidateRenderStateCache();
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::invalidateRenderStateCache()
	{
		HRESULT hr;

		// Index buffer.
		hr = m_pDevice->SetIndices(m_oIndexBuffer.pIndexBuffer);
		checkOperationResult(hr, __FUNCTION__, "SetIndices");
		// Clear Vertex buffers.
		for (UINT nIndex=0; nIndex < m_oDeviceCaps.MaxStreams; ++nIndex)
		{
			hr = m_pDevice->SetStreamSource(nIndex, NULL, 0, 0);
			checkOperationResult(hr, __FUNCTION__, "SetStreamSource");
		}

		// Clear textures.
		for (UINT nIndex = 0; nIndex < m_oDeviceCaps.MaxSimultaneousTextures; ++nIndex)
		{
			hr = m_pDevice->SetTexture(nIndex, NULL);
			checkOperationResult(hr, __FUNCTION__, "SetTexture");
		}

		// Vertex declaration.
		hr = m_pDevice->SetVertexDeclaration(NULL);
		checkOperationResult(hr, __FUNCTION__, "SetVertexDeclaration");
		m_pCurVertexDecl = nullptr;

		// Vertex shader.
		hr = m_pDevice->SetVertexShader(NULL);
		checkOperationResult(hr, __FUNCTION__, "SetVertexShader");
		m_pCurVertexShader = nullptr;
		m_nVertexCBHash = 0;
		
		// Pixel shader.
		hr = m_pDevice->SetPixelShader(NULL);
		checkOperationResult(hr, __FUNCTION__, "SetVertexShader");
		m_pCurPixelShader = nullptr;
		m_nPixelCBHash = 0;

		// Raster states.
		hr = m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_CULLMODE");

		const float fZero = 0.0f;

		hr = m_pDevice->SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)&fZero));
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_DEPTHBIAS");

		hr = m_pDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&fZero));
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SLOPESCALEDEPTHBIAS");

		// Depth buffer states.
		hr = m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_ZENABLE");

		hr = m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_ZWRITEENABLE");

		hr = m_pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_ZFUNC");
		
		// Stencil buffer states.
		m_oCurRenderState.f.stencilMode = Noesis::StencilMode::Disabled;
		hr = m_pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILENABLE");

		hr = m_pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILFAIL");

		hr = m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILZFAIL");

		hr = m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILPASS");

		hr = m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILFUNC");

		// Case two sided stencil is supported -> make sure it's disabled.
		if (m_oDeviceCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)
		{
			hr = m_pDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_TWOSIDEDSTENCILMODE");
		}
		
		m_uCurStencilRef = 0;
		hr = m_pDevice->SetRenderState(D3DRS_STENCILREF, 0);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILREF");

		hr = m_pDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILMASK");

		hr = m_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_STENCILWRITEMASK");
		
		// Blend states.
		m_oCurRenderState.f.blendMode = Noesis::BlendMode::Src;
		hr = m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_ALPHABLENDENABLE");

		hr = m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SRCBLEND");

		hr = m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_DESTBLEND");

		hr = m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_BLENDOP");

		hr = m_pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SRCBLENDALPHA");

		hr = m_pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_DESTBLENDALPHA");

		hr = m_pDevice->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_BLENDOPALPHA");

		hr = m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SEPARATEALPHABLENDENABLE");
		
		// Update the current render state.
		m_oCurRenderState.f.wireframe = 0;
		hr = m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_FILLMODE");

		m_oCurRenderState.f.scissorEnable = 0;
		hr = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SCISSORTESTENABLE");
		
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::SetRenderTarget(Noesis::RenderTarget* surface)
	{
		NoesisGUIRenderTargetD3D9* pRenderTarget = static_cast<NoesisGUIRenderTargetD3D9*>(surface);
		NoesisGUITextureD3D9* pTexture = static_cast<NoesisGUITextureD3D9*>(pRenderTarget->GetTexture());
		HRESULT hr;

		// Set the color surface.
		hr = m_pDevice->SetRenderTarget(0, pTexture->GetSurface());
		checkOperationResult(hr, __FUNCTION__, "SetRenderTarget");

		// Set the depth stencil surface.
		if (pRenderTarget->GetDepthStencilSurface() != nullptr)
		{
			hr = m_pDevice->SetDepthStencilSurface(pRenderTarget->GetDepthStencilSurface());
			checkOperationResult(hr, __FUNCTION__, "SetDepthStencilSurface");
		}
		
		// Set the viewport.
		D3DVIEWPORT9 oViewPort;
		oViewPort.X = 0;
		oViewPort.Y = 0;
		oViewPort.Width = pRenderTarget->GetWidth();
		oViewPort.Height = pRenderTarget->GetHeight();
		oViewPort.MinZ = 0.0f;
		oViewPort.MaxZ = 1.0f;

		hr = m_pDevice->SetViewport(&oViewPort);
		checkOperationResult(hr, __FUNCTION__, "SetViewport");
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::BeginTile(const Noesis::Tile & tile, UINT surfaceWidth, UINT surfaceHeight)
	{
		// Configure the scissor rect.
		UINT x = tile.x;
		UINT y = (UINT)surfaceHeight - (tile.y + tile.height);
		RECT oRect;

		oRect.left = x;
		oRect.top = y;
		oRect.right = oRect.left + tile.width;
		oRect.bottom = oRect.top + tile.height;

		HRESULT hr;

		// Make sure scissor test supported.
		if (m_oDeviceCaps.RasterCaps & D3DPRASTERCAPS_SCISSORTEST)
		{
			// Enable scissor test.
			m_oCurRenderState.f.scissorEnable = 1;
			hr = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SCISSORTESTENABLE");


			// Define the scissor rect.
			hr = m_pDevice->SetScissorRect(&oRect);
			checkOperationResult(hr, __FUNCTION__, "SetScissorRect");
		}

		// Clear the color and stencil.
		hr = m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		checkOperationResult(hr, __FUNCTION__, "Clear Render Target");
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::EndTile()
	{

	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, UINT numTiles)
	{
		NoesisGUIRenderTargetD3D9* pRenderTarget = static_cast<NoesisGUIRenderTargetD3D9*>(surface);

		if (pRenderTarget->GetSampleCount() != 1)
		{
			HRESULT hr;

			// Enable scissor test.
			hr = m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			checkOperationResult(hr, __FUNCTION__, "SetRenderState - D3DRS_SCISSORTESTENABLE");

			for (UINT nIndex = 0; nIndex < numTiles; ++nIndex)
			{
				const Noesis::Tile& tile = tiles[nIndex];

				UINT x = tile.x;
				UINT y = (UINT)pRenderTarget->GetHeight() - (tile.y + tile.height);

				RECT oRect;

				oRect.left = x;
				oRect.top = y;
				oRect.right = oRect.left + tile.width;
				oRect.bottom = oRect.top + tile.height;

				// Define the scissor rect.
				hr = m_pDevice->SetScissorRect(&oRect);
				checkOperationResult(hr, __FUNCTION__, "SetScissorRect");

				// TODO: Copy the content of the MSSA render target to the destination non sampled texture.
			}
		}
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::EndRender()
	{

	}

	//-----------------------------------------------------------------------------
	void* NoesisGUIRenderDeviceD3D9::MapVertices(UINT bytes)
	{
		//assert(bytes < m_oVertexBuffer.size);

		DWORD dwLockFlags = 0;

		if (bytes <= m_oVertexBuffer.size - m_oVertexBuffer.pos)
		{
			dwLockFlags = D3DLOCK_NOOVERWRITE;
		}
		else
		{
			dwLockFlags = D3DLOCK_DISCARD;
			m_oVertexBuffer.pos = 0;
		}

		m_oVertexBuffer.drawPos = m_oVertexBuffer.pos;
		m_oVertexBuffer.pos += bytes;

		void* pBuffer = NULL;

		HRESULT hr = m_oVertexBuffer.pVertexBuffer->Lock(m_oVertexBuffer.drawPos, bytes, &pBuffer, dwLockFlags);

		checkOperationResult(hr, __FUNCTION__, "LockVertexBuffer");

		return pBuffer;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::UnmapVertices()
	{
		HRESULT hr = m_oVertexBuffer.pVertexBuffer->Unlock();

		checkOperationResult(hr, __FUNCTION__, "UnlockVertexBuffer");
	}

	//-----------------------------------------------------------------------------
	void* NoesisGUIRenderDeviceD3D9::MapIndices(UINT bytes)
	{
		//assert(bytes < m_oIndexBuffer.size);

		DWORD dwLockFlags = 0;

		if (bytes <= m_oIndexBuffer.size - m_oIndexBuffer.pos)
		{
			dwLockFlags = D3DLOCK_NOOVERWRITE;
		}
		else
		{
			dwLockFlags = D3DLOCK_DISCARD;
			m_oIndexBuffer.pos = 0;
		}

		m_oIndexBuffer.drawPos = m_oIndexBuffer.pos;
		m_oIndexBuffer.pos += bytes;

		void* pBuffer = NULL;

		HRESULT hr = m_oIndexBuffer.pIndexBuffer->Lock(0, 0, &pBuffer, dwLockFlags);

		checkOperationResult(hr, __FUNCTION__, "LockIndexBuffer");

		return (BYTE*)pBuffer + m_oIndexBuffer.drawPos;
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::UnmapIndices()
	{
		HRESULT hr = m_oIndexBuffer.pIndexBuffer->Unlock();

		checkOperationResult(hr, __FUNCTION__, "UnlockIndexBuffer");
	}

	//-----------------------------------------------------------------------------
	void NoesisGUIRenderDeviceD3D9::DrawBatch(const Noesis::Batch & batch)
	{	
		HRESULT hr;

 		//assert(batch.shader.v < NS_COUNTOF(Programs));
 		//assert(Programs[batch.shader.v].vShaderIdx != -1);
 		//assert(Programs[batch.shader.v].pShaderIdx != -1);
		//assert(Programs[batch.shader.v].vShaderIdx < NS_COUNTOF(m_aVertexShaders));
		//assert(Programs[batch.shader.v].pShaderIdx < NS_COUNTOF(m_aPixelShaders));
 
		// Prepare the rendering pipeline.
 		setShaders(batch);
		setVertexStageResources(batch);
 		setRenderState(batch.renderState, batch.stencilRef);
 		setTextures(batch);
 
		// Execute the draw indexed operation.
		hr = m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,
			0,
			static_cast<UINT>(batch.numVertices),
			static_cast<UINT>(batch.startIndex + (m_oIndexBuffer.drawPos / 2)),
			static_cast<UINT>(batch.numIndices / 3)
		);

		checkOperationResult(hr, __FUNCTION__, "DrawIndexedPrimitive");
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::BaseShader::BaseShader()
	{
		lpMicroCode = NULL;
		lpConstantTable = NULL;

	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::BaseShader::~BaseShader()
	{
		SAFE_RELEASE(lpMicroCode);
		SAFE_RELEASE(lpConstantTable);
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::VertexShader::VertexShader()
	{
		pVertexShader = NULL;
		hProjMatrix = NULL;
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::VertexShader::~VertexShader()
	{
		SAFE_RELEASE(pVertexShader);
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::PixelShader::PixelShader()
	{
		pPixelShader = NULL;
		hColor = NULL;
		hOpacity = NULL;
		hRadialGrad = NULL;
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::PixelShader::~PixelShader()
	{
		SAFE_RELEASE(pPixelShader);
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::VertexDeclaration::VertexDeclaration()
	{
		pVertexDeclaration = NULL;
		nStride = 0;
	}

	//-----------------------------------------------------------------------------
	NoesisGUIRenderDeviceD3D9::VertexDeclaration::~VertexDeclaration()
	{
		SAFE_RELEASE(pVertexDeclaration);
	}

}

#endif