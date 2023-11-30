#pragma once

#define GAME_NAME			"rPT"

#define FPS					70
const double MS_PER_FRAME = (1000.0f / ((double)FPS));

#define BMPImage			unsigned int
#define TGAImage			unsigned int

#define UNKNOWN				DWORD

static int PT_PI_FIX_BUG( int iAngleFix )
{
	if ( (iAngleFix > (2048 - 12)) && (iAngleFix <= 2048) )
		iAngleFix -= 12;
	else if ( (iAngleFix < (2048 + 12)) && (iAngleFix >= 2048) )
		iAngleFix += 12;

	return iAngleFix;
};

#define	PI					3.14159265358979323846f
#define	PTAngleToDegrees(a)	((((PT_PI_FIX_BUG(a) & PTANGLE_Mask) * 360) / PTANGLE_360) + 180)
#define	PTAngleToRadians(a)	((((float)PTAngleToDegrees(a) - 180.0f) * PI / 180.0f))

#define RELEASE(p)			{ if(p != NULL) p->Release(); p = NULL;}
#define INIT(p)				{ if(p != NULL) p->Init();}
#define INITP(p, ptr)		{ if(p != NULL && ptr != NULL) p->Init(ptr);}
#define DELET(p)			{ if(p != NULL) delete p; p = NULL;}
#define DELETA(p)			{ if(p != NULL) delete[] p; p = NULL;}
#define SHUTDOWN(p)			{ if(p != NULL) p->Shutdown();}
#define ADDREF(p)			{ if(p != NULL) p->AddRef();}
#define CLEARPTR(p)			{ if(p != NULL) p->Clear();}
#define RESETPTR(p)			{ if(p != NULL) p.reset(); p = NULL;}

#define DSSUCCEEDED(hr)		(hr == DS_OK)


#define IMPFNC				static const DWORD
#define STCFNC				static void

#define NAKED				__declspec(naked)
#define NAKEDVOID			__declspec(naked) void

#define PUSHALL				{ __asm PUSH EAX __asm PUSH ECX __asm PUSH EDX __asm PUSH EBX __asm PUSH EBP __asm PUSH ESI __asm PUSH EDI }
#define POPALL				{ __asm POP EDI __asm POP ESI __asm POP EBP __asm POP EBX __asm POP EDX __asm POP ECX __asm POP EAX }

#define NOP					{ __asm NOP }
#define WRITENOP( addr, count )			{ for( int i = 0; i < count; i++ ) (*(BYTE*)(addr+i)) = 0x90; }
#define CRASH				{ __asm { __asm MOV EAX, DWORD PTR DS:[0x00000000] } }
#define JMP( pointer )		{ __asm { __asm MOV EAX, pointer __asm JMP EAX } }
#define JUMP( pointer )		{ __asm JMP DWORD PTR DS:[pointer] }
#define READTHIS( p )		{ __asm { MOV p, ECX } }

#define WRITEDWORD( a, b )	{ *(DWORD*)a = (DWORD)b; }
#define READDWORD( a )		(*(DWORD*)a)

#define WRITEINT( a, b )	{ *(int*)a = (int)b; }
#define READINT( a )		(*(int*)a)

#define WRITEWORD( a, b )	{ *(WORD*)a = (WORD)b; }
#define READWORD( a )		(*(WORD*)a)

#define WRITEBYTE( a, b )	{ *(BYTE*)a = (BYTE)b; }
#define READBYTE( a )		(*(BYTE*)a)

enum EPTAngle : int
{
	PTANGLE_360		= 4096,
	PTANGLE_1		= (PTANGLE_360 / 360),
	PTANGLE_45		= (PTANGLE_360 / 8),
	PTANGLE_90		= (PTANGLE_360 / 4),
	PTANGLE_180		= (PTANGLE_360 / 2),
	PTANGLE_270		= (PTANGLE_180 + PTANGLE_90),
	PTANGLE_Mask	= (PTANGLE_360 - 1),
};

DWORD CALLT( DWORD ptr1, DWORD t );
DWORD CALLT_WITH_ARG1( DWORD ptr1, DWORD t, DWORD arg1 );
DWORD CALLT_WITH_ARG2( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2 );
DWORD CALLT_WITH_ARG3( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3 );
DWORD CALLT_WITH_ARG4( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4 );
DWORD CALLT_WITH_ARG5( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5 );
DWORD CALLT_WITH_ARG6( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6 );
DWORD CALLT_WITH_ARG7( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7 );
DWORD CALLT_WITH_ARG8( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8 );
DWORD CALLT_WITH_ARG9( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9 );
DWORD CALLT_WITH_ARG10( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9, DWORD arg10 );

DWORD CALL( DWORD ptr1 );
DWORD CALL_WITH_ARG1( DWORD ptr1, DWORD arg1 );
DWORD CALL_WITH_ARG2( DWORD ptr1, DWORD arg1, DWORD arg2 );
DWORD CALL_WITH_ARG3( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3 );
DWORD CALL_WITH_ARG4( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4 );
DWORD CALL_WITH_ARG5( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5 );
DWORD CALL_WITH_ARG6( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6 );
DWORD CALL_WITH_ARG7( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7 );
DWORD CALL_WITH_ARG8( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8 );
DWORD CALL_WITH_ARG9( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9 );
DWORD CALL_WITH_ARG10( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9, DWORD arg10 );