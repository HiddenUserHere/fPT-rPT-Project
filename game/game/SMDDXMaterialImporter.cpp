#include "stdafx.h"
#include "SMDDXMaterialImporter.h"
#include "ImportSMDFile.h"


SMDMaterialImporter::SMDMaterialImporter()
{
}


SMDMaterialImporter::~SMDMaterialImporter()
{
}

std::vector<DX::Material_ptr> SMDMaterialImporter::GetLoadMaterials( RawSMDMaterialHeader * psRawMaterialHeader, BOOL bTerrain )
{
	std::vector<DX::Material_ptr> vMaterials;

	auto pTextureFactory = GetGraphicsDevice()->GetTextureFactory();

	for ( unsigned int i = 0; i < psRawMaterialHeader->uMaterialCount; i++ )
	{
		//New Material
		RawSMDMaterial * psSMDMaterial = psRawMaterialHeader->paRawSMDMaterial + i;
		auto pMaterial = std::make_shared<DX::Material>();

		//Load Textures for the current Material
		for ( unsigned int j = 0; j < psSMDMaterial->uTextureCount; j++ )
		{
			//It will be always 3D
			RawSMDMaterial::Texture * psTexture = psSMDMaterial->paTexture[j];
			pMaterial->AddTexture( pTextureFactory->MakeTexture( psTexture->szTextureFilePath, TRUE ), (DX::Material::ETextureType)psSMDMaterial->iTextureType );
		}

		//Settings for Material
		pMaterial->SetTwoSided( psSMDMaterial->iIsTwoSided == 0 ? FALSE : TRUE );
		pMaterial->SetDiffuseRed( psSMDMaterial->fDiffuseRed );
		pMaterial->SetDiffuseGreen( psSMDMaterial->fDiffuseGreen );
		pMaterial->SetDiffuseBlue( psSMDMaterial->fDiffuseBlue );
		pMaterial->SetTransparent( psSMDMaterial->iHasTransparency );
		pMaterial->SetTransparency( psSMDMaterial->fTransparency );
		pMaterial->SetSelfIllumination( psSMDMaterial->fSelfIllumination );
		pMaterial->SetAlphaBlend( (DX::Material::EAlphaBlend)psSMDMaterial->iBlendType );
		pMaterial->SetInvisible( (psSMDMaterial->eMaterialType1 & RawSMDMaterial::EMaterialType::MATERIALTYPE_Invisible) ? TRUE : FALSE );
		pMaterial->SetUseType( (DX::Material::EType)psSMDMaterial->eMaterialType1 );
		pMaterial->SetMeshType( (DX::Material::EType)psSMDMaterial->eMaterialType2 );

		//Set Effect
		{
			std::string strEffectFile;
			std::string strTechnique;
			UINT uNumPasses				= 1;

			//Terrain?
			if ( bTerrain )
			{
				strEffectFile				= "game\\scripts\\shaders\\Terrain_p.fx";
				strTechnique				= pMaterial->GetTransparent() ? "Transparent" : "Opaque";
			}
			else
			{
				strEffectFile				= "game\\scripts\\shaders\\Mesh.fx";
				strTechnique				= pMaterial->GetTransparent() ? "Transparent" : "Opaque";
			}

			pMaterial->SetEffect( GetGraphicsDevice()->GetEffectFactory()->MakeEffect( strEffectFile ) );
			pMaterial->SetEffectTechnique( strTechnique );
			pMaterial->SetEffectNumPasses( uNumPasses );
		}

		vMaterials.push_back( pMaterial );
	}

	return vMaterials;
}
