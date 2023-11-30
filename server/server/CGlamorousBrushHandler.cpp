#include "stdafx.h"
#include "CGlamorousBrushHandler.h"

CGlamorousBrushHandler::CGlamorousBrushHandler()
{
}

CGlamorousBrushHandler::~CGlamorousBrushHandler()
{
}

void CGlamorousBrushHandler::HandlePacket( User * pcUser, PacketGlamorousBrush * psPacket )
{
    psPacket->bResult = FALSE;

    switch ( psPacket->eGlamorousItemID )
    {
        case ITEMID_GlamorousAttireGlaze:
            if ( ITEMSERVER->UseItem( pcUser, psPacket->eGlamorousItemID, psPacket->iChk1, psPacket->iChk2 ) >= 0 )
            {
                if ( (psPacket->cItemData.sItem.sItemID.ToItemType() == ITEMTYPE_Armor) || (psPacket->cItemData.sItem.sItemID.ToItemType() == ITEMTYPE_Robe) )
                {
                    if ( ITEMSERVER->UseItem( pcUser, psPacket->cItemData.sItem.sItemID.ToItemID(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 ) >= 0 )
                    {
                        ClearItemStats( psPacket->cItemData.sItem );
                        psPacket->cItemData.sItem.bCostumeItem = 1;

                        ITEMSERVER->ValidateItem( &psPacket->cItemData.sItem );

                        psPacket->bResult = TRUE;

                        ITEMSERVER->SaveItemData( &psPacket->cItemData.sItem );

                        ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->cItemData.sItem.sItemID.ToItemID(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 );

                        LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_GlamorousAttireGlaze, "AccountName[%s] Character[%s] Item Become a Skin ([%s][0x%08X][%d x %d])",
                                                pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
                                                psPacket->cItemData.sItem.szItemName, psPacket->cItemData.sItem.sItemID.ToInt(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 );
                    }
                }
            }
            break;

        case ITEMID_GlamorousArmamentVarnish:
            if ( ITEMSERVER->UseItem( pcUser, psPacket->eGlamorousItemID, psPacket->iChk1, psPacket->iChk2 ) >= 0 )
            {
                if ( (psPacket->cItemData.sItem.sItemID.ToItemBase() == ITEMBASE_Weapon) || (psPacket->cItemData.sItem.sItemID.ToItemType() == ITEMTYPE_Shield) )
                {
                    if ( ITEMSERVER->UseItem( pcUser, psPacket->cItemData.sItem.sItemID.ToItemID(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 ) >= 0 )
                    {
                        ClearItemStats( psPacket->cItemData.sItem );

                        psPacket->cItemData.sItem.bCostumeItem = 1;

                        ITEMSERVER->ValidateItem( &psPacket->cItemData.sItem );

                        psPacket->bResult = TRUE;

                        ITEMSERVER->SaveItemData( &psPacket->cItemData.sItem );

                        ITEMSERVER->AddItemInventory( pcUser->pcUserData, psPacket->cItemData.sItem.sItemID.ToItemID(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 );

                        LOGSERVER->OnLogItemEx( pcUser, ITEMLOGID_GlamorousArmamentVarnish, "AccountName[%s] Character[%s] Item Become a Skin ([%s][0x%08X][%d x %d])",
                                                pcUser->pcUserData->szAccountName, CHARACTERSERVER->GetCharacterName( pcUser->pcUserData ),
                                                psPacket->cItemData.sItem.szItemName, psPacket->cItemData.sItem.sItemID.ToInt(), psPacket->cItemData.sItem.iChk1, psPacket->cItemData.sItem.iChk2 );
                    }
                    else
                    {
                        //Item Name
                        char szItemName[32] = {0};
                        if ( ITEMSERVER->FindItemName( psPacket->cItemData.sItem.sItemID.ToInt(), szItemName ) )
                        {
                            //Log It...
                            PacketLogCheat sPacket;
                            sPacket.iCheatID = CHEATLOGID_GlamorousItemError;
                            sPacket.LParam = psPacket->cItemData.sItem.sItemID.ToInt();
                            sPacket.SParam = psPacket->cItemData.sItem.iChk1;
                            sPacket.EParam = psPacket->cItemData.sItem.iChk2;
                            STRINGCOPY( sPacket.szBuffer1, szItemName );
                            LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
                        }
                    }
                }
            }
            else
            {
                //Item Name
                char szItemName[32] = {0};
                if ( ITEMSERVER->FindItemName( psPacket->eGlamorousItemID, szItemName ) )
                {
                    //Log It...
                    PacketLogCheat sPacket;
                    sPacket.iCheatID = CHEATLOGID_GlamorousItemError;
                    sPacket.LParam = psPacket->eGlamorousItemID;
                    sPacket.SParam = psPacket->iChk1;
                    sPacket.EParam = psPacket->iChk2;
                    STRINGCOPY( sPacket.szBuffer1, szItemName );
                    LOGSERVER->OnLogCheat( pcUser->pcUserData, &sPacket );
                }
            }
            break;

        default:
            break;
    }

    ProcessPacket( pcUser, psPacket );
}

void CGlamorousBrushHandler::ProcessPacket( User * pcUser, PacketGlamorousBrush * psPacket )
{
    psPacket->iLength = sizeof( PacketGlamorousBrush );
    psPacket->iHeader = PKTHDR_GlamorousBrush;
    SENDPACKET( pcUser, psPacket );
}

void CGlamorousBrushHandler::ClearItemStats( Item & sItem )
{
    Item sItemOld;
    CopyMemory( &sItemOld, &sItem, sizeof( Item ) );

    if ( sItemOld.eCraftType == ITEMCRAFTTYPE_Aging )
    {
        int iLevelAge = Base::Coin::GetLevelItemEachAge( &sItemOld );

        if ( iLevelAge > 0 )
            sItemOld.iLevel -= (sItemOld.sAgeLevel / iLevelAge);
    }

    //Clear Stats
    ZeroMemory( &sItem, sizeof( Item ) );

    sItem.bCostumeItem = true;

    STRINGCOPY( sItem.szItemName, sItemOld.szItemName );

    sItem.sItemID = sItemOld.sItemID;
    sItem.iChk1 = sItemOld.iChk1;
    sItem.iChk2 = sItemOld.iChk2;

    ITEMSERVER->UpgradeNewItem( &sItemOld, &sItem );

    sItem.sIntegrity = sItemOld.sIntegrity;
    sItem.sIntegrity.sCur = sItemOld.sIntegrity.sMax;

    sItem.iWeight = (sItemOld.iWeight * 25) / 100;

    sItem.iSalePrice = sItemOld.iSalePrice >> 1;

    sItem.iLevel = sItemOld.iLevel;
}
