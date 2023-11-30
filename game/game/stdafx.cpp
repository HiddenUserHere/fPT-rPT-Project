#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

static DWORD dwAllocs = 0;
static DWORD dwPAllocs = 0;

DWORD GetMajorAllocator()
{
	return dwAllocs;
}

DWORD GetMajorAllocatorP()
{
	return dwPAllocs;
}


void * operator new[](std::size_t s) throw(std::bad_alloc)
{	

	typedef void*(__cdecl *tfnNewOperator)(std::size_t s);
	tfnNewOperator fnNewOperator = (tfnNewOperator)0x00766698;

	void * p = fnNewOperator( s );
	return p;
}
void operator delete[](void *p) throw()
{
	typedef void( __cdecl *tfnDeleteOperator )(void * p);
	tfnDeleteOperator fnDeleteOperator = (tfnDeleteOperator)0x00767512;
	fnDeleteOperator( p );
}

void * operator new(std::size_t s) throw(std::bad_alloc)
{
	typedef void*(__cdecl *tfnNewOperator)(std::size_t s);
	tfnNewOperator fnNewOperator = (tfnNewOperator)0x00766698;

	void * p = fnNewOperator( s );
	return p;
}
void operator delete(void *p) throw()
{
	typedef void( __cdecl *tfnDeleteOperator )(void * p);
	tfnDeleteOperator fnDeleteOperator = (tfnDeleteOperator)0x00767512;
	fnDeleteOperator( p );
}
