#pragma once

class CPetMountHandler
{
    CPetMountHandler();
    virtual ~CPetMountHandler();

    UnitData                    * Load( Unit * pcUnitCharacter, const std::string strFile );

    void                        Free( Unit * pcUnitCharacter );

    void                        Init();

    void                        Frame( Unit * pcUnitCharacter );

    void                        Render( Unit * pcUnitCharacter, Point3D * psCamera, Point3D * psAngle );


};

