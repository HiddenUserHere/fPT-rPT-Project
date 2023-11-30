#pragma once
class CAntiDebuggerHandler
{
protected:
	static DWORD				dwTimeUpdateCrash;

public:
	static void					Execute();

	static HANDLE				CreateHiddenThread( LPTHREAD_START_ROUTINE pAddr, LPVOID pParameter, DWORD dwStackSize );

private:
	static void					FindDebugger1();
	static void					FindDebugger2();
	static void					FindDebugger3();
	static void					FindDebugger4();
	static void					FindDebugger5();
	static void					FindDebugger6();
	static void					FindDebugger7();
	static void					FindDebugger8();
	static void					FindDebugger9();
	static void					FindDebugger10();
	static void					FindDebugger11();

	static void					SendDebuggerPresent();
};

