#include "stdafx.h"
#include "DXMaterial.h"

namespace DX
{

Material::Material()
{
	vTextures.reserve( MAX_MATERIAL_TEXTURES );
}

Material::~Material()
{
}

}