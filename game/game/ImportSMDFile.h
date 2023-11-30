#pragma once

#include "ImportFile.h"

#include "RawSMDHeader.h"
#include "RawSMDObjectInfo.h"
#include "RawSMDMaterialHeader.h"
#include "RawSMDMaterial.h"
#include "RawSMDObject.h"
#include "RawSMDTerrain.h"


namespace Import
{

class SMDFile : public File
{
public:
	enum EType
	{
		Unknown		= -1,
		Model		= 0,
		Terrain		= 1,
	};

public:
							  SMDFile( const std::string & strFilePath );
	virtual					 ~SMDFile();

	bool					  Load();
	void					  Unload();

	bool					  IsLoaded() const { return bLoaded; }

	BOOL					  ToStage( RawSMDTerrain * pcThis );

	static const char		* TypeToString( EType eType );
	
private:
	bool					  LoadModel();
	void					  UnloadModel();

	bool					  LoadTerrain( int iTerrainVersion );
	void					  UnloadTerrain();

	void					  LoadMaterials(BOOL bUse = FALSE);
	void					  UnloadMaterials();


	void					  SetBuffer( char * pbNewBuffer );
	char					  * GetBuffer() { return pbBuffer; }

	void					  ReadBuffer( void * pBuffer, int iSize, int iCount );
	void					  Read( void * pBuffer, size_t iElementSize, size_t iCount );

public:
	EType					  eType = Unknown;

	RawSMDHeader			  sRawHeader;

	RawSMDMaterialHeader	  sRawMaterialHeader;

	RawSMDObjectInfo		* psaRawObjectInfo = NULL;
	RawSMDObject			* psaRawObject = NULL;

	RawSMDTerrain			* psRawTerrain = NULL;

	int						iTerrainVersion = -1;

	char					* pbBuffer = NULL;

	int						iCurrentBuffer = 0;

	UglyRefactor::PTModelVersion sModelVersion;

private:
	bool					  bLoaded = false;
};

}
