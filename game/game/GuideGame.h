#pragma once
class CGuideGame
{
public:
	char		cPadding001[0x5C];
	char		szBuffer[10000];

	BOOL guide_handle( int iLevel, ECharacterClass iClass );
	void open_guide( char * h, char * k, char * g );
};

