#pragma once


enum class EMountType : int
{
    MOUNTTYPE_None,
    MOUNTTYPE_Hopy,
    MOUNTTYPE_Horse,
    MOUNTTYPE_Unicorn,
    MOUNTTYPE_Wolf,

    MOUNTTYPE_Hopy_1,
    MOUNTTYPE_Hopy_2,

    MOUNTTYPE_Horse_1,
    MOUNTTYPE_Horse_2,
    MOUNTTYPE_Horse_3,

    MOUNTTYPE_Pingu,
    MOUNTTYPE_Pingu_1,
    MOUNTTYPE_Pingu_2,

    MOUNTTYPE_Wolf_1,
    MOUNTTYPE_Wolf_2,
    MOUNTTYPE_Wolf_3,
    MOUNTTYPE_Wolf_4,
    MOUNTTYPE_Wolf_5,
    MOUNTTYPE_Wolf_6,
    MOUNTTYPE_Wolf_7,
    MOUNTTYPE_Wolf_8,
    MOUNTTYPE_Wolf_9,

    MOUNTTYPE_Unicorn_1,
    MOUNTTYPE_Unicorn_2,
    MOUNTTYPE_Unicorn_3,
    MOUNTTYPE_Unicorn_4,

    MOUNTTYPE_ObscureHog,

    MOUNTTYPE_Wolf_10,
    MOUNTTYPE_Wolf_11,

    MOUNTTYPE_Unicorn_5,
    MOUNTTYPE_Unicorn_6,
    MOUNTTYPE_Unicorn_7,
    MOUNTTYPE_Unicorn_8,

    MOUNTTYPE_Lion,
    MOUNTTYPE_Lion_1,
    MOUNTTYPE_Lion_2,
    MOUNTTYPE_Lion_3,
    MOUNTTYPE_Lion_4,
    MOUNTTYPE_Lion_5,
    MOUNTTYPE_Lion_6,

    MOUNTTYPE_Hog,
    MOUNTTYPE_Hog_1,

    MOUNTTYPE_Tiger,
    
    MOUNTTYPE_Snowdeer,
    MOUNTTYPE_Snowdeer_1,
    MOUNTTYPE_Snowdeer_2,
    MOUNTTYPE_Snowdeer_3,
    MOUNTTYPE_Snowdeer_4,

    MOUNTTYPE_Horse_4,
    MOUNTTYPE_Horse_5,

    MOUNTTYPE_Rabie,

    MOUNTTYPE_Raptor,
    MOUNTTYPE_Raptor_1,
    MOUNTTYPE_Raptor_2,
    MOUNTTYPE_Raptor_3,
    MOUNTTYPE_Raptor_4,
    MOUNTTYPE_Raptor_5,

};

enum class EMountRareType : int
{
    MOUNTRARETYPE_None,
    MOUNTRARETYPE_Common                = 10,
    MOUNTRARETYPE_Rare                  = 20,
    MOUNTRARETYPE_Epic                  = 30,
    MOUNTRARETYPE_Legendary             = 40,
};

static const char * GetMountRareTypeToString( EMountRareType eRareType )
{
    switch ( eRareType )
    {
        case EMountRareType::MOUNTRARETYPE_Common:
            return "Common";
            break;

        case EMountRareType::MOUNTRARETYPE_Rare:
            return "Rare";
            break;

        case EMountRareType::MOUNTRARETYPE_Epic:
            return "Epic";
            break;

        case EMountRareType::MOUNTRARETYPE_Legendary:
            return "Legendary";
            break;
    }

    return "";
};


static DWORD GetMountRareTypeToColor( EMountRareType eRareType )
{
#if defined(_GAME)
    switch ( eRareType )
    {
        case EMountRareType::MOUNTRARETYPE_Common:
            return D3DCOLOR_ARGB( 255, 0, 191, 255 );
            break;

        case EMountRareType::MOUNTRARETYPE_Rare:
            return D3DCOLOR_ARGB( 255, 128, 0, 128 );
            break;

        case EMountRareType::MOUNTRARETYPE_Epic:
            return D3DCOLOR_ARGB( 255, 255, 140, 0 );
            break;

        case EMountRareType::MOUNTRARETYPE_Legendary:
            return D3DCOLOR_ARGB( 255, 255, 255, 0 );
            break;
    }

    return D3DCOLOR_ARGB( 255, 255, 255, 255 );
#else
    return 0;
#endif
};

static EMountType GetMountTypeByItemID( EItemID eItemID )
{
    EMountType eMountType = EMountType::MOUNTTYPE_None;

    switch ( eItemID )
    {
        case ITEMID_MountObscureHog:
            eMountType = EMountType::MOUNTTYPE_ObscureHog;
            break;
        case ITEMID_PinguMountMysterious:
            eMountType = EMountType::MOUNTTYPE_Pingu_2;
            break;
        case ITEMID_MountSnowflakeUnicorn:
            eMountType = EMountType::MOUNTTYPE_Unicorn_4;
            break;
        case ITEMID_MountHog:
            eMountType = EMountType::MOUNTTYPE_Hog;
            break;
        case ITEMID_MountHog1:
            eMountType = EMountType::MOUNTTYPE_Hog_1;
            break;
        case ITEMID_MountHopy:
            eMountType = EMountType::MOUNTTYPE_Hopy;
            break;
        case ITEMID_MountHopy1:
            eMountType = EMountType::MOUNTTYPE_Hopy_1;
            break;
        case ITEMID_MountHopy2:
            eMountType = EMountType::MOUNTTYPE_Hopy_2;
            break;
        case ITEMID_MountHorse:
            eMountType = EMountType::MOUNTTYPE_Horse;
            break;
        case ITEMID_MountHorse1:
            eMountType = EMountType::MOUNTTYPE_Horse_1;
            break;
        case ITEMID_MountHorse2:
            eMountType = EMountType::MOUNTTYPE_Horse_2;
            break;
        case ITEMID_MountLion1:
            eMountType = EMountType::MOUNTTYPE_Lion_1;
            break;
        case ITEMID_MountLion5:
            eMountType = EMountType::MOUNTTYPE_Lion_5;
            break;
        case ITEMID_MountUnicorn3:
            eMountType = EMountType::MOUNTTYPE_Unicorn_3;
            break;
        case ITEMID_MountPingu1:
            eMountType = EMountType::MOUNTTYPE_Pingu_1;
            break;
        case ITEMID_MountGhostWolf:
            eMountType = EMountType::MOUNTTYPE_Wolf;
            break;
        case ITEMID_MountHorsePTM:
            eMountType = EMountType::MOUNTTYPE_Horse_3;
            break;
        case ITEMID_MountSnowdeer:
            eMountType = EMountType::MOUNTTYPE_Snowdeer;
            break;
        case ITEMID_MountRudolf:
            eMountType = EMountType::MOUNTTYPE_Snowdeer_1;
            break;
        case ITEMID_MountWhiteDeer:
            eMountType = EMountType::MOUNTTYPE_Snowdeer_2;
            break;
        case ITEMID_MountIronCharger:
            eMountType = EMountType::MOUNTTYPE_Horse_4;
            break;
        case ITEMID_MountMecaballus:
            eMountType = EMountType::MOUNTTYPE_Horse_5;
            break;
        case ITEMID_MountZira:
            eMountType = EMountType::MOUNTTYPE_Lion_4;
            break;
        case ITEMID_MountIceTiger:
            eMountType = EMountType::MOUNTTYPE_Tiger;
            break;
        case ITEMID_MountUnicorn6:
            eMountType = EMountType::MOUNTTYPE_Unicorn_6;
            break;
        case ITEMID_MountRabie:
            eMountType = EMountType::MOUNTTYPE_Rabie;
            break;
        case ITEMID_MountRaptor:
            eMountType = EMountType::MOUNTTYPE_Raptor;
            break;
        case ITEMID_MountRaptor1:
            eMountType = EMountType::MOUNTTYPE_Raptor_1;
            break;
        case ITEMID_MountRaptor2:
            eMountType = EMountType::MOUNTTYPE_Raptor_2;
            break;
        case ITEMID_MountRaptor3:
            eMountType = EMountType::MOUNTTYPE_Raptor_3;
            break;

        default:
            break;
    }

    return eMountType;
};

struct MountModelPath
{
    EMountType      iMountType;
    char            szMountPath[64];

    EMountType      eAnimationByMountType;

    int             iHeight;
};

struct MountCharacterAnimation
{
    EMountType      iMountType;
    ECharacterClass eCharacterClass;

    EAnimationType  eMountAnimationType;
    int             iCharacterAnimationID;
};

static const MountModelPath saMountModels[] =
{
    {EMountType::MOUNTTYPE_Hopy,                  "game\\objects\\mounts\\Hopt\\rPT_Hopt.ini",                           EMountType::MOUNTTYPE_Hopy,                 1256},
    {EMountType::MOUNTTYPE_Hopy_1,                "game\\objects\\mounts\\Hopt\\rPT_Hopt_1.ini",                         EMountType::MOUNTTYPE_Hopy,                 1256},
    {EMountType::MOUNTTYPE_Hopy_2,                "game\\objects\\mounts\\Hopt\\rPT_Hopt_2.ini",                         EMountType::MOUNTTYPE_Hopy,                 1256},
    {EMountType::MOUNTTYPE_Horse,                 "game\\objects\\mounts\\Horse\\Horse.ini",                             EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Horse_1,               "game\\objects\\mounts\\Horse\\Horse_1.ini",                           EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Horse_2,               "game\\objects\\mounts\\Horse\\Horse_2.ini",                           EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Horse_3,               "game\\objects\\mounts\\Horse\\Horse_3.ini",                           EMountType::MOUNTTYPE_Snowdeer,             3768},
    {EMountType::MOUNTTYPE_Unicorn,               "game\\objects\\mounts\\Unicorn\\Unicorn.ini",                         EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_1,             "game\\objects\\mounts\\Unicorn\\Unicorn_1.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_2,             "game\\objects\\mounts\\Unicorn\\Unicorn_2.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_3,             "game\\objects\\mounts\\Unicorn\\Unicorn_3.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_4,             "game\\objects\\mounts\\Unicorn\\Unicorn_4.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_5,             "game\\objects\\mounts\\Unicorn\\Unicorn_5.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_6,             "game\\objects\\mounts\\Unicorn\\Unicorn_6.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_7,             "game\\objects\\mounts\\Unicorn\\Unicorn_7.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Unicorn_8,             "game\\objects\\mounts\\Unicorn\\Unicorn_8.ini",                       EMountType::MOUNTTYPE_Horse,                3768},
    {EMountType::MOUNTTYPE_Wolf,                  "game\\objects\\mounts\\Wolf\\Wolf.ini",                               EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_1,                "game\\objects\\mounts\\Wolf\\Wolf_1.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_2,                "game\\objects\\mounts\\Wolf\\Wolf_2.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_3,                "game\\objects\\mounts\\Wolf\\Wolf_3.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_4,                "game\\objects\\mounts\\Wolf\\Wolf_4.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_5,                "game\\objects\\mounts\\Wolf\\Wolf_5.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_6,                "game\\objects\\mounts\\Wolf\\Wolf_6.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_7,                "game\\objects\\mounts\\Wolf\\Wolf_7.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_8,                "game\\objects\\mounts\\Wolf\\Wolf_8.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_9,                "game\\objects\\mounts\\Wolf\\Wolf_9.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_10,               "game\\objects\\mounts\\Wolf\\Wolf_10.ini",                            EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Wolf_11,               "game\\objects\\mounts\\Wolf\\Wolf_11.ini",                            EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Pingu,                 "game\\objects\\mounts\\Pingu\\Pingu.ini",                             EMountType::MOUNTTYPE_Pingu,                1256},
    {EMountType::MOUNTTYPE_Pingu_1,               "game\\objects\\mounts\\Pingu\\Pingu_1.ini",                           EMountType::MOUNTTYPE_Pingu,                1256},
    {EMountType::MOUNTTYPE_Pingu_2,               "game\\objects\\mounts\\Pingu\\Pingu_2.ini",                           EMountType::MOUNTTYPE_Pingu,                3400},
    {EMountType::MOUNTTYPE_ObscureHog,            "game\\objects\\mounts\\Hog\\ObscureHog.ini",                          EMountType::MOUNTTYPE_Wolf,                 3400},
    {EMountType::MOUNTTYPE_Hog,                   "game\\objects\\mounts\\Hog\\Hog.ini",                                 EMountType::MOUNTTYPE_Wolf,                 3400},
    {EMountType::MOUNTTYPE_Hog_1,                 "game\\objects\\mounts\\Hog\\Hog_1.ini",                               EMountType::MOUNTTYPE_Wolf,                 3400},
    {EMountType::MOUNTTYPE_Lion,                  "game\\objects\\mounts\\Lion\\Lion.ini",                               EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Lion_1,                "game\\objects\\mounts\\Lion\\Lion_1.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Lion_2,                "game\\objects\\mounts\\Lion\\Lion_2.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Lion_3,                "game\\objects\\mounts\\Lion\\Lion_3.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Lion_4,                "game\\objects\\mounts\\Lion\\Lion_4.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Lion_5,                "game\\objects\\mounts\\Lion\\Lion_5.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Lion_6,                "game\\objects\\mounts\\Lion\\Lion_6.ini",                             EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Tiger,                 "game\\objects\\mounts\\Tiger\\IceTiger.ini",                          EMountType::MOUNTTYPE_Wolf,                 3768},
    {EMountType::MOUNTTYPE_Snowdeer,              "game\\objects\\mounts\\Horse\\Snowdeer.ini",                          EMountType::MOUNTTYPE_Snowdeer,             3768},
    {EMountType::MOUNTTYPE_Snowdeer_1,            "game\\objects\\mounts\\Horse\\Snowdeer_1.ini",                        EMountType::MOUNTTYPE_Snowdeer,             3768},
    {EMountType::MOUNTTYPE_Snowdeer_2,            "game\\objects\\mounts\\Horse\\Snowdeer_2.ini",                        EMountType::MOUNTTYPE_Snowdeer,             3768},
    {EMountType::MOUNTTYPE_Horse_4,               "game\\objects\\mounts\\Horse\\Steam_Horse.ini",                       EMountType::MOUNTTYPE_Snowdeer,             3768},
    {EMountType::MOUNTTYPE_Horse_5,               "game\\objects\\mounts\\Horse\\Steam_Horse_1.ini",                     EMountType::MOUNTTYPE_Snowdeer,             3768},
    {EMountType::MOUNTTYPE_Rabie,                 "game\\objects\\mounts\\Rabbie\\Rabbie.ini",                           EMountType::MOUNTTYPE_Rabie,                3768},
    {EMountType::MOUNTTYPE_Raptor,                "game\\objects\\mounts\\Raptor\\Raptor.ini",                           EMountType::MOUNTTYPE_Raptor,               3768},
    {EMountType::MOUNTTYPE_Raptor_1,              "game\\objects\\mounts\\Raptor\\Raptor_1.ini",                         EMountType::MOUNTTYPE_Raptor,               3768},
    {EMountType::MOUNTTYPE_Raptor_2,              "game\\objects\\mounts\\Raptor\\Raptor_2.ini",                         EMountType::MOUNTTYPE_Raptor,               3768},
    {EMountType::MOUNTTYPE_Raptor_3,              "game\\objects\\mounts\\Raptor\\Raptor_3.ini",                         EMountType::MOUNTTYPE_Raptor,               3768},
    {EMountType::MOUNTTYPE_Raptor_4,              "game\\objects\\mounts\\Raptor\\Raptor_4.ini",                         EMountType::MOUNTTYPE_Raptor,               3768},
    {EMountType::MOUNTTYPE_Raptor_5,              "game\\objects\\mounts\\Raptor\\Raptor_5.ini",                         EMountType::MOUNTTYPE_Raptor,               3768},

};

static const MountCharacterAnimation saMountCharacterAnimation[] =
{
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      128},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         129},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      130},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         131},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      132},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         133},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      134},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         135},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      138},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         139},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      140},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         141},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Running,      142},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Fighter,         ANIMATIONTYPE_Idle,         143},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      128},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         129},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      130},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         131},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      132},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         133},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      134},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         135},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      138},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         139},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      140},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         141},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Running,      142},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Mechanician,     ANIMATIONTYPE_Idle,         143},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      85},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         86},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      87},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         88},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      89},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         90},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      91},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         92},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      95},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         96},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      97},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         98},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Archer,          ANIMATIONTYPE_Running,      99},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Archer,          ANIMATIONTYPE_Idle,         100},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      78},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         79},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      80},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         81},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      82},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         83},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      84},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         85},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      88},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         89},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      90},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         91},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Running,      92},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Pikeman,         ANIMATIONTYPE_Idle,         93},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      85},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         86},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      87},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         88},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      89},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         90},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      91},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         92},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      95},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         96},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      97},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         98},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Running,      99},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Atalanta,        ANIMATIONTYPE_Idle,         100},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      128},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         129},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      130},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         131},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      132},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         133},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      134},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         135},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      138},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         139},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      140},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         141},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Knight,          ANIMATIONTYPE_Running,      142},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Knight,          ANIMATIONTYPE_Idle,         143},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      67},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         68},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      69},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         70},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      71},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         72},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      73},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         74},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      77},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         78},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      79},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         80},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Magician,        ANIMATIONTYPE_Running,      81},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Magician,        ANIMATIONTYPE_Idle,         82},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      66},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         67},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      68},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         69},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      70},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         71},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      72},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         73},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      76},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         77},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      78},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         79},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Running,      80},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Priestess,       ANIMATIONTYPE_Idle,         81},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      91},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         92},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      93},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         94},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      95},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         96},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      97},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         98},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      101},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         102},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      103},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         104},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Running,      105},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Assassin,        ANIMATIONTYPE_Idle,         106},

    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      85},
    {EMountType::MOUNTTYPE_Hopy,                CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         86},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      87},
    {EMountType::MOUNTTYPE_Horse,               CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         88},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      89},
    {EMountType::MOUNTTYPE_Wolf,                CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         90},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      91},
    {EMountType::MOUNTTYPE_Pingu,               CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         92},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      95},
    {EMountType::MOUNTTYPE_Snowdeer,            CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         96},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      97},
    {EMountType::MOUNTTYPE_Rabie,               CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         98},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Running,      99},
    {EMountType::MOUNTTYPE_Raptor,              CHARACTERCLASS_Shaman,          ANIMATIONTYPE_Idle,         100},

};

#define MAX_MOUNTS_PER_DATA       5

struct MountUserListData
{
    int             iID;

    EMountType      eMountType;
    EMountRareType  eRareType;

    char            szName[32];
    char            szImageFile[64];

    int             iItemCount;
    Item            saItemList[3];

    BOOL            bIsSelected;
};

struct PacketMountUserList : Packet
{
    MinMax              sPacketCount;
    bool				IsFirst() { return sPacketCount.sMin == 1 ? true : false; }
    bool				IsOnly() { return sPacketCount.sMin == sPacketCount.sMax ? true : false; }

    int                 iCount;
    MountUserListData   saMounts[MAX_MOUNTS_PER_DATA];
};

struct PacketNewMount : Packet, MountUserListData
{
    int         iUnk0001;
};

struct PacketMountDelete : Packet
{
    EMountType      eMountType;
    
    PacketMountDelete( EMountType eType ) { eMountType = eType; };

    ~PacketMountDelete() {};
};
