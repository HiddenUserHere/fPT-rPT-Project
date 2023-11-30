#pragma once

#include "DXTerrain.h"

namespace DX
{

class TerrainImporter
{
public:
	virtual bool Load( const std::string & strFilePath ) = 0;

	virtual Terrain_ptr Build() = 0;
};

}