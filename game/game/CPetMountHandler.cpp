#include "stdafx.h"
#include "CPetMountHandler.h"

CPetMountHandler::CPetMountHandler()
{
}

CPetMountHandler::~CPetMountHandler()
{
}

UnitData * CPetMountHandler::Load( Unit * pcUnitCharacter, const std::string strFile )
{
    auto pcUnitData = new UnitData();

    Point3D sPosition   = pcUnitCharacter->GetPosition();
    Point3D sAngle      = pcUnitCharacter->GetAngle();

    LoadUnitData( pcUnitData, strFile.c_str(), "", &sPosition, &sAngle );

    return pcUnitData;
}

void CPetMountHandler::Free( Unit * pcUnitCharacter )
{
}

void CPetMountHandler::Init()
{
}

void CPetMountHandler::Frame( Unit * pcUnitCharacter )
{
    if ( (pcUnitCharacter->pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitCharacter->pcUnitData->iCharacterType == CHARACTERTYPE_Player) )
        if ( pcUnitCharacter->pcUnitDataMount )
        {
            pcUnitCharacter->pcUnitDataMount->Main();

            pcUnitCharacter->pcUnitDataMount->sPosition = pcUnitCharacter->GetPosition();
        }
}

void CPetMountHandler::Render( Unit * pcUnitCharacter, Point3D * psCamera, Point3D * psAngle )
{
    if ( (pcUnitCharacter->pcUnitData->sCharacterData.iType == CHARACTERTYPE_Player) || (pcUnitCharacter->pcUnitData->iCharacterType == CHARACTERTYPE_Player) )
        if ( pcUnitCharacter->pcUnitDataMount )
            pcUnitCharacter->pcUnitDataMount->Render();
}
