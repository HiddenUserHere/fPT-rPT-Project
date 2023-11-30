#pragma once

#include "EXEVertex.h"
#include "EXEFace.h"
#include "EXEMatrix.h"
#include "EXEFrame.h"
#include "EXEPoint.h"

struct EXEAnimationRotation
{
	int iFrame;

	float fX;
	float fY;
	float fZ;
	float fW;
};

struct EXEAnimationPosition
{
	int iFrame;

	float fX;
	float fY;
	float fZ;
};

struct EXEAnimationScale
{
	int iFrame;
	
	int iX;
	int iY;
	int iZ;
};

enum EMeshID
{
	MESHID_None,
	MESHID_LeftFoot,
	MESHID_RightFoot,
	MESHID_LeftHand,
	MESHID_RightHand,
	MESHID_Head,
	MESHID_Chest,
	MESHID_Belly,
	MESHID_Pelvis,
};

struct MeshNameData
{
	EMeshID					iMeshID;
	char					* pszMeshName;
};

static const MeshNameData saMeshNameData[8] =
{
	{ MESHID_LeftFoot, "Bip01 L Foot" },
	{ MESHID_RightFoot, "Bip01 R Foot" },
	{ MESHID_LeftHand, "Bip01 L Hand" },
	{ MESHID_RightHand, "Bip01 R Hand" },
	{ MESHID_Head, "Bip01 Head" },
	{ MESHID_Chest, "Bip01 Spine1" },
	{ MESHID_Belly, "Bip01 Spine" },
	{ MESHID_Pelvis, "Bip01 Pelvis" },
};

const int MAX_PTMESH_INFLUENCES = 4;

struct EXEMeshInfluenceVertex
{
	short			saBoneIndex[MAX_PTMESH_INFLUENCES];
	float			faWeight[MAX_PTMESH_INFLUENCES];
};

class EXEMesh
{
private:
	IMPFNC					pfnFinalRender		= 0x00476148;
	IMPFNC					pfnReformTexture	= 0x004742A0;
	IMPFNC					pfnBeginRender		= 0x00474F60;
public:
	DWORD					dwHeader;

	EXEVertex			  * pMeshVertex;
	EXEFace				  * pMeshFace;
	EXEFacePosition		  * pMeshFacePosition;

	EXEMesh				 ** pVertexBoneList;
	EXEVertex				sZeroVertex;

	IMinMax					sZ;
	IMinMax					sY;
	IMinMax					sX;

	int						iUnk1;
	int						iBound;

	int						iMaxVertex;
	int						iMaxFaces;

	int						iVertexCount;
	int						iFaceCount;

	int						iTextureLinkCount;

	int						iColorEffect;
	DWORD					dwUnused;

	Point3D					sPosition;
	Point3D					sCameraPosition;
	Point3D					sAngle;

	int						iTriangle[8];

	char					szName[32];
	char					szParentName[32];
	EXEMesh				  * pParentMesh;

	EXEMatrixI				sTm;
	EXEMatrixI				sTmInvert;

	EXEMatrixF				sTmResult;
	EXEMatrixI				sTmRotate;

	EXEMatrixI				sWorld;
	EXEMatrixI				sLocal;

	int						iAniFrame;

	float					qx, qy, qz, qw;
	int						sx, sy, sz;
	int						px, py, pz;

	EXEAnimationRotation  * sRot;
	EXEAnimationPosition  * sPos;
	EXEAnimationScale	  * sScale;

	EXEMatrixF			  * sPreviousRotate;

	int						iRotateCount;
	int						iPosCount;
	int						iScaleCount;

	EXEFramePosition		saRotateFrame[32];
	EXEFramePosition		saPosFrame[32];
	EXEFramePosition		saScaleFrame[32];

	int						iFrameCount;

	Graphics::Mesh * pcNewMesh;

	BOOL								bUseInfluences;
	std::vector<EXEMeshInfluenceVertex>	vInfluences;
	std::vector<EXEVertexTangentF>			vFloatVertices;
	Math::Matrix4						sTransformationMatrixF;

    EXEMesh();
	~EXEMesh();
	EXEMesh( int iNumVertexCount, int iNumFaceCount );

	void					Constructor();
	void					Destructor();
	void					Constructor( int iNumVertices, int iNumFaces );

	int						BeginRender();

	BOOL					IsInView( int iX, int iY, int iZ, Point3D * psLocalAngle, Point3D * psWorldAngle );
	BOOL					Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEFrame * psFrameInfo = NULL );
	BOOL					Animate( int iFrame, int iAngleX, int iAngleY, int iAngleZ, EXEModel * pcTalkAnimation, EXEFrame * psFrameInfo = NULL );

	BOOL					AnimateTalk( int iFrame );

	BOOL					SetPosition( Point3D * psPosition, Point3D * psAngle );

	void					ClearVertex();

	int						GetFrameRotate( int iFrame );
	int						GetFrameRotate( EXEMatrixF & sMatrix, EXEMatrixF * prevMatrix, EXEAnimationRotation * pRot, int iRotate, int iFrame, EXEFrame * psFrameInfo = NULL );

	int						GetFramePosition( int iFrame );
	int						GetFramePosition( float & x, float & y, float & z, EXEAnimationPosition * pPos, int iFrame );

	int						GetFrameScale( int iFrame );
	int						GetFrameScale( int & x, int & y, int & z, EXEAnimationScale * pScale, int iFrame );

	BOOL					ReformTexture( int iType, int iMaterialID, int iMaterialCount, Point3D * psPosi, Point3D * psSize );

	BOOL					PreRender( Point3D * psCamera, Point3D * psAngle );
	int						Render( Point3D * psCamera, Point3D * psAngle );
	int						FinalRender();

	BOOL					WorldForm();

	void					UpdateVertexPhysique();

	BOOL					LoadFromHandle( HANDLE hFile, EXEModel * pcBone = nullptr );

	static FILE				* ReadGeometry( FILE * fp, EXEMesh * pcOut, char * pszNode = NULL );

	EXEMatrixI				& GetWorldMatrix() { return sWorld; }

	int						GetSaveSize( bool bUseOldSMD = false );

	void					SortVertices();

	void					AddVertex( int iX, int iY, int iZ );

	void					ComputeNormals();

	void					ComputeTransformation();

    static EXEMesh			* NewMeshGate( EXEMesh * pcMeshGame );
    static EXEMesh			* NewMeshGateVertexFaces( EXEMesh * pcMeshGame, int iNumVertices, int iNumFaces );
};


class EXEMeshOldClearData
{
private:
	IMPFNC					pfnFinalRender = 0x00476148;
	IMPFNC					pfnReformTexture = 0x004742A0;
	IMPFNC					pfnBeginRender = 0x00474F60;
public:
	DWORD					dwHeader;

	EXEVertex * pMeshVertex;
	EXEFace * pMeshFace;
	EXEFacePosition * pMeshFacePosition;

	EXEMesh ** pVertexBoneList;
	EXEVertex				sZeroVertex;

	IMinMax					sZ;
	IMinMax					sY;
	IMinMax					sX;

	int						iUnk1;
	int						iBound;

	int						iMaxVertex;
	Graphics::Mesh * pcNewMesh;

	int						iVertexCount;
	int						iFaceCount;

	int						iTextureLinkCount;

	int						iColorEffect;
	DWORD					dwUnused;

	Point3D					sPosition;
	Point3D					sCameraPosition;
	Point3D					sAngle;

	int						iTriangle[8];

	char					szName[32];
	char					szParentName[32];
	EXEMesh * pParentMesh;

	EXEMatrixI				sTm;
	EXEMatrixI				sTmInvert;

	EXEMatrixF				sTmResult;
	EXEMatrixI				sTmRotate;

	EXEMatrixI				sWorld;
	EXEMatrixI				sLocal;

	int						iAniFrame;

	float					qx, qy, qz, qw;
	int						sx, sy, sz;
	int						px, py, pz;

	EXEAnimationRotation * sRot;
	EXEAnimationPosition * sPos;
	EXEAnimationScale * sScale;

	EXEMatrixF * sPreviousRotate;

	int						iRotateCount;
	int						iPosCount;
	int						iScaleCount;

	EXEFramePosition		saRotateFrame[32];
	EXEFramePosition		saPosFrame[32];
	EXEFramePosition		saScaleFrame[32];

	int						iFrameCount;

	void				Constructor();
	void				Destructor();

	EXEMeshOldClearData();
    ~EXEMeshOldClearData();
};
