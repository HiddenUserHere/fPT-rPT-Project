#pragma once

class CTimer
{
public:
					  CTimer();
	virtual			 ~CTimer() {}

	double			  Update();

	void			  SetMultiCore( BOOL b ) { bMultiCore = b; }

protected:
	double			  dFrequency;

	BOOL			  bMultiCore;

	LARGE_INTEGER	  liLastTick;

};