#pragma once

#include "CMutex.h"

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define ERRORBOX( msg, ... ) Debug::GetInstance()->ErrorBox( msg, ##__VA_ARGS__ )
#define ERRORBOX_INT( i ) { Debug::GetInstance()->ErrorBox( "INT: %d", i ); }
#define WRITEERR( msg, ... ) Debug::GetInstance()->WriteLine( __FILENAME__, __LINE__, msg, ##__VA_ARGS__ )
#define WRITEFST( fn, msg, ... ) WriteFast( fn, msg, ##__VA_ARGS__ )
#define WRITEDMP( fil, dmp, siz ) Debug::GetInstance()->WriteDump( fil, dmp, siz )
#ifdef _DEBUG
#define WRITEDBG( msg, ... ) { Debug::GetInstance()->WriteLine( __FILENAME__, __LINE__, msg, ##__VA_ARGS__ ); }
#else
#define WRITEDBG( msg, ...) {}
#endif

#define TEST( msg, val1, val2 ) { if( val1 != val2 ) { ERRORBOX( "Test Failed for %s!\n\n0x%X != 0x%X", msg, val1, val2 ); ExitProcess( -1 ); } }

class Debug
{
private:
	static bool			  bPrepared;
	CMutex				* pcMutex;

	void				  WriteDebugger( char * msg );
	void				  WriteFile( char * msg );
public:
	static Debug		* GetInstance() { static Debug d; d.Prepare(); return &d; }

	void				  Prepare();
	void				  Clear();

	void				  ErrorBox( const char * msg, ... );

	void				  WriteLine( char * file, int line, char * msg, ... );
	void				  WriteDump( int file, void * dmp, int size );
};

void WriteFast( char * fn, char * msg, ... );