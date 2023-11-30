#pragma once

#include "EXETexture.h"

#define DXPROJECTION_W				(*(float*)0x03245870)
#define DXPROJECTION_H				(*(float*)0x03245874)
#define DXPROJECTION_Q1				(*(float*)0x03245860)
#define DXPROJECTION_Q2				(*(float*)0x02E45820)
#define DXPROJECTION_CENTERX		(*(int*)0x02E45828)
#define DXPROJECTION_CENTERY		(*(int*)0x0324585C)
#define DXPROJECTION_IS_ZOOM		(*(BOOL*)0x02E45824)
#define DXPROJECTION_ZOOMIN_DIV_X	(*(float*)0x03245868)
#define DXPROJECTION_ZOOMIN_DIV_Y	(*(float*)0x03245858)
#define DXPROJECTION_ZOOMIN_ADD_X	(*(float*)0x02E4586C)
#define DXPROJECTION_ZOOMIN_ADD_Y	(*(float*)0x03245864)
#define DXPROJECTION_MIDDLE_X		(*(int*)0x0082BF04)
#define DXPROJECTION_MIDDLE_Y		(*(int*)0x0082BF08)

struct EXERenderVertex
{
	int					tx, ty, tz;

	DWORD				dwClipAnd;

	float				xy[2];
	float				zbuffer;
	union
	{
		float				rhw;

		StageNormalVertex * psNormal;
	};
	
    union
    {
        struct
        {
            int					nx, ny, nz;
        };
        float			fViewWorldPos[3];
    };

		

	short				sColor[4];			//RGBA
	union
	{
		BYTE			bVertexColors[4];	//RGBA
		DWORD			dwVertexColor;
	};
	union
	{
		BYTE			bSpecular[4];		//RGBA
		DWORD			dwSpecular;
	};
	void			  * pcSourceVertex;
};

struct EXERenderFace
{
	EXERenderVertex	  * psRenderVertex[3];
	int					iMaterialID;

	void			  * pcTextureLink;
	DWORD				dwClipAnd;

	EXERenderFace	  * psNextRenderFace;
};

struct EXERenderMaterial
{
	int					iunk1;

	int					iMaterialCounter;
	EXERenderFace	  * psRenderFace;
	EXERenderFace	  * psLastRenderFace;

	int					iClipMaterialCounter;
	EXERenderFace	  * psClipRenderFace;
	EXERenderFace	  * psClipLastRenderFace;
};

struct EXERenderLight
{
	int					iType;
	Point3D				sPosi;
	Point3D				sRangePosi;
	int					iRange;
	int					iRange2;

	int					r, g, b, a;
};

struct EXECameraTrace
{
	Point3D				sPosition;
	Point3D				sPositionUnk;

	EXEMatrixE			sRotationMatrix;

	Point2D				sAngle;
};

struct EXEVertexCacheTest
{
	D3DVALUE			vertex[600 * 3 * 64];

	int					iMaterialID;

	int					iState;
};

class EXERender
{
private:
	IMPFNC				pRenderBuffer			= 0x02E45830;

	IMPFNC				pfnAddFaceRender		= 0x004830E0;
	IMPFNC				pfnAddMaterialRender	= 0x004805D0;
	IMPFNC				pfnSetCameraPosition	= 0x0047E880;
	IMPFNC				pfnSetStateRender		= 0x00484040;
	IMPFNC				pfnSetStateRenderClip	= 0x00481BD0;
	IMPFNC				pfnSetStateRenderAlpha	= 0x00482020;
	IMPFNC				pfnHandleVertexLight	= 0x004813F0;
	IMPFNC				pfnHandleVertex			= 0x00481030;
	IMPFNC				pfnSetColor				= 0x0047E0D0;
	IMPFNC				pfnHandleVertexColor	= 0x00481730;
	IMPFNC				pfnAddDynamicLight		= 0x0047EF10;
	IMPFNC				pfnCloseCamera			= 0x0047F200;


	IMPFNC				pfnAddFaceObject		 = 0x0047FF80;
	IMPFNC				pfnAddVertexObject		 = 0x0047F990;
	IMPFNC				pfnAddVertexRenderLeft	 = 0x00480660;
	IMPFNC				pfnAddVertexRenderRight  = 0x004807D0;
	IMPFNC				pfnAddVertexRenderTop	 = 0x00480AC0;
	IMPFNC				pfnAddVertexRenderBottom = 0x00480950;
	IMPFNC				pfnAddVertexRenderNearZ	 = 0x00480C30;
	IMPFNC				pfnAddVertexRenderFarZ	 = 0x00480D90;
	IMPFNC				pfnAddFaceRender2		 = 0x00480310;
	IMPFNC				pfnAddTextureLinkRender  = 0x0047F340;
	IMPFNC				pfnInit					 = 0x0047F2E0;
	IMPFNC				pfnVertex2DGeo			 = 0x00483EA0;
	IMPFNC				pfnGetRectangle2D		 = 0x0047E5F0;
	IMPFNC				pfnSetCamera2			 = 0x0047F1A0;
	IMPFNC				pfnSetColorVertex		 = 0x0047E160;

	IMPFNC				pfnAddBillboard							= 0x0058C130;
	IMPFNC				pfnAddBillboardRotation					= 0x0058C9A0;
	IMPFNC				pfnAddBillboardHorizontal				= 0x0058B730;
	IMPFNC				pfnAddBillboardHorizontalRotation		= 0x0058BA70;
	IMPFNC				pfnAddBillboardVerticalRotation			= 0x0058C460;


public:

	struct Billboard
	{
		int		  x;
		int		  y;
		int		  z;

		UINT	  w;
		UINT	  h;

		float	  u;
		float	  u2;
		float	  v;
		float	  v2;

		UINT	  tid;

		UINT	  a;
		UINT	  r;
		UINT	  g;
		UINT	  b;
	};


	static WORD			saIndices[3 * 16384];
	static int			iIndexIndices;

	int					iRenderType; //1 = Render Ice Footstep

	BOOL				bFogIsRendered;
	int					iFogMode;
	float				fFogStart;
	float				fFogEnd;
	DWORD				dwFogColor;

	int					iGameFieldView;
	int					iGameFieldViewStep;

	EXERenderVertex	  * psRenderVertex;
	EXERenderFace	  * psRenderFace;

	EXETextureLink		psaTempTexture[80000];		
	EXERenderMaterial	psaRenderMaterial[5000];

	DWORD				pdwRenderMaterialList[5000];

	EXETextureMaterial* psRenderMaterial;
	int					iRenderMaterialCount;

	int					iDispMask;

	int					iMaterialListCount;
	int					iMaterialListRearCount;

	int					iVertexCount;
	int					iFaceCount;
	int					iTextures;

	int					iShiftPerspectiveWidth;
	int					iMultPerspectiveHeight;

	StageVertex		  * psVertex;
	void			  * psFace;
	void			  * psTexture;

	EXERenderLight		sObjectLight[64];
	int					iObjectLightCount;

	BOOL				bApplyObjectLight;

	EXERenderLight		sLight[64];
	int					iLightCount;

	int					iSumCount;

	int					iCameraPositionMask;
	EXEMatrixI			sCameraM;
	Point3D				sCameraPos;
	Point3D				sCameraAngle;

	Point3D				sTexturePosi;
	Point3D				sTextureSize;

	DWORD				dwClipAnd;
	int					iRenderTime;

	int					r, g, b, a;

	int					iTextureModifier[8];
	int					iAlphaTestDepth;

	BOOL				bRenderAlpha;

	int					iObjectVertexScale;
	int					ZWriteAuto;
	int					iDistortionMesh;

	BOOL				bRenderOutline;
	float				fOutlineRatio;
	float				fOutlineX;
	float				fOutlineY;
	DWORD				dwOutlineColor;

	bool                bLoadUsingNewRender;

	static EXEDrawVertexMinor saGlowVertex[600];

	int					Init();

	int					Render();
	int					Render( int iMaterial );

	int					RenderGlow3D();
	int					RenderGlow3D( int iMaterial );

	void				SetGlow3D( BOOL bSelected, int iDistance = 0, float fX = 0.0f, float fY = 0.0f, DWORD dwColor = 0 );

	BOOL				SetRenderClip( DWORD dwClip );

	BOOL				AddDynamicLight( int iX, int iY, int iZ, int iR, int iG, int iB, int iA, int iRange );

	BOOL				ClearLight();

	BOOL				AddMaterialRender( int iMaterialID );

	bool				IsUsingNewRenderToLoad() const{ return bLoadUsingNewRender; }
	void				SetUseNewRenderToLoad( bool LoadUsingNewRender ) { bLoadUsingNewRender = LoadUsingNewRender; }
	void				ResetUseNewRenderToLoad(){ bLoadUsingNewRender = false; }

	static BOOL			IsRenderingWithShaders(){ return bRenderWithShaders; }
	static void			SetRenderWithShaders( bool RenderWithShaders ) { bRenderWithShaders = RenderWithShaders; }
	static void			ResetRenderWithShaders(){ bRenderWithShaders = false; }

	EXERenderFace *		AddBillboard( Billboard * psBillboard );
	EXERenderFace *		AddBillboardRotation( Billboard * psBillboard, float fPitch, float fYaw, float fRoll );
	EXERenderFace *		AddBillboardHorizontal( Billboard * psBillboard );
	EXERenderFace *		AddBillboardHorizontalRotation( Billboard * psBillboard, float fPitch, float fYaw, float fRoll );
	EXERenderFace *		AddBillboardVertical( Billboard * psBillboard );
	EXERenderFace *		AddBillboardVerticalRotation( Billboard * psBillboard, float fPitch, float fYaw, float fRoll );

	EXERenderFace		* AddFaceRender( EXERenderVertex * v0, EXERenderVertex * v1, EXERenderVertex * v2, int iMaterialID, EXETextureLink * psTexture );

	BOOL				AddTextureLinkRender( EXERenderFace * psFace, DWORD * pdw, float u0, float v0, float u1, float v1, float u2, float v2 );

	EXERenderVertex		* AddObjectVertex( EXEVertex * psVertex, DWORD dwObjectCode = 0 );
	EXERenderFace		* AddObjectFace( EXEFace * psFace );
	EXERenderVertex		* AddStageVertex( StageVertex * psStageVertex, EXETextureMaterial * psMaterial );


	EXERenderVertex		* AddVertexRenderLeft( EXERenderFace * psFace, int iA, int iB, float * fMul );
	EXERenderVertex		* AddVertexRenderRight( EXERenderFace * psFace, int iA, int iB, float * fMul );
	EXERenderVertex		* AddVertexRenderTop( EXERenderFace * psFace, int iA, int iB, float * fMul );
	EXERenderVertex		* AddVertexRenderBottom( EXERenderFace * psFace, int iA, int iB, float * fMul );
	EXERenderVertex		* AddVertexRenderNearZ( EXERenderFace * psFace, int iA, int iB, float * fMul );
	EXERenderVertex		* AddVertexRenderFarZ( EXERenderFace * psFace, int iA, int iB, float * fMul );

	BOOL				AddFaceRender( EXERenderFace * psFace, int iValue );

	BOOL				Vertex2DGeometry( int iColor );

	int					GetRectangle2D( int iX, int	iY, int iZ, int iWidth, int iHeight, RECT * psRectangle2D, Point2D * psPosition = NULL );

	BOOL				SetStateRender( EXETextureMaterial * psTextureMaterial );
	BOOL				SetStateRender( EXETextureMaterial * psTextureMaterial, BOOL bClip );
	BOOL				SetStateRenderAlpha( EXETextureMaterial * psTextureMaterial, BOOL bClip );

	void				SetColor( short * psColor, EXETextureMaterial * psTextureMaterial );

	BOOL				SetColorVertex( EXERenderVertex * rv, short sColors[4], BYTE Color[4], BYTE Specular[4] );

	void				AddIndices( int iCount );

	int					HandleVertexLight( float* pRenderBuffer, int iMaterialID, BOOL bClip );
	int					HandleVertex( float * pRenderBuffer, int iMaterialID, BOOL bClip );
	int					HandleVertexColor( float* pRenderBuffer, int iMaterialID, BOOL bClip, short * psColor );

	BOOL				SetTextureManager( EXETextureManager * pcTexture );
	BOOL				SetCameraPosition( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );
	BOOL				SetCameraPosition( int iX, int iY, int iZ, EXEMatrixE * psMatrix );
	BOOL				GetCameraPosition( Point3D & sPosition, Point3D & sAnglePosition );

	BOOL				ClipFaceRender();

	void				CloseCamera();

	BOOL				UpdateFogState( BOOL b );

	DWORD				GetTextureAverageColor( EXETextureHandle * psTextureHandle, BOOL bHighColor );


	//Getters
	static EXETextureHandle			* GeTextureGameHandle(){ return *(EXETextureHandle**)0x2E4581C; }
	static EXERender				* GetRender(){ return (EXERender*)0x3245878; }

	static DWORD					GetFVF( int iTextureCount );
	static int						GetSizeFVF( int iTextureCount );

	static EXERenderFace			* AddStageFace( Stage * pcStage, StageFace * psStageFace );

	static PTModelCache			* pcModelCache;

	static DWORD					dwSamplerState;

	static BOOL						bShadowMapping;

	static BOOL						bNoClipFaceRender;
	static BOOL						bRenderWithShaders;

	static std::vector<StageNormalVertex>		vStageNormals;
};

void EXEDXProjectionWork( EXERenderVertex * psVertex );

#define RENDERER ((EXERender*)0x3245878)