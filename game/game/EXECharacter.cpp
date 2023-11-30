#include "stdafx.h"
#include "EXECharacter.h"

NAKED UnitData * EXECharacter::GetUnitDataColision( int iX, int iY, int iZ )
{
    JMP( pfnGetUnitDataColision );
}

NAKED void EXECharacter::FreeUnitDataTarget()
{
    JMP( pfnFreeUnitDataTarget );
}

const CharacterWaterAnimationData & EXECharacter::GetCharacterWaterAnimationData( ECharacterClass eCharacterClass, EAnimationType eAnimationType )
{
    for ( const auto & s : saCharacterWaterAnimationData )
        if ( s.eCharacterClass == eCharacterClass )
            if ( s.eAnimationType == eAnimationType )
                return s;

    return saCharacterWaterAnimationData[0];
}


void EXECharacter::UpdateSelfCharacterMovement( UnitData * pcUnitData, int iSpeed )
{
    pcUnitData->iMovementSpeed = 250 + 10 * pcUnitData->sCharacterData.iMovementSpeed;

    bool bRiding = MOUNTHANDLER->IsRiding( pcUnitData );

    if ( pcUnitData->iFreezeTimeLeft )
    {
        pcUnitData->iMovementSpeed *= iSpeed;
        pcUnitData->iMovementSpeed >>= 8;
    }

    if ( pcUnitData->iMoveCount > 0 )
    {
        pcUnitData->iMoveCount--;
        if ( pcUnitData->iMoveCount <= 0 )
            pcUnitData->bWalking = FALSE;
    }

    Point3D sPositionOld = pcUnitData->sPosition;

    if ( pcUnitData->iProjectileType == 0 )
        pcUnitData->pcTarget = nullptr;

    if ( pcUnitData->iWaterHeight != (-32767) )
    {
        if ( pcUnitData->sPosition.iY == pcUnitData->iWaterHeight )
        {
            if ( bRiding )
                MOUNTHANDLER->RemoveMount( pcUnitData );
        }
    }

    if ( ((GM_MODE == FALSE) && (pcUnitData->pcBody == nullptr)) || (pcUnitData->sCharacterData.sWeight.sCur > pcUnitData->sCharacterData.sWeight.sMax) )
        pcUnitData->bRunMode = FALSE;

    if ( (pcUnitData->bRunMode == FALSE) || pcUnitData->iProjectileType || (CHARACTERGAME->GetCurrentSP() == 0) )
    {
        if ( (pcUnitData->GetAnimation() != ANIMATIONTYPE_Falling) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Walking) )
        {
            if ( (bRiding == false) && ((pcUnitData->iWaterHeight == (-32767)) || (pcUnitData->sPosition.iY != pcUnitData->iWaterHeight)) )
                pcUnitData->Animate( ANIMATIONTYPE_Walking );
            else
            {
                if ( bRiding )
                    MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorWalk );

                pcUnitData->bWalking = FALSE;
            }
        }

        if ( pcUnitData->bWalking )
            pcUnitData->Move( pcUnitData->iMovementSpeed );
    }
    else
    {
        bool bCanRun = true;

        if ( (pcUnitData->GetAnimation() != ANIMATIONTYPE_Falling) && (pcUnitData->GetAnimation() != ANIMATIONTYPE_Running) )
        {
            if ( bRiding )
            {
                if ( MAP_ID == MAPID_BlessCastle )
                {
                    MOUNTHANDLER->ShowMessage( MOUNTMESSAGE_ErrorBC );

                    pcUnitData->bWalking = FALSE;

                    bCanRun = false;
                }
            }

            if ( bCanRun )
                pcUnitData->Animate( ANIMATIONTYPE_Running );
        }

        bool bFaster = (GM_MODE == TRUE) && IsShiftKeyPressed();

        if ( bCanRun )
            pcUnitData->Move( bFaster ? 3000 : (pcUnitData->iMovementSpeed * 460) >> 8 );

        if ( ((pcUnitData->iLoopTimer & 0x7) == 0) && (GetWeatherType() < 2) )
        {
            if ( (MAP_ID != MAPID_Bellatra) && ((MAP_TYPE == MAPTYPE_Wasteland) || (MAP_TYPE == MAPTYPE_Desert)) )
            {

                if ( (pcUnitData->iLoopTimer & 8) == 0 )
                    GetMoveLocation( 4 * 256, 0, 0, 0, pcUnitData->sAngle.iY, 0 );
                else
                    GetMoveLocation( -4 * 256, 0, 0, 0, pcUnitData->sAngle.iY, 0 );

                //Desert Effect
                int iRange = 10 + ((TICKCOUNT >> 2) & 0xF);
                StartGlobalEffect( pcUnitData->sPosition.iX + RADIAN3D_X, pcUnitData->sPosition.iY + 12 * 256, pcUnitData->sPosition.iZ + RADIAN3D_Z, iRange, iRange, 0x044C );
            }
        }
    }

    if ( pcUnitData->bCollision || (pcUnitData->sPosition.iX != sPositionOld.iX) || (pcUnitData->sPosition.iZ != sPositionOld.iZ) )
    {
        if ( UnitData * pcUnitDataCollide = GetUnitDataColision( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ ) )
        {
            if ( pcUnitData->bCollision == FALSE )
            {
                pcUnitData->sPosition.iX = sPositionOld.iX;
                pcUnitData->sPosition.iZ = sPositionOld.iZ;

                if ( ACTIONMODE )
                {
                    if ( pcUnitDataCollide->IsPlayer( false ) )
                    {
                        if ( CURRENT_TARGET != pcUnitDataCollide )
                        {
                            //Close Target
                            if ( CURRENT_TARGET )
                                FreeUnitDataTarget();

                            //Deprecated???
                            //OpenEachPlayer( lpChar );
                        }

                        //Deprecated???
                        /*
                        if ( chrEachMaster )
                            EachTradeButton = TRUE;
                        */
                    }
                }
            }

            if ( GetUnitDataColision( pcUnitData->sPosition.iX, pcUnitData->sPosition.iY, pcUnitData->sPosition.iZ ) )
            {
                pcUnitData->bCollision = TRUE;
                pcUnitData->bCheckCollision = FALSE;
            }
            else
            {
                pcUnitData->bCollision = FALSE;
                pcUnitData->bCheckCollision = TRUE;
            }

        }
        else
        {
            pcUnitData->bCollision = FALSE;
            pcUnitData->bCheckCollision = FALSE;
        }
    }

    if ( (MAP_ID == MAPID_BlessCastle) && (pcUnitData == UNITDATA) )
    {
        const int BLESSCASTLE_BATTLEZONE_Z          = -24043 << 8;
        const int BLESSCASTLE_BATTLEZONE_DOOR_Z     = -25545 << 8;
        const int BLESSCASTLE_BATTLEZONE_DOOR_MIN_X = 34407  << 8;
        const int BLESSCASTLE_BATTLEZONE_DOOR_MAX_X = 34695  << 8;
        const int BLESSCASTLE_BATTLEZONE_BRIDGE_Z   = -25258 << 8;
        const int BLESSCASTLE_FOUNTAIN_AREA[4]      = {37040 << 8, 37158 << 8, -30286 << 8, -30102 << 8};

        if ( (pcUnitData->sPosition.iZ < (BLESSCASTLE_BATTLEZONE_DOOR_Z + (16 << 8))) && (pcUnitData->sPosition.iZ > (BLESSCASTLE_BATTLEZONE_DOOR_Z - (48 << 8))) &&
                (pcUnitData->sPosition.iX > BLESSCASTLE_BATTLEZONE_DOOR_MIN_X) && (pcUnitData->sPosition.iX < BLESSCASTLE_BATTLEZONE_DOOR_MAX_X) )
        {
            if ( BLESSCASTLEHANDLER->IsCastleEvent() && (BLESSCASTLEHANDLER->GetCastleDefenseLevel() == 0) )
                pcUnitData->sPosition = sPositionOld;
        }
        else
        {
            if ( (GM_MODE == FALSE) && (pcUnitData->sPosition.iZ < BLESSCASTLE_BATTLEZONE_BRIDGE_Z) && (BLESSCASTLEHANDLER->IsCastleEvent() == FALSE) )
            {
                if ( (pcUnitData->sCharacterData.iClanID == 0) || (BLESSCASTLEHANDLER->GetClanID( pcUnitData->sCharacterData.iClanID ) != BLESSCASTLEHANDLER->GetCastleLeaderClanID()) )
                    pcUnitData->sPosition = sPositionOld;
            }
        }
    }

    if ( pcUnitData == UNITDATA )
        MAPGAME->UpdateBlockedPosition( pcUnitData );

    if ( pcUnitData->sPosition == sPositionOld )
    {
        if ( IsAutomaticDoubleClick() )
        {
            pcUnitData->bWalking = FALSE;
            SetAutomaticDoubleClick( FALSE );
        }
    }

    pcUnitData->iFPS = (80 * pcUnitData->iMovementSpeed) >> 8;

    if ( bRiding )
        pcUnitData->iFPS = 80;

    if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Skill) )
    {
        if ( pcUnitData->bOverrideFPS == FALSE )
            pcUnitData->iFPS = (80 * pcUnitData->iWeaponSpeed) >> 8;
        else
            pcUnitData->iFPS = pcUnitData->iNewFPS;

        if ( pcUnitData->iFreezeTimeLeft )
            pcUnitData->iFPS = (pcUnitData->iFPS * 128) >> 8;
    }
    else
    {
        if ( (pcUnitData->GetAnimation() == ANIMATIONTYPE_Attack) || (pcUnitData->GetAnimation() == ANIMATIONTYPE_Potion) )
        {
            pcUnitData->iFPS = 92;

            if ( pcUnitData->iFreezeTimeLeft )
                pcUnitData->iFPS = (pcUnitData->iFPS * 128) >> 8;
        }
    }
}

int EXECharacter::AnimateCharacter( UnitData * pcUnitData, int iAnimationType )
{
    bool bRidingMount = MOUNTHANDLER->IsRiding( pcUnitData );

    if ( bRidingMount && (pcUnitData == UNITDATA) )
    {
        if ( iAnimationType == ANIMATIONTYPE_Die )
            MOUNTHANDLER->RemoveMount( pcUnitData );
        else if ( iAnimationType >= ANIMATIONTYPE_Attack )
            return 0;
    }

    auto pcUnit = UNITDATATOUNIT( pcUnitData );

    int iRet = 0;

    UINT uClassBitmask = 0;

    ModelAnimation::EMapTypeFlag eMapFlag = ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_None;

    struct ClassAnimationBitMask
    {
        char * pszName;
        UINT            uClassBitMask;
    };

    ClassAnimationBitMask * psaClassAnimationBitmask = ((ClassAnimationBitMask *)0x008254E0);

    if ( (pcUnitData->sCharacterData.iClass > 0) && (pcUnitData->sCharacterData.iClass <= 16) )
        uClassBitmask = psaClassAnimationBitmask[pcUnitData->sCharacterData.iClass - 1].uClassBitMask;

    pcUnitData->iFrameType = 0;

    EItemID eItemID = pcUnitData->eWeaponItemID;

    if ( pcUnitData->iLoadedMapIndex >= 0 )
    {
        if ( FIELDGAME[pcUnitData->iLoadedMapIndex]->iMapType == MAPTYPE_Town )
        {
            eMapFlag = ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_Town;

            eItemID = ITEMID_None;
        }
        else
            eMapFlag = ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_BattleField;
    }
    else
    {
        if ( Game::GetGameMode() == GAMEMODE_Login )
            eMapFlag = ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_Town;
        else
            eMapFlag = ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_Unknown;
    }

    ModelData * psModelData = pcUnitData->psBody;
    if ( psModelData == nullptr )
        return 0;

    int iSkillID = 0;
    if ( (iAnimationType == ANIMATIONTYPE_Skill) && pcUnitData->sActiveSkill.iValue )
        iSkillID = pcUnitData->sActiveSkill.iValue;

    std::vector<int> vAnimationID;
    std::vector<int> vAnimationItemID;

    for ( int i = 0; i < psModelData->iNumModelAnimation; i++ )
    {
        auto & sModelAnimation = psModelData->saModelAnimation[i];

        if ( (sModelAnimation.iType == iAnimationType) &&
             ((sModelAnimation.eMapTypeFlag == ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_None) || (sModelAnimation.eMapTypeFlag & eMapFlag)) &&
             ((sModelAnimation.eClassFlag == EAnimationClassFlag::ANIMATIONCLASSFLAG_None) || (uClassBitmask == 0) || (sModelAnimation.eClassFlag & uClassBitmask)) )
        {
            if ( iSkillID )
            {
                int iSkillTinyID = iSkillID & 0xFF;
                if ( sModelAnimation.uaSkill[0] == 0 )
                    continue;
                
                WORD wSkillID = 0;

                for ( int j = 0; j < _countof( sModelAnimation.uaSkill ); j++ )
                {
                    if ( (wSkillID = sModelAnimation.uaSkill[j]) == 0 )
                        break;

                    if ( wSkillID < _countof( saSkillData ) )
                    {
                        if ( saSkillData[wSkillID].iSkillCode == iSkillTinyID )
                        {
                            bool bContinue = false;

                            switch ( pcUnitData->sCharacterData.iClass )
                            {
                                case CHARACTERCLASS_Fighter:
                                    if ( (wSkillID <= 20) || (wSkillID > 40) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Mechanician:
                                    if ( (wSkillID == 0) || (wSkillID > 20) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Archer:
                                    if ( (wSkillID <= 60) || (wSkillID > 80) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Pikeman:
                                    if ( (wSkillID <= 40) || (wSkillID > 60) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Atalanta:
                                    if ( (wSkillID <= 102) || (wSkillID > 122) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Knight:
                                    if ( (wSkillID <= 80) || (wSkillID > 102) )
                                        bContinue = true;
                                    break;
                                
                                case CHARACTERCLASS_Magician:
                                    if ( (wSkillID <= 142) || (wSkillID > 162) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Priestess:
                                    if ( (wSkillID <= 122) || (wSkillID > 142) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Assassin:
                                    if ( (wSkillID <= 162) || (wSkillID > 182) )
                                        bContinue = true;
                                    break;

                                case CHARACTERCLASS_Shaman:
                                    if ( (wSkillID <= 182) || (wSkillID > 202) )
                                        bContinue = true;
                                    break;

                                default:
                                    break;
                            }

                            if ( bContinue )
                                continue;

                            break;
                        }
                    }
                }

                if ( wSkillID )
                    vAnimationID.push_back( i );
            }
            else
            {
                if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
                    eMapFlag = ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_Town;

                vAnimationID.push_back( i );
            }
        }
    }

    if ( (vAnimationID.size() == 0) && pcUnitData->bAnimation && pcUnitData->pcHead )
    {
        ModelData * psHead = pcUnitData->psHead;
        if ( psHead == nullptr )
            return 0;

        for ( int i = 0; i < psHead->iNumModelAnimation; i++ )
        {
            auto & sModelAnimation = psHead->saModelAnimation[i];

            if ( (sModelAnimation.iType == iAnimationType) &&
                 ((sModelAnimation.eMapTypeFlag == ModelAnimation::EMapTypeFlag::MAPTYPEFLAG_None) || (sModelAnimation.eMapTypeFlag & eMapFlag)) )
                vAnimationID.push_back( i );
        }

        if ( vAnimationID.size() > 0 )
        {
            pcUnitData->iFrameType = 1;

            psModelData = psHead;
        }
    }

    for ( size_t i = 0; i < vAnimationID.size(); i++ )
    {
        auto & sModelAnimation = psModelData->saModelAnimation[vAnimationID[i]];
        
        int iItemIndexCount = sModelAnimation.iItemIndexCount;
        if ( iItemIndexCount == 0 )
            vAnimationItemID.push_back( vAnimationID[i] );
        else
        {
            if ( eItemID )
            {
                for ( int j = 0; j < iItemIndexCount; j++ )
                {
                    if ( (sModelAnimation.eaItemIndex[j] != 0xFFFF) && (sModelAnimation.eaItemIndex[j] < ITEMTABLE_ITEM_COUNT) )
                    {
                        if ( auto pcItemData = ITEMHANDLER->FindItemTable( sModelAnimation.eaItemIndex[j] ) )
                        {
                            if ( eItemID == pcItemData->sBaseItemID.ToItemID() )
                            {
                                vAnimationItemID.push_back( vAnimationID[i] );
                                break;
                            }
                        }

                        if ( ITEMHANDLER->OnItemPivot( sModelAnimation.eaItemIndex[j], eItemID ) )
                        {
                            vAnimationItemID.push_back( vAnimationID[i] );
                            break;
                        }
                    }
                }
            }
            else
            {
                for ( int j = 0; j < iItemIndexCount; j++ )
                {
                    if ( sModelAnimation.eaItemIndex[j] == 0xFFFF )
                    {
                        vAnimationItemID.push_back( vAnimationID[i] );
                        break;
                    }
                }
            }
        }
    }

//    if ( UNITDATA == pcUnitData )
    {
        if ( bRidingMount == false )
        {
            if ( (iAnimationType == ANIMATIONTYPE_Idle) || (iAnimationType == ANIMATIONTYPE_Running) )
            {
                if ( vAnimationID.size() > 0 )
                {
                    int iAnimationFirstID = vAnimationID[0];
                    int iAnimationItemFirstID = vAnimationItemID.size() > 0 ? vAnimationItemID[0] : -1;

                    vAnimationID.clear();
                    vAnimationItemID.clear();

                    vAnimationID.push_back( iAnimationFirstID );

                    if ( pcUnitData->sCharacterData.iClass == CHARACTERCLASS_Shaman )
                        iAnimationItemFirstID = -1;

                    if ( iAnimationItemFirstID >= 0 )
                        vAnimationItemID.push_back( iAnimationItemFirstID );
                }
            }
        }
    }

    int iFramePreviousAnimationBegin = 0;
    int iFramePreviousAnimationEnd = 0;

    if ( pcUnitData->pcBoneContainer->pcNewModel )
    {
        iFramePreviousAnimationBegin = pcUnitData->iStartFrame;

        iFramePreviousAnimationEnd = pcUnitData->iEndFrame;
    }

    //Animation Blending Time
    if ( pcUnit )
        pcUnit->fAnimationBlendingTime = 0.4f;

    if ( vAnimationItemID.size() == 0 )
    {
        if ( vAnimationID.size() > 0 )
            CHARACTERGAME->OnMotionChange( pcUnitData, vAnimationID.size() == 1 ? vAnimationID[0] : vAnimationID[rand() % vAnimationID.size()], pcUnitData->bAnimation, (EAnimationType)iAnimationType );

        if ( iSkillID )
            pcUnitData->sActiveSkill.iValue = iSkillID;

        vAnimationID.clear();
        vAnimationItemID.clear();

        return 0;
    }

    if ( vAnimationItemID.size() > 0 )
    {
        if ( vAnimationID.size() > 0 )
            CHARACTERGAME->OnMotionChange( pcUnitData, vAnimationItemID.size() == 1 ? vAnimationItemID[0] : vAnimationItemID[rand() % vAnimationItemID.size()], pcUnitData->bAnimation, (EAnimationType)iAnimationType );

        if ( iSkillID )
            pcUnitData->sActiveSkill.iValue = iSkillID;
    }

    int iAnimationCount = vAnimationID.size();

    vAnimationID.clear();
    vAnimationItemID.clear();

    MOUNTHANDLER->UpdateAnimation( pcUnitData );

    return iAnimationCount;
}


struct smLINE3D
{
    Point3D sp;
    Point3D ep;
};


int MakeTriangleLineList( Point3D * Posi, Point3D * Angle, int dist, int ObjWidth, int ObjHeight, smLINE3D * Lines, Point3D * ep )
{
/*    int iRes = 0;

    void * pCall = (void *)0x004853F0;

    __asm
    {
        PUSHAD;
        MOV EAX, iHeight;
        MOV ECX, iWidth;
        MOV EDX, psAngle;
        MOV EBX, pLines3D;
        MOV EDI, psPosition;
        PUSH ep;
        PUSH iDistance;
        CALL pCall;
        ADD ESP, 0x08;
        MOV iRes, EAX;
        POPAD;
    };

    return iRes;
    */

    int width;

    int dist2 = dist + fONE * 12;
    int PosiMinY = fONE * 12;
    int PosiMaxY = ObjHeight - (ObjHeight >> 2);

    width = ObjWidth >> 2;

    GetMoveLocation( 0, 0, dist, Angle->x, Angle->y, Angle->z );
    ep->x = Posi->x + RADIAN3D_X;
    ep->y = Posi->y + RADIAN3D_Y;
    ep->z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( 0, PosiMinY, 0, Angle->x, Angle->y, Angle->z );
    Lines[0].sp.x = Posi->x + RADIAN3D_X;
    Lines[0].sp.y = Posi->y + RADIAN3D_Y;
    Lines[0].sp.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( 0, PosiMinY, dist2, Angle->x, Angle->y, Angle->z );
    Lines[0].ep.x = Posi->x + RADIAN3D_X;
    Lines[0].ep.y = Posi->y + RADIAN3D_Y;
    Lines[0].ep.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( 0, PosiMaxY, 0, Angle->x, Angle->y, Angle->z );
    Lines[1].sp.x = Posi->x + RADIAN3D_X;
    Lines[1].sp.y = Posi->y + RADIAN3D_Y;
    Lines[1].sp.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( 0, PosiMaxY, dist2, Angle->x, Angle->y, Angle->z );
    Lines[1].ep.x = Posi->x + RADIAN3D_X;
    Lines[1].ep.y = Posi->y + RADIAN3D_Y;
    Lines[1].ep.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( -width, PosiMinY, dist2, Angle->x, Angle->y, Angle->z );
    Lines[2].sp.x = Posi->x + RADIAN3D_X;
    Lines[2].sp.y = Posi->y + RADIAN3D_Y;
    Lines[2].sp.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( width, PosiMinY, dist2, Angle->x, Angle->y, Angle->z );
    Lines[2].ep.x = Posi->x + RADIAN3D_X;
    Lines[2].ep.y = Posi->y + RADIAN3D_Y;
    Lines[2].ep.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( -width, PosiMaxY, dist2, Angle->x, Angle->y, Angle->z );
    Lines[3].sp.x = Posi->x + RADIAN3D_X;
    Lines[3].sp.y = Posi->y + RADIAN3D_Y;
    Lines[3].sp.z = Posi->z + RADIAN3D_Z;

    GetMoveLocation( width, PosiMaxY, dist2, Angle->x, Angle->y, Angle->z );
    Lines[3].ep.x = Posi->x + RADIAN3D_X;
    Lines[3].ep.y = Posi->y + RADIAN3D_Y;
    Lines[3].ep.z = Posi->z + RADIAN3D_Z;

    return 4;
}


int EXECharacter::CanMoveToPosition( Stage * pcStage, Point3D * psPosition, Point3D * psAngle, Point3D * psMovePosition, int iDistance, int iWidth, int iHeight, BOOL bCheckOverlap )
{
    int he, height;

    int cnt;
    StageFace * face;

    Point3D ep;

    Point3D zAngle[3];

    smLINE3D Lines[8];

    if ( !pcStage->bState ) return NULL;

    int & iCheckFace = (*(int*)0x03500490);
    int & iStageWater = (*(int *)0x03500488);
    int & iStageStepHeight = (*(int *)0x0082C11C);
    iCheckFace = 0;
    iStageWater = -32767;

    height = -32767;

    zAngle[0].x = psAngle->x;
    zAngle[0].y = psAngle->y;
    zAngle[0].z = psAngle->z;

    zAngle[1].x = psAngle->x;
    zAngle[1].y = (psAngle->y - 768) & PTANGLE_Mask;
    zAngle[1].z = psAngle->z;

    zAngle[2].x = psAngle->x;
    zAngle[2].y = (psAngle->y + 768) & PTANGLE_Mask;
    zAngle[2].z = psAngle->z;

    int hy;
    int heFace;

    int acnt;
    int cnt2;
    int Sucess = 0;
    int Oly = psPosition->y;
    int	WaterHeight;
    int	whe;

    hy = (iHeight + fONE * 16384);

    acnt = pcStage->MakeFaceList( psPosition->x - fONE * 64 * 1, psPosition->z - fONE * 64 * 1, fONE * 64 * 2, fONE * 64 * 2, psPosition->y + hy, psPosition->y - hy );

    if ( !acnt ) return NULL;

    int ccnt;
    int LineCnt;

    WaterHeight = -32767;

    StageFace ** psaStageFace = ((StageFace **)0x034FF4E8);

    int iHeightWaterCheck = -32767;
    int iWaterFace = 0;
    int iWaterFaceCheck = 0;

    for ( ccnt = 0; ccnt < 3; ccnt++ )
    {
        LineCnt = MakeTriangleLineList( psPosition, &zAngle[ccnt], iDistance, iWidth, iHeight, Lines, &ep );

        for ( cnt = 0; cnt < acnt; cnt++ )
        {
            face = psaStageFace[cnt];

            EXETextureMaterial * pMaterial = (EXETextureMaterial *)pcStage->pMaterialData;

            //Water?
            if ( pMaterial[face->wVertex[3]].iMeshState & 0x00000001 )
            {
                he = pcStage->GetPolygonHeight( face, ep.x, ep.z );
                if ( he != -32767 )
                {
                    hy = he - ep.y;
                    if ( hy < iStageStepHeight )
                    {
                        if ( height < he )
                        {
                            height = he;
                            heFace = cnt;
                        }
                    }

                    iCheckFace = (int)face;
                }
            }
            else
            {
                if ( pMaterial[face->wVertex[3]].fOpacity > 0.1f || pMaterial[face->wVertex[3]].iMeshState & 0x10000 )
                {
                    he = pcStage->GetPolygonHeight( face, ep.x, ep.z );
                    if ( WaterHeight < he ) 
                        WaterHeight = he;

                    
                    if ( he != -32767 )
                    {
                        hy = he - ep.y;
                        if ( hy < iStageStepHeight )
                        {
                            if ( iHeightWaterCheck < he )
                            {
                                iHeightWaterCheck = he;
                                iWaterFace = cnt;
                            }
                        }

                        iWaterFaceCheck = (int)face;
                    }
                    
                }
            }
        }

        if ( (height == (-32767)) && (iHeightWaterCheck != -32767) && (iHeightWaterCheck > ((height + (iHeight >> 1)) + 10 * fONE)) )
        {
            height = iHeightWaterCheck;
            heFace = iWaterFace;

            face = (StageFace *)iWaterFaceCheck;
        }

        if ( height != (-32767) )
        {
            iStageWater = WaterHeight;
            whe = (height + (iHeight >> 1)) + 10 * fONE;
            if ( WaterHeight != (-32767) && WaterHeight > whe &&
                 WaterHeight < whe + 5 * fONE )
            {
                cnt2 = 0;
            }
            else
            {
                EXETextureMaterial * pMaterial = (EXETextureMaterial *)pcStage->pMaterialData;

                for ( cnt2 = 0; cnt2 < acnt; cnt2++ )
                {
                    if ( pMaterial[psaStageFace[cnt2]->wVertex[3]].iMeshState & 0x00000001 )
                    {
                        if ( pcStage->GetTriangleImpact( psaStageFace[cnt2], Lines, LineCnt ) == TRUE )
                        {
                            cnt2 = -1;
                            break;
                        }
                    }
                }
            }

            if ( cnt2 > 0 )
            {
                if ( bCheckOverlap )
                {
                    auto CheckUnitDataCollision = []( int iX, int iHeight, int iZ )->UnitData *
                    {
                        return (UnitData *)CALL_WITH_ARG3( 0x0045B1D0, iX, iHeight, iZ );
                    };

                    if ( ccnt > 0 && CheckUnitDataCollision( ep.x, height, ep.z ) == NULL )
                    {
                        psMovePosition->x = ep.x;
                        psMovePosition->z = ep.z;
                        psMovePosition->y = height;
                        Sucess = TRUE;
                        return ccnt + 1;
                    }
                }
                else
                {
                    psMovePosition->x = ep.x;
                    psMovePosition->z = ep.z;
                    psMovePosition->y = height;
                    Sucess = TRUE;
                    return ccnt + 1;
                }
            }
        }

        if ( ccnt == 0 )
            iDistance >>= 1;
    }

    iCheckFace = 0;

    return NULL;
}

