#pragma once

#include "DXModel.h"

namespace DX
{

class GraphicsDevice;

typedef std::shared_ptr<class ModelFactory> ModelFactory_ptr;

class ModelFactory
{
public:
								  ModelFactory( GraphicsDevice * pGraphicsDevice );

	void						  OnLostDevice();
	void						  OnResetDevice();

	Model_ptr					  GetModelCache( std::string strModelFile, std::string strBoneFile );
	Model_ptr					  GetBoneCache( std::string strBoneFile );

	Model_ptr					  MakeModel( std::string strModelFile, std::string strBoneFile = "" );
	void						  AddModel( Model_ptr pNewModel );

	static Model_ptr			  MakeModel( UnitData * pcUnitData );
	static void					  FreeModel( UnitData * pcUnitData );

private:
	GraphicsDevice				* pGraphicsDevice;

	std::vector<Model_ptr>		  vCache;
	std::vector<Model_ptr>		  vCacheBone;
};

}