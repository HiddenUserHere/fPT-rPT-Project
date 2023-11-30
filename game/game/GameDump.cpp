#include "stdafx.h"
#include "GameDump.h"


CGameDump::CGameDump()
{
}


CGameDump::~CGameDump()
{
}

void CGameDump::SendErrorReport( EXCEPTION_POINTERS * pep )
{
	char * pszIP = ((char*)0x00CDE160);

	static PacketCrash p;

	p.dwExceptionCode		= pep->ExceptionRecord->ExceptionCode;
	p.dwExceptionFlags		= pep->ExceptionRecord->ExceptionFlags;
	p.pvExceptionAddress	= pep->ExceptionRecord->ExceptionAddress;

	p.EDI					= pep->ContextRecord->Edi;
	p.ESI					= pep->ContextRecord->Esi;
	p.EBX					= pep->ContextRecord->Ebx;
	p.EDX					= pep->ContextRecord->Edx;
	p.ECX					= pep->ContextRecord->Ecx;
	p.EAX					= pep->ContextRecord->Eax;
	p.EBP					= pep->ContextRecord->Ebp;
	p.EIP					= pep->ContextRecord->Eip;

	p.dwVersion				= (*(DWORD*)0x0440DFFC);
	
	CopyMemory( p.szAccountName, ((char*)0x039033E8), _countof( p.szAccountName ) );

	//Ready to send, connect
	bool bFailSend = true;
	if ( pszIP != NULL )
	{
		SOCKET s = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		if ( s != INVALID_SOCKET )
		{
			sockaddr_in sin;
			sin.sin_family		= AF_INET;
			sin.sin_addr.s_addr = inet_addr( pszIP );
			sin.sin_port		= htons( 10009 );

			if ( connect( s, (SOCKADDR *)&sin, sizeof( sin ) ) != SOCKET_ERROR )
			{
				LINGER l;
				l.l_onoff	= TRUE;
				l.l_linger	= 30;
				setsockopt( s, SOL_SOCKET, SO_LINGER, (char*)&l, sizeof( l ) );

				send( s, (char*)&p, p.iLength, 0 );

				bFailSend = false;
			}

			Sleep( 2500 );

			closesocket( s );
		}
	}
}
