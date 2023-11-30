#pragma once

namespace IOCP
{

class CIocpBase : public OVERLAPPED
{
public:
    enum IocpType : int
    {
        IOCPTYPE_Receive,
        IOCPTYPE_Send,
        IOCPTYPE_Sent,
        IOCPTYPE_Accept,
        IOCPTYPE_Disconnect,
    };

    CIocpBase( SOCKET _s, INT64 _iID, IocpType _eType, unsigned int _iBufferSize );

    ~CIocpBase();

    void        Reset( bool bClear = false );

    UINT        Add( UINT _iTransferred, bool bCanReset = true );
    void        BackTop( UINT _iTransferred, UINT uTotal );

    std::vector<char>      vData;

    WSABUF      sWSABuffer;

    SOCKET      sSocket;

    INT64       iID;

    IocpType    eType;

    UINT        iBufferSize;

    UINT        iTransferred = 0;

    UINT        iRemainingData = 0;

    UINT        iSeqNumber = -1;
};

};