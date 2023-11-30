#pragma once

#define ESkyID unsigned int
#define ESky unsigned int

class Sky
{
private:
	IMPFNC					pfnReset	= 0x005A84B0;
	IMPFNC					pfnRender	= 0x005AA030;
	IMPFNC					pfnDraw		= 0x005AA3A0;
	IMPFNC					pfnSetSky	= 0x005A9BB0;
	IMPFNC					pfnUpdate	= 0x005A9080;

public:
	int						  iUnknown;
	void					* pUnknown1;
	void					* pUnknown2;
	void					* pUnknown3;
	ESkyID					  iSkyID;
	ESky					  iSky;
	ESky					  iPreviousSky;
	int						  iVisibilityP;				//Percentage 0 ~ 100
	int						  iPreviousVisibilityP;		//Percentage 0 ~ 100
	int						  iVisibilityFlatP;			//Percentage 0 ~ 100
	int						  iPreviousVisibilityFlatP;	//Percentage 0 ~ 100
	BOOL					  bUnknown;
	int						  iFramesAlive;

public:
	static Sky				* GetInstance() { return (Sky *)0x03911FB8; }

	void					  Reset();

	void					  Render( int iX, int iY, int iZ, int iAX, int iAY, int iAZ );

	static void				  Draw( int a, int b, int c, int d, int e, int f );
	static void				  SetSky( ESkyID iSkyID );

	void					  Update();

};