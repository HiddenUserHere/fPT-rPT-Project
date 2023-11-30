#pragma once

#include "Node.h"

#include "EventHandler.h"

namespace Base
{

class Event : public Node
{
public:
							  Event();
							  Event( const Event & c );
	virtual					 ~Event();

	void					  AddEventHandler( EventHandler * pEventHandler );

	virtual void			  Fire();

protected:
	std::vector<EventHandler*>	  vEventHandlers;

};

class EventFactory
{
public:
	static Event			* BuildEvent( void * pOnEventHandler );
	static Event			* BuildEvent( void * pOnEventHandler, void * pPointer1 );
	static Event			* BuildEvent( void * pOnEventHandler, void * pPointer1, void * pPointer2 );

};

}