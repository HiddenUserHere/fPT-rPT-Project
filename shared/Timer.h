#pragma once

#include "Node.h"

#include "Event.h"

namespace Base
{

class Timer : public Node
{
public:
				  Timer( double _fTick );
				  Timer( double _fTick, Event * _pEvent );
				  Timer( const Timer & c );
	virtual		 ~Timer();

	BOOL		  Update( double fTime );

	void		  SetEvent( Event * _pEvent ) { pEvent = _pEvent; }

protected:
	BOOL		  Fire();

protected:
	Event		* pEvent;

	double		  fTick;
	double		  fOffs;

	int			  iTicks;
	int			  iMaxTicks;

};

}