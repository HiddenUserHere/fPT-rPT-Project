#pragma once

#include "CSystem.h"
#include "CProcess.h"
#include "CWindow.h"

class CApplication
{
protected:
	UINT		  iInitCode;

	CSystem		* pcSystem;
	CProcess	* pcProcess;
	CWindow		* pcWindow;

public:
				  CApplication( CWindow * _pcWindow );
				 ~CApplication();

	BOOL		  Init( HINSTANCE hInstance );
	BOOL		  Run();
	BOOL		  Shutdown();

	UINT		  GetInitCode() { return iInitCode; }

	CSystem		* GetSystem() { return pcSystem; }
	CProcess	* GetProcess() { return pcProcess; }
	CWindow		* GetWindow() { return pcWindow; }

};