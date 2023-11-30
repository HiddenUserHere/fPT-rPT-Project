#include "stdafx.h"
#include "BotServerHandler.h"


CBotServerHandler::CBotServerHandler()
{
	ZeroMemory( &sBotData, sizeof( SBotServer ) * _MAX_BOTSERVER );
}


CBotServerHandler::~CBotServerHandler()
{
}

SBotServer * CBotServerHandler::GetFreeSlot()
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		if ( !sBotData[i].bValid )
		{
			return &sBotData[i];
		}
	}
	return NULL;
}

BOOL CBotServerHandler::Inside( int iX, int iZ )
{
	if ( iX >= _RICARTEN_XMIN && iX <= _RICARTEN_XMAX )
	{
		if ( iZ >= _RICARTEN_ZMIN && iZ <= _RICARTEN_ZMAX )
			return TRUE;
	}
	return FALSE;
}

BOOL CBotServerHandler::CreateBot( char * pszCharName, int iX, int iY, int iZ )
{
	SBotServer * psBotData = GetFreeSlot();
	if ( psBotData )
	{
		char szFileName[128] = { 0 };
		STRINGFORMAT( szFileName, "..\\Login\\Data\\Character\\%s.chr", pszCharName );
		FILE * fp = NULL;
		fopen_s( &fp, szFileName, "rb" );
		if ( fp )	
		{
			fseek( fp, 0x10, SEEK_SET );
			fread( &psBotData->sCharInfo, sizeof( CharacterData ), 1, fp );
			fclose( fp );
			psBotData->iX		= iX;
			psBotData->iY		= iY;
			psBotData->iZ		= iZ;
			psBotData->iPositionItemHandL = -1;
			psBotData->iPositionItemHandR = -1;
			psBotData->iPositionItemHandB = -1;
			DWORD dwID = 0;
			__asm
			{
				PUSH EAX;
				PUSH ECX;
				MOV EAX, 0x0054CEE0;
				CALL EAX;
				MOV dwID, EAX;
				POP ECX;
				POP EAX;
			}
			psBotData->dwObjectID	= dwID;
			psBotData->bValid		= TRUE;
			PacketBotLogin sPacket;
			sPacket.iLength = sizeof( PacketBotLogin );
			sPacket.iHeader = PKTHDR_NetBotAdd;
			sPacket.dwObjectID = dwID;
			sPacket.bAdd = TRUE;
			STRINGCOPY( sPacket.szCharName, psBotData->sCharInfo.szName );
			NETSERVER->SendBotAdd( &sPacket );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CBotServerHandler::DeleteBot( char * pszCharName )
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psBotData->bValid && STRINGCOMPAREI( psBotData->sCharInfo.szName, pszCharName ) )
		{
			PacketBotLogin sPacket;
			sPacket.iLength = sizeof( PacketBotLogin );
			sPacket.iHeader = PKTHDR_NetBotAdd;
			sPacket.dwObjectID = psBotData->dwObjectID;
			sPacket.bAdd = FALSE;
			NETSERVER->SendBotAdd( &sPacket );
			ZeroMemory( psBotData, sizeof( SBotServer ) );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CBotServerHandler::IsOnlineBot( char * pszCharName )
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psBotData->bValid && STRINGCOMPAREI( psBotData->sCharInfo.szName, pszCharName ) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CBotServerHandler::AddGameBot( struct PacketBotLogin * psPacket )
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psPacket->bAdd )
		{
			if ( !psBotData->bValid )
			{
				STRINGCOPY( psBotData->sCharInfo.szName, psPacket->szCharName );
				psBotData->dwObjectID = psPacket->dwObjectID;
				psBotData->bValid = TRUE;
				break;
			}
		}
		else
		{
			if ( psBotData->bValid && psBotData->dwObjectID == psPacket->dwObjectID )
			{
				STRINGCOPY( psBotData->sCharInfo.szName, "" );
				psBotData->dwObjectID = 0;
				psBotData->bValid = FALSE;
				break;
			}
		}
	}
}

void CBotServerHandler::OnGetUnitInfo( User * pcUser, PacketSimple * psPacket )
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psBotData->bValid && psBotData->dwObjectID == psPacket->iUnk2 )
		{
			{
				PacketGetUnitInfo sPacket;
				ZeroMemory( &sPacket, sizeof( PacketGetUnitInfo ) );
				sPacket.iLength = sizeof( PacketGetUnitInfo );
				sPacket.iHeader = PKTHDR_UnitInfoQuick;
				sPacket.dwObjectID = psBotData->dwObjectID;
				sPacket.iX = psBotData->iX;
				sPacket.iY = psBotData->iY;
				sPacket.iZ = psBotData->iZ;
				CopyMemory( &sPacket.sCharInfo, &psBotData->sCharInfo, 352 );
				SENDPACKET( pcUser, &sPacket );
			}
			
		}
	}
}

BOOL CBotServerHandler::SetItemHand( int iHand, char * pszCharName, char * pszItemName )
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psBotData->bValid && STRINGCOMPAREI( psBotData->sCharInfo.szName, pszCharName ) )
		{
			if ( iHand == 1 )
			{
				psBotData->iPositionItemHandL = ITEMSERVER->FindItemPositionTable( pszItemName );
				if ( psBotData->iPositionItemHandL == -1 )
					return FALSE;
			}
			else if ( iHand == 2 )
			{
				psBotData->iPositionItemHandR = ITEMSERVER->FindItemPositionTable( pszItemName );
				if ( psBotData->iPositionItemHandR == -1 )
					return FALSE;
			}
			else if ( iHand == 3 )
			{
				psBotData->iPositionItemHandB = ITEMSERVER->FindItemPositionTable( pszItemName );
				if ( psBotData->iPositionItemHandB == -1 )
					return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CBotServerHandler::SetBotSay( char * pszCharName, char * pszText )
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psBotData->bValid && STRINGCOMPAREI( psBotData->sCharInfo.szName, pszCharName ) )
		{
			STRINGCOPY( psBotData->szTextSay, pszText );
			psBotData->bSay = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

void CBotServerHandler::Tick()
{
	for ( int i = 0; i < _MAX_BOTSERVER; i++ )
	{
		SBotServer * psBotData = sBotData + i;
		if ( psBotData->bValid && psBotData->dwTime < TICKCOUNT )
		{
			psBotData->dwTime = TICKCOUNT + 5000;
			PacketPlayDataGroupBot sPacketGroup;
			sPacketGroup.iLength = sizeof( PacketPlayDataGroupBot );
			sPacketGroup.iHeader = 0x48470014;
			sPacketGroup.iPlayDataCount = 1;
			ZeroMemory( &sPacketGroup.sPlayData, sizeof( PacketPlayData ) );
			sPacketGroup.sPlayData.iLength		= sizeof( PacketPlayData );
			sPacketGroup.sPlayData.iHeader		= PKTHDR_PlayData;
			sPacketGroup.sPlayData.dwObjectID	= psBotData->dwObjectID;
			sPacketGroup.sPlayData.bEventInfo[0] = 100;
			sPacketGroup.sPlayData.sMapID		= 3;
			sPacketGroup.sPlayData.sUnk		= 3;
			sPacketGroup.sPlayData.sHp[0]		= psBotData->sCharInfo.sHP.sMax;
			sPacketGroup.sPlayData.sHp[1]		= psBotData->sCharInfo.sHP.sMax;
			sPacketGroup.sPlayData.iPlayBuffCount = 1;
		
			ZeroMemory( &sPacketGroup.sPlayBuffData.sPlayBuff, sizeof( PlayBufferData ) );
			sPacketGroup.sPlayBuffData.iCount = 1;
			sPacketGroup.sPlayBuffData.sPlayBuff.iX = psBotData->iX;
			sPacketGroup.sPlayBuffData.sPlayBuff.iY = psBotData->iY;
			sPacketGroup.sPlayBuffData.sPlayBuff.iZ = psBotData->iZ;
			sPacketGroup.sPlayBuffData.sPlayBuff.saAngle[3] = 0x40;
			sPacketGroup.sPlayBuffData.sPlayBuff.dwFrame = 0xFFFF0000|80;
			sPacketGroup.sPlayBuffData.sPlayBuff.sStickItems[0] = (short)psBotData->iPositionItemHandB;
			sPacketGroup.sPlayBuffData.sPlayBuff.sStickItems[1] = (short)psBotData->iPositionItemHandL;
			sPacketGroup.sPlayBuffData.sPlayBuff.sStickItems[2] = (short)psBotData->iPositionItemHandR;
			for ( int j = 0; j < PLAYERS_MAX; j++ )
			{
				User * pcUser = USERDATATOUSER( &USERSDATA[j] );
				if ( pcUser && pcUser->pcUserData && pcUser->pcUserData->iID && pcUser->b64 )
				{
					if ( Inside( pcUser->pcUserData->sPosition.iX, pcUser->pcUserData->sPosition.iZ ) )
					{
						if ( psBotData->bSay )
							CHATSERVER->SendUserBoxChat( pcUser->pcUserData, psBotData->dwObjectID, "%s: %s", psBotData->sCharInfo.szName, psBotData->szTextSay );

						SENDPACKET( pcUser, &sPacketGroup );
					}
				}
			}
			psBotData->bSay = FALSE;
		}
	}
}
