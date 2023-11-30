#pragma once

#include "RawSMDVertex.h"
#include "RawSMDFace.h"
#include "RawSMDTextureLink.h"
#include "RawSMDVertexPhysique.h"
#include "RawSMDMatrix.h"
#include "RawSMDQuaternion.h"
#include "RawSMDVector3.h"
#include "RawSMDKeyRotation.h"
#include "RawSMDKeyPosition.h"
#include "RawSMDKeyScale.h"
#include "RawSMDFrame.h"

struct RawSMDObject
{
	int						  iUnknown1;
	union
	{
		RawSMDVertex * paVertex;

		EXEVertexTangentF * paNewVertex;
	};
	RawSMDFace				* paFace;
	RawSMDTextureLink		* paTextureLink;
	RawSMDVertexPhysique	* paPhysique;

	int						  iaUnknown6[6];

	int						  iaUnknown7[6];

	int						  iaUnknown8[2];

	int						  iaUnknown9[2];

	int						  iVertexCount;
	int						  iFaceCount;
	int						  iTextureLinkCount;

	int						  iUnknown10;
	int						  iUnknown11;

	int						  iUnknown12[9];
	int						  iUnknown13[8];

	char					  szObjectName[32];
	char					  szParentObjectName[32];

	int						  iUnknown14;

	RawSMDMatrixI			  sMatrixTM;
	RawSMDMatrixI			  sInvertedMatrixTM;
	RawSMDMatrixF			  sFrameMatrix;
	RawSMDMatrixI			  sRotationMatrix;

	RawSMDMatrixI			  sWorldMatrix;
	RawSMDMatrixI			  sLocalMatrix;

	int						  iUnknown15;

	RawSMDQuaternion		  sRotationQuaternion;
	RawSMDVector3I			  sScalingVector;
	RawSMDVector3I			  sTranslationVector;

	RawSMDKeyRotation		* paKeyRotation;
	RawSMDKeyPosition		* paKeyPosition;
	RawSMDKeyScale			* paKeyScale;
	RawSMDMatrixF			* paRotation;

	int						  iKeyRotationCount;
	int						  iKeyPositionCount;
	int						  iKeyScaleCount;

	RawSMDFrame				  aRotationFrame[32];
	RawSMDFrame				  aTranslationFrame[32];
	RawSMDFrame				  aScalingFrame[32];
	int						  iFrameCount;

	BOOL						bUseInfluences;
	std::vector<EXEMeshInfluenceVertex> vInfluences;
	std::vector<EXEVertexTangentF>			vFloatVertices;
	Math::Matrix4						sTransformationMatrixF;
};