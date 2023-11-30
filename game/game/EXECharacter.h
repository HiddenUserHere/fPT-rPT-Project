#pragma once

struct CharacterWaterAnimationData
{
    ECharacterClass         eCharacterClass;
    EAnimationType          eAnimationType;
    int                     iAnimationID;
};

static const CharacterWaterAnimationData saCharacterWaterAnimationData[] =
{
    {CHARACTERCLASS_Fighter,                    ANIMATIONTYPE_Running,              136},
    {CHARACTERCLASS_Fighter,                    ANIMATIONTYPE_Idle,                 137},

    {CHARACTERCLASS_Mechanician,                ANIMATIONTYPE_Running,              136},
    {CHARACTERCLASS_Mechanician,                ANIMATIONTYPE_Idle,                 137},

    {CHARACTERCLASS_Archer,                     ANIMATIONTYPE_Running,              93},
    {CHARACTERCLASS_Archer,                     ANIMATIONTYPE_Idle,                 94},

    {CHARACTERCLASS_Pikeman,                    ANIMATIONTYPE_Running,              86},
    {CHARACTERCLASS_Pikeman,                    ANIMATIONTYPE_Idle,                 87},

    {CHARACTERCLASS_Atalanta,                   ANIMATIONTYPE_Running,              93},
    {CHARACTERCLASS_Atalanta,                   ANIMATIONTYPE_Idle,                 94},

    {CHARACTERCLASS_Knight,                     ANIMATIONTYPE_Running,              136},
    {CHARACTERCLASS_Knight,                     ANIMATIONTYPE_Idle,                 137},

    {CHARACTERCLASS_Magician,                   ANIMATIONTYPE_Running,              75},
    {CHARACTERCLASS_Magician,                   ANIMATIONTYPE_Idle,                 76},

    {CHARACTERCLASS_Priestess,                  ANIMATIONTYPE_Running,              74},
    {CHARACTERCLASS_Priestess,                  ANIMATIONTYPE_Idle,                 75},

    {CHARACTERCLASS_Assassin,                   ANIMATIONTYPE_Running,              99},
    {CHARACTERCLASS_Assassin,                   ANIMATIONTYPE_Idle,                 100},

    {CHARACTERCLASS_Shaman,                     ANIMATIONTYPE_Running,              93},
    {CHARACTERCLASS_Shaman,                     ANIMATIONTYPE_Idle,                 94},

};


class EXECharacter
{
private:
    IMPFNC                      pfnShiftKeyIsPressed = 0x00A16328;
    IMPFNC                      pfnAutomaticDoubleClick = 0x00CF4814;

    IMPFNC                      pfnWeatherType = 0x00CF47A4;
    IMPFNC                      pfnGetUnitDataColision = 0x0045B1D0;

    IMPFNC                      pfnFreeUnitDataTarget = 0x00454E40;

    static bool                 IsShiftKeyPressed() { return (bool)READBYTE( pfnShiftKeyIsPressed ); }

    static BOOL                 IsAutomaticDoubleClick() { return (*(BOOL *)pfnAutomaticDoubleClick); }
    static void                 SetAutomaticDoubleClick( BOOL b ) { (*(BOOL *)pfnAutomaticDoubleClick) = b; }


   static int                   GetWeatherType() { return READINT( pfnWeatherType ); }

    static UnitData             * GetUnitDataColision( int iX, int iY, int iZ );

    static void                 FreeUnitDataTarget();

public:

    static const CharacterWaterAnimationData & GetCharacterWaterAnimationData( ECharacterClass eCharacterClass, EAnimationType eAnimationType );

    static void                 UpdateSelfCharacterMovement( UnitData * pcUnitData, int iSpeed );

    static int                  AnimateCharacter( UnitData * pcUnitData, int iAnimationType );

    static int                  CanMoveToPosition( Stage * pcStage, Point3D * psPosition, Point3D * psAngle, Point3D * psMovePosition, int iDistance, int iWidth, int iHeight, BOOL bCheckOverlap );
};

