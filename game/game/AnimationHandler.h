#pragma once

static __declspec(naked) PTModel * LoadPTModelPhysique( const char * pszFile )
{
	JMP( 0x0047B360 );
}

#define _ANIMATIONDYNAMIC_DATA_HEADER_		"Dynamic Animation 1.00"

//Animations
struct DynamicAnimationData
{
	char									szHeader[32]; //Dynamic Animation 1.00

	int										iCount;

	struct AnimationData
	{
		char								szAnimationFile[256];
		char								szModelDataFile[256];

		int									iID;

		BOOL								bLoop;

		int									iAreaType;

		int									iClassID;
	};

	AnimationData							sAnimation[64];
};

struct FrameInfoData
{
	DWORD									dwStart;
	DWORD									dwEnd;
};

struct FalconDataPattern
{
	int										iUnk;

	float									fWorldX;
	float									fWorldY;
	float									fWorldZ;

	char									szUnknown[0x44];

	class UnitData							* pcUnitData;
};

struct AnimationDataInfo
{
	BOOL									bActive;
	void									* pcThis;
	int										iFrame;
	int										iAngleX;
	int										iAngleY;
	int										iAngleZ;
	BOOL									bFrame;
	struct FrameInfoData					sFrameInfo;
};


void InjectKeyRotation( struct RawSMDObject * psRawObject, int iIndex, int iFrame, struct RawSMDKeyRotation * psKeyRotation, struct RawSMDMatrixF * psKeyRotationMatrix );
void InjectKeyPosition( struct RawSMDObject * psRawObject, int iIndex, int iFrame, struct RawSMDKeyPosition * psKeyPosition );
void InjectKeyScale( struct RawSMDObject * psRawObject, int iIndex, int iFrame, struct RawSMDKeyScale * psKeyScale );
BOOL MergeKeyRotations( struct RawSMDObject * psRawObject );
BOOL MergeKeyPositions( struct RawSMDObject * psRawObject );
BOOL MergeKeyScalings( struct RawSMDObject * psRawObject );

class CAnimationHandler
{
public:
											CAnimationHandler();
	virtual							       ~CAnimationHandler();

	void									Init();

	AnimationDataInfo						* GetFreeData();

	static std::vector<AnimationDataInfo>	vAnimationData;

	static DWORD WINAPI						AnimationThreadData();


	int										AddAnimationObject( void * pcThis, int iFrame, int iAngleX, int iAngleY, int iAngleZ, FrameInfoData * psFrameInfo );

	void									SetFalconMove( FalconDataPattern  * psData );

	BOOL									HandleBoneCache( struct EXEModelData * psModelData );

	BOOL									AddCacheModelData( ModelData * psModelData );

	BOOL									EncryptBoneFile( char * pszFile );

	virtual BOOL							DecryptBoneFile( char * pszFile );
	virtual BOOL							DecryptBoneFile( PTModel * pcModelAnimation );

	BOOL									InjectDynamicAnimation( struct EXEModelData * psModelData, PTModel * pcModelAnimation, int iID, BOOL bLoop, int iClassFlag, int iAreaType, int iaEvent[4], int iItemType, int iTypeAnim );

	void									LoadDynamicAnimation();


	void									PrintInfoINX( const char * pszFilePath );
	void									FixSMBINXSubFileMotion( const char * pszSMBFile, const char * pszINXFile, const char * pszSMBFileOut, const char * pszINXFileOut );
	BOOL									InjectAnimation( const char * pszSMBFileFrom, const char * pszSMBFileTo, const char * pszINXFileTo, int eAnimationType, int iAreaType, BOOL bLoop, unsigned int uClassFlag, unsigned int uSkillCode = 0 );

	static void								LoadTerrain( char * pszFilePath );

	int										SplitFrames( struct RawINXFile * psRawInx, struct RawSMDFrame * psSplit, int iMaxCount, int iCount, bool bUpdateInx = false );

private:

	std::vector<ModelData *>				vAnimationModels;
};

