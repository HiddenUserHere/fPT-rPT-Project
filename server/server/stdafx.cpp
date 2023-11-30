// stdafx.cpp : source file that includes just the standard includes
// server.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

BOOL bBlockNew = FALSE;


void * operator new[](std::size_t s) throw(std::bad_alloc)
{
	typedef void*(__cdecl *tfnNewOperator)(std::size_t s);
	tfnNewOperator fnNewOperator = (tfnNewOperator)0x005C6F40;
	return fnNewOperator( s );
}
void operator delete[](void *p) throw()
{
	typedef void( __cdecl *tfnDeleteOperator )(void * p);
	tfnDeleteOperator fnDeleteOperator = (tfnDeleteOperator)0x005C7B20;
	fnDeleteOperator( p );
}

void * operator new(std::size_t s) throw(std::bad_alloc)
{
	typedef void*(__cdecl *tfnNewOperator)(std::size_t s);
	tfnNewOperator fnNewOperator = (tfnNewOperator)0x005C6F40;
	return fnNewOperator( s );
}
void operator delete(void *p) throw()
{
	typedef void( __cdecl *tfnDeleteOperator )(void * p);
	tfnDeleteOperator fnDeleteOperator = (tfnDeleteOperator)0x005C7B20;
	fnDeleteOperator( p );
}
