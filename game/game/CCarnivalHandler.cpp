#include "stdafx.h"
#include "CCarnivalHandler.h"

CCarnivalHandler::CCarnivalHandler()
{
    pcWindow = new CCarnivalWindowHandler();
}

CCarnivalHandler::~CCarnivalHandler()
{
    DELET( pcWindow );
}

void CCarnivalHandler::Init()
{
    pcWindow->Init();
}

bool CCarnivalHandler::HaveItems()
{
    return ((ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterA ) >= 2) && 
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterC ) >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterI ) >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterL ) >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterN ) >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterR ) >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalLetterV ) >= 1));
}

bool CCarnivalHandler::HaveFruits()
{
    return ((ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalApple )      >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalBanana )     >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalJabuticaba ) >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalMaracuja )   >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalPitanga )    >= 1) &&
            (ITEMHANDLER->GetItemAmountByCode( ITEMID_CarnivalStrawberry ) >= 1));
}

void CCarnivalHandler::ExchangeItems()
{
    PacketNPCCarnival sPacket;
    sPacket.iCount = 0;
    sPacket.bDeleteItems = false;

    auto AddItemPacket = [&sPacket]( ItemData * pcItemData )-> void
    {
        sPacket.iaItemID[sPacket.iCount]    = pcItemData->sItem.sItemID.ToItemID();
        sPacket.iaChk1[sPacket.iCount]      = pcItemData->sItem.iChk1;
        sPacket.iaChk2[sPacket.iCount]      = pcItemData->sItem.iChk2;
        sPacket.iCount++;

        //ITEMHANDLER->DeleteItem( pcItemData );
    };

    if ( HaveFruits() )
    {
        if ( auto pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_CarnivalApple ) )
            AddItemPacket( pcItemData );
        if ( auto pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_CarnivalBanana ) )
            AddItemPacket( pcItemData );
        if ( auto pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_CarnivalJabuticaba ) )
            AddItemPacket( pcItemData );
        if ( auto pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_CarnivalMaracuja ) )
            AddItemPacket( pcItemData );
        if ( auto pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_CarnivalPitanga ) )
            AddItemPacket( pcItemData );
        if ( auto pcItemData = ITEMHANDLER->GetItemInventoryByCode( ITEMID_CarnivalStrawberry ) )
            AddItemPacket( pcItemData );

        ProcessPacket( &sPacket );
    }
}

void CCarnivalHandler::HandlePacket( PacketNPCCarnival * psPacket )
{
    if ( psPacket->bDeleteItems == false )
        GetWindow()->Open();
    else
    {
        for ( int i = 0; i < psPacket->iCount; i++ )
        {
            if ( auto pcItemData = CGameInventory::GetInstance()->GetInventoryItemIndexEx( psPacket->iaItemID[i], psPacket->iaChk1[i], psPacket->iaChk2[i] ) )
                if ( pcItemData->bValid )
                    ITEMHANDLER->DeleteItem( pcItemData );
        }

        GetWindow()->Close();
    }

    SAVE;
}

void CCarnivalHandler::Update( float fTime )
{
    GetWindow()->Update( fTime );

    if ( Game::GetGameMode() == GAMEMODE_InGame )
    {
        if ( MAP_ID == MAPID_RicartenTown )
        {
            static const Point3D sSoundPosition( 624 << 8, 205 << 8, -17230 << 8 );

            const int SAMBAGIRL_SOUND_DISTANCE = (250 * 250);

            if ( fTimeSound == 0 )
            {
                int iDistanceA, iDistanceB, iDistanceC, iDistanceD;
                UNITDATA->sPosition.GetPTDistanceXZY( &sSoundPosition, iDistanceA, iDistanceB, iDistanceC, iDistanceD );

                if ( iDistanceA < SAMBAGIRL_SOUND_DISTANCE )
                {
                    int iVolume = ((SAMBAGIRL_SOUND_DISTANCE - iDistanceA) * 80) / SAMBAGIRL_SOUND_DISTANCE;

                    if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects || (SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume == 0) )
                        iVolume = 0;

                    SKILLMANAGERHANDLER->SkillPlaySound( 0x4859, sSoundPosition.iX, sSoundPosition.iY, sSoundPosition.iZ );
                    SOUNDHANDLER->ChangeFrequencySoundSkill( 0x4859, 4410 );
                    SOUNDHANDLER->SetVolumeSkill( 246, iVolume );

                    bActiveSound = false;

                    fTimeSound = 2310.0f;
                }
            }
            else if ( bActiveSound )
            {
                fTimeSound -= fTime;
                int iDistanceA, iDistanceB, iDistanceC, iDistanceD;
                UNITDATA->sPosition.GetPTDistanceXZY( &sSoundPosition, iDistanceA, iDistanceB, iDistanceC, iDistanceD );

                if ( iDistanceA < SAMBAGIRL_SOUND_DISTANCE )
                {
                    int iVolume = ((SAMBAGIRL_SOUND_DISTANCE - iDistanceA) * 80) / SAMBAGIRL_SOUND_DISTANCE;

                    if ( SETTINGSHANDLER->GetModel()->GetSettings().bNoEffects || (SETTINGSHANDLER->GetModel()->GetSettings().iEffectVolume == 0) )
                        iVolume = 0;

                    SOUNDHANDLER->SetVolumeSkill( 246, iVolume );
                }

                static UINT FIREWORK_SAMBAGIRL = 0;
                if ( ((FIREWORK_SAMBAGIRL++) % 2) == 0 )
                {
                    switch ( RandomI( 1, 3 ) )
                    {
                        case 1:
                            SetDynamicLight( sSoundPosition.iX, sSoundPosition.iY + (4 << 8), sSoundPosition.iZ, 255, 50, 50, 0, 75, 10 );
                            break;
                        case 2:
                            SetDynamicLight( sSoundPosition.iX, sSoundPosition.iY + (4 << 8), sSoundPosition.iZ, 50, 255, 50, 0, 75, 10 );
                            break;
                        case 3:
                            SetDynamicLight( sSoundPosition.iX, sSoundPosition.iY + (4 << 8), sSoundPosition.iZ, 50, 50, 255, 0, 75, 10 );
                            break;
                    }
                }

                if ( fTimeSound <= 0 )
                {
                    bActiveSound = false;
                    fTimeSound = 0.0f;
                }
            }
        }
    }
}

void CCarnivalHandler::ProcessPacket( PacketNPCCarnival * psPacket )
{
    psPacket->iLength = sizeof( PacketNPCCarnival );
    psPacket->iHeader = PKTHDR_CarnivalNPC;
    SENDPACKETL( psPacket );
}
