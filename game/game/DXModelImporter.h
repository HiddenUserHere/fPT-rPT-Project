#pragma once

#include "DXModel.h"
#include "SMDDXModelImporter.h"

namespace DX
{

class ModelImporter
{
public:
	virtual bool Load( const std::string & strFilePath ) = 0;

	virtual Model_ptr Build( Model_ptr pBoneModel = nullptr ) = 0;
};

}