#include "stdafx.h"
#include "CTimer.h"

CTimer::CTimer()
{
	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency( &liFrequency );
	dFrequency			= (double)liFrequency.QuadPart;

	bMultiCore			= FALSE;

	liLastTick.QuadPart	= 0;
}

double CTimer::Update()
{
	LARGE_INTEGER liNewTick;
	QueryPerformanceCounter( &liNewTick );

	double dUpdate = (((double)(liNewTick.QuadPart - liLastTick.QuadPart)) * 1000.0f) / dFrequency;
	liLastTick = liNewTick;

	return dUpdate;
}