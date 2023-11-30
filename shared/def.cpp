#include "stdafx.h"
#include "def.h"

DWORD CALLT( DWORD ptr1, DWORD t )
{
	DWORD retVal;

	__asm
	{
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG1( DWORD ptr1, DWORD t, DWORD arg1 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg1
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG2( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG3( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG4( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG5( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG6( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG7( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG8( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg8;
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG9( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg9;
		PUSH arg8;
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALLT_WITH_ARG10( DWORD ptr1, DWORD t, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9, DWORD arg10 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg10;
		PUSH arg9;
		PUSH arg8;
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV ECX, t;
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL( DWORD ptr1 )
{
	DWORD retVal;

	__asm
	{
		MOV EAX, ptr1;
		CALL EAX;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG1( DWORD ptr1, DWORD arg1 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 4;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG2( DWORD ptr1, DWORD arg1, DWORD arg2 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 8;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG3( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 12;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG4( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 16;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG5( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 20;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG6( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 24;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG7( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 28;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG8( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg8;
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 32;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG9( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg9;
		PUSH arg8;
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 36;
		MOV retVal, EAX;
	};

	return retVal;
}

DWORD CALL_WITH_ARG10( DWORD ptr1, DWORD arg1, DWORD arg2, DWORD arg3, DWORD arg4, DWORD arg5, DWORD arg6, DWORD arg7, DWORD arg8, DWORD arg9, DWORD arg10 )
{
	DWORD retVal;

	__asm
	{
		PUSH arg10;
		PUSH arg9;
		PUSH arg8;
		PUSH arg7;
		PUSH arg6;
		PUSH arg5;
		PUSH arg4;
		PUSH arg3;
		PUSH arg2;
		PUSH arg1;
		MOV EAX, ptr1;
		CALL EAX;
		ADD ESP, 40;
		MOV retVal, EAX;
	};

	return retVal;
}