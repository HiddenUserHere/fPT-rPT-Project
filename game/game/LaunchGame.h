#pragma once

typedef std::shared_ptr<class CLaunchGame> Launch_ptr;
typedef class CLaunchGame Launch;

class CLaunchGame
{
private:
	BOOL									DecryptArg( char * Arg );
	BOOL									IsVMWare();
	BOOL									IsVirtualPC();
public:
											CLaunchGame();
	virtual									~CLaunchGame();
	
	void									OpenGameHandler( char * Arg );
};

