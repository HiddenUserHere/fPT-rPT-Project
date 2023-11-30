#include "stdafx.h"
#include "Timer.h"


namespace Base
{

Timer::Timer( double _fTick ) : Node()
{
	pEvent		= NULL;

	fTick		= _fTick;
	fOffs		= 0.0f;

	iTicks		= 0;
	iMaxTicks	= INFINITE;
}

Timer::Timer( double _fTick, Event * _pEvent ) : Node()
{
	pEvent		= _pEvent;

	fTick		= _fTick;
	fOffs		= 0.0f;

	iTicks		= 0;
	iMaxTicks	= INFINITE;
}

Timer::Timer( const Timer & c ) : Node(c)
{
	pEvent		= c.pEvent ? new Event( *c.pEvent ) : NULL;

	fTick		= c.fTick;
	fOffs		= 0.0f;

	iTicks		= 0;
	iMaxTicks	= c.iMaxTicks;
}

Timer::~Timer()
{
	DELET( pEvent );
}

BOOL Timer::Update( double fTime )
{
	if( (iMaxTicks != INFINITE) && (iTicks >= iMaxTicks) )
		return TRUE;

	fOffs += fTime;
	if( fOffs >= fTick )
	{
		do
		{
			if( Fire() )
				return TRUE;

			fOffs -= fTick;
		} while( fOffs >= fTick );
	}

	return FALSE;
}

BOOL Timer::Fire()
{
	if( pEvent )
		pEvent->Fire();

	iTicks++;
	if( (iMaxTicks != INFINITE) && (iTicks >= iMaxTicks) )
		return TRUE;

	return FALSE;
}

}