#include "stdafx.h"
#include "CIocpImpl.h"

namespace IOCP
{

void CIocpImpl::Send( INT64 iID, std::vector<char> & vData )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
	{
		if ( auto pBaseSend = pConnection->CreateSend() )
		{
			pBaseSend->vData.swap( vData );
			pBaseSend->Reset();

			if ( CIocpUtils::PostSendQueue( cIocpData, (*pBaseSend) ) != NO_ERROR )
			{
				pConnection->cSendQueue.RemoveSend( pBaseSend.get() );
				vData.swap( pBaseSend->vData );
			}
		}
	}
}

void CIocpImpl::Send( INT64 iID, char * pBuffer, UINT iLength )
{
	std::vector<char> vData( pBuffer, pBuffer + iLength );
	Send( iID, vData );
}

void CIocpImpl::Shutdown( INT64 iID, int iHow )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
		::shutdown( pConnection->sSocket, iHow );
}

void CIocpImpl::Disconnect( INT64 iID )
{
	if ( auto pConnection = cIocpData.cConnectionManager.GetConnection( iID ) )
	{
		Shutdown( iID, SD_BOTH );

		::InterlockedIncrement( &pConnection->iDisconnecting );

		CIocpUtils::PostDisconnect( cIocpData, (*pConnection) );
	}
}

};