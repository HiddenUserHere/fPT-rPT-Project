#include "stdafx.h"
#include "CIocpBase.h"

namespace IOCP
{

CIocpBase::CIocpBase( SOCKET _s, INT64 _iID, IocpType _eType, unsigned int _iBufferSize ) :
    sSocket( _s ), iID( _iID ), eType( _eType ), iBufferSize( _iBufferSize ), iTransferred( 0 ), iRemainingData( 0 )
{
    if ( (eType == IOCPTYPE_Receive) || (eType == IOCPTYPE_Accept) )
    {
        vData.resize( iBufferSize );
        Reset( true );
    }

    ZeroMemory( this, sizeof( OVERLAPPED ) );
}

CIocpBase::~CIocpBase()
{
}

void CIocpBase::Reset( bool bClear )
{
    if ( bClear )
        std::fill( vData.begin(), vData.end(), 0 );

    sWSABuffer.buf = vData.data();
    sWSABuffer.len = vData.size() * sizeof( char );

    iTransferred = 0;
}

UINT CIocpBase::Add( UINT _iTransferred, bool bCanReset )
{
    iTransferred += _iTransferred;
    sWSABuffer.buf = vData.data() + iTransferred;
    sWSABuffer.len -= _iTransferred;

    if ( bCanReset )
        if ( iTransferred >= iBufferSize )
            Reset( true );

    return iTransferred;
}

void CIocpBase::BackTop( UINT _iTransferred, UINT uTotal )
{
    std::vector<char> vLocal( vData );
    
    Reset( true );
    
    CopyMemory( vData.data(), vLocal.data() + _iTransferred, vData.size() - _iTransferred );

    Add( uTotal - _iTransferred );

    vLocal.clear();
}

};