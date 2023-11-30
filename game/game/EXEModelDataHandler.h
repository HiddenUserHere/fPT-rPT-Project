#pragma once

struct EXEModelData;

#define			MAX_MODELDATA	4096

PTModel * LoadPTModelGate1( const char * pszFileName, const char * pszFileModelOther = nullptr );

PTModel * LoadPTModelBoneGate1( const char * pszFileName );

class EXEModelDataHandler
{
private:
	EXEModelDataHandler		* pcModelDataBone;

	EXEModelData			saModelData[MAX_MODELDATA];

	int						GetFreeModel();

	int						GetModelID( const char * pszFileName );

public:
	EXEModelData			* LoadBoneData( const char * pszFile );

	EXEModelData			* LoadModelData( const char * pszFile );
};

