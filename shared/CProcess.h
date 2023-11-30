#pragma once

class CProcess
{
public:
					  CProcess();
					 ~CProcess();

	void			  SetInstanceHandle( HINSTANCE _hInstance );
	HINSTANCE		  GetInstanceHandle() { return hInstance; }

	void			  ForceExit( UINT uExitCode = 0 );

protected:
	HINSTANCE		  hInstance;

};