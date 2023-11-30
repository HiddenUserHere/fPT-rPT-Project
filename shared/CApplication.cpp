#include "stdafx.h"
#include "CApplication.h"

CApplication::CApplication( CWindow * _pcWindow )
{
	pcWindow = _pcWindow;

	iInitCode = 0;

	pcSystem	= new CSystem();
	pcProcess	= new CProcess();
}

CApplication::~CApplication()
{
	DELET( pcSystem );
	DELET( pcProcess );
	DELET( pcWindow );
}

BOOL CApplication::Init( HINSTANCE hInstance )
{
	pcProcess->SetInstanceHandle( hInstance );

	pcWindow->SetApplication( this );

	iInitCode = pcWindow->Init();

	if( iInitCode == 1 )
		return TRUE;

	return FALSE;
}

BOOL CApplication::Run()
{
	return pcWindow->Run();
}

BOOL CApplication::Shutdown()
{
	return pcWindow->Shutdown();
}