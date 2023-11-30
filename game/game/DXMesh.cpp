#include "stdafx.h"
#include "DXMesh.h"

//#include "X3DMaths.h"

namespace DX
{

Mesh::Mesh( std::string strName ) : strName( strName )
{
}

Mesh::~Mesh()
{
}

void Mesh::AddOpaquePart( MeshPart_ptr pPart )
{
	lOpaqueParts.push_front( pPart );
}

void Mesh::AddTransparentPart( MeshPart_ptr pPart, BOOL bEnd )
{
	if ( bEnd )
		lTransparentParts.push_back( pPart );
	else
		lTransparentParts.push_front( pPart );
}

void Mesh::SetFrame( int iNewFrame )
{
	//Above Max Frame?
	if ( (iMaxFrame > 0) && (iNewFrame >= iMaxFrame) )
		iNewFrame = iMaxFrame - 1;

	//Invalid Frame?
	if ( iNewFrame < 0 )
		return;

	//Already on this Frame?
	if ( iNewFrame == iFrame )
		return;

	//Build Fix Matrix (swap y and z)
	const float f[16] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
	const X3D::Matrix4 mFix( f );

	//Mesh has no Frames?
	if ( iMaxFrame <= 0 )
	{
		//Remember current Frame
		iFrame = 0;

		//Use Transformation
		if ( pParent )
			mFrame = mTransformation * pParent->GetTransformationInverse();
		else
			mFrame = mTransformation;
	}
	else
	{
		//Remember current Frame
		iFrame = iNewFrame;

		//Get Scaling Matrix for New Frame
		X3D::Matrix4 mScaling = GetScaling( iFrame );

		//Get Rotation Matrix for New Frame
		X3D::Matrix4 mRotation = GetRotation( iFrame );

		//Get Translation Matrix for New Frame
		X3D::Matrix4 mTranslation = GetTranslation( iFrame );

		//Set Frame Matrix based on New Frame
		mFrame = mScaling * mRotation * mTranslation;
	}

	//Apply Parent
	if ( pParent )
		mFrame = mFrame * pParent->GetFrameMatrix();

	//Set Mesh Local Matrix
	mLocal = mFrame * mFix;
}

void Mesh::UpdateBoundingVolumes( const std::vector<X3D::Matrix4> * pvBoneMatrices )
{
	if ( vVertices.size() <= 0 )
		return;

	if ( (pvBoneMatrices) && (vVertices.size() != vSkinIndices.size()) )
		return;

	bool bFirst = true;

	//Building Bounding AABB Volume
	for ( size_t i = 0; i < vVertices.size(); i++ )
	{
		X3D::Matrix4 mMatrix;

		if ( pvBoneMatrices )
		{
			int iIndex = (int)(vSkinIndices[i]);

			if ( iIndex >= (int)(*pvBoneMatrices).size() )
				return;

			mMatrix = (*pvBoneMatrices)[iIndex];
		}
		else
		{
			mMatrix = mLocal;
		}

		X3D::Vector3 vTransformedVector = mMatrix * vVertices[i];

		if ( bFirst )
		{
			cBoundingAABB.min = vTransformedVector;
			cBoundingAABB.max = vTransformedVector;

			bFirst = false;
		}
		else
		{
			if ( vTransformedVector.x < cBoundingAABB.min.x )
				cBoundingAABB.min.x = vTransformedVector.x;
			if ( vTransformedVector.y < cBoundingAABB.min.y )
				cBoundingAABB.min.y = vTransformedVector.y;
			if ( vTransformedVector.z < cBoundingAABB.min.z )
				cBoundingAABB.min.z = vTransformedVector.z;

			if ( vTransformedVector.x > cBoundingAABB.max.x )
				cBoundingAABB.max.x = vTransformedVector.x;
			if ( vTransformedVector.y > cBoundingAABB.max.y )
				cBoundingAABB.max.y = vTransformedVector.y;
			if ( vTransformedVector.z > cBoundingAABB.max.z )
				cBoundingAABB.max.z = vTransformedVector.z;
		}
	}

	//Building Bounding Sphere Volume
	cBoundingSphere.center = cBoundingAABB.computeCenter();
	cBoundingSphere.radius = (cBoundingAABB.max - cBoundingAABB.min).length() / 2;
}

X3D::Matrix4 Mesh::GetScaling( int iFrame )
{
	//There are no Scalings?
	if ( vKeyScalings.size() == 0 )
		return X3D::Matrix4();

	//Frame of first Key Scaling higher than Frame?
	if ( vKeyScalings[0].iFrame > iFrame )
		return X3D::Matrix4();

	//Find List Index based on Frame
	int iF1 = 0, iF2 = 0;
	int i = 0;
	int j = vKeyScalings.size() - 1;
	for ( ; i < j; i++ )
	{
		iF1 = vKeyScalings[i].iFrame;
		iF2 = vKeyScalings[i + 1].iFrame;

		if ( (iF1 <= iFrame) && (iF2 > iFrame) )
			break;
	}

	//Nothing found?
	if ( i == j )
		return X3D::Matrix4();

	//Frame Difference and Advancement
	int iFrameDiff = iF2 - iF1;
	int iFrameAdvancement = iFrame - iF1;

	//Compute interpolation T [0,1]
	float t = (float)iFrameAdvancement / (float)iFrameDiff;

	//Get Vectors
	X3D::Vector3 & rV1 = vKeyScalings[i].vScaling;
	X3D::Vector3 & rV2 = vKeyScalings[i + 1].vScaling;

	//Lerp it
	X3D::Vector3 vResult;
	vResult.x = X3D::lerpf( rV1.x, rV2.x, t );
	vResult.y = X3D::lerpf( rV1.y, rV2.y, t );
	vResult.z = X3D::lerpf( rV1.z, rV2.z, t );

	//Return Matrix of Scaling
	return X3D::Matrix4().scale( vResult );
}

X3D::Matrix4 Mesh::GetRotation( int iFrame )
{
	//There are no Rotations?
	if ( vKeyRotations.size() == 0 )
		return mTransformationRotation;

	//Frame of first Key Rotation higher than Frame?
	if ( vKeyRotations[0].iFrame > iFrame )
		return X3D::Matrix4();

	//Find List Index based on Frame
	int iF1 = 0, iF2 = 0;
	int i = 0;
	int j = vKeyRotations.size() - 1;
	for ( ; i < j; i++ )
	{
		iF1 = vKeyRotations[i].iFrame;
		iF2 = vKeyRotations[i + 1].iFrame;

		if ( (iF1 <= iFrame) && (iF2 > iFrame) )
			break;
	}

	//Nothing found?
	if ( i == j )
		return mTransformationRotation;

	//Frame Difference and Advancement
	int iFrameDiff = iF2 - iF1;
	int iFrameAdvancement = iFrame - iF1;

	//Compute interpolation T [0,1]
	float t = (float)iFrameAdvancement / (float)iFrameDiff;

	//Get Quaternions
	X3D::Quaternion rQ1 = X3D::Quaternion( 0.0f, 0.0f, 0.0f, 0.0f );
	X3D::Quaternion & rQ2 = vKeyRotations[i + 1].qRotation;

	//Slerp it
	X3D::Quaternion qResult = rQ1.slerp( rQ2, t );

	//Return Matrix of Rotation
	return vKeyRotations[i].mRotation * qResult.toMatrix();
}

X3D::Matrix4 Mesh::GetTranslation( int iFrame )
{
	//There are no Translations?
	if ( vKeyTranslations.size() == 0 )
		return mTransformationTranslation;

	//Frame of first Key Translation higher than Frame?
	if ( vKeyTranslations[0].iFrame > iFrame )
		return X3D::Matrix4();

	//Find List Index based on Frame
	int iF1 = 0, iF2 = 0;
	int i = 0;
	int j = vKeyTranslations.size() - 1;
	for ( ; i < j; i++ )
	{
		iF1 = vKeyTranslations[i].iFrame;
		iF2 = vKeyTranslations[i + 1].iFrame;

		if ( (iF1 <= iFrame) && (iF2 > iFrame) )
			break;
	}

	//Nothing found?
	if ( i == j )
		return mTransformationTranslation;

	//Frame Difference and Advancement
	int iFrameDiff = iF2 - iF1;
	int iFrameAdvancement = iFrame - iF1;

	//Compute interpolation T [0,1]
	float t = (float)iFrameAdvancement / (float)iFrameDiff;

	//Get Vectors
	X3D::Vector3 & rV1 = vKeyTranslations[i].vTranslation;
	X3D::Vector3 & rV2 = vKeyTranslations[i + 1].vTranslation;

	//Lerp it
	X3D::Vector3 vResult;
	vResult.x = X3D::lerpf( rV1.x, rV2.x, t );
	vResult.y = X3D::lerpf( rV1.y, rV2.y, t );
	vResult.z = X3D::lerpf( rV1.z, rV2.z, t );

	//Return Matrix of Translation
	return X3D::Matrix4().translate( vResult );
}

}