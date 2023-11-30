#include "stdafx.h"
#include "CDropItemAreaHandler.h"

CDropItemAreaHandler::CDropItemAreaHandler()
{
}

CDropItemAreaHandler::~CDropItemAreaHandler()
{
    for ( auto ps : vItems )
        DELET( ps );

    vItems.clear();
}

void CDropItemAreaHandler::Init()
{
}

void CDropItemAreaHandler::OnDisconnected( User * pcUser )
{
    RemoveItem( pcUser );
}

void CDropItemAreaHandler::RemoveItem( User * pcUser )
{
    for ( auto it = vItems.begin(); it != vItems.end(); )
    {
        auto ps = (*it);
        if ( ps )
        {
            if ( ps->iUserID == pcUser->GetID() )
            {
                DELET( ps );
                it = vItems.erase( it );
            }
            else
                it++;
        }
        else
            it++;
    }
}

BOOL CDropItemAreaHandler::IsExclusiveDrop( int iID, Map::Loot * psLoot, bool bParty )
{
    BOOL bRet = FALSE;

    for ( auto ps : vItems )
    {
        if ( (iID == psLoot->lExclusiveOwnerID) && (ps->iUserID == psLoot->lExclusiveOwnerID) && (ps->bParty == bParty) )
        {
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}

BOOL CDropItemAreaHandler::IsExclusiveDrop( Map::Loot * psLoot )
{
    BOOL bRet = FALSE;

    for ( auto ps : vItems )
    {
        if ( ps->iUserID == psLoot->lExclusiveOwnerID )
        {
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}

void CDropItemAreaHandler::AddItem( User * pcUser, Map::Loot * psLoot, int iRadius, bool bParty )
{
    ItemAreaDropData * ps = new ItemAreaDropData();
    ps->iUserID = pcUser->GetID();
    ps->iItemID = psLoot->sLootItem.sItem.sItemID.ToItemID();
    ps->iChk1 = psLoot->sLootItem.sItem.iChk1;
    ps->iChk2 = psLoot->sLootItem.sItem.iChk2;
    ps->bParty = bParty;
    ps->sPosition = psLoot->sPosition;
    ps->iRadius = iRadius;
    ps->bIsItem = true;
    vItems.push_back( ps );

    if ( pcUser->bParty )
    {
        auto vUsers = PARTYHANDLER->GetPartyMembers( pcUser->psParty, false );
        for ( auto pcUserParty : vUsers )
            SendUserArea( pcUserParty );

        vUsers.clear();
    }
    else
        SendUserArea( pcUser );
}

void CDropItemAreaHandler::AddItem( User * pcUser, Point3D sPosition, int iRadius, bool bParty )
{
    ItemAreaDropData * ps = new ItemAreaDropData();
    ps->iUserID = pcUser->GetID();
    ps->iItemID = ITEMID_None;
    ps->iChk1 = 0;
    ps->iChk2 = 0;
    ps->bParty = bParty;
    ps->sPosition = sPosition;
    ps->iRadius = iRadius;
    ps->bIsItem = false;
    vItems.push_back( ps );
}

void CDropItemAreaHandler::OnLeaveParty( User * pcUser )
{
    RemoveItem( pcUser );
}

void CDropItemAreaHandler::SendUserArea( User * pcUser )
{
    PacketItemAreaDrop sPacket;
    sPacket.iCount = 0;

    for ( auto ps : vItems )
    {
        if ( ps->sPosition.WithinPTDistance( &pcUser->pcUserData->sPosition, ps->iRadius ) )
        {
            if ( pcUser->bParty && ps->bParty )
            {
                if ( User * pcLeader = pcUser->psParty ? pcUser->psParty->pcLeader : nullptr )
                {
                    if ( pcLeader->GetID() == ps->iUserID )
                        CopyMemory( &sPacket.saArea[sPacket.iCount++], ps, sizeof( ItemAreaDropData ) );
                }
            }
            else if ( (pcUser->bParty == FALSE) && (ps->bParty == false) )
            {
                if ( pcUser->GetID() == ps->iUserID )
                    CopyMemory( &sPacket.saArea[sPacket.iCount++], ps, sizeof( ItemAreaDropData ) );
            }
        }

        if ( sPacket.iCount == MAX_DROPITEM_AREA )
            break;
    }

    ProcessPacket( pcUser, &sPacket );
}

void CDropItemAreaHandler::ProcessPacket( User * pcUser, PacketItemAreaDrop * psPacket )
{
    if ( psPacket->iCount > 0 )
    {
        psPacket->iLength = sizeof( Packet ) + sizeof( int ) + (sizeof( ItemAreaDropData ) * psPacket->iCount);
        psPacket->iHeader = PKTHDR_DropItemAreaUpdate;
        SENDPACKET( pcUser, psPacket );
    }
}
